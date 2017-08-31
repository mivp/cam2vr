#include "CardboardDistorter.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

namespace cam2vr {

CardboardDistorter::CardboardDistorter(): m_meshWidth(20), m_meshHeight(20), m_bufferScale(0.5),
											m_program(0), m_texture(0)
{
	// From: webvr-polyfill
	// Scales the recommended buffer size reported by WebVR, which can improve performance.
  	// UPDATE(2016-05-03): Setting this to 0.5 by default since 1.0 does not
  	// perform well on many mobile devices.

	m_deviceInfo = new DeviceInfo();
	m_mesh = new Mesh();

	setTextureBounds();

	computeMeshVertices(m_meshWidth, m_meshHeight);
	computeMeshIndices(m_meshWidth, m_meshHeight);
}

CardboardDistorter::~CardboardDistorter()
{
	if(m_deviceInfo)
		delete m_deviceInfo;
	if(m_mesh)
		delete m_mesh;
	if(m_texture)
		delete m_texture;
}

void CardboardDistorter::setTextureBounds() {
	float leftBounds[] = {0, 0, 0.5, 1};
	float rightBounds[] = {0.5, 0, 0.5, 1};

	// Left eye
	m_viewportOffsetScale[0] = leftBounds[0]; // X
	m_viewportOffsetScale[1] = leftBounds[1]; // Y
	m_viewportOffsetScale[2] = leftBounds[2]; // Width
	m_viewportOffsetScale[3] = leftBounds[3]; // Height

	// Right eye
	m_viewportOffsetScale[4] = rightBounds[0]; // X
	m_viewportOffsetScale[5] = rightBounds[1]; // Y
	m_viewportOffsetScale[6] = rightBounds[2]; // Width
	m_viewportOffsetScale[7] = rightBounds[3]; // Height
}

float lerp(float a, float b, float t) {
	return a + ((b - a) * t);
}

void CardboardDistorter::computeMeshVertices(int width, int height) {

	m_mesh->vertices.resize(2 * width * height * 5);

	float lensFrustum[4];
	m_deviceInfo->getLeftEyeVisibleTanAngles(lensFrustum);
	
	float noLensFrustum[4];
	m_deviceInfo->getLeftEyeNoLensTanAngles(noLensFrustum);
	
	float viewport[4];
	m_deviceInfo->getLeftEyeVisibleScreenRect(noLensFrustum, viewport);
	//viewport[4]: x, y, width, height

	float vidx = 0;
	float iidx = 0;
	for (int e = 0; e < 2; e++) {
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++, vidx++) {
				float u = 1.0 * i / (width - 1);
				float v = 1.0 * j / (height - 1);

				// Grid points regularly spaced in StreoScreen, and barrel distorted in
				// the mesh.
				float s = u;
				float t = v;
				float x = lerp(lensFrustum[0], lensFrustum[2], u);
				float y = lerp(lensFrustum[3], lensFrustum[1], v);
				float d = sqrt(x * x + y * y);
				float r = m_deviceInfo->distortInverse(d);
				float p = x * r / d;
				float q = y * r / d;
				u = (p - noLensFrustum[0]) / (noLensFrustum[2] - noLensFrustum[0]);
				v = (q - noLensFrustum[3]) / (noLensFrustum[1] - noLensFrustum[3]);

				// Convert u,v to mesh screen coordinates.
				float aspect = m_deviceInfo->getDevice().widthMeters / m_deviceInfo->getDevice().heightMeters;

				// FIXME: The original Unity plugin multiplied U by the aspect ratio
				// and didn't multiply either value by 2, but that seems to get it
				// really close to correct looking for me. I hate this kind of "Don't
				// know why it works" code though, and wold love a more logical
				// explanation of what needs to happen here.
                u = (viewport[0] + u * viewport[2] - 0.5) * 2.0; // * aspect;
				v = (viewport[1] + v * viewport[3] - 0.5) * 2.0;

				m_mesh->vertices[(vidx * 5) + 0] = u; // position.x
				m_mesh->vertices[(vidx * 5) + 1] = v; // position.y
				m_mesh->vertices[(vidx * 5) + 2] = s; // texCoord.x
				m_mesh->vertices[(vidx * 5) + 3] = t; // texCoord.y
				m_mesh->vertices[(vidx * 5) + 4] = e; // texCoord.z (viewport index)
                
                cout << u << " " << v << endl;
			}
		}
		float w = lensFrustum[2] - lensFrustum[0];
		lensFrustum[0] = -(w + lensFrustum[0]);
		lensFrustum[2] = w - lensFrustum[2];
		w = noLensFrustum[2] - noLensFrustum[0];
		noLensFrustum[0] = -(w + noLensFrustum[0]);
		noLensFrustum[2] = w - noLensFrustum[2];
		viewport[0] = 1 - (viewport[0] + viewport[2]);
	}
}

void CardboardDistorter::computeMeshIndices(int width, int height) {

	m_mesh->indices.resize(2 * (width - 1) * (height - 1) * 6);

	float halfwidth = width / 2;
	float halfheight = height / 2;
	float vidx = 0;
	float iidx = 0;
	for (int e = 0; e < 2; e++) {
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++, vidx++) {
				if (i == 0 || j == 0)
				  continue;
				// Build a quad.  Lower right and upper left quadrants have quads with
				// the triangle diagonal flipped to get the vignette to interpolate
				// correctly.
				if ((i <= halfwidth) == (j <= halfheight)) {
					// Quad diagonal lower left to upper right.
					m_mesh->indices[iidx++] = vidx;
					m_mesh->indices[iidx++] = vidx - width - 1;
					m_mesh->indices[iidx++] = vidx - width;
					m_mesh->indices[iidx++] = vidx - width - 1;
					m_mesh->indices[iidx++] = vidx;
					m_mesh->indices[iidx++] = vidx - 1;
				} else {
					// Quad diagonal upper left to lower right.
					m_mesh->indices[iidx++] = vidx - 1;
					m_mesh->indices[iidx++] = vidx - width;
					m_mesh->indices[iidx++] = vidx;
					m_mesh->indices[iidx++] = vidx - width;
					m_mesh->indices[iidx++] = vidx - 1;
					m_mesh->indices[iidx++] = vidx - width - 1;
				}
			}
		}
  	}
}

void CardboardDistorter::setup() {

	if(!m_program) {
		m_program = new GLSLProgram();
		string shaderDir = SHADER_DIR;
		cout << "INFO: Shader directory: " << shaderDir << endl;
		string filename;
        filename = shaderDir; filename.append("cardboard.vert");
        m_program->compileShader(filename.c_str());
        filename = shaderDir; filename.append("cardboard.frag");
        m_program->compileShader(filename.c_str());
        m_program->link();
	}

	if(!m_texture) {
		//load an image for testing
		string img_filename="testdata/sidebyside.jpeg";
		int x,y,n;
		unsigned char* data;
        data = stbi_load(img_filename.c_str(), &x, &y, &n, 0);
        if(!data) {
            cout << "ERROR: Failed to load texture " << img_filename << endl;
            exit(0);
        }

		m_texture = new Texture(0, x, y, GL_RGBA, GL_RGB);
		m_texture->init();
		m_texture->update(data, x, y);

		stbi_image_free(data);
	}
}

void CardboardDistorter::render() {
	setup();
	m_mesh->render(m_program, m_texture, m_viewportOffsetScale);
}

}; // namespace

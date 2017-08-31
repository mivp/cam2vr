#ifndef CARDBOARD_DISTORTER_H
#define CARDBOARD_DISTORTER_H

#include "Program.h"
#include "Mesh.h"
#include "Texture.h"
#include "DeviceInfo.h"

namespace cam2vr {

class CardboardDistorter {
public:
	CardboardDistorter();
	~CardboardDistorter();

	void setTextureBounds();
	
	void render();

private:
	void computeMeshVertices(int width, int height);
	void computeMeshIndices(int width, int height);
	void setup();

private:
	int m_meshWidth, m_meshHeight;
	int m_bufferWidth, m_bufferHeight;
	float m_bufferScale;
	float m_viewportOffsetScale[8];

	DeviceInfo* m_deviceInfo;
	GLSLProgram* m_program;
	Mesh* m_mesh;
	Texture* m_texture;
};

}; // namespace


#endif
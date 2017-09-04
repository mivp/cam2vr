#include "DeviceInfo.h"

#include <algorithm>
#include <math.h>
#include <stdlib.h>

namespace cam2vr {

#ifndef PI
#define PI 3.14159265359
#endif

#define DEG2RAG PI/180
#define RAG2DEG 180/PI
 	

DeviceInfo::DeviceInfo() 
{
	//device
	DefaultAndroid = Device(0.110, 0.062, 0.004);
	DefaultIOS = Device(0.1038, 0.0584, 0.004);
	m_device = DefaultAndroid;

	//viewer
	CardboardV1 = CardboardViewer();
	CardboardV1.id = "CardboardV1";
    CardboardV1.label = "Cardboard I/O 2014";
    CardboardV1.fov = 40;
    CardboardV1.interLensDistance = 0.060;
    CardboardV1.baselineLensDistance = 0.035;
    CardboardV1.screenLensDistance = 0.042;
    CardboardV1.distortionCoefficients[0] = 0.441;
    CardboardV1.distortionCoefficients[1] = 0.156;
    float v1_ic[] = {-0.4410035, 0.42756155, -0.4804439, 0.5460139,
      -0.58821183, 0.5733938, -0.48303202, 0.33299083, -0.17573841,
      0.0651772, -0.01488963, 0.001559834};
    for(int i=0; i < 12; i++)
        CardboardV1.inverseCoefficients[i] = v1_ic[i];

    CardboardV2 = CardboardViewer();
	CardboardV2.id = "CardboardV2";
    CardboardV2.label = "Cardboard I/O 2015";
    CardboardV2.fov = 60;
    CardboardV2.interLensDistance = 0.064;
    CardboardV2.baselineLensDistance = 0.035;
    CardboardV2.screenLensDistance = 0.039;
    CardboardV2.distortionCoefficients[0] = 0.34;
    CardboardV2.distortionCoefficients[1] = 0.55;
    float v2_ic[] = {-0.33836704, -0.18162185, 0.862655, -1.2462051,
      1.0560602, -0.58208317, 0.21609078, -0.05444823, 0.009177956,
      -9.904169E-4, 6.183535E-5, -1.6981803E-6};
    for(int i=0; i < 12; i++)
        CardboardV2.inverseCoefficients[i] = v2_ic[i];
    
    m_viewer = CardboardV2;

    // others

}

DeviceInfo::~DeviceInfo()
{

}

float DeviceInfo::distort(float radius) {
	float r2 = radius * radius;
	float ret = 0;
	for (int i = 0; i < 2; i++) {
		ret = r2 * (ret + m_viewer.distortionCoefficients[i]);
	}
	return (ret + 1) * radius;
}

float DeviceInfo::distortInverse(float radius) {
	// Secant method.
	float r0 = 0;
	float r1 = 1;
	float dr0 = radius - distort(r0);
	while (fabs(r1 - r0) > 0.0001 /** 0.1mm */) {
		float dr1 = radius - distort(r1);
		float r2 = r1 - dr1 * ((r1 - r0) / (dr1 - dr0));
		r0 = r1;
		r1 = r2;
		dr0 = dr1;
	}
	return r1;
}

void DeviceInfo::getLeftEyeVisibleTanAngles(float* result) {
	
	// Tan-angles from the max FOV.
	float fovLeft = tan(-DEG2RAG * m_viewer.fov);
	float fovTop = tan(DEG2RAG * m_viewer.fov);
	float fovRight = tan(DEG2RAG * m_viewer.fov);
	float fovBottom = tan(-DEG2RAG * m_viewer.fov);
	// Viewport size.
	float halfWidth = m_device.widthMeters / 4;
	float halfHeight = m_device.heightMeters / 2;
	// Viewport center, measured from left lens position.
	float verticalLensOffset = (m_viewer.baselineLensDistance - m_device.bevelMeters - halfHeight);
	float centerX = m_viewer.interLensDistance / 2 - halfWidth;
	float centerY = -verticalLensOffset;
	float centerZ = m_viewer.screenLensDistance;
	// Tan-angles of the viewport edges, as seen through the lens.
	float screenLeft = distort((centerX - halfWidth) / centerZ);
	float screenTop = distort((centerY + halfHeight) / centerZ);
	float screenRight = distort((centerX + halfWidth) / centerZ);
	float screenBottom = distort((centerY - halfHeight) / centerZ);
	// Compare the two sets of tan-angles and take the value closer to zero on each side.
	result[0] = std::max(fovLeft, screenLeft);
	result[1] = std::min(fovTop, screenTop);
	result[2] = std::min(fovRight, screenRight);
	result[3] = std::max(fovBottom, screenBottom);
}

void DeviceInfo::getLeftEyeNoLensTanAngles(float* result) {
	// Tan-angles from the max FOV.
	float fovLeft = distortInverse(tan(-DEG2RAG * m_viewer.fov));
	float fovTop = distortInverse(tan(DEG2RAG * m_viewer.fov));
	float fovRight = distortInverse(tan(DEG2RAG * m_viewer.fov));
	float fovBottom = distortInverse(tan(-DEG2RAG * m_viewer.fov));
	// Viewport size.
	float halfWidth = m_device.widthMeters / 4;
	float halfHeight = m_device.heightMeters / 2;
	// Viewport center, measured from left lens position.
	float verticalLensOffset = (m_viewer.baselineLensDistance - m_device.bevelMeters - halfHeight);
	float centerX = m_viewer.interLensDistance / 2 - halfWidth;
	float centerY = -verticalLensOffset;
	float centerZ = m_viewer.screenLensDistance;
	// Tan-angles of the viewport edges, as seen through the lens.
	float screenLeft = (centerX - halfWidth) / centerZ;
	float screenTop = (centerY + halfHeight) / centerZ;
	float screenRight = (centerX + halfWidth) / centerZ;
	float screenBottom = (centerY - halfHeight) / centerZ;
	// Compare the two sets of tan-angles and take the value closer to zero on each side.
	result[0] = std::max(fovLeft, screenLeft);
	result[1] = std::min(fovTop, screenTop);
	result[2] = std::min(fovRight, screenRight);
	result[3] = std::max(fovBottom, screenBottom);
}

// return x, y, width, height
void DeviceInfo::getLeftEyeVisibleScreenRect(const float* undistortedFrustum, float* result) {
	float dist = m_viewer.screenLensDistance;
	float eyeX = (m_device.widthMeters - m_viewer.interLensDistance) / 2;
	float eyeY = m_viewer.baselineLensDistance - m_device.bevelMeters;
	float left = (undistortedFrustum[0] * dist + eyeX) / m_device.widthMeters;
	float top = (undistortedFrustum[1] * dist + eyeY) / m_device.heightMeters;
	float right = (undistortedFrustum[2] * dist + eyeX) / m_device.widthMeters;
	float bottom = (undistortedFrustum[3] * dist + eyeY) / m_device.heightMeters;
	result[0] = left;
	result[1] = bottom;
	result[2] = right - left;
	result[3] = top - bottom;
}

}; //namespace

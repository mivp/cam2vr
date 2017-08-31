#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H

#include  <string>
using namespace std;

namespace cam2vr {



	struct Device {
		float widthMeters;
  		float heightMeters;
  		float bevelMeters;
  		Device() {}
  		Device(float w, float h, float b) {
  			widthMeters = w;
  			heightMeters = h;
  			bevelMeters = b;
  		}
	};

	struct CardboardViewer {
		CardboardViewer() {}
		std::string id;
    	std::string label;
    	float fov;
    	float interLensDistance;
    	float baselineLensDistance;
    	float screenLensDistance;
    	float distortionCoefficients[2];
    	float inverseCoefficients[12];
 	};


	class DeviceInfo {
	public:
		DeviceInfo();
		~DeviceInfo();

		float getWidth() { return m_width; }
		float getHeight() { return m_height; }
		float getWidthMeters() { return m_widthMeters; }
		float getHeightMeters() { return m_heightMeters; }
		Device getDevice() { return m_device; }
		CardboardViewer getViewer() { return m_viewer; }

		float distort(float radius);
		float distortInverse(float radius);

		void getLeftEyeVisibleTanAngles(float* result);
		void getLeftEyeNoLensTanAngles(float* result);
		void getLeftEyeVisibleScreenRect(const float* undistortedFrustum, float* result);

	public:
		CardboardViewer CardboardV1, CardboardV2;
		Device DefaultIOS, DefaultAndroid;

	private:
		float m_width, m_height;
		float m_widthMeters, m_heightMeters, m_bevelMeters;
		Device m_device;
		CardboardViewer m_viewer;  		
	};

}; // namespace


#endif
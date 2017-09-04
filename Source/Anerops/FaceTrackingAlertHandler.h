// "Anerops" is licenced under the GNU GPL 3 licence. Visit <https://www.gnu.org/licenses/> for more information

#pragma once
#include "RealSense/Face/FaceConfiguration.h"

using namespace Intel::RealSense;

/**
 * Class responsable for handling Realsense alerts
 */
class ANEROPS_API FaceTrackingAlertHandler : public Face::FaceConfiguration::AlertHandler
{
public:
	FaceTrackingAlertHandler();
	virtual void PXCAPI OnFiredAlert(const Face::FaceData::AlertData* alertData);

	inline bool shouldMaskBeHidden(){return m_shouldMaskBeHidden;}
	inline bool shouldCaptureDefault(){return m_shouldCaptureDefault;}
	inline void resetShouldCaptureDefault(){m_shouldCaptureDefault = false;}

private:
	bool m_shouldMaskBeHidden;
	bool m_shouldCaptureDefault;
};

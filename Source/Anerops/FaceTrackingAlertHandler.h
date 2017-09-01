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

	~FaceTrackingAlertHandler();
};

// "Anerops" is licenced under the GNU GPL 3 licence. Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "FaceTrackingAlertHandler.h"

FaceTrackingAlertHandler::FaceTrackingAlertHandler()
{
	UE_LOG(GeneralLog, Warning, TEXT("---Created Alert Handler---"));
}

FaceTrackingAlertHandler::~FaceTrackingAlertHandler()
{
}

using namespace Face;

void PXCAPI FaceTrackingAlertHandler::OnFiredAlert(const FaceData::AlertData* alertData)
{
	UE_LOG(GeneralLog, Warning, TEXT("new alert"));

	switch(alertData->label)
	{
	case FaceData::AlertData::ALERT_NEW_FACE_DETECTED:
		UE_LOG(GeneralLog, Warning, TEXT("ALERT_NEW_FACE_DETECTED"));
		break;
	case FaceData::AlertData::ALERT_FACE_OUT_OF_FOV:
		UE_LOG(GeneralLog, Warning, TEXT("ALERT_FACE_OUT_OF_FOV"));
		break;
	case FaceData::AlertData::ALERT_FACE_BACK_TO_FOV:
		UE_LOG(GeneralLog, Warning, TEXT("ALERT_FACE_BACK_TO_FOV"));
		break;
	case FaceData::AlertData::ALERT_FACE_OCCLUDED:
		UE_LOG(GeneralLog, Warning, TEXT("ALERT_FACE_OCCLUDED"));
		break;
	case FaceData::AlertData::ALERT_FACE_NO_LONGER_OCCLUDED:
		UE_LOG(GeneralLog, Warning, TEXT("ALERT_FACE_NO_LONGER_OCCLUDED"));
		break;
	case FaceData::AlertData::ALERT_FACE_LOST:
		UE_LOG(GeneralLog, Warning, TEXT("ALERT_FACE_LOST"));
		break;
	default:
		UE_LOG(GeneralLog, Warning, TEXT("UNKNOWN_ALERT"));
		break;
	}
}

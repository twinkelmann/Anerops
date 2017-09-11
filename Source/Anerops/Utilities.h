// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#pragma once

#include "Anerops.h"
#include "RealSense/Type.h"
#include "RealSense/Face/FaceData.h"
#include "RealSense/SampleReader.h"
#include "RealSenseActor.h"

//basic RealSense namespace
using namespace Intel::RealSense;

/**
 * @brief The Utilities class
 * Static class containing utility methods
 */
class ANEROPS_API Utilities
{
public:
	static FVector RsToUnrealVector(const Point3DF32 &position);
	static FQuat RsToUnrealQuat(const Face::FaceData::PoseQuaternion& rotation);
	//static void UpdateTexture(UTexture2D* tex, Sample* sample);
};

namespace Constantes
{
	const uint32 STREAM_WIDTH = 1920;
	const uint32 STREAM_HEIGHT = 1080;
	const uint32 STREAM_FRAMERATE = 0;
	const uint32 MAX_FACES = 1;
	const uint32 NUM_LANDMARKS = 32;
	const uint32 BYTES_PER_PIXEL = 4;
} //namespace constantes

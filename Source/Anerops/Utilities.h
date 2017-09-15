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
	const uint32 STREAM_WIDTH = /*1920*/960;
	const uint32 STREAM_HEIGHT = /*1080*/540;
	const uint32 STREAM_FRAMERATE = 60;
	const uint32 STREAM_WIDTH_DEPTH = /*1920*/640;
	const uint32 STREAM_HEIGHT_DEPTH = /*1080*/480;
	const uint32 STREAM_FRAMERATE_DEPTH = 60;
	const uint32 MAX_FACES = 1;
	const uint32 NUM_LANDMARKS = 32;
	const uint32 BYTES_PER_PIXEL = 4;
	const uint32 FRAME_TIMOUT = 1000 / STREAM_FRAMERATE * 4 ; //milliseconds
} //namespace constantes

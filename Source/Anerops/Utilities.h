// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#pragma once

#include "Anerops.h"
#include "RealSense/Type.h"
#include "RealSense/Face/FaceData.h"
#include "RealSense/SampleReader.h"

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
	static void UpdateTexture(UTexture2D* tex, Sample* sample);
};

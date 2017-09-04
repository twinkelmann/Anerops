// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "Utilities.h"

/**
 * @brief Utilities::RsToUnrealVector
 * Converts a vector to the UE world from the RealSense world
 * @param position a Point3DF32 in the RealSense world
 * @return an FVector in the UE world
 */
FVector Utilities::RsToUnrealVector(const Point3DF32 &position)
{
	/*
	Gathered data for the SR300:
	TODO: move to documentation
	x - left-right, left positive
	y - up-down, up positive
	z - closer-further, further positive

	screen-chair: z ~= 800

	x in range -240 - +280 (at y = 0)
	y in range -280 - +150 (at x = 0)
	z in range 400 - 1300
	*/
	return FVector(-position.x, -position.z, position.y);
}

/**
 * @brief Utilities::RsToUnrealQuat
 * Converts a Quaternion to the UE world from the RealSense world
 * @param rotation a PoseQuaternion in the RealSense world
 * @return an FQuat in the UE world
 */
FQuat Utilities::RsToUnrealQuat(const Face::FaceData::PoseQuaternion &rotation)
{
	/*
	Gathered data:
	TODO: move to documentation
	pitch - turn around left axis, positif back
	yaw   - turn around up axis, positif right
	roll  - turn around forward axis, positif right
	*/
	return FQuat(rotation.z, rotation.x, -rotation.y, rotation.w);
}

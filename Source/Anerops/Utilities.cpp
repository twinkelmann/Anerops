// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "Utilities.h"

FVector Utilities::RsToUnrealVector(Point3DF32 &position)
{
	/*
	SR300
	x - left-right, left positive
	y - up-down, up positive
	z - closer,further, further positive, screen-chair : 800

	x in range -240 - +280 (at 0 y)
	y in range -280 - +150 (at 0 x)
	z  in range 400 - 1300
	*/
	return FVector(-position.x, -position.z, position.y);
}

FQuat Utilities::RsToUnrealQuat(Face::FaceData::PoseQuaternion &rotation)
{
	/*
	pitch - turn around left axis, positif back
	yaw  - turn around up axis, positif right
	roll   - turn around forward axis, positif right
	*/
	return FQuat(rotation.z, rotation.x, -rotation.y, rotation.w);
}
/*
void AddLocalRotationQuat(const FQuat &DeltaRotation, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport)
{
	AddLocalRotation(DeltaRotation,bSweep,OutSweepHitResult,Teleport);
}
*/

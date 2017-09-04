// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "ActorUtilities.h"

AActorUtilities::AActorUtilities()
{
	//disable this actor's capacity of ticking every frame
	PrimaryActorTick.bCanEverTick = false;

}

/**
 * @brief AActorUtilities::SetActorRotationQuat
 * Set the Actor's rotation instantly to the specified quaternion rotation.
 * @param Target the target Actor to rotate
 * @param NewRotation the new rotation for the Actor
 * @param bTeleportPhysics whether we teleport the physics state (if physics collision is enabled for this object).
 *			If true, physics velocity for this object is unchanged (so ragdoll parts are not affected by change in location).
 *			If false, physics velocity is updated based on the change in position (affecting ragdoll parts).
 * @return whether the rotation was successfully set
 */
bool AActorUtilities::SetActorRotationQuat(AActor* Target, const FQuat &NewRotation, bool bTeleportPhysics)
{
	ETeleportType Teleport = bTeleportPhysics ?
				ETeleportType::TeleportPhysics :
				ETeleportType::None;
	return Target->SetActorRotation(NewRotation, Teleport);
}

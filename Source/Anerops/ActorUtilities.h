// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#pragma once

#include "GameFramework/Actor.h"
#include "ActorUtilities.generated.h"

/**
  Fully static Actor class used to expose custom Blueprint functions
  */
UCLASS()
class ANEROPS_API AActorUtilities : public AActor
{
	GENERATED_BODY()

public:
	AActorUtilities();

	/**
	 * Set the Actor's rotation instantly to the specified quaternion rotation.
	 *
	 * @param	Target The target Actor to rotate
	 * @param	NewRotation	The new rotation for the Actor.
	 * @param	bTeleportPhysics Whether we teleport the physics state (if physics collision is enabled for this object).
	 *			If true, physics velocity for this object is unchanged (so ragdoll parts are not affected by change in location).
	 *			If false, physics velocity is updated based on the change in position (affecting ragdoll parts).
	 * @return	Whether the rotation was successfully set.
	 */
	UFUNCTION(BlueprintCallable,
			  meta=(DisplayName = "SetActorRotationQuat"),
			  Category="Utilities|Transformation|RealSense")
	static bool SetActorRotationQuat(AActor* Target,
									 const FQuat& NewRotation,
									 bool bTeleportPhysics);
};

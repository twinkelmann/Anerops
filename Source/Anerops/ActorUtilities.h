// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#pragma once

#include "GameFramework/Actor.h"
#include "ActorUtilities.generated.h"

/**
 * @brief The AActorUtilities class
 * Fully static Actor class used to expose custom Blueprint functions
 */
UCLASS()
class ANEROPS_API AActorUtilities : public AActor
{
	GENERATED_BODY()

public:
	AActorUtilities();

	UFUNCTION(BlueprintCallable,
			  meta=(DisplayName = "SetActorRotationQuat"),
			  Category="Utilities|Transformation|RealSense")
	static bool SetActorRotationQuat(AActor* Target,
									 const FQuat &NewRotation,
									 bool bTeleportPhysics);
};

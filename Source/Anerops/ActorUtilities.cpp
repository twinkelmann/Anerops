// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "ActorUtilities.h"

AActorUtilities::AActorUtilities()
{
	//disable this actor's capacity of ticking every frame
	PrimaryActorTick.bCanEverTick = false;

}

bool AActorUtilities::SetActorRotationQuat(AActor* Target, const FQuat& NewRotation, bool bTeleportPhysics)
{
	ETeleportType Teleport = ETeleportType::None;
	if(bTeleportPhysics)
	{
		Teleport = ETeleportType::TeleportPhysics;
	}
	return Target->SetActorRotation(NewRotation,Teleport);
}

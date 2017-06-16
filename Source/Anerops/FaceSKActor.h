// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#pragma once

#include "Animation/SkeletalMeshActor.h"
#include "Components/PoseableMeshComponent.h"
#include "FaceSKActor.generated.h"

UCLASS()
class ANEROPS_API AFaceSKActor : public ASkeletalMeshActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFaceSKActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UActorComponent* m_poseableMesh;
	float m_delta;
};

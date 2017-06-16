// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#pragma once

#include "GameFramework/Actor.h"
#include "Components/PoseableMeshComponent.h"
#include "FaceActor.generated.h"

UCLASS()
class ANEROPS_API AFaceActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFaceActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPoseableMeshComponent* m_poseableMesh;
	float m_delta;
};

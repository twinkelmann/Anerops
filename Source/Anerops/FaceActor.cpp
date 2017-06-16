// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "FaceActor.h"

// Sets default values
AFaceActor::AFaceActor()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;
	m_delta = 0;

	//UPoseableMeshComponent* set = AFaceActor::GetComponentByClass(UPoseableMeshComponent);
	//UActorComponent* comp = set.Array()[0];


	/*UE_LOG(GeneralLog,
		   Warning,
		   TEXT("num comp %d"), set->GetName())*/

	/*UE_LOG(GeneralLog,
		   Warning,
		   TEXT("comp 0 name: %s"), *comp->GetName())*/
}

// Called when the game starts or when spawned
void AFaceActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFaceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_delta+=DeltaTime;

	//m_poseableMesh->Activate();
	//int numBones = m_poseableMesh->GetNumBones();
	//TArray<FName> names = m_poseableMesh->GetBoneName(0)

	/*
	for (int i = 0; i < numBones; i++)
	{
		FName const boneName = m_poseableMesh->GetBoneName(i);

		FTransform Transform(SkeletonDataActual.Rotations[i], SkeletonDataActual.Locations[i], FVector(SkeletonDataActual.Scale));

		PoseableMesh->SetBoneTransformByName(BoneName, Transform, EBoneSpaces::WorldSpace);
	}
	*/

	/*
	UE_LOG(GeneralLog,
		   Warning,
		   TEXT("Bones %d"), numBones)
		   */
/*
	UE_LOG(GeneralLog,
		   Warning,
		   TEXT("Bone name %s"), *m_poseableMesh->GetBoneName(0).ToString())*/

}

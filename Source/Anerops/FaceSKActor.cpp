// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "FaceSKActor.h"

// Sets default values
AFaceSKActor::AFaceSKActor()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	//AFaceActor::AddInstanceComponent(new UPoseableMeshComponent);
	//m_poseableMesh = AFaceSKActor::GetSkeletalMeshComponent();
	m_delta = 0;

	//TSet<UActorComponent*> components = AFaceSKActor::GetComponents();

	//USkeletalMeshComponent* comp = AFaceSKActor::GetSkeletalMeshComponent();
	//TArray<USceneComponent*> children;
	//USceneComponent* def = AFaceSKActor::GetDefaultAttachComponent();
	//comp->GetChildrenComponents(true,children);
	//const TSubclassOf<UActorComponent> a;

	//USkeletalMeshComponent* skelcomp = Super::FindComponentByClass<USkeletalMeshComponent>();
	//skelcomp->GetChildrenComponents();

	/*UE_LOG(GeneralLog,
		   Warning,
		   TEXT("num children %d"), children.Num())*/


	/*UE_LOG(GeneralLog,
		   Warning,
		   TEXT("comp name: %s"), *skelcomp->GetName())*/

}

// Called when the game starts or when spawned
void AFaceSKActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFaceSKActor::Tick(float DeltaTime)
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

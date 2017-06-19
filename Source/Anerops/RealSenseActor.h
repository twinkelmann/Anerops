// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#pragma once

#include "GameFramework/Actor.h"

#include "pxcsensemanager.h"
#include "pxcsession.h"
#include "RealSense/Face/FaceConfiguration.h"
#include "RealSense/Face/FaceData.h"
#include "RealSense/Face/FaceModule.h"

#include <vector>
#include "Utilities.h"

#include "RealSenseActor.generated.h"

USTRUCT(BlueprintType)
struct ANEROPS_API FLandmark
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Location"))
	FVector location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="RealSense",  Meta=(DisplayName = "Identifier"))
	int identifier;

	FLandmark()
	{
		location = FVector(0,0,0);
		identifier = 0;
	}
};

using namespace Intel::RealSense;

UCLASS()
class ANEROPS_API ARealSenseActor : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Landmarks"))
	TArray<FLandmark> m_landmarks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Last Landmarks"))
	TArray<FLandmark> m_lastLandmarks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Head Location"))
	FVector m_headLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Head Rotation"))
	FQuat m_headRotation;

	// Sets default values for this actor's properties
	ARealSenseActor();
	~ARealSenseActor();

	// Called every frame
	virtual void Tick(float deltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	//realsense componants
	SenseManager* m_manager;
	Session* m_session;
	NSStatus::Status m_status;

	Face::FaceModule* m_faceAnalyzer;
	Face::FaceData* m_outputData;
	Face::FaceConfiguration* m_config;
	bool m_firstFrame;

	FVector smoothVector(const FVector &current, const FVector &last);

	UFUNCTION(BlueprintCallable, meta=(DisplayName = "GetLandmarkById"), Category="RealSense")
	static FLandmark getLandmarkById(TArray<FLandmark> landmarks, int id);

	UPROPERTY(EditAnywhere)
	float m_lowThreshold;
	UPROPERTY(EditAnywhere)
	float m_highThreshold;

	unsigned int MAX_FACES = 1;
};

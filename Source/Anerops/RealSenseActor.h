// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#pragma once

#include "GameFramework/Actor.h"

#include "pxcsensemanager.h"
#include "pxcsession.h"
#include "RealSense/Face/FaceConfiguration.h"
#include "RealSense/Face/FaceData.h"
#include "RealSense/Face/FaceModule.h"
#include "Realsense/Utility/Smoother.h"
#include "RealSense/SampleReader.h"

#include <list>
#include <vector>
#include "Utilities.h"
#include "FaceTrackingAlertHandler.h"

#include "RealSenseActor.generated.h"


/**
 * @brief The FLandmark struct
 * Contains a FVector position and an int identifier
 * representing a face landmark
 */
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

//base RealSense namespace
using namespace Intel::RealSense;

/**
 * @brief The ARealSenseActor class
 * This class is the middleman between UE4 and the RealSense SDK
 */
UCLASS()
class ANEROPS_API ARealSenseActor : public AActor
{
	GENERATED_BODY()

public:
	//the list of landmarks, updated every frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Landmarks"))
	TArray<FLandmark> m_landmarks;
	//the head location in space, updated every frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Head Location"))
	FVector m_headLocation;
	//the head rotation in space, updated every frame
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Head Rotation"))
	FQuat m_headRotation;
	//boolean saying if the mask should be hidden based on the face visibility
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Should Mask Be Hidden"))
	bool m_shouldMaskBeHidden;
	//boolean saying if a new face has been detected and the defaults should be captured
	//it has to be reset to false by whoever uses it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Should Capture Default"))
	bool m_shouldCaptureDefault;
	//boolean saying if we should draw debug points at each landmarks or not
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RealSense", Meta=(DisplayName = "Show Landmarks"))
	bool m_showLandmarks;

	ARealSenseActor();
	~ARealSenseActor();

	virtual void Tick(float deltaTime) override;

	UFUNCTION(BlueprintCallable, meta=(DisplayName = "GetLandmarkById"), Category="RealSense")
	static FLandmark getLandmarkById(TArray<FLandmark> landmarks, int id);

protected:
	virtual void BeginPlay() override;

private:
	//RealSense componants
	Session*                 m_session;
	SenseManager*            m_manager;
	NSStatus::Status         m_status;
	Face::FaceModule*        m_faceAnalyzer;
	Face::FaceData*          m_outputData;
	Face::FaceConfiguration* m_config;
	SampleReader*			 m_reader;

	//the smoother object for the face position
	Utility::Smoother::Smoother3D* m_headSmoother;
	//the list of smoother objects for each landmarks
	std::vector<Utility::Smoother::Smoother3D*> m_landmarkSmoothers;
	//the custom alert handler
	FaceTrackingAlertHandler m_alertHandler;


	UTexture2D* m_texture;

	//maximum number of faces to track. should stay at 1
	static const int m_maxFaces = 1;
	//number of named landmarks in the realsense SDK
	static const int m_numLandmarks = 32;
	static const int m_streamWidth = 1280;
	static const int m_streamHeight = 720;
	static const int m_streamFps = 0;
};

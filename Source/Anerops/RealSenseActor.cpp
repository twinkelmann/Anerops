// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "RealSenseActor.h"
#include "Utilities.h"

// Sets default values
ARealSenseActor::ARealSenseActor() :
	m_manager(NULL),
	m_session(NULL),
	m_headSmoother(NULL),
	m_status(STATUS_NO_ERROR)
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	m_headLocation = FVector(0,0,0);
	m_headRotation = FQuat(0,0,0,0);
	//m_landmarks.Empty();
}

ARealSenseActor::~ARealSenseActor()
{
	m_config->Release();


	if(m_headSmoother != NULL)
	{
		m_headSmoother->Release();
	}
	/*

	for(auto smoother : m_landmarkSmoothers)
	{
		smoother->Release();
	}
	*/

	//has to be last to get released
	if(m_manager != NULL)
	{
		m_manager->Close();
		m_manager->Release();
	}
}

// Called when the game starts or when spawned
void ARealSenseActor::BeginPlay()
{
	Super::BeginPlay();

	m_manager = SenseManager::CreateInstance();
	if(m_manager == NULL)
	{
		//logging with a custom defined log type
		UE_LOG(GeneralLog,
			   Warning,
			   TEXT("Could not create RealSense Instance. Exiting."));
		//request for a clean exit
		UKismetSystemLibrary::QuitGame(GetWorld(),
									   NULL,
									   EQuitPreference::Type::Quit);
	}

	m_session = m_manager->QuerySession();
	if(m_session == NULL)
	{
		UE_LOG(GeneralLog,
			   Warning,
			   TEXT("Could not retrieve session. Exiting."));
		UKismetSystemLibrary::QuitGame(GetWorld(),
									   NULL,
									   EQuitPreference::Type::Quit);
	}


	Utility::Smoother* smootherFactory = NULL;
	//create smoother for the head position
	m_session->CreateImpl<Utility::Smoother>(&smootherFactory);
	m_headSmoother = smootherFactory->Create3DQuadratic(0.1f);

	if(m_headSmoother == NULL)
	{
		UE_LOG(GeneralLog,
			   Warning,
			   TEXT("Could not create head smoother. Exiting"));
		UKismetSystemLibrary::QuitGame(GetWorld(),
									   NULL,
									   EQuitPreference::Type::Quit);
	}

	//create a smoother for each landmark
	/*
	for(int i = 0; i < m_numLandmarks; i++)
	{
		m_landmarkSmoothers.push_back(smootherFactory->Create3DQuadratic(0.4f));
	}
	*/

	smootherFactory->Release();

	//enable face module for landmark finding
	m_status = m_manager->EnableFace();
	if(m_status < STATUS_NO_ERROR)
	{
		UE_LOG(GeneralLog,
			   Warning,
			   TEXT("Error enabling faces: %d. Exiting."),
			   static_cast<int>(m_status));
		UKismetSystemLibrary::QuitGame(GetWorld(),
									   NULL,
									   EQuitPreference::Type::Quit);
	}

	//object responsable for face analyzing
	m_faceAnalyzer = m_manager->QueryFace();
	if(m_faceAnalyzer == NULL)
	{
		UE_LOG(GeneralLog,
			   Warning,
			   TEXT("Error creating face analyser. Exiting."));
		UKismetSystemLibrary::QuitGame(GetWorld(),
									   NULL,
									   EQuitPreference::Type::Quit);
	}

	//steaming pipeling
	m_status = m_manager->Init();
	if(m_status < STATUS_NO_ERROR)
	{
		UE_LOG(GeneralLog,
			   Warning,
			   TEXT("Error initializing streaming pipeline: %d. Exiting."),
			   static_cast<int>(m_status));
		UKismetSystemLibrary::QuitGame(GetWorld(),
									   NULL,
									   EQuitPreference::Type::Quit);
	}

	m_outputData = m_faceAnalyzer->CreateOutput();

	//configuration of the analyzer
	//TODO: remove useless things : face detection, position ?
	m_config = m_faceAnalyzer->CreateActiveConfiguration();
	m_config->SetTrackingMode(Face::FaceConfiguration::TrackingModeType::
							  FACE_MODE_COLOR_PLUS_DEPTH);

	//face detection
	m_config->detection.isEnabled = true;
	m_config->detection.maxTrackedFaces = m_maxFaces;
	//landmark detection
	m_config->landmarks.isEnabled = true;
	m_config->landmarks.maxTrackedFaces = m_maxFaces;
	//position detection
	m_config->pose.isEnabled = true;
	m_config->pose.maxTrackedFaces = m_maxFaces;
	//events
	//m_config->EnableAllAlerts();

	m_config->ApplyChanges();

}

using namespace Face;

// Called every frame
void ARealSenseActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	//AcquireFrame true
	//true -> wait for all sensors to be ready before getting new frame
	m_status = m_manager->AcquireFrame(true);

	//values greater than 0 means warnings. below 0 are errors
	if(m_status >= STATUS_NO_ERROR)
	{
		m_outputData->Update();
		m_landmarks.Empty();

		// iterate through faces
		const int numOfFaces = m_outputData->QueryNumberOfDetectedFaces();

		if(numOfFaces > 0)
		{
			//we only care about one face, so we take the first one
			FaceData::Face* trackedFace = m_outputData->QueryFaceByIndex(0);
			if(trackedFace != NULL)
			{

				//position data
				FaceData::PoseData* poseData = trackedFace->QueryPose();
				if(poseData != NULL)
				{
					FaceData::PoseQuaternion headRot;
					FaceData::HeadPosition headPose;

					//rotation
					if(poseData->QueryPoseQuaternion(&headRot) != NULL)
					{
						m_headRotation = Utilities::RsToUnrealQuat(headRot);
					}

					//position
					if(poseData->QueryHeadPosition(&headPose))
					{
						Point3DF32 smoothedPoint = m_headSmoother->SmoothValue(
									headPose.headCenter);
						m_headLocation = Utilities::RsToUnrealVector(
									smoothedPoint);
					}
				}

				//landmark data
				FaceData::LandmarksData* landmarkData =
						trackedFace->QueryLandmarks();
				if(landmarkData != NULL)
				{
					const int numPoints = landmarkData->QueryNumPoints();
					//static list that will contain the landmarks
					FaceData::LandmarkPoint* landmarkPoints =
							new FaceData::LandmarkPoint[numPoints];

					if(landmarkData->QueryPoints(landmarkPoints) != NULL)
					{
						//points are valid, use them
						for(int i = 0; i < numPoints; i++)
						{
							//create a new landmark structure
							FLandmark landmark;
							//get the identifier
							landmark.identifier = landmarkPoints[i].source.alias;

							//smooth the position with it's personnal smoother.
							//-1 because ids start at 1
							//Utility::Smoother::Smoother3D* smoother = m_landmarkSmoothers[landmark.identifier - 1];

							Point3DF32 smoothedPoint = /*smoother->SmoothValue(*/landmarkPoints[i].world;
							//convert realsens pos
							FVector pose = Utilities::RsToUnrealVector(smoothedPoint);
							//meters to milimeters
							pose *= 1000.f;

							landmark.location = pose;

							m_landmarks.Add(landmark);

							/*
							//debug point
							DrawDebugPoint(GetWorld(),
										   pose,
										   3.f,
										   FColor(0, 255, 0),
										   false,
										   0.03f);

							DrawDebugString(GetWorld(),
											pose,
											FString::FromInt(
												landmark.identifier),
											0,
											FColor(255,0,0),.001f);
											*/
						}
						delete[] landmarkPoints;
					}
				}
			}
		}
	}
	else
	{
		//in case of error, we simply report it
		UE_LOG(GeneralLog,
			   Warning,
			   TEXT("Error getting frame: %d."),
			   static_cast<int>(m_status));
	}

	//release the frame in any case
	m_manager->ReleaseFrame();
}

FLandmark ARealSenseActor::getLandmarkById(TArray<FLandmark> landmarks, int id)
{
	int index = 0;
	for(int i = 0; i < landmarks.Num(); i++)
	{
		if(landmarks[i].identifier == id)
		{
			index = i;
		}
	}
	return landmarks[index];
}

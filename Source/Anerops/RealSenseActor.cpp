// "Anerops" is licenced under the GNU GPL 3 licence.
// Visit <https://www.gnu.org/licenses/> for more information

#include "Anerops.h"
#include "RealSenseActor.h"
#include "Utilities.h"

//Intel::Realsense::Face namespace
using namespace Face;

/**
 * @brief ARealSenseActor::ARealSenseActor
 * The constructor of the RealSense Actor
 * Sets default values
 */
ARealSenseActor::ARealSenseActor() :
	m_session(NULL),
	m_manager(NULL),
	m_faceAnalyzer(NULL),
	m_outputData(NULL),
	m_config(NULL),
	m_headSmoother(NULL),
	m_reader(NULL),
	m_alertHandler(),
	m_status(STATUS_NO_ERROR),
	m_shouldMaskBeHidden(true),
	m_showLandmarks(false),
	m_headLocation(0, 0, 0),
	m_headRotation(0, 0, 0, 0)
{
	UE_LOG(GeneralLog, Warning, TEXT("--RealSense actor construction---"));

	//Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	UE_LOG(GeneralLog, Warning, TEXT("--Done constructing RealSense actor---"));
}

/**
 * @brief ARealSenseActor::~ARealSenseActor
 * The destructor of the RealSense Actor
 * Releases everything is should
 */
ARealSenseActor::~ARealSenseActor()
{
	UE_LOG(GeneralLog, Warning, TEXT("--RealSense actor destruction---"));

	if(m_config != NULL)
	{
		m_config->Release();
	}

	if(m_outputData != NULL)
	{
		m_outputData->Release();
	}

	if(m_faceAnalyzer != NULL)
	{
		m_faceAnalyzer->Release();
	}

	if(m_reader != NULL)
	{
		m_reader->Release();
	}

	for(int i = 0; i < m_landmarkSmoothers.size(); i++)
	{
		if(m_landmarkSmoothers[i] != NULL)
		{
			m_landmarkSmoothers[i]->Release();
		}
	}

	if(m_headSmoother != NULL)
	{
		m_headSmoother->Release();
	}

	if(m_session != NULL)
	{
		m_session->Release();
	}

	//has to be last to get released
	if(m_manager != NULL)
	{
		//m_manager->Release();
	}

	UE_LOG(GeneralLog, Warning, TEXT("--Done destroying RealSense actor---"));
}

/**
 * @brief ARealSenseActor::BeginPlay
 * Called by UE4 when the Actor is spawned
 * Initializes everything related to RealSense
 */
void ARealSenseActor::BeginPlay()
{
	//begin play for parent class
	Super::BeginPlay();

	UE_LOG(GeneralLog, Warning, TEXT("--BeginPlay for RealSense actor---"));

	m_session = Session::CreateInstance();
	if(m_session == NULL)
	{
		UE_LOG(GeneralLog, Warning,
			   TEXT("Couldn't create Session. Exiting."));
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL,
									   EQuitPreference::Type::Quit);
	}

	m_manager = SenseManager::CreateInstance(m_session);
	if(m_manager == NULL)
	{
		UE_LOG(GeneralLog, Warning,
			   TEXT("Couldn't create Manager. Exiting."));
		//request for a clean exit
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL,
									   EQuitPreference::Type::Quit);
	}

	Utility::Smoother* smootherFactory = NULL;
	//create smoother for the head position
	m_session->CreateImpl<Utility::Smoother>(&smootherFactory);
	m_headSmoother = smootherFactory->Create3DQuadratic(0.1f);

	if(m_headSmoother == NULL)
	{
		UE_LOG(GeneralLog, Warning,
			   TEXT("Couldn't create head smoother. Exiting"));
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL,
									   EQuitPreference::Type::Quit);
	}

	//create a smoothers for each landmark
	for(int i = 0; i < Constantes::NUM_LANDMARKS; i++)
	{
		m_landmarkSmoothers.push_back(smootherFactory->Create3DQuadratic(0.1f));

		if(m_landmarkSmoothers[m_landmarkSmoothers.size() - 1] == NULL)
		{
			UE_LOG(GeneralLog, Warning,
				   TEXT("Couldn't create landmark smoother. Exiting"));
			UKismetSystemLibrary::QuitGame(GetWorld(), NULL,
										   EQuitPreference::Type::Quit);
		}
	}

	//we don't need the factory anymore
	smootherFactory->Release();

	UE_LOG(GeneralLog, Warning, TEXT("--RealSense config---"));

	//enable face module for landmark finding
	m_status = m_manager->EnableFace();

	//status below 0 are errors. above 0 are warnings
	if(m_status != STATUS_NO_ERROR)
	{
		UE_LOG(GeneralLog, Warning,
			   TEXT("Error enabling faces: %d. Exiting"),
			   static_cast<int>(m_status));
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL,
									   EQuitPreference::Type::Quit);
	}

	//object responsable for face analyzing
	m_faceAnalyzer = m_manager->QueryFace();
	if(m_faceAnalyzer == NULL)
	{
		UE_LOG(GeneralLog, Warning,
			   TEXT("Error creating face analyser. Exiting."));
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL,
									   EQuitPreference::Type::Quit);
	}


	//enable video stream
	m_reader = SampleReader::Activate(m_manager);
	if(m_reader == NULL)
	{
		UE_LOG(GeneralLog, Warning,
			   TEXT("Error creating stream reader. Exiting."));
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL,
									   EQuitPreference::Type::Quit);
	}
	m_reader->EnableStream(StreamType::STREAM_TYPE_COLOR,
						   Constantes::STREAM_WIDTH,
						   Constantes::STREAM_HEIGHT,
						   Constantes::STREAM_FRAMERATE);

	//steaming pipeling
	m_status = m_manager->Init();
	if(m_status != STATUS_NO_ERROR)
	{
		UE_LOG(GeneralLog, Warning,
			   TEXT("Error initializing streaming pipeline: %d. Exiting"),
			   static_cast<int>(m_status));
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL,
									   EQuitPreference::Type::Quit);
	}

	m_outputData = m_faceAnalyzer->CreateOutput();

	//configuration of the analyzer
	m_config = m_faceAnalyzer->CreateActiveConfiguration();

	//alerts
	m_config->EnableAllAlerts();
	m_status = m_config->SubscribeAlert(&m_alertHandler);

	if(m_status != STATUS_NO_ERROR)
	{
		UE_LOG(GeneralLog, Warning,
			   TEXT("Error subscribing alert handler: %d. Exiting"),
			   static_cast<int>(m_status));
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL,
									   EQuitPreference::Type::Quit);
	}

	m_config->SetTrackingMode(FaceConfiguration::TrackingModeType::
							  FACE_MODE_COLOR_PLUS_DEPTH);
	//face detection
	m_config->detection.isEnabled = true;
	m_config->detection.maxTrackedFaces = Constantes::MAX_FACES;
	//landmark detection
	m_config->landmarks.isEnabled = true;
	m_config->landmarks.maxTrackedFaces = Constantes::MAX_FACES;
	//position detection
	m_config->pose.isEnabled = true;
	m_config->pose.maxTrackedFaces = Constantes::MAX_FACES;

	m_config->ApplyChanges();


	UE_LOG(GeneralLog, Warning, TEXT("--Config done for RealSense actor---"));
}

/**
 * @brief ARealSenseActor::Tick
 * Routine called every frame by UE4
 * Requests a new frame from the camera and processes the data
 * @param deltaTime how much time passed since the last frame
 */
void ARealSenseActor::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	//true -> wait for all sensors to be ready before getting new frame
	m_status = m_manager->AcquireFrame(true);

	if(m_status == STATUS_NO_ERROR)
	{
		//video stream

		if(m_stream != NULL)
		{
			m_stream->updateImage(m_reader->GetSample()->color);
		}
		else
		{
			UE_LOG(GeneralLog, Warning, TEXT("--Background actor is NULL---"));
		}

		//landmark

		m_status = m_outputData->Update();

		if(m_status != STATUS_NO_ERROR)
		{
			UE_LOG(GeneralLog, Warning,
				   TEXT("Error updating output data: %d. Continuing"),
				   static_cast<int>(m_status));
		}

		m_landmarks.Empty();

		if(m_outputData->QueryNumberOfDetectedFaces() > 0)
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
				else
				{
					UE_LOG(GeneralLog, Warning, TEXT("poseData is NULL"));
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
							//we ignore landmarks of alias 0 (we can't know
							//what they are)
							//we also ignore landmarks with a depth of 0. it
							//means it is lost
							if(landmarkPoints[i].source.alias != 0 &&
							   landmarkPoints[i].source.alias != 30 &&
							   landmarkPoints[i].source.alias != 31 &&
							   landmarkPoints[i].world.z != 0)
							{
								//create a new landmark structure
								//TODO: remove structure. store location
								//directly at the identifier's index
								FLandmark landmark;

								landmark.identifier = landmarkPoints[i].source.alias;

								//smooth the position with it's personnal smoother.
								//landmarks go from 1 to 32,
								//indicies go from 0 to 31, so "id - 1"
								Point3DF32 location = m_landmarkSmoothers
										[landmark.identifier - 1]->SmoothValue(
											landmarkPoints[i].world);

								//convert realsens pos
								//meters to milimeters
								landmark.location = Utilities::RsToUnrealVector(
											location) * 1000.f;

								m_landmarks.Add(landmark);


								if(m_showLandmarks)
								{
									DrawDebugPoint(GetWorld(),
												   landmark.location,
												   3.f,
												   FColor(255, 0, 0),
												   false,
												   0.03f);
								}

								/*
								/debug text
								DrawDebugString(GetWorld(),
												pose,
												FString::FromInt(
													landmark.identifier),
												0,
												FColor(255,0,0),.001f);
												*/
							}
						}
						delete[] landmarkPoints;
					}
				}
				else
				{
					UE_LOG(GeneralLog, Warning, TEXT("landmarkData is NULL"));
				}
			}
			else
			{
				UE_LOG(GeneralLog, Warning, TEXT("trackedFace is NULL"));
			}
		}
	}
	else
	{
		//in case of error, we simply report it
		UE_LOG(GeneralLog, Warning, TEXT("Error getting frame: %d. Continuing"),
			   static_cast<int>(m_status));
	}

	m_shouldMaskBeHidden = m_alertHandler.shouldMaskBeHidden();
	m_shouldCaptureDefault = m_alertHandler.shouldCaptureDefault();
	m_alertHandler.resetShouldCaptureDefault();

	//release the frame in any case
	m_manager->ReleaseFrame();
}

/**
 * @brief ARealSenseActor::getLandmarkById
 * Get a specific landmark  in an array given it's identifier
 * This is a static blueprint method
 * @param landmarks a TArray of FLandmark to seach in
 * @param id the identifier of the landmark we are looking for
 * @return the landmark in the array with the id, or the first one
 */
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

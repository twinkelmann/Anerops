// Fill out your copyright notice in the Description page of Project Settings.

#include "Anerops.h"
#include "RealSenseActor.h"
#include "Utilities.h"


// Sets default values
ARealSenseActor::ARealSenseActor() :
	m_manager(NULL),
	m_session(NULL),
	m_status(STATUS_NO_ERROR)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ARealSenseActor::~ARealSenseActor()
{
	if (m_manager != NULL)
	{
		m_manager->Release();
	}
}

// Called when the game starts or when spawned
void ARealSenseActor::BeginPlay()
{
	Super::BeginPlay();

	m_manager = SenseManager::CreateInstance();
	if (m_manager == NULL)
	{
		//logging with a custom defined log type
		UE_LOG(GeneralLog, Warning, TEXT("Coulnd not create RealSense Instance. Exiting."));
		//request for a clean exit
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Type::Quit);
	}

	m_session = m_manager->QuerySession();
	if (m_session == NULL)
	{
		UE_LOG(GeneralLog, Warning, TEXT("Coulnd not retrieve session. Exiting."));
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Type::Quit);
	}

	//enable face module for landmark finding
	m_status = m_manager->EnableFace();
	if (m_status < STATUS_NO_ERROR)
	{
		UE_LOG(GeneralLog, Warning, TEXT("Error enabling faces: %d. Exiting."), static_cast<int>(m_status));
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Type::Quit);
	}

	//object responsable for face analyzing
	m_faceAnalyzer = m_manager->QueryFace();
	if (m_faceAnalyzer == NULL)
	{
		UE_LOG(GeneralLog, Warning, TEXT("Error creating face analyser. Exiting."));
		UKismetSystemLibrary::QuitGame(GetWorld(), NULL, EQuitPreference::Type::Quit);
	}

	//steaming pipeling
	m_status = m_manager->Init();
	if (m_status < STATUS_NO_ERROR)
	{
		UE_LOG(GeneralLog, Warning, TEXT("Error initializing streaming pipeline: %d. Exiting."), static_cast<int>(m_status));
		UKismetSystemLibrary::QuitGame(GetWorld(),NULL,EQuitPreference::Type::Quit);
	}

	m_outputData = m_faceAnalyzer->CreateOutput();

	//configuration of the analyzer
	//TODO: remove useless things : face detection, position ?
	m_config = m_faceAnalyzer->CreateActiveConfiguration();
	m_config->SetTrackingMode(Face::FaceConfiguration::TrackingModeType::FACE_MODE_COLOR_PLUS_DEPTH);
	//face detection
	m_config->detection.isEnabled = true;
	m_config->detection.maxTrackedFaces = MAX_FACES;
	//landmark detection
	m_config->landmarks.isEnabled = true;
	m_config->landmarks.maxTrackedFaces = MAX_FACES;
	//position detection
	m_config->pose.isEnabled = true;
	m_config->pose.maxTrackedFaces = MAX_FACES;
	//events
	m_config->EnableAllAlerts();

	m_config->ApplyChanges();

}

// Called every frame
void ARealSenseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//AcquireFrame true -> wait for all cameras to be ready before getting new frame
	m_status = m_manager->AcquireFrame(true);
	//values greater than 0 means warnings. below 0 are errors
	if(m_status >= STATUS_NO_ERROR)
	{
		m_outputData->Update();

		// iterate through faces
		int32_t numOfFaces = m_outputData->QueryNumberOfDetectedFaces();

		for (int32_t i = 0; i < numOfFaces; i++)
		{
			//for each face by index
			Face::FaceData::Face *trackedFace = m_outputData->QueryFaceByIndex(i);
			if (trackedFace != NULL)
			{
				//landmark data
				Face::FaceData::LandmarksData* landmarkData = trackedFace->QueryLandmarks();
				if (landmarkData != NULL)
				{
					pxcI32 numPoints = landmarkData->QueryNumPoints();
					//static list that will contain the points
					Face::FaceData::LandmarkPoint* landmarkPoints = new Face::FaceData::LandmarkPoint[numPoints];

					if (landmarkData->QueryPoints(landmarkPoints) != NULL)
					{
						//points are valid, use them
						for (int y = 0; y < numPoints; y++)
						{
							//convert realsens pos
							FVector pose = Utilities::RsToUnrealVector(landmarkPoints[y].world);
							//meters to milimeters
							pose *= 1000.f;

							//draw a debug points at each face landmark
							DrawDebugPoint(GetWorld(), pose, 3.f, FColor(0, 255, 0), false, 0.03f);
						}
						delete landmarkPoints;
					}
				}

				//position data
				Face::FaceData::PoseData* poseData = trackedFace->QueryPose();
				if (poseData != NULL)
				{
					Face::FaceData::PoseQuaternion headRot;
					Face::FaceData::HeadPosition headPose;

					//rotation
					if (poseData->QueryPoseQuaternion(&headRot) != NULL)
					{
						/*
						pitch - turn around left axis, positif back
						yaw  - turn around up axis, positif right
						roll   - turn around forward axis, positif right
						*/
						FQuat newQuat(headRot.z, headRot.x, -headRot.y, headRot.w);
						SetActorRotation(newQuat);
					}

					//position
					if (poseData->QueryHeadPosition(&headPose))
					{
						/*
						SR300
						x - left-right, left positive
						y - up-down, up positive
						z - closer,further, further positive, mid point (chair): 800

						x in range -240 - +280 (at 0 y)
						y in range -280 - +150 (at 0 x)
						z  in range 400 - 1300
						*/

						FVector newPos = Utilities::RsToUnrealVector(headPose.headCenter);
						SetActorLocation(newPos);
					}
				}
			}
		}
	}
	else
	{
		//in case of error
		UE_LOG(GeneralLog, Warning, TEXT("Error getting frame: %d."), static_cast<int>(m_status));
	}

	m_manager->ReleaseFrame();
}

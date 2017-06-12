// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"

#include <pxcsensemanager.h>
#include <pxcsession.h>
#include <RealSense/Face/FaceConfiguration.h>
#include <RealSense/Face/FaceData.h>
#include <RealSense/Face/FaceModule.h>

//#include <vector>

#include "RealSenseActor.generated.h"

using namespace Intel::RealSense;

UCLASS()
class ANEROPS_API ARealSenseActor : public AActor
{
	GENERATED_BODY()

public:
	//FVector m_headLocation;
	//FQuat m_headRotation;
	//std::vector<FVector> m_landmarks;

	// Sets default values for this actor's properties
	ARealSenseActor();
	~ARealSenseActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	UPROPERTY(EditAnywhere)
	unsigned int MAX_FACES = 1;
};

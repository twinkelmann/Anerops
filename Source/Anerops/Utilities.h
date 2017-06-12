// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Anerops.h"
#include "RealSense/Type.h"
#include "RealSense/Face/FaceData.h"

using namespace Intel::RealSense;

/**
 * Static class containing utility functions
 */
class ANEROPS_API Utilities
{
public:
	/** returns a vector in the unreal world space from a 3D point in the realsense world space*/
	static FVector RsToUnrealVector(Point3DF32 &position);
	/** returns a quaternion in the unreal world space from a pose quaternion in the realsense world space*/
	static FQuat RsToUnrealQuat(Face::FaceData::PoseQuaternion &rotation);
};

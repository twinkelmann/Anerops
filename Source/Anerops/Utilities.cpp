// Fill out your copyright notice in the Description page of Project Settings.

#include "Utilities.h"

FVector Utilities::RsToUnrealVector(Point3DF32 &position)
{
	return FVector(-position.x, -position.z, position.y);
}

FQuat Utilities::RsToUnrealQuat(Face::FaceData::PoseQuaternion &rotation)
{
	return FQuat(rotation.z, rotation.x, -rotation.y, rotation.w);
}

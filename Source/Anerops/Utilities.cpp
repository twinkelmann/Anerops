// Fill out your copyright notice in the Description page of Project Settings.

#include "Anerops.h"
#include "Utilities.h"

FVector Utilities::RsToUnrealVector(Point3DF32 &position)
{
	return FVector(-position.x, -position.z, position.y);
}

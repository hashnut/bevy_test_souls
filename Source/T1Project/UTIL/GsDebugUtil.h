// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsDebugUtil.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsDebugUtil : public UObject
{
	GENERATED_BODY()
	
public:	
	static void DrawDebugSolidTriangle(UWorld* in_world
		, const FVector& in_v1, const FVector& in_v2, const FVector& in_v3
		, const FColor& in_color = FColor::Red
		, bool bPersistent = true, float LifeTime = -1.f
		, uint8 DepthPriority = 0);
	static void DrawDebugSolidCylinder(UWorld* in_world
		, const FVector& in_location
		, float in_radius
		, float in_height = 100.0f
		, int32 in_section = 8		
		, const FColor& in_color = FColor::Red
		, bool in_persistent = true, float in_lifeTime = -1.0f
		, uint8 in_depthPriority = 0);
	static void DrawDebugSolidFence(UWorld* in_world
		, const TArray<FVector>& in_pointArray
		, float in_height
		, bool in_loop
		, const FColor& in_color = FColor::Red
		, bool in_persistent = true, float in_lifeTime = -1.0f
		, uint8 in_depthPriority = 0);

	static void DrawDebugFan(UWorld* in_world, 
		const FVector& in_pos, 
		const FVector& in_dir, 
		float in_height, 
		float in_angle, 
		float in_min,
		float in_max,
		const FColor& in_color, 
		float in_lifeTime);
};

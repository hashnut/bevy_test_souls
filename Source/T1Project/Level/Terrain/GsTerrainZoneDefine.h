// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "GsTerrainZoneDefine.generated.h"

//unsigned int32 4,294,967,29


//Debug
const float DEBUG_TERRAIN_SHAPE_HEIGHT = 100.0f;
const FName LANDSCAPE_COLLISION_PROFILE = FName(TEXT("BlockAll"));
const FVector DEBUG_TERRAIN_DOWN_DIRECTION = FVector(0, 0, -10000.0f);
const FVector DEBUG_TERRAIN_FLOOR_FLOAT = FVector(0, 0, 10.0f);
const FVector DEBUG_TERRAIN_SPAWN_SCALE = FVector(50.0f, 50.0f, 50.0f);

//
UENUM(BlueprintType)
enum class ETerrainObjType : uint8
{
	Zone,
	Gather,
	Npc,
	Max
};

UENUM(BlueprintType)
enum class ETerrainSpawnType : uint8
{	
	Gather,
	Npc,
	Max
};

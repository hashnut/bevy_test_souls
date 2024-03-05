// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "GsTerritoryZoneDefine.generated.h"

//unsigned int32 4,294,967,29


//Debug
const float DEBUG_TERRITORY_SHAPE_HEIGHT = 100.0f;
const FName TERRITORY_LANDSCAPE_COLLISION_PROFILE = FName(TEXT("BlockAll"));
const FVector DEBUG_TERRITORY_DOWN_DIRECTION = FVector(0, 0, -10000.0f);
const FVector DEBUG_TERRITORY_FLOOR_FLOAT = FVector(0, 0, 10.0f);
const FVector DEBUG_TERRITORY_SPAWN_SCALE = FVector(50.0f, 50.0f, 50.0f);

//
UENUM(BlueprintType)
enum class EGsTerritoryObjType : uint8
{
	Zone = 0,	
	Npc = 1,
	Spot = 2,
	Warp = 3,
	Fence = 4,
	Territory = 5,//set fence's property
	Prop = 6,
	Line = 7,
	Max
};

UENUM(BlueprintType)
enum class EGsTerritorySpawnType : uint8
{		
	Npc,
	Prop,
	Max
};

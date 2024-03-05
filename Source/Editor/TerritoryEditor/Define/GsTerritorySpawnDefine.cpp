// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritorySpawnDefine.h"
#include "T1Project/UTIL/GsLevelUtil.h"
#include "Runtime/Engine/Classes/Engine/Engine.h"

//Territory
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneCircleShape.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"

FString FGsTerritoryCompareCaution::ToString()
{
	FString log;

	if (Source)
	{
		log += (TEXT("Source - ") + Source->ToString());
	}
	if (Dest)
	{
		log += (TEXT("\tDest - ") + Dest->ToString() + TEXT("\n"));
	}

	return log;
}
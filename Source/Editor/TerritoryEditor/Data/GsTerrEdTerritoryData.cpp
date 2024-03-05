// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrEdTerritoryData.h"
#include "Actor/Warp/GsTerritoryWarp.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "TerritoryEditor.h"

//territory editor
#include "DataSchema/Map/GsSchemaMapWarpSpot.h"
#include "Map/GsSchemaMapTerritoryInfo.h"
#include "UTIL/GsTableUtil.h"
#include "Map/GsSchemaMapTerritoryData.h"

FGsTerrEdTerritoryData::FGsTerrEdTerritoryData()
{
}

FGsTerrEdTerritoryData::~FGsTerrEdTerritoryData()
{
}

bool FGsTerrEdTerritoryData::IsNeedToSave(ULevel* inLevel, const FString& inLevelKey)
{
	return false;
}


void FGsTerrEdTerritoryData::SaveTable(ULevel* inLevel, const FString& inLevelKey)
{
}
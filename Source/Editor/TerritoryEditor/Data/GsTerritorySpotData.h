// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GsTerritoryDataManager.h"

//t1 project
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "DataSchema/Map/Spot/GsSchemaSpotData.h"

/**
 * 
 */
class TERRITORYEDITOR_API FGsTerritorySpotData : public FGsTerritoryDataBase
{
private:
	FString _spotTableName;

	UGsTable* _spotTable;

public:
	FGsTerritorySpotData();
	virtual ~FGsTerritorySpotData();

public:
	virtual bool Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inLevel, const FString& inLevelKey, bool inSaveForce) override;
	void SaveSpotData(const FString& inLevelKey
		, const TArray<FGsSchemaSpotInfo>& inSpawnSpotList
		, const TArray<FGsSchemaSpotInfo>& inResurrectionSpotList
		, const TArray<FGsSchemaSpotInfo>& inWarpSpotList	
		, const TArray<FGsSchemaSpotInfo>& inTeleportSpotList		
		, const TArray<FGsSchemaSpotInfo>& inSpotList);
	bool IsNeedToSaveSpotData(const FString& inLevelKey, const TArray<FGsSchemaSpotInfo>& inSoptList);
	virtual void Export(const FString& inLevelKey) override;

	//Map data
private:
	void SaveMapTerritoryData(const FString& inLevelKey
	, const TArray<FGsSchemaSpotInfo>& inSpawnSpotList
	, const TArray<FGsSchemaSpotInfo>& inResurrectionSpotList
	, const TArray<FGsSchemaSpotInfo>& inWarpSpotList
	, const TArray<FGsSchemaSpotInfo>& inTeleportSpotList);
};

static bool operator != (const FGsSchemaMapSpotInfo& inSource, const FGsSchemaMapSpotInfo& inDest)
{
	return ((inSource.mapId != inDest.mapId)
		|| (inSource.spotIndex != inDest.spotIndex));
}

static bool operator != (const FGsSchemaSpotInfo& inSource, const FGsSchemaSpotInfo& inDest)
{
	return ((inSource.pos != inDest.pos)
		|| (inSource.dir != inDest.dir)
		|| (inSource.radius != inDest.radius)
		|| (inSource.id != inDest.id)		
		|| (false == inSource.name.EqualTo(inDest.name)));
}



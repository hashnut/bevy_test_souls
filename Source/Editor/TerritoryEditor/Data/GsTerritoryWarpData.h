// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GsTerritoryDataManager.h"

//t1 project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "UTIL/GsDir.h"

/**
 * 
 */
class TERRITORYEDITOR_API FGsTerritoryWarpData : public FGsTerritoryDataBase
{
public:
	FGsTerritoryWarpData();
	virtual ~FGsTerritoryWarpData();

protected:
	virtual bool Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce) override;
	bool IsNeedToSaveWarpData(const FString& inLevelKey, const TArray<FGsSchemaMapWarpInfo>& inWarpList);
	void SaveWarpData(const FString& inLevelKey, TArray<FGsSchemaMapWarpInfo>& inWarpList);
};

static bool operator != (const FGsSchemaMapWarpInfo& inSource, const FGsSchemaMapWarpInfo& inDest)
{
	return ((inSource.warpId != inDest.warpId)
		|| (inSource.shapeId != inDest.shapeId)
		|| (inSource.sourceSpot.MapId != inDest.sourceSpot.MapId)
		|| (inSource.sourceSpot.SpotIndex != inDest.sourceSpot.SpotIndex)
		|| (inSource.destSpot.MapId != inDest.destSpot.MapId)
		|| (inSource.destSpot.SpotIndex != inDest.destSpot.SpotIndex)
		|| (inSource.WarpType != inDest.WarpType)
		|| (inSource.groupId != inDest.groupId));
}
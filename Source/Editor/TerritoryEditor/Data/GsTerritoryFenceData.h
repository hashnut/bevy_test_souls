// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GsTerritoryDataManager.h"

struct FGsSchemaFenceElement;

//t1 project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "UTIL/GsDir.h"
#include "Map/Fence/GsSchemaFenceElement.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "Map/Town/GsSchemaTownData.h"
#include "DataSchema/Map/GsSchemaMapTerritoryEnv.h"
#include "DataSchema/Map/GsSchemaMapTerritorySpaceCrack.h"
#include "DataSchema/Map/GsSchemaMapTerritorySound.h"
#include "DataSchema/Map/GsSchemaMapTerritoryInfo.h"

/**
 * 
 */
class TERRITORYEDITOR_API FGsTerritoryFenceData : public FGsTerritoryDataBase
{
public:
	FGsTerritoryFenceData();
	virtual ~FGsTerritoryFenceData();

private:
	virtual bool Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce = false) override;
	bool IsNeedToSaveFenceData(const FString& inLevelKey, const TArray<FGsSchemaFenceElement>& inFenceList);
	bool IsNeedToSaveFenceData(const FString& inLevelKey
		, const TArray<FGsSchemaMapTerritoryInfo>& inServerTerritoryList
		, const TArray<FGsSchemaMapTerritoryInfo>& inNoCampZoneList
		, const TArray<FGsSchemaMapTerritoryEnv>& inEnvTerritoryList
		, const TArray<FGsSchemaMapTerritorySound>& inSoundTerritoryList
		, const TArray<FGsSchemaMapTerritorySpaceCrack>& spaceCrackTerritoryList
		, const TArray<FGsSchemaFenceElement>& inFenceList);
	bool IsNeedToSaveMapTerritoryInternal(const FString& inLevelKey
		, const TArray<FGsSchemaMapTerritoryInfo>& inServerTerritoryList
		, const TArray<FGsSchemaMapTerritoryInfo>& inNoCampZoneList
		, const TArray<FGsSchemaMapTerritoryEnv>& inEnvTerritoryList
		, const TArray<FGsSchemaMapTerritorySound>& inSoundTerritoryList
		, const TArray<FGsSchemaMapTerritorySpaceCrack>& spaceCrackTerritoryList);
	bool IsNeedToSaveFenceInternal(const FString& inLevelKey, const TArray<FGsSchemaFenceElement>& inFenceList);
	void SaveFenceData(const FString& inLevelKey, TArray<FGsSchemaFenceElement>& inFenceList);
	void SaveFenceData(const FString& inLevelKey
		, const TArray<FGsSchemaMapTerritoryInfo>& inServerTerritoryList
		, const TArray<FGsSchemaMapTerritoryInfo>& inNoCampZoneList
		, const TArray<FGsSchemaMapTerritoryEnv>& inEnvTerritoryList
		, const TArray<FGsSchemaMapTerritorySound>& inSoundTerritoryList
		, const TArray<FGsSchemaMapTerritorySpaceCrack>& spaceCrackTerritoryList
		, const TArray<FGsSchemaFenceElement>& inFenceList);
	void SaveMapTerritoryInternal(const FString& inLevelKey
		, const TArray<FGsSchemaMapTerritoryInfo>& inServerTerritoryList
		, const TArray<FGsSchemaMapTerritoryInfo>& inNoCampZoneList
		, const TArray<FGsSchemaMapTerritoryEnv>& inEnvTerritoryList
		, const TArray<FGsSchemaMapTerritorySound>& inSoundTerritoryList
		, const TArray<FGsSchemaMapTerritorySpaceCrack>& inSpaceCrackTerritoryList);
	void SaveFenceInternal(const FString& inLevelKey, const TArray<FGsSchemaFenceElement>& inFenceList);

	virtual void Export(const FString& inLevelKey) override;

private:
	bool IsNeedToSaveTownData(const TArray<const AGsTerritorySpawnActor*>& inClientSpawnList, const TArray<const AGsTerritorySpawnActor*>& inServerSpawnList, const FString& inLevelKey);
	void SaveZonesInTown(const FString& inLevelKey, const TArray<const AGsTerritorySpawnActor*>& inClientSpawnList, const TArray<const AGsTerritorySpawnActor*>& inServerSpawnList);
	bool TryGetZoneListInTerritory(const FGsSchemaFenceElement* inTerritory, const TArray<AGsTerritoryZone*>& inZoneList, OUT TArray<const AGsTerritoryZone*>& outZoneList);
	bool TryGetNpcInTown(OUT TArray<const AGsTerritorySpawnActor*>& outClientSpawnActorList, OUT TArray<const AGsTerritorySpawnActor*>& outServerSpawnActorList, TArray<class AGsTerritoryFenceBase*>& inTerritoryArray, const TArray<AGsTerritoryZone*>& inZoneList);
};

static bool operator != (const FGsSchemaFenceElement& inSource, const FGsSchemaFenceElement& inDest)
{
	bool isEqual = ((inSource.Center != inDest.Center)
		|| (inSource.Radius != inDest.Radius)
		|| (inSource.FenceId != inDest.FenceId)
		|| (inSource.vertexList.Num() != inDest.vertexList.Num()));

	if (isEqual)
	{
		return true;
	}

	int32 num = inSource.vertexList.Num();

	for (int i = 0; i < num; ++i)
	{
		if (inSource.vertexList[i] != inDest.vertexList[i])
		{
			return true;
		}
	}

	return false;
}

static bool operator != (const FGsSchemaTownData& inSource, const AGsTerritorySpawnActor& inDest)
{
	if (inSource.id != inDest.GetTerritoryUniqueID())
	{
		return true;
	}

	if (inSource.CreatureId.RowName.ToString() != FString::FromInt(inDest._spawnInfo.TargetID))
	{
		return true;
	}

	if (inSource.Pos != inDest.GetActorLocation())
	{
		return true;
	}

	if (inSource.Dir != FGsDir::RotatorToAngle(inDest.GetActorRotation()))
	{
		return true;
	}

	return false;
}

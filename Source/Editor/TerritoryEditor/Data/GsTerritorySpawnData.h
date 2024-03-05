// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//t1 project
#include "DataSchema/Map/Spawn/GsSchemaTerritoryNpc.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "DataSchema/Map/Spawn/GsSchemaTerritoryZoneShape.h"

#include "Data/GsTerritoryDataManager.h"
#include "Map/Spawn/GsSchemaClientSpawnData.h"
#include "Map/Spawn/GsSchemaSpawnElement.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "UTIL/GsDir.h"
#include "DataSchema/Map/Spawn/GsSchemaEventSpawnData.h"
#include "SharedInclude/SharedTypes.h"

/**
 * 
 */
class TERRITORYEDITOR_API FGsTerritorySpawnData : public FGsTerritoryDataBase
{
private:
	TArray<int64> _spacePortalList;	

public:
	FGsTerritorySpawnData();
	virtual ~FGsTerritorySpawnData();

public:
	virtual bool Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inLevel, const FString& inLevelKey, bool inSaveForce=false) override;

protected:		
	virtual void Export(const FString& inLevelKey) override;	

private:
	bool IsNeedToSaveSpawnData(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray);
	void SaveSpawnData(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray);
	void SaveClientSpawnData(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray);
	void SaveEventSpawnData(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray);
	void SaveSpawnVisibleData(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray);	

private:
	void CheckMapObstactle(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray);
	bool IsObstacleMap(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray);

private:
	void ParseSpawnDataToClientSpawnData(const FGsSchemaSpawnData& inSource, FGsSchemaClientSpawnData& inDest);	
};

//이미 pointer type이 기본으로 != operator 타입에 정의 되었기 때문에 비교를 하려면 참조 형태로 비교하도록 해야한다.
static bool operator != (const FGsSchemaSpawnElement& inSource, const AGsTerritorySpawnActor& inDest)
{
	if (inSource.SpawnId != inDest.GetTerritoryUniqueID()
		|| inSource.memberType != inDest.GetSpawnGroupMemberType()
		|| inSource.SpawnType != inDest.GetCreatureSpawnType()
		|| inSource.Pos != inDest.GetActorLocation()
		|| inSource.Dir != FGsDir::RotatorToAngle(inDest.GetActorRotation()))
	{
		return true;
	}

	const FGsTerritorySpawnInfo& destSpawnInfo = inDest._spawnInfo;
	if(inSource.CreatureId != destSpawnInfo.TargetID
		|| inSource.spawnLevel != destSpawnInfo.SpawnLevel
		|| inSource.SpawnCount != destSpawnInfo.MaxSpawnNum
		|| inSource.RandomPos != destSpawnInfo.CanRandomLocationSpawn
		|| inSource.RespawnInterval != FTimespan::FromSeconds(destSpawnInfo.RespawnInterval).GetTotalMilliseconds()		
		|| inSource.groupLimitRange != destSpawnInfo.groupLimitRange
		|| inSource.isFixedLocation != destSpawnInfo.isFixedLocation
		|| inSource.skipTerrainCheck != destSpawnInfo.SkipTerrainCheck
		|| inSource.spawnRate != destSpawnInfo.SpawnRate
		|| inSource.InitVisible != destSpawnInfo.InitVisible
		|| inSource.VisibleType != destSpawnInfo.VisibleType
		|| inSource.AiOptimizeOnOff != destSpawnInfo.AiOptimizeOnOff)
	{
		return true;
	}

	int32 sourceConditionNum = inSource.QuestCondition.Num();
	if (sourceConditionNum != destSpawnInfo.QuestCondition.Num())
	{
		return true;
	}

	for (int32 i = 0; i < sourceConditionNum; ++i)
	{
		const FGsSchemaSpawnObjectShowHide& souceSpawnObjectShowHide = inSource.QuestCondition[i];
		const FGsTerrEdSpawnObjectShowHide& destSpawnObjectShowHide = inDest._spawnInfo.QuestCondition[i];
		const FGsSchemaQuestRow& sourceStartQuest = souceSpawnObjectShowHide.StartConditionQuestId;
		const FGsSchemaQuestRow& sourceEndQuest = souceSpawnObjectShowHide.EndConditionQuestId;
		const FGsTerrEdSpawnObjectShowHide& destQuest = inDest._spawnInfo.QuestCondition[i];

		const QuestId sourceStartId = (sourceStartQuest.GetRow() ? sourceStartQuest.GetRow()->id : 0);
		const QuestId sourceEndId = (sourceEndQuest.GetRow() ? sourceEndQuest.GetRow()->id : 0);
		const QuestId destStartId = FCString::Strtoi(*destQuest.StartConditionQuestId, NULL, 10); ;
		const QuestId destEndId = FCString::Strtoi(*destQuest.EndConditionQuestId, NULL, 10);

		if (sourceStartId != destStartId
			|| sourceEndId != destEndId
			|| souceSpawnObjectShowHide.QuestCheckType != destSpawnObjectShowHide.QuestCheckType)
		{
			return true;
		}
	}

	return false;
}

static bool operator != (const FGsSchemaSpawnData& inZoneData, const AGsTerritoryZone& inZone)
{
	//Diff zone shape
	if (inZoneData.Region.Center != inZone.GetActorLocation()
		|| inZoneData.SpawnGroupType != inZone._spawnGroupType
		|| inZoneData.Region.Radius != inZone._radius
		|| inZoneData.SpawnGroupId != inZone.GetTerritoryUniqueID()
		|| inZoneData.byClient != inZone._byClient
		|| inZoneData.byDefault != inZone._byDefault
		|| inZoneData.groupLineMove != inZone._isGroupLineMove		
		|| inZoneData.lineId != inZone._lineId.LineID)
	{
		return true;
	}

	if (inZoneData.SpawnInfoList.Num() != inZone._spawnActors.Num())
	{
		return true;
	}

	//Check npc data
	for (int i = 0; i < inZoneData.SpawnInfoList.Num(); ++i)
	{
		if (inZone._spawnActors[i])
		{
			if (inZoneData.SpawnInfoList[i] != (*inZone._spawnActors[i]))
			{
				return true;
			}
		}		
	}

	return false;
}

static bool operator != (const FGsSchemaSpawnObjectShowHide& inSource, const FGsSchemaSpawnObjectShowHide& inDest)
{
	if (inSource.StartConditionQuestId != inDest.StartConditionQuestId
		|| inSource.EndConditionQuestId != inDest.EndConditionQuestId
		|| inSource.QuestCheckType != inDest.QuestCheckType)
	{
		return true;
	}

	return false;
}

static bool operator != (const FGsSchemaSpawnElement& inSource, const FGsSchemaSpawnElement& inDest)
{
	if (inSource.SpawnId != inDest.SpawnId
		|| inSource.memberType != inDest.memberType
		|| inSource.SpawnType != inDest.SpawnType
		|| inSource.CreatureId != inDest.CreatureId
		|| inSource.SpawnCount != inDest.SpawnCount
		|| inSource.RandomPos != inDest.RandomPos
		|| inSource.spawnLevel != inDest.spawnLevel
		|| inSource.Pos != inDest.Pos
		|| inSource.Dir != inDest.Dir
		|| inSource.RespawnInterval != inDest.RespawnInterval
		|| inSource.spawnRate != inDest.spawnRate
		|| inSource.groupLimitRange != inDest.groupLimitRange
		|| inSource.ClientGameId != inDest.ClientGameId
		|| inSource.InitVisible != inDest.InitVisible
		|| inSource.VisibleType != inDest.VisibleType
		|| inSource.AiOptimizeOnOff != inDest.AiOptimizeOnOff
		|| inSource.isFixedLocation != inDest.isFixedLocation
		|| inSource.skipTerrainCheck != inDest.skipTerrainCheck)
	{
		return true;
	}

	if (inSource.QuestCondition.Num() != inDest.QuestCondition.Num())
	{
		return true;
	}

	int32 questConditionNum = inSource.QuestCondition.Num();
	for (int32 questConditionIndex = 0; questConditionIndex < questConditionNum; ++questConditionIndex)
	{
		if (inSource.QuestCondition[questConditionIndex] != inDest.QuestCondition[questConditionIndex])
		{
			return true;
		}
	}

	return false;
}

static bool operator != (const FGsSchemaTerritoryZoneShape& inSource, const FGsSchemaTerritoryZoneShape& inDest)
{
	if (inSource.Center != inDest.Center
		|| inSource.Radius != inDest.Radius)
	{
		return true;
	}

	return false;
}

static bool operator != (const FGsSchemaSpawnData& inSource, const FGsSchemaSpawnData& inDest)
{
	//Diff zone
	if (inSource.SpawnGroupId != inDest.SpawnGroupId		
		|| inSource.groupId != inDest.groupId
		|| inSource.SpawnGroupType != inDest.SpawnGroupType
		|| inSource.byClient != inDest.byClient
		|| inSource.byDefault != inDest.byDefault
		|| inSource.MapId != inDest.MapId		
		|| inSource.Region != inDest.Region
		|| inSource.lineId != inDest.lineId		
		|| inSource.groupLineMove != inDest.groupLineMove)
	{
		return true;
	}

	if (inSource.SpawnInfoList.Num() != inDest.SpawnInfoList.Num())
	{
		return true;
	}

	//Check npc data
	int32 spawnDataNum = inSource.SpawnInfoList.Num();
	for (int spawnDataIndex = 0; spawnDataIndex < spawnDataNum; ++spawnDataIndex)
	{		
		if (inSource.SpawnInfoList[spawnDataIndex] != (inDest.SpawnInfoList[spawnDataIndex]))
		{
			return true;
		}
	}

	if (inSource.randomSpawnMinDistance != inDest.randomSpawnMinDistance)
	{
		return true;
	}

	return false;
}
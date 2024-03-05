// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GsTerritoryDataManager.h"

//t1 project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "UTIL/GsDir.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeMapData.h"

/**
 *
 */
class TERRITORYEDITOR_API FGsTerrEdMonsterKnowledgeMapData : public FGsTerritoryDataBase
{
private:
	const UGsTable* _npcTable;
	const UGsTable* _mapTable;
	const UGsTable* _worldMapMonsterTable;
	UGsTable* _monsterKnowledgeMapDataTable;

public:
	FGsTerrEdMonsterKnowledgeMapData();
	virtual ~FGsTerrEdMonsterKnowledgeMapData();

protected:
	virtual bool Save(uint32 inSaveFlag, class UWorld* inWorld, bool inSuccess, class ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce = false) override;
	bool IsNeedToSaveMonsterKnowledgeMapData(const TMap<int32, FGsSchemaMonsterKnowledgeMapData>& inDataMap);
	void SaveMonsterKnowledgeMapData(const TMap<int32, FGsSchemaMonsterKnowledgeMapData>& inDataMap);
	bool TryGetAllMonsterKnowledgeMapData(OUT TMap<int32, FGsSchemaMonsterKnowledgeMapData>& outDataMap);	
};

static bool operator != (const FGsSchemaMonsterKnowledgeMapData& inSource, const FGsSchemaMonsterKnowledgeMapData& inDest)
{
	if (inSource.npcData.GetRow() != inDest.npcData.GetRow())
	{
		return true;
	}

	int32 spawnMapNum = inSource.spawnMapDataArray.Num();
	int32 destMapNum = inDest.spawnMapDataArray.Num();
	if (spawnMapNum != destMapNum)
	{
		return true;
	}

	for (int32 i = 0; i < spawnMapNum; ++i)
	{
		const FGsSchemaMapDataRow& sourceRow = inSource.spawnMapDataArray[i];
		const FGsSchemaMapDataRow& destRow = inDest.spawnMapDataArray[i];
		if (sourceRow.GetRow() != destRow.GetRow())
		{
			return true;
		}
	}	

	return false;
}
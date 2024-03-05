// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GsTerritoryDataManager.h"

//t1 project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "UTIL/GsDir.h"
#include "DataSchema/MonsterDropItem/GsSchemaMonsterInMapData.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 *
 */

const int32 LAUNCHER_LEVEL_KEY_MAP = 0;
const int32 BEGIN_LEVEL_KEY_MAP = 1;

class TERRITORYEDITOR_API FGsTerrEdMonsterToMapData : public FGsTerritoryDataBase
{
private:
	const UGsTable* _npcTable;
	const UGsTable* _mapTable;
	const UGsTable* _worldMapMonsterTable;
	UGsTable* _monsterToMapTable;

public:
	FGsTerrEdMonsterToMapData();
	virtual ~FGsTerrEdMonsterToMapData();

protected:
	virtual bool Save(uint32 inSaveFlag, class UWorld* inWorld, bool inSuccess, class ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce = false) override;
	bool IsNeedToSaveMonsterToMapData(TMap<int32, TMap<int32, TArray<FGsSchemaMonsterInMapData>>>& inData);
	void SaveMonsterToMapData(TMap<int32, TMap<int32, TArray<FGsSchemaMonsterInMapData>>>& inData);
	bool TryGetAllMonsterToMapData(OUT TMap<int32, TMap<int32, TArray<FGsSchemaMonsterInMapData>>>& outAllMonsterMap);
	bool TryGetAllMonsterInMapData(int32 inCreatureId, int32 inLevel, int32 inMapId, OUT FGsSchemaMonsterInMapData& outMonsterInMapData);
	bool TryGetAllMonsterInRegionDataList(int32 inCreatureId, int32 inLevel, int32 inMapId, UGsTable* inSpawnDataTable, OUT TArray<FGsSchemaMonsterInRegionData>& outSpawnDataArray);
};
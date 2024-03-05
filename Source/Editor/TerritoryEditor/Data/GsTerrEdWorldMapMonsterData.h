// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GsTerritoryDataManager.h"

//t1 project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "UTIL/GsDir.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterElementData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "SharedEnums/SharedItemEnum.h"
#include "Item/GsSchemaItemCommon.h"
#include "Map/WorldMap/GsSchemaWorldMapSpoilsMonsterLevel.h"

/**
 *
 */
const int32 LAUNCHER_LEVEL_KEY = 0;
const int32 BEGIN_LEVEL_KEY = 1;

class TERRITORYEDITOR_API FGsTerrEdWorldMapMonsterData : public FGsTerritoryDataBase
{
private:
	const UGsTable* _npcTable;
	const UGsTable* _mapTable;
	const UGsTable* _itemCommonTable;
	const UGsTable* _regionTable;	
	const UGsTable* _fieldBossTable;
	const UGsTable* _worldBossTable;
	const UGsTable* _dropTable;
	const UGsTable* _invadeMapDataTable;

private:
	UGsTable* _worldMapMonsterTable;
	UGsTable* _worldMapLevelTable;
	UGsTable* _worldMapSpoilsTable;
	UGsTable* _worldMapMonsterElementDataTable;

public:
	FGsTerrEdWorldMapMonsterData();
	virtual ~FGsTerrEdWorldMapMonsterData();

protected:
	virtual bool Save(uint32 inSaveFlag, class UWorld* inWorld, bool inSuccess, class ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce = false) override;
	bool IsNeedToSaveWorldMapMonsterData(const TMap<int32, TArray<FGsSchemaWorldMapMonsterElementData>>& inData);
	void SaveWorldMapMonsterData(const TMap<int32, TArray<FGsSchemaWorldMapMonsterElementData>>& inData);
	void SaveWorldMapLevelData();
	void SaveWorldMapSpoilsData();

protected:
	bool TryGetAllMonsterData(OUT TMap<int32, TArray<FGsSchemaWorldMapMonsterElementData>>& outAllMonsterMap);
	bool TryGetMapData(OUT TArray<const struct FGsSchemaMapData*>& outMapArray);
	bool TryGetMonsterList(const struct FGsSchemaMapData* inMapData, class UGsTable* inSpawnTable, OUT TArray<FGsSchemaWorldMapMonsterElementData>& outWorldMapMonster);
	bool TryGetMonsterElement(OUT TArray<FGsSchemaWorldMapMonsterElementData>& outData, const struct FGsSchemaMapData* inMapData, TArray<const struct FGsSchemaSpawnData*>& inSpawnList, const struct FGsSchemaNpcData* inNpcData);
	bool GetOffensive(TArray<const struct FGsSchemaSpawnData*>& inSpawnList, const FGsSchemaNpcData* inNpcData);
	bool TryGetDropItemArray(const struct FGsSchemaMapData* inMapData, const struct FGsSchemaNpcData* inNpcData, TArray<const struct FGsSchemaSpawnData*>& inSpawnList, const TArray<int32>& inLevelArray, OUT TArray<ItemId>& outDropItemArray);
	//DropTable에서 드랍 관련된 조건에 따라 드랍 데이터를 가져온다.
	bool TryGetDropDataArray(const struct FGsSchemaMapData* inMapData, const struct FGsSchemaNpcData* inNpcData, int32 inLevel, OUT TArray<const struct FGsSchemaDropData*>& outDropData);
	bool TryGetMonsterDropItem(OUT TArray<FGsSchemaWorldMapMonsterElementData>& outData, const struct FGsSchemaMapData* inMapData, TArray<const struct FGsSchemaSpawnData*>& inSpawnList, const FGsSchemaNpcData* inNpcData);
	bool TryGetBossMonsterDropItem(OUT TArray<FGsSchemaWorldMapMonsterElementData>& outData, const struct FGsSchemaMapData* inMapData, TArray<const struct FGsSchemaSpawnData*>& inSpawnList, const FGsSchemaNpcData* inNpcData);
	bool TryGetRewardData(OUT FGsSchemaWorldMapMonsterElementData& outData, const FGsSchemaNpcBalanceLevel* inFindBalance);
	bool TryGetRewardFieldBossDataData(OUT FGsSchemaWorldMapMonsterElementData& outData, const struct FGsSchemaFieldBossData* inFieldBossData);
	bool TryGetRewardWorldBossDataData(OUT FGsSchemaWorldMapMonsterElementData& outData, const struct FGsSchemaWorldBossData* inBossData);
	bool TryGetItemFromRewardBox(OUT FGsSchemaWorldMapMonsterElementData& outData, const FGsSchemaRewardBoxData* inRewardBox);
	bool IsInvadePossibleMap(int inMapId);
	bool TryGetInvadeDropItem(const FGsSchemaNpcData* npcData, InvadeDropType inInvadeType, OUT TArray<const struct FGsSchemaItemCommon*>& outEquipItemArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEtcItemArray);
	bool TryGetInvadeDropData(const FGsSchemaNpcData* npcData, InvadeDropType inInvadeType, OUT TArray<const struct FGsSchemaDropData*>& outDropDataArray);
	bool TryConvertDropToItemData(TArray<const struct FGsSchemaDropData*>& inDropDataArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEquipItemArray, OUT TArray<const struct FGsSchemaItemCommon*>& outEtcItemArray);

	//전리품 정보 얻어오기
public:
	bool TryGetAllItemOnMap(ItemType itemType, const struct FGsSchemaWorldMapMonsterData* inMonsterData, OUT TArray<FGsSchemaItemCommonRow>& outLevelData);
	bool TryGetAllMonsterRelatedItem(const FGsSchemaItemCommonRow& inItem, const FGsSchemaWorldMapMonsterData* inMonsterData, OUT TArray<FGsSchemaWorldMapSpoilsMonsterLevel>& outNpcData);

public:
	bool TryGetLevelArray(const FGsSchemaMapData* inMapData, const FGsSchemaNpcData* inNpcData, TArray<const struct FGsSchemaSpawnData*>& inSpawnList, OUT TArray<int32>& outLevelArray);
};

static bool operator != (const FGsSchemaWorldMapMonsterElementData& inSource, const FGsSchemaWorldMapMonsterElementData& inDest)
{
	int32 equipItemNum = inSource.equipItemList.Num();
	int32 etcItemNum = inSource.etcItemList.Num();
	if (equipItemNum != inDest.equipItemList.Num()
	|| etcItemNum != inDest.etcItemList.Num())
	{
		return true;
	}

	for (int32 i = 0; i < equipItemNum; ++i)
	{
		const FGsSchemaItemCommonRow& sourceRow = inSource.equipItemList[i];
		const FGsSchemaItemCommonRow& destRow = inDest.equipItemList[i];
		if (nullptr == sourceRow.GetRow()
			|| nullptr == destRow.GetRow())
		{
			return true;
		}

		if (sourceRow.GetRow()->id != destRow.GetRow()->id)
		{
			return true;
		}
	}	

	for (int32 i = 0; i < etcItemNum; ++i)
	{		
		const FGsSchemaItemCommonRow& sourceRow = inSource.etcItemList[i];
		const FGsSchemaItemCommonRow& destRow = inDest.etcItemList[i];
		if (nullptr == sourceRow.GetRow()
			|| nullptr == destRow.GetRow())
		{
			return true;
		}

		if (destRow.GetRow()->id != sourceRow.GetRow()->id)
		{
			return true;
		}
	}

	return ((inSource.id.GetRow()->id != inSource.id.GetRow()->id)
		|| (inSource.isOffensive != inDest.isOffensive)
		|| (inSource.rank != inDest.rank));
}
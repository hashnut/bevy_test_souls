// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrEdMonsterKnowledgeMapData.h"
#include "DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeMapData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterData.h"


FGsTerrEdMonsterKnowledgeMapData::FGsTerrEdMonsterKnowledgeMapData()
{
}

FGsTerrEdMonsterKnowledgeMapData::~FGsTerrEdMonsterKnowledgeMapData()
{
}

bool FGsTerrEdMonsterKnowledgeMapData::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce)
{	
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();	
	_npcTable = tableManager.GetTable(FGsSchemaNpcData::StaticStruct());	
	_mapTable = tableManager.GetTable(FGsSchemaMapData::StaticStruct());	
	_worldMapMonsterTable = tableManager.GetTable(FGsSchemaWorldMapMonsterData::StaticStruct());	
	_monsterKnowledgeMapDataTable = tableManager.GetTable(FGsSchemaMonsterKnowledgeMapData::StaticStruct());

	//Get all monster list 
	TMap<int32, FGsSchemaMonsterKnowledgeMapData> monsterKnowledgeMapData;
	TryGetAllMonsterKnowledgeMapData(monsterKnowledgeMapData);

	if (inSaveForce)
	{
		SaveMonsterKnowledgeMapData(monsterKnowledgeMapData);
	}
	else
	{
		if (IsNeedToSaveMonsterKnowledgeMapData(monsterKnowledgeMapData))
		{
			SaveMonsterKnowledgeMapData(monsterKnowledgeMapData);
		}
	}	

	return true;
}


bool FGsTerrEdMonsterKnowledgeMapData::TryGetAllMonsterKnowledgeMapData(OUT TMap<int32, FGsSchemaMonsterKnowledgeMapData>& outDataMap)
{
	//월드 몬스터 데이터에서 npc가 포함된 맵을 찾아 FGsSchemaMonsterKnowledgeMapData 에 저장한다. 

	outDataMap.Empty();
	TArray<const FGsSchemaWorldMapMonsterData*> worldMapMonsterDataArray;
	_worldMapMonsterTable->GetAllRows<const FGsSchemaWorldMapMonsterData>(worldMapMonsterDataArray);

	TArray<const FGsSchemaNpcData*> npcDataArray;
	_npcTable->GetAllRows<const FGsSchemaNpcData>(npcDataArray);
	for (const FGsSchemaNpcData* npcData : npcDataArray)
	{
		FGsSchemaMonsterKnowledgeMapData data;
		data.RowName = *FString::FromInt(npcData->id);
		FGsSchemaNpcDataRow npcIdRow;
		npcIdRow.DataTable = _npcTable->GetCachedDataTable();
		npcIdRow.RowName = npcData->RowName;
		data.id = npcData->id;
		data.npcData = npcIdRow;		

		for (const FGsSchemaWorldMapMonsterData* worldMapMonster : worldMapMonsterDataArray)
		{
			const FGsSchemaWorldMapMonsterElementDataRow* findWorldMapMonsterRow = worldMapMonster->monsters.FindByPredicate([&](const FGsSchemaWorldMapMonsterElementDataRow& elementDataRow)->bool {
				const FGsSchemaWorldMapMonsterElementData* elementData = elementDataRow.GetRow();
				return (nullptr != elementData) && elementData->id.GetRow() == npcData;
				});
			if (nullptr == findWorldMapMonsterRow)
			{
				continue;
			}

			const FGsSchemaWorldMapMonsterElementData* findWorldMapMonster = findWorldMapMonsterRow->GetRow();
			if (findWorldMapMonster)
			{				
				FGsSchemaMapDataRow mapData;
				mapData.DataTable = _mapTable->GetCachedDataTable();
				mapData.RowName = *FString::FromInt(worldMapMonster->mapId);
				data.spawnMapDataArray.Emplace(mapData);
			}
		}

		outDataMap.Emplace(npcData->id, data);
	}

	return outDataMap.Num() > 0;
}

void FGsTerrEdMonsterKnowledgeMapData::SaveMonsterKnowledgeMapData(const TMap<int32, FGsSchemaMonsterKnowledgeMapData>& inDataMap)
{
	_monsterKnowledgeMapDataTable->Empty();
	for (const TPair<int32, FGsSchemaMonsterKnowledgeMapData>& pair : inDataMap)
	{
		_monsterKnowledgeMapDataTable->AddRow(pair.Value);
	}

	_monsterKnowledgeMapDataTable->SaveDataTable();
}

bool FGsTerrEdMonsterKnowledgeMapData::IsNeedToSaveMonsterKnowledgeMapData(const TMap<int32, FGsSchemaMonsterKnowledgeMapData>& inDataMap)
{
	if (nullptr == _monsterKnowledgeMapDataTable)
	{
		return false;
	}

	TArray<const FGsSchemaMonsterKnowledgeMapData*> monsterKnowledgeMapDataList;
	_monsterKnowledgeMapDataTable->GetAllRows(monsterKnowledgeMapDataList);

	if (monsterKnowledgeMapDataList.Num() != inDataMap.Num())
	{
		return true;
	}

	int32 num = 0;
	for (const FGsSchemaMonsterKnowledgeMapData* data : monsterKnowledgeMapDataList)
	{
		if (nullptr == data)
		{
			continue;
		}

		if (false == inDataMap.Contains(data->id))
		{
			return true;
		}

		if ((*data) != inDataMap[data->id])
		{
			return true;
		}
	}

	return false;
}
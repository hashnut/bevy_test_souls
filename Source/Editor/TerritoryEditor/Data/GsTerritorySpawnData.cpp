// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritorySpawnData.h"

//T1 project
#include "Editor/GsSelectionPrevantInterface.h"
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "UTIL/GsDir.h"

//territory editor
#include "Util/GsTerritoryLevelUtil.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryNpc.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Map/Spawn/GsSchemaSpawnData.h"
#include "Map/Spawn/GsSchemaEventSpawnData.h"
#include "GsTerritoryDataManager.h"
#include "LogMacros.h"
#include "Map/Spawn/GsSchemaClientSpawnData.h"
#include "TerritoryEditor.h"
#include "Map/Spawn/GsSchemaSpawnVisibleData.h"
#include "Array.h"
#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsGameObjectUtil.h"
#include "RSQLEditor/Private/Helper/GsNotificatorOnDestruction.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"
#include "SharedInclude/SharedTypes.h"
#include "DataSchema/Map/GsSchemaMapTerritorySpaceCrack.h"

FGsTerritorySpawnData::FGsTerritorySpawnData()
{
}

FGsTerritorySpawnData::~FGsTerritorySpawnData()
{
}

bool FGsTerritorySpawnData::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce)
{		
	FString spawnDataTableName;
	FString clientspawnDataTableName;
	FString eventSpawnDataTableName;
	FString spawnVisibleTableName;
	
	UGsTable* spawnDataTable;
	UGsTable* clientSpawnDataTable;
	UGsTable* eventSpawnDataTable;
	UGsTable* spawnVisibleDataTable;	

	//table이 mapdata에 등록되어 있고 존재하는가?
	//spawn table 존재하지 않는다면 생성		
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaSpawnData, UGsTableSpawnData>(spawnDataTable, spawnDataTableName, inLevelKey);
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaClientSpawnData, UGsTableClientSpawnData>(clientSpawnDataTable, clientspawnDataTableName, inLevelKey);
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaEventSpawnData, UGsTableEventSpawnData>(eventSpawnDataTable, eventSpawnDataTableName, inLevelKey);
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaSpawnVisibleData, UGsTableSpawnVisibleData>(spawnVisibleDataTable, spawnVisibleTableName, inLevelKey);	

	const FString serverLevelName = UGsTerritoryLevelUtil::GetServerLevelName(FCString::Atoi(*inLevelKey));
	const TArray<FGsSchemaSpawnData> spawnDataArray = GetAllActorForSchema<FGsSchemaSpawnData, AGsTerritoryZone>(inCurrLevel, serverLevelName);

	//저장해야하는지 비교
	if (inSaveForce)
	{
		SaveSpawnData(inLevelKey, spawnDataArray);
		SaveClientSpawnData(inLevelKey, spawnDataArray);
		SaveEventSpawnData(inLevelKey, spawnDataArray);		
		SaveSpawnVisibleData(inLevelKey, spawnDataArray);
		CheckMapObstactle(inLevelKey, spawnDataArray);

		Export(inLevelKey);
	}
	else
	{
		if (IsNeedToSaveSpawnData(inLevelKey, spawnDataArray))
		{
			SaveSpawnData(inLevelKey, spawnDataArray);
			SaveClientSpawnData(inLevelKey, spawnDataArray);
			SaveEventSpawnData(inLevelKey, spawnDataArray);			
			SaveSpawnVisibleData(inLevelKey, spawnDataArray);
			CheckMapObstactle(inLevelKey, spawnDataArray);

			Export(inLevelKey);
		}
	}

	return true;
}

bool FGsTerritorySpawnData::IsNeedToSaveSpawnData(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(inLevelKey);
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableMgr.GetTable(FGsSchemaSpawnData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("TERRITORY_EDITOR_DEBUG - Missing table : %s"), *tableName);
		return false;
	}

	TArray<const FGsSchemaSpawnData*> spawnList;
	table->GetAllRows<FGsSchemaSpawnData>(spawnList);

	//Check zone number
	if (spawnList.Num() != inSpawnDataArray.Num())
	{
		return true;
	}

	const FGsSchemaSpawnData* spawnData = nullptr;
	int32 spawnNum = inSpawnDataArray.Num();
	for (int spawnDataIndex = 0; spawnDataIndex < spawnNum; ++spawnDataIndex)
	{
		//Check zone data	
		if (table->FindRow<FGsSchemaSpawnData>(*FString::FromInt(spawnDataIndex), spawnData))
		{
			if ((*spawnData) != inSpawnDataArray[spawnDataIndex])
			{
				return true;
			}
		}
	}

	return false;
}

void FGsTerritorySpawnData::SaveSpawnData(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray)
{
	FString spawnDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaSpawnData::StaticStruct(), spawnDataTableName);
	if (nullptr == table)
	{
		return;
	}

	table->Empty();

	int32 spawnDataNum = inSpawnDataArray.Num();
	FGsSchemaSpawnData newSpawnData;
	for (int32 spawnDataIndex = 0; spawnDataIndex < spawnDataNum; ++spawnDataIndex)
	{		
		newSpawnData = inSpawnDataArray[spawnDataIndex];
		newSpawnData.RowName = FName(*FString::FromInt(spawnDataIndex));
		table->AddRow(newSpawnData);
	}

	table->SaveDataTable();
}

void FGsTerritorySpawnData::SaveClientSpawnData(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray)
{
	FString clientSpawnDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableClientSpawnData>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaClientSpawnData::StaticStruct(), clientSpawnDataTableName);
	if (nullptr == table)
	{
		return;
	}

	table->Empty();
	int32 spawnDataNum = inSpawnDataArray.Num();	
	FGsSchemaClientSpawnData clientSpawnData;
	int32 levelKey = FCString::Atoi(*inLevelKey);

	int32 clidentSpawnDataIndex = 0;
	for (int32 spawnDataIndex = 0; spawnDataIndex < spawnDataNum; ++spawnDataIndex)
	{
		if (inSpawnDataArray[spawnDataIndex].byClient)
		{								
			ParseSpawnDataToClientSpawnData(inSpawnDataArray[spawnDataIndex], clientSpawnData);			 
			clientSpawnData.RowName = FName(*FString::FromInt(clidentSpawnDataIndex));
			table->AddRow(clientSpawnData);
			clidentSpawnDataIndex++;
		}		
	}

	table->SaveDataTable();	
}

void FGsTerritorySpawnData::SaveEventSpawnData(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray)
{
	FString eventSpawnDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableEventSpawnData>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaEventSpawnData::StaticStruct(), eventSpawnDataTableName);
	if (nullptr == table)
	{
		return;
	}

	table->Empty();
	int32 spawnDataNum = inSpawnDataArray.Num();
	FGsSchemaEventSpawnData eventSpawnData;
	int32 levelKey = FCString::Atoi(*inLevelKey);

	int32 eventSpawnDataIndex = 0;
	for (int32 spawnDataIndex = 0; spawnDataIndex < spawnDataNum; ++spawnDataIndex)
	{
		if (false == inSpawnDataArray[spawnDataIndex].byDefault)
		{						
			eventSpawnData.RowName = FName(*FString::FromInt(eventSpawnDataIndex));
			eventSpawnData.MapId = levelKey;
			eventSpawnData.SpawnGroupId = inSpawnDataArray[spawnDataIndex].SpawnGroupId;						
			table->AddRow(eventSpawnData);
			eventSpawnDataIndex++;
		}
	}

	table->SaveDataTable();	
}

void FGsTerritorySpawnData::SaveSpawnVisibleData(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray)
{
	FString spawnVisibleDataName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnVisibleData>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaSpawnVisibleData::StaticStruct(), spawnVisibleDataName);
	if (nullptr == table)
	{
		return;
	}	

	UGsTable* npcTable = tableManager.GetTable(FGsSchemaNpcData::StaticStruct());
	UGsTableNpcData* castNpcTable = Cast<UGsTableNpcData>(npcTable);
	if (nullptr == castNpcTable)
	{
		return;
	}

	table->Empty();

	int32 spawnDataNum = inSpawnDataArray.Num();
	int32 spawnChildNum = 0;
	int32 questConditionNum = 0;
	QuestId startConditionQuestId = 0;
	QuestId endConditionQuestId = 0;
	FGsSchemaSpawnVisibleData spawnVisibleData;	
	/*FString startConditionQuestIdString = "";
	FString endConditionQuestIdString = "";*/
	const FGsSchemaQuest* startCondtionQuest = nullptr;
	const FGsSchemaQuest* endConditionQuest = nullptr;
	FGsSchemaSpawnVisibleData visibleData;
	const FGsSchemaNpcData* npcFind = nullptr;	

	for (int32 spawnDataIndex = 0; spawnDataIndex < spawnDataNum; ++spawnDataIndex)
	{
		const FGsSchemaSpawnData& spawnData = inSpawnDataArray[spawnDataIndex];
		if (spawnData.byClient)
		{
			continue;
		}

		spawnChildNum = spawnData.SpawnInfoList.Num();
		for (int32 spawnChildIndex = 0; spawnChildIndex < spawnChildNum; ++spawnChildIndex)
		{
			const FGsSchemaSpawnElement& spawnChild = inSpawnDataArray[spawnDataIndex].SpawnInfoList[spawnChildIndex];
			questConditionNum = spawnChild.QuestCondition.Num();
			for (int32 questConditionIndex = 0; questConditionIndex < questConditionNum; ++questConditionIndex)
			{
				startCondtionQuest = spawnChild.QuestCondition[questConditionIndex].StartConditionQuestId.GetRow();
				if (nullptr == startCondtionQuest)
				{
					continue;
				}

				startConditionQuestId = startCondtionQuest->id;
												
				endConditionQuest = spawnChild.QuestCondition[questConditionIndex].EndConditionQuestId.GetRow();
				if (nullptr == endConditionQuest)
				{
					continue;
				}

				if (endConditionQuestId != 0 && startConditionQuestId > endConditionQuestId)
				{										
					continue;
				}

				endConditionQuestId = endConditionQuest->id;
				
				//startConditionQuestId = FCString::Strtoi64(*startConditionQuestIdString, NULL, 10);
				//endConditionQuestId = FCString::Strtoi64(*endConditionQuestIdString, NULL, 10);
				
				visibleData.spawnId = spawnChild.SpawnId;
				visibleData.spawnType = spawnChild.SpawnType;

				//샌드백은 서버에서 스폰해주며 npcFuctionType이 None 이면 샌드백인지 알 수 있다. 
				if (visibleData.spawnType == CreatureSpawnType::NPC)
				{					
					if (castNpcTable->FindRowById(spawnChild.CreatureId, npcFind))
					{
						visibleData.npcFunctionType = npcFind->npcFunctionType;
					}
				}
				else
				{
					visibleData.npcFunctionType = NpcFunctionType::NONE;
				}				

				visibleData.spawnData = spawnChild;				

				table->AddRow(visibleData);
			}
		}
	}

	table->SaveDataTable();
}

void FGsTerritorySpawnData::CheckMapObstactle(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray)
{
	//map 데이타에 맵 id가 등록되어 있어야만 서버용 맵으로 사용할 수 있기 때문에 수정할 때는 찾기만 하면된다. 
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* row;
	FGsSchemaMapTerritoryData newMapData;
	table->FindRow<FGsSchemaMapTerritoryData>(*inLevelKey, row);
	if (nullptr != row)
	{
		newMapData = (*row);
		table->RemoveRow(row);
	}

	newMapData.RowName = *inLevelKey;
	newMapData.isObstacleMap = IsObstacleMap(inLevelKey, inSpawnDataArray);

	table->AddRow(newMapData);
	table->SaveDataTable();
}

bool FGsTerritorySpawnData::IsObstacleMap(const FString& inLevelKey, const TArray<FGsSchemaSpawnData>& inSpawnDataArray)
{
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* npcTable = tableManager.GetTable(FGsSchemaNpcData::StaticStruct());
	UGsTableNpcData* castNpcTable = Cast<UGsTableNpcData>(npcTable);
	if (nullptr == castNpcTable)
	{
		return false;
	}

	bool isObstacleMap = false;
	const FGsSchemaNpcData* npcFind = nullptr;
	for (const FGsSchemaSpawnData& data : inSpawnDataArray)
	{
		for (int32 i = 0; i < data.SpawnInfoList.Num(); ++i)
		{
			const FGsSchemaSpawnElement& spawnChild = data.SpawnInfoList[i];

			if (spawnChild.SpawnType == CreatureSpawnType::NPC)
			{
				if (castNpcTable->FindRowById(spawnChild.CreatureId, npcFind))
				{
					if (npcFind->npcFunctionType == NpcFunctionType::MAGIC_OBJECT
						|| npcFind->npcFunctionType == NpcFunctionType::CART
						|| npcFind->npcFunctionType == NpcFunctionType::DOOR)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

void FGsTerritorySpawnData::Export(const FString& inLevelKey)
{
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();

	//spawn data
	FString spawnDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(inLevelKey);
	if (UGsTable* table = tableManager.GetTable(FGsSchemaSpawnData::StaticStruct(), spawnDataTableName))
	{
		FGsTerritoryDataManager::ExportTable(table, TEXT("spawnData"));
	}	
}

void FGsTerritorySpawnData::ParseSpawnDataToClientSpawnData(const FGsSchemaSpawnData& inSource, FGsSchemaClientSpawnData& inDest)
{
	//Diff zone shape
	inDest.Region = inSource.Region;
	inDest.SpawnGroupId = inSource.SpawnGroupId;
	inDest.byClient = inSource.byClient;
	inDest.byDefault = inSource.byDefault;
	inDest.SpawnInfoList = inSource.SpawnInfoList;
}

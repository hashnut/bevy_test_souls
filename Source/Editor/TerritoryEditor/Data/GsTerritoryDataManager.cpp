// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryDataManager.h"

//T1 project
#include "Editor/GsSelectionPrevantInterface.h"
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "UTIL/GsDir.h"

//territory editor
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "Actor/Spot/GsTerritorySpot.h"
#include "Actor/Warp/GsTerritoryWarp.h"

#include "Util/GsTerritoryLevelUtil.h"
#include "TerritoryEditor.h"

#include "Data/GsTerritorySpawnData.h"
#include "Data/GsTerritorySpotData.h"
#include "Data/GsTerritoryWarpData.h"
#include "GsTerritoryFenceData.h"
#include "Map/GsSchemaMapData.h"
#include "Map/Area/GsSchemaAreaData.h"
#include "GsTerritoryLineData.h"
#include "GsTerrEdTerritoryData.h"
#include "Map/GsSchemaMapTerritoryData.h"
#include "GsTerrEdWorldMapMonsterData.h"
#include "UTIL/GsTableUtil.h"
#include "Map/Spawn/GsSchemaSpawnData.h"
#include "Map/Spawn/GsSchemaEventSpawnData.h"
#include "Map/WorldMap/GsSchemaWorldMapMonsterData.h"
#include "Map/Fence/GsSchemaFenceData.h"
#include "GsTerritoryUniqueData.h"
#include "Helper/GsNotificatorOnDestruction.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "GsTerrEdMonsterKnowledgeMapData.h"
#include "GsTerrEdMonsterToMapData.h"
#include "GsTerritoryBlockData.h"

FGsTerritoryDataManager::FGsTerritoryDataManager()
{

}

FGsTerritoryDataManager::~FGsTerritoryDataManager()
{
	Clear();
}

bool FGsTerritoryDataManager::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, bool inSaveForce)
{
	if (nullptr == inWorld)
	{
		return false;
	}

	if (nullptr == GEditor->GetEditorWorldContext().World()->WorldComposition)
	{
		return false;
	}

	ULevel* level = inWorld->GetCurrentLevel();
	if (nullptr == level)
	{
		return false;
	}

	const FString& path = level->GetPathName();
	const FString& levelName = FPackageName::GetShortName(FPackageName::FilenameToLongPackageName(path));

	FString levelKey;	
	FString persistantLevelName;	
	EGsTerritoryMapType mapType = EGsTerritoryMapType::Art;

	if (UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
	{
		mapType = EGsTerritoryMapType::Server;
	}
	else if(UGsTerritoryLevelUtil::IsValidDesignLevelName(levelName, persistantLevelName, levelKey))
	{
		mapType = EGsTerritoryMapType::Design;
	}

	if (mapType == EGsTerritoryMapType::Art)
	{
		return false;
	}
	
	bool isServerLevel = UGsTerritoryLevelUtil::IsRegisteredLevel(persistantLevelName);	
	if (false == isServerLevel)
	{
		return false;
	}

	//check creat map territory data
	FString mapTerritoryTableName;
	UGsTable* mapTerritoryDataTable;
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaMapTerritoryData, UGsTableMapTerritoryData>(mapTerritoryDataTable, mapTerritoryTableName, levelKey);

	for (TSharedPtr<FGsTerritoryDataBase> data : _autoSaveDataManager)
	{
		if (false == data.IsValid())
		{
			continue;
		}

		if (mapType != data.Get()->GetMapType())
		{
			continue;
		}
		
		data.Get()->Save(inSaveFlag, inWorld, inSuccess, level, levelKey, inSaveForce);		
	}

	Export();
	
	//check map data 
	CheckTerritoryDataMissing();

	return true;
}

void FGsTerritoryDataManager::SaveManually()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	FString levelKeyText;
	ULevel* level = UGsTerritoryLevelUtil::GetCurrentServerLevel(world, levelKeyText);	
	for(TSharedPtr<FGsTerritoryDataBase> data : _manualSaveDataManagerList)
	{
		if (data.IsValid())
		{
			data->Save(false, world, false, level, levelKeyText, false);
		}		
	}
}

void FGsTerritoryDataManager::Initialize()
{
	InitAutoSaveData();
	InitManualSaveData();

	_isSaveValid = true;	
}

void FGsTerritoryDataManager::Clear()
{
	ClearAutoSaveData();
	ClearManualSaveData();
}

void FGsTerritoryDataManager::InitAutoSaveData()
{
	//Spawn	
	_autoSaveDataManager.Add(MakeShareable(new FGsTerritorySpawnData()));

	//Spot	
	_autoSaveDataManager.Add(MakeShareable(new FGsTerritorySpotData()));

	//Warp	
	_autoSaveDataManager.Add(MakeShareable(new FGsTerritoryWarpData()));

	//Fence	
	_autoSaveDataManager.Add(MakeShareable(new FGsTerritoryFenceData()));

	//Territory, fence	
	_autoSaveDataManager.Add(MakeShareable(new FGsTerrEdTerritoryData()));

	//Line	
	_autoSaveDataManager.Add(MakeShareable(new FGsTerritoryLineData()));

	//Block
	_autoSaveDataManager.Add(MakeShareable(new FGsTerritoryBlockData()));

#if SAVE_RTT
	//unique data like rtt	
	_autoSaveDataManager.Add(MakeShareable(new FGsTerritoryUniqueData()));	
#endif
}

void FGsTerritoryDataManager::ClearAutoSaveData()
{
	for (TSharedPtr<FGsTerritoryDataBase> data : _autoSaveDataManager)
	{
		if (data.IsValid())
		{
			data->Clear();
		}
	}

	_autoSaveDataManager.Empty();
}

void FGsTerritoryDataManager::Export()
{
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* mapTable = tableManager.GetTable(FGsSchemaMapData::StaticStruct());	

	ExportTable(mapTable, TEXT("mapData"));
}

void FGsTerritoryDataManager::InitManualSaveData()
{
	//Territory
	TSharedPtr<FGsTerrEdWorldMapMonsterData> worldMapMonsterData = MakeShareable(new FGsTerrEdWorldMapMonsterData());
	_manualSaveDataManagerList.Add(worldMapMonsterData);

	TSharedPtr<FGsTerrEdMonsterKnowledgeMapData> monsterKnowledgeMapData = MakeShareable(new FGsTerrEdMonsterKnowledgeMapData());
	_manualSaveDataManagerList.Add(monsterKnowledgeMapData);

	TSharedPtr<FGsTerrEdMonsterToMapData> monsterToMapData = MakeShareable(new FGsTerrEdMonsterToMapData());
	_manualSaveDataManagerList.Add(monsterToMapData);

	_worldMapMonsterData = worldMapMonsterData;
	_monsterKnowledgeMapData = monsterKnowledgeMapData;
	_monsterToMapData = monsterToMapData;
}

void FGsTerritoryDataManager::ClearManualSaveData()
{
	for (TSharedPtr<FGsTerritoryDataBase> data : _manualSaveDataManagerList)
	{
		if (data.IsValid())
		{
			data->Clear();
		}
	}

	_manualSaveDataManagerList.Empty();
}

void FGsTerritoryDataManager::ExportTable(UGsTable* inTable, const FString& inGroupName)
{
	if (nullptr == inTable)
	{
		return;
	}

	TArray<TSoftObjectPtr<UGsDataTable>> rsqlArray;

	rsqlArray.Add(inTable->GetCachedDataTable());

	FString root = FPaths::ProjectDir();
	FString absolutePath = FPaths::ConvertRelativePathToFull(root);
	FString path = absolutePath + "Saved/DataCenter/" + inTable->GetInnerPackagePath();
	
	UGsTableManagerEditor::ExportTerritoryDataTableJson(path, inGroupName, &rsqlArray);
}

void FGsTerritoryDataManager::CheckTerritoryDataMissing()
{
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* mapTable = tableManager.GetTable(FGsSchemaMapData::StaticStruct());
	if (mapTable)
	{
		TArray<const FGsSchemaMapData*> mapDataList;
		mapTable->GetAllRows(mapDataList);
		for (const FGsSchemaMapData* mapData : mapDataList)
		{
			if (mapData)
			{
				if (nullptr == mapData->territoryInfo.GetRow())
				{
					FString log = TEXT("map id ") + FString::FromInt(mapData->id) + TEXT("\t territory data is missing");
					FGsNotificatorOnDestruction::ShowNotificationNow(FText::FromString(log)
						, SNotificationItem::ECompletionState::CS_Fail
						, nullptr
						, FGsNotificatorOnDestruction::eHyperLinkType::Select
						, 60.0f);
				}

				const FGsSchemaMapBalance* mapBalanceData = mapData->balanceId.GetRow();
				if (mapBalanceData)
				{
					if (nullptr == mapBalanceData->resurrectionDataIdx.GetRow())
					{
						FString log = TEXT("map id ") + FString::FromInt(mapData->id) + TEXT("\t resurrection data is missing");
						FGsNotificatorOnDestruction::ShowNotificationNow(FText::FromString(log)
							, SNotificationItem::ECompletionState::CS_Fail
							, nullptr
							, FGsNotificatorOnDestruction::eHyperLinkType::Select
							, 60.0f);
					}
				}
			}
		}
	}
}

void FGsTerritoryDataManager::ClearAllTerritoryData()
{
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* mapTable = tableManager.GetTable(FGsSchemaMapData::StaticStruct());
	if (nullptr == mapTable)
	{
		return;
	}

	TArray<const FGsSchemaMapData*> mapList;
	if (false == mapTable->GetAllRows(mapList))
	{
		return;
	}

	for (const FGsSchemaMapData* map : mapList)
	{
		if (nullptr == map)
		{
			continue;
		}

		int32 levelId = map->id;
		if (levelId == 0)
		{
			continue;
		}

		FString levelIdName = FString::FromInt(levelId);

		//spawn
		FString spawnDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpawnData>(levelIdName);
		TryClearCustomDataTable<FGsSchemaSpawnData>(spawnDataTableName);

		FString clientSpawnDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableClientSpawnData>(levelIdName);
		TryClearCustomDataTable<FGsSchemaClientSpawnData>(clientSpawnDataTableName);

		FString eventSpawnDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableEventSpawnData>(levelIdName);
		TryClearCustomDataTable<FGsSchemaEventSpawnData>(eventSpawnDataTableName);

		//warp, territory data
		FString spotDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(levelIdName);
		TryClearCustomDataTable<FGsSchemaSpotData>(spotDataTableName);

		FString fenceDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableFenceData>(levelIdName);
		TryClearCustomDataTable<FGsSchemaFenceData>(fenceDataTableName);

		FString mapTerritoryDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(levelIdName);
		TryClearCustomDataTable<FGsSchemaMapTerritoryData>(mapTerritoryDataTableName);

		FString townDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableTownData>(levelIdName);
		TryClearCustomDataTable<FGsSchemaTownData>(townDataTableName);

		FString worldMapMonsterDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableWorldMapMonsterData>(levelIdName);
		TryClearCustomDataTable<FGsSchemaWorldMapMonsterData>(worldMapMonsterDataTableName);
	}
}

bool FGsTerritoryDataBase::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrentLevel, const FString& inLevelKey, bool inSaveForce)
{
	//기존 데이터랑 비교해서 저장해야하는가?
	if (inSaveForce)
	{
		SaveTable(inCurrentLevel, inLevelKey);
		Export(inLevelKey);
	}
	else
	{
		if (IsNeedToSave(inCurrentLevel, inLevelKey))
		{
			SaveTable(inCurrentLevel, inLevelKey);
			Export(inLevelKey);
		}
	}	

	return true;
}

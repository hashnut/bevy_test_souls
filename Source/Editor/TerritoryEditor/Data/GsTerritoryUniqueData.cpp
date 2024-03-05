// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryUniqueData.h"

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
#include "Map/GsSchemaMapTerritoryData.h"
#include "Map/Spot/GsSchemaSpotData.h"
#include "GsTable.h"
#include "Actor/Unique/GsTerrEdRTTActor.h"
#include "Map/GsSchemaMapSpotInfo.h"
#include "GsSchemaRowBase.h"

FGsTerritoryUniqueData::FGsTerritoryUniqueData()
{
}

FGsTerritoryUniqueData::~FGsTerritoryUniqueData()
{
}


bool FGsTerritoryUniqueData::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce)
{	
	if (nullptr == inCurrLevel)
	{
		return false;
	}

#if SAVE_RTT
	AActor** find = nullptr;
	AGsTerrEdRTTActor* rtt = nullptr;
	find = inCurrLevel->Actors.FindByPredicate([](const AActor* actor)->bool {
		if (actor)
		{
			return actor->IsA(AGsTerrEdRTTActor::StaticClass());
		}

		return false;
	});

	if (find)
	{
		rtt = Cast<AGsTerrEdRTTActor>(*find);
		if (inSaveForce)
		{
			SaveRTT(inLevelKey, rtt);
			ExportRTT(inLevelKey);
		}
		else
		{
			if (IsNeedToSaveRTT(inLevelKey, rtt))
			{
				SaveRTT(inLevelKey, rtt);
				ExportRTT(inLevelKey);
			}
		}		
	}	
#endif

	return true;
}

bool FGsTerritoryUniqueData::IsNeedToSaveRTT(const FString& inLevelKey, const AGsTerrEdRTTActor* inRTT)
{
#if SAVE_RTT
	//map 데이타에 맵 id가 등록되어 있어야만 서버용 맵으로 사용할 수 있기 때문에 수정할 때는 찾기만 하면된다. 
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* territoryTable = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == territoryTable)
	{
		return false;
	}

	FString spotDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(inLevelKey);
	UGsTable* spotTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), spotDataTableName);
	if (nullptr == spotTable)
	{
		return false;
	}

	const FGsSchemaMapTerritoryData* row;	
	territoryTable->FindRow<FGsSchemaMapTerritoryData>(*inLevelKey, row);
	if (nullptr == row || nullptr == row->RTT.mapId.GetRow())
	{
		return false;
	}	
	
	FString destMap = FString::FromInt(inRTT->_destMapId);	
	FString rttDestMapName = row->RTT.mapId.GetRow()->GetRowName().ToString();

	return ((false == rttDestMapName.Equals(destMap)) || (row->RTT.spotIndex != inRTT->_destSpotId));
#else
	return false;
#endif	
}

void FGsTerritoryUniqueData::SaveRTT(const FString& inLevelKey, const AGsTerrEdRTTActor* inRTT)
{
#if SAVE_RTT
	//map 데이타에 맵 id가 등록되어 있어야만 서버용 맵으로 사용할 수 있기 때문에 수정할 때는 찾기만 하면된다. 
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* territoryTable = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == territoryTable)
	{
		return;
	}

	FString spotDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(inLevelKey);
	UGsTable* spotTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), spotDataTableName);
	if (nullptr == spotTable)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* row;
	FGsSchemaMapTerritoryData newMapData;

	territoryTable->FindRow<FGsSchemaMapTerritoryData>(*inLevelKey, row);
	if (nullptr != row)
	{
		newMapData = (*row);
		territoryTable->RemoveRow(row);
	}

	FGsSchemaSpotDataRow spotDataRow;
	spotDataRow.DataTable = spotTable->GetCachedDataTable();
	spotDataRow.RowName = *inLevelKey;

	newMapData.RowName = *inLevelKey;
	newMapData.RTT.mapId = spotDataRow;
	newMapData.RTT.spotIndex = inRTT->_destSpotId;

	territoryTable->AddRow(newMapData);
	territoryTable->SaveDataTable();
#endif
}

void FGsTerritoryUniqueData::ExportRTT(const FString& inLevelKey)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(inLevelKey);
	if (UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), tableName))
	{
		FGsTerritoryDataManager::ExportTable(table, TEXT("rttData"));
	}
}
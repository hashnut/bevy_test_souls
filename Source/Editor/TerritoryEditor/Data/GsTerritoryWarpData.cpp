// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryWarpData.h"
#include "Actor/Warp/GsTerritoryWarp.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "TerritoryEditor.h"

//territory editor
#include "DataSchema/Map/GsSchemaMapWarpSpot.h"
#include "UTIL/GsTableUtil.h"
#include "Map/GsSchemaMapTerritoryData.h"

FGsTerritoryWarpData::FGsTerritoryWarpData()
{
}

FGsTerritoryWarpData::~FGsTerritoryWarpData()
{
}

bool FGsTerritoryWarpData::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce)
{
	const FString serverLevelName = UGsTerritoryLevelUtil::GetServerLevelName(FCString::Atoi(*inLevelKey));
	TArray<FGsSchemaMapWarpInfo> warpList = GetAllActorForSchema<FGsSchemaMapWarpInfo, AGsTerritoryWarp>(inCurrLevel, serverLevelName);

	if (inSaveForce)
	{
		SaveWarpData(inLevelKey, warpList);
		Export(inLevelKey);
	}
	else
	{
		if (IsNeedToSaveWarpData(inLevelKey, warpList))
		{
			SaveWarpData(inLevelKey, warpList);
			Export(inLevelKey);
		}
	}	

	return true;
}

bool FGsTerritoryWarpData::IsNeedToSaveWarpData(const FString& inLevelKey, const TArray<FGsSchemaMapWarpInfo>& inWarpList)
{	
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return false;
	}

	const FGsSchemaMapTerritoryData* row;

	if (false == table->FindRow<FGsSchemaMapTerritoryData>(*inLevelKey, row))
	{
		return true;
	}

	if (nullptr == row)
	{
		return true;
	}	

	if (row->warpList.Num() != inWarpList.Num())
	{
		return true;
	}

	for (int32 i = 0; i < inWarpList.Num(); ++i)
	{
		if (inWarpList[i] != row->warpList[i])
		{
			return true;
		}
	}

	return false;
}

void FGsTerritoryWarpData::SaveWarpData(const FString& inLevelKey, TArray<FGsSchemaMapWarpInfo>& inWarpList)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(inLevelKey);	
	UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* row;

	table->FindRow<FGsSchemaMapTerritoryData>(*inLevelKey, row);	

	FGsSchemaMapTerritoryData temp;

	if (nullptr != row)
	{
		temp = *row;
		table->RemoveRow(row);
	}

	temp.warpList = inWarpList;
	temp.RowName = *inLevelKey;

	table->AddRow(temp);
	table->SaveDataTable();
}
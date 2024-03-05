// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritoryLineData.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Actor/Line/GsTerritoryLine.h"
#include "TerritoryEditor.h"
#include "DataSchema/Map/Line/GsSchemaLineData.h"
#include "GsTable.h"
#include "UTIL/GsTableUtil.h"

FGsTerritoryLineData::FGsTerritoryLineData()
{
}

FGsTerritoryLineData::~FGsTerritoryLineData()
{
}

bool FGsTerritoryLineData::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce)
{
	//table이 mapdata에 등록되어 있고 존재하는가?
	//spawn table 존재하지 않는다면 생성	
	_dataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableLineData>(inLevelKey);
	UGsTable* table;
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaLineData, UGsTableLineData>(table, _dataTableName, inLevelKey);

	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	_table = tableManager.GetTable(FGsSchemaLineData::StaticStruct(), _dataTableName);

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return false;
	}
	
	const FString serverLevelName = UGsTerritoryLevelUtil::GetServerLevelName(FCString::Atoi(*inLevelKey));
	const TArray<FGsSchemaLineElement> lineDataArray = GetAllActorForSchema<FGsSchemaLineElement, AGsTerritoryLine>(inCurrLevel, serverLevelName);

	//저장해야하는지 비교
	if (inSaveForce)
	{
		SaveLineData(inLevelKey, lineDataArray);
		Export(inLevelKey);
	}
	else
	{
		if (IsNeedToSaveLineData(inLevelKey, lineDataArray))
		{
			SaveLineData(inLevelKey, lineDataArray);
			Export(inLevelKey);
		}
	}

	return true;
}

bool FGsTerritoryLineData::IsNeedToSaveLineData(const FString& inLevelKey, const TArray<FGsSchemaLineElement>& inLineElementDataArray)
{
	if (false == _table.IsValid())
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("Missing table : %s"), *_dataTableName);
		return false;
	}

	const FGsSchemaLineData* lineData;
	if (false == _table->FindRow<FGsSchemaLineData>(*inLevelKey, lineData))
	{
		return true;
	}

	//spawn count 
	if (lineData->LineList.Num() != inLineElementDataArray.Num())
	{
		return true;
	}

	int32 num = inLineElementDataArray.Num();
	for (int32 i = 0; i < num; ++i)
	{
		if (lineData->LineList[i] != inLineElementDataArray[i])
		{
			return true;
		}
	}

	return false;
}

void FGsTerritoryLineData::SaveLineData(const FString& inLevelKey, const TArray<FGsSchemaLineElement>& inLineDataArray)
{
	if (false == _table.IsValid())
	{
		return;
	}

	_table.Get()->Empty();

	int32 num = inLineDataArray.Num();
	FGsSchemaLineData newData;
	newData.RowName = *inLevelKey;
	newData.MapId = FCString::Atoi(*inLevelKey);
	newData.LineList = inLineDataArray;

	_table->AddRow(newData);
	_table->SaveDataTable();
}

void FGsTerritoryLineData::Export(const FString& inLevelKey)
{
	if (_table.IsValid())
	{
		FGsTerritoryDataManager::ExportTable(_table.Get(), TEXT("lineData"));
	}	
}
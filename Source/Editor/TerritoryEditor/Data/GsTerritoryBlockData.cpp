// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryBlockData.h"
#include "Components/BoxComponent.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "GsTerritoryDataManager.h"
#include "DataSchema/Map/Box/GsSchemaMapBlockInfo.h"
#include "LogMacros.h"
#include "TerritoryEditor.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "UI/GsAreaNameHUD.h"

FGsTerritoryBlockData::FGsTerritoryBlockData()
{

}

FGsTerritoryBlockData::~FGsTerritoryBlockData()
{

}

bool FGsTerritoryBlockData::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce /*= false*/)
{
	//check is design level
	const FString& longPakageName = inCurrLevel->GetPathName();
	if (false == UGsTerritoryLevelUtil::IsDesignLevel(longPakageName))
	{
		return false;
	}

	UGsTable* mapBlockInfoTable;
	FString mapBlockInfoTableName;
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaMapBlockInfo, UGsTableMapBlockInfo>(mapBlockInfoTable, mapBlockInfoTableName, inLevelKey);

	//get all boxcollision
	TArray<UBoxComponent*> boxComponentArray;
	UActorComponent* boxComponent = nullptr;
	for (AActor* actor : inCurrLevel->Actors)
	{
		if (nullptr == actor)
		{
			continue;
		}

		if (actor->IsA(ALevelBounds::StaticClass())
			|| actor->IsA(AGsAreaNameHUD::StaticClass()))
		{
			continue;
		}

		boxComponent = actor->GetComponentByClass(UBoxComponent::StaticClass());
		if (boxComponent)
		{
			boxComponentArray.Add(Cast<UBoxComponent>(boxComponent));
		}
	}	

	if (inSaveForce)
	{
		SaveBlockData(inLevelKey, boxComponentArray);
		Export(inLevelKey);
	}
	else
	{
		if (IsNeedToSaveBlockData(inLevelKey, boxComponentArray))
		{
			SaveBlockData(inLevelKey, boxComponentArray);
			Export(inLevelKey);
		}
	}

	return true;
}

bool FGsTerritoryBlockData::IsNeedToSaveBlockData(const FString& inLevelKey, const TArray<UBoxComponent*>& inBoxComponentArray)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapBlockInfo>(inLevelKey);
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableMgr.GetTable(FGsSchemaMapBlockInfo::StaticStruct(), tableName);
	if (nullptr == table)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("TERRITORY_EDITOR_DEBUG - Missing table : %s"), *tableName);
		return false;
	}

	const FGsSchemaMapBlockInfo* find = nullptr;
	if (false == table->FindRow(*inLevelKey, find))
	{
		return true;
	}

	if (inBoxComponentArray.Num() > 0
		&& find->blockList.Num() == 0)
	{
		return true;
	}
	

	int32 levelKey = FCString::Atoi(*inLevelKey);
	const TArray<FGsSchemaBox>& blockList = find->blockList;

	if (inBoxComponentArray.Num() != blockList.Num())
	{
		return true;
	}

	const FGsSchemaMapBlockInfo* blockInfo = nullptr;
	int32 num = blockList.Num();
	for (int i = 0; i < num; ++i)
	{
		//Check zone data			
		if (inBoxComponentArray[i]->GetComponentLocation() != blockList[i].center
			|| inBoxComponentArray[i]->GetScaledBoxExtent() != blockList[i].size)
		{
			return true;
		}

		const TArray<FVector>& sourcePointList = UGsLevelManager::GetFloorPointList(inBoxComponentArray[i]);
		const TArray<FVector>& destPointList = blockList[i].pointList;

		if (IsDifferentArray(sourcePointList, destPointList))
		{
			return true;
		}
	}

	return false;
}

void FGsTerritoryBlockData::SaveBlockData(const FString& inLevelKey, const TArray<UBoxComponent*>& inBoxComponentArray)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapBlockInfo>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaMapBlockInfo::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return;
	}

	table->Empty();	

	int32 num = inBoxComponentArray.Num();
	FGsSchemaMapBlockInfo mapInfo;
	mapInfo.RowName = *inLevelKey;	
	mapInfo.maplId = FCString::Atoi(*inLevelKey);

#if TERRITORY_BLOCK_DEBUG
	UWorld* world = GEditor->GetEditorWorldContext().World();
	TArray<int32> indexArray = { 0, 1, 2, 0, 2, 3 };
#endif

	for (UBoxComponent* boxComp : inBoxComponentArray)
	{
		FGsSchemaBox box;
		box.center = boxComp->GetComponentLocation();
		box.size = boxComp->GetScaledBoxExtent();
		box.pointList = UGsLevelManager::GetFloorPointList(boxComp);
		mapInfo.blockList.Add(box);				

#if TERRITORY_BLOCK_DEBUG
		DrawDebugMesh(world, box.pointList, indexArray, FColor::Yellow, false, 300);
#endif
	}

	table->AddRow(mapInfo);
	table->SaveDataTable();
}

void FGsTerritoryBlockData::Export(const FString& inLevelKey)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapBlockInfo>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaMapBlockInfo::StaticStruct(), tableName);
	if (table)
	{
		FGsTerritoryDataManager::ExportTable(table, TEXT("mapBlockInfo"));
	}
}

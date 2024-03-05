// Fill out your copyright notice in the Description page of Project Settings.

#define  RESURRECT_SPOT_SAVE false
#include "GsTerritorySpotData.h"

//t1 project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "UTIL/GsDir.h"

//territory editor
#include "Actor/Spot/GsTerritorySpot.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "TerritoryEditor.h"
#include "UTIL/GsTableUtil.h"
#include "Map/GsSchemaMapTerritoryData.h"
#include "Map/Spot/GsSchemaSpotData.h"
#include "Actor/Spot/GsTerritoryGameSpot.h"
#include "Actor/Spot/GsTerritoryWarpSpot.h"
#include "Actor/Spot/GsTerritorySpaceCrackPortalSpot.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "T1Project.h"
#include "Actor/Fence/GsTerritoryFenceTown.h"
#include "GameObject/Fence/GsFence.h"
#include "Actor/Fence/GsTerritoryFenceTownChaos.h"

FGsTerritorySpotData::FGsTerritorySpotData()
{
}

FGsTerritorySpotData::~FGsTerritorySpotData()
{
}

bool FGsTerritorySpotData::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce)
{	
	_spotTableName = "";
	_spotTable = nullptr;

	_spotTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(inLevelKey);	
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaSpotData, UGsTableSpotData>(_spotTable, _spotTableName, inLevelKey);

	//get table	
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	_spotTable = tableManager.GetTable(FGsSchemaSpotData::StaticStruct(), _spotTableName);

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return false;
	}

	TArray<AGsTerritoryFenceBase*> townArray;
	TArray<AActor*> fenceArray;

	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryFenceTown::StaticClass(), fenceArray);	
	for(AActor* fence : fenceArray)
	{		
		if (AGsTerritoryFenceTown* castTown = Cast<AGsTerritoryFenceTown>(fence))
		{
			townArray.Add(castTown);
		}
	}	
	
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryFenceTownChaos::StaticClass(), fenceArray);
	for (AActor* fence : fenceArray)
	{
		if (AGsTerritoryFenceTownChaos* castTownChaos = Cast<AGsTerritoryFenceTownChaos>(fence))
		{
			townArray.Add(castTownChaos);
		}
	}

	const FString serverLevelName = UGsTerritoryLevelUtil::GetServerLevelName(FCString::Atoi(*inLevelKey));
	const TArray<FGsSchemaSpotInfo> normalSpotList = GetAllActorForSchema<FGsSchemaSpotInfo, AGsTerritoryNormalSpot>(inCurrLevel, serverLevelName);
	const TArray<FGsSchemaSpotInfo> spawnSpotList = GetAllActorForSchema<FGsSchemaSpotInfo, AGsTerritorySpawnSpot>(inCurrLevel, serverLevelName);
	const TArray<FGsSchemaSpotInfo> ressurectionSpotList = GetAllActorForSchema<FGsSchemaSpotInfo, AGsTerritoryResurrectionSpot>(inCurrLevel, serverLevelName);
	const TArray<FGsSchemaSpotInfo> warpSpotList = GetAllActorForSchema<FGsSchemaSpotInfo, AGsTerritoryWarpSpot>(inCurrLevel, serverLevelName);
	TArray<FGsSchemaSpotInfo> teleportSpotList = GetAllActorForSchema<FGsSchemaSpotInfo, AGsTerritoryTeleportSpot>(inCurrLevel, serverLevelName);
	TArray<FVector2D> fencePointArray;

	for(AGsTerritoryFenceBase* town : townArray)
	{
		town->TryGet2DPointArray(fencePointArray);
		for(int32 i = 0; i < teleportSpotList.Num(); ++i)
		{			
			if (FGsFence::IsInPolygon(fencePointArray, FVector2D(teleportSpotList[i].pos)))
			{
				teleportSpotList[i].isInTown = true;

#if false
				if (teleportSpotList[i].isInTown)
				{
					UE_LOG(LogTemp, Error, TEXT("spot in town - id : %lld\t pos : %s"), teleportSpotList[i].id, *teleportSpotList[i].pos.ToString());
				}
#endif
			}
		}		
	}

	TArray<FGsSchemaSpotInfo> spotList;
	spotList.Append(normalSpotList);
	spotList.Append(spawnSpotList);
	spotList.Append(ressurectionSpotList);
	spotList.Append(warpSpotList);
	spotList.Append(teleportSpotList);	

	if (inSaveForce)
	{
		SaveSpotData(inLevelKey, spawnSpotList, ressurectionSpotList, warpSpotList, teleportSpotList, spotList);
		Export(inLevelKey);
	}
	else
	{
		if (IsNeedToSaveSpotData(inLevelKey, spotList))
		{
			SaveSpotData(inLevelKey, spawnSpotList, ressurectionSpotList, warpSpotList, teleportSpotList, spotList);
			Export(inLevelKey);
		}
	}	

	return true;
}

bool FGsTerritorySpotData::IsNeedToSaveSpotData(const FString& inLevelKey, const TArray<FGsSchemaSpotInfo>& inSoptList)
{
	if (nullptr == _spotTable)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("Missing table : %s"), *_spotTableName);
		return false;
	}

	const FGsSchemaSpotData* spotData;
	if (false == _spotTable->FindRow<FGsSchemaSpotData>(*inLevelKey, spotData))
	{
		return true;
	}

	//spawn count 
	if (spotData->SpotList.Num() != inSoptList.Num())
	{
		return true;
	}	

	for (int32 i = 0; i < inSoptList.Num(); ++i)
	{
		if (spotData->SpotList[i] != inSoptList[i])
		{
			return true;
		}
	}

	return false;
}

void FGsTerritorySpotData::SaveSpotData(const FString& inLevelKey
	, const TArray<FGsSchemaSpotInfo>& inSpawnSpotList
	, const TArray<FGsSchemaSpotInfo>& inResurrectionSpotList
	, const TArray<FGsSchemaSpotInfo>& inWarpSpotList	
	, const TArray<FGsSchemaSpotInfo>& inTeleportSpotList	
	, const TArray<FGsSchemaSpotInfo>& inSpotList)
{	
	if (nullptr == _spotTable)
	{
		return;
	}

	_spotTable->Empty();

	FGsSchemaSpotData temp;
	temp.RowName = *inLevelKey;
	temp.mapId = FCString::Atoi(*inLevelKey);
	temp.SpotList = inSpotList;	

	_spotTable->AddRow(temp);
	_spotTable->SaveDataTable();

	//divided territory data table
	SaveMapTerritoryData(inLevelKey, inSpawnSpotList, inResurrectionSpotList, inWarpSpotList, inTeleportSpotList);
}

void FGsTerritorySpotData::SaveMapTerritoryData(const FString& inLevelKey
, const TArray<FGsSchemaSpotInfo>& inSpawnSpotList
, const TArray<FGsSchemaSpotInfo>& inResurrectionSpotList
, const TArray<FGsSchemaSpotInfo>& inWarpSpotList
, const TArray<FGsSchemaSpotInfo>& inTeleportSpotList)
{
	//map 데이타에 맵 id가 등록되어 있어야만 서버용 맵으로 사용할 수 있기 때문에 수정할 때는 찾기만 하면된다. 
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(inLevelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* table = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == _spotTable)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* row;
	table->FindRow<FGsSchemaMapTerritoryData>(*inLevelKey, row);
	FGsSchemaMapTerritoryData newMapData;
	if (row)
	{
		newMapData = (*row);	
		table->RemoveRow(row);
	}

	newMapData.RowName = *inLevelKey;
	newMapData.spawnList.Empty();
#if RESURRECT_SPOT_SAVE
	newMapData.resurrectList.Empty();
#endif
	newMapData.teleportList.Empty();	

	int32 levelKey = FCString::Atoi(*inLevelKey);
	FGsSchemaMapSpotInfo temp;
	FGsSchemaSpotDataRow spotDataRow;

	for (const FGsSchemaSpotInfo& info : inSpawnSpotList)
	{
		spotDataRow.DataTable = _spotTable->GetCachedDataTable();
		spotDataRow.RowName = *FString::FromInt(levelKey);

		temp.mapId = spotDataRow;
		temp.spotIndex = info.id;

		newMapData.spawnList.Add(temp);
	}

#if RESURRECT_SPOT_SAVE
	for (const FGsSchemaSpotInfo& info : inResurrectionSpotList)
	{
		spotDataRow.DataTable = _spotTable->GetCachedDataTable();
		spotDataRow.RowName = *FString::FromInt(levelKey);

		temp.mapId = spotDataRow;
		temp.spotIndex = info.id;

		newMapData.resurrectList.Add(temp);
	}	
#endif

	for (const FGsSchemaSpotInfo& info : inTeleportSpotList)
	{
		spotDataRow.DataTable = _spotTable->GetCachedDataTable();
		spotDataRow.RowName = *FString::FromInt(levelKey);

		temp.mapId = spotDataRow;
		temp.spotIndex = info.id;		

		newMapData.teleportList.Add(temp);
	}

	table->AddRow(newMapData);
	table->SaveDataTable();
}

void FGsTerritorySpotData::Export(const FString& inLevelKey)
{	
	if (_spotTable)
	{
		FGsTerritoryDataManager::ExportTable(_spotTable, TEXT("spotData"));
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceData.h"
#include "Actor/Warp/GsTerritoryWarp.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "TerritoryEditor.h"

//territory editor
#include "DataSchema/Map/GsSchemaMapWarpSpot.h"
#include "Map/Fence/GsSchemaFenceData.h"
#include "Map/Fence/GsSchemaFenceElement.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryNpc.h"
#include "UTIL/GsTableUtil.h"
#include "GsTerritoryDataManager.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "Map/Town/GsSchemaTownData.h"
#include "Actor/Fence/GsTerritoryFenceTown.h"
#include "Actor/Fence/GsTerritoryFenceSafeZone.h"
#include "Actor/Fence/GsTerritoryFenceArena.h"
#include "Actor/Fence/GsTerritoryFenceArenaReady.h"
#include "Actor/Fence/GsTerritoryFenceEnv.h"
#include "Actor/Fence/GsTerritoryFenceSound.h"
#include "Actor/Fence/GsTerritoryFenceSpaceCrack.h"
#include "Actor/Fence/GsTerritoryFenceArrival.h"
#include "Actor/Fence/GsTerritoryFenceTownChaos.h"
#include "Actor/Fence/GsTerritoryFenceNoCampZone.h"
#include "Actor/Fence/GsTerritoryFenceNonPK.h"
#include "DataSchema/Map/GsSchemaMapTerritoryData.h"

FGsTerritoryFenceData::FGsTerritoryFenceData()
{
}

FGsTerritoryFenceData::~FGsTerritoryFenceData()
{
}

bool FGsTerritoryFenceData::Save(uint32 inSaveFlag, UWorld* inWorld, bool inSuccess, ULevel* inCurrLevel, const FString& inLevelKey, bool inSaveForce)
{
	//펜스 저장
	FString fenceDataTableName;
	UGsTable* fenceDataTable;	

	const FString serverLevelName = UGsTerritoryLevelUtil::GetServerLevelName(FCString::Atoi(*inLevelKey));
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaFenceData, UGsTableFenceData>(fenceDataTable, fenceDataTableName, inLevelKey);	
	
	FString mapTerritoryDataTableName;
	UGsTable* mapTerritoryFenceDataTable;
	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaMapTerritoryData, UGsTableMapTerritoryData>(mapTerritoryFenceDataTable, mapTerritoryDataTableName, inLevelKey);	

	TArray<FGsSchemaMapTerritoryInfo> townTerritoryList;
	TArray<FGsSchemaFenceElement> townFenceList;	
	TryGetAllActorForSchema<AGsTerritoryFenceTown, FGsSchemaMapTerritoryInfo, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, townTerritoryList, townFenceList);

	TArray<FGsSchemaMapTerritoryInfo> townChaosTerritoryList;
	TArray<FGsSchemaFenceElement> townChaosFenceList;
	TryGetAllActorForSchema<AGsTerritoryFenceTownChaos, FGsSchemaMapTerritoryInfo, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, townChaosTerritoryList, townChaosFenceList);

	TArray<FGsSchemaMapTerritoryInfo> safeZoneTerritoryList;
	TArray<FGsSchemaFenceElement> safeZoneFenceList;	
	TryGetAllActorForSchema<AGsTerritoryFenceSafeZone, FGsSchemaMapTerritoryInfo, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, safeZoneTerritoryList, safeZoneFenceList);

	TArray<FGsSchemaMapTerritoryInfo> arenaTerritoryList;
	TArray<FGsSchemaFenceElement> arenaFenceList;
	TryGetAllActorForSchema<AGsTerritoryFenceArena, FGsSchemaMapTerritoryInfo, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, arenaTerritoryList, arenaFenceList);

	TArray<FGsSchemaMapTerritoryInfo> arenaReadyTerritoryList;
	TArray<FGsSchemaFenceElement> arenaReadyFenceList;
	TryGetAllActorForSchema<AGsTerritoryFenceArenaReady, FGsSchemaMapTerritoryInfo, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, arenaReadyTerritoryList, arenaReadyFenceList);

	TArray<FGsSchemaMapTerritoryInfo> arrivalTerritoryList;
	TArray<FGsSchemaFenceElement> arrivalFenceList;
	TryGetAllActorForSchema<AGsTerritoryFenceArrival, FGsSchemaMapTerritoryInfo, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, arrivalTerritoryList, arrivalFenceList);

	TArray<FGsSchemaMapTerritoryInfo> nonPKTerritoryList;
	TArray<FGsSchemaFenceElement> nonPKFenceList;
	TryGetAllActorForSchema<AGsTerritoryFenceNonPK, FGsSchemaMapTerritoryInfo, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, nonPKTerritoryList, nonPKFenceList);

	//환경 
	TArray<FGsSchemaMapTerritoryEnv> envTerritoryList;
	TArray<FGsSchemaFenceElement> envFenceList;
	TryGetAllActorForSchema<AGsTerritoryFenceEnv, FGsSchemaMapTerritoryEnv, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, envTerritoryList, envFenceList);

	//사운드
	TArray<FGsSchemaMapTerritorySound> soundTerritoryList;
	TArray<FGsSchemaFenceElement> soundFenceList;
	TryGetAllActorForSchema<AGsTerritoryFenceSound, FGsSchemaMapTerritorySound, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, soundTerritoryList, soundFenceList);

	//시공의 틈새
	TArray<FGsSchemaMapTerritorySpaceCrack> spaceCrackTerritoryList;
	TArray<FGsSchemaFenceElement> spaceCrackFenceList;
	TryGetAllActorForSchema<AGsTerritoryFenceSpaceCrack, FGsSchemaMapTerritorySpaceCrack, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, spaceCrackTerritoryList, spaceCrackFenceList);

	TArray<FGsSchemaMapTerritoryInfo> noCampZoneTerritoryList;
	TArray<FGsSchemaFenceElement> noCampZoneFenceList;
	TryGetAllActorForSchema<AGsTerritoryFenceNoCampZone, FGsSchemaMapTerritoryInfo, FGsSchemaFenceElement>(inCurrLevel, serverLevelName, noCampZoneTerritoryList, noCampZoneFenceList);

	TArray<FGsSchemaMapTerritoryInfo> serverTerritoryList;
	serverTerritoryList.Append(townTerritoryList);
	serverTerritoryList.Append(townChaosTerritoryList);
	serverTerritoryList.Append(safeZoneTerritoryList);
	serverTerritoryList.Append(arenaTerritoryList);
	serverTerritoryList.Append(arenaReadyTerritoryList);
	serverTerritoryList.Append(arrivalTerritoryList);	
	serverTerritoryList.Append(nonPKTerritoryList);

	TArray<FGsSchemaFenceElement> fenceArray;
	fenceArray.Append(townFenceList);
	fenceArray.Append(townChaosFenceList);
	fenceArray.Append(safeZoneFenceList);
	fenceArray.Append(arenaFenceList);
	fenceArray.Append(arenaReadyFenceList);
	fenceArray.Append(arrivalFenceList);
	fenceArray.Append(envFenceList);
	fenceArray.Append(soundFenceList);
	fenceArray.Append(spaceCrackFenceList);	
	fenceArray.Append(noCampZoneFenceList);
	fenceArray.Append(nonPKFenceList);

	if (inSaveForce)
	{
		SaveFenceData(inLevelKey
			, serverTerritoryList
			, noCampZoneTerritoryList
			, envTerritoryList, soundTerritoryList, spaceCrackTerritoryList
			, fenceArray);
		Export(inLevelKey);
	}
	else
	{
		if (IsNeedToSaveFenceData(inLevelKey
			, serverTerritoryList
			, noCampZoneTerritoryList
			, envTerritoryList, soundTerritoryList, spaceCrackTerritoryList
			, fenceArray))
		{
			SaveFenceData(inLevelKey
				, serverTerritoryList
				, noCampZoneTerritoryList
				, envTerritoryList, soundTerritoryList, spaceCrackTerritoryList
				, fenceArray);
			Export(inLevelKey);
		}
	}

	//Town내에 있는 Npc 저장
	FString townDataTableName;
	UGsTable* townDataTable;

	FGsTerritoryDataManager::CheckCreateDataTable<FGsSchemaTownData, UGsTableTownData>(townDataTable, townDataTableName, inLevelKey);
	TArray<AGsTerritoryZone*> zoneList = UGsTerritoryLevelUtil::GetAllTerritoryActor<AGsTerritoryZone>(inCurrLevel);

	TArray<AGsTerritoryFenceBase*> territoryArray;
	TryGetAllActor<AGsTerritoryFenceBase>(inCurrLevel, serverLevelName, territoryArray);

	TArray<const AGsTerritorySpawnActor*> npcClientSpawnInTownList;
	TArray<const AGsTerritorySpawnActor*> npcServerSpawnInTownList;
	TryGetNpcInTown(npcClientSpawnInTownList, npcServerSpawnInTownList, territoryArray, zoneList);

	if (inSaveForce)
	{
		SaveZonesInTown(inLevelKey, npcClientSpawnInTownList, npcServerSpawnInTownList);
		Export(inLevelKey);
	}
	else
	{
		if (IsNeedToSaveTownData(npcClientSpawnInTownList, npcServerSpawnInTownList, inLevelKey))
		{
			SaveZonesInTown(inLevelKey, npcClientSpawnInTownList, npcServerSpawnInTownList);
			Export(inLevelKey);
		}
	}

	return true;
}

bool FGsTerritoryFenceData::IsNeedToSaveFenceData(const FString& inLevelKey, const TArray<FGsSchemaFenceElement>& inFenceList)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableFenceData>(inLevelKey);	
	UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaFenceData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("Missing table : %s"), *tableName);
		return false;
	}	

	const FGsSchemaFenceData* row;
	if (false == table->FindRow<FGsSchemaFenceData>(*inLevelKey, row))
	{
		return true;
	}

	TArray<FGsSchemaFenceElement> sourceFenceList = row->FenceList;

	int32 sourceNum = sourceFenceList.Num();
	int32 destNum = inFenceList.Num();

	if (sourceNum != destNum)
	{
		return true;
	}

	for (int32 i = 0; i < sourceNum; ++i)
	{
		if (sourceFenceList[i] != inFenceList[i])
		{
			return true;
		}
	}

	return false;
}


bool FGsTerritoryFenceData::IsNeedToSaveFenceData(const FString& inLevelKey
	, const TArray<FGsSchemaMapTerritoryInfo>& inServerTerritoryList
	, const TArray<FGsSchemaMapTerritoryInfo>& inNoCampZoneList
	, const TArray<FGsSchemaMapTerritoryEnv>& inEnvTerritoryList
	, const TArray<FGsSchemaMapTerritorySound>& inSoundTerritoryList
	, const TArray<FGsSchemaMapTerritorySpaceCrack>& spaceCrackTerritoryList
	, const TArray<FGsSchemaFenceElement>& inFenceList)
{
	return IsNeedToSaveMapTerritoryInternal(inLevelKey
		, inServerTerritoryList
		, inNoCampZoneList
		, inEnvTerritoryList
		, inSoundTerritoryList
		, spaceCrackTerritoryList)
		|| IsNeedToSaveFenceInternal(inLevelKey, inFenceList);
}

bool FGsTerritoryFenceData::IsNeedToSaveMapTerritoryInternal(const FString& inLevelKey
	, const TArray<FGsSchemaMapTerritoryInfo>& inServerTerritoryList
	, const TArray<FGsSchemaMapTerritoryInfo>& inNoCampZoneList
	, const TArray<FGsSchemaMapTerritoryEnv>& inEnvTerritoryList
	, const TArray<FGsSchemaMapTerritorySound>& inSoundTerritoryList
	, const TArray<FGsSchemaMapTerritorySpaceCrack>& inSpaceCrackTerritoryList)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(inLevelKey);
	UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("Missing table : %s"), *tableName);
		return false;
	}

	const FGsSchemaMapTerritoryData* row;
	if (false == table->FindRow<FGsSchemaMapTerritoryData>(*inLevelKey, row))
	{
		return true;
	}

	if (IsDifferentArray(inServerTerritoryList, row->territoryList))
	{
		return true;
	}

	if (IsDifferentArray(inNoCampZoneList, row->noCampZoneList))
	{
		return true;
	}

	if (IsDifferentArray(inEnvTerritoryList, row->envTerritoryList))
	{
		return true;
	}

	if (IsDifferentArray(inSoundTerritoryList, row->soundTerritoryList))
	{
		return true;
	}

	if (IsDifferentArray(inSpaceCrackTerritoryList, row->spaceCrackZoneList))
	{
		return true;
	}

	return false;
}

bool FGsTerritoryFenceData::IsNeedToSaveFenceInternal(const FString& inLevelKey
	, const TArray<FGsSchemaFenceElement>& inFenceList)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableFenceData>(inLevelKey);
	UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaFenceData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("Missing table : %s"), *tableName);
		return false;
	}

	const FGsSchemaFenceData* row;
	if (false == table->FindRow<FGsSchemaFenceData>(*inLevelKey, row))
	{
		return true;
	}

	TArray<FGsSchemaFenceElement> sourceFenceList = row->FenceList;

	int32 sourceNum = sourceFenceList.Num();
	int32 destNum = inFenceList.Num();

	if (sourceNum != destNum)
	{
		return true;
	}

	for (int32 i = 0; i < sourceNum; ++i)
	{
		if (sourceFenceList[i] != inFenceList[i])
		{
			return true;
		}
	}

	return false;
}

void FGsTerritoryFenceData::SaveFenceData(const FString& inLevelKey, TArray<FGsSchemaFenceElement>& inFenceList)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableFenceData>(inLevelKey);	
	UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaFenceData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return;
	}

	int32 levelKey = FCString::Atoi(*inLevelKey);

	table->Empty();	

	FGsSchemaFenceData temp;

	temp.RowName = *inLevelKey;
	temp.mapId = FCString::Atoi(*inLevelKey);
	temp.FenceList = inFenceList;

	table->AddRow(temp);		
	table->SaveDataTable();
}

void FGsTerritoryFenceData::SaveFenceData(const FString& inLevelKey
	, const TArray<FGsSchemaMapTerritoryInfo>& inServerTerritoryList
	, const TArray<FGsSchemaMapTerritoryInfo>& inNoCampZoneList
	, const TArray<FGsSchemaMapTerritoryEnv>& inEnvTerritoryList
	, const TArray<FGsSchemaMapTerritorySound>& inSoundTerritoryList
	, const TArray<FGsSchemaMapTerritorySpaceCrack>& inSpaceCrackTerritoryList
	, const TArray<FGsSchemaFenceElement>& inFenceList)
{
	SaveMapTerritoryInternal(inLevelKey
		, inServerTerritoryList
		, inNoCampZoneList
		, inEnvTerritoryList
		, inSoundTerritoryList
		, inSpaceCrackTerritoryList);
	SaveFenceInternal(inLevelKey, inFenceList);
}

void FGsTerritoryFenceData::SaveMapTerritoryInternal(const FString& inLevelKey
	, const TArray<FGsSchemaMapTerritoryInfo>& inServerTerritoryList
	, const TArray<FGsSchemaMapTerritoryInfo>& inNoCampZoneList
	, const TArray<FGsSchemaMapTerritoryEnv>& inEnvTerritoryList
	, const TArray<FGsSchemaMapTerritorySound>& inSoundTerritoryList
	, const TArray<FGsSchemaMapTerritorySpaceCrack>& inSpaceCrackTerritoryList)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(inLevelKey);
	UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == table)
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
	newMapData.territoryList.Empty();
	newMapData.spaceCrackZoneList.Empty();
	newMapData.envTerritoryList.Empty();
	newMapData.soundTerritoryList.Empty();
	newMapData.noCampZoneList.Empty();
	
	//territory 	
	newMapData.territoryList = inServerTerritoryList;
	newMapData.spaceCrackZoneList = inSpaceCrackTerritoryList;
	newMapData.envTerritoryList = inEnvTerritoryList;
	newMapData.soundTerritoryList = inSoundTerritoryList;
	newMapData.noCampZoneList = inNoCampZoneList;

	table->AddRow(newMapData);
	table->SaveDataTable();
}

void FGsTerritoryFenceData::SaveFenceInternal(const FString& inLevelKey
	, const TArray<FGsSchemaFenceElement>& inFenceList)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableFenceData>(inLevelKey);
	UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaFenceData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		return;
	}

	int32 levelKey = FCString::Atoi(*inLevelKey);

	table->Empty();

	FGsSchemaFenceData temp;

	temp.RowName = *inLevelKey;
	temp.mapId = FCString::Atoi(*inLevelKey);
	temp.FenceList = inFenceList;

	table->AddRow(temp);
	table->SaveDataTable();
}

void FGsTerritoryFenceData::Export(const FString& inLevelKey)
{
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableFenceData>(inLevelKey);	
	if (UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaFenceData::StaticStruct(), tableName))
	{
		FGsTerritoryDataManager::ExportTable(table, TEXT("fenceData"));
	}
}

bool FGsTerritoryFenceData::IsNeedToSaveTownData(const TArray<const AGsTerritorySpawnActor*>& inClientSpawnList
	, const TArray<const AGsTerritorySpawnActor*>& inServerSpawnList
	, const FString& inLevelKey)
{
	TArray<const AGsTerritorySpawnActor*> spawnList;
	spawnList.Append(inClientSpawnList);
	spawnList.Append(inServerSpawnList);

	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableTownData>(inLevelKey);
	UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaTownData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("Missing table : %s"), *tableName);
		return false;
	}

	TArray<const FGsSchemaTownData*> townNpcList;	
	if (false == table->GetAllRows<FGsSchemaTownData>(townNpcList))
	{
		return true;
	}

	int32 sourceNum = townNpcList.Num();
	int32 destNum = spawnList.Num();

	//Fence num check
	if (sourceNum != destNum)
	{
		return true;
	}

	//npc num check 
	for (int32 i = 0; i < sourceNum; ++i)
	{
		if(townNpcList[i] 
		&& spawnList[i]
		&& ((*townNpcList[i]) != (*spawnList[i])))
		{
			return true;
		}		
	}

	return false;
}

void FGsTerritoryFenceData::SaveZonesInTown(const FString& inLevelKey
	, const TArray<const AGsTerritorySpawnActor*>& inClientSpawnList
	, const TArray<const AGsTerritorySpawnActor*>& inServerSpawnList)
{
	//Get Data
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableTownData>(inLevelKey);
	UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaTownData::StaticStruct(), tableName);
	if (nullptr == table)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("Missing table : %s"), *tableName);
		return;
	}

	table->Empty();

	UGsTable* npcTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaNpcData::StaticStruct());

	//Get all npc
	for (const AGsTerritorySpawnActor* spawnActor : inClientSpawnList)
	{		
		if (nullptr == spawnActor)
		{
			continue;
		}

		if (spawnActor->GetCreatureSpawnType() == CreatureSpawnType::NPC)
		{
			FGsSchemaTownData newData;			

			newData.RowName = *FString::Printf(TEXT("%lld"), spawnActor->GetTerritoryUniqueID());
			newData.id = spawnActor->GetTerritoryUniqueID();			
			newData.Pos = spawnActor->GetActorLocation();
			newData.Dir = FGsDir::RotatorToAngle(spawnActor->GetActorRotation());
			newData.isClientSpawn = true;

			FGsSchemaNpcDataRow npcData;
			npcData.DataTable = npcTable->GetCachedDataTable();
			npcData.RowName = *FString::FromInt(spawnActor->_spawnInfo.TargetID);
			newData.CreatureId = npcData;

			table->AddRow(newData);
		}	
	}

	//Get all npc
	for (const AGsTerritorySpawnActor* spawnActor : inServerSpawnList)
	{
		if (nullptr == spawnActor)
		{
			continue;
		}

		if (spawnActor->GetCreatureSpawnType() == CreatureSpawnType::NPC)
		{
			FGsSchemaTownData newData;

			newData.RowName = *FString::Printf(TEXT("%lld"), spawnActor->GetTerritoryUniqueID());
			newData.id = spawnActor->GetTerritoryUniqueID();
			newData.Pos = spawnActor->GetActorLocation();
			newData.Dir = FGsDir::RotatorToAngle(spawnActor->GetActorRotation());
			newData.isClientSpawn = false;

			FGsSchemaNpcDataRow npcData;
			npcData.DataTable = npcTable->GetCachedDataTable();
			npcData.RowName = *FString::FromInt(spawnActor->_spawnInfo.TargetID);
			newData.CreatureId = npcData;

			table->AddRow(newData);
		}
	}
	
	table->SaveDataTable();
}

bool FGsTerritoryFenceData::TryGetZoneListInTerritory(const FGsSchemaFenceElement* inTerritory
, const TArray<AGsTerritoryZone*>& inZoneList
, OUT TArray<const AGsTerritoryZone*>& outZoneList)
{	
	outZoneList.Empty();
	if (nullptr == inTerritory)
	{
		return false;
	}

	TArray<FVector2D> pointArray;
	for (int32 i = 0; i < inTerritory->vertexList.Num(); ++i)
	{
		pointArray.Add(FVector2D(inTerritory->vertexList[i]));
	}
		
	for (const AGsTerritoryZone* zone : inZoneList)
	{
		if (nullptr == zone)
		{
			continue;
		}

		if ((inTerritory->Center - zone->GetActorLocation()).Size() > inTerritory->Radius)
		{
			continue;
		}
		
		if (false == FGsFence::IsInPolygon(pointArray, FVector2D(zone->GetActorLocation())))
		{
			continue;
		}

		outZoneList.Add(zone);
	}

	return true;
}

bool FGsTerritoryFenceData::TryGetNpcInTown(OUT TArray<const AGsTerritorySpawnActor*>& outClientSpawnActorList
	, OUT TArray<const AGsTerritorySpawnActor*>& outServerSpawnActorList
	, TArray<AGsTerritoryFenceBase*>& inTerritoryArray
	, const TArray<AGsTerritoryZone*>& inZoneList)
{
	outClientSpawnActorList.Empty();
	outServerSpawnActorList.Empty();
	//Get all npc list in territory

	TArray<const AGsTerritoryZone*> zoneInFenceList;
	for(AGsTerritoryFenceBase* territory : inTerritoryArray)
	{
		if (territory->GetFenceType() != TerritoryType::TOWN
			&& territory->GetFenceType() != TerritoryType::TOWN_CHAOS)
		{
			continue;
		}
		
		FGsSchemaFenceElement fenceData;
		FGsSchemaMapTerritoryInfo territoryInfo;
		if (AGsTerritoryFenceTownChaos* chaos = Cast<AGsTerritoryFenceTownChaos>(territory))
		{
			chaos->Export(territoryInfo, fenceData);
		}
		else if (AGsTerritoryFenceTown* town = Cast<AGsTerritoryFenceTown>(territory))
		{
			town->Export(territoryInfo, fenceData);
		}

		if (false == TryGetZoneListInTerritory(&fenceData, inZoneList, zoneInFenceList))
		{
			continue;	
		}

		for (const AGsTerritoryZone* zone : zoneInFenceList)
		{
			if (nullptr == zone)
			{
				continue;
			}
			
			if (zone->_byClient)
			{
				outClientSpawnActorList.Append(zone->_spawnActors);
			}
			else
			{
				outServerSpawnActorList.Append(zone->_spawnActors);
			}								
		}
	}	

	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrEdRecoveryUtil.h"

//UE4
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/LevelEditorViewport.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

//T1 Project
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsDir.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "DataSchema/Map/Spawn/GsSchemaTerritoryNpc.h"

//Territory editor
#include "Actor/Spot/GsTerritorySpot.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "GsTerritoryEditorSetting.h"
#include "TerritoryEditor.h"
#include "Map/Area/GsSchemaAreaData.h"
#include "GsTerritoryUniqueIDManager.h"
#include "UTIL/GsText.h"
#include "GameObject/Npc/GsSchemaNpcData.h"
#include "GameObject/GsSchemaShapeData.h"
#include "GsTable.h"
#include "GsSchemaBase.h"
#include "GsTableManagerEditor.h"
#include "GsTerritoryLevelUtil.h"
#include "Blutility/GsTerritoryGroundActionUtility.h"
#include "Blutility/Input/GsTerritorySpotInput.h"
#include "Actor/Spot/GsTerritoryWarpSpot.h"


void UGsTerrEdRecoveryUtil::DestroyAllTerritoryActors()
{
	FTerritoryEditor::GetInstance()->GetDataManager()->SetSaveValid(false);

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	UGsTerritoryLevelUtil::LoadWorkLevels(world, EGsTerritoryLevelType::Server);

	TArray<AActor*> zones;

	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryZone::StaticClass(), zones);

	for (AActor* temp : zones)
	{
		if (temp)
		{
			if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(temp))
			{
				zone->DestroyManually();
			}
		}
	}

	FTerritoryEditor::GetInstance()->GetDataManager()->SetSaveValid(true);
}

void UGsTerrEdRecoveryUtil::CreateTerritoryActorFromDataTable(const FGsSchemaSpawnData* inData)
{
	/*if (inData)
	{
		if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
		{
			UClass* zoneClass = settings->_circleZoneClass;
			UClass* npcClass = settings->_npcClass;

			if (UWorld* world = GEditor->GetEditorWorldContext().World())
			{
				FTerritoryEditor::_isCanSetUniqueID = false;

				for (const FGsSchemaTerritoryNpc& zoneInfo : inData->SpawnInfoList)
				{
					FTransform zoneTrs;

					zoneTrs.SetLocation(zoneInfo.Region.Center);

					if (AActor* temp = GEditor->AddActor(world->GetCurrentLevel(), zoneClass, zoneTrs))
					{
						if (AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(temp))
						{
							zone->SetTerritoryUniqueID(zoneInfo.SpawnGroupId);
							zone->_radius = zoneInfo.Region.Radius;
							zone->SetActorLocation(zoneInfo.Region.Center);

							for (const FGsSchemaTerritoryNpc& npcInfo : zoneInfo.SpawnInfoList)
							{
								FGsTerritorySpawnData spawnData;

								spawnData.Type = EGsTerritorySpawnType::Npc;
								spawnData.UniqueID = npcInfo.SpawnId;
								spawnData.Rotation = FGsDir::AngleToRotator(npcInfo.Dir).Vector();
								spawnData.TargetID = npcInfo.CreatureId;
								spawnData.MaxSpawnNum = npcInfo.SpawnCount;
								spawnData.RespawnInterval = npcInfo.RespawnInterval;
								spawnData.CanRandomLocationSpawn = npcInfo.RandomPos;

								if (AGsTerritorySpawnActor* spawnActor = zone->AddTerritorySpawnActor(spawnData, npcInfo.Pos))
								{
									spawnActor->SetTerritoryUniqueID(npcInfo.SpawnId);
								}
							}

							zone->CreateView(zoneTrs);
						}
					}
				}

				FTerritoryEditor::_isCanSetUniqueID = true;
			}
		}
	}*/
}

void UGsTerrEdRecoveryUtil::DestroyAllMapMarkers()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	if (nullptr == world)
	{
		return;
	}

	UGsTerritoryLevelUtil::LoadWorkLevels(world, EGsTerritoryLevelType::Server);

	TArray<AActor*> markers;

	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritorySpot::StaticClass(), markers);

	for (AActor* temp : markers)
	{
		if (temp)
		{
			if (AGsTerritorySpot* marker = Cast<AGsTerritorySpot>(temp))
			{
				marker->ConditionalBeginDestroy();
			}
		}
	}
}

void UGsTerrEdRecoveryUtil::CreateMapMarkersFromDataTable(const FGsSchemaMapData* inData)
{
	//if (inData)
	//{
	//	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	//	{
	//		UClass* spawnMarkerClass = settings->_spawnSpotClass;
	//		UClass* resseractionMarkerClass = settings->_ressurectionSpotClass;

	//		if (UWorld* world = GEditor->GetEditorWorldContext().World())
	//		{
	//			for (const FGsSchemaMapSpotInfo& info : inData->spawnList)
	//			{
	//				/*FTransform trs;

	//				trs.SetLocation(info.p);

	//				const FRotator dir = FGsDir::AngleToRotator(info.dir);
	//				trs.SetRotation(dir.Quaternion());

	//				if (AActor* spawn = GEditor->AddActor(world->GetCurrentLevel(), spawnMarkerClass, trs))
	//				{
	//					if (AGsTerritorySpawnSpot* marker = Cast<AGsTerritorySpawnSpot>(spawn))
	//					{
	//						marker->_teamType = info.teamType;
	//						marker->_radius = info.posRandomRange;
	//					}
	//				}*/
	//			}

	//			for (const FGsSchemaMapSpotInfo& info : inData->resurrectList)
	//			{
	//				/*FTransform trs;

	//				trs.SetLocation(info.pos);

	//				const FRotator dir = FGsDir::AngleToRotator(info.dir);
	//				trs.SetRotation(dir.Quaternion());

	//				if (AActor* spawn = GEditor->AddActor(world->GetCurrentLevel(), resseractionMarkerClass, trs))
	//				{
	//					if (AGsTerritoryResurrectionSpot* marker = Cast<AGsTerritoryResurrectionSpot>(spawn))
	//					{
	//						marker->_teamType = info.teamType;
	//						marker->_radius = info.posRandomRange;
	//					}
	//				}*/
	//			}
	//		}
	//	}
	//}
}

void UGsTerrEdRecoveryUtil::ResolveMultiUniqueID()
{
	FTerritoryEditor* instance = FTerritoryEditor::GetInstance();
	if (nullptr == instance)
	{
		return;
	}

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	ULevel* level = world->GetCurrentLevel();
	if (nullptr == level)
	{
		return;
	}

	if (false == UGsTerritoryLevelUtil::IsServerLevel(level->GetPathName()))
	{
		FString log = "Not server level : " + level->GetFName().ToString();
		FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("TerritoryEditor"), *log);

		return;
	}

	FGsTerritoryUniqueIDManager* uniqueIdManager = instance->GetUniqueIDManager();

	bool success = true;

	//get all multi key instance
	for (int32 i = 0; i < (int32)EGsTerritoryObjType::Max; ++i)
	{
		EGsTerritoryObjType objType = (EGsTerritoryObjType)i;

		if (objType == EGsTerritoryObjType::Spot)
		{
			continue;
		}

		TArray<AGsTerritoryActor*> territoryActors;
		TSet<int64> instanceMap;
		TArray<AGsTerritoryActor*> multiInstanceActors;

		if (UGsTerritoryLevelUtil::TryGetAllTerritoryActor(level, objType, territoryActors))
		{
			for (AGsTerritoryActor* temp : territoryActors)
			{
				if (instanceMap.Contains(temp->GetTerritoryInstanceId()))
				{
					multiInstanceActors.Add(temp);
				}
				else
				{
					instanceMap.Add(temp->GetTerritoryInstanceId());
				}
			}
		}

		for (AGsTerritoryActor* actor : multiInstanceActors)
		{
			if (nullptr == actor)
			{
				continue;
			}

			uint32 oldInstanceId = (uint32)actor->GetTerritoryInstanceId();
			int64 instanceId;
			int64 uniqueId;

			if (false == uniqueIdManager->TryGenerateUniqueID(level, objType, uniqueId, instanceId))
			{
				success = false;

				FString type = FGsTextUtil::GetEnumValueAsString("EGsTerritoryObjType", objType);
				UE_LOG(LogTerritoryEditor, Error, TEXT("Resolve fail - objType %s\tid : %d"), *type, oldInstanceId);

				continue;
			}

			actor->SetTerritoryUniqueID(uniqueId, instanceId);
		}
	}

	if (success)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("TerritoryEditor"), FString("Resolve multi key success"));
	}
	else
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("TerritoryEditor"), FString("Resolve multi key fail"));
	}
}


void UGsTerrEdRecoveryUtil::RecoveryNpcTidFromMesh()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	TArray<AActor*> npcList;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryNpc::StaticClass(), npcList);

	UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();

	const UGsTable* npcTable = mgr.GetTable(FGsSchemaNpcData::StaticStruct());
	const UGsTable* shapeTable = mgr.GetTable(FGsSchemaShapeData::StaticStruct());
	if (nullptr == npcTable || nullptr == shapeTable)
	{
		return;
	}

	TArray<const FGsSchemaNpcData*> npcInfoList;
	TArray<const FGsSchemaShapeData*> shapeInfoList;
	npcTable->GetAllRows<FGsSchemaNpcData>(npcInfoList);
	shapeTable->GetAllRows<FGsSchemaShapeData>(shapeInfoList);

	for (AActor* actor : npcList)
	{
		if (nullptr == actor)
		{
			continue;
		}

		AGsTerritoryNpc* npc = Cast<AGsTerritoryNpc>(actor);

		if (nullptr == npc)
		{
			continue;
		}

		if (npc->_npcChildActor
			&& npc->_npcChildActor->GetChildActorClass())
		{
			FString path = npc->_npcChildActor->GetChildActorClass()->GetPathName();

			const FGsSchemaShapeData** findShapeData = shapeInfoList.FindByPredicate([&](const FGsSchemaShapeData* shapeData)->bool {

				FString meshPath = shapeData->skeletalMeshPath.GetAssetPathName().ToString();

				return meshPath.Equals(path);
			});

			if (*findShapeData)
			{
				const FGsSchemaNpcData** findNpcData = npcInfoList.FindByPredicate([&](const FGsSchemaNpcData* npcData)->bool {

					const FGsSchemaShapeData* rowShapeData = npcData->shapeId.GetRow();
					return  nullptr != rowShapeData && (*findShapeData)->id == rowShapeData->id;
				});

				if (*findNpcData)
				{
					npc->_spawnInfo.TargetID = (*findNpcData)->id;
				}
			}
		}
	}
}

void UGsTerrEdRecoveryUtil::SetAllNpcRandomLocation()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	TArray<AActor*> npcList;

	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryNpc::StaticClass(), npcList);

	for (AActor* actor : npcList)
	{
		if (nullptr == actor)
		{
			continue;
		}

		AGsTerritoryNpc* npc = Cast<AGsTerritoryNpc>(actor);
		if (nullptr == npc)
		{
			continue;
		}

		if (nullptr == npc->_zone)
		{
			continue;
		}

		int32 degree = FMath::RandRange(0, 360);
		FVector direction = FVector::ForwardVector * FMath::RandRange(0.0f, npc->_zone->_radius);
		FVector relativeLocation = direction.RotateAngleAxis(degree, FVector::UpVector);

		relativeLocation.Z = 0;

		npc->SetTerritoryRelativeLocation(relativeLocation);
		npc->SetActorLocation(npc->_zone->GetActorLocation() + relativeLocation);
	}
}

void UGsTerrEdRecoveryUtil::RecoveryTerritoryActorsLabel()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	ULevel* level = world->GetCurrentLevel();
	if (nullptr == level)
	{
		return;
	}

	if (false == UGsTerritoryLevelUtil::IsServerLevel(level->GetPathName()))
	{
		return;
	}

	FString levelName = UGsTerritoryLevelUtil::GetLevelName(level);
	FString persistantLevelName;
	FString levelKey;	
	if (false == UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
	{
		return;
	}

	TArray<AActor*> actorArray = level->Actors;
	for (AActor* actor : actorArray)
	{
		if (nullptr == actor)
		{
			continue;
		}
		
		if (AGsTerritoryActor* territoryActor = Cast<AGsTerritoryActor>(actor))
		{
			FGsTerritoryUniqueIDManager::InvalidTerritoryActorName(levelKey, territoryActor);
		}
	}
}

void UGsTerrEdRecoveryUtil::RecoveryRTTFromTerritoryDataTable()
{
#if SAVE_RTT
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	ULevel* level = world->GetCurrentLevel();
	if (nullptr == level)
	{
		return;
	}

	if (false == UGsTerritoryLevelUtil::IsServerLevel(level->GetPathName()))
	{
		return;
	}

	FString levelName = UGsTerritoryLevelUtil::GetLevelName(level);
	FString persistantLevelName;
	FString levelKey;

	UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey);

	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(levelKey);
	UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	UGsTable* territoryTable = tableManager.GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == territoryTable)
	{
		return;
	}

	const FGsSchemaMapTerritoryData* row;
	FGsSchemaMapTerritoryData newMapData;
	territoryTable->FindRow<FGsSchemaMapTerritoryData>(*levelKey, row);
	if (nullptr == row)
	{
		return;
	}

	FGsTerritoryMapSpot rttData;
	rttData._mapId = FCString::Atoi(*row->RTT.mapId.GetRow()->GetRowName().ToString());
	rttData._spotId = row->RTT.spotIndex;

	UGsTerritoryGroundActionUtility::EditRTT(rttData);
#endif
}

void UGsTerrEdRecoveryUtil::RecoveryWarpSourceIndex()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	ULevel* level = world->GetCurrentLevel();
	if (nullptr == level)
	{
		return;
	}

	if (false == UGsTerritoryLevelUtil::IsServerLevel(level->GetPathName()))
	{
		return;
	}

	TArray<FString> actorList;

	TArray<AActor*>& actorArray = level->Actors;
	for (AActor* actor : actorArray)
	{
		if (actor)
		{
			if (AGsTerritoryWarpSpot* territoryActor = Cast<AGsTerritoryWarpSpot>(actor))
			{
				if (territoryActor->InvalidSpotIndex())
				{
					actorList.Add(territoryActor->GetActorLabel());
				}
			}
		}
	}

	if (actorList.Num() > 0)
	{
		FString log = "Warp source index recovery success\n";
		for (int32 i = 0; i < actorList.Num(); ++i)
		{
			log+= (actorList[i] + "\n");
		}

		FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("TerritoryEditor"), log);
	}	
}

void UGsTerrEdRecoveryUtil::SaveAllMap()
{

}

void UGsTerrEdRecoveryUtil::SaveAllMapRTTFromTable()
{
	UGsTable* mapTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaMapData::StaticStruct());
	if (nullptr == mapTable)
	{
		return;
	}

	TArray<const FGsSchemaMapData*> mapDataList;
	mapTable->GetAllRows(mapDataList);
	TArray<FGsTerrEdMapParam> mapParamList;
	for (const FGsSchemaMapData* mapData : mapDataList)
	{
		int32 id = mapData->id;
		FString persistantLevelName = mapData->areaId.GetRow()->levelName;

		FGsTerrEdMapParam param;
		param._levelName = persistantLevelName;
		param._levelId = id;

		mapParamList.Add(param);
	}

	//save map list

	//open map 

	//open server streaming level

	//save map
}

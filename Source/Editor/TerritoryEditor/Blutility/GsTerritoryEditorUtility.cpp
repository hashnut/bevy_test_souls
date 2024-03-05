//Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritoryEditorUtility.h"

//UE4
#include "Settings/ProjectPackagingSettings.h"
#include "Misc/ConfigCacheIni.h"
#include "HAL/FileManagerGeneric.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Json/Public/Serialization/JsonWriter.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Char.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Slate/Public/Framework/Application/SlateApplication.h"
#include "Runtime/Engine/Classes/Engine/WorldComposition.h"
#include "Runtime/Engine/Classes/Engine/LevelStreaming.h"
#include "Runtime/Core/Public/Modules/ModuleManager.h"
#include "Math/UnrealMathVectorConstants.h"

//T1 Project
#include "UTIL/GsLevelUtil.h"
#include "Level/GsLevelDefine.h"
#include "T1Project.h"
#include "UTIL/GsDir.h"
#include "Runtime/Public/GsTable.h"

#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"

#if TERRITORY_EDITOR_UTIL
#include "DataSchema/Map/MapMarker/GsSchemaMapMarkerInfo.h"
#include "DataSchema/Map/MapMarker/GsSchemaMapData.h"
#endif

//Territory editor
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Actor/Spot/GsTerritorySpot.h"
#include "Util/GsTerritoryEditorFileUtil.h"
#include "TerritoryEditor.h"
#include "Util/GsTerritoryLevelUtil.h"

TMap<int32, TSharedRef<FGsTerritoryGroup>> UGsTerritoryEditorUtility::GetGroupTerrain(TArray<AActor*>& in_array)
{
	TMap<int32, TSharedRef<FGsTerritoryGroup>> groupMap;
	int32 groupID;
	AGsTerritoryZone* zone;

	for (AActor* actor : in_array)
	{
		if (nullptr == actor)
		{
			continue;
		}

		zone = Cast<AGsTerritoryZone>(actor);
		if (nullptr == zone)
		{
			continue;
		}

		groupID = zone->_groupID;

		if (false == groupMap.Contains(groupID))
		{
			groupMap.Add(groupID, MakeShareable(new FGsTerritoryGroup()));
		}

		if (zone)
		{
			TSharedRef<FGsTerritoryGroup> temp = groupMap[groupID];

			temp.Get().TerritoryZoneArray.Add(zone);
		}
	}

	return groupMap;
}

void UGsTerritoryEditorUtility::TestAddPerformance()
{	
	//uint32 max = TNumericLimits<uint32>::Max();

	uint32 max = 9999999;

	FDateTime preTime = FDateTime::Now();	

	_uniqueMap.Empty();

	for (uint32 i = 0; i < max; ++i)
	{
		_uniqueMap.Add(i, i);
	}

	FDateTime endTime = FDateTime::Now();

	double seconds = (endTime - preTime).GetTotalSeconds();

	UE_LOG(LogTemp, Error, TEXT("spend add time : %lf"), seconds);
}

void UGsTerritoryEditorUtility::TestFindPerformance()
{
	//uint32 max = TNumericLimits<uint32>::Max();
	uint32 max = 9999999;

	uint32 rand = FMath::RandRange(0, max);

	FDateTime preTime = FDateTime::Now();

	if (uint32* find = _uniqueMap.Find(rand))
	{

	}

	FDateTime endTime = FDateTime::Now();

	double seconds = (endTime - preTime).GetTotalSeconds();

	UE_LOG(LogTemp, Error, TEXT("spend find key time : %lf"), seconds);
}

void UGsTerritoryEditorUtility::TestGenerateKeyPerformance()
{
	FDateTime preTime = FDateTime::Now();

	TArray<uint32> keyArray;
	keyArray.Reserve(9999999);

	_keyArray.Reserve(10000000);

	_uniqueMap.GenerateKeyArray(keyArray);

	FDateTime endTime = FDateTime::Now();

	double seconds = (endTime - preTime).GetTotalSeconds();	

	for (uint32 i = 0; i < 9999999; ++i)
	{
		_keyArray.Add(i);
	}

	UE_LOG(LogTemp, Error, TEXT("spend generate key time : %lf"), seconds);
}

void UGsTerritoryEditorUtility::TestIterateKeyPerformance()
{
	FDateTime preTime = FDateTime::Now();	
	
	//_keyArray.Reserve(9999999);
	_keyArray.Reset(9999999);

	for (uint32 i = 0; i < 9999999; ++i)
	{
		_keyArray.Add(i);
	}

	FDateTime endTime = FDateTime::Now();
	double seconds = (endTime - preTime).GetTotalSeconds();

	UE_LOG(LogTemp, Error, TEXT("spend iterate key time : %lf"), seconds);
}

#if TERRITORY_EDITOR_UTIL
bool UGsTerritoryEditorUtility::TrySaveMapInfo(FGsSchemaMapData& mapInfo)
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	TArray<AActor*> spawnList;

	UGameplayStatics::GetAllActorsOfClass(world, AGsSpawnMarker::StaticClass(), spawnList);

	for (AActor* marker : spawnList)
	{
		if (marker)
		{
			FGsSchemaMapMarkerInfo info;

			if (AGsSpawnMarker* spawnMarker = Cast<AGsSpawnMarker>(marker))
			{
				info.teamType = spawnMarker->_teamType;
				info.pos = spawnMarker->GetActorLocation();
				info.posRandomRange = spawnMarker->_posRandomRange;

				mapInfo.spawnList.Add(info);
			}
		}
	}

	TArray<AActor*> resserectionList;

	UGameplayStatics::GetAllActorsOfClass(world, AGsResserectionMarker::StaticClass(), resserectionList);

	for (AActor* marker : resserectionList)
	{
		if (marker)
		{
			FGsSchemaMapMarkerInfo info;

			if (AGsResserectionMarker* resserectionMarker = Cast<AGsResserectionMarker>(marker))
			{
				info.teamType = resserectionMarker->_teamType;
				info.pos = resserectionMarker->GetActorLocation();
				info.posRandomRange = resserectionMarker->_posRandomRange;

				mapInfo.resurrectList.Add(info);
			}
		}
	}

	return true;
}
#endif

#if TERRITORY_EDITOR_UTIL
void UGsTerritoryEditorUtility::LoadZoneRSQL(const FGsSchemaTerritoryZone& InZone)
{
	AGsTerritoryZone* zone = nullptr;

	//Create zone

	for (const FGsSchemaTerritoryNpc& spawn : InZone.SpawnInfoList)
	{
		LoadZoneChildRSQL(zone, spawn);
	}	
}

void UGsTerritoryEditorUtility::LoadZoneChildRSQL(AGsTerritoryZone* InZone, const FGsSchemaTerritoryNpc& InSpawn)
{
	AGsTerritoryNpc* npc = nullptr;

	//Create npc

	if (npc)
	{
		InZone->_spawnActors.Add(npc);
	}	
}
#endif

void UGsTerritoryEditorUtility::LoadAllLevels()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	UGsTerritoryLevelUtil::LoadWorkLevels(world, EGsTerritoryLevelType::All);
}

void UGsTerritoryEditorUtility::UnloadAllLevels()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	UGsTerritoryLevelUtil::UnloadWorkLevels(world, EGsTerritoryLevelType::All);
}

void UGsTerritoryEditorUtility::LoadAllServerLevels()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	UGsTerritoryLevelUtil::LoadWorkLevels(world, EGsTerritoryLevelType::Server);
}

void UGsTerritoryEditorUtility::UnloadAllServerLevels()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	UGsTerritoryLevelUtil::UnloadWorkLevels(world, EGsTerritoryLevelType::Server);
}

void UGsTerritoryEditorUtility::VerifyTerrainUniqueKeys()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();

	LoadAllServerLevels();

	FString log;
	FString temp;
	bool isWrong = false;

	if (false == CheckTerrainActorUniqueKeys<AGsTerritoryZone>(world, temp))
	{
		log += temp;
		isWrong = true;
	}

	if (false == CheckTerrainActorUniqueKeys<AGsTerritoryNpc>(world, temp))
	{
		log += temp;
		isWrong = true;
	}

	/*if (false == CheckTerrainActorUniqueKeys<AGsEditorTerrainGather>(GetWorld(), temp))
	{
		log += temp;
		isWrong = true;
	}*/

	//Aler
	if (isWrong)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, log);
	}
	else
	{
		UnloadAllServerLevels();
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define  TERRITORY_EDITOR_FILE_UTIL false

#include "CoreMinimal.h"

//T1 Project

#if TERRITORY_EDITOR_FILE_UTIL
#include "DataSchema/Territory/MapMarker/GsSchemaMapData.h"
#include "DataSchema/Territory/Spawn/GsSchemaSpawnData.h"
#endif

#include "DataSchema/GameObject/Npc/GsSchemaNpcMapData.h"

//Territory edtior
#include "GsTerritoryEditorFileUtil.generated.h"

/**
 *
 */
class UWorld;
class AGsTerritoryZone;

UCLASS()
class TERRITORYEDITOR_API UGsTerritoryEditorFileUtil : public UObject
{
	GENERATED_BODY()


	//spawn data
public:	
	static void SaveSpawnData(ULevel* inLevel, const FString& inFileName);

#if TERRITORY_EDITOR_FILE_UTIL
private:
	static bool TrySaveSpawnData(UWorld* inWorld, FGsSchemaSpawnData& inTerritory);
	static void SaveZone(FGsSchemaSpawnData& inTerritory, const TArray<AGsTerritoryZone*>& inZones);
	static void SaveZoneChild(FGsSchemaTerritoryZone& inRSQLZone, const AGsTerritoryZone* inZone);
#endif

public:
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorFile")
		static void ExportSpawnData();
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorFile")
		static void ImportSpawnData();


	//map marker
public:
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorFile")
		static void SaveMapMarkers(UWorld* inWorld);

#if TERRITORY_EDITOR_FILE_UTIL
private:
	static bool TrySaveMapData(UWorld* inWorld, FGsSchemaMapData& mapInfo);
#endif

public:
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorFile")
		static void ExportMapData();
	UFUNCTION(BlueprintCallable, Category = "TerritoryEditorFile")
		static void ImportMapData();	

	//reigster server level to uncook
public:
	static void RegisterServerLevelDirectorysToNeverCook();
	static void RegisterLevelDirectorysToPakage();
	static bool TryGetAllServerLevelDirectorys(TArray<FString>& OutDirecotrys);
	static bool TryGetUnresisteredServerLevelDirectorysInNeverCook(TArray<FString>& OutPaths, TArray<FString>& InSeverMapPaths);
	static void RegisterServerLevelsToNeverCook(TArray<FString>& InPaths);
	static bool TryGetAllLevelDirectorys(TArray<FString>& OutDirecotrys);
	static bool TryGetUnregisteredLevelDirectorysInPakage(TArray<FString>& OutDirecotrys, TArray<FString>& InMapPaths);
	static void RegisterLevelsToPakage(TArray<FString>& InPaths);
	static TArray<FString> GetRegistedLevelInAreaData(const TArray<FString>& inPathList);

	//find npc dummy data
public:
	static UGsDataTable* FindNpcDummyData();
	static bool TryFindNpcInMap(const class UGsTableMapData* inMapData
	, const struct FGsSchemaNpcData* inNpcData
	, const class UGsTableGuildDungeon* inGuildDungeon
	, const class UGsTableFieldBossData* inFieldBoss
	, OUT FGsSchemaNpcMapData& outNpcMapData);
	static bool TryFindNpcInSpawnData(const class UGsTableSpawnData* inSpawnTable, const struct FGsSchemaNpcData* inNpcData, const struct FGsSchemaGuildDungeon* inGuildDungeon, OUT int32& outCount);
	static bool TryFindNpcInClientSpawnData(const class UGsTableClientSpawnData* inSpawnTable, const struct FGsSchemaNpcData* inNpcData, OUT int32& outCount);
	static bool TryFindNpcInEventSpawnData(const class UGsTableEventSpawnData* inEventSpawnTable
	, const class UGsTableSpawnData* inSpawnTable
	, const struct FGsSchemaNpcData* inNpcData
	, const struct FGsSchemaFieldBossData* inFieldBoss
	, OUT int32& outCount);
	/*static bool TryFindNpcInSpaceCrack(const class UGsTableSpaceCrackData* inSpaceCrack
		, const  struct FGsSchemaNpcData* inNpcData
		, OUT FGsSchemaNpcMapData& outNpcMapData);*/
	static bool TryFindNpcInGuildDungeon(const class UGsTableGuildDungeon* inGuildDungoen
	, const  struct FGsSchemaNpcData* inNpcData	
	, OUT FGsSchemaNpcMapData& outNpcMapData);
	static bool TryFindNpcInFieldBoss(const  class UGsTableFieldBossData* inFieldBoss
	, const  struct FGsSchemaNpcData* inNpcData	
	, const class UGsTableMapData* inMapTable	
	, OUT FGsSchemaNpcMapData& outNpcMapData);
	static const TArray<int32> FindNpcIdArray(const class UGsTableSpawnData* inSpawnTable, int32 inGroupId);
	static void ConvertMapTerritory();

public:
	static void ConvertRTTtoRTTList();
};

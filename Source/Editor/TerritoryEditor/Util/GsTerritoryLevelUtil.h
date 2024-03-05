// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define  TERRITORY_LEVEL_UTIL true

#include "CoreMinimal.h"

//UE4
#include "Editor/WorldBrowser/Public/WorldBrowserModule.h"
#include "Editor/WorldBrowser/Private/LevelCollectionModel.h"
#include "Editor/WorldBrowser/Private/LevelModel.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Selection.h"

//t1 project
#include "ActorComponentEx/WidgetComponentEx.h"

//Territory editor
#include "Actor/Spawn/GsTerritoryActor.h"

#include "GsTerritoryLevelUtil.generated.h"

/**
 *
 */
UENUM()
enum class EGsTerritoryLevelType
{
	Art,
	Design,
	Server,
	All
};

UENUM()
enum class EGsTerritoryActorInfoShow
{
	HideAll,
	SelectShow,
	ShowAll,
};

struct FGsSchemaMapData;
struct FGsSchemaSpawnData;
class AGsTerritoryZone;
class AGsTerritorySpawnActor;
struct FGsSchemaNpcData;
struct FGsSchemaShapeData;

UCLASS()
class TERRITORYEDITOR_API UGsTerritoryLevelUtil : public UObject
{
	GENERATED_BODY()

public:
	static EGsTerritoryActorInfoShow _showTerritoryInfoType;

	//Camera
public:
	static FRotator GetFaceRotationForEditorCamera(const FVector& in_actor);

	//Terrain
public:
	static void LoadWorkLevels(UWorld* in_world, EGsTerritoryLevelType in_tyep = EGsTerritoryLevelType::All);
	static void UnloadWorkLevels(UWorld* in_world, EGsTerritoryLevelType in_tyep = EGsTerritoryLevelType::All);
	static bool TryGetWorkLevels(TWeakPtr<FLevelCollectionModel>& out_levelCollection, TArray<TSharedPtr<FLevelModel>>& out_levelDesignLevelList, UWorld* in_world, EGsTerritoryLevelType in_tyep = EGsTerritoryLevelType::All);
	static bool TryGetLevelCollection(TWeakPtr<FLevelCollectionModel>& out_levels, UWorld* in_world);
	static bool CheckLevelType(const FName& in_level, const FString& in_type);

	//Territory actor
public:	

	template<typename TerritoryActorClass>
	static TArray<TerritoryActorClass*> GetAllTerritoryActor(ULevel* inLevel)
	{
		TArray<TerritoryActorClass*> actorList;

		if (nullptr == inLevel)
		{
			return actorList;
		}

		for (AActor* actor : inLevel->Actors)
		{
			if (actor)
			{
				if (actor->IsA(TerritoryActorClass::StaticClass()))
				{
					actorList.Add(Cast<TerritoryActorClass>(actor));
				}
			}
		}

		return actorList;
	}

	static const TArray<AGsTerritoryZone*> GetAllZones(UWorld* inWorld);
	static const TArray<AGsTerritoryZone*> GetAllZones(UWorld* inWorld, FName inLevel);
	static TArray<AGsTerritoryZone*> GetAllZone(ULevel* inLevel);
	static const bool TryGetAllTerritoryActor(ULevel* inLevel, EGsTerritoryObjType inType, TArray<AGsTerritoryActor*>& outActors);
	static void RemoveTerritorySpawnActor();

	//server level check
public:	
	static bool IsActorInServerLevel(AActor* inActor);
	static bool IsServerLevel(const FString& inPath);
	static bool IsDesignLevel(const FString& inPath);
	static bool IsLevelInServerFolder(const FString& inServerLevel);
	static bool IsLevelInDesignFolder(const FString& inDesignLevel);
	static bool IsLevelInFolder(const FString& inLevelPath, const FString& inFolderName);
	//Get server level nameParseServerLevelName
	static FString GetLevelName(const ULevel* inLevel);
	static FString GetLevelName(const FString& inLevelPath);	
	static FString GetLevelName(const int32 inLevelKey);
	static FString GetServerLevelName(const int32 inLevelKey);
	//Is registered in MapData GsTable
	static bool IsRegisteredLevel(const FString& inLevelKey, const FString& inLevelName);
	static bool IsRegisteredLevel(const FString& inLevelName);
	//parse server level
	static bool IsValidServerLevel(const ULevel* inLevel, FString& outPersistantLevel, FString& outLevelKey);
	static bool IsValidServerLevelName(const FString& inLevelName,  FString& outPersistantLevel, FString& outLevelKey);
	static bool IsValidDesignLevel(const ULevel* inLevel, FString& outPersistantLevel, FString& outLevelKey);
	static bool IsValidDesignLevelName(const FString& inLevelName, FString& outPersistantLevel, FString& outLevelKey);
	static ULevel* GetCurrentServerLevel(UWorld* inWorld, FString& outLevelKey);		
	
public:
	static bool IsSpawnActorOnZone(AActor* inZone, AActor* inSpawnActor);
	static bool IsSpawnActorSpawnOnNavmesh(AActor* inSpawnActor, OUT FVector& outLocation);
	static bool IsLocationOnNavmesh(const FVector& inLocation, OUT FVector& outLocation);
	static bool CheckAllSpawnActorCanSpawnOnNavmesh(TArray<AGsTerritorySpawnActor*>& outActorList);	

	static void SetSelectedLevelActors(const TArray<AActor*>& inActorArray);
	static void SetSelectedLevelActors(const TArray<AActor*>& inActorArray, TArray<TSharedPtr<FString>>& inSelectArray);
	static void DeselectAll();
	static const FGsSchemaNpcData* FindNpcData(int32 inNpcId);
	static const FGsSchemaShapeData* FindShapeDataByNpcId(int32 inNpcId);
	static const FGsSchemaShapeData* FindShapeData(int32 inShapeId);
	static const CreatureSpawnType ConvertCreatureSpawnTypeToTerritorySpawnType(EGsTerritorySpawnType inType);	
};

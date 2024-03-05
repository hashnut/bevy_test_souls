// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
#include "Map/Spawn/GsSchemaSpawnData.h"
#include "GsTerrEdRecoveryUtil.generated.h"

/**
 *
 */
 USTRUCT()
struct TERRITORYEDITOR_API FGsTerrEdMapParam
 {
	 GENERATED_BODY()

 public:
	FString _levelName;
	int32 _levelId;
 };

UCLASS()
class TERRITORYEDITOR_API UGsTerrEdRecoveryUtil : public UObject
{
	GENERATED_BODY()

		//Recovery territory actor using data table
public:
	UFUNCTION(BlueprintCallable, Category = "Territory recovery")
		static void DestroyAllTerritoryActors();
	UFUNCTION(BlueprintCallable, Category = "Territory recovery")
		static void DestroyAllMapMarkers();
	static void CreateTerritoryActorFromDataTable(const FGsSchemaSpawnData* inData);
	static void CreateMapMarkersFromDataTable(const FGsSchemaMapData* inData);
	UFUNCTION(BlueprintCallable, Category = "Territory recovery")
		static void ResolveMultiUniqueID();

public:
	UFUNCTION(BlueprintCallable, Category = "Territory recovery")
		static void RecoveryNpcTidFromMesh();
	UFUNCTION(BlueprintCallable, Category = "Territory recovery")
		static void SetAllNpcRandomLocation();
	UFUNCTION(BlueprintCallable, Category = "Territory recovery")
		static void RecoveryTerritoryActorsLabel();
	UFUNCTION(BlueprintCallable, Category = "Territory recovery")
		static void RecoveryRTTFromTerritoryDataTable();
	UFUNCTION(BlueprintCallable, Category = "Territory recovery")
		static void RecoveryWarpSourceIndex();
	UFUNCTION(BlueprintCallable, Category = "Territory recovery")
		static void SaveAllMap();
	UFUNCTION(BlueprintCallable, Category = "Territory recovery")
		static void SaveAllMapRTTFromTable();

};

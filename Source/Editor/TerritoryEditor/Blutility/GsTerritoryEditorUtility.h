// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define TERRITORY_EDITOR_UTIL false

#include "CoreMinimal.h"

//UE4
#include "Editor/Blutility/Classes/EditorUtilityObject.h"
//#include "Editor/Blutility/Classes/GlobalEditorUtilityBase.h"
#include "Editor/WorldBrowser/Public/WorldBrowserModule.h"
#include "Editor/WorldBrowser/Private/LevelCollectionModel.h"
#include "Editor/WorldBrowser/Private/LevelModel.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

//T1 Project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"

#if TERRITORY_EDITOR_UTIL
#include "DataSchema/Territory/MapMarker/GsSchemaMapData.h"
#endif

//Territory editor
#include "Define/GsTerritorySpawnDefine.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryNpc.h"
//#include "Editor/TerrainEditor/Terrain/Zone/GsEditorTerrainGather.h"

#include "GsTerritoryEditorUtility.generated.h"

/**
 *
 */
class UGsTable;
class AGsTerritoryZone;

const FString LEVEL_DESIGN_FOLDER = TEXT("LevelDesign");

UCLASS(Blueprintable)
class TERRITORYEDITOR_API UGsTerritoryEditorUtility : public UEditorUtilityObject
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UDataTable* _mapTable;

public: 
	//levels controller		
	UFUNCTION(BlueprintCallable, Category = "Terrain organize")
		void LoadAllLevels();
	UFUNCTION(BlueprintCallable, Category = "Terrain editor")
		void UnloadAllLevels();
	UFUNCTION(BlueprintCallable, Category = "Terrain editor")
		void LoadAllServerLevels();
	UFUNCTION(BlueprintCallable, Category = "Terrain editor")
		void UnloadAllServerLevels();

	//Verify unique key
	UFUNCTION(Exec, BlueprintCallable, Category = "Terrain editor")
		void VerifyTerrainUniqueKeys();
	
#if TERRITORY_EDITOR_UTIL
private:
	bool TrySaveMapInfo(FGsSchemaMapData& mapInfo);
private:
	void LoadZoneRSQL(const FGsSchemaTerritoryZone& InZone);
	void LoadZoneChildRSQL(AGsTerritoryZone* InZone, const FGsSchemaTerritoryNpc& InSpawn);
#endif

private:	
	TMap<int32, TSharedRef<FGsTerritoryGroup>> GetGroupTerrain(TArray<AActor*>& in_array);

public:
	UPROPERTY()
	TMap<uint32, uint32> _uniqueMap;
	UPROPERTY()
	TArray<uint32> _keyArray;

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category="test")
		void TestAddPerformance();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "test")
		void TestFindPerformance();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "test")
		void TestGenerateKeyPerformance();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "test")
		void TestIterateKeyPerformance();

	//Check terrain unique keys
private:
	template<typename T = AGsTerritoryActor>
	bool CheckTerrainActorUniqueKeys(UWorld* in_world, FString& in_log)
	{
		if (nullptr == in_world)
		{
			return true;
		}

		TArray<AActor*> actorArray;
		UGameplayStatics::GetAllActorsOfClass(in_world, T::StaticClass(), actorArray);

		if (0 == actorArray.Num())
		{
			return true;
		}

		TMap<int64, AGsTerritoryActor*> actorMap;
		TArray<FGsTerritoryCompareCaution> cationArray;
		FGsTerritoryCompareCaution caution;
		AGsTerritoryActor** find;

		for (AActor* actor : actorArray)
		{
			if (actor)
			{
				T* temp = Cast<T>(actor);

				if (temp)
				{
					int64 uniqueID = temp->GetTerritoryUniqueID();

					if (false == actorMap.Contains(uniqueID))
					{
						actorMap.Add(uniqueID, temp);
					}
					else
					{
						find = actorMap.Find(uniqueID);

						if (find)
						{
							caution.Source = (*find);
							caution.Dest = temp;
							cationArray.Add(caution);
						}
					}
				}
			}
		}

		if (cationArray.Num() > 0)
		{
			for (FGsTerritoryCompareCaution& temp : cationArray)
			{
				in_log += temp.ToString();
			}

			return false;
		}

		return true;
	}
};

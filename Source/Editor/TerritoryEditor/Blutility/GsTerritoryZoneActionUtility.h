// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor/Blutility/Classes/ActorActionUtility.h"

//T1 project
#include "Level/Territory/GsTerritoryZoneDefine.h"

#include "Define/GsTerritoryShapeDefine.h"

//Territory 
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Define/GsTerritorySpawnDefine.h"
#include "Define/GsTerritoryZoneShapeDefine.h"
#include "Blutility/Input/GsTerritorySpawnActorInput.h"
#include "Editor/GsTerritoryPropLayout.h"

#include "GsTerritoryZoneActionUtility.generated.h"

class AGsTerritoryZone;
/**
 *
 */
UCLASS(Blueprintable)
class TERRITORYEDITOR_API UGsTerritoryZoneActionUtility : public UActorActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void SelectAllTerritoryActor();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void DeleteZone();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void CopyZone();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void DeleteAllSpawnActor();

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action", Meta=(SimpleDisplay))
		void AddNpcDataAtLocation(FGsTerritoryNpcInput in_type, int32 inSpawnCount = 1, EGsTerritorySpawnShapeType inShape = EGsTerritorySpawnShapeType::Random);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action", Meta = (SimpleDisplay))
		void TryAddNpcDataInZone(FGsTerritoryNpcInput inNpc, int32 inTrySpawnCount = 100, float inMinDistance = 100);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action", Meta = (SimpleDisplay))
		void AddPropDataAtLocation(FGsTerritoryPropInput in_type, int32 inSpawnCount = 1, EGsTerritorySpawnShapeType inShape = EGsTerritorySpawnShapeType::Random);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void SetAllNpcSpawnLevel(int32 inLevel);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void SetSection(const int32 inSection);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void SetNote(const FString& inNote);

private:
	bool CheckSpawnLocationDistance(const FVector& inLocation, const TArray<FVector>& inSpawnLocationList, float inMinDistance);

protected:
	void AddSpawnDataAtLocation(const FGsTerritorySpawnInfo& inSpawn, int32 inSpawnCount /*= 1*/, EGsTerritorySpawnShapeType inShape /*= EGsTerritorySpawnShapeType::Random*/);

private:
	FVector GetRandomLocationInZone(const FVector& inZoneLocaiton, float inRadius);

private:
	AGsTerritoryZone* GetSelectedTerritoryZone();
	bool IsSelectOnlyOneZone();
	bool IsSelectsZone();
	AActor* CreateZone(EGsTerritoryZoneShapeType in_type, const FVector& in_location);
};

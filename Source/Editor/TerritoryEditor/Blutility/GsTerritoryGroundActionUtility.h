// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//UE4
#include "Editor/Blutility/Classes/ActorActionUtility.h"
#include "Runtime/Landscape/Classes/Landscape.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"

//Territory editor
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Define/GsTerritorySpawnDefine.h"
#include "Input/GsTerritorySpawnActorInput.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Input/GsTerritorySpotInput.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "GsTerritoryGroundActionUtility.generated.h"

/**
 *
 */
class AGsTerrEdRTTActor;

UCLASS(Blueprintable)
class TERRITORYEDITOR_API UGsTerritoryGroundActionUtility : public UActorActionUtility
{
	GENERATED_BODY()

	//zone
public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddZone(NpcSpawnGroupType inSpawnGroupType
			, bool inByClient = false
			, bool inByDefault = true
			, EGsTerritoryZoneShapeType inShape = EGsTerritoryZoneShapeType::Circle);
public:
	static UClass* GetZoneTemplate(EGsTerritoryZoneShapeType inShape);

	//spot
public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Spot")
		void AddSpotNormal(FText inName, float inRandomSpawnRange = 100);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Spot")
		void AddSpotSpawn(FText inName, IffTeamType inTeamType = IffTeamType::NEUTRAL, float inRandomSpawnRange = 100);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Spot")
		void AddSpotResurrection(FText inName, IffTeamType inTeamType = IffTeamType::NEUTRAL, float inRandomSpawnRange = 100);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Spot")
		void AddSpotTeleport(FText inName, IffTeamType inTeamType = IffTeamType::NEUTRAL, float inRandomSpawnRange = 100);
	
		//warp
public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddSpotWarp(FGsTerritoryMapWarp inInput, WarpType inWarpType, float inRadius = 100.0f, int32 inShapeID = 8282, int32 inGroupId = 0);

	//line
public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddLinePatrolLine(float inRot = 0, float inDist = 1000, bool inReturn = false, bool inRepeat = false, float inReatDelay = 0, bool inIsSuicide = false);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddLinePatrolCircle(float inRadius = 1000, bool inReturn = false, bool inRepeat = false, float inReatDelay = 0, bool inIsSuicide = false);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddLinePatrolPolygon(bool inReturn, bool inRepeat, float inReatDelay, bool inIsSuicide);

private:
	class AGsTerritoryLinePatrol* CreatePatrolLine(bool inReturn, bool inRepeat, float inReatDelay, bool inIsSuicide);
	
	//RTT
public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Spot", meta = (ToolTip = "ReturnToTown"))
		static void EditRTT(FGsTerritoryMapSpot inSpot);
	static AGsTerrEdRTTActor* CreateRTT();

private:
	void SaveRTT(const FString& inLevelKey, const FGsTerritoryMapSpot& inRtt);

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceTown(FText inName, FGsSchemaSoundResDataRow inSound);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceTownChaos(FText inName, FGsSchemaSoundResDataRow inSound);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceSafeZone(FText inName, FGsSchemaSoundResDataRow inSound);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceArena(FText inName, FGsSchemaSoundResDataRow inSound);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceArenaReady(FText inName, FGsSchemaSoundResDataRow inSound);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceArrival(FText inName, FGsSchemaSoundResDataRow inSound);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceSound(FText inName, TArray<FGsQuestSound> inQuestSoundArray);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceEnv(FText inName, TArray<FGsEnvCondition> inEnvConditionArray);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceSpaceCrack(FText inName, EGsEnvEvent inEnvEvent);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceNoCampZone(FText inName);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory ground action")
		void AddFenceNonPK(FText inName);

public:
	static bool IsCorrectSelect();
};

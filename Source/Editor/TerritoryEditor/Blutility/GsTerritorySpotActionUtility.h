// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "ActorActionUtility.h"
#include "Input/GsTerritorySpotInput.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "GsTerritorySpotActionUtility.generated.h"

/**
 * 
 */

class AGsTerritorySpot;

UCLASS()
class TERRITORYEDITOR_API UGsTerritorySpotActionUtility : public UActorActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void Delete();
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		virtual void ModifyIndex(int64 inIndex);	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void ModifySpotName(FText inName);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void ChangeToNormalSpot(FText inName, float inRandomSpawnRange = 0);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void ChangeToSpawnSpot(FText inName, IffTeamType inTeamType = IffTeamType::NEUTRAL, float inRandomSpawnRange = 0);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void ChangeToResurrectSpot(FText inName, IffTeamType inTeamType = IffTeamType::NEUTRAL, float inRandomSpawnRange = 0);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void ChangeToTeleportSpot(FText inName, IffTeamType inTeamType = IffTeamType::NEUTRAL, float inRandomSpawnRange = 0);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action")
		void ChangeToWarp(FGsTerritoryMapWarp inInput, WarpType inWarpType, float inRadius = 0, int32 inShapeID = 8282);

private:
	AGsTerritorySpot* GetSelectedSpot();
};

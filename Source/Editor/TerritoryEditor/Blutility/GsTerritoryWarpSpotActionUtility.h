// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritorySpotActionUtility.h"
#include "ActorActionUtility.h"
#include "Input/GsTerritorySpotInput.h"
#include "GsTerritoryWarpSpotActionUtility.generated.h"

/**
 * 
 */
UCLASS()
class TERRITORYEDITOR_API UGsTerritoryWarpSpotActionUtility : public UGsTerritorySpotActionUtility
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action", meta=(ShortToolTip = "Modify warp dest spot"))
		void ModifyDestSpot(FGsTerritoryMapWarp inInput);
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action", meta = (ShortToolTip = "Modify warp dest spot"))
		void ChangeShapeId(FGsTerritoryShapeId inShapeId);	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Territory zone action", meta = (ShortToolTip = "Modify warp dest spot"))
		void ChangeWarpType(WarpType inWarpType);
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Territory editor
#include "GsTerritorySpotInput.generated.h"

/**
 * 
 */
 USTRUCT(BlueprintType)
struct FGsTerritoryMapWarp
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warp")
		int32 DestMapID;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warp")
		int64 SpotIndex;	
};

 USTRUCT(BlueprintType)
	 struct FGsTerritoryMapSpot
 {
	 GENERATED_BODY()

 public:
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warp")
		 int32 _mapId;
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warp")
		 int64 _spotId;
 };

 USTRUCT(BlueprintType)
	 struct FGsTerritoryCurrentMapSpot
 {
	 GENERATED_BODY()

 public:	 
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warp")
		 int64 _spotId;
 };

 USTRUCT(BlueprintType)
	 struct FGsTerritoryShapeId
 {
	 GENERATED_BODY()

 public:
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warp")
		 int32 TargetID;
 };

 USTRUCT(BlueprintType)
	 struct FGsTerritoryLineId
 {
	 GENERATED_BODY()

 public:
	 UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Warp")
		 int64 LineID;
 };
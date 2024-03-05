// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "GsTerritoryZoneShapeDefine.generated.h"
/**
 *
 */

const int32 INVALIDE_INDEX = -1;

USTRUCT(BlueprintType)
struct FGsTerritoryZoneShapeData
{
	GENERATED_BODY()

public:
	UPROPERTY()
		TArray<FVector> PointArray;
	UPROPERTY()
		float Radius;

public:
	virtual ~FGsTerritoryZoneShapeData() {}
};

USTRUCT(BlueprintType)
struct FGsTerritoryZoneCircleShapeData : public FGsTerritoryZoneShapeData
{
	GENERATED_BODY()

public:
	float Radius;

public:
	virtual ~FGsTerritoryZoneCircleShapeData() {}
};

USTRUCT(BlueprintType)
struct FGsTerritoryZonePolygonShapeData : public FGsTerritoryZoneShapeData
{
	GENERATED_BODY()

public:	
	virtual ~FGsTerritoryZonePolygonShapeData() {}
};
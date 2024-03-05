// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//UE4
#include "Runtime/Engine/Classes/Engine/Polys.h"

//T1 Project


//Territory editor
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Define/GsTerritoryZoneShapeDefine.h"

#include "GsTerritoryZonePolygonShape.generated.h"

const FString POLYGON_TEXT = TEXT("polygon");

//----------------------------------------------------------------
// Scene 내 닫힌 폴리곤 영역을 보여주는 액터
//----------------------------------------------------------------
UCLASS(Blueprintable)
class TERRITORYEDITOR_API AGsTerritoryZonePolygonShape : public AGsTerritoryZoneShape
{
	GENERATED_BODY()

public:
	/*UPROPERTY()
		FGsTerritoryZonePolygonData _shape;*/

public:
	UFUNCTION(BlueprintCallable, Category = "Game service eidtor terrain polygon")
	virtual void InitZoneShape() override;
	UFUNCTION(BlueprintCallable, Category = "Game service eidtor terrain polygon")
	virtual void DrawZoneShape(const FTransform& in_transform) override;

public:
	virtual FString GetZoneShapeTypeString() const override { return POLYGON_TEXT; }
	virtual EGsTerritoryZoneShapeType GetZoneShapeType() const override { return EGsTerritoryZoneShapeType::Polygon; }
	virtual bool IsInvalidePointNum(int32 in_num) const override;
	virtual void GetDefaultPoints(TArray<FVector>& out_pointArray) const override;

public:
	bool TryGetTriangleArray(TArray<FPoly>& out_polyArray) const;
};

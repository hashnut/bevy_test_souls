// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Territory editor
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Define/GsTerritoryZoneShapeDefine.h"
#include "GsTerritoryZoneCircleShape.generated.h"

const FString CIRCLE_TEXT = TEXT("circle");

//----------------------------------------------------------------
// Scene 내 닫힌 원 영역을 보여주는 액터
//----------------------------------------------------------------
UCLASS(Blueprintable)
class TERRITORYEDITOR_API AGsTerritoryZoneCircleShape : public AGsTerritoryZoneShape
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Territory editor", meta = (EditCondition = "_bCircle"))
		float _radius = DEFAULT_TERRITORY_FENCE_DISTANCE;	

public:
	UFUNCTION(BlueprintCallable, Category = "Territory editor")
	virtual void InitZoneShape() override;
	UFUNCTION(BlueprintCallable, Category = "Territory editor")
	virtual void DrawZoneShape(const FTransform& in_transform, const TArray<FVector>& pointArray, float in_radius) override;

protected:
	virtual void DrawZoneShape(const FTransform& in_transform) override;

public://get set
	virtual FString GetZoneShapeTypeString() const override { return CIRCLE_TEXT; }
	virtual EGsTerritoryZoneShapeType GetZoneShapeType() const override { return EGsTerritoryZoneShapeType::Circle; }
	float GetRadius() const;
	virtual bool IsInvalidePointNum(int32 in_num) const override;
	virtual void GetDefaultPoints(TArray<FVector>& out_pointArray) const override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryShape.h"
#include "Map/GsTerritoryShapeType.h"
#include "GsTerritoryShapeCircle.generated.h"

UCLASS(Blueprintable)
class TERRITORYEDITOR_API AGsTerritoryShapeCircle : public AGsTerritoryShape
{
	GENERATED_BODY()

public:
	virtual bool IsInvalidePointNum(int32 in_num) override;
	virtual EGsTerritoryShapeType GetShapeType() const override { return EGsTerritoryShapeType::Circle; }
};

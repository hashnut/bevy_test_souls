// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerrainShapeDefine.generated.h"

UENUM(BlueprintType)
enum class ETerrainShapeType : uint8
{
	Polygon,
	Circle,
	Line,
	Max,
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

const float DEFAULT_TERRITORY_FENCE_DISTANCE = 1000.0f;
const float DEFAULT_TERRITORY_FENCE_HEIGHT = 100.0f;

const int32 DEFAULT_TERRITORY_LINE_POINT_NUM = 2;
const int32 DEFAULT_TERRITORY_POLYGON_POINT_NUM = 3;
const int32 DEFAULT_TERRITORY_CIRCLE_POINT_NUM = 8;
const int32 DEFAULT_TERRITORY_SQUARE_POINT_NUM = 4;
const int32 MAX_FENCE_POINT_NUM = 16;

const FString INVALID_FENCE_NUM = TEXT("Fence is must lower than 16");
const FString FENCE_IS_CONCAVE = TEXT("Fence is concave, it must be not concave");
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataSchema/GsSchemaEnums.h"

/**
 * 꺾은선 그래프 데이터
 */
struct T1PROJECT_API FGsLineGraphData final
{
public:
	int32 _XCount = 0;
	FVector2D _graphSize;

	EGsStatisticsValueType _valueType;
	TArray<uint64> _valueList; // 7개 고정값	

	TArray<FVector2D> _gridList; // grid 그리기 위한 것
	TArray<FVector2D> _pointList; // point 위치	
	

public:
	FGsLineGraphData() = default;
	~FGsLineGraphData() = default;

public:
	void CalcPointPos();
	void GetMinMaxValue(OUT uint64& OutMin, OUT uint64& OutMax);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "DataSchema/Statistics/GsSchemaStatisticsData.h"

/**
 * 통계 시스템 데이터
 */
class T1PROJECT_API FGsGameStatisticsData final
{
public:
	StatisticsId _id = INVALID_STATISTICS_ID;
	uint64 _value = 0;
	uint64 _lastWeekValue = 0;

	const FGsSchemaStatisticsData* _table = nullptr;

public:
	FGsGameStatisticsData() = default;
	~FGsGameStatisticsData() = default;

public:
	void ResetValue();
	void SetTable(const FGsSchemaStatisticsData* InTable);

	void SetData(const StatisticsInfo& InData);

	bool IsSameLastWeek() const;
};

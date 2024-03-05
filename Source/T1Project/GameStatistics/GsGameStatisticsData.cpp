// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameStatisticsData.h"


void FGsGameStatisticsData::ResetValue()
{
	_value = 0;
	_lastWeekValue = 0;
}

void FGsGameStatisticsData::SetTable(const FGsSchemaStatisticsData* InTable)
{
	_id = InTable->id;
	_table = InTable;
}

void FGsGameStatisticsData::SetData(const StatisticsInfo& InData)
{
	_value = InData.mThisWeekValue;
	_lastWeekValue = InData.mLastWeekValue;
}

bool FGsGameStatisticsData::IsSameLastWeek() const
{
	return (_value == _lastWeekValue) ? true : false;
}

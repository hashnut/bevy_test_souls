// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUILineGraph.h"
#include "UI/UIContent/Window/GameStatistics/GsUILineGraphLines.h"
#include "UI/UIContent/Window/GameStatistics/GsUILineGraphPoints.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GameStatistics/GsGameStatisticsData.h"
#include "Management/ScopeGame/GsGameStatisticsManager.h"


void UGsUILineGraph::SetData(TArray<int32> InValueList, EGsStatisticsValueType InValueType)
{
	// 주의: 테스트용 함수
	_data._XCount = DAY_OF_THE_WEEK_COUNT;
	_data._graphSize = _graphSize;

	_data._valueType = InValueType;
	
	for (int i = 0; i < DAY_OF_THE_WEEK_COUNT; ++i)
	{
		if (i < InValueList.Num())
		{
			_data._valueList.Emplace(InValueList[i]);
		}
		else
		{
			_data._valueList.Emplace(0);
		}
	}
	
	_data.CalcPointPos();

	_uiLines->SetData(&_data);
	_uiPoints->SetData(&_data);
}

void UGsUILineGraph::SetData(const FGsGameStatisticsData* InData)
{
	if (nullptr == InData ||
		nullptr == InData->_table)
	{
		return;
	}

	// 기본세팅
	_data._XCount = DAY_OF_THE_WEEK_COUNT;
	_data._graphSize = _graphSize;

	// 값 세팅
	_data._valueType = InData->_table->valueType;
	_data._valueList.Empty();
	GSGameStatistics()->GetDailyDataList(InData->_id, _data._valueList);

	// point 위치 연산
	_data.CalcPointPos();

	// 값 세팅
	_uiLines->SetData(&_data);
	_uiPoints->SetData(&_data);
}

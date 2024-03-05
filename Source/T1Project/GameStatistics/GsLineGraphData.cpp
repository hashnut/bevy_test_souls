// Fill out your copyright notice in the Description page of Project Settings.


#include "GsLineGraphData.h"


void FGsLineGraphData::CalcPointPos()
{	
	float XSpace = _graphSize.X / _XCount;
	float offsetX = XSpace * 0.5f;			

	_gridList.Empty();
	_gridList.Emplace(FVector2D(0, 0));
	_gridList.Emplace(FVector2D(0, _graphSize.Y));
	_gridList.Emplace(_graphSize);
	
	// Key: min, Value: max
	uint64 minValue = 0;
	uint64 maxValue = 0;
	GetMinMaxValue(minValue, maxValue);
	
	// 모두 같은 값일 경우 무조건 중앙에 그림
	if (minValue == maxValue)
	{
		_pointList.Empty();
		for (int i = 0; i < _XCount; ++i)
		{
			// 좌하단(0,0) 보이도록 그리지만, 실제 그려지는건 좌상단(0,0) 기준 좌표임에 유의
			FVector2D point;
			point.X = i * XSpace + offsetX;
			point.Y = _graphSize.Y * 0.5f;

			_pointList.Emplace(point);
		}

		return;
	}

	const float offsetRate = 0.2f;
	float offsetY = _graphSize.Y * offsetRate;

	// 위아래 offset 뺀 영역이 실제 그려질 영역임
	double drawSizeY = _graphSize.Y - (offsetY * 2);
	double valueSize = static_cast<double>(maxValue - minValue);

	_pointList.Empty();
	for (int i = 0; i < _XCount; ++i)
	{
		// 좌하단(0,0) 보이도록 그리지만, 실제 그려지는건 좌상단(0,0) 기준 좌표임에 유의
		FVector2D point;
		point.X = i * XSpace + offsetX;

		double normalizedValue = static_cast<double>(_valueList[i] - minValue) / valueSize;

		point.Y = FMath::Clamp(
			static_cast<float>(_graphSize.Y - offsetY - (normalizedValue * drawSizeY)),
			0.f, _graphSize.Y);

		_pointList.Emplace(point);
	}	
}

void FGsLineGraphData::GetMinMaxValue(OUT uint64& OutMin, OUT uint64& OutMax)
{
	OutMin = UINT64_MAX;
	OutMax = 0;

	for (int i = 0; i < _XCount; ++i)
	{
		if (_valueList[i] < OutMin)
		{
			OutMin = _valueList[i];
		}

		if (_valueList[i] > OutMax)
		{
			OutMax = _valueList[i];
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "T1Project.h"
/*
 * FGsIntervalChecker
 * : 주어진 시간이 지났는지 체크하기 위한 용도
 * - n초간 패킷 요청 막기, 클릭막기 등에 사용 위함
 */
struct T1PROJECT_API FGsIntervalChecker
{
private:
	float _intervalSecond = 0.f;
	double _endTimeApp = 0;
	 
public:
	FGsIntervalChecker()
	{
		SetIntervalSecond(0.f);
		_endTimeApp = 0;
	}

	explicit FGsIntervalChecker(float InIntervalSecond)
	{
		SetIntervalSecond(InIntervalSecond);
		_endTimeApp = 0;
	}

	void SetIntervalSecond(float InSecond) { _intervalSecond = InSecond; }

	void Reset()
	{
		_endTimeApp = 0;
	}

	void UpdateTime()
	{
		SetEndTime(_intervalSecond);
	}

	bool IsTimeOver() const
	{
		double currTime = FApp::GetCurrentTime();
		return (_endTimeApp <= currTime) ? true : false;
	}

	float GetRemainTime() const
	{
		double remainSeconds = _endTimeApp - FApp::GetCurrentTime();
		return (0.f < remainSeconds) ? static_cast<float>(remainSeconds) : 0.f;
	}
	
	void SetEndTime(float InRemainSecond)
	{
		double currTime = FApp::GetCurrentTime();		
		_endTimeApp = currTime + static_cast<double>(InRemainSecond);
	}
};

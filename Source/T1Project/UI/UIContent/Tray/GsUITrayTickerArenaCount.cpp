// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerArenaCount.h"

#include "Components/TextBlock.h"

#include "UTIL/GsTimeSyncUtil.h"


void UGsUITrayTickerArenaCount::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 남은 시간
	time_t serverNowSecond = FGsTimeSyncUtil::GetServerNowSecond();
	time_t remainTime = _endTime - serverNowSecond;
	if (_remainCount > remainTime)
	{
		_remainCount = (remainTime > _maxCount) ? _maxCount : remainTime;
		OnExitAnimation();
	}
}

void UGsUITrayTickerArenaCount::OnExitAnimation()
{
	if (_remainCount > 0)
	{
		_textBlockTime->SetText(FText::AsNumber(_remainCount));
		PlayCountAnimation();
	}
	else
	{
		Super::OnExitAnimation();
	}
}

void UGsUITrayTickerArenaCount::SetCount(time_t InStartTime, int32 InCount)
{
	_startTime = InStartTime;
	_endTime = InStartTime + InCount;
	time_t serverNowSecond = FGsTimeSyncUtil::GetServerNowSecond() + 1;
	time_t remainTime = _endTime - serverNowSecond;
	_maxCount = InCount;
	_remainCount = (remainTime > InCount) ? InCount : remainTime;
	OnExitAnimation();
}
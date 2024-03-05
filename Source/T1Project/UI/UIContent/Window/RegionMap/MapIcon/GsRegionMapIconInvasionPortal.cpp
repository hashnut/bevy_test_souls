// Fill out your copyright notice in the Description page of Project Settings.


#include "GsRegionMapIconInvasionPortal.h"
#include "UI/UIContent/Common/Icon/GsUIIconRegionMapInvasion.h"
#include "UTIL/GsTimeSyncUtil.h"

void UGsRegionMapIconInvasionPortal::SetUpdateTime()
{
	auto curTime = FGsTimeSyncUtil::GetServerNowDateTime();
	FTimespan remainTime = _endTIme - curTime;

	if (_endTIme.GetTicks() > 0 && remainTime > 0 && _invasionIcon)
	{
		auto remainText = FString::Printf(TEXT("%02d:%02d"), int32(remainTime.GetTotalMinutes()), remainTime.GetSeconds());
		_invasionIcon->SetTimeText(FText::FromString(remainText));
	}
}

void UGsRegionMapIconInvasionPortal::Tick(float InDelta)
{
	Super::Tick(InDelta);

	SetUpdateTime();
}

void UGsRegionMapIconInvasionPortal::SetRemainTime(FDateTime endTime)
{
	_endTIme = endTime;

	_invasionIcon = Cast<UGsUIIconRegionMapInvasion>(_icon);
}

void UGsRegionMapIconInvasionPortal::SetActivePortal(bool isactive)
{
	if (_invasionIcon) _invasionIcon->SetActiveSelect(isactive ? 1 : 0);
}

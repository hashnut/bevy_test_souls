// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDSystemInfo.h"
#include "TimerManager.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Reward/GsRewardHelper.h"



void UGsUIHUDSystemInfo::NativeConstruct()
{
	_tempInterval = _invalidateInterval;
	
	Super::NativeConstruct();
}
	
void UGsUIHUDSystemInfo::InvalidateAllInternal()
{
	InvalidateExp();
	InvalidateSystemInfo();
}

bool UGsUIHUDSystemInfo::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	// _invalidateInterval (10초)에 한번씩 갱신
	_tempInterval -= InDeltaTime;
	if (0.0f > _tempInterval)
	{
		_tempInterval = _invalidateInterval;

		InvalidateSystemInfo();
	}

	return true;
}

void UGsUIHUDSystemInfo::InvalidateExp()
{
	float expRate = 0.f;
	if (FGsGameDataManager* gameDataMgr = GGameData())
	{
		expRate = GGameData()->GetCurrentLevelExpRate();
	}

	SetExpUI(expRate);
}

void UGsUIHUDSystemInfo::InvalidateSystemInfo()
{
	// 현재 시간
	FDateTime now = FDateTime::Now();		
	FString strTime = FString::Printf(TEXT("%02d:%02d"), now.GetHour(), now.GetMinute());
	_textTime = FText::FromString(strTime);

	// 배터리
	SetBatteryUI(GetBatteryLevel());

	// 배터리 충전여부
	SetBatteryChargeState(FPlatformMisc::IsBatteryChargeState());

	// 네트워크
	SetNetworkTypeUIInternal(GetNetworkConnectionType());
}

void UGsUIHUDSystemInfo::SetExpUI(float InRate)
{
	if (_barExp)
	{
		_barExp->SetPercent(InRate);
	}

	if (_textBlockExp)
	{
		// 소수점 5자리 표기
		FString strExp = FString::Printf(TEXT("%.5f%%"), InRate * 100.0f);
		_textBlockExp->SetText(FText::FromString(strExp));
	}
}

void UGsUIHUDSystemInfo::SetBatteryUI(int InBatteryLevel)
{
	bool isLowBattery = (InBatteryLevel < _lowBatteryMax) ? true : false;

	if (_barBattery)
	{
		_barBattery->SetPercent(InBatteryLevel * 0.01f);
		_barBattery->SetFillColorAndOpacity((isLowBattery) ? _colorLowBattery : FLinearColor::White);
	}

	if (_imgBattery)
	{
		_imgBattery->SetColorAndOpacity((isLowBattery) ? _colorLowBattery : FLinearColor::White);
	}
}

void UGsUIHUDSystemInfo::SetBatteryChargeState(bool InBatteryCharge)
{
	if (_imgBatteryCharge)
	{
		_imgBatteryCharge->SetVisibility(InBatteryCharge ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UGsUIHUDSystemInfo::SetNetworkTypeUI(uint8 InType)
{
	SetNetworkTypeUIInternal(static_cast<ENetworkConnectionType>(InType));
}

void UGsUIHUDSystemInfo::SetNetworkTypeUIInternal(ENetworkConnectionType InNetworkType)
{
	switch (InNetworkType)
	{
	case ENetworkConnectionType::WiFi:
		_switcherNetwork->SetActiveWidgetIndex(1);
		break;
	case ENetworkConnectionType::Cell:
		_switcherNetwork->SetActiveWidgetIndex(2);
		break;
	default:
		_switcherNetwork->SetActiveWidgetIndex(0);
		break;
	}
}

int UGsUIHUDSystemInfo::GetBatteryLevel() const
{
	int percent = FPlatformMisc::GetBatteryLevel();
	if (0 > percent || 100 < percent)
	{
		return 100;
	}

	return percent;

	/*
#if PLATFORM_ANDROID
	return FAndroidMisc::GetBatteryLevel();
#elif PLATFORM_IOS
	return FIOSPlatformMisc::GetBatteryLevel();
#elif PLATFORM_WINDOWS
	return FWindowsPlatformMisc::GetBatteryLevel();
#endif
	return 100;
	*/
}

ENetworkConnectionType UGsUIHUDSystemInfo::GetNetworkConnectionType() const
{
	return FPlatformMisc::GetNetworkConnectionType();

	/*
#if PLATFORM_ANDROID
	return FAndroidMisc::GetNetworkConnectionType();
#elif PLATFORM_IOS
	return FIOSPlatformMisc::GetNetworkConnectionType();
#endif
	return ENetworkConnectionType::Ethernet;
	*/
}

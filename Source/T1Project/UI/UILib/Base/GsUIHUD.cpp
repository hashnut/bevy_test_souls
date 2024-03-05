// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUD.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"


bool UGsUIHUD::UpdateManagerTick(float InDeltaTime)
{
	// NativeTick처럼 현재 보이지 않을 경우 틱을 돌지 않도록 처리
	if (false == IsVisible())
	{
		return false;
	}

	IGsInvalidateUIInterface::CheckInvalidate();
	
	return true;
}

void UGsUIHUD::SetHUDMode(EGsUIHUDMode InMode)
{
	if (IsHideMode(InMode))
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

bool UGsUIHUD::IsHideMode(EGsUIHUDMode InMode) const
{
	if (_hideHUDModeList.Contains(EGsUIHUDMode::OBSERVER))
	{
		if (GSObserver() && GSObserver()->GetIsObserveStart())
		{
			return true;
		}
	}

	if (_hideHUDModeList.Contains(EGsUIHUDMode::INVADE))
	{
		if (GGameData() && GGameData()->IsInvadeWorld())
		{
			return true;
		}
	}

	return _hideHUDModeList.Contains(InMode);
}

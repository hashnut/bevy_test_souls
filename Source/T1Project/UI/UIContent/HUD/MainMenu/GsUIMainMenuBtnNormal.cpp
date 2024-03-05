// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMainMenuBtnNormal.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "RedDot/GsRedDotCheckFunc.h"


void UGsUIMainMenuBtnNormal::SetRedDotCheck(IGsRedDotCheck* InLogic, EGsIconRedDotType InType)
{
	_redDotCheck = InLogic;
	_redDotIconType = InType;
}

bool UGsUIMainMenuBtnNormal::UpdateRedDot(bool bUpdate)
{
	bool bIsRedDot = false;
	if (_redDotCheck)
	{
		bIsRedDot = _redDotCheck->CheckRedDot(bUpdate);
		if (_redDot)
		{
			_redDot->SetRedDot(bIsRedDot, _redDotIconType, _redDotCheck->GetRedDotCount());
		}
	}

	if (bIsRedDot)
	{
		return true;
	}
	else
	{
		if (_reinforceCheck)
		{
			bIsRedDot = _reinforceCheck->CheckRedDot(bUpdate);
			if (_redDot)
			{
				_redDot->SetRedDot(bIsRedDot, EGsIconRedDotType::REINFORCE, 0);
			}
		}
	}

	// 강화 레드닷은 없는걸로 친다
	return false;
}

void UGsUIMainMenuBtnNormal::SetContentIsNew(bool bIsNew)
{
	_imgNew->SetVisibility(bIsNew ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIMainMenuBtnNormal::SetMenuOpen(bool bIsOpen)
{
	OnSubMenuOpen(bIsOpen);
}

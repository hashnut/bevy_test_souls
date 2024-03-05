// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMainMenuBtnHamberger.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "RedDot/GsRedDotCheckFunc.h"


void UGsUIMainMenuBtnHamberger::NativeConstruct()
{
	Super::NativeConstruct();

	// 좀더 짧게 변경
	_btnSlot->SetOnClickLockSec(0.1f);
}

void UGsUIMainMenuBtnHamberger::SetRedDotCheck(IGsRedDotCheck* InLogic, EGsIconRedDotType InType)
{
	_redDotCheck = InLogic;
	_redDotIconType = InType;
}

bool UGsUIMainMenuBtnHamberger::UpdateRedDot(bool bUpdate)
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

	return bIsRedDot;
}

void UGsUIMainMenuBtnHamberger::SetMenuOpen(bool bIsOpen)
{
	_btnSlot->SetSwitcherIndex(bIsOpen ? 1 : 0);
}

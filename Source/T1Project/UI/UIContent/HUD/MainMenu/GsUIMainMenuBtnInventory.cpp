// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMainMenuBtnInventory.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "RedDot/GsRedDotCheckFunc.h"
#include "Message/MessageParam/GsInvenMessageParam.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemHelper.h"


void UGsUIMainMenuBtnInventory::SetRedDotCheck(IGsRedDotCheck* InLogic, EGsIconRedDotType InType)
{
	_redDotCheck = InLogic;
	_redDotIconType = InType;
}

bool UGsUIMainMenuBtnInventory::UpdateRedDot(bool bUpdate)
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

void UGsUIMainMenuBtnInventory::UpdateInvenWeight(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsUpdateInvenWeightInfo* param = InParam->Cast<const FGsUpdateInvenWeightInfo>();
	if (nullptr == param)
	{
		return;
	}

	WeightPenaltyGrade penaltyGradeType = GItem()->GetWeightPenaltyGradeType();
	_switcherWeightPenalty->SetActiveWidgetIndex((uint8)penaltyGradeType);

	FText percentText;
	FGsItemHelper::GetWeightPercentText(param->_weightPercent, percentText, false);

	_textBlockLow->SetText(percentText);
	_textBlockMid->SetText(percentText);
	_textBlockHigh->SetText(percentText);
}

void UGsUIMainMenuBtnInventory::SetContentIsNew(bool bIsNew)
{
	_imgNew->SetVisibility(bIsNew ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIMainMenuBtnInventory::SetMenuOpen(bool bIsOpen)
{
	OnSubMenuOpen(bIsOpen);
}

void UGsUIMainMenuBtnInventory::UpdateWeightPenaltyType(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}
	
	const FGsUpdateInvenWeightPenaltyType* param = InParam->Cast<const FGsUpdateInvenWeightPenaltyType>();
	if (nullptr == param)
	{
		return;
	}

	_switcherWeightPenalty->SetActiveWidgetIndex((uint8)param->_gradeType);
}

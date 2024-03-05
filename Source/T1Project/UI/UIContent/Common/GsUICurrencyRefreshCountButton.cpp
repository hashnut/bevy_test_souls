// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICurrencyRefreshCountButton.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Currency/GsCurrencyHelper.h"


void UGsUICurrencyRefreshCountButton::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUICurrencyRefreshCountButton::OnClickSlot);
}

void UGsUICurrencyRefreshCountButton::SetData(int32 InRemainCount, CurrencyType InType, uint64 InAmount)
{
	SetRemainCount(InRemainCount);

	_currencyType = InType;
	SetAmount(InAmount);

	if (false == _btnSlot->GetIsEnabled())
	{
		return;
	}

	if (UPaperSprite* sprite = FGsUIHelper::LoadSpriteFromPath(FGsCurrencyHelper::GetCurrencyIconPath(InType)))
	{
		_imgIcon->SetVisibility(ESlateVisibility::Visible);
		_imgIcon->SetBrushFromAtlasInterface(sprite);
	}
	else
	{
		_imgIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUICurrencyRefreshCountButton::SetRemainCount(int32 InRemainCount)
{
	_remainCount = InRemainCount;
	_textBlockRemainCount->SetText(FText::AsNumber(_remainCount));

	SetEnableButton((0 < _remainCount) ? true : false);
}

void UGsUICurrencyRefreshCountButton::SetEnableButton(bool bInEnable)
{
	_btnSlot->SetIsEnabled(bInEnable);
	_switcherState->SetActiveWidgetIndex(bInEnable ? 1 : 0);
}

void UGsUICurrencyRefreshCountButton::SetAmount(uint64 InAmount)
{
	_amount = InAmount;
	_textBlockAmount->SetText(FText::AsNumber(InAmount));
}

void UGsUICurrencyRefreshCountButton::SetAmountTextColor(EGsUIColorType InColorType)
{
	_textBlockAmount->SetColorAndOpacity(FGsUIColorHelper::GetColor(InColorType));
}

void UGsUICurrencyRefreshCountButton::UpdateAmountTextColorLocalPlayer()
{
	bool bIsEnough = FGsCurrencyHelper::CheckCurrency(_currencyType, _amount, false);

	SetAmountTextColor(bIsEnough ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);
}

void UGsUICurrencyRefreshCountButton::OnClickSlot()
{
	OnClickRefreshButton.ExecuteIfBound();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICurrencyDetailInfo.h"

#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

#include "Currency/GsCurrencyHelper.h"

#include "DataSchema/GsSchemaCurrencyData.h"

#include "UI/UIContent/Helper/GsUIStringHelper.h"


void UGsUICurrencyDetailInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	// 포커싱 가능한 위젯으로 만든다
	bIsFocusable = true;
}

void UGsUICurrencyDetailInfo::NativeConstruct()
{
	Super::NativeConstruct();

	_currencyList = nullptr;
}

void UGsUICurrencyDetailInfo::NativeDestruct()
{
	OnHide.Clear();

	Super::NativeDestruct();
}

FReply UGsUICurrencyDetailInfo::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	OnHide.Broadcast();
	if (_currencyList)
	{
		_currencyList->SetFocus();
	}

	return FReply::Handled();
}

void UGsUICurrencyDetailInfo::SetData(CurrencyType InType)
{
	SetFocus();

	if (const FGsSchemaCurrencyData* currencyData = FGsCurrencyHelper::GetCurrencyData(InType))
	{
		_textBlockName->SetText(FGsCurrencyHelper::GetCurrencyLocalizedText(InType));
		SetAmount(InType);

		_panelAcquire->SetVisibility(currencyData->acq.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
		_textBlockAcquire->SetText(currencyData->acq);

		_panelUsedfor->SetVisibility(currencyData->usedfor.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
		_textBlockUsedfor->SetText(currencyData->usedfor);

		_textBlockDesc->SetText(currencyData->desc);
	}
}

void UGsUICurrencyDetailInfo::SetAmount(CurrencyType InType)
{
	FText textAmount;
	FGsUIStringHelper::GetAmountText(FGsCurrencyHelper::GetCurrencyAmount(InType), 0, textAmount);
	_textBlockAmount->SetText(textAmount);
}

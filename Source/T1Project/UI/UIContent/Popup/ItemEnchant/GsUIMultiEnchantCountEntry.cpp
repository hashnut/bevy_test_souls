// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/ItemEnchant/GsUIMultiEnchantCountEntry.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"



void UGsUIMultiEnchantCountEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOnClick->OnClicked.AddDynamic(this, &UGsUIMultiEnchantCountEntry::OnclickNum);
}

void UGsUIMultiEnchantCountEntry::OnShow()
{
	if (true == OnClickNum.IsBound())
	{
		OnClickNum.Clear();
	}

	_index = 0;
	_stateflag = true;
	_isSelected = false;
}

void UGsUIMultiEnchantCountEntry::OnHide()
{
	if (true == OnClickNum.IsBound())
	{
		OnClickNum.Clear();
	}
}

void UGsUIMultiEnchantCountEntry::SetData(int32 InIndex, bool InIsEnableBtn, bool InIsSelcted)
{
	_stateflag = InIsEnableBtn;
	_isSelected = InIsSelcted;
	_stateSwitcher->SetActiveWidgetIndex(_stateflag ? 1 : 0);
	_index = (uint8)InIndex;
	_numText->SetText(FText::AsNumber(_index));
	_numText1->SetText(FText::AsNumber(_index));

	_selectionImg->SetVisibility(_isSelected ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIMultiEnchantCountEntry::OnclickNum()
{
	if (OnClickNum.IsBound())
	{
		OnClickNum.Broadcast(_index, _stateflag);
	}
}
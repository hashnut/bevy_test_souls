// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/ItemEnchant/GsUIEnchantPropTitleEntry.h"
#include "Components/TextBlock.h"


void UGsUIEnchantPropTitleEntry::OnShow()
{

}

void UGsUIEnchantPropTitleEntry::OnHide()
{

}

void UGsUIEnchantPropTitleEntry::SetTitleText(FText& InText)
{
	_titleNameText->SetText(InText);
}

void UGsUIEnchantPropTitleEntry::SetTitleColor(const FLinearColor& InColor)
{
	_titleNameText->SetColorAndOpacity(InColor);
}
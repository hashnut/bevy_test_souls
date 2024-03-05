// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/BMShop/GsUIBMShopProductTextEntry.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"


void UGsUIBMShopProductTextEntry::OnShow()
{
}

void UGsUIBMShopProductTextEntry::OnHide()
{
}

void UGsUIBMShopProductTextEntry::SetTitleText(const FText& InText)
{
	_titleText->SetText(InText);
}

void UGsUIBMShopProductTextEntry::SetDescText(const FText& InText)
{
	_descRichText->SetText(InText);
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITooltipTitleDesc.h"
#include "Components/TextBlock.h"


void UGsUITooltipTitleDesc::NativeConstruct()
{
	Super::NativeConstruct();

	SetDefaultColor();
}

void UGsUITooltipTitleDesc::SetTitleText(const FText& InText)
{
	_textBlockTitle->SetText(InText);
}

void UGsUITooltipTitleDesc::SetDescText(const FText& InText)
{
	_textBlockDesc->SetText(InText);
}

void UGsUITooltipTitleDesc::SetTitleColor(const FLinearColor& InColor)
{
	_textBlockTitle->SetColorAndOpacity(InColor);
}

void UGsUITooltipTitleDesc::SetDescColor(const FLinearColor& InColor)
{
	_textBlockDesc->SetColorAndOpacity(InColor);
}

void UGsUITooltipTitleDesc::SetDefaultColor()
{
	_textBlockTitle->SetColorAndOpacity(_colorDefaultTitle);
	_textBlockDesc->SetColorAndOpacity(_colorDefaultDesc);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITooltipTitleDescVertical.h"
#include "Components/TextBlock.h"


void UGsUITooltipTitleDescVertical::SetDefaultColor()
{
	SetDescColor(_colorDefaultDesc, _colorDescSecond);
}

void UGsUITooltipTitleDescVertical::SetDescText(const FText& InFirstText, const FText& InSecondText)
{
	Super::SetDescText(InFirstText);

	_textBlockDescSecond->SetText(InSecondText);
}

void UGsUITooltipTitleDescVertical::SetDescColor(const FLinearColor& InFirstColor, const FLinearColor& InSecondColor)
{
	Super::SetDescColor(InFirstColor);

	_textBlockDescSecond->SetColorAndOpacity(InSecondColor);
}
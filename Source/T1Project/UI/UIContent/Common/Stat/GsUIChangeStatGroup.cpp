// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIChangeStatGroup.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"


void UGsUIChangeStatGroup::OnShow()
{
	SetStatGroupUIPanel(false);

	_statName->SetText(FText::FromString(""));
	_currentValue->SetText(FText::FromString(""));
	_expectValue->SetText(FText::FromString(""));
}

void UGsUIChangeStatGroup::OnHide()
{
}

void UGsUIChangeStatGroup::SetStatName(const FText& InStatName)
{
	_statName->SetText(InStatName);
}

void UGsUIChangeStatGroup::SetCurrentStatValue(const FText& InCurValue)
{
	_currentValue->SetText(InCurValue);
}

void UGsUIChangeStatGroup::SetExpectStatValue(const FText& InExpectValue)
{
	_expectValue->SetText(InExpectValue);
}

void UGsUIChangeStatGroup::SetStatGroupUIPanel(bool InActive, bool InIsRefineOption)
{
	_panel->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	_arrowImage->SetVisibility(true == InIsRefineOption ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	_currentValue->SetVisibility(true == InIsRefineOption ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UGsUIChangeStatGroup::SetStatNameColor(const FLinearColor& InColor)
{
	_statName->SetColorAndOpacity(InColor);
}

void UGsUIChangeStatGroup::SetExpectValueColor(const FLinearColor& InColor)
{
	_expectValue->SetColorAndOpacity(InColor);
}
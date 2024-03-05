// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/ItemEnchant/GsUIChangeItemPropertyEntry.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"



void UGsUIChangeItemPropertyEntry::OnShow()
{
}

void UGsUIChangeItemPropertyEntry::OnHide()
{
}

// 공격력 30 (강화 +8) +8강 성공시 획득가능
void UGsUIChangeItemPropertyEntry::SetDataText(FText InStatNameText, FText InStatValueText)
{
	_statNameText->SetText(InStatNameText);
	_statValueText->SetText(InStatValueText);
}

void UGsUIChangeItemPropertyEntry::SetChangeTextColor(const FLinearColor& InColor)
{
	_statNameText->SetColorAndOpacity(InColor);
	_statValueText->SetColorAndOpacity(InColor);
}

void UGsUIChangeItemPropertyEntry::SetExpectStatText(FText InExpectText)
{
	_expectGainText->SetText(InExpectText);
}

void UGsUIChangeItemPropertyEntry::SetExpectTextVisible(const bool InState)
{
	_expectGainText->SetVisibility(true == InState ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIChangeItemPropertyEntry::SetChangeExpectTextColor(bool InIsCursed)
{
	_expectGainText->SetColorAndOpacity(FGsUIColorHelper::GetColor(true == InIsCursed ?
		EGsUIColorType::ENCHANT_CURSED_EXPECT_VALUE : EGsUIColorType::ENCHANT_EXPECT_GAIN_OPTION));
}

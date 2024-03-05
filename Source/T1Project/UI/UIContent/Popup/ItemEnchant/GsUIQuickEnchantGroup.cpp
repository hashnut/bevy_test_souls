// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIQuickEnchantGroup.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

void UGsUIQuickEnchantGroup::NativeConstruct()
{
	Super::NativeConstruct();

	_warnTextRootPanel->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIQuickEnchantGroup::NativeDestruct()
{

	Super::NativeDestruct();
}

void UGsUIQuickEnchantGroup::SetEnchantGoalNumText(const FText InText, const bool InIsUseWarningColor)
{
	_enchantNumText->SetText(InText);
	_enchantNumText->SetColorAndOpacity(FGsUIColorHelper::GetColor(true == InIsUseWarningColor ? 
		EGsUIColorType::ENCHANT_WARNING_DESTROY : EGsUIColorType::ENCHANT_WARNING_NORMAL));
}

void UGsUIQuickEnchantGroup::SetEnchantWarnText(const FText InWarnText, const bool InIsUseWarningColor)
{
	_enchantWarnText->SetText(InWarnText);
	_enchantWarnText->SetColorAndOpacity(FGsUIColorHelper::GetColor(true == InIsUseWarningColor ?
		EGsUIColorType::ENCHANT_WARNING_DESTROY : EGsUIColorType::ENCHANT_WARNING_NORMAL));
}

void UGsUIQuickEnchantGroup::SetWarnTextColor(const EGsUIColorType InColorType)
{
	_enchantWarnText->SetColorAndOpacity(FGsUIColorHelper::GetColor(InColorType));
}

void UGsUIQuickEnchantGroup::SetEnchantGoalNumTextColor(const EGsUIColorType InColorType)
{
	_enchantNumText->SetColorAndOpacity(FGsUIColorHelper::GetColor(InColorType));
}

void UGsUIQuickEnchantGroup::SetEnchantWarnTextVisibility(const bool InState)
{
	_warnTextRootPanel->SetVisibility(true == InState ? ESlateVisibility::SelfHitTestInvisible 
		: ESlateVisibility::Collapsed);
}
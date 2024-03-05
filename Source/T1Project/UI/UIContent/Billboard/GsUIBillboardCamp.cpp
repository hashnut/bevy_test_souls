// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Billboard/GsUIBillboardCamp.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/GsUIGuildIcon.h"


void UGsUIBillboardCamp::NativeConstruct()
{
	Super::NativeConstruct();

	//_hpProgressBar->SetPercent(0.f);
}

void UGsUIBillboardCamp::NativeDestruct()
{
	_hpProgressBar->SetPercent(0.f);

	Super::NativeDestruct();
}

void UGsUIBillboardCamp::SetCampNameText(FText InText)
{
	_campNameText->SetText(InText);
}

void UGsUIBillboardCamp::SetProgressBarValue(const float InValue)
{
	_hpProgressBar->SetPercent(InValue);
}

float UGsUIBillboardCamp::GetCurProgressBarValue()
{
	return _hpProgressBar->Percent;
}

void UGsUIBillboardCamp::ShowHPBar(bool In_isShow)
{
	ESlateVisibility hpBarVisibility =
		(In_isShow == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
	_hpProgressBar->SetVisibility(hpBarVisibility);
}

void UGsUIBillboardCamp::SetCampEmblemIcon(const uint8 InEmblemId)
{
	_guildIcon->SetEmblemId(InEmblemId, false);
}

void UGsUIBillboardCamp::SetSanctumNameText(const FText& InText)
{
	_textBlockSanctum->SetVisibility(InText.IsEmpty() ? 
		ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	_textBlockSanctum->SetText(InText);
}

void UGsUIBillboardCamp::ShowNamePanel(bool InShow)
{
	_panelRootName->SetVisibility(InShow ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

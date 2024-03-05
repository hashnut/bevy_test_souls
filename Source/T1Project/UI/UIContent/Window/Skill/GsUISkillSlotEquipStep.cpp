// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillSlotEquipStep.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUISkillSlotEquipStep::NativeConstruct()
{
	Super::NativeConstruct();

	_btnSlot->SetVisibility(ESlateVisibility::Visible);
}

void UGsUISkillSlotEquipStep::OnClickSlot()
{
	OnClickSkillSlot.ExecuteIfBound(this);
}

void UGsUISkillSlotEquipStep::SetIsSelected(bool bInEnable)
{
	_effectSelected->SetVisibility(bInEnable ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUISkillSlotEquipStep::SetIsLearned(bool bIsLearned)
{
	SetRenderOpacity(bIsLearned ? 1.f : 0.3f);
}

void UGsUISkillSlotEquipStep::SetIsLastSlot(bool bIsLastSlot)
{
	// 마지막 단계는 우측 화살표를 감춤
	_uiArrow->SetVisibility(bIsLastSlot ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillDetailInfoCostume.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Skill/GsUISkillDetailInfoDefault.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillUIData.h"
#include "Skill/GsSkillHelper.h"


void UGsUISkillDetailInfoCostume::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnLearn->OnClicked.AddDynamic(this, &UGsUISkillDetailInfoCostume::OnClickLearn);
	_btnEquip->OnClicked.AddDynamic(this, &UGsUISkillDetailInfoCostume::OnClickEquip);
	_btnRemoveEquip->OnClicked.AddDynamic(this, &UGsUISkillDetailInfoCostume::OnClickRemoveFromSlot);
	_btnCycleSetting->OnClicked.AddDynamic(this, &UGsUISkillDetailInfoCostume::OnClickCycleSetting);

	_btnLearn->SetOnClickLockSec(1.f);
	_btnEquip->SetOnClickLockSec(0.5f);
	_btnRemoveEquip->SetOnClickLockSec(0.5f);
}

void UGsUISkillDetailInfoCostume::SetDataAll(FGsSkillUIData* InData)
{
	_detailInfo->SetDataAll(InData);

	if (nullptr == InData)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	const FGsSkill* skill = InData->GetSkill();
	if (nullptr == skill)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 습득 여부 확인
	if (skill->_isLearn)
	{
		_btnLearn->SetVisibility(ESlateVisibility::Collapsed);
		_switcherEquip->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{		
		_btnLearn->SetVisibility(ESlateVisibility::Visible);
		_switcherEquip->SetVisibility(ESlateVisibility::Collapsed);

		bool bCanLearn = FGsSkillHelper::CanLearn(skill);
		_btnLearn->SetIsEnabled(bCanLearn);
	}
	
	// 장착여부 확인
	_switcherEquip->SetActiveWidgetIndex((0 == skill->_slotId) ? 0 : 1);

	// 코스튬 스킬은 업그레이드 스킬이 없으므로 배웠는지만 체크하면 됨
	bool bShowCycleSetting = skill->_isLearn;
	_btnCycleSetting->SetVisibility(bShowCycleSetting ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUISkillDetailInfoCostume::SetData(const FGsSkill* InData, int32 InOverrideLevel)
{
	_detailInfo->SetData(InData, InOverrideLevel);

	// 코스튬 스킬은 업그레이드 스킬이 없으므로 배웠는지만 체크하면 됨
	bool bShowCycleSetting = InData->_isLearn;
	_btnCycleSetting->SetVisibility(bShowCycleSetting ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUISkillDetailInfoCostume::SetPassivityData(PassivityId InPassivityId)
{
	_detailInfo->SetPassivityData(InPassivityId);
}

void UGsUISkillDetailInfoCostume::OnClickLearn()
{
	OnClickEnchantOrLearn.ExecuteIfBound(false);
}

void UGsUISkillDetailInfoCostume::OnClickEquip()
{
	OnClickEquipOrCancel.ExecuteIfBound(true);
}

void UGsUISkillDetailInfoCostume::OnClickRemoveFromSlot()
{
	OnClickRemoveEquip.ExecuteIfBound();
}

void UGsUISkillDetailInfoCostume::OnClickCycleSetting()
{
	OnClickSkillCycleSetting.ExecuteIfBound();
}

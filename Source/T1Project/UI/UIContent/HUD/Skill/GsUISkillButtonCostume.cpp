// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUISkillButtonCostume.h"
#include "Components/Image.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"


void UGsUISkillButtonCostume::OnClickSlot()
{
	Super::OnClickSlot();

	// 스킬 언락 체크
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::SKILL, true))
	{
		return;
	}

	// 코스튬 스킬 개방 체크
	if (false == IsLearnedCostumeSkill())
	{
		// TEXT: 에픽 등급 이상의 코스튬을 획득하면 코스튬 스킬을 익힐 수 있습니다.
		FText textMsg;
		if (FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_CostumeSkillSlotLocked"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		return;
	}

	if (IsEmpty())
	{
		// TEXT: 장착된 코스튬 스킬이 없습니다.
		FText textMsg;
		if (FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_CostumeSkillEquipError"), textMsg))
		{
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}
	}
}

void UGsUISkillButtonCostume::SetData(const FGsSkill* InData)
{
	if (nullptr == InData)
	{
		SetEmptySlot();
		return;
	}

	Super::SetData(InData);

	SetLockImage(!IsUnlockCostumeSkill());
}

void UGsUISkillButtonCostume::SetEmptySlot()
{
	Super::SetEmptySlot();

	SetLockImage(!IsUnlockCostumeSkill());
}

void UGsUISkillButtonCostume::SetLockImage(bool bIsLocked)
{
	_imgLock->SetVisibility(bIsLocked ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

bool UGsUISkillButtonCostume::IsLearnedCostumeSkill() const
{
	if (FGsSkillUIDataHandler* skillDataHandler = GSkill()->GetSkillUIDataHandler())
	{
		if (false == skillDataHandler->IsLearnedCostumeSkill())
		{
			return false;
		}
	}

	return true;
}

bool UGsUISkillButtonCostume::IsUnlockCostumeSkill() const
{
	// 코스튬 개방이 안되어도 괜찮음. 스킬 언락만 체크(기획팀 확인 완료)
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::SKILL, false))
	{
		return false;
	}

	if (false == IsLearnedCostumeSkill())
	{
		return false;
	}

	return true;
}

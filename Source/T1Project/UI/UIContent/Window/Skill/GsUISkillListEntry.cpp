// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillListEntry.h"
#include "Components/Widget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "PaperSprite.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Common/Skill/GsUISkillIcon.h"
#include "UI/UIContent/Common/Skill/GsUISkillDetailInfoDefault.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillUIData.h"
#include "Skill/GsSkillHelper.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Net/GsNetSendServiceWorld.h"
#include "DataSchema/Costume/GsSchemaCostumeCommon.h"


void UGsUISkillListEntry::NativeOnInitialized()
{
	_btnSlot->OnClicked.AddDynamic(this, &UGsUISkillListEntry::OnClickSlot);
	_sBtnAuto->OnClicked.AddDynamic(this, &UGsUISkillListEntry::OnClickAuto);

	_switcherChainList.Empty();
	_switcherChainList.Emplace(_switcherChain1);
	_switcherChainList.Emplace(_switcherChain2);
	_switcherChainList.Emplace(_switcherChain3);

	Super::NativeOnInitialized();
}

void UGsUISkillListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_imgSelected->SetVisibility((_bIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

bool UGsUISkillListEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUISkillListEntry::SetData(FGsSkillUIData* InData)
{
	const FGsSkill* skill = InData->GetSkill();
	if (nullptr == skill)
	{
		return;
	}

	_weaponType = skill->_tableData->requireWeapon;
	_skillId = skill->GetSkillId();
	_bIsLearned = InData->IsLearn();

	bool bIsEquipped = false;

	if (InData->_costume)
	{
		_panelCostume->SetVisibility(ESlateVisibility::HitTestInvisible);
		_panelLevelRoot->SetVisibility(ESlateVisibility::Collapsed);

		_textBlockCostumeName->SetText(InData->_costume->name);
		_textBlockCostumeName->SetColorAndOpacity(FGsUIColorHelper::GetColorSummonCostumeGradeText(InData->_costume->grade));
						
		// 슬롯에 장착 되었는가
		bIsEquipped = (0 < skill->_slotId) ? true : false;

		_panelTreeLineRoot->SetVisibility(ESlateVisibility::Collapsed);
		_sBtnAuto->SetVisibility(ESlateVisibility::Collapsed);
		SetChainStep(0, 0);
	}
	else
	{
		_panelCostume->SetVisibility(ESlateVisibility::Collapsed);

#if USE_SKILL_ENCHANT
		if (SkillCategorySet::PASSIVE == skill->GetSkillCateogrySet())
		{
			_panelLevelRoot->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			_panelLevelRoot->SetVisibility(ESlateVisibility::HitTestInvisible);

			// 레벨
			FString strLevel = FString::Format(TEXT("Lv.{0}"), { skill->GetTotalLevel() });
			_textBlockLevel->SetText(FText::FromString(strLevel));

			// 보너스 레벨(SKILL_LEVEL 패시비티에 의해 올라가는 값)
			if (0 < skill->GetBonusLevel())
			{
				_textBlockLevel->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::SKILL_LEVEL_ADD_BONUS));

				FString strBonusLevel = FString::Format(TEXT("(+{0})"), { skill->GetBonusLevel() });
				_textBlockBonusLevel->SetText(FText::FromString(strBonusLevel));
			}
			else
			{
				_textBlockLevel->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::SKILL_LEVEL_DEFAULT));
				_textBlockBonusLevel->SetText(FText::GetEmpty());
			}
		}
#else
		_panelLevelRoot->SetVisibility(ESlateVisibility::Collapsed);
#endif

		// 체인스킬 단계
		if (skill->IsChainSkill())
		{
			// chainStep은 1부터 시작하는 수
			int32 currChainStep = skill->GetChainStep();

			// Tree 라인 표현 on/off
			_panelTreeLineRoot->SetVisibility((1 < currChainStep) ?
				ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
			_imgTreeLineLast->SetVisibility(skill->IsLastChainStep() ?
				ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);

			SetChainStep(currChainStep, skill->GetMaxChainStep());

			// 체인 2단계부터 자동사용 체크박스 활성화
			if (1 < currChainStep)
			{
				// 1단계 스킬이 슬롯에 장착 되었는가
				if (const FGsSkill* firstChainSkill = InData->GetFirstChainSkill())
				{
					bIsEquipped = (0 < firstChainSkill->_slotId) ? true : false;
				}

				_sBtnAuto->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_sBtnAuto->SetIsSelected(skill->IsAuto());
			}
			else
			{
				// 슬롯에 장착 되었는가
				bIsEquipped = (0 < skill->_slotId) ? true : false;
				_sBtnAuto->SetVisibility(ESlateVisibility::Collapsed);
			}

			_sBtnAuto->SetIsEnabled(skill->_isLearn);
		}
		else
		{
			// 슬롯에 장착 되었는가
			bIsEquipped = (0 < skill->_slotId) ? true : false;

			_panelTreeLineRoot->SetVisibility(ESlateVisibility::Collapsed);
			_sBtnAuto->SetVisibility(ESlateVisibility::Collapsed);
			SetChainStep(0, 0);
		}
	}

	// 배운 스킬인지
	_dimmed->SetVisibility(_bIsLearned ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	_imgEquipMark->SetVisibility(bIsEquipped ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);

	_textBlockName->SetText(skill->_userSkillSetData->nameText);
	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(skill->_tableData->grade));

	_uiSkillIcon->SetData(skill);

	_imgCycleSetting->SetVisibility(skill->IsActiveSkillCycle() ? 
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	// 레드닷 표시
	SetRedDot(InData->IsRedDot(), EGsIconRedDotType::NORMAL);
	SetRedDot(InData->IsReinforce(), EGsIconRedDotType::REINFORCE);
}

void UGsUISkillListEntry::SetRedDot(bool bInActive, EGsIconRedDotType InRedDotType)
{
	_uiRedDot->SetRedDot(bInActive, InRedDotType);
}

void UGsUISkillListEntry::SetChainStep(int32 InCurrStep, int32 InMaxStep)
{
	int32 currIndex = InCurrStep - 1;
	int32 maxIndex = InMaxStep - 1;

	for (int32 i = 0; i < _switcherChainList.Num(); ++i)
	{
		if (i > maxIndex)
		{
			_switcherChainList[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
		else
		{
			_switcherChainList[i]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_switcherChainList[i]->SetActiveWidgetIndex((i == currIndex) ? 1 : 0);
		}
	}
}

void UGsUISkillListEntry::OnClickSlot()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

void UGsUISkillListEntry::OnClickAuto()
{
	if (_bIsLearned)
	{
		// 값을 바꾸기 위해 반대로 저장해서 보냄
		bool bAutoUse = !_sBtnAuto->GetIsSelected();
		
		SkillId rootSkillId = _skillId;
		if (const FGsSkill* skill = GSkill()->FindSkill(_skillId, CreatureWeaponType::MAX, true))
		{
			rootSkillId = FGsSkillHelper::GetUpgradeRootSkillId(skill);
			if (INVALID_SKILL_ID == rootSkillId)
			{
				rootSkillId = _skillId;
			}
		}

		FGsNetSendServiceWorld::SendRequestSkillSlotAuto(_weaponType, rootSkillId, _skillId, bAutoUse);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUICostumeActiveSkill.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Skill/GsUISkillIcon.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Popup/GsUIPopupSkillDetailInfo.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "DataSchema/Costume/GsSchemaCostumeSkill.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "Skill/GsSkillHelper.h"
#include "Skill/GsSkill.h"


void UGsUICostumeActiveSkill::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSkillIcon->OnClicked.AddDynamic(this, &UGsUICostumeActiveSkill::OnClickSkillIcon);
	_btnShortcut->OnClicked.AddDynamic(this, &UGsUICostumeActiveSkill::OnClickShortcut);
}

void UGsUICostumeActiveSkill::NativeConstruct()
{
	Super::NativeConstruct();

	_recoverCostumeId = INVALID_COSTUME_ID;
}

void UGsUICostumeActiveSkill::OnClickSkillIcon()
{
	if (INVALID_SKILL_ID == _skillId)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupSkillDetailInfo"));
	if (widget.IsValid())
	{
		if (UGsUIPopupSkillDetailInfo* popup = Cast<UGsUIPopupSkillDetailInfo>(widget.Get()))
		{
			popup->SetData(_skillId);
		}
	}
}

void UGsUICostumeActiveSkill::OnClickShortcut()
{
	if (INVALID_SKILL_ID == _skillId)
	{
		return;
	}

	// 언락 여부 체크
	if (FGsUnlockManager* unlockMgr = GSUnlock())
	{
		if (false == unlockMgr->IsContentsUnlock(EGsUnlockContentsType::SKILL, true))
		{
			return;
		}
	}

	_recoverCostumeId = _costumeId;

	if (FGsSkillUIDataHandler* skillDataHandler = GSkill()->GetSkillUIDataHandler())
	{
		skillDataHandler->SetWindowOpenSelectedSkillId(_skillId);
	}

	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_SKILL);
}

void UGsUICostumeActiveSkill::SetData(TWeakPtr<FGsCostumeData> costumeData)
{
	if (SetDataInter(costumeData))
	{
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UGsUICostumeActiveSkill::SetDataInter(TWeakPtr<FGsCostumeData> data)
{
	_skillId = INVALID_SKILL_ID;

	if (false == data.IsValid())
	{
		return false;
	}

	auto costumeData = data.Pin();
	_costumeId = costumeData->_id;

	const FGsSchemaCostumeSkill* costumeSkill = costumeData->GetCostumeActiveSkill();
	if (nullptr == costumeSkill)
	{
		return false;
	}

	const FGsSchemaUserSkillSet* userSkillSet = costumeSkill->userSkillSetId.GetRow();
	if (nullptr == userSkillSet)
	{
		return false;
	}

	CreatureGenderType gender = CreatureGenderType::MALE;
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		gender = userData->mGender;
	}

	TWeakPtr<const FGsSkill> skillPtr = FGsSkillHelper::GetSkillByUserSkillSet(userSkillSet, gender);
	if (false == skillPtr.IsValid() ||
		nullptr == skillPtr.Pin()->_tableData)
	{
		return false;
	}

	const FGsSkill* skill = skillPtr.Pin().Get();
	_skillId = skill->GetSkillId();

	// 아이콘 표시
	_uiSkillIcon->SetData(skill);

	// 상태 표시
	SetUISkillState(costumeData->isCollectedCostume(), skill->_isLearn);

	// 스킬 이름 표시
	_textBlockSkillName->SetText(userSkillSet->nameText);
	_textBlockSkillName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(skill->_tableData->grade));
	_textBlockSkillName->SetRenderOpacity(skill->_isLearn ? 1.f : _dimmedOpacity);

	return true;
}

void UGsUICostumeActiveSkill::SetUISkillState(bool bIsCollected, bool bIsLearn)
{
	FText textState;	
	bool bIsRedDot = false;

	if (bIsCollected)
	{
		if (bIsLearn)
		{
			// TEXT: 사용 가능
			FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_Skill_Usable"), textState);
			_textBlockSkillState->SetColorAndOpacity(_colorCanUseSkill);
		}
		else
		{
			// TEXT: 습득 가능
			FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_Skill_Acquirable"), textState);
			_textBlockSkillState->SetColorAndOpacity(_colorCanLearnSkill);

			bIsRedDot = true;
		}
	}
	else
	{
		// TEXT: 코스튬 미보유
		FText::FindText(TEXT("CostumeText"), TEXT("CostumeUI_Skill_NotHaveCostume"), textState);
		_textBlockSkillState->SetColorAndOpacity(_colorNoCostume);
	}

	_textBlockSkillState->SetText(textState);

	// 레드닷 표시
	_uiRedDot->SetRedDot(bIsRedDot, EGsIconRedDotType::NORMAL);
}

CostumeId UGsUICostumeActiveSkill::GetRecoverCostumeId()
{
	CostumeId tempCostumeId = _recoverCostumeId;
	_recoverCostumeId = INVALID_COSTUME_ID;

	return tempCostumeId;
}

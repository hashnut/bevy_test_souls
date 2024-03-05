// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialActionSkillLearn.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "DataSchema/Tutorial/GsSchemaTutorialActionUIInteraction.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillHelper.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Window/GsUIWindowSkill.h"
#include "UI/UIContent/Tray/GsUITrayTutorial.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"


void FGsTutorialActionSkillLearn::OnPrevStart(const IGsMessageParam* InParam)
{
	if (const FGsTutorialMsgParamSkill* param = InParam->Cast<const FGsTutorialMsgParamSkill>())
	{
		_skillId = param->_data;
	}
}

bool FGsTutorialActionSkillLearn::OnStart()
{
	if (false == Super::OnStart())
	{
		return false;
	}

	if (INVALID_SKILL_ID == _skillId)
	{
		return false;
	}

	// 버튼 포커스 처리
	_actionUITable = _table->actionUIInteractionRow.GetRow();
	if (nullptr == _actionUITable)
	{
		return false;
	}

	const FGsSkill* skill = GSkill()->FindSkill(_skillId, CreatureWeaponType::MAX, true);
	if (nullptr == skill)
	{
		return false;
	}

	// 배울 수 있는 상태인지 체크
	if (false == FGsSkillHelper::CanLearn(skill, false))
	{
		return false;
	}

	if (false == _tutorialUI->SetTargetData(_table->id, _actionUITable))
	{
		return false;
	}

	return true;
}

void FGsTutorialActionSkillLearn::OnEnd(bool bInSendMessage)
{
	_actionUITable = nullptr;
	_skillId = INVALID_SKILL_ID;

	Super::OnEnd(bInSendMessage);
}

void FGsTutorialActionSkillLearn::OnLearnSkill(SkillId InSkillId)
{
	if (EGsTutorialActionConditionType::LEARN_SKILL == _table->endCondition)
	{
		if (_skillId == InSkillId)
		{
			SetEndCondition(true);			
		} 
	}

	if (CheckEndCondition())
	{
		OnEnd();
	}
}

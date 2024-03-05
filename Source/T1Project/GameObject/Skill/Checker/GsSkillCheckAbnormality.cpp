// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillCheckAbnormality.h"
#include "Skill/GsSkill.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerLocalPlayer.h"
#include "GameObject/Abnormality/CrowdControl/GsCrowdControlBase.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Stat/GsStatBase.h"


EGsSkillCheckResultType FGsSkillCheckAbnormality::GetErrType() const
{
	return EGsSkillCheckResultType::Abnormality;
}

bool FGsSkillCheckAbnormality::ProcessCheck(const FGsSkill* NextSkill)
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == local)
	{
		return false;
	}

	FGsAbnormalityHandlerBase* abnormalityHandler = local->GetAbnormalityHandler();
	if (nullptr == abnormalityHandler)
	{
		return false;
	}

	if (false == NextSkill->_tableData->skillTriggerConditionInfo.ignoreCreatureBehavior)
	{
		if (FGsCrowdControlBase* crowdControl = abnormalityHandler->GetCrowdControl())
		{
			if (crowdControl->IsCreatureAction())
			{
				if (NextSkill->_tableData->isNormal)
				{
					return !crowdControl->IsCreatureActionType(CreatureActionType::CREATURE_ACTION_TYPE_SKILL_BASIC);
				}
				else if (NextSkill->_tableData->category == SkillCategory::BUFF)
				{
					return !crowdControl->IsCreatureActionType(CreatureActionType::CREATURE_ACTION_TYPE_BUFF_SKILL);
				}
				else
				{
					// 평타는 사용 가능
					return !crowdControl->IsCreatureActionType(CreatureActionType::CREATURE_ACTION_TYPE_SKILL);
				}
			}
		}
	}
	return true;
}

FText FGsSkillCheckAbnormality::GetErrMessage() const
{
	FText findText;
	FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_Abnormality"), findText);

	return findText;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillCheckExecuteConditionAbnormality.h"
#include "Skill/GsSkill.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerLocalPlayer.h"

#include "DataSchema/Skill/Param/GsSchemaSkillExecuteConditionMethodAbnormality.h"

EGsSkillCheckResultType FGsSkillCheckExecuteConditionAbnormality::GetErrType() const
{
	return EGsSkillCheckResultType::ExecuteConditionAbnormality;
}

bool FGsSkillCheckExecuteConditionAbnormality::ProcessCheck(const FGsSkill* NextSkill)
{
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == local)
	{
		return false;
	}

	const FGsSchemaSkillSet* skillTable = NextSkill->_tableData;
	const FGsSchemaSkillExecuteConditionSet* skillConditionSet = skillTable->skillTriggerConditionInfo.executeConditionId.GetRow();
	if (nullptr == skillConditionSet)
	{
		return true;
	}

	if (skillConditionSet->conditionMethod != SkillExecuteConditionMethod::ABNORMALITY)
	{
		return true;
	}

	const FGsSchemaSkillExecuteConditionMethodAbnormality* conditionTable =
		skillConditionSet->conditionMethodParam.GetRowByType<FGsSchemaSkillExecuteConditionMethodAbnormality>();
	if (nullptr == conditionTable)
	{
		return true;
	}

	// 캐스터 일때만 확인할수 있다.
	if (conditionTable->target == SkillTargetType::CASTER)
	{
		// Type none이면 무조건 성공
		if (conditionTable->type == SkillExecuteConditionAbnormalityConditionType::NONE)
		{
			return true;
		}

		bool haveAbnormality = false;

		if (FGsAbnormalityHandlerBase* abnormalityHandler = local->GetAbnormalityHandler())
		{
			if (conditionTable->type == SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_TYPE)
			{
				haveAbnormality = abnormalityHandler->IsAbnormalityEffectType(conditionTable->param1);
			}
			else if (conditionTable->type == SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_ID)
			{
				haveAbnormality = abnormalityHandler->IsAbnormalityId(conditionTable->param2);
			}
		}

		if (conditionTable->setAbnormalityAllow == SetAbnormalityAllow::DISALLOW)
		{
			haveAbnormality = !haveAbnormality;
		}
		return haveAbnormality;
	}

	return true;
}

FText FGsSkillCheckExecuteConditionAbnormality::GetErrMessage() const
{
	FText findText;
// #if WITH_EDITOR
// 	FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_ExecuteConditionAbnormality"), findText);
// #endif
	return findText;
}
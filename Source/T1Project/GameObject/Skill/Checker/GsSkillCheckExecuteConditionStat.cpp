// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillCheckExecuteConditionStat.h"
#include "Skill/GsSkill.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "DataSchema/Skill/Param/GsSchemaSkillExecuteConditionMethodStat.h"

EGsSkillCheckResultType FGsSkillCheckExecuteConditionStat::GetErrType() const
{
	return EGsSkillCheckResultType::ExecuteConditionStat;
}

bool FGsSkillCheckExecuteConditionStat::ProcessCheck(const FGsSkill* NextSkill)
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

	const FGsSchemaSkillExecuteConditionMethodStat* conditionTable =
		skillConditionSet->conditionMethodParam.GetRowByType<FGsSchemaSkillExecuteConditionMethodStat>();
	if (nullptr == conditionTable)
	{
		return true;
	}
	
	// 캐스터 일때만 확인할수 있다.
	if (conditionTable->target == SkillTargetType::CASTER)
	{
		StatType statType = conditionTable->statType.GetStatType();
		if (statType == StatType::NONE)
		{
			return true;
		}

		FGsGameObjectDataCreature* objdata = local->GetCastData<FGsGameObjectDataCreature>();
		FGsStatBase* userStat = objdata->GetStatBase().Get();
		if (const FGsStatInfo* statInfo = userStat->GetStatInfo(statType))
		{
			bool compareValue = true;
			int32 statValue = statInfo->GetStatValue();
			bool result = true;
			if (conditionTable->type == SkillExecuteConditionStatConditionType::VALUE)
			{
				result = statValue >= conditionTable->param;
			}
			else if (userStat->IsMaxStatType(statType)) // conditionTable->type == SkillExecuteConditionStatConditionType::RATIO
			{
				int32 maxStatValue = statInfo->GetMaxStatValue();
				result = (statValue / maxStatValue) >= (statValue / maxStatValue);
			}

			// 이하를 원했겠지만.... 미만이 된다.
			if (conditionTable->statCondition == SkillExecuteConditionStatConditionMethod::LESS)
			{
				result = !result;
			}

			return result;
		}
	}

	return true;
}

FText FGsSkillCheckExecuteConditionStat::GetErrMessage() const
{
	FText findText;
// #if WITH_EDITOR
// 	FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_ExecuteConditionStat"), findText);
// #endif
	return findText;
}
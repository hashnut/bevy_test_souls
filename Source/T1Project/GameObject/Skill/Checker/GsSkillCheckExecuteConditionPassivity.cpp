// Fill out your copyright notice in the Description page of Project Settings.

#include "GsSkillCheckExecuteConditionPassivity.h"
#include "Skill/GsSkill.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerLocalPlayer.h"

#include "DataSchema/Skill/Param/GsSchemaSkillExecuteConditionMethodPassivity.h"

EGsSkillCheckResultType FGsSkillCheckExecuteConditionPassivity::GetErrType() const
{
	return EGsSkillCheckResultType::ExecuteConditionPassivity;
}

bool FGsSkillCheckExecuteConditionPassivity::ProcessCheck(const FGsSkill* NextSkill)
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

	if (skillConditionSet->conditionMethod != SkillExecuteConditionMethod::PASSIVITY)
	{
		return true;
	}

	const FGsSchemaSkillExecuteConditionMethodPassivity* conditionTable =
		skillConditionSet->conditionMethodParam.GetRowByType<FGsSchemaSkillExecuteConditionMethodPassivity>();
	if (nullptr == conditionTable)
	{
		return true;
	}

	// 캐스터 일때만 확인할수 있다.
	if (conditionTable->target == SkillTargetType::CASTER)
	{
		bool havePassivity = false;

		FGsAbnormalityHandlerBase* abnormalityHandler = local->GetAbnormalityHandler();
		FGsAbnormalityHandlerLocalPlayer* localAbnormalityHandler = static_cast<FGsAbnormalityHandlerLocalPlayer*>(abnormalityHandler);
		return localAbnormalityHandler->IsPassivity(conditionTable->value);
	}

	return true;
}

FText FGsSkillCheckExecuteConditionPassivity::GetErrMessage() const
{
	FText findText;
// #if WITH_EDITOR
// 	FText::FindText(TEXT("SkillUIText"), TEXT("Notice_SkillCheckResult_ExecuteConditionPassivity"), findText);
// #endif
	return findText;
}
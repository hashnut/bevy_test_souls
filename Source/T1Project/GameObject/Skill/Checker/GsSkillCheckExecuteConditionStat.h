// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillCheckBase.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillCheckExecuteConditionStat : public IGsSkillCheckBase
{
	using Super = IGsSkillCheckBase;

public:
	explicit FGsSkillCheckExecuteConditionStat() = default;
	virtual ~FGsSkillCheckExecuteConditionStat() = default;

public:
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) override;

protected:
	bool CheckConditionAbnormality(const class FGsSkill* NextSkill, class FGsAbnormalityHandlerBase* AbnormalityHandler,
		const struct FGsSchemaSkillExecuteConditionMethodAbnormality* ConditionAbnormalityTable);
	bool CheckConditionPassivity(const class FGsSkill* NextSkill, class FGsAbnormalityHandlerBase* AbnormalityHandler,
		const struct FGsSchemaSkillExecuteConditionMethodPassivity* ConditionPassivity);
	bool CheckConditionStat(const class FGsSkill* NextSkill, class FGsStatBase* UserStat,
		const struct FGsSchemaSkillExecuteConditionMethodStat* ConditionStat);

public:
	virtual EGsSkillCheckResultType GetErrType() const override;
	virtual FText GetErrMessage() const override;
};

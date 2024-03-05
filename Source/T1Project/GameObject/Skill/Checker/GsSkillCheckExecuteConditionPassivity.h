// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillCheckBase.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillCheckExecuteConditionPassivity : public IGsSkillCheckBase
{
	using Super = IGsSkillCheckBase;

public:
	explicit FGsSkillCheckExecuteConditionPassivity() = default;
	virtual ~FGsSkillCheckExecuteConditionPassivity() = default;

public:
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) override;

public:
	virtual EGsSkillCheckResultType GetErrType() const override;
	virtual FText GetErrMessage() const override;
};

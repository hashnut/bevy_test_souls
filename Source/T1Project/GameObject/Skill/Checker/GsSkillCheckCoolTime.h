// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillCheckBase.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillCheckCoolTime : public IGsSkillCheckBase
{
	using Super = IGsSkillCheckBase;

public:
	explicit FGsSkillCheckCoolTime() = default;
	virtual ~FGsSkillCheckCoolTime() = default;

public:
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) override;

public:
	virtual EGsSkillCheckResultType GetErrType() const override;
	virtual FText GetErrMessage() const override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillCheckBase.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillCheckCancel : public IGsSkillCheckBase
{
	using Super = IGsSkillCheckBase;

private:
	class FGsSkillHandlerLocalPlayer* _skillHandlerLocal = nullptr;

public:
	explicit FGsSkillCheckCancel(class FGsSkillHandlerLocalPlayer* SkillHandler) :
		_skillHandlerLocal(SkillHandler) {}
	virtual ~FGsSkillCheckCancel()	= default;

public:
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) override;

public:
	virtual EGsSkillCheckResultType GetErrType() const override;
	virtual FText GetErrMessage() const override;
};

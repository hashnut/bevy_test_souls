// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillCheckBase.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillCheckSame : public IGsSkillCheckBase
{
	using Super = IGsSkillCheckBase;

private:
	class FGsSkillHandlerLocalPlayer* _skillHandlerLocal = nullptr;
	class FGsGameObjectStateManager* _fsm = nullptr;

public:
	explicit FGsSkillCheckSame(class FGsSkillHandlerLocalPlayer* SkillHandler, class FGsGameObjectStateManager* Fsm) :
		_skillHandlerLocal(SkillHandler),
		_fsm(Fsm)	{}
	virtual ~FGsSkillCheckSame() = default;

public:
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) override;

public:
	virtual EGsSkillCheckResultType GetErrType() const override;
	virtual FText GetErrMessage() const override;
};

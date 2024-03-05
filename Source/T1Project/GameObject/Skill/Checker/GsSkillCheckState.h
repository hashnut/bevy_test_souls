// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillCheckBase.h"

/**
 * State 일괄 처리가 애매할 경우 클래스 세분화를 진행
 */
class T1PROJECT_API FGsSkillCheckState : public IGsSkillCheckBase
{
	using Super = IGsSkillCheckBase;

public:
	explicit FGsSkillCheckState() = default;
	virtual ~FGsSkillCheckState() = default;

public:
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) override;

public:
	virtual EGsSkillCheckResultType GetErrType() const override;
	virtual FText GetErrMessage() const override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillCheckBase.h"

/**
 * 실제 서버에서 받은 스킬데이터 체크
 */
class T1PROJECT_API FGsSkillCheckSyncUserData : 
	public IGsSkillCheckBase
{
	using Super = IGsSkillCheckBase;

public:
	explicit FGsSkillCheckSyncUserData() = default;
	virtual ~FGsSkillCheckSyncUserData() = default;

public:
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) override;

public:
	virtual EGsSkillCheckResultType GetErrType() const override;
	virtual FText GetErrMessage() const override;
};

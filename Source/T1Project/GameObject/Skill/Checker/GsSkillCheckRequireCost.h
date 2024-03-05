// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsSkillCheckBase.h"

/**
 * 
 */
class T1PROJECT_API FGsSkillCheckRequireCost : public IGsSkillCheckBase
{
	using Super = IGsSkillCheckBase;

	enum RequireCostErrType
	{
		None,
		WeightFull,
		NotEnoughHP,
		NotEnoughMP,
		RequireItem
	};
	RequireCostErrType _currentErrType;
	int32 _currentErrItemId = 0;

public:
	explicit FGsSkillCheckRequireCost() = default;
	virtual ~FGsSkillCheckRequireCost() = default;

public:
	virtual bool ProcessCheck(const class FGsSkill* NextSkill) override;

private:
	void SetErrorType(RequireCostErrType ErrType);

public:
	virtual EGsSkillCheckResultType GetErrType() const override;
	virtual FText GetErrMessage() const override;
};

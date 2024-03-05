// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"

/**
 *  스킬 슬롯 데이터(UI표시용)
 */
enum class EGsSkillSlotCheckType
{
	CheckStageMode = 0,
	CheckRequireCost = 1,
	CheckExecuteConditionAbnormality = 2,
	CheckExecuteConditionPassivity = 3,
	CheckExecuteConditionStat = 4,
	CheckReaction = 5,
	CheckCoolTime = 6,

	MAX,
};

struct T1PROJECT_API FGsSkillSlotData final
{
public:
	bool _bUseAuto = false;
	bool _isReserve = false;

	uint16 _skillCheckResult = 0;
};

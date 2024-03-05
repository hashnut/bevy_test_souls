// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 레벨 업 티커
 */
struct FGsStatChangeData
{
public:
	StatType _statType;
	int32 _oldValue;
	int32 _newValue;

public:
	FGsStatChangeData(StatType inStat, int32 inOldValue, int32 inNewValue)
		: _statType(inStat), _oldValue(inOldValue), _newValue(inNewValue)
	{

	}
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"

/**
 * 
 */
class T1PROJECT_API FGsItemStat
{

private:
	StatType _type;
	int32 _oriValue;
	int32 _calcValue; // Balance Data 수치까지 계산된 값


public:
	FGsItemStat(StatType InType , int32 InValue);
	FGsItemStat(StatType InType, int32 InOriValue , int32 InCalcValue);
	~FGsItemStat() = default;

public:
	void SetStatType(const StatType InStatType) { _type = InStatType; }
	void SetOriStatValue(const int32 InValue) { _oriValue = InValue; }
	void SetCalcStatValue(const int32 InValue) { _calcValue = InValue; }

public:
	const StatType GetStatType() { return _type; }
	const int32 GetStatOriValue() { return _oriValue; }
	const int32 GetCalcStatValue() { return _calcValue; }
		 
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStatInfo.h"
#include "T1Project.h"


void FGsStatInfo::SetStatValue(int32 InValue)
{
}

void FGsStatInfo::SetMaxStatValue(int32 InValue)
{
	_maxStatValue = InValue;
}

void FGsStatInfo::SetStatType(StatType InType)
{
	_statType = InType;
}

/////////////////////////////////////////////////////////////////////

int32 FGsStatInfo::GetStatValue() const
{
	GSLOG(Error, TEXT("Called non overridden function."));
	return 0;
}

int32 FGsStatInfo::GetMaxStatValue() const
{
	return _maxStatValue;
}

StatType FGsStatInfo::GetStatType() const
{
	return _statType;
}

/////////////////////////////////////////////////////////////////////

void FGsStatInfoCommon::SetStatValue(int32 InValue)
{
	_statValue = InValue;
}

int32 FGsStatInfoCommon::GetStatValue() const
{
	return _statValue;
}

/////////////////////////////////////////////////////////////////////

void FGsStatInfoSecure::SetStatValue(int32 InValue)
{
	_statValue.Set(InValue);
}

int32 FGsStatInfoSecure::GetStatValue() const
{
	int32 StatValue;
	_statValue.Get(StatValue);
	return StatValue;
}

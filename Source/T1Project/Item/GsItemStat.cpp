// Fill out your copyright notice in the Description page of Project Settings.


#include "GsItemStat.h"



FGsItemStat::FGsItemStat(StatType InType, int32 InValue)
{
	_type = InType;
	_oriValue = InValue;
	_calcValue = 0;
}

FGsItemStat::FGsItemStat(StatType InType, int32 InOriValue, int32 InCalcValue)
{
	_type = InType;
	_oriValue = InOriValue;
	_calcValue = InCalcValue;
}

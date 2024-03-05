// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStatePropBase.h"

/**
* NonPlayer À¯ÈÞ »óÅÂ
*/
class T1PROJECT_API FGsStatePropFinish :
	public FGsStateBaseSingleProp,
	public TGsStateSingleton<FGsStatePropFinish>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleProp);
};
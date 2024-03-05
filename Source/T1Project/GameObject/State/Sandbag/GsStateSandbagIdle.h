// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateSandbagBase.h"

/**
* NonPlayer À¯ÈÞ »óÅÂ
*/
class T1PROJECT_API FGsStateSandbagIdle : 
	public FGsStateBaseSingleSandbag,
	public TGsStateSingleton<FGsStateSandbagIdle>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleSandbag);
};
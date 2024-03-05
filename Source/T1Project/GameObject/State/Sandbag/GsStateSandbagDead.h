// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateSandbagBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateSandbagDead : 
	public FGsStateBaseSingleSandbag,
	public TGsStateSingleton<FGsStateSandbagDead>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleSandbag);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateNonPlayerDead : 
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerDead>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);
};

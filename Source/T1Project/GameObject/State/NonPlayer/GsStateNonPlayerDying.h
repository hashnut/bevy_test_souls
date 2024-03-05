// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateNonPlayerDying : 
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerDying>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);
};

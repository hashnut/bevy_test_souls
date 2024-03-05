// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateRemotePlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateRemotePlayerWalk : 
	public FGsStateRemotePlayerMoveBase,
	public TGsStateSingleton<FGsStateRemotePlayerWalk>
{
	GS_STATE_GENERATED_BODY(FGsStateRemotePlayerMoveBase);
};

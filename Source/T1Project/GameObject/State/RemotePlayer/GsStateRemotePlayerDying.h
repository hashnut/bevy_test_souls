// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateRemotePlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateRemotePlayerDying : 
	public FGsStateBaseRemotePlayer,
	public TGsStateSingleton<FGsStateRemotePlayerDying>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseRemotePlayer);
};

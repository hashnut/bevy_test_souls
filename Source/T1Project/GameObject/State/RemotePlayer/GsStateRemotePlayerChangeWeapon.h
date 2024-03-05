// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateRemotePlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateRemotePlayerChangeWeapon	:
	public FGsStateBaseRemotePlayer,
	public TGsStateSingleton<FGsStateRemotePlayerChangeWeapon>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseRemotePlayer);
};

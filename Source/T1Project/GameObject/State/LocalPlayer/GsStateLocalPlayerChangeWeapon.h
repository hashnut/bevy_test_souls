// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
 * 무기 교체 (클래스 체인지)
 */
class T1PROJECT_API FGsStateLocalPlayerChangeWeapon :
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerChangeWeapon>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);
};

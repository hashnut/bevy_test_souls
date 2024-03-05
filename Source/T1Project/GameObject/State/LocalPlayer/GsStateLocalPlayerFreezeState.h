// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
* Idle 상태에서 파생되는 State가 없어야 하는 경우(ex: CutScene Play 등)
*/
class T1PROJECT_API FGsStateLocalPlayerFreezeState : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerFreezeState>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);
};

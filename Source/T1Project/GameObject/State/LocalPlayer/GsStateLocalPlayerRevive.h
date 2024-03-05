// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
* Local 부활하는 상태
*/
class T1PROJECT_API FGsStateLocalPlayerRevive : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerRevive>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);
};
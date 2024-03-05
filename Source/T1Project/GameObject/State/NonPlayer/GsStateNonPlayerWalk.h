// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/**
* NPC ╟х╠Б ╩Себ
*/
class T1PROJECT_API FGsStateNonPlayerWalk : 
	public FGsStateNonPlayerMoveBase,
	public TGsStateSingleton<FGsStateNonPlayerWalk>
{
	GS_STATE_GENERATED_BODY(FGsStateNonPlayerMoveBase);
};

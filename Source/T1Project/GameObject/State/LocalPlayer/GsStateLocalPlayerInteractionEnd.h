// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateLocalPlayerInteractionEnd :
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerInteractionEnd>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);
};

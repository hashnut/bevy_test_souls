// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateRemotePlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateRemotePlayerIdle : 
	public FGsStateBaseRemotePlayer,
	public TGsStateSingleton<FGsStateRemotePlayerIdle>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseRemotePlayer);
};
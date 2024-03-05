// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateSandbagBase.h"
#include "GameObject/State/GsStateBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateSandbagDying : 
	public FGsStateBaseSingleSandbag,
	public TGsStateSingleton<FGsStateSandbagDying>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleSandbag);
};

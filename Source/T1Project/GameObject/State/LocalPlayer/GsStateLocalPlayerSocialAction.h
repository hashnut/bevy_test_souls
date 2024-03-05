// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateLocalPlayerSocialAction	:
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerSocialAction>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	virtual bool CheckDeadlock() { return false; }

public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;
};

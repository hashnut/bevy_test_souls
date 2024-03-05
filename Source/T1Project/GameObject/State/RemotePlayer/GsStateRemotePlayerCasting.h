// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateRemotePlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateRemotePlayerCasting :
	public FGsStateBaseRemotePlayer,
	public TGsStateSingleton<FGsStateRemotePlayerCasting>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseRemotePlayer);

public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;
};

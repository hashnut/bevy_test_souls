// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateRemotePlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateRemotePlayerSocialAction :
	public FGsStateBaseRemotePlayer,
	public TGsStateSingleton<FGsStateRemotePlayerSocialAction>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseRemotePlayer);

public:
	void ReEnter(UGsGameObjectBase* Owner) override;
};

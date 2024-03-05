// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateRemotePlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateRemotePlayerSocial :
	public FGsStateBaseRemotePlayer,
	public TGsStateSingleton<FGsStateRemotePlayerSocial>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseRemotePlayer);

public:
	void ReEnter(UGsGameObjectBase* Owner) override;
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
};

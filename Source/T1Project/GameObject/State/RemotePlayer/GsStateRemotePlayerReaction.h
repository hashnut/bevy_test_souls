// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateRemotePlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateRemotePlayerReaction : 
	public FGsStateBaseRemotePlayer,
	public TGsStateSingleton<FGsStateRemotePlayerReaction>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseRemotePlayer);

protected:
	bool _isJumped{ false };
public:
	virtual void ReEnter(class UGsGameObjectBase* Owner) override;
	virtual void Update(class UGsGameObjectBase* Owner, float Delta) override;
};
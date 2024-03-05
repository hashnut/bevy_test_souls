// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateNonPlayerReaction : 
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerReaction>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);

protected:
	bool _isJumped{ false };
public:
	virtual void ReEnter(class UGsGameObjectBase* Owner) override;
	virtual void Update(class UGsGameObjectBase* Owner, float Delta) override;
};

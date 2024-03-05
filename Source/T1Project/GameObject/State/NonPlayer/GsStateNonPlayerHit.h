// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateNonPlayerHit : 
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerHit>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);

public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;
	virtual int GetAniRandomCount() const override { return 2; }
};

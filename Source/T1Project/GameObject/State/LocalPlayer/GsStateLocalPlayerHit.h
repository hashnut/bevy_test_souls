// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateLocalPlayerHit : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerHit>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;
	virtual int GetAniRandomCount() const override { return 2; }
};

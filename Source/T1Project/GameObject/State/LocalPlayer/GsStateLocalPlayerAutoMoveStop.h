// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateLocalPlayerAutoMoveStop : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerAutoMoveStop>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
};

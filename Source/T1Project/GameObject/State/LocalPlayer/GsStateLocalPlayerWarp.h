// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
* Warp CommonAction
*/
class T1PROJECT_API FGsStateLocalPlayerWarp : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerWarp>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
	virtual void ReEnter(UGsGameObjectBase* Owner) override;
};

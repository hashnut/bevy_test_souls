// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
* Local 전방 이동 상태
*/
class T1PROJECT_API FGsStateLocalPlayerWalk : 
	public FGsStateLocalPlayerMoveBase,
	public TGsStateSingleton<FGsStateLocalPlayerWalk>
{
	GS_STATE_GENERATED_BODY(FGsStateLocalPlayerMoveBase);

public:
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
};
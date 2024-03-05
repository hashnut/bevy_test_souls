// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
* Local 달리기 이동 상태
*/
class T1PROJECT_API FGsStateLocalPlayerRun : 
	public FGsStateLocalPlayerMoveBase,
	public TGsStateSingleton<FGsStateLocalPlayerRun>
{
	GS_STATE_GENERATED_BODY(FGsStateLocalPlayerMoveBase);

public:
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/**
* NPC 달리기 상태
*/
class T1PROJECT_API FGsStateNonPlayerRun : 
	public FGsStateNonPlayerMoveBase,
	public TGsStateSingleton<FGsStateNonPlayerRun>
{
	GS_STATE_GENERATED_BODY(FGsStateNonPlayerMoveBase);

private:
	bool IsLock(class UGsGameObjectBase* Owner) const;
};
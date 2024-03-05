// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/**
* NPC 스폰 상태
*/
class T1PROJECT_API FGsStateNonPlayerSpawn : 
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerSpawn>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);

public:
	bool CanSpawnAction(class UGsGameObjectBase* Owner) const;
};

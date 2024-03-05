// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
* Local 스폰 상태
*/
class T1PROJECT_API FGsStateLocalPlayerSpawn : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerSpawn>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;
};
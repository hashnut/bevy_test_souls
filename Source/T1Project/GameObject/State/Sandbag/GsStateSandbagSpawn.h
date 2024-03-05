// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateSandbagBase.h"

/**
* NPC 스폰 상태
*/
class T1PROJECT_API FGsStateSandbagSpawn : 
	public FGsStateBaseSingleSandbag,
	public TGsStateSingleton<FGsStateSandbagSpawn>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleSandbag);
};

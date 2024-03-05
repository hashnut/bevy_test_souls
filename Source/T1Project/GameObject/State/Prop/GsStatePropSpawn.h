// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStatePropBase.h"

/**
* NPC 스폰 상태
*/
class T1PROJECT_API FGsStatePropSpawn : 
	public FGsStateBaseSingleProp,
	public TGsStateSingleton<FGsStatePropSpawn>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleProp);
};

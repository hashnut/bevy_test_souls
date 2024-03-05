// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

class UGsGameObjectBase;

/**
* Local À¯ÈÞ »óÅÂ
*/
class T1PROJECT_API FGsStateLocalPlayerActionEnd :
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerActionEnd>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
};
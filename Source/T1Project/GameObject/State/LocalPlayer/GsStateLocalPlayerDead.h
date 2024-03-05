// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
 * 죽어있는 상태 처리
 */
class T1PROJECT_API FGsStateLocalPlayerDead : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerDead>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

private:	
	int32 _maxTime;
	bool _isShowRevive;

public:
	virtual void Update(class UGsGameObjectBase* Owner, float Delta) override;
};

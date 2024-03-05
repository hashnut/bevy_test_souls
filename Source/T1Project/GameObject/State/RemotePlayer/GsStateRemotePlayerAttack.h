// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateRemotePlayerBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateRemotePlayerAttack : 	
	public FGsStateBaseRemotePlayer,
	public TGsStateSingleton<FGsStateRemotePlayerAttack>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseRemotePlayer);

public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;

public:
	virtual bool AllowMotionStop() const override { return false; }

private:
	// 스킬 캔슬 가능 상태일때 이동 전환
	bool CanMove(UGsGameObjectBase* Owner) const;

	void PlaySkill(UGsGameObjectBase* Owner);
};

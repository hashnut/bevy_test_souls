// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/**
* NonPlayer 공격 상태
*/
class T1PROJECT_API FGsStateNonPlayerAttack : 
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerAttack>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);

public:
	virtual bool AllowMotionStop() const override { return false; }

public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;

private:
	void PlaySkill(UGsGameObjectBase* Owner);
	bool CanCancel(UGsGameObjectBase* Owner) const;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
 * LocalPlayer 스킬 캐스팅 상태
 */
class T1PROJECT_API FGsStateLocalPlayerCasting :
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerCasting>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;

public:
	virtual bool AllowMotionStop() const override { return false; }

private:
	bool CanCancel(UGsGameObjectBase* Owner) const;
};

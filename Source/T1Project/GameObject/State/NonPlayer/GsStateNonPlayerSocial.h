// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/*
 NonPlayer social ป๓ลย
*/

class T1PROJECT_API FGsStateNonPlayerSocial : 
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerSocial>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);
public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
};

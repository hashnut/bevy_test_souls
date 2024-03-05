// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/*
 NonPlayer social ป๓ลย
*/

class T1PROJECT_API FGsStateNonPlayerSocialAction : 
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerSocialAction>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);

public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;
};

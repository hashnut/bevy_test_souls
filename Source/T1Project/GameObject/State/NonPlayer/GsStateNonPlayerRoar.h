// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/*
 NonPlayer roar ป๓ลย
*/

class T1PROJECT_API FGsStateNonPlayerRoar : 
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerRoar>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);
};
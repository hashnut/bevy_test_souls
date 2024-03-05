// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
* 탑승 상태 클래스
*/
class T1PROJECT_API FGsStateLocalPlayerRide : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerRide>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);
};
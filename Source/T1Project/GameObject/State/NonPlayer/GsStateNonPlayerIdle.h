// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateNonPlayerBase.h"

/**
* NonPlayer À¯ÈÞ »óÅÂ
*/
class T1PROJECT_API FGsStateNonPlayerIdle : 
	public FGsStateBaseSingleNonPlayer,
	public TGsStateSingleton<FGsStateNonPlayerIdle>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleNonPlayer);

protected:
	bool CheckBattleIdle(UGsGameObjectBase* Owner) const;
};
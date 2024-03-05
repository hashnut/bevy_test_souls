// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
* Local CommonAction
*/
class T1PROJECT_API FGsStateLocalPlayerChangeIdle : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerChangeIdle>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	virtual void ReEnter(UGsGameObjectBase* Owner) override;

private:
	bool CanCancel(UGsGameObjectBase* Owner) const;
};
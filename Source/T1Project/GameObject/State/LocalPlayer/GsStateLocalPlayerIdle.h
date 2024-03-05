// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

/**
* Local 유휴 상태
*/
class T1PROJECT_API FGsStateLocalPlayerIdle : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerIdle>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	// 전투/평화 상태 전환
	static void ChangeIdleMotion(UGsGameObjectBase* Owner);
public:
	virtual bool Ready(UGsGameObjectBase* Owner) override;
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;

public:
	virtual bool CheckDeadlock() { return false; }

protected:
	bool CheckBattleIdle(UGsGameObjectBase* Owner) const;
};
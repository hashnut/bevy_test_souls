// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsStateLocalPlayerBase.h"

// 자동 이동용 상태
class T1PROJECT_API FGsStateLocalPlayerAutoMove : 
	public FGsStateBaseSingleLocalPlayer,
	public TGsStateSingleton<FGsStateLocalPlayerAutoMove>
{
	GS_STATE_GENERATED_BODY(FGsStateBaseSingleLocalPlayer);

public:
	virtual void ReEnter(class UGsGameObjectBase* Owner) override;
	virtual void Update(class UGsGameObjectBase* Owner, float Delta) override;

	virtual void IgnoreState(UGsGameObjectBase* Owner, class IGsStateBase* ChangeState) override;
	
private:
	void MoveToReservedMoveData(class UGsGameObjectBase* Owner);
	bool IsShapeChangedLock(class UGsGameObjectBase* Owner) const;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/State/GsStateBase.h"


/**
* 
*/
class T1PROJECT_API FGsStateBaseSingleSandbag : public IGsStateBase
{
public:
	// 애님 블루프린트에 가장 최우선으로 상태를 전송해줘야한다.
	virtual void Enter(UGsGameObjectBase* Owner) override;
	virtual void ReEnter(UGsGameObjectBase* Owner) override {}
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override {}
	virtual void Exit(UGsGameObjectBase* Owner) override {}

	// NonPlayer State에 해당 되는 모든 State Getter
public:
	virtual IGsStateBase* GetState(uint8 StateID) const override;
};

/**
* 무브 관련 베이스 스테이트
*/
class T1PROJECT_API FGsStateSandbagMoveBase : public FGsStateBaseSingleSandbag
{
	using Super = FGsStateBaseSingleSandbag;

public:
	virtual void Enter(UGsGameObjectBase* Owner) override;
	virtual void Exit(UGsGameObjectBase* Owner) override;
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
};
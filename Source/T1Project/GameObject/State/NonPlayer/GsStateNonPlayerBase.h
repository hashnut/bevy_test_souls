// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GsStateBase.h"


/**
* NPC 관련 상태들은 상하체 분리 구조가 아닌걸 가정한다.
*/
class T1PROJECT_API FGsStateBaseSingleNonPlayer : public IGsStateBase
{
public:
	// 애님 블루프린트에 가장 최우선으로 상태를 전송해줘야한다.
	virtual bool Ready(UGsGameObjectBase* Owner) override;
	virtual void Enter(UGsGameObjectBase* Owner) override;
	virtual void ReEnter(UGsGameObjectBase* Owner) override {}
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override {}
	virtual void Exit(UGsGameObjectBase* Owner) override;

	// NonPlayer State에 해당 되는 모든 State Getter
public:
	virtual IGsStateBase* GetState(uint8 StateID) const override;
};

/**
* 무브 관련 베이스 스테이트
*/
class T1PROJECT_API FGsStateNonPlayerMoveBase : public FGsStateBaseSingleNonPlayer
{
	using Super = FGsStateBaseSingleNonPlayer;

public:
	virtual void Enter(UGsGameObjectBase* Owner) override;
	virtual void Exit(UGsGameObjectBase* Owner) override;
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
};
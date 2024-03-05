// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GsStateBase.h"

/**
 *  타 유저 관련 상태 처리 Base 클래스
 */
class T1PROJECT_API FGsStateBaseRemotePlayer : public IGsStateBase
{
public:
	//애님 블루프린트에 상태 변경을 전송
	virtual void Enter(UGsGameObjectBase* Owner) override;
	virtual void ReEnter(UGsGameObjectBase* Owner) override { }
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override {}
	virtual void Exit(UGsGameObjectBase* Owner) override;

protected:
	// animBP 스테이트 머신의 디폴트 상태 정의
	static uint8 GetDefaultStateId(UGsGameObjectBase* Owner)
	{
		return static_cast<uint8>(EGsStateBase::Idle);
	}

	// RemotePlayer State에 해당 되는 모든 State Getter
public:
	virtual IGsStateBase* GetState(uint8 StateID) const override;
};


// Move Base
class T1PROJECT_API FGsStateRemotePlayerMoveBase : public FGsStateBaseRemotePlayer
{
	using Super = FGsStateBaseRemotePlayer;

public:
	virtual void Enter(UGsGameObjectBase* Owner) override;
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
	virtual void Exit(UGsGameObjectBase* Owner) override;
	virtual void ReEnter(UGsGameObjectBase* Owner) override;

public:
	virtual FName GetBPStateName() const override
	{
		return TEXT("Move");
	}
protected:
	bool IsLock(class UGsGameObjectBase* Owner) const;
	bool IsShapeChangedLock(class UGsGameObjectBase* Owner) const;
};
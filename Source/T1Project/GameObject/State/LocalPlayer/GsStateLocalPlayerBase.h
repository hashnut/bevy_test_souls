// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GsStateBase.h"

/**
* Local캐릭터 Base State 부모 클래스
* 진입시 AnimBlueprint 상태 전환 변수를 동기화 시켜주는 공통 로직을 포함
* @see : void Enter(UGsGameObjectBase* Owner)
*/

class T1PROJECT_API FGsStateBaseSingleLocalPlayer : public IGsStateBase
{
protected:
	static float _staticLocalStateTimer;

public:
	//애님 블루프린트에 상태 변경을 전송
	virtual void Enter(UGsGameObjectBase* Owner) override;
	virtual void ReEnter(UGsGameObjectBase* Owner) override {}
	virtual void Update(UGsGameObjectBase* Owner, float Delta) override;
	virtual void Exit(UGsGameObjectBase* Owner) override;
	virtual bool Ready(UGsGameObjectBase* Owner) override;

public:
	// LocalPlayer State에 해당 되는 모든 State Getter
	// [ToDo] Static 클래스로 따로 제공 할것
	virtual IGsStateBase* GetState(uint8 StateID) const override;

protected:
	// 교착 상태에 빠졌을경우 해당 함수를 호출하여 복구 시도를 한다.
	virtual void Restore(UGsGameObjectBase* Owner) {}

	// 이 Return값을 false로 변경하면 데드락(교착상태) 체크에 포함되지 않음
	virtual bool CheckDeadlock() { return true; }
	virtual float GetCheckDelayTime() { return 600.f; }
};

/**
* Local 무브 관련 베이스 상태 클래스
* 캐릭터는 전방, 측면, 후방이동으로 분류(임의)
*/
class T1PROJECT_API FGsStateLocalPlayerMoveBase : public FGsStateBaseSingleLocalPlayer
{
	using Super = FGsStateBaseSingleLocalPlayer;

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

class T1PROJECT_API FGsStateLocalPlayerCommonAction
{
public:
	static void Enter(UGsGameObjectBase* Owner);
	static void Update(UGsGameObjectBase* Owner, float Delta);
	static void Exit(UGsGameObjectBase* Owner);
};
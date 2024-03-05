#pragma once
#include "Engine/Classes/Engine/EngineTypes.h"

class FGsAttackInfoBase
{
protected:
	// 키값
	int64 _gameId;
	
	bool _isAttack = false;

	FTimerHandle _timeHandleAttack;

public:
	FGsAttackInfoBase(int64 In_gameId) : _gameId(In_gameId) {}
	FGsAttackInfoBase() {}

	virtual ~FGsAttackInfoBase() = default;

	// virtual function
public:
	virtual float GetWaitTime() { return 0.0f; }

	virtual void EndTime();

	// 로직 함수
public:
	
	// 공격 시작(시간 저장)
	void StartAttack();

	

	void ClearTimer();

	void Finalize();
};
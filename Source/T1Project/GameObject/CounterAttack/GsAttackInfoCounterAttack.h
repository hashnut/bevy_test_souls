#pragma once
#include "Scan/GsAttackInfoBase.h"

class FGsAttackInfoCounterAttack : public FGsAttackInfoBase
{
	// constructor
public:
	FGsAttackInfoCounterAttack()
	{
	}
	virtual ~FGsAttackInfoCounterAttack() = default;

	// virtual function
public:
	virtual float GetWaitTime() override;
	virtual void EndTime() override;

	// logic function
public:
	void StartCounterAttackShow(int64 In_gameId);

	void UpdateUIHUD();

	void StopCountAttackShow();
public:
	int64 GetTargetGameId()
	{
		return _gameId;
	}	
	bool GetIsCounterAttackShow()
	{
		return _isAttack;
	}
};
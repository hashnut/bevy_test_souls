#pragma once
#include "CoreMinimal.h"

#include "Scan/GsAttackInfoBase.h"

/*
* 스캔에서 사용하는 공격자 정보(local을 공격)
* n초 이후 삭제(ex:10초)
*/
class FGsAttackInfoScan : public FGsAttackInfoBase
{
public:
	// call back func time end
	TFunction<void(int64)> CallbackTimeEnd;

	// 생성자
public:
	FGsAttackInfoScan(int64 In_gameId):
		FGsAttackInfoBase(In_gameId)
	{
		StartAttack();
	}

	virtual ~FGsAttackInfoScan() = default;

	// virtual function
public:
	virtual float GetWaitTime() override;	
	virtual void EndTime() override;

};
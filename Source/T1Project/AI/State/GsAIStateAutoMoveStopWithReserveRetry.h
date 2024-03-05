#pragma once
#include "AI/GsAIBaseState.h"

//----------------------------------
// auto move stop && reserve retry
//----------------------------------

class EGsAIStateAutoMoveStopWithReserveRetry final : public FGsAIBaseState
{
	// 생성자, 소멸자
public:
	virtual ~EGsAIStateAutoMoveStopWithReserveRetry() = default;

	// 가상함수
public:
	virtual void Enter() override;
	virtual void Update(float In_delta) override;

	virtual bool IsHighPriority() { return true; }
};

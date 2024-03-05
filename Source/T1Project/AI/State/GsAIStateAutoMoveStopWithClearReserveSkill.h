#pragma once
#include "AI/GsAIBaseState.h"

// auto move stop && clear skill reserve

class EGsAIStateAutoMoveStopWithClearReserveSkill final : public FGsAIBaseState
{
	// 생성자, 소멸자
public:
	virtual ~EGsAIStateAutoMoveStopWithClearReserveSkill() = default;

	// 가상함수
public:
	// 입장
	virtual void Enter() override;
	// 갱신
	virtual void Update(float In_deltaTime) override;

	virtual bool IsHighPriority() { return true; }
};
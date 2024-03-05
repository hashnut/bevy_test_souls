#pragma once
#include "AI/GsAIBaseState.h"

/*
	ai 상태: 대기 상태
*/

class FGsAIStateWaitSecond final : public FGsAIBaseState
{
private:
	// 대기 시작 시간
	int64 _waitStartTime;
	// 대기 최대 시간(초당 틱 곱한값)
	float _maxAIWaitSecond;
public:
	FGsAIStateWaitSecond() = default;
	virtual ~FGsAIStateWaitSecond() = default;

	// 가상함수
public:
	// 입장
	virtual void Enter() override;
	// 갱신
	virtual void Update(float In_deltaTime) override;
};

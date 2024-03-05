#pragma once
#include "AI/GsAIBaseState.h"

// auto move stop request

class EGsAIStateAutoMoveStop final : public FGsAIBaseState
{
	// 생성자, 소멸자
public:
	virtual ~EGsAIStateAutoMoveStop() = default;

	// 가상함수
public:
	// 입장
	virtual void Enter() override;
	// 갱신
	virtual void Update(float In_deltaTime) override;
};
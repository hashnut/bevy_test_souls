#pragma once

#include "AI/GsAIBaseState.h"

//-----------------------------------
// ai 상태: 대기 상태(조건 업데이트)
//-----------------------------------

class FGsAIStateIdle final : public FGsAIBaseState
{
	// 생성자, 소멸자
public :
	virtual ~FGsAIStateIdle() = default;

	// 가상함수
public:
	virtual void Update(float In_deltaTime) override;
};

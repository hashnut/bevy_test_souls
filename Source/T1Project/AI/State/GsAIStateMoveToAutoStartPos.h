#pragma once
#include "AI/GsAIBaseState.h"

//----------------------------------
// ai 상태: 제자리(저장된 위치)로 이동
//----------------------------------

class FGsAIStateMoveToAutoStartPos final : public FGsAIBaseState
{
	// 생성자, 소멸자
public:
	virtual ~FGsAIStateMoveToAutoStartPos() = default;

	// 가상함수
public:
	virtual void Enter() override;
	virtual void Update(float In_delta) override;
};

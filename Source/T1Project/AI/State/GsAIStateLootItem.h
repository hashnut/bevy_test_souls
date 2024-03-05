#pragma once

#include "AI/GsAIBaseState.h"

/*
	ai 상태: 루팅 아이템
*/

class FGsAIStateLootItem : public FGsAIBaseState
{
	// 생성자, 소멸자
public:
	virtual ~FGsAIStateLootItem() = default;

	// 가상함수
public:
	// 입장
	virtual void Enter() override;
	// 갱신
	virtual void Update(float In_deltaTime) override;
};
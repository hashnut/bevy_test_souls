#pragma once

#include "AI/GsAIBaseState.h"

//----------------------------
// ai 상태: 공격(검색, 이동, 공격 다처리)
// 기존 find anyone, move to target, attack 을 gameobject 내부에서 처리한다
//----------------------------

class FGsAIStateAttackAnyone final :public FGsAIBaseState
{
	// 생성자, 소멸자
public:
	virtual ~FGsAIStateAttackAnyone() = default;

	// 가상함수
public:
	// 입장
	virtual void Enter() override;
	// 갱신
	virtual void Update(float In_deltaTime) override;	
};
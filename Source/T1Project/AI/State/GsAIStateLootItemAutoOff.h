#pragma once

#include "AI/State/GsAIStateLootItem.h"

/*
* ai 상태: 루팅 아이템(auto off 일때, 퀘스트 자동 이동중에 흘린 아이템 줍기)
* https://jira.com2us.com/jira/browse/C2URWQ-8303
*/

class FGsAIStateLootItemAutoOff final : public FGsAIStateLootItem
{
public:
	virtual ~FGsAIStateLootItemAutoOff() = default;

	// 가상함수
public:
	// 입장
	virtual void Enter() override;
};
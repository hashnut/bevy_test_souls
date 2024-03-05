#pragma once
#include "GsStageLobbyBase.h"

//------------------------------------------------------------------------------
// 스테이지 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class FGsStageLobbyCharacterCreate : public FGsStageLobbyBase
{
public:
	FGsStageLobbyCharacterCreate();
	virtual ~FGsStageLobbyCharacterCreate();

protected:
	uint64 _envSoundId = 0;
public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;
};


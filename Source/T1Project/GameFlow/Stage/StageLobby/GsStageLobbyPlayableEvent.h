#pragma once
#include "GameFlow/Stage/StageLobby/GsStageLobbyBase.h"

// 로비-플레이에이블 이벤트 스테이지
class FGsStageLobbyPlayableEvent : public FGsStageLobbyBase
{
	using Super = FGsStageLobbyBase;

private:
	uint64 _envSoundId = 0;

public:
	FGsStageLobbyPlayableEvent();
	virtual ~FGsStageLobbyPlayableEvent() {};

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;
};
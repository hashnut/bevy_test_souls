#pragma once

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"

class FGsMovementHandlerPlayableEventLocalPlayer : public FGsMovementHandlerLocalPlayer
{
	using Super = FGsMovementHandlerLocalPlayer;
public:
	// 이동 서버 전송 안한다
	virtual void UpdateNetSender(float In_delta) override{}

protected:
	virtual bool ChangeState(FGsMovementState& inState) override;
};
#pragma once

#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "Input/GsInputEventMsgBase.h"

/*
* 플레이에이블 로컬 타겟 핸들러
*/

class FGsTargetHandlerPlayableEventLocalPlayer : public FGsTargetHandlerLocalPlayer
{
public:
	FGsTargetHandlerPlayableEventLocalPlayer() = default;
	virtual ~FGsTargetHandlerPlayableEventLocalPlayer() = default;
};
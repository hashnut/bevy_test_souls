#include "GsMovementHandlerPlayableEventLocalPlayer.h"

bool FGsMovementHandlerPlayableEventLocalPlayer::ChangeState(FGsMovementState& inState)
{
	// 바로 윗단 부모 클래스 local 에서는 서버 전송처리가 있어서
	// 더 윗단을 호출
	return FGsMovementHandlerBase::ChangeState(inState);
}
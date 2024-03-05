#include "GsMovementState.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

void FGsMovementState::SetDirection(const FVector& inDir)
{
	_direction = inDir.GetSafeNormal2D();
}

void FGsMovementState::SetMoveSpeed(StatValue inMoveSpeed)
{
	_moveSpeed = inMoveSpeed;
}

const FVector& FGsMovementState::GetDirection() const
{
	return _direction;
}

void FGsMovementState::Stop(UGsGameObjectCreature* inOwner, bool inSendFsmMessage)
{
	if (true == inSendFsmMessage)
	{
		if (inOwner)
		{
			FGsGameObjectStateManager* fsm = inOwner->GetBaseFSM();
			check(fsm);
			fsm->ProcessEvent(EGsStateBase::Idle);
		}
	}

	if (_stopFuncRouter.IsBound())
	{
		_stopFuncRouter.Broadcast();
	}
}
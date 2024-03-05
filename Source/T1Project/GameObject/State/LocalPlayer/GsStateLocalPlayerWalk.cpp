#include "GsStateLocalPlayerWalk.h"

#include "../../ObjectClass/GsGameObjectCreature.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GsStateLocalPlayerHeader.h"
#include "../../ObjectClass/GsGameObjectLocalPlayer.h"

uint8 FGsStateLocalPlayerWalk::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::ForwardWalk);
}

FName FGsStateLocalPlayerWalk::GetStateName() const
{
	return TEXT("StateLocalPlayerForwardWalk");
}

FName FGsStateLocalPlayerWalk::GetBPStateName() const
{
	return Super::GetBPStateName();
}

bool FGsStateLocalPlayerWalk::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_ACTION:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:		
		case EGsStateBase::Warp:
		case EGsStateBase::Idle:
		case EGsStateBase::Run:
		case EGsStateBase::AutoMove:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::ReactionEnd:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerWalk::Update(UGsGameObjectBase* Owner, float Delta)
{
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();

	EGsMovementState moveStateType = movement->GetState();
	if (EGsMovementState::INPUT == moveStateType)
	{
		if (movement->GetStateInputMove().IsRunnableScaleValue())
		{
			FGsGameObjectStateManager* fsm = local->GetBaseFSM();
			fsm->ProcessEvent(EGsStateBase::Run);
		}
	}
	else if (EGsMovementState::AUTO == moveStateType)
	{
		FGsGameObjectStateManager* fsm = local->GetBaseFSM();
		fsm->ProcessEvent(EGsStateBase::AutoMove);
	}
	else if (EGsMovementState::STOP == moveStateType)
	{
		FGsGameObjectStateManager* fsm = local->GetBaseFSM();
		fsm->ProcessEvent(EGsStateBase::Idle);
	}

	// Movement Update를 가장 나중에 한다.
	Super::Update(Owner, Delta);
}

void FGsStateLocalPlayerWalk::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);
}

void FGsStateLocalPlayerWalk::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

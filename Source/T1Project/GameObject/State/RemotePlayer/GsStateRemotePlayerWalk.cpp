// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerWalk.h"
#include "GsStateRemotePlayerHeader.h"

uint8 FGsStateRemotePlayerWalk::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::ForwardWalk);
}

FName FGsStateRemotePlayerWalk::GetStateName() const
{
	return TEXT("StateRemotePlayerWalk");
}

FName FGsStateRemotePlayerWalk::GetBPStateName() const
{
	return Super::GetBPStateName();
}

bool FGsStateRemotePlayerWalk::CanTransition(UGsGameObjectBase* Owner, int StateID) const
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
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::ReactionEnd:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerWalk::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);
}

void FGsStateRemotePlayerWalk::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

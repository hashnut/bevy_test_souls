// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateNonPlayerWalk.h"
#include "GsStateNonPlayerHeader.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"

uint8 FGsStateNonPlayerWalk::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::ForwardWalk);
}

FName FGsStateNonPlayerWalk::GetStateName() const
{
	return TEXT("StateNonPlayerWalk");
}

FName FGsStateNonPlayerWalk::GetBPStateName() const
{
	return TEXT("Move");
}

bool FGsStateNonPlayerWalk::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:
		case EGsStateBase::Attack:
		case EGsStateBase::Dying:
		case EGsStateBase::Reaction:
		case EGsStateBase::Social:
		case EGsStateBase::SocialAction:
		case EGsStateBase::RunAway:
		case EGsStateBase::ReactionEnd:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateNonPlayerWalk::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);
}

void FGsStateNonPlayerWalk::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

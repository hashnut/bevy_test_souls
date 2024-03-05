// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerIdle.h"
#include "GsStateRemotePlayerHeader.h"
#include "../../ObjectClass/GsGameObjectCreature.h"

uint8 FGsStateRemotePlayerIdle::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Idle);
}

FName FGsStateRemotePlayerIdle::GetStateName() const
{
	return TEXT("StateRemotePlayerIdle");
}

FName FGsStateRemotePlayerIdle::GetBPStateName() const
{
	return TEXT("Idle");
}

bool FGsStateRemotePlayerIdle::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		GS_STATE_GROUP_ACTION:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Warp:
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:		
		case EGsStateBase::ChangeIdle:
		case EGsStateBase::ChangeWeapon:
		case EGsStateBase::Spawn:
		case EGsStateBase::Social:
			return true;
		case EGsStateBase::Hit:
			return Owner->LegacyHit;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerIdle::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectCreature* castObj = Owner->CastGameObject<UGsGameObjectCreature>();
	castObj->ChangeIdleType(castObj->IsBattleMode() ? EGsAnimStateIdleType::Battle : EGsAnimStateIdleType::Normal);
}

void FGsStateRemotePlayerIdle::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

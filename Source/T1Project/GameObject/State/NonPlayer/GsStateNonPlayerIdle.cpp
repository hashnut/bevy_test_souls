// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateNonPlayerIdle.h"
#include "GsStateNonPlayerHeader.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "ActorEx/GsCharacterBase.h"

uint8 FGsStateNonPlayerIdle::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Idle);
}

FName FGsStateNonPlayerIdle::GetStateName() const
{
	return TEXT("StateNonPlayerIdle");
}

FName FGsStateNonPlayerIdle::GetBPStateName() const
{
	return TEXT("Idle");
}

bool FGsStateNonPlayerIdle::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:
		case EGsStateBase::Attack:
		case EGsStateBase::Dying:
		case EGsStateBase::Reaction:
		case EGsStateBase::Roar:
		case EGsStateBase::SocialAction:		
		case EGsStateBase::SpaceCrackIdle:
		case EGsStateBase::RunAway:
			return true;
		/*case EGsStateBase::Hit:
			return Owner->LegacyHit;*/
		case EGsStateBase::Social:
			return !CheckBattleIdle(Owner);
		default:
			return false;
		}
	}
	return false;
}

void FGsStateNonPlayerIdle::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectCreature* castObj = Owner->CastGameObject<UGsGameObjectCreature>();
	castObj->ChangeIdleType(castObj->IsBattleMode() ? EGsAnimStateIdleType::Battle : EGsAnimStateIdleType::Normal);

	if (AGsCharacterBase* character = Cast<AGsCharacterBase>(castObj->GetActor()))
	{
		character->IdleEvent();
	}
}

void FGsStateNonPlayerIdle::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

bool FGsStateNonPlayerIdle::CheckBattleIdle(UGsGameObjectBase* Owner) const
{
	if (auto creature = Owner->CastGameObject<UGsGameObjectCreature>())
		return creature->IsBattleMode();
	return false;
}
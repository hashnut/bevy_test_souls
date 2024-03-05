// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateNonPlayerSpaceCrackIdle.h"
#include "GsStateNonPlayerHeader.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "Animation/GsAnimInstanceState.h"

uint8 FGsStateNonPlayerSpaceCrackIdle::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Freeze);
}

FName FGsStateNonPlayerSpaceCrackIdle::GetStateName() const
{
	return TEXT("StateNonPlayerSpaceCrackIdle");
}

FName FGsStateNonPlayerSpaceCrackIdle::GetBPStateName() const
{
	return TEXT("SpaceCrackIdle");
}

bool FGsStateNonPlayerSpaceCrackIdle::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:
		case EGsStateBase::Attack:
		case EGsStateBase::Dying:
		case EGsStateBase::Reaction:
		case EGsStateBase::Roar:
		case EGsStateBase::Social:
		case EGsStateBase::SocialAction:
		case EGsStateBase::Hit:		
			return true;
		default:
			return false;
		}
	}

	return false;
}

void FGsStateNonPlayerSpaceCrackIdle::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectCreature* castObj = Owner->CastGameObject<UGsGameObjectCreature>();
	castObj->ChangeIdleType(castObj->IsBattleMode() ? EGsAnimStateIdleType::Battle : EGsAnimStateIdleType::Normal);

	if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
	{		
		anim->Pose();
	}
}

void FGsStateNonPlayerSpaceCrackIdle::Exit(UGsGameObjectBase* Owner)
{
	if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
	{
		anim->Resume();		
	}

	Super::Exit(Owner);
}

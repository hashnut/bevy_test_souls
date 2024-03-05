// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerActionEnd.h"
#include "GsStateLocalPlayerHeader.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "Animation/GsAnimInstanceState.h"

uint8 FGsStateLocalPlayerActionEnd::GetStateID() const
{
	// 현재는 AttackEnd, ReactionEnd, CommonActionEnd, UnFreeze 공용으로 쓰고 있는 클래스라서 StateID정의가 아직 모호함
	return static_cast<uint8>(EGsStateBase::None);
}

FName FGsStateLocalPlayerActionEnd::GetStateName() const
{
	return TEXT("StateLocalPlayerActionEnd");
}

FName FGsStateLocalPlayerActionEnd::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerActionEnd::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		// 같은 타입만 제외
		return !IsSameState(StateID);
	}
	return false;
}

void FGsStateLocalPlayerActionEnd::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

void FGsStateLocalPlayerActionEnd::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsAnimInstanceState* anim = Owner->GetAnimInstance();
	anim->StopMontagePlay();
}

void FGsStateLocalPlayerActionEnd::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		fsm->ProcessEvent(EGsStateBase::Idle);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateNonPlayerHit.h"
#include "GsStateNonPlayerHeader.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "../../ObjectClass/GsGameObjectBase.h"
#include "../ActorEx/GsCharacterBase.h"

uint8 FGsStateNonPlayerHit::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Hit);
}
FName FGsStateNonPlayerHit::GetStateName() const
{
	return TEXT("StateNonPlayerHit");
}
FName FGsStateNonPlayerHit::GetBPStateName() const
{
	return TEXT("Hit");
}

bool FGsStateNonPlayerHit::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	// Hit State는 전혀 중요하지않은 연출용이므로 모든 상태 전환을 허용
	return Super::CanTransition(Owner, StateID);
}

void FGsStateNonPlayerHit::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		if (false == fsm->IsDelayEvent())
		{
			Enter(Owner);
		}
	}
}

void FGsStateNonPlayerHit::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	FGsGameObjectStateManager* fsm = Owner->GetBaseFSM();
	AGsCharacterBase* characterActor = Owner->GetCharacter();
	if (fsm && characterActor)
	{
		if (UGsAnimInstanceState* anim = characterActor->GetAnim())
		{
			float aniLen = anim->GetStateAnimationLength(fsm->GetBPStateName(), GetBPStateName());
			fsm->DelayProcessEvent(aniLen, EGsStateBase::Hit, EGsStateBase::Idle);
		}
	}
}

void FGsStateNonPlayerHit::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

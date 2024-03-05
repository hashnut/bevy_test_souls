// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerHit.h"
#include "GsStateRemotePlayerHeader.h"
#include "Animation/GsAnimInstanceState.h"
#include "../../../ActorEx/GsCharacterBase.h"
#include "../../ObjectClass/GsGameObjectBase.h"

uint8 FGsStateRemotePlayerHit::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Hit);
}
FName FGsStateRemotePlayerHit::GetStateName() const
{
	return TEXT("StateRemotePlayerHit");
}
FName FGsStateRemotePlayerHit::GetBPStateName() const
{
	return TEXT("Hit");
}

bool FGsStateRemotePlayerHit::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	// Hit State는 전혀 중요하지않은 연출용이므로 모든 상태 전환을 허용
	return Super::CanTransition(Owner, StateID);
}

void FGsStateRemotePlayerHit::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);

	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		if (false == fsm->IsDelayEvent())
		{
			// Anim 상태머신 초기화
			UGsAnimInstanceState* anim = Owner->GetAnimInstance();
			anim->ResetCurrentState(fsm->GetBPStateName());

			Enter(Owner);
		}
	}
}

void FGsStateRemotePlayerHit::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	FGsGameObjectStateManager* fsm = Owner->GetBaseFSM();
	AGsCharacterBase* characterActor = Owner->GetCharacter();
	if (fsm && characterActor)
	{
		if (UGsAnimInstanceState* anim = characterActor->GetAnim())
		{
			float aniLen = anim->GetStateAnimationLength(fsm->GetBPStateName(), GetBPStateName());
			fsm->DelayProcessEvent(aniLen, EGsStateBase::Hit,  EGsStateBase::Idle);
		}
	}
}

void FGsStateRemotePlayerHit::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

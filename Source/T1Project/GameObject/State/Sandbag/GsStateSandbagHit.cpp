// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateSandbagHit.h"
#include "GsStateSandbagHeader.h"
#include "Animation/GsSandbagAnimInstance.h"
#include "Animation/GsAnimInstanceState.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "T1Project/Animation/GsAnimInstanceState.h"
#include "T1Project/ActorEx/GsCharacterBase.h"

uint8 FGsStateSandbagHit::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Hit);
}
FName FGsStateSandbagHit::GetStateName() const
{
	return TEXT("StateSandbagHit");
}
FName FGsStateSandbagHit::GetBPStateName() const
{
	return TEXT("Hit");
}

bool FGsStateSandbagHit::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:
		case EGsStateBase::Dying:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateSandbagHit::Enter(UGsGameObjectBase* Owner)
{
	//Super::Enter(Owner);

	if (UGsAnimInstanceState* anim = Owner->GetAnimInstance())
	{
		// AnimBP 스테이트와 연동된 상태는 AnimBP 에 전달한다.
		// 아닐 경우 디폴트 상태로 연동
		anim->ChangeState(IsMontagePlayState() ? GetDefaultStateId(Owner) : GetStateID(),
			GetAniRandomCount());
		
	}

	FGsGameObjectStateManager* fsm = Owner->GetBaseFSM();
	AGsCharacterBase* characterActor = Owner->GetCharacter();
	if (fsm && characterActor)
	{
		if (UGsAnimInstanceState* anim = characterActor->GetAnim())
		{
			FName statName = fsm->GetBPStateName();			
			float aniLen = anim->GetStateAnimationLength(statName, GetBPStateName());
			fsm->DelayProcessEvent(aniLen, EGsStateBase::Hit, EGsStateBase::Idle);
		}
	}
}

void FGsStateSandbagHit::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

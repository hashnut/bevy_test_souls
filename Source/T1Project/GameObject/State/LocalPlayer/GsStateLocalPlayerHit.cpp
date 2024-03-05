#include "GsStateLocalPlayerHit.h"

#include "../../ObjectClass/GsGameObjectBase.h"
#include "../GsGameObjectStateManager.h"
#include "ActorEx/GsCharacterBase.h"
#include "Animation/GsAnimInstanceState.h"
#include "GsStateLocalPlayerHeader.h"
#include "Animation/GsAnimInstanceState.h"

uint8 FGsStateLocalPlayerHit::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Hit);
}
FName FGsStateLocalPlayerHit::GetStateName() const
{
	return TEXT("StateLocalPlayerHit");
}
FName FGsStateLocalPlayerHit::GetBPStateName() const
{
	return TEXT("Hit");
}

bool FGsStateLocalPlayerHit::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	// Hit State는 전혀 중요하지않은 연출용이므로 모든 상태 전환을 허용
	return Super::CanTransition(Owner, StateID);
}

void FGsStateLocalPlayerHit::ReEnter(UGsGameObjectBase* Owner)
{
	Super::ReEnter(Owner);
	
	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		if (false ==fsm->IsDelayEvent())
		{
			Enter(Owner);
		}
	}
}

void FGsStateLocalPlayerHit::Enter(UGsGameObjectBase* Owner)
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

void FGsStateLocalPlayerHit::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

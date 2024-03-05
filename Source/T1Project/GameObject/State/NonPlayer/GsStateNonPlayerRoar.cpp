// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateNonPlayerRoar.h"
#include "GsStateNonPlayerHeader.h"
#include "../../ObjectClass/GsGameObjectNonPlayerBase.h"
#include "../../ObjectClass/GsGameObjectCreature.h"
#include "../../../ActorEx/GsCharacterBase.h"

uint8 FGsStateNonPlayerRoar::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Roar);
}

FName FGsStateNonPlayerRoar::GetStateName() const
{
	return TEXT("StateNonPlayerBattleRoar");
}

FName FGsStateNonPlayerRoar::GetBPStateName() const
{
	return TEXT("Roar");
}

bool FGsStateNonPlayerRoar::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:
		case EGsStateBase::Dying:
		case EGsStateBase::RunAway:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateNonPlayerRoar::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectNonPlayerBase* npc = Owner->CastGameObject<UGsGameObjectNonPlayerBase>();
	if (nullptr == npc)
	{
		return;
	}

	if (UGsAnimInstanceState* anim = npc->GetCharacter()->GetAnim())
	{
		if (FGsGameObjectStateManager* fsm = npc->GetBaseFSM())
		{
			// roar 후 배틀 idle로 돌아감
			float aniLen = anim->GetStateAnimationLength(fsm->GetBPStateName(), GetBPStateName());
			fsm->DelayProcessEvent(aniLen, EGsStateBase::Roar, EGsStateBase::Idle);
#ifdef TEST_DEBUG_LOG
			TestPrintAniEnd(Owner, aniLen);
#endif
#ifdef TEST_DEBUG_LOG
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 20.0f, FColor::Red,
					TEXT("FGsStateNonPlayerRoar enter")
				);
			}
#endif
		}
	}
}

void FGsStateNonPlayerRoar::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

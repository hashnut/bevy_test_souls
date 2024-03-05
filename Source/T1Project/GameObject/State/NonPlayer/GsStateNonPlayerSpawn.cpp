// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateNonPlayerSpawn.h"
#include "GsStateNonPlayerHeader.h"
#include "GameObject/Npc/GsSchemaNpcBalance.h"
#include "GameObject/Npc/GsSchemaNpcData.h"
#include "../../ObjectClass/GsGameObjectNonPlayerBase.h"
#include "../../ObjectClass/GsGameObjectBase.h"
#include "../ActorEx/GsCharacterBase.h"

uint8 FGsStateNonPlayerSpawn::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Spawn);
}

FName FGsStateNonPlayerSpawn::GetStateName() const
{
	return TEXT("StateNonPlayerSpawn");
}

FName FGsStateNonPlayerSpawn::GetBPStateName() const
{
	return TEXT("Spawn");
}

bool FGsStateNonPlayerSpawn::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:
		case EGsStateBase::Dying:
		case EGsStateBase::Reaction:
		case EGsStateBase::SpaceCrackIdle:
			return true;
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::Run:
		case EGsStateBase::Attack:
		case EGsStateBase::Roar:
		case EGsStateBase::Social:
		case EGsStateBase::SocialAction:
			return false == CanSpawnAction(Owner);		
		/*case EGsStateBase::Hit:
			return (Owner->LegacyHit) && (false == CanSpawnAction(Owner));*/
		default:
			return false;
		}
	}
	return false;
}

void FGsStateNonPlayerSpawn::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);
	
	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		if (AGsCharacterBase* characterActor = Owner->GetCharacter())
		{
			if (UGsAnimInstanceState* anim = characterActor->GetAnim())
			{
				float aniLen = anim->GetStateAnimationLength(fsm->GetBPStateName(), GetBPStateName());
				fsm->DelayProcessEvent(aniLen, EGsStateBase::Spawn, EGsStateBase::Idle);
			}
		}
	}
}

void FGsStateNonPlayerSpawn::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

bool FGsStateNonPlayerSpawn::CanSpawnAction(UGsGameObjectBase* Owner) const
{
	if (UGsGameObjectNonPlayerBase* castNpc = Owner->CastGameObject<UGsGameObjectNonPlayerBase>())
	{
		const FGsSchemaNpcData* npcTable = castNpc->GetNpcData();
		return npcTable->spawnActionTime > 0.f;
	}
	return false;
}
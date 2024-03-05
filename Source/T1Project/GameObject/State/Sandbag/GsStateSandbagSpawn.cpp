// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateSandbagSpawn.h"
#include "GsStateSandbagHeader.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"

uint8 FGsStateSandbagSpawn::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Spawn);
}

FName FGsStateSandbagSpawn::GetStateName() const
{
	return TEXT("StateSandbagSpawn");
}

FName FGsStateSandbagSpawn::GetBPStateName() const
{
	return TEXT("Spawn");
}

bool FGsStateSandbagSpawn::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:		
		case EGsStateBase::Dying:
		case EGsStateBase::Hit:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateSandbagSpawn::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		//sandbag은 보스 등급 몬스터가 존재하지 않는다
		fsm->DelayProcessEvent(1.5f, EGsStateBase::Spawn, EGsStateBase::Idle);
	}
}

void FGsStateSandbagSpawn::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

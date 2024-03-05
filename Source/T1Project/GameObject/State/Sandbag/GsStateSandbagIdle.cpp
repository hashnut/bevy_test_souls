// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateSandbagIdle.h"
#include "GsStateSandbagHeader.h"
#include "GameObject/ObjectClass/GsGameObjectCreature.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "Animation/GsSandbagAnimInstance.h"

uint8 FGsStateSandbagIdle::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Idle);
}

FName FGsStateSandbagIdle::GetStateName() const
{
	return TEXT("StateNonPlayerIdle");
}

FName FGsStateSandbagIdle::GetBPStateName() const
{
	return TEXT("Idle");
}

bool FGsStateSandbagIdle::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Dying:
		case EGsStateBase::Hit:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateSandbagIdle::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectCreature* castObj = Owner->CastGameObject<UGsGameObjectCreature>();
	if (castObj)
	{
		castObj->ChangeIdleType(castObj->IsBattleMode() ? EGsAnimStateIdleType::Battle : EGsAnimStateIdleType::Normal);
		UGsGameObjectNonPlayerBase* npc = Cast<UGsGameObjectNonPlayerBase>(castObj);
		if (npc)
		{
			npc->InvalidateQuestMark();
		}
	}	
}

void FGsStateSandbagIdle::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

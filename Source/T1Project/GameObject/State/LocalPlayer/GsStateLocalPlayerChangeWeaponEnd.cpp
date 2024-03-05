// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLocalPlayerChangeWeaponEnd.h"

uint8 FGsStateLocalPlayerChangeWeaponEnd::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::ChangeWeaponEnd);
}
FName FGsStateLocalPlayerChangeWeaponEnd::GetStateName() const
{
	return TEXT("StateLocalPlayerChangeWeaponEnd");
}
FName FGsStateLocalPlayerChangeWeaponEnd::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerChangeWeaponEnd::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		if (Super::CanTransition(Owner, StateID))
		{
			switch (static_cast<EGsStateBase>(StateID))
			{
			GS_STATE_GROUP_ABNORLITY:
			GS_STATE_GROUP_DIE:
			case EGsStateBase::Warp:
			case EGsStateBase::CommonActionEnd:
				return true;
			default:
				return false;
			}
		}
	}
	return false;
}

void FGsStateLocalPlayerChangeWeaponEnd::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);
	FGsGameObjectStateManager::ProcessEvent(Owner, EGsStateBase::CommonActionEnd);
}

void FGsStateLocalPlayerChangeWeaponEnd::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

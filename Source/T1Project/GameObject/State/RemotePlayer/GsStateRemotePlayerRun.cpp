// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateRemotePlayerRun.h"
#include "GsStateRemotePlayerHeader.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "../../ObjectClass/GsGameObjectBase.h"
#include "../../Define/GsGameObjectDefine.h"

uint8 FGsStateRemotePlayerRun::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Run);
}

FName FGsStateRemotePlayerRun::GetStateName() const
{
	return TEXT("StateRemotePlayerRun");
}

FName FGsStateRemotePlayerRun::GetBPStateName() const
{
	return Super::GetBPStateName();
}

bool FGsStateRemotePlayerRun::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{		
		GS_STATE_GROUP_ACTION_END:
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		GS_STATE_GROUP_ACTION:
			return true;
		case EGsStateBase::Warp:
		case EGsStateBase::Idle:
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::ChangeWeapon:
			return false == IsLock(Owner);
		default:
			return false;
		}
	}
	return false;
}

void FGsStateRemotePlayerRun::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);
}

void FGsStateRemotePlayerRun::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

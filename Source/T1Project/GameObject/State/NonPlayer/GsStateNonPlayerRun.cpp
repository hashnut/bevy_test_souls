// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateNonPlayerRun.h"
#include "GsStateNonPlayerHeader.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "../../ObjectClass/GsGameObjectBase.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

uint8 FGsStateNonPlayerRun::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Run);
}

FName FGsStateNonPlayerRun::GetStateName() const
{
	return TEXT("StateNonPlayerRun");
}

FName FGsStateNonPlayerRun::GetBPStateName() const
{
	return TEXT("Move");
}

bool FGsStateNonPlayerRun::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Idle:
		case EGsStateBase::Attack:		
			return false == IsLock(Owner);
		case EGsStateBase::Dying:
		case EGsStateBase::Reaction:
		case EGsStateBase::Social:
		case EGsStateBase::SocialAction:
		case EGsStateBase::RunAway:
		case EGsStateBase::ReactionEnd:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateNonPlayerRun::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);
}

void FGsStateNonPlayerRun::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

bool FGsStateNonPlayerRun::IsLock(UGsGameObjectBase* Owner) const
{
	if (FGsAbnormalityHandlerBase* abnormalityHandler = Owner->GetAbnormalityHandler())
	{
		return abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::CC_FEAR);
	}
	return false;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerRun.h"
#include "GsStateLocalPlayerHeader.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "../../ObjectClass/GsGameObjectLocalPlayer.h"

uint8 FGsStateLocalPlayerRun::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Run);
}

FName FGsStateLocalPlayerRun::GetStateName() const
{
	return TEXT("StateLocalPlayerRun");
}

FName FGsStateLocalPlayerRun::GetBPStateName() const
{
	return Super::GetBPStateName();
}

bool FGsStateLocalPlayerRun::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{		
		GS_STATE_GROUP_ABNORLITY:
		GS_STATE_GROUP_DIE:
		case EGsStateBase::Spawn:
		case EGsStateBase::Warp:
		case EGsStateBase::ReactionEnd:
			return true;
		// 특정 상태이상 체크
		GS_STATE_GROUP_ACTION:
		case EGsStateBase::Idle:
		case EGsStateBase::ForwardWalk:
		case EGsStateBase::AutoMove:
		case EGsStateBase::ChangeWeapon:		
			return false == IsLock(Owner);
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerRun::Update(UGsGameObjectBase* Owner, float Delta)
{
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	EGsMovementState moveStateType = movement->GetState();
	if (EGsMovementState::INPUT == moveStateType)
	{
		if (false == movement->GetStateInputMove().IsRunnableScaleValue())
		{
			FGsGameObjectStateManager* fsm = local->GetBaseFSM();
			fsm->ProcessEvent(EGsStateBase::ForwardWalk);
		}
	}
	else if(EGsMovementState::AUTO == moveStateType)
	{
		FGsGameObjectStateManager* fsm = local->GetBaseFSM();
		fsm->ProcessEvent(EGsStateBase::AutoMove);
	}
	else if (EGsMovementState::STOP == moveStateType)
	{
		FGsGameObjectStateManager* fsm = local->GetBaseFSM();
		fsm->ProcessEvent(EGsStateBase::Idle);
	}

	// Movement Update를 가장 나중에 한다.
	Super::Update(Owner, Delta);
}

void FGsStateLocalPlayerRun::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);
	
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	local->SetFairyVehicleCondition(EGsFairyVehicleCondition::MOVE_STATE);
}

void FGsStateLocalPlayerRun::Exit(UGsGameObjectBase* Owner)
{
	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	local->SetFairyVehicleCondition(EGsFairyVehicleCondition::MOVE_STATE, true);

	Super::Exit(Owner);
}

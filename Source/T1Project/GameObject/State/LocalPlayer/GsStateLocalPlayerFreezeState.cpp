// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerFreezeState.h"
#include "GsStateLocalPlayerHeader.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "../../ObjectClass/GsGameObjectLocalPlayer.h"

#include "Management/ScopeGame/GsAIManager.h"


uint8 FGsStateLocalPlayerFreezeState::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Freeze);
}

FName FGsStateLocalPlayerFreezeState::GetStateName() const
{
	return TEXT("StateLocalPlayerFreezeState");
}

FName FGsStateLocalPlayerFreezeState::GetBPStateName() const
{
	return TEXT("Freeze");
}

bool FGsStateLocalPlayerFreezeState::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::UnFreeze:
		case EGsStateBase::Warp:
		GS_STATE_GROUP_DIE:
			return true;

		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerFreezeState::Enter(UGsGameObjectBase* Owner)
{
	//GSLOG(Log , TEXT("FGsStateLocalPlayerFreezeState::Enter() ~~~~~~~~~~~~~"));
	if (UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>())
	{
		if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
		{
			movement->SetMoveable(false);
			movement->Stop();
		}
	}

	Super::Enter(Owner);

	/*
	UGsGameObjectLocalPlayer* local = Cast<UGsGameObjectLocalPlayer>(Owner);
	check(local);
	FGsGameObjectStateManager* fsm = local->GetBaseFSM();
	check(fsm);
	UGsAnimInstanceState* anim = local->GetLocalCharacter()->GetAnim();
	check(anim);

	float aniLen = anim->GetStateAnimationLength(fsm->GetBPStateName(), GetBPStateName());
	ChangeDelayState<FGsStateLocalPlayerIdle>(local->GetBaseFSM(), aniLen - 0.2f);
	*/

	// Freeze 吝俊绰 公利贸府 窃
	Owner->SetIsInvincible(true);

	GSAI()->SetAIStop(true);
}

void FGsStateLocalPlayerFreezeState::Exit(UGsGameObjectBase* Owner)
{
	//GSLOG(Log, TEXT("FGsStateLocalPlayerFreezeState::Exit() ~~~~~~~~~~~~~"));
	if (UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>())
	{
		if (FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
		{
			movement->SetMoveable(true);
		}
	}

	Owner->SetIsInvincible(false);

	GSAI()->SetAIStop(false);

	Super::Exit(Owner);
}
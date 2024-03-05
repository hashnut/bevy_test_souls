// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLocalPlayerAutoMoveStop.h"
#include "GsStateLocalPlayerHeader.h"
#include "../../ObjectClass/GsGameObjectLocalPlayer.h"
#include "../../Movement/GsMovementHandlerLocalPlayer.h"

uint8 FGsStateLocalPlayerAutoMoveStop::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::AutoMoveStop);
}
FName FGsStateLocalPlayerAutoMoveStop::GetStateName() const
{
	return TEXT("StateLocalPlayerAutoMoveStop");
}
FName FGsStateLocalPlayerAutoMoveStop::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerAutoMoveStop::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	return true;
}
void FGsStateLocalPlayerAutoMoveStop::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	FGsMovementHandlerLocalPlayer* movement = local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	movement->Stop();
}

void FGsStateLocalPlayerAutoMoveStop::Update(UGsGameObjectBase* Owner, float Delta)
{
	Super::Update(Owner, Delta);

	if (FGsGameObjectStateManager* fsm = Owner->GetBaseFSM())
	{
		fsm->ProcessEvent(EGsStateBase::Idle);
	}
}

void FGsStateLocalPlayerAutoMoveStop::Exit(UGsGameObjectBase* Owner)
{
	Super::Exit(Owner);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLocalPlayerRide.h"
#include "GsStateLocalPlayerHeader.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "../../ObjectClass/GsGameObjectLocalPlayer.h"

uint8 FGsStateLocalPlayerRide::GetStateID() const
{
	return static_cast<uint8>(EGsStateBase::Ride);
}

FName FGsStateLocalPlayerRide::GetStateName() const
{
	return TEXT("StateLocalPlayerRide");
}

FName FGsStateLocalPlayerRide::GetBPStateName() const
{
	return NAME_None;
}

bool FGsStateLocalPlayerRide::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateBase>(StateID))
		{
		case EGsStateBase::Dying:
		case EGsStateBase::Idle:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStateLocalPlayerRide::Enter(UGsGameObjectBase* Owner)
{
	Super::Enter(Owner);

	// 피직 정보 끄기
	//UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	//local->GetLocalCharacter()->DisableCollision();

	//// 컨트롤러 변경
	//if (APlayerController* controller = UGameplayStatics::GetPlayerController(Owner->GetWorld(), 0))
	//{
	//	controller->UnPossess();
	//	controller->Possess(local->GetVehicle()->GetWhellVehicle());
	//}
}

void FGsStateLocalPlayerRide::Exit(UGsGameObjectBase* Owner)
{
	// 컨트롤러 원복
	/*UGsGameObjectLocalPlayer* local = Owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (APlayerController* controller = UGameplayStatics::GetPlayerController(Owner->GetWorld(), 0))
	{
		controller->UnPossess();
		controller->Possess(local->GetLocalCharacter());
	}

	local->GetLocalCharacter()->EnableCollision();*/
	Super::Exit(Owner);
}

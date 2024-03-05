// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStatePropSpawn.h"
#include "Animation/GsPropAnimInstance.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"

uint8 FGsStatePropSpawn::GetStateID() const
{
	return static_cast<uint8>(EGsStateProp::Spawn);
}

FName FGsStatePropSpawn::GetStateName() const
{
	return TEXT("StatePropSpawn");
}

FName FGsStatePropSpawn::GetBPStateName() const
{
	return TEXT("Spawn");
}

bool FGsStatePropSpawn::CanTransition(UGsGameObjectBase* inOwner, int StateID) const
{
	if (Super::CanTransition(inOwner, StateID))
	{
		switch (static_cast<EGsStateProp>(StateID))
		{			
		case EGsStateProp::Idle:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStatePropSpawn::Enter(UGsGameObjectBase* inOwner)
{
	Super::Enter(inOwner);

	//spawn 상태에서는 interaction이 되지 말아야한다. 
	if (UGsGameObjectPropInteraction* prop = Cast<UGsGameObjectPropInteraction>(inOwner))
	{
		prop->SetCanInteraction(false);
		prop->SetQuestTargetActive(false);		
	}
}

void FGsStatePropSpawn::Exit(UGsGameObjectBase* inOwner)
{
	Super::Exit(inOwner);
}
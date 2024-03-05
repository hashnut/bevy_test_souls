// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStatePropIdle.h"
#include "Animation/GsPropAnimInstance.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "ActorEx/GsActorPropTouch.h"

uint8 FGsStatePropIdle::GetStateID() const
{
	return static_cast<uint8>(EGsStateProp::Idle);
}

FName FGsStatePropIdle::GetStateName() const
{
	return TEXT("StatePropIdle");
}

FName FGsStatePropIdle::GetBPStateName() const
{
	return TEXT("Idle");
}

bool FGsStatePropIdle::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateProp>(StateID))
		{
		case EGsStateProp::Spawn:
		case EGsStateProp::Action:		
		case EGsStateProp::Finish:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStatePropIdle::Enter(UGsGameObjectBase* inOwner)
{
	Super::Enter(inOwner);

	if (UGsGameObjectPropInteraction* prop = Cast<UGsGameObjectPropInteraction>(inOwner))
	{	
		prop->InvalidWidget();
		
		if (AActor* actor = prop->GetActor())
		{
			if (AGsActorPropTouch* propActor = Cast<AGsActorPropTouch>(actor))
			{
				if (propActor->GetAnim())
				{
					propActor->GetAnim()->ChangeState(EGsStateProp::Idle);
				}				
			}
		}
	}
}

void FGsStatePropIdle::Exit(UGsGameObjectBase* inOwner)
{
	if (UGsGameObjectPropInteraction* prop = Cast<UGsGameObjectPropInteraction>(inOwner))
	{
		prop->SetQuestTargetActive(false);
	}

	Super::Exit(inOwner);
}

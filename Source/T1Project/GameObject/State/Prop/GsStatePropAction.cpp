// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStatePropAction.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "Animation/GsPropAnimInstance.h"
#include "ActorEx/GsActorPropTouch.h"


uint8 FGsStatePropAction::GetStateID() const
{
	return static_cast<uint8>(EGsStateProp::Action);
}

FName FGsStatePropAction::GetStateName() const
{
	return TEXT("StatePropAction");
}

FName FGsStatePropAction::GetBPStateName() const
{
	return TEXT("Action");
}

bool FGsStatePropAction::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateProp>(StateID))
		{
		case EGsStateProp::Finish:
		case EGsStateProp::Idle:
			return true;
		default:
			return false;
		}
	}
	return false;
}

void FGsStatePropAction::Enter(UGsGameObjectBase* inOwner)
{
	Super::Enter(inOwner);
	
	UGsGameObjectPropInteraction* prop = Cast<UGsGameObjectPropInteraction>(inOwner);
	if (prop)
	{
		prop->StartActionTimer();

		if (AGsActorPropTouch* propActor = Cast<AGsActorPropTouch>(prop->GetActor()))
		{
			if (propActor->GetAnim())
			{
				propActor->GetAnim()->ChangeState(EGsStateProp::Action);
			}

			propActor->EventPropInteractionBegin();
		}					
	}
}

void FGsStatePropAction::Exit(UGsGameObjectBase* inOwner)
{
	UGsGameObjectPropInteraction* prop = Cast<UGsGameObjectPropInteraction>(inOwner);
	if (prop)
	{
		prop->PlayActionFinishEffect();

		if (AGsActorPropTouch* propActor = Cast<AGsActorPropTouch>(prop->GetActor()))
		{
			propActor->EventPropInteractionActionFinish();
		}		
	}	

	Super::Exit(inOwner);
}
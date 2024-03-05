// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStatePropFinish.h"
#include "Animation/GsPropAnimInstance.h"
#include "GameObject/ObjectClass/GsGameObjectPropInteraction.h"
#include "ActorEx/GsActorPropTouch.h"
#include "T1Project.h"

uint8 FGsStatePropFinish::GetStateID() const
{
	return static_cast<uint8>(EGsStateProp::Finish);
}

FName FGsStatePropFinish::GetStateName() const
{
	return TEXT("StatePropFinish");
}

FName FGsStatePropFinish::GetBPStateName() const
{
	return TEXT("Finish");
}

bool FGsStatePropFinish::CanTransition(UGsGameObjectBase* Owner, int StateID) const
{
	if (Super::CanTransition(Owner, StateID))
	{
		switch (static_cast<EGsStateProp>(StateID))
		{		
		case EGsStateProp::Spawn:
		case EGsStateProp::Idle:
			return true;
		default:
			return false;
		}		
	}
	return false;
}

void FGsStatePropFinish::Enter(UGsGameObjectBase* inOwner)
{
	Super::Enter(inOwner);

	//spawn 상태에서는 interaction이 되지 말아야한다. 
	if (UGsGameObjectPropInteraction* prop = Cast<UGsGameObjectPropInteraction>(inOwner))
	{
		prop->PlayFinishEffect();
		prop->SetCanInteraction(false);
		prop->SetQuestTargetActive(false);

		if (AActor* actor = prop->GetActor())
		{
			if (AGsActorPropTouch* propActor = Cast<AGsActorPropTouch>(actor))
			{
				if (propActor->GetAnim())
				{
					propActor->GetAnim()->ChangeState(EGsStateProp::Finish);					
#if false 
					GSLOG(Log, TEXT("FGsStatePropFinish - Enter - Change anim state finish - prop name : %s"), *actor->GetActorLabel());
#endif
				}				

				propActor->EventPropInteractionFinish();
			}
		}
	}
}

void FGsStatePropFinish::Exit(UGsGameObjectBase* inOwner)
{
	Super::Exit(inOwner);
}

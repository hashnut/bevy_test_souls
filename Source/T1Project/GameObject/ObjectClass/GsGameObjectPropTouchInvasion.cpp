// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/ObjectClass/GsGameObjectPropTouchInvasion.h"
#include "ActorEx/GsActorInvasionPropTouch.h"
#include "Net/GsNetSendServiceWorld.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsUnlockManager.h"

EGsGameObjectType UGsGameObjectPropTouchInvasion::GetObjectType() const
{
	return EGsGameObjectType::PropInteractInvasion;
}

void UGsGameObjectPropTouchInvasion::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectPropTouchInvasion::GetObjectType());
}

void UGsGameObjectPropTouchInvasion::InitializeActor(AActor* inOwner)
{
	Super::InitializeActor(inOwner);

	if (_propInstantStart.Value.IsValid())
	{
		GMessage()->GetGameObject().Remove(_propInstantStart);
	}

	_propInstantStart = GMessage()->GetGameObject().AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_INVASIONPROP_START,
		this, &UGsGameObjectPropTouchInvasion::InstantStartPropInteraction);
}

void UGsGameObjectPropTouchInvasion::SetInteractionActive(bool In_isActive, int In_Index /*= 0*/)
{
	Super::SetInteractionActive(In_isActive, In_Index);

	if (IsForceHideInteractionButton())
	{
		if (auto invasionPropActor = Cast<AGsActorInvasionPropTouch>(GetActor()))
		{
			invasionPropActor->SetDeactive();
			invasionPropActor->SetCallbackInvasion(nullptr);
		}
	}
	else
	{
		SetAciveCallbackInvasion(true); // _touchAction == InvasionTouchAction::Invasion);
	}
}

void UGsGameObjectPropTouchInvasion::SetInteractPropEnd(bool inSuccess)
{
	_isInteracting = false;
	if (!inSuccess)
		 InvalidWidget();
	else HideWidget();

	//if (!inSuccess && _touchAction == InvasionTouchAction::EnrollingIn)
	//{
	//	SetAciveCallbackInvasion(false);
	//}
}

void UGsGameObjectPropTouchInvasion::SetAciveCallbackInvasion(bool active)
{
	//InvasionTouchAction state = active ? InvasionTouchAction::Invasion : InvasionTouchAction::Register;
	//_touchAction = state;
	
	if (auto invasionPropActor = Cast<AGsActorInvasionPropTouch>(GetActor()))
	{
		auto curState = invasionPropActor->GetInvasionPropState();
		if(curState != InvasionPropState::ActiveIdle && active)
		{
			invasionPropActor->SetActive();
			invasionPropActor->SetCallbackInvasion([this](bool In_isByWidget) {
				OnClickInteract(In_isByWidget);
				});
		}
		else if(curState != InvasionPropState::DeActiveIdle && !active)
		{
			invasionPropActor->SetDeactive();
			invasionPropActor->SetCallbackInvasion(nullptr);
		}
	}
}

void UGsGameObjectPropTouchInvasion::SetInvasionInteractEnd(bool isSuccess)
{
	FGsPropInteractionMessageParamContents param(GetData()->GetGameId(), GetData()->GetTableId());
	GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_CONTENTS_BY_INVASIONPROP_END, &param);
}

void UGsGameObjectPropTouchInvasion::UpdatePropState()
{
	if (auto invastionActor = Cast<AGsActorInvasionPropTouch>(GetActor()))
	{
		invastionActor->UpdateInvaderInfoPlate();
	}
}

void UGsGameObjectPropTouchInvasion::InvalidWidget()
{
	Super::InvalidWidget();


	//if (_touchAction != InvasionTouchAction::Register)
	//{	
	//	SetInteractionActive(false);
	//}
}

bool UGsGameObjectPropTouchInvasion::IsContentsUnlock()
{
	return GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::INVADE_ENTER, true);
}

void UGsGameObjectPropTouchInvasion::InstantStartPropInteraction(const struct IGsMessageParam* inMessage)
{
	const FGsPropInteractionMessageParamContents* param = 
		inMessage != nullptr ? inMessage->Cast<const FGsPropInteractionMessageParamContents>() : nullptr;

	if (_touchAction == InvasionTouchAction::Invasion && param != nullptr && param->_gameId == GetGameId())
	{
		FGsNetSendServiceWorld::SendInteractInvasion(GetData()->GetGameId());
	}
	
}

void UGsGameObjectPropTouchInvasion::InstantcEndPropInteraction(const struct IGsMessageParam* inMessage)
{
	
}

void UGsGameObjectPropTouchInvasion::OnStartInvasionInterAct()
{
	_touchAction = InvasionTouchAction::Invasion;
	SetAciveCallbackInvasion(true);
	//GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_BUTTON_CLICK, &_interactionMessageParam);
}

void UGsGameObjectPropTouchInvasion::OnCancelInvasionInterAct()
{
	_touchAction = InvasionTouchAction::Register;
	SetAciveCallbackInvasion(true);
}
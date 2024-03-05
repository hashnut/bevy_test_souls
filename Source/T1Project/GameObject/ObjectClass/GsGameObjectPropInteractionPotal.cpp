
#include "GsGameObjectPropInteractionPotal.h"

#include "Management/GsMessageHolder.h"
#include "../Management/ScopeGlobal/GsQuestManager.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"

#include "GameFramework/Actor.h"
#include "GsGameObjectPropInteraction.h"
#include "ActorEx/GsActorPropTouch.h"
#include "ActorEx/GsActorEnteredDungeonPropTouch.h"
#include "UI/UIContent/HUD/GsUIHUDPropTouchProgress.h"
#include "UI/UIContent/Billboard/GsUIBillboardEliteDungeonWarp.h"

#include "GsGameObjectLocalPlayer.h"
#include "../Interaction/GsInteractionHandlerLocalPlayer.h"

#include "DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"





void UGsGameObjectPropInteractionPotal::InitializeActor(AActor* InOwner)
{
	UGsGameObjectPropInteraction::InitializeActor(InOwner);

	_isInteracting = false;

	if (nullptr == InOwner)
	{
		return;
	}

	AGsActorPropTouch* actor = Cast<AGsActorPropTouch>(InOwner);
	if (nullptr == actor)
	{
		return;
	}

	actor->SetInteractionClickCallback([this](bool In_isByWidget)
		{
			OnClickInteract(In_isByWidget);
		}
	);

	// 2022/08/08 PKT - quest 타입 아님.
	SetQuestMarkActive(false);
	// 2022/08/08 PKT - default 
	SetInteractionActive(true);


	/**
	 * Remove Message
	 */
	if (_propProgressStart.Value.IsValid())
	{
		GMessage()->GetGameObject().Remove(_propProgressStart);
	}

	if (_propProgressEnd.Value.IsValid())
	{
		GMessage()->GetGameObject().Remove(_propProgressEnd);
	}

	if (_propProgressCancel.Value.IsValid())
	{
		GMessage()->GetGameObject().Remove(_propProgressCancel);
	}

	/**
	 * Add Message
	 */
	_propProgressStart = GMessage()->GetGameObject().AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_START, this, &UGsGameObjectPropInteractionPotal::OnStartPropInteraction);
	_propProgressEnd = GMessage()->GetGameObject().AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_END, this, &UGsGameObjectPropInteractionPotal::OnEndPropInteraction);
	_propProgressCancel = GMessage()->GetGameObject().AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_CANCEL, this, &UGsGameObjectPropInteractionPotal::OnEndPropInteraction);

	_progressWidget = nullptr;
	_interactionHandler = nullptr;
	_destinationData = nullptr;

	actor->SetActorHiddenInGame(false);

	if (auto widget = actor->GetWidget())
	{
		widget->SetTarget(this);
	}
}

void UGsGameObjectPropInteractionPotal::OnClickInteract(bool In_isByWidget)
{
	if (this->GetData())
	{
		GSQuest()->ClearAutoProgressQuestKey();
		GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
		
		if (const FGsGameObjectData* data = GetData())
		{
			FGsPropInteractionMessageParamContents param(data->GetGameId(), data->GetTableId());
			GMessage()->GetDungeon().SendMessage(MessageContentDungeon::INTERACTION_PROP_PORTAL, &param);
		}
	}
}


void UGsGameObjectPropInteractionPotal::OnStartPropInteraction(const IGsMessageParam* inMessage)
{
	const FGsPropInteractionMessageParamContents* param 
		= (inMessage) ? static_cast<const FGsPropInteractionMessageParamContents*>(inMessage) : nullptr;

	if (nullptr == param || param->_gameId != GetGameId())
	{
		return;
	}

	// 2022/08/08 PKT - Handler
	SetInteractionHandler();

	// 2022/08/08 PKT - UI Widget
	SetProgressWidget();

	SetPotalHUDName();
	
	//Start Prop Interaction
	_isInteracting = true;
}

void UGsGameObjectPropInteractionPotal::OnEndPropInteraction(const IGsMessageParam* inMessage)
{
	UGsGameObjectPropTouch::EndPropInteraction(inMessage);
}

void UGsGameObjectPropInteractionPotal::SetPotalHUDName()
{
	if (_progressWidget)
	{
		_progressWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		FText name = FText::GetEmpty();

		if (_propData && PropType::ELITE_DUNGEON_PORTAL_HIDDEN == _propData->propType)
		{	// 2023/7/28 PKT - Hidden
			name = _propData->nameString;
		}
		else if (_destinationData)
		{	// 2023/7/28 PKT - 상층으로 가는 프랍
			name = FText::FromString(_destinationData->nameText);
		}

		_progressWidget->SetName(name);
	}
}

const FGsSchemaMapData* UGsGameObjectPropInteractionPotal::GetDestinationMapData() const
{
	return _destinationData;
}

void UGsGameObjectPropInteractionPotal::SetDestinationMapData(const struct FGsSchemaMapData* InDestinationData)
{
	_destinationData = InDestinationData;
	
	SetProgressWidget();

	SetPotalHUDName();
}

int32 UGsGameObjectPropInteractionPotal::GetUseCount() const
{	// 2022/08/08 PKT - Interaction이 끝난 카운트가 사용한 카운트가 된다.
	return _interactionEndUserCount;
}

void UGsGameObjectPropInteractionPotal::SendInteractionEnd(const struct FGsPropInteractionMessageParamContents* inParam)
{
	
}

void UGsGameObjectPropInteractionPotal::UpdatePropState()
{
	if (auto actor = Cast<AGsActorEnteredDungeonPropTouch>(GetActor()))
	{
		if (auto plate = Cast<UGsUIBillboardEliteDungeonWarp>(actor->GetWidget()))
		{
			plate->UpdateCount();
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectPropTouch.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Widget.h"

#include "ActorEx/GsActorDropObject.h"

#include "ActorComponentEx/EGsMeshShaderEffect.h"

#include "Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsItemUtil.h"
#include "UTIL/GsUIUtil.h"

#include "GsTableManager.h"
#include "WidgetComponent.h"

#include "UI/UIContent/Billboard/GsUIBillboardDropObject.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/Billboard/GsUIBillboardPropTouch.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

#include "ActorEx/GsActorPropObject.h"
#include "ActorEx/GsActorPropTouch.h"

#include "GameFramework/Actor.h"
#include "GameObject/DropObject/GsSchemaDropObject.h"
#include "GameObject/DropObject/GsSchemaDropObjectEffect.h"
#include "GameObject/Prop/GsSchemaPropData.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/Interaction/GsInteractionHandlerBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"

#include "Net/GsNetSendServiceWorld.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "DataSchema/Map/Spawn/GsSchemaSpawnElement.h"

#include "Data/GsDataContainManager.h"

#include "ActorComponentEx/GsMeshShaderControlComponent.h"

#include "GsGameObjectPlayer.h"
#include "GsGameObjectLocalPlayer.h"

#include "T1Project/UI/UIContent/HUD/GsUIHUDPropTouchProgress.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "Animation/GsPropAnimInstance.h"

#include "SharedInclude/SharedTypes.h"


EGsGameObjectType UGsGameObjectPropTouch::GetObjectType() const
{
	return Super::GetObjectType();
}

void UGsGameObjectPropTouch::Initialize(const FGsSpawnParam* inSpawnParam)
{
	Super::Initialize(inSpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectPropTouch::GetObjectType());
	
	_interactionMessageParam._objectType = GetObjectType();
	_interactionMessageParam._targetGameID = GetGameId();

	_isInitVisible = inSpawnParam->_isInitVisible;
	_isInteracting = false;
	_isCanActiveInteractionWidget = true;

	//bak1210 : final 에서호출
	CreateSubClass();
	InitializeFSM();
}

void UGsGameObjectPropTouch::Update(float Delta)
{
	Super::Update(Delta);	

	if(_fsm)
	{

		if (_fsm->IsState((uint8)EGsStateProp::Idle))
		{
			UpdateInteractionProgress();
		}

		if (GSInteraction()->GetIsDrawInteractionRange())
		{
			DrawDebugInteractionRange();
		}
	}

}

void UGsGameObjectPropTouch::InitializeActor(AActor* inOwner)
{
	Super::InitializeActor(inOwner);

	if (nullptr == inOwner)
	{
		return;
	}

	AGsActorPropTouch* actor = Cast<AGsActorPropTouch>(inOwner);
	if (nullptr == actor)
	{
		return;
	}

	actor->SetInteractionClickCallback([this](bool In_isByWidget)
	{
		OnClickInteract(In_isByWidget);
	});
	
	//https://jira.com2us.com/wiki/pages/viewpage.action?pageId=300560554 와는 무관한 quest ui 초기화인데... 왜 막혔을까요? 
	// 이유가 있을 듯 하여 invalidate에서 처리합니다
	//SetQuestTargetActive(false);

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

	_propProgressStart = GMessage()->GetGameObject().AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_START, this, &UGsGameObjectPropTouch::StartPropInteraction);
	_propProgressEnd = GMessage()->GetGameObject().AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_END, this, &UGsGameObjectPropTouch::EndPropInteraction);
	_propProgressCancel = GMessage()->GetGameObject().AddUObject(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_CANCEL, this, &UGsGameObjectPropTouch::EndPropInteraction);

	_progressWidget = nullptr;
	_interactionHandler = nullptr;	
		
	actor->SetActorHiddenInGame(!_isInitVisible);
	
	if (auto widget = actor->GetWidget())
	{
		widget->SetTarget(this);
	}
	
}

void UGsGameObjectPropTouch::ActorDespawn()
{
	AGsActorPropTouch* actor = Cast<AGsActorPropTouch>(_actor);
	if (actor)
	{
		actor->SetInteractionClickCallback(nullptr);

		GMessage()->GetGameObject().Remove(_propProgressStart);
		GMessage()->GetGameObject().Remove(_propProgressEnd);
		GMessage()->GetGameObject().Remove(_propProgressCancel);
	}

	//interaction 도중 prop이 디스폰 할 경위 interaction 취소 
	if (UGsGameObjectBase* findLocal = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		UGsGameObjectLocalPlayer* castLocal = findLocal->CastGameObject<UGsGameObjectLocalPlayer>();
		if (castLocal)
		{
			FGsInteractionHandlerBase* interationHandler = castLocal->GetInteractionHandler();
			if (interationHandler)
			{
				UGsGameObjectBase* target = interationHandler->GetInteractionTarget();
				if (target && target == this)
				{
					interationHandler->CancelInteration(target);					
				}
			}
		}
	}


	_progressWidget = nullptr;
	_interactionHandler = nullptr;

	Super::ActorDespawn();
}

void UGsGameObjectPropTouch::OnClickInteract(bool In_isByWidget)
{
	GSQuest()->ClearAutoProgressQuestKey();
	GMessage()->GetContentsHud().SendMessage(MessageContentHud::INVALIDATE_QUEST);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::INTERACTION_BUTTON_CLICK, &_interactionMessageParam);
}


void UGsGameObjectPropTouch::DrawDebugInteractionRange()
{
	if (GetWorld() && _propData)
	{
		// 시야 거리
		DrawDebugCircle(GetWorld(), GetActor()->GetActorLocation(),
			_propData->touchInteractionRange, 100, FColor::Red, false, 1.f,
			0, 0.0f, FVector(1.f, 0.f, 0.f), FVector(0.f, 1.f, 0.f));
	}
}

void UGsGameObjectPropTouch::StartPropInteraction(const struct IGsMessageParam* inMessage)
{
	if (nullptr == inMessage)
	{
		return;
	}

	const FGsPropInteractionMessageParamContents* param = static_cast<const FGsPropInteractionMessageParamContents*>(inMessage);
	int64 gameId = GetGameId();	

	if (nullptr == param
		|| param->_gameId != gameId)
	{
		return;
	}

	SetInteractionHandler();
	SetProgressWidget();	

	if (_progressWidget)
	{
		_progressWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_progressWidget->SetName(_propData->nameString);
	}

	//start prop interaction
	_isInteracting = true;

	InvalidWidget();
}

void UGsGameObjectPropTouch::EndPropInteraction(const struct IGsMessageParam* inMessage)
{
	if (nullptr == inMessage)
	{
		return;
	}

	const FGsPropInteractionMessageParamContents* param = static_cast<const FGsPropInteractionMessageParamContents*>(inMessage);
	if (nullptr == param
	&& param->_gameId != GetGameId())
	{
		return;		
	}	

	HideInteractProgressWidget();

	//InvalidWidget();
}

void UGsGameObjectPropTouch::CancelPropInteraction()
{
	SetProgressWidget();
	
	if (_progressWidget)
	{
		_progressWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	_isInteracting = false;
	_isCanActiveInteractionWidget = true;

	InvalidWidget();
}

void UGsGameObjectPropTouch::SetInteractionActive(bool In_isActive, int In_Index)
{
	AGsActorPropTouch* actor = Cast<AGsActorPropTouch>(_actor);
	if (nullptr == actor)
	{
		return;
	}

	UGsUIBillboardPropTouch* interactionButton = actor->GetPropTouchButton();
	UWidgetComponent* widgetComponent = actor->GetInteractionWidgetComponent();
	if (nullptr == widgetComponent ||
		nullptr == interactionButton)
	{
		return;
	}

	if (IsForceHideInteractionButton())
	{
		actor->SetInteractionClickCallback(nullptr);

		widgetComponent->SetActive(false);
		widgetComponent->SetVisibility(false);

		_isInteractionActive = false;

		actor->SetUseInteractionButton(false);
		interactionButton->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		//Spawn 상태일 수 있으므로 상태는 저장한다
		_isInteractionActive = In_isActive;

		//상태가 Idle이고 터치할 수 있는 자리가 있고 퀘스트 타겟이라면 켜진다
		// none 이 아니면 델리게이트 연결
		// 그리고 세팅 한 값으로 액티브
		if (In_isActive
			&& IsCanInteractionInternal())
		{
			actor->SetInteractionClickCallback([this](bool In_isByWidget)
				{
					OnClickInteract(In_isByWidget);
				});

			widgetComponent->SetActive(true);
			widgetComponent->SetVisibility(true);

			_isInteractionActive = true;

			actor->SetUseInteractionButton(true);
			interactionButton->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			actor->SetInteractionClickCallback(nullptr);

			widgetComponent->SetActive(false);
			widgetComponent->SetVisibility(false);

			_isInteractionActive = false;

			actor->SetUseInteractionButton(false);
			interactionButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}	
}

bool UGsGameObjectPropTouch::IsForceHideInteractionButton()
{
	return GGameData()->IsInvadeWorld()
	|| _isCanActiveInteractionWidget == false;
}

void UGsGameObjectPropTouch::StartFadeIn()
{
	if (AGsActorPropTouch* actor = Cast<AGsActorPropTouch>(_actor))
	{
		if (_fsm 
		&& GData()->GetGlobalData())
		{			
			_fsm->ProcessEvent((uint8)EGsStateProp::Spawn);

			UCurveFloat* curve = GData()->GetGlobalData()->_propDissolveCurve;
			if (curve)
			{
				actor->GetMeshShaderComponent()->Add(EGsMeshShaderEffect::Dissolve
					, actor->_propMesh
					, curve
					, [this]() {			
					_fsm->ProcessEvent((uint8)EGsStateProp::Idle);
				});

				actor->GetMeshShaderComponent()->Start(true);
			}			
		}		
	}
}

void UGsGameObjectPropTouch::SetProgressWidget()
{
	if (nullptr != _progressWidget)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("HUDMain"), true);
	if (widget.IsValid())
	{
		if (UGsUIHUDFrameMain* mainWidget = Cast<UGsUIHUDFrameMain>(widget.Get()))
		{
			_progressWidget = mainWidget->GetPropInteractionProgress();
		}
	}
}

void UGsGameObjectPropTouch::SetInteractionHandler()
{
	if (nullptr != _interactionHandler)
	{
		return;
	}
	
	UGsGameObjectBase* find = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == find)
	{
		return;
	}

	if (UGsGameObjectLocalPlayer* localPlayer = Cast<UGsGameObjectLocalPlayer>(find))
	{
		_interactionHandler = static_cast<FGsInteractionHandlerLocalPlayer*>(localPlayer->GetInteractionHandler());					
	}
}

void UGsGameObjectPropTouch::UpdateInteractionProgress()
{			
	if (_interactionHandler
	&& _progressWidget)
	{		
		_progressWidget->SetRatio(_interactionHandler->GetRatio());
		_progressWidget->SetTime(_interactionHandler->GetTimerInfo().GetRemainTime());
	}
}

void UGsGameObjectPropTouch::SetVisibleInfo(const FGsSchemaSpawnElement* inVisible)
{
	_isInitVisible = inVisible->InitVisible;
	_questCondtion.Empty();

	QuestId startQuestId;
	QuestId endQuestId;
	FGsSpawnObjectShowHide temp;
	int32 num = inVisible->QuestCondition.Num();

	for (const FGsSchemaSpawnObjectShowHide& questCondition : inVisible->QuestCondition)
	{				
		startQuestId = (questCondition.StartConditionQuestId.GetRow() ? questCondition.StartConditionQuestId.GetRow()->id : 0);
		endQuestId = (questCondition.EndConditionQuestId.GetRow() ? questCondition.EndConditionQuestId.GetRow()->id  : 0);

		temp.StartConditionQuestId = startQuestId;
		temp.EndConditionQuestId = endQuestId;
		temp.QuestCheckType = questCondition.QuestCheckType;

		_questCondtion.Add(temp);		
	}

	_visibleType = inVisible->VisibleType;

	UpdateVisible();
}

void UGsGameObjectPropTouch::UpdateVisible()
{	
	bool isVisible = GSQuest()->IsVisibleShowInfoArray(_questCondtion,
		_isInitVisible, _visibleType);
	if (GetActor())
	{
		GetActor()->SetActorHiddenInGame(!isVisible);
	}
}
// visible 바로 세팅(대사 연출 관련 처리)
void UGsGameObjectPropTouch::SetGameObjectActorVisible(bool In_isVisible)
{

	bool isVisible = false;
	// 키는거면 퀘스트 조건 체크후 켬
	if (In_isVisible == true)
	{
		isVisible =
			GSQuest()->IsVisibleShowInfoArray(_questCondtion,
				_isInitVisible, _visibleType);
	}
	Super::SetGameObjectActorVisible(isVisible);
	
}

bool UGsGameObjectPropTouch::IsCanInteractionInternal()
{
	return _propData
		&& _canInteraction
		&& _fsm && _fsm->IsState((uint8)EGsStateProp::Idle);
}

void UGsGameObjectPropTouch::HideInteractProgressWidget()
{
	if (_progressWidget)
	{
		_progressWidget->SetVisibility(ESlateVisibility::Hidden);
		_progressWidget->SetName(_propData->nameString);
	}
}

void UGsGameObjectPropTouch::SendInteractionStart(const struct FGsPropInteractionMessageParamContents* inParam)
{
	FGsNetSendServiceWorld::SendInteractPropStart(GetGameId());
}

void UGsGameObjectPropTouch::SendInteractionEnd(const struct FGsPropInteractionMessageParamContents* inParam)
{
	FGsNetSendServiceWorld::SendInteractPropEnd(GetGameId());
}
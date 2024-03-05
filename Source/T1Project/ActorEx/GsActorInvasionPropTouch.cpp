// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorEx/GsActorInvasionPropTouch.h"
#include "Components/SphereComponent.h"
#include "UMG/Public/Components/WidgetComponent.h"
#include "GameObject/Define/GsGameObjectText.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "UI/UIContent/Billboard/GsUIBillboardInvasionWarp.h"
#include "ActorComponentEx/GsWidgetComponentButton.h"
#include "../Management/ScopeGlobal/GsSoundManager.h"
//#include "UMG/Public/Components/WidgetInteractionComponent.h"

AGsActorInvasionPropTouch::AGsActorInvasionPropTouch()
{
	_rootWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetRoot"));
	_rootWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.f));
	_rootWidget->SetupAttachment(RootComponent);

	_rootWidget->SetActive(true);

	_nameplateWidget = CreateDefaultSubobject<UWidgetComponent>(NameplateWidgetComponentName);
	_nameplateWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	_nameplateWidget->SetWidgetSpace(EWidgetSpace::Screen);
	_nameplateWidget->SetDrawSize(FVector2D(200, 100.0f));
	_nameplateWidget->SetCastShadow(false);
	_nameplateWidget->SetupAttachment(_rootWidget);

	_interactionWidgetInvasion = CreateDefaultSubobject<UGsWidgetComponentButton>(TEXT("InteractionEnterWidget"));
	_interactionWidgetInvasion->SetWidgetSpace(EWidgetSpace::Screen);
	_interactionWidgetInvasion->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	_interactionWidgetInvasion->SetDrawSize(FVector2D(200, 100.0f));
	_interactionWidgetInvasion->SetCastShadow(false);
	_interactionWidgetInvasion->SetupAttachment(_rootWidget);
}

void AGsActorInvasionPropTouch::BeginPlay()
{
	Super::BeginPlay();

	if (const UGsGlobalConstant* globalData = GData()->GetGlobalData())
	{
		if (UClass* widgetClass = GPath()->GetClassByObjectPath(globalData->_portalNamePlatePath))
		{
			_nameplateWidget->SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
		}

		if (UClass* widgetClass = GPath()->GetClassByObjectPath(globalData->_portaInvasionTouchPath))
		{
			_interactionWidgetInvasion->SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
		}
	}

	//SetActive(); //default active
}

UGsUIBillboardInvasionWarp* AGsActorInvasionPropTouch::GetInvaderInfoPlate() const
{
	return Cast<UGsUIBillboardInvasionWarp>(GetWidget());
}

void AGsActorInvasionPropTouch::UpdateInvaderInfoPlate()
{
	if (auto namePlate = GetInvaderInfoPlate())
	{
		namePlate->UpdateInvaderInfo();
	}
}

UGsUIBillboard* AGsActorInvasionPropTouch::GetWidget() const
{
	if (_nameplateWidget && _nameplateWidget->GetUserWidgetObject())
	{
		return Cast<UGsUIBillboard>(_nameplateWidget->GetUserWidgetObject());
	}

	return nullptr;
}

void AGsActorInvasionPropTouch::OnChangedState(InvasionPropState curState)
{
	if (_curIdleState == curState) return;
	_curIdleState = curState;

	_interactionWidgetComponent->SetActive(_curIdleState != InvasionPropState::ActiveIdle);
	_interactionWidgetInvasion->SetActive(_curIdleState == InvasionPropState::ActiveIdle);
	if (_curIdleState == InvasionPropState::ActiveIdle)
	{
		if(_interactionWidgetInvasion->GetUserWidgetObject() == nullptr)
			_interactionWidgetComponent->InitWidget();

		if (auto widget = _interactionWidgetInvasion->GetUserWidgetObject())
		{
			widget->SetVisibility(ESlateVisibility::Visible);
			widget->SetIsEnabled(true);
		}

		_interactionWidgetComponent->SetHiddenInGame(true);
		_interactionWidgetInvasion->SetHiddenInGame(false);

	//	if (auto soundPlayer = GSoundPlayer())
	//	{
	//		soundPlayer->PlaySound2D(TEXT("UI_Invade_Portal"));
	//	}
	}
	else if (_curIdleState == InvasionPropState::DeActiveIdle)
	{
		_interactionWidgetComponent->SetHiddenInGame(false);
		_interactionWidgetInvasion->SetHiddenInGame(true);
	}
}


void AGsActorInvasionPropTouch::SetCallbackInvasion(TFunction<void(bool)> In_callback)
{
	_callbackInvasioinClick = In_callback;
	if (_interactionWidgetInvasion != nullptr)
	{
		if (UGsWidgetComponentButton* wcButton = Cast<UGsWidgetComponentButton>(_interactionWidgetInvasion))
		{
			wcButton->SetClickCallback(In_callback);
		}
	}
}

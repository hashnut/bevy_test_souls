// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorPropTouch.h"
#include "Engine/CollisionProfile.h"
#include "DrawDebugHelpers.h"
#include "WidgetComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"
#include "UI/UIContent/Billboard/GsUIBillboardPropTouch.h"
#include "ActorComponentEx/GsMeshShaderControlComponent.h"
#include "ActorComponentEx/GsWidgetComponentButton.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Data/GsDataContainManager.h"
#include "Components/BoxComponent.h"
#include "Data/GsGlobalConstant.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Animation/GsPropAnimInstance.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "UTIL/GsLevelUtil.h"
#include "ActorComponentEx/EGsMeshShaderEffect.h"
#include "GameObject/Define/GsGameObjectText.h"
#include "ActorComponentEx/GsEffectComponent.h"

#define  ACTOR_PROP_TOUCH_DEBUG false

AGsActorPropTouch::AGsActorPropTouch()
{
	PrimaryActorTick.bCanEverTick = true;

	_boxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	if (_boxComponent)
	{		
		SetRootComponent(_boxComponent);
		_boxComponent->SetHiddenInGame(true);
	}

	//prop shape
	_propMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (_propMesh)
	{
		//_propMesh->SetupAttachment(RootComponent);
		_propMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

	_meshShaderComponent = CreateDefaultSubobject<UGsMeshShaderControlComponent>(TEXT("MeshShader"));
	
	//interaction
	_interactionWidgetComponent = CreateDefaultSubobject<UGsWidgetComponentButton>(TEXT("InteractionWidget"));
	if (_interactionWidgetComponent)
	{
		_interactionWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		_interactionWidgetComponent->SetDrawSize(FVector2D(100, 100.0f));
		_interactionWidgetComponent->SetCastShadow(false);		
		_interactionWidgetComponent->SetupAttachment(RootComponent);
	}

	//quest
	_questTargetMark = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QuestMarkMesh"));
	if (_questTargetMark)
	{
		_questTargetMark->SetCastShadow(false);
		_questTargetMark->SetupAttachment(RootComponent);
	}

	_effectComponent = CreateDefaultSubobject<UGsEffectComponent>(NameEffectComponent);
}

void AGsActorPropTouch::BeginPlay()
{
	Super::BeginPlay();

	SetCollision();
	SetInteractionLocation();

	if (const UGsGlobalConstant* globalData = GData()->GetGlobalData())
	{
		//set quest mark
		if (_questTargetMark)
		{
			FSoftObjectPath path = globalData->_questTargetMark;
			UStaticMesh* mesh = GResource()->LoadSync<UStaticMesh>(path);
			if (mesh)
			{
				_questTargetMark->SetStaticMesh(mesh);
			}
		}	

		if (_interactionWidgetComponent)
		{
			UClass* widgetClass = GPath()->GetClassByObjectPath(globalData->_propTouchPath);
			if (widgetClass)
			{
				_interactionWidgetComponent->SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
			}			
		}
	}	
	
	FVector location;
	if (UGsLevelUtil::TryGetNearestLocationOnGround(GetWorld(), location, this))
	{					
#if WITH_EDITOR && ACTOR_PROP_TOUCH_DEBUG
		FString actorLabel = GetActorLabel();
		DrawDebugSphere(GetWorld(), location, 100.0f, 12, FColor::Yellow, true);
		GSLOG(Log, TEXT("Pre Prop location - name : %s \tlocation : %s"), *actorLabel, *location.ToString());
#endif
		
		float height = _boxComponent->GetScaledBoxExtent().Z;// / 2.0f;
		location += FVector(0, 0, height);		

#if WITH_EDITOR && ACTOR_PROP_TOUCH_DEBUG
		GSLOG(Log, TEXT("Next Prop location - name : %s \theight : %f"), *actorLabel, height);
		DrawDebugSphere(GetWorld(), location, 100.0f, 12, FColor::Blue, true);
		GSLOG(Log, TEXT("Next Prop location - name : %s \tlocation : %s"), *actorLabel, *location.ToString());
#endif

		SetActorLocation(location);		
	}
}

void AGsActorPropTouch::EndPlay(EEndPlayReason::Type inReason)
{
	_callbackInteractionClick = nullptr;

	if (EEndPlayReason::Destroyed == inReason)
	{
		if (_effectComponent)
		{
			_effectComponent->DestroyComponent();
		}
		_effectComponent = nullptr;
	}

	Super::EndPlay(inReason);
}

void AGsActorPropTouch::SetInteractionLocation()
{
	if (nullptr == _propMesh)
	{
		return;
	}

	if (_propMesh->SkeletalMesh)
	{
		FTransform trans = _propMesh->GetSocketTransform(TEXT("OverHeadSocket"), RTS_Component);
		if (const UGsGlobalConstant* data = GData()->GetGlobalData())
		{
			FVector interactionLocation = trans.GetTranslation() + data->_interactionButtonPosOffset;
			FVector progressLocation = GetActorLocation() - data->_interactionButtonPosOffset;
			// 퀘스트 타겟 마크
			if (_questTargetMark)
			{
				_questTargetMark->SetRelativeLocation(interactionLocation);
			}

			if (_interactionWidgetComponent)
			{
				_interactionWidgetComponent->SetRelativeLocation(interactionLocation);
			}
		}
	}
}

void AGsActorPropTouch::SetCollision()
{

	if (_boxComponent)
	{
		_boxComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	}
}

//Interaction interface에 의해 호출 된다. 
void AGsActorPropTouch::ClickInteraction()
{
	if (_callbackInteractionClick)
	{
		_callbackInteractionClick(false);
	}
}

UGsUIBillboardPropTouch* AGsActorPropTouch::GetPropTouchButton() const
{
	if (_interactionWidgetComponent
		&& _interactionWidgetComponent->GetUserWidgetObject())
	{
		return Cast<UGsUIBillboardPropTouch>(_interactionWidgetComponent->GetUserWidgetObject());
	}

	return nullptr;
}

void AGsActorPropTouch::SetUseInteractionButton(bool inUse)
{
	if (inUse)
	{
		if (nullptr == _interactionWidgetComponent->GetUserWidgetObject())
		{
			_interactionWidgetComponent->InitWidget();
		}
		_interactionWidgetComponent->SetHiddenInGame(false);
	}
	else
	{
		_interactionWidgetComponent->SetHiddenInGame(true);
	}	
}

void AGsActorPropTouch::InitMeshShaderComponent()
{
	if (_meshShaderComponent)
	{
		_meshShaderComponent->Initialize();
	}
}

UGsPropAnimInstance* AGsActorPropTouch::GetAnim()
{
	if (_propMesh)
	{
		return Cast<UGsPropAnimInstance>(_propMesh->GetAnimInstance());
	}

	return nullptr;
}

void AGsActorPropTouch::EventPropInteractionBegin_Implementation()
{

}

void AGsActorPropTouch::EventPropInteractionActionFinish_Implementation()
{

}

void AGsActorPropTouch::EventPropInteractionFinish_Implementation()
{

}

void AGsActorPropTouch::StartMeshShader(EGsMeshShaderEffect inType, bool inIsRestor /*= false*/)
{
	if (GetMeshShaderComponent())
	{
		UCurveFloat* curve = GData()->GetGlobalData()->_propDissolveCurve;
		if (curve)
		{
			GetMeshShaderComponent()->Add(inType
				, _propMesh
				, curve
				, nullptr
				, inIsRestor);
			GetMeshShaderComponent()->Start();
		}
	}
}
void AGsActorPropTouch::SetInteractionClickCallback(TFunction<void(bool)> In_callback)
{ 
	_callbackInteractionClick = In_callback; 
	if (_interactionWidgetComponent != nullptr)
	{
		if (UGsWidgetComponentButton* wcButton = Cast<UGsWidgetComponentButton>(_interactionWidgetComponent))
		{
			wcButton->SetClickCallback(In_callback);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "GsActorDropObject.h"
#include "WidgetComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameObject/ObjectClass/GsGameObjectDropObject.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ActorComponentEx/GsDropObjectSpawnComponent.h"
#include "ActorComponentEx/GsWidgetComponentButton.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"

// Sets default values
AGsActorDropObject::AGsActorDropObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (_mesh)
	{
		_mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_mesh->SetCollisionProfileName(TEXT("NoCollision"));
		_mesh->SetVisibility(true);
	}

	_widget = CreateDefaultSubobject<UGsWidgetComponentButton>(TEXT("Widget"));
	if (_widget)
	{
		_widget->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_widget->SetRelativeLocation(FVector(0, 0, 100.0f));
		_widget->SetWidgetSpace(EWidgetSpace::Screen);
		_widget->SetVisibility(true);
	}

	_spawnComponent = CreateDefaultSubobject<UGsDropObjectSpawnComponent>(TEXT("SpawnEffect"));

	_gradeParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("GradeEffect"));
	if (_gradeParticleSystemComponent)
	{
		_gradeParticleSystemComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_gradeParticleSystemComponent->SetVisibility(true);
	}

	_dropItemEffectParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DropItemEffect"));
	if (_dropItemEffectParticleSystemComponent )
	{
		_dropItemEffectParticleSystemComponent ->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_dropItemEffectParticleSystemComponent ->SetVisibility(true);
	}

	_sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	if (_sphereCollision)
	{
		_sphereCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_sphereCollision->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
		_sphereCollision->SetCollisionProfileName(TEXT("DropObject"));

#if WITH_EDITOR
		_sphereCollision->SetVisibility(true);
#else
		_sphereCollision->SetVisibility(false);
#endif
	}
}

void AGsActorDropObject::BeginPlay()
{
	Super::BeginPlay();

	_sphereCollision->SetCollisionProfileName(TEXT("DropObject"));

#if DROP_OBJECT_DEBUG
	_sphereCollision->SetHiddenInGame(false);
#endif
}

// Called when the game starts or when spawned
void AGsActorDropObject::EndPlay(const EEndPlayReason::Type inReason)
{
	if (_sphereCollision)
	{
		_sphereCollision->OnInputTouchBegin.Clear();
	}

	Super::EndPlay(inReason);
}

void AGsActorDropObject::Active(bool inSpawn) const
{
	if (_mesh)
	{
		_mesh->SetVisibility(inSpawn);
	}

#if DROPOBJECT_PICKING_ACTIVE
	if (_widget)
	{
		if (inSpawn)
		{
			//_sphereCollision->SetActive(true);
			_widget->SetActive(true);
		}
		else
		{
			//_sphereCollision->SetActive(false);
			_widget->SetActive(false);
		}
	}
#endif

	if (_gradeParticleSystemComponent)
	{
		_gradeParticleSystemComponent->SetVisibility(inSpawn);
	}

	if (_dropItemEffectParticleSystemComponent)
	{
		_dropItemEffectParticleSystemComponent->SetVisibility(inSpawn);
	}
}

void AGsActorDropObject::StartSpawn(const FVector& inStart, const FVector& inDest)
{
	Active(false);

	if (_spawnComponent)
	{
		_spawnComponent->Start(inStart, inDest);
	}
}

void AGsActorDropObject::SetInteractionClickCallback(TFunction<void(bool)> In_callback)
{
	if (_widget)
	{
		_widget->SetClickCallback(In_callback);
	}
}

//void AGsActorDropObject::ClickInteraction()
//{
//#if false
//	// TODO: 구현
//	// 가장 가까우면서 룻 가능한 오브젝트 찾음
//	if (UGsGameObjectDropObject* findDrop = GSGameObject()->FindDropObjectByActor(this))
//	{
//		// 메세지 Send
//		FGsInteractionMessageParam messageParam(findDrop->GetGameId()
//			, findDrop->GetObjectType()
//			, findDrop->GetItemAmount()
//			, false);
//		GMessage()->GetGameObject().SendMessage(MessageGameObject::DROPOBJECT_PICK, &messageParam);
//	}
//#endif
//}

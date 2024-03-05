// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger/GsFootPrintEffectTriggerActor.h"

#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Runtime/Engine/Classes/Components/ShapeComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "T1Project/ActorEx/GsCharacterLocalPlayer.h"
#if WITH_EDITOR
#include "T1Project/ActorComponentEx/WidgetComponentEx.h"
#include "Classes/Particles/ParticleSystem.h"
#include "Editor/Widget/GsEditorWidget.h"
#endif
#include "../Editor/Widget/GsEditorWidget.h"
#include "UMG/Public/Components/TextBlock.h"

// Sets default values
AGsFootPrintEffectTriggerActor::AGsFootPrintEffectTriggerActor() : ABaseTriggerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Fransform 정보가 초기화 되는 ScenComponent
	_Root = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));

	RootComponent = _Root;

}

void AGsFootPrintEffectTriggerActor::OnCollsionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (nullptr == FootPrintEffect)
		return;

	if (nullptr == OtherActor)
		return;
	
	if (AGsCharacterPlayer* playerActor = Cast<AGsCharacterPlayer>(OtherActor))
	{
		playerActor->AddFootPrintEffectByTrigger(FootPrintEffect, FootPrintMaterial);
	}
}

void AGsFootPrintEffectTriggerActor::OnCollsionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (nullptr == FootPrintEffect)
		return;

	if (nullptr == OtherActor)
		return;

	if (AGsCharacterPlayer* playerActor = Cast<AGsCharacterPlayer>(OtherActor))
	{
		playerActor->RemoveFootPrintEffectByTrigger(FootPrintEffect, FootPrintMaterial);
	}
}

void AGsFootPrintEffectTriggerActor::BeginPlay()
{
	ABaseTriggerActor::BeginPlay();

	Collision = Cast<UShapeComponent>(GetDefaultSubobjectByName(TEXT("Trigger")));

	if (Collision)
	{
		if (false == Collision->OnComponentBeginOverlap.IsBound())
		{
			Collision->OnComponentBeginOverlap.AddDynamic(this, &AGsFootPrintEffectTriggerActor::OnCollsionBeginOverlap);
		}
		if (false == Collision->OnComponentEndOverlap.IsBound())
		{
			Collision->OnComponentEndOverlap.AddDynamic(this, &AGsFootPrintEffectTriggerActor::OnCollsionEndOverlap);
		}
		

		// bak1210 : 처리 확인용(BP에서 편집가능하도록 풀어놓는다.
		/*Collision->SetCollisionProfileName(TEXT("FootPrint"));
		Collision->SetCanEverAffectNavigation(false);
		Collision->AreaClass = nullptr;
		Collision->bApplyImpulseOnDamage = false;*/

#if WITH_EDITOR
		Collision->SetVisibility(true);
		Collision->bHiddenInGame = false;

		EditorWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
#else
		Collision->SetVisibility(false);
		Collision->bHiddenInGame = true;
		
		Collision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
#endif
	}

}

void AGsFootPrintEffectTriggerActor::BeginDestroy()
{
	if (Collision)
	{
		if (Collision->OnComponentBeginOverlap.IsBound())
		{
			Collision->OnComponentBeginOverlap.RemoveDynamic(this, &AGsFootPrintEffectTriggerActor::OnCollsionBeginOverlap);
		}
		if (Collision->OnComponentEndOverlap.IsBound())
		{
			Collision->OnComponentEndOverlap.RemoveDynamic(this, &AGsFootPrintEffectTriggerActor::OnCollsionEndOverlap);
		}
	}

	Super::BeginDestroy();
}

#if WITH_EDITOR
void AGsFootPrintEffectTriggerActor::RefreshWidgetText()
{
	if (EditorWidgetComp)
	{
		if (UUserWidget* widget = EditorWidgetComp->GetUserWidgetObject())
		{
			if (UGsEditorWidget* editorWidget = Cast<UGsEditorWidget>(widget))
			{
				FString log = "None Effect";
				if (nullptr != FootPrintEffect)
				{
					FootPrintEffect->GetName(log);
				}
				editorWidget->DescriptionText->SetText(FText::FromString(log));
			}
		}
	}
}

void AGsFootPrintEffectTriggerActor::SetVisible(bool inVisible)
{
	if (nullptr != Collision)
	{
		Collision->SetVisibility(inVisible);
		EditorWidgetComp->SetVisibility(inVisible);
	}
}

#endif
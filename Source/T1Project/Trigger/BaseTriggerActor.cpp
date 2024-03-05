// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTriggerActor.h"
#include "T1Project/ActorEx/GsCharacterLocalPlayer.h"
#include "T1Project/ActorComponentEx/WidgetComponentEx.h"

// Sets default values
ABaseTriggerActor::ABaseTriggerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITOR
	EditorWidgetComp = CreateDefaultSubobject<UWidgetComponentEx>(TEXT("EidtorWidgetComp"));

	if (EditorWidgetComp)
	{
		EditorWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		EditorWidgetComp->_enableFaceToCamera = true;

		EditorWidgetComp->SetVisibility(true, true);
	}
#endif
}

void ABaseTriggerActor::OnConstruction(const FTransform& InTransform)
{
#if WITH_EDITOR
	if (EditorWidgetComp)
	{
		EditorWidgetComp->SetDrawSize(WidgetScreenSize);
	}
#endif
}

void ABaseTriggerActor::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();

#if WITH_EDITOR
	RefreshWidget();
#endif
}

void ABaseTriggerActor::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	RefreshWidget();
#endif
}

bool ABaseTriggerActor::IsPlayer(AActor* in_actor) const
{
	if (in_actor)
	{
		return in_actor->IsA(AGsCharacterLocalPlayer::StaticClass());
	}

	return false;
}

#if WITH_EDITOR
void ABaseTriggerActor::RefreshWidget()
{
	if (EditorWidgetComp)
	{
		FTransform trs = GetTransform();
		FVector scale = trs.GetScale3D();
		FVector widgetScale = FVector::OneVector / scale;

		EditorWidgetComp->SetRelativeScale3D(widgetScale);
		EditorWidgetComp->SetDrawSize(WidgetScreenSize);
		RefreshWidgetLocation();
		RefreshWidgetText();
	}
}

void ABaseTriggerActor::RefreshWidgetLocation()
{
	if (EditorWidgetComp)
	{		
		EditorWidgetComp->SetWorldLocation(GetWidgetLocation());		
	}
}

void ABaseTriggerActor::RefreshWidgetText()
{
	
}

void ABaseTriggerActor::SetVisible(bool inVisible)
{

}

#endif
// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorWarp.h"
#include "Components/SphereComponent.h"
#include "WidgetComponent.h"
#include "ActorComponentEx/GsDropObjectSpawnComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameMode/GsGameModeTestClient.h"

// Sets default values
AGsActorWarp::AGsActorWarp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	

	_sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	if (_sphereCollision)
	{
		_sphereCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_sphereCollision->SetRelativeLocation(FVector::ZeroVector);
		_sphereCollision->bHiddenInGame = true;
	}
}

void AGsActorWarp::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	if (GetWorld()->GetAuthGameMode()->IsA(AGsGameModeTestClient::StaticClass()))
	{
		SetActorHiddenInGame(true);
	}
#endif
}

void AGsActorWarp::EndPlay(EEndPlayReason::Type inType)
{
	_sphereCollision->OnComponentBeginOverlap.Clear();
	_sphereCollision->OnComponentEndOverlap.Clear();

	Super::EndPlay(inType);
}

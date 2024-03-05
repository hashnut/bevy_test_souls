// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorPropObject.h"
#include "Components/SphereComponent.h"

// Sets default values
AGsActorPropObject::AGsActorPropObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

// Called when the game starts or when spawned
void AGsActorPropObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGsActorPropObject::BeginDestroy()
{
	Super::BeginDestroy();
}

// Called every frame
void AGsActorPropObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


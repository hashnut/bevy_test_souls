// Fill out your copyright notice in the Description page of Project Settings.


#include "GsActorLobbyObject.h"

// Sets default values
AGsActorLobbyObject::AGsActorLobbyObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGsActorLobbyObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGsActorLobbyObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
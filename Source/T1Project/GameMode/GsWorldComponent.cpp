// Fill out your copyright notice in the Description page of Project Settings.


#include "GsWorldComponent.h"
#include "Management/GsMessageholder.h"
#include "GsGameModeWorld.h"
#include "GSGameInstance.h"


// Sets default values for this component's properties
UGsWorldComponent::UGsWorldComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UGsWorldComponent::BeginPlay()
{
	Super::BeginPlay();	
}

// GsGameModeWorld 
void UGsWorldComponent::Init()
{	
	GMessage()->GetStage().SendMessage(MessageStage::ENTER_TOWN);
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
}
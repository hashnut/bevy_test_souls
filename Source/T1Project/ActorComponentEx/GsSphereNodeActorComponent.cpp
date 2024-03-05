// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponentEx/GsSphereNodeActorComponent.h"

#include "SphereTree/GsSphereTree.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

// Sets default values for this component's properties
UGsSphereNodeActorComponent::UGsSphereNodeActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGsSphereNodeActorComponent::BeginPlay()
{
	Super::BeginPlay();

	FBoxSphereBounds ownerBound = GetOwner()->GetComponentsBoundingBox();

	if (nullptr != GSGameObject())
	{
		_pNodeData = GSGameObject()->GetSphereHandler()->AddSphere(GetOwner()->GetActorLocation(), ownerBound.GetSphere().W);
	}
	
}


void UGsSphereNodeActorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	if (nullptr != GSGameObject() && nullptr != _pNodeData)
	{
		GSGameObject()->GetSphereHandler()->Remove(_pNodeData);
		_pNodeData = nullptr;
	}
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UGsSphereNodeActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (nullptr != _pNodeData)
	{
		_pNodeData->NewPos(GetOwner()->GetActorLocation());
	}
}


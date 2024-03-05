// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorEx/GsCampNavModifier.h"
#include "NavigationSystem/Public/NavModifierComponent.h"


// Sets default values
AGsCampNavModifier::AGsCampNavModifier() : Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGsCampNavModifier::BeginPlay()
{
	Super::BeginPlay();

}

void AGsCampNavModifier::InvalidateModifier()
{
	TArray<UActorComponent*> findComps = GetComponentsByTag(UNavModifierComponent::StaticClass(), TEXT("CampNavModifier"));
	if (findComps.IsValidIndex(0))
	{
		UNavModifierComponent* findComp = Cast<UNavModifierComponent>(findComps[0]);
		if (nullptr != findComp)
		{
			findComp->UpdateNavigationBounds();
			findComp->RefreshNavigationModifiers();
		}
	}
}

void AGsCampNavModifier::SetNavModifierSize(FVector InSize)
{
	if (InSize.IsZero())
		return;

	TArray<UActorComponent*> findComps = GetComponentsByTag(UNavModifierComponent::StaticClass(), TEXT("CampNavModifier"));
	if (findComps.IsValidIndex(0))
	{
		UNavModifierComponent* findComp = Cast<UNavModifierComponent>(findComps[0]);
		if (nullptr != findComp)
		{
			findComp->FailsafeExtent.X = InSize.X;
			findComp->FailsafeExtent.Y = InSize.Y;
			findComp->UpdateNavigationBounds();
			findComp->RefreshNavigationModifiers();
		}
	}
}

FVector AGsCampNavModifier::GetFailSafePos()
{
	TArray<UActorComponent*> findComps = GetComponentsByTag(UNavModifierComponent::StaticClass(), TEXT("CampNavModifier"));
	if (findComps.IsValidIndex(0))
	{
		UNavModifierComponent* findComp = Cast<UNavModifierComponent>(findComps[0]);
		if (nullptr != findComp)
		{
			return findComp->FailsafeExtent;
		}
	}

	return FVector(0.f, 0.f, 0.f);
}

void AGsCampNavModifier::SetActiveModifier(bool InState)
{
	TArray<UActorComponent*> findComps = GetComponentsByTag(UNavModifierComponent::StaticClass(), TEXT("CampNavModifier"));
	if (findComps.IsValidIndex(0))
	{
		findComps[0]->SetActive(InState);
		findComps[0]->SetActiveFlag(InState);

		/*
		UNavModifierComponent* findComp = Cast<UNavModifierComponent>(findComps[0]);
		if (nullptr != findComp)
		{
			if (InState)
			{
				//findComp->Activate(true);
				findComp->SetActive(true,true);
			}
			else
			{
				//findComp->Deactivate();
				findComp->SetActive(false);
			}
		}
		*/
	}
}

void AGsCampNavModifier::EndPlay(const EEndPlayReason::Type in_type)
{

	Super::EndPlay(in_type);
}

// Called every frame
void AGsCampNavModifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


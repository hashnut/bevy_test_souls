// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritoryShapeOwnerInterface.h"
#include "Components/SplineComponent.h"

UGsTerritoryShapeOwnerInterface::UGsTerritoryShapeOwnerInterface(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UGsTerritoryShapeOwnerInterface::SetSplinePoints(class USplineComponent* inComponent, const TArray<FVector>& inPointArray)
{
	if (inComponent)
	{
		inComponent->ClearSplinePoints();
		for (const FVector& point : inPointArray)
		{
			inComponent->AddSplineWorldPoint(point);
		}		
	}		
}

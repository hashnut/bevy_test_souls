// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMovementDebugShapeComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"

#include "Runtime/Engine/Classes/Engine/World.h"


UGsMovementDebugShapeComponent::UGsMovementDebugShapeComponent()
{
#if (WITH_EDITOR)
	PrimaryComponentTick.bCanEverTick = true;
#else
	PrimaryComponentTick.bCanEverTick = false;
#endif
}


// Called every frame
void UGsMovementDebugShapeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	_epicLocation += _epicVelocity;

#if (WITH_EDITOR)
	if (_showDebugShape)
	{
		DrawDebugShape();
	}
#endif

}

#if WITH_EDITOR
// TestCode
	/*
	FVector center = GetComponentLocation();
	FVector halfH = FVector(0, 0, _helfheight);
	UKismetSystemLibrary::DrawDebugCylinder(GetWorld(), center - halfH, center + halfH, _radius, _segments, _locationColor, _lifeTime, _lineThickness);
	*/
void UGsMovementDebugShapeComponent::DrawDebugShape()
{	
	//현재위치를 그리고
	DrawDebugCylinder(_location, _locationColor);

	//목적지를 그린다.
	DrawDebugCylinder(_destination, _destinationColor);

	//서버좌표를 그린다.
	DrawDebugCylinder(_serverLocation, 	_serverLocationColor);

	//알고리즘 좌표를 그린다.
	DrawDebugCylinder(_epicLocation, _epicLocationColor);

	//타유저의 방향을 그린다.
	DrawDebugArrow(_netDirection, _directionColor);

	//서버 디버깅 정보를 찍는다.
	DrawDebugCylinder(_ServerDebugPos, _serverDebugColor);

	//디버그 메시지를 뿌린다.
	//DrawDebugString(_debugString, _directionColor, FVector(0, 0, 20.0f));
}


void UGsMovementDebugShapeComponent::DrawDebugCylinder(const FVector& inLocation, const FLinearColor& inColor)
{
	FVector halfH = FVector(0, 0, _helfheight);
	FVector floor = inLocation;
	UKismetSystemLibrary::DrawDebugCylinder(GetWorld(), floor, floor + 2 * halfH, _radius, _segments, inColor, _lifeTime, _lineThickness);
}

void UGsMovementDebugShapeComponent::DrawDebugArrow(const FVector& inDirection, const FLinearColor& inColor)
{
	FVector halfH = FVector(0, 0, _helfheight);
	FVector actorLoc = GetOwner()->GetActorLocation() + 2 * halfH;
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), actorLoc, (actorLoc + (inDirection * 150.0f)), 30.0f, inColor);
}

void UGsMovementDebugShapeComponent::DrawDebugString(const FString& inText, const FLinearColor& inColor, const FVector& inOffset)
{
	UKismetSystemLibrary::DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), _debugString, GetOwner(), inColor);
}

#endif


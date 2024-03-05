// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTargetRotateCamera.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

AGsTargetRotateCamera::AGsTargetRotateCamera()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGsTargetRotateCamera::BeginPlay()
{
	Super::BeginPlay();

	if (_target)
	{
		_initialRotateVector = GetActorLocation() - _target->GetActorLocation();
		_currentRotateVector = _initialRotateVector;
	}	
}

void AGsTargetRotateCamera::Tick(float inDelta)
{
	Super::Tick(inDelta);

	_currentRotateVector = _currentRotateVector.RotateAngleAxis(inDelta * _degree, FVector::UpVector);

	FVector location = _target->GetActorLocation() + _currentRotateVector;
	FRotator rotation = UKismetMathLibrary::FindLookAtRotation(location, _target->GetActorLocation());
	
	SetActorLocationAndRotation(location, rotation);
}

void AGsTargetRotateCamera::Active(bool inInit)
{	
	if (APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		controller->SetViewTarget(this);
	}

	if (inInit)
	{
		_currentRotateVector = _initialRotateVector;
	}
}

void AGsTargetRotateCamera::Deactive()
{	
	if (APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		controller->SetViewTarget(controller->GetPawn());
	}
}

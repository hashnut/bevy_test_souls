// Fill out your copyright notice in the Description page of Project Settings.


#include "GsRandomParticleSpawn.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Engine/World.h"
#include "CoreMinimal.h"

// Sets default values
AGsRandomParticleSpawn::AGsRandomParticleSpawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGsRandomParticleSpawn::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr != _ParticleSystem)
	{
		ResetTimer();
	}
}

void AGsRandomParticleSpawn::EndPlay(const EEndPlayReason::Type in_type)
{
	ClearTimer();
	Super::EndPlay(in_type);
}

void AGsRandomParticleSpawn::ResetTimer()
{
	ClearTimer();

	float fMin, fMax;
	if (_ShowTimeRandomOffset < 0.0f)
	{
		fMin = _ShowTimeDelay + _ShowTimeRandomOffset;
		fMax = _ShowTimeDelay - _ShowTimeRandomOffset;
	}
	else
	{
		fMin = _ShowTimeDelay - _ShowTimeRandomOffset;
		fMax = _ShowTimeDelay + _ShowTimeRandomOffset;
	}
	float fResult = FMath::RandRange(fMin, fMax);
	

	if (false == NextPossessTimer.IsValid())
	{
		GetWorld()->GetTimerManager().SetTimer(NextPossessTimer, FTimerDelegate::CreateLambda([this]()
		{
			SpawnParticle();
			ResetTimer();
		}),
		fResult, false);
	}
}


void AGsRandomParticleSpawn::ClearTimer()
{
	if (NextPossessTimer.IsValid())
	{
		if (nullptr != GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(NextPossessTimer);
		}
		NextPossessTimer.Invalidate();
	}
}

void AGsRandomParticleSpawn::CalcRandomPosition(FVector& outRandomRange)
{
	outRandomRange.X = FMath::RandRange(-_RandomRangePosition.X, _RandomRangePosition.X);
	outRandomRange.Y = FMath::RandRange(-_RandomRangePosition.Y, _RandomRangePosition.Y);
	outRandomRange.Z = FMath::RandRange(-_RandomRangePosition.Z, _RandomRangePosition.Z);
}

float AGsRandomParticleSpawn::CalcRandomScale()
{
	if (_RandomRangeScaleMin < _RandomRangeScaleMax)
	{
		return FMath::RandRange(_RandomRangeScaleMin, _RandomRangeScaleMax);
	}
	return FMath::RandRange(_RandomRangeScaleMax, _RandomRangeScaleMin);
}

void AGsRandomParticleSpawn::SpawnParticle()
{
	if (UWorld* world = GetWorld())
	{
		FVector rangeRandomLocation;
		CalcRandomPosition(rangeRandomLocation);

		UGameplayStatics::SpawnEmitterAtLocation(world,
			_ParticleSystem,
			GetActorLocation() + rangeRandomLocation,
			FRotator::ZeroRotator,
			(FVector)CalcRandomScale(),
			true,
			EPSCPoolMethod::AutoRelease);
	}
}

void AGsRandomParticleSpawn::BeginDestroy()
{
	Super::BeginDestroy();
}



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GsRandomParticleSpawn.generated.h"

// bak1210 
// VFX 지원용 액터 추가
// 액터 포지션 기준 랜덤 포지션 / 스케일 / 발동시간 출력을 지원한다.

UCLASS()
class T1PROJECT_API AGsRandomParticleSpawn final : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomParticle")
	FVector _RandomRangePosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomParticle")
	float _RandomRangeScaleMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomParticle")
	float _RandomRangeScaleMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomParticle")
	float _ShowTimeDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomParticle")
	float _ShowTimeRandomOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomParticle")
	UParticleSystem* _ParticleSystem;

private:
	FTimerHandle NextPossessTimer;

public:	
	// Sets default values for this actor's properties
	AGsRandomParticleSpawn();
	virtual void BeginDestroy() override;

protected:
	void	ResetTimer();
	void	ClearTimer();

	void	SpawnParticle();
	void	CalcRandomPosition(FVector& outRandomRange);
	float	CalcRandomScale();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;
};

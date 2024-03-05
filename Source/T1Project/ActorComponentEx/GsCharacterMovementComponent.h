// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GsCharacterMovementComponent.generated.h"

class AGsCharacterBase;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	AGsCharacterBase*	_owner = nullptr;
	int					_prevCount = -1;
	bool				_bMoved = false;	
	
public:
	virtual void BeginPlay() override;

public:
	void ResetCheckOverlapActorCount();
	virtual bool IsWalkable(const FHitResult& Hit) const override;

public:
	virtual void CheckOverlapActor();

	bool IsMoved() const;

public:
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

//protected:
//	virtual void ComputeFloorDist(const FVector& CapsuleLocation, float LineDistance, float SweepDistance, FFindFloorResult& OutFloorResult, float SweepRadius, const FHitResult* DownwardSweepResult) const override;

// for cheat gs.CurrentFloor

public:
	void TickComponent(float DeltaTime, enum ELevelTick TickType, struct FActorComponentTickFunction* ThisTickFunction) final;
};

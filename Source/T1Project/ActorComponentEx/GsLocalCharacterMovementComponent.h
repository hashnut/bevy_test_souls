// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsCharacterMovementComponent.h"
#include "GsLocalCharacterMovementComponent.generated.h"

/**
 * 로컬 플레이어가 블럭킹으로 이동하지 못할때 네트워크 전송을 위해 작성됨
 */

UCLASS()
class T1PROJECT_API UGsLocalCharacterMovementComponent : public UGsCharacterMovementComponent
{
	GENERATED_BODY()

private:
	bool _bBlockingHit = false;

public:
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice = 0.f, const FVector& MoveDelta = FVector::ZeroVector) override;
	
public:
	bool IsHitBlocked() { return _bBlockingHit && (!_bMoved); }
};


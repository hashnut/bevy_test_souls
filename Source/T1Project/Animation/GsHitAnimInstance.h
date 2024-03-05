// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GsHitAnimInstance.generated.h"

// 블랜드 타입
UENUM(BlueprintType)
enum class EGsAnimStateBlendType : uint8
{
	BodyFull = 0,
	BodyUpper,
	BodyHead,
	BodyNone,
};

// 출력방향
UENUM(BlueprintType)
enum class EGsAnimStateHitDirection : uint8
{
	DIR_FRONT = 0,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_MAX,
};

/**
 *  
 */
UCLASS()
class T1PROJECT_API UGsHitAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	EGsAnimStateBlendType BodyBlendType;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	float BlendWeightsHitMotion;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "GsAnimInstanceState")
	EGsAnimStateHitDirection HitMotionDirection;

};

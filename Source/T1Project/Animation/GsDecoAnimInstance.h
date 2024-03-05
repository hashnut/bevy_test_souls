// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GsDecoAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsDecoAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/** Anim Sequence */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* AnimSequence;

	/** Rotate Spine1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotate_Spine1;

	/** Rotate Neck */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotate_Neck;

	/** Rotate Head */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotate_Head;
	
};

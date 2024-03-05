// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MotionBlur.generated.h"

class UCurveFloat;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UAnimNotify_MotionBlur : public UAnimNotify
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool IgnorePreBlur = false;

	UPROPERTY(EditAnywhere)
	UCurveFloat* _motionBlurTime;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

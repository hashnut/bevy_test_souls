// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#if BSAM_TEMP
#include "MatineeCameraShake.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#else
#include "Runtime/Engine/Classes/Camera/CameraShake.h"
#endif
#include "AnimNotify_PlayCameraShake.generated.h"

/**
 * 외부 카메라 쉐이크Bp를 처리하기 노티파이
   사용 타겟층은 아트팀입니다.
 */
#if BSAM_TEMP
#else
class UMatineeCameraShake;
#endif


UCLASS(hidecategories = Object, meta = (DisplayName = "Play CameraShake"))
class T1PROJECT_API UAnimNotify_PlayCameraShake : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (ExposeOnSpawn = true))
	TSubclassOf<UMatineeCameraShake> CameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (ExposeOnSpawn = true))
	FVector TargetOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (ExposeOnSpawn = true))
	float InnerRadius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (ExposeOnSpawn = true))
	float OuterRadius = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (ExposeOnSpawn = true))
	float Falloff = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CameraShake", meta = (ExposeOnSpawn = true))
	bool bOrientShakeTowardsEpicenter = false;
	
public:
	UAnimNotify_PlayCameraShake();

	// Begin UAnimNotify interface
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlayMeshDissolveShader.generated.h"

/**
 * 외부 카메라 쉐이크Bp를 처리하기 노티파이
   사용 타겟층은 아트팀입니다.
 */
class UMatineeCameraShake;

UCLASS(hidecategories = Object, meta = (DisplayName = "Play Dissolve"))
class T1PROJECT_API UAnimNotify_PlayMeshDissolveShader : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "PlayMeshDissolveShader")
	bool CanReverse = false;

	// Curve정보가 있으면 이정보로 효과처리
	UPROPERTY(EditAnywhere, Category = "PlayMeshDissolveShader")
	class UCurveFloat* _curveData = nullptr;
		
public:
	UAnimNotify_PlayMeshDissolveShader();

	// Begin UAnimNotify interface
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};

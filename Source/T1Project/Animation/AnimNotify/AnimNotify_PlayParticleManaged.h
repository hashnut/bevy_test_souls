// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_PlayParticleParam.h"
#include "AnimNotify_PlayParticleManaged.generated.h"

UENUM(BlueprintType)
enum class EPlayParticleManagedType : uint8
{
	ImmediateDestroy,			// 애니메이션 상태 변경시 즉시 삭제
	TargetHitSync_Immediate,	// 피격된 대상이 있을때 즉시 삭제
	TargetHitSync_OneTarget,    // 타겟이 1만 설정되어 있을때
};

/**
 * 
 */
UCLASS(hidecategories = Object, meta = (DisplayName = "Play ParticleManaged"))
class T1PROJECT_API UAnimNotify_PlayParticleManaged : public UAnimNotify_PlayParticleParam
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify")
	EPlayParticleManagedType _Type;
	
protected:
	// Spawns the ParticleSystemComponent. Called from Notify.
	virtual UParticleSystemComponent* SpawnParticleSystem(USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation) override;
	
};

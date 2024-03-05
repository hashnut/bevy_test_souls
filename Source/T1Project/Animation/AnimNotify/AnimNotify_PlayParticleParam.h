// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlayParticleEffect.h"
#include "Particles/ParticleSystemComponent.h"
#include "AnimNotify_PlayParticleParam.generated.h"
/**
 * 파티클 시스템 인스턴스 파라미터 정보를 제공함으로써
 * 1개의 BP를 만들어두고 재활용성을 높이기 위함
 */
UCLASS(hidecategories = Object, meta = (DisplayName = "Play Particle InstanceParameters"))
class T1PROJECT_API UAnimNotify_PlayParticleParam : public UAnimNotify_PlayParticleEffect
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Particles)
	TArray<FParticleSysParam> InstanceParameters;
	
protected:
	// Spawns the ParticleSystemComponent. Called from Notify.
	virtual UParticleSystemComponent* SpawnParticleSystem(USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation) override;
	
};

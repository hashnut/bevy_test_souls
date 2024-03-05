// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlayParticleEffect.h"
#include "AnimNotify_PlayParticleVehicle.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UAnimNotify_PlayParticleVehicle : public UAnimNotify_PlayParticleEffect
{
	GENERATED_BODY()

protected:
	// Spawns the ParticleSystemComponent. Called from Notify.
	virtual UParticleSystemComponent* SpawnParticleSystem(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation) override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_PlayParticleParam.h"
#include "AnimNotify_FootPrintEffect.generated.h"

/**
 * 발자국 이펙트 처리를 위해 추가한 노티파이
 * 지형타입에 따라 어떤 이펙트를 출력할지 결정하는 구조
 * 1차 맵타입, 2차 볼륨에 따라 분기처리함
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Play Footprint Effect"))
class T1PROJECT_API UAnimNotify_FootPrintEffect : public UAnimNotify_PlayParticleParam
{
	GENERATED_BODY()
private:
	// Cached version of the Rotation Offset already in Quat form
	FQuat _RotationOffsetQuat;

	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:
	// Spawns the ParticleSystemComponent. Called from Notify.
	virtual UParticleSystemComponent* SpawnParticleSystem(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};

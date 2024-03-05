// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_PlayParticleParam.h"
#include "AnimNotify_FairySkillEffect.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UAnimNotify_FairySkillEffect : public UAnimNotify_PlayParticleParam
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};

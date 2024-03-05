// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsAnimNotify_PlaySound.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "GsPlaySound"))
class T1PROJECT_API UGsAnimNotify_PlaySound : public UAnimNotify_PlaySound
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	bool bSpeedScale = true;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;	

private:
	bool IsCharacter(class USkeletalMeshComponent* MeshComp);
	EGsFootSoundType GetFootSoundType(class USkeletalMeshComponent* MeshComp);
};

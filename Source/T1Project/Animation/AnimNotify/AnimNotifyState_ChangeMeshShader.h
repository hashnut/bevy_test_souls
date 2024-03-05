// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ChangeMeshShader.generated.h"
/**
 *
 */
class UCurveFloat;
class USkeletalMeshComponent;
class UAnimSequenceBase;

UCLASS(Blueprintable, meta = (DisplayName = "Change dissolve"))
class T1PROJECT_API UAnimNotifyState_ChangeMeshShader : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(EditAnywhere)
		UCurveFloat* DissolveCurve;
	UPROPERTY(EditAnywhere)
		FName ParameterName;

private:
	UPROPERTY(Transient)
		float CurrentTime;

public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation) override;
};
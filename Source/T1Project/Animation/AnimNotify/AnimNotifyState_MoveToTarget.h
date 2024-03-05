// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_MoveToTarget.generated.h"


UCLASS()
class T1PROJECT_API UAnimNotifyState_MoveToTarget : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

	FVector _destPos;
	FVector _velocity;
	float _totalTime = 0.f;
	float _elapsedTime = 0.f;
	class UGsGameObjectBase* _gameObject = nullptr;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_FrameDeltaTime) override;
	// virtual void NotifyEnd(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation) override;

public:
	float GetTotalTime() const;
};
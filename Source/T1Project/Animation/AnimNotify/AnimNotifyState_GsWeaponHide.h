// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_GsWeaponHide.generated.h"

/**
 * 인게임에서 사용하는 무기 메시(AGsWeaponActor 클래스) 숨김 처리
 */
UCLASS()
class T1PROJECT_API UAnimNotifyState_GsWeaponHide : public UAnimNotifyState
{
	GENERATED_BODY()

	TArray<TWeakObjectPtr<class AGsWeaponActor>> _listCacheActor;

public:
	//UObject
	virtual void BeginDestroy() override;
	virtual void NotifyBegin(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation, float in_TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* in_MeshComp, UAnimSequenceBase* in_Animation) override;

protected:
	void Restore();
};

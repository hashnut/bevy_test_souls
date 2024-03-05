// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Camp.generated.h"

class UAnimSequenceBase;
class USkeletalMeshComponent;
/**
 * 서버에서 캠프 단계별 완료 시 별도 notify 패킷 주긴하는데
   클라쪽에서 별도 에니메이션 종료 시점 알아야되는 상황 생길수도 있어서
   우선은 class 생성
 */
UCLASS()
class T1PROJECT_API UAnimNotify_Camp : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_Camp();

	// Begin UObject interface
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// End UObject interface

	// Begin UAnimNotify interface
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
#if WITH_EDITOR
	virtual void ValidateAssociatedAssets() override;
#endif
	// End UAnimNotify interface

	
};

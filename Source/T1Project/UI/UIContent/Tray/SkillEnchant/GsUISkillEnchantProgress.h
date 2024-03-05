// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUISkillEnchantProgress.generated.h"

/**
 * 스킬 강화 연출(서버와 패킷 주고받을 시간 벌기)
 */
UCLASS()
class T1PROJECT_API UGsUISkillEnchantProgress : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FGsSkillEnchantProgressTimeFinished);

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GsUI")
	float _progressTime = 0.8f;

protected:
	float _remainTime = 0.f;

public:
	FGsSkillEnchantProgressTimeFinished OnProgressTimeFinished;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void StartProgress();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void StartProgressAnimation();
};

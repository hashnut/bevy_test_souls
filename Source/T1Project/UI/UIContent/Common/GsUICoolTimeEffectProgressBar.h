// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/GsUICoolTimeEffect.h"
#include "GsUICoolTimeEffectProgressBar.generated.h"

class UProgressBar;
class UPanelWidget;

/**
 * 쿨타임 흐르는 연출 UI
 */
UCLASS()
class T1PROJECT_API UGsUICoolTimeEffectProgressBar : public UGsUICoolTimeEffect
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _barHeight = 110.f;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBar;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelThumb;

protected:
	FVector2D _thumbPos;

protected:
	virtual void NativeConstruct() override;
	
public:
	virtual void SetProgress(float InRate) override;
};

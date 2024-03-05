// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Customize/GsUICustomizeBase.h"
#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
#include "GsUICustomizeBody.generated.h"

class UGsUIStepSlider;

/**
 * UGsUICustomizingBody
 * : Body 커스터마이징 메뉴들
 */
UCLASS()
class T1PROJECT_API UGsUICustomizeBody : public UGsUICustomizeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIStepSlider* _sliderHeight; // 키

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIStepSlider* _sliderHead; // 머리크기

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIStepSlider* _sliderChest; // 가슴크기

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIStepSlider* _sliderWaist; // 허리크기

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIStepSlider* _sliderPelvis; // 골반 크기

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIStepSlider* _sliderLegs; // 다리 두께

protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;

public:
	virtual void Show() override;

protected:
	UFUNCTION()
	void OnSliderHeight(float InValue); // 키

	UFUNCTION()
	void OnSliderHead(float InValue); // 머리

	UFUNCTION()
	void OnSliderChest(float InValue); // 가슴

	UFUNCTION()
	void OnSliderWaist(float InValue); // 허리

	UFUNCTION()
	void OnSliderPelvis(float InValue); // 골반

	UFUNCTION()
	void OnSliderLegs(float InValue); // 다리
};

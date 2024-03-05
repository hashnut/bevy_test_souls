// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIStepSlider.generated.h"

class USlider;
class UGsButton;

/**
 * 증감 버튼이 달린 슬라이더
 */
UCLASS()
class T1PROJECT_API UGsUIStepSlider : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnSliderValueChanged, float)

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIStepSlider", meta = (BindWidget))
	USlider* _slider;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIStepSlider", meta = (BindWidget))
	UGsButton* _btnIncrease;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIStepSlider", meta = (BindWidget))
	UGsButton* _btnDecrease;

public:
	FOnSliderValueChanged OnValueChanged;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetValue(float InValue);
	float GetValue() const;

protected:
	UFUNCTION()
	void OnClickIncrease();
	UFUNCTION()
	void OnClickDecrease();

	UFUNCTION()
	void OnSliderValueChanged(float InValue);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUITwoSideSlider.generated.h"

class UBorder;
class UImage;

/**
 * min/max를 정의할 수 있는 양방향 슬라이더
 */
UCLASS()
class T1PROJECT_API UGsUITwoSideSlider : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_TwoParams(FOnEndSlideValue, float /*currMinStepValue*/, float /*currMaxStapValue*/);	
	DECLARE_DELEGATE_TwoParams(FOnEndSlideRatio, float /*currMinRatio*/, float /*currMaxRatio*/);

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsUITwoSideSlider")
	float _minStepValue = 0.f;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsUITwoSideSlider")
	float _maxStepValue = 1.f;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "GsUITwoSideSlider")
	float _stepSize = 0.1f;

	// 값 표기 방법
	UPROPERTY(BlueprintReadOnly, Category = "GsUITwoSideSlider", meta = (BindWidget))
	UBorder* _brThumbMin;
	UPROPERTY(BlueprintReadOnly, Category = "GsUITwoSideSlider", meta = (BindWidget))
	UBorder* _brThumbMax;
	UPROPERTY(BlueprintReadOnly, Category = "GsUITwoSideSlider", meta = (BindWidget))
	UImage* _imgBg;
	UPROPERTY(BlueprintReadOnly, Category = "GsUITwoSideSlider", meta = (BindWidget))
	UImage* _imgBar;

protected:
	bool _bIsPressedTumbMin = false;
	bool _bIsPressedTumbMax = false;	
	bool _bReserveUpdate = false;

	// 주의: 0 ~ 1 사이의 값
	float _currMinRatio = 0.f;
	float _currMaxRatio = 1.f;

protected:
	FOnEndSlideValue _onEndSlideValue;
	FOnEndSlideRatio _onEndSlideRatio;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
	UFUNCTION()
	FEventReply OnPressThumbMin(FGeometry InGeometry, const FPointerEvent& InMouseEvent);
	UFUNCTION()
	FEventReply OnPressThumbMax(FGeometry InGeometry, const FPointerEvent& InMouseEvent);
	
public:
	void SetMinStepValue(float InValue) { _minStepValue = InValue; }
	void SetMaxStepValue(float InValue) { _maxStepValue = InValue; }
	void SetStepSize(float InValue) { _stepSize = InValue; }
	void SetCurrentValue(float InCurrMinValue, float InCurrMaxValue, bool bInExecute);	
	void SetCurrentRatio(float InCurrMinRatio, float InCurrMaxRatio, bool bInExecute);

public:
	FOnEndSlideValue& OnEndSlideValue() { return _onEndSlideValue; }
	FOnEndSlideRatio& OnEndSlideRatio() { return _onEndSlideRatio; }

protected:
	void UpdateBar(float InMinRatio, float InMaxRatio);
	void UpdateThumb();
	void ExecuteEvent();

protected:
	void SetMinRatio(float InRatio);
	void SetMaxRatio(float InRatio);

	float GetNearestStepRatio(float InRatio);
	float GetMaxValue() const;
	float GetMaxSize() const;
};

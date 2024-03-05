// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "Misc/DateTime.h"
#include "Button.h"
#include "Blueprint/UserWidget.h"
#include "GsAccelationButton.generated.h"

/**
 * 누르고 있으면 가속 해주는 버튼, Click과 Pressed의 차이를 가져와서 커브에서 가속 값을 얻어온다. 
 */
 DECLARE_MULTICAST_DELEGATE_OneParam(FLongTouchDelegate, float)
 DECLARE_MULTICAST_DELEGATE(FShortTouchDelegate)

UCLASS()
class T1PROJECT_API UGsAccelationButton : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* _button;	
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
		UCurveFloat* _curve;	

public:
	UPROPERTY(BlueprintReadOnly)
		float _maxTime = 2.0f;
	UPROPERTY(BlueprintReadOnly)
		float _tickTime = 0.25f;

public:
	FShortTouchDelegate _shortTouchEvent;
	FLongTouchDelegate _longTouchEvent;

private:
	float _currentTime;
	float _accumTime;
	float _accelation;
	FDateTime _startTime;
	bool _isPressed;

public:		
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;	
	virtual void BeginDestroy() override;	

public:	
	UFUNCTION()
	void Clicked();
	UFUNCTION()
	void Pressed();
	UFUNCTION()
	void Released();

public:
	float GetCurrentAccelation() {
		return _accelation;
	}
	virtual void SetIsEnabled(bool inEnable) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ButtonEnableEvent();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ButtonDisableEvent();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITrayFadeEffect.generated.h"

DECLARE_DELEGATE(FFinishDelegate);
/**
 *  Tray FadeOut
 */
UCLASS()
class T1PROJECT_API UGsUITrayFadeEffect : public UGsUITray
{
	GENERATED_BODY()

public:
	enum EGsFadeAnimationType
	{
		FadeIn = 0,
		FadeOut,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imgBG;

	TFunction<void()> _animationFinishedCallback;
	float _duration = 0.f;
	float _elapsedTime = 0.f;
	EGsFadeAnimationType _currFadeType;

	// Delegate 
	FFinishDelegate _delegateAction;

	bool _tickStart = false;

	// [B1] | ejrrb10 | 자체 타이머를 위한 변수 추가
	bool _isWaitingToStart = false;
	float _timeToWait = 0.f;

protected:
	virtual void NativeDestruct() override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

	virtual bool IsEnableManagerTick() const override { return true; }
	virtual void OnManagerTick(float InDeltaTime) override;

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_SYSTEM_TRAY; }

public:
	void SetFadeAnimation(EGsFadeAnimationType InFadeType, const FLinearColor& InColor = FLinearColor::Black, float InDuration = 1.f,
		bool InStartNow = true);
	void SetFadeAnimationWithDelay(EGsFadeAnimationType InFadeType, const FLinearColor& InColor = FLinearColor::Black, float InDuration = 1.f, float InDelayTime = 0.f);
	void SetAnimationFinishedCallback(TFunction<void()> InFunc);	
	void SetAnimationFinishedCallback(FFinishDelegate const& InDelegate);

	void StartAnimation()
	{
		_tickStart = true;
	}

protected:

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnEndFadeAnimation();

	void ProcessAnimation(float InRate);
};

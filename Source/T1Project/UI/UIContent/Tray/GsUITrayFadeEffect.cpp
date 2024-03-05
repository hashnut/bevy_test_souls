// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayFadeEffect.h"
#include "Components/Image.h"

#include "Management/GsMessageHolder.h"

#include "Message/GsMessageUI.h"

void UGsUITrayFadeEffect::NativeDestruct()
{
	_animationFinishedCallback = nullptr;
	_duration = 0.f;
	_elapsedTime = 0.f;
	_delegateAction.Unbind();
	_tickStart = false;

	Super::NativeDestruct();
}

// 터치 이벤트 무조건 막기(카메라 회전 막기)
FReply UGsUITrayFadeEffect::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITrayFadeEffect::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITrayFadeEffect::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

void UGsUITrayFadeEffect::OnManagerTick(float InDeltaTime)
{
	Super::OnManagerTick(InDeltaTime);

	if (true == _isWaitingToStart)
	{
		_timeToWait -= InDeltaTime;
		if (_timeToWait < 0)
		{
			_isWaitingToStart = false;
			_timeToWait = 0;
			_tickStart = true;
		}
	}

	if (true == _tickStart && 0 < _duration)
	{
		_elapsedTime += InDeltaTime;

		if (_duration <= _elapsedTime)
		{
			_duration = 0.f;
			_elapsedTime = 0.f;

			ProcessAnimation(1);

			OnEndFadeAnimation();

			_tickStart = false;
		}
		else
		{
			ProcessAnimation(_elapsedTime / _duration);
		}
	}
}

void UGsUITrayFadeEffect::SetFadeAnimation(EGsFadeAnimationType InFadeType, const FLinearColor& InColor, float InDuration, bool InStartNow)
{
	_currFadeType = InFadeType;
	_imgBG->SetColorAndOpacity(InColor);		
	_duration = InDuration;

	_elapsedTime = 0.f;

	ProcessAnimation(0.f);

	_tickStart = InStartNow;
}

void UGsUITrayFadeEffect::SetFadeAnimationWithDelay(EGsFadeAnimationType InFadeType, const FLinearColor& InColor, float InDuration, float InDelayTime)
{
	_currFadeType = InFadeType;
	_imgBG->SetColorAndOpacity(InColor);
	_duration = InDuration;

	_elapsedTime = 0.f;

	ProcessAnimation(0.f);

	_tickStart = false;

	_isWaitingToStart = true;
	_timeToWait = InDelayTime;
}

void UGsUITrayFadeEffect::SetAnimationFinishedCallback(TFunction<void()> InFunc)
{
	_animationFinishedCallback = InFunc;
}

void UGsUITrayFadeEffect::SetAnimationFinishedCallback(FFinishDelegate const& InDelegate)
{
	_delegateAction = InDelegate;
}

void UGsUITrayFadeEffect::OnEndFadeAnimation()
{
	if (_animationFinishedCallback)
	{
		_animationFinishedCallback();
	}

	if (_delegateAction.IsBound())
	{
		_delegateAction.ExecuteIfBound();
	}

	GMessage()->GetUI().SendMessage(MessageUI::TRAY_FADE_EFFECT_ANI_FINISHED, nullptr);
}

void UGsUITrayFadeEffect::ProcessAnimation(float InRate)
{
	switch (_currFadeType)
	{
	case EGsFadeAnimationType::FadeIn:
		SetRenderOpacity(FMath::Abs(1 - InRate));
		break;
	case EGsFadeAnimationType::FadeOut:
		SetRenderOpacity(FMath::Abs(InRate));
		break;
	}
}

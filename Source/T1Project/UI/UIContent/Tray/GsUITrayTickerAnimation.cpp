// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerAnimation.h"

void UGsUITrayTickerAnimation::SetData(TFunction<void()> InCallback)
{
	_exitAnimationCallback = InCallback;
}

void UGsUITrayTickerAnimation::OnExitAnimation()
{
	if (nullptr != _exitAnimationCallback)
	{
		_exitAnimationCallback();
	}

	Close();
}

void UGsUITrayTickerAnimation::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsAnimationPause() == false)
	{
		OnCommandPlayAnimation();
	}
}

void UGsUITrayTickerAnimation::RestartAllAnimations()
{
	if (IsAnimationPause() == true)
	{
		Super::RestartAllAnimations();

		OnCommandPlayAnimation();
	}
}

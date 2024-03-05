// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayLongPressBar.h"
#include "Components/ProgressBar.h"
#include "T1Project.h"
#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"


void UGsUITrayLongPressBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_progressBar->SetPercent(0.0f);
	_time = 0.f;
}

void UGsUITrayLongPressBar::NativeConstruct()
{
	Super::NativeConstruct();
	_time = 0.f;
	_isFullGauge = false;

	// CachedGeometry가 계산되어 중앙으로 위치조정이 끝날때까지 감춰둠	
	if (_bReserveShow)
	{
		SetRenderOpacity(0.0f);
	}
}

void UGsUITrayLongPressBar::NativeDestruct()
{
	if (_onFinishedLongPress.IsBound())
	{
		_onFinishedLongPress.Clear();
	}

	_progressBar->SetPercent(0.0f);
	_time = 0.f;

	_bReserveShow = false;

	Super::NativeDestruct();
}

void UGsUITrayLongPressBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_isFullGauge)
		return;

	if (_bReserveShow)
	{
		if (false == MyGeometry.GetLocalSize().IsZero())
		{
			_bReserveShow = false;

			FVector2D targetPos = _absTargetPos;
			targetPos.X -= MyGeometry.GetAbsoluteSize().X * 0.5f;

			FVector2D pixel;
			FVector2D viewport;
			USlateBlueprintLibrary::AbsoluteToViewport(this, targetPos, pixel, viewport);

			SetPositionInViewport(pixel);
			SetRenderOpacity(1.0f);
		}
	}

	if (1.0f <= _progressBar->Percent)
	{
		OnFinished();
		return;
	}

	// 1초당 기준으로 설정
	// 0초 설정이면 바로 뜨게함
	if (0.f >= _longPressTotalDurationTime)
	{
		_progressBar->SetPercent(1.0f);
		OnFinished();
	}
	else
	{
		// 1초를 맥시멈으로 두고, 1초를 넘게 입력하면 1초로 간주한다
		_longPressTotalDurationTime = 1.f < _longPressTotalDurationTime ? 1.f : _longPressTotalDurationTime;
		_time += InDeltaTime;
		float percent = _time / _longPressTotalDurationTime;
		_progressBar->SetPercent(percent);
	}
	//GSLOG(Warning, TEXT("[UGsUITrayLongPressBar::NativeTick] - InDeltaTime: %f"), InDeltaTime);
}

void UGsUITrayLongPressBar::OnFinished()
{
	_isFullGauge = true;

	_onFinishedLongPress.Broadcast(_isFullGauge);

	CloseUI();
}

void UGsUITrayLongPressBar::CloseUI()
{
	Close();
}

void UGsUITrayLongPressBar::SetTargetPosition(const FVector2D& InAbsPos)
{
	_absTargetPos = InAbsPos;
	_bReserveShow = true;

	SetPositionInViewport(FVector2D::ZeroVector);
}

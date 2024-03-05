// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITwoSideSlider.h"
#include "Components/Border.h"
#include "Components/Image.h"


void UGsUITwoSideSlider::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_brThumbMin->OnMouseButtonDownEvent.BindDynamic(this, &UGsUITwoSideSlider::OnPressThumbMin);
	_brThumbMax->OnMouseButtonDownEvent.BindDynamic(this, &UGsUITwoSideSlider::OnPressThumbMax);

	_imgBar->SetRenderTransformPivot(FVector2D(0.f, 0.f));
}

void UGsUITwoSideSlider::NativeConstruct()
{
	Super::NativeConstruct();

	// 틱이 한번 돌아야 CachedGeometry 정보가 채워지므로 예약을 걸어둠
	if (0 == GetMaxSize())
	{
		_bReserveUpdate = true;
	}
}

void UGsUITwoSideSlider::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 틱이 한번 돌아야 Geometry 정보가 채워진다
	if (_bReserveUpdate)
	{
		if (0 < GetMaxSize())
		{
			UpdateBar(_currMinRatio, _currMaxRatio);
			UpdateThumb();

			_bReserveUpdate = false;
		}
	}
}

FEventReply UGsUITwoSideSlider::OnPressThumbMin(FGeometry InGeometry, const FPointerEvent& InMouseEvent)
{
	_bIsPressedTumbMin = true;

	return FEventReply(false);
}

FEventReply UGsUITwoSideSlider::OnPressThumbMax(FGeometry InGeometry, const FPointerEvent& InMouseEvent)
{
	_bIsPressedTumbMax = true;

	return FEventReply(false);
}

FReply UGsUITwoSideSlider::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	if (_bIsPressedTumbMin || _bIsPressedTumbMax)
	{
		return FReply::Handled().CaptureMouse(TakeWidget());
	}

	return FReply::Handled();
}

 FReply UGsUITwoSideSlider::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	 Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	if (_bIsPressedTumbMin)
	{
		FVector2D posLocal = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
		posLocal.X = FMath::Clamp(posLocal.X, 0.f, GetMaxSize());
		posLocal.Y = 0.f;
		_brThumbMin->SetRenderTranslation(posLocal);

		float minRatio = posLocal.X / GetMaxSize();
		if (_currMaxRatio < minRatio)
		{
			UpdateBar(_currMaxRatio, minRatio);
		}
		else
		{
			UpdateBar(minRatio, _currMaxRatio);
		}

		return FReply::Handled();
	}

	if (_bIsPressedTumbMax)
	{
		FVector2D posLocal = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
		posLocal.X = FMath::Clamp(posLocal.X, 0.f, GetMaxSize());
		posLocal.Y = 0.f;
		_brThumbMax->SetRenderTranslation(posLocal);

		float maxRatio = posLocal.X / GetMaxSize();
		if (_currMinRatio > maxRatio)
		{
			UpdateBar(maxRatio, _currMinRatio);
		}
		else
		{
			UpdateBar(_currMinRatio, maxRatio);
		}

		return FReply::Handled();
	}

	return FReply::Handled();
}

 FReply UGsUITwoSideSlider::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
 {
	 Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	 if (_bIsPressedTumbMin)
	 {
		 _bIsPressedTumbMin = false;

		 FVector2D posLocal = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
		 float minValue = GetNearestStepRatio(posLocal.X / GetMaxSize());
		 SetMinRatio(minValue);

		 UpdateBar(_currMinRatio, _currMaxRatio);
		 UpdateThumb();

		 return FReply::Handled().ReleaseMouseCapture();
	 }

	 if (_bIsPressedTumbMax)
	 {
		 _bIsPressedTumbMax = false;

		 FVector2D posLocal = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
		 float maxValue = GetNearestStepRatio(posLocal.X / GetMaxSize());
		 SetMaxRatio(maxValue);

		 UpdateBar(_currMinRatio, _currMaxRatio);
		 UpdateThumb();

		 return FReply::Handled().ReleaseMouseCapture();
	 }

	 return FReply::Unhandled();
 }

 void UGsUITwoSideSlider::SetCurrentValue(float InCurrMinValue, float InCurrMaxValue, bool bInExecute)
 {
	 float maxValue = GetMaxValue();
	 if (0 < maxValue)
	 {
		 SetCurrentRatio(InCurrMinValue / maxValue, InCurrMaxValue / maxValue, bInExecute);
	 }
	 else
	 {
		 SetCurrentRatio(0.f, 0.f, bInExecute);
	 }
 }

 void UGsUITwoSideSlider::SetCurrentRatio(float InCurrMinRatio, float InCurrMaxRatio, bool bInExecute)
 {
	 _currMinRatio = GetNearestStepRatio(InCurrMinRatio);
	 _currMaxRatio = GetNearestStepRatio(InCurrMaxRatio);

	 if (0 < GetMaxSize())
	 {
		 UpdateBar(_currMinRatio, _currMaxRatio);
		 UpdateThumb();
	 }

	 if (bInExecute)
	 {
		 ExecuteEvent();
	 }
 }

 void UGsUITwoSideSlider::UpdateBar(float InMinRatio, float InMaxRatio)
 {
	 FVector2D localPos(InMinRatio * GetMaxSize(), 0.f);
	 _imgBar->SetRenderTranslation(localPos);

	 float diffRatio = FMath::Abs(InMaxRatio - InMinRatio);
	 FVector2D localScale(diffRatio, 1.f);
	 _imgBar->SetRenderScale(localScale);
 }

 void UGsUITwoSideSlider::UpdateThumb()
 {
	 FVector2D minPos(_currMinRatio * GetMaxSize(), 0.f);
	 FVector2D maxPos(_currMaxRatio * GetMaxSize(), 0.f);

	 _brThumbMin->SetRenderTranslation(minPos);
	 _brThumbMax->SetRenderTranslation(maxPos);
 }

 void UGsUITwoSideSlider::ExecuteEvent()
 {
	 float maxValue = GetMaxValue();
	 _onEndSlideValue.ExecuteIfBound(_currMinRatio * maxValue + _minStepValue, _currMaxRatio * maxValue + _minStepValue);
	 _onEndSlideRatio.ExecuteIfBound(_currMinRatio, _currMaxRatio);
 }

 void UGsUITwoSideSlider::SetMinRatio(float InRatio)
 {
	 float targetRatio = FMath::Clamp(InRatio, 0.f, 1.f);

	 // max값을 넘어갔음
	 if (_currMaxRatio < targetRatio)
	 {
		 _currMinRatio = _currMaxRatio;
		 _currMaxRatio = targetRatio;
	 }
	 else
	 {
		 _currMinRatio = targetRatio;
	 }

	 ExecuteEvent();
 }

 void UGsUITwoSideSlider::SetMaxRatio(float InRatio)
 {
	 float targetRatio = FMath::Clamp(InRatio, 0.f, 1.f);

	 // min값을 넘어갔음
	 if (_currMinRatio > targetRatio)
	 {
		 _currMaxRatio = _currMinRatio;
		 _currMinRatio = targetRatio;
	 }
	 else
	 {
		 _currMaxRatio = targetRatio;
	 }

	 ExecuteEvent();
 }


 float UGsUITwoSideSlider::GetNearestStepRatio(float InRatio)
 {
	 float maxValue = GetMaxValue();
	 if (0 >= maxValue)
	 {
		 return 0.f;
	 }

	 float unitRatio = _stepSize / maxValue;
	 int32 index = FMath::FloorToInt(InRatio / unitRatio);
	
	 // 반올림
	 float remainder = InRatio - (unitRatio * index);	 
	 if ((unitRatio * 0.5f) < remainder)
	 {
		 ++index;
	 }

	 float targetRatio = index * unitRatio;

	 // 값 제한
	 targetRatio = FMath::Clamp(targetRatio, 0.f, 1.f);

	 return targetRatio;
 }

 float UGsUITwoSideSlider::GetMaxValue() const
 {
	 float diff = _maxStepValue - _minStepValue;
	 if (0 >= diff)
	 {
		 return 0.f;
	 }

	 return diff;
 }

float UGsUITwoSideSlider::GetMaxSize() const
{
	FVector2D localSize = _imgBg->GetCachedGeometry().GetLocalSize();
	return localSize.X;
}



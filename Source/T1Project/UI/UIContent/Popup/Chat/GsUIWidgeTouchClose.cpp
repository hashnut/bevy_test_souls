// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWidgeTouchClose.h"

FReply UGsUIWidgeTouchClose::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	OnTouchStart.ExecuteIfBound();

	return FReply::Unhandled().ReleaseMouseCapture();
}
FReply UGsUIWidgeTouchClose::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

FReply UGsUIWidgeTouchClose::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled().ReleaseMouseCapture();
}

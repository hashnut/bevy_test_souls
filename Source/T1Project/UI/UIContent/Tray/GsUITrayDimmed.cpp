// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Tray/GsUITrayDimmed.h"
#include "UMG/Public/Components/Image.h"

void UGsUITrayDimmed::NativeDestruct()
{
	Super::NativeDestruct();
}

// 터치 이벤트 무조건 막기(카메라 회전 막기)
FReply UGsUITrayDimmed::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITrayDimmed::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITrayDimmed::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

void UGsUITrayDimmed::SetDimmed(bool isDimmedOn, const FLinearColor& InColor)
{
	if (isDimmedOn)
	{
		_imgBG->SetColorAndOpacity(InColor);
	}
	else
	{
		Close();
	}
}

void UGsUITrayDimmed::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);
}
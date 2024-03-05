// Copyright Epic Games, Inc. All Rights Reserved.

#include "SButtonEx.h"

static FName SButtonExTypeName("SButtonEx");

void SButtonEx::Construct(const FArguments& InArgs)
{
	SButton::Construct(InArgs);

	// Only do this if we're exactly an SButton
	if (GetType() == SButtonExTypeName)
	{
		SetCanTick(false);
	}
}

FReply SButtonEx::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (IsEnabled() && (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.IsTouchEvent()))
	{
		OnMouseButtonDownEx.ExecuteIfBound(MyGeometry, MouseEvent);
	}

	return SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SButtonEx::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	const EButtonClickMethod::Type InputClickMethod = GetClickMethodFromInputType(MouseEvent);
	const bool bMustBePressed = InputClickMethod == EButtonClickMethod::DownAndUp || InputClickMethod == EButtonClickMethod::PreciseClick;
	const bool bMeetsPressedRequirements = (!bMustBePressed || (bIsPressed && bMustBePressed));

	if (bMeetsPressedRequirements && ((MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton || MouseEvent.IsTouchEvent())))
	{
		OnMouseButtonUpEx.ExecuteIfBound(MyGeometry, MouseEvent);
	}

	return SButton::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SButtonEx::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	OnMouseButtonMoveEx.ExecuteIfBound(MyGeometry, MouseEvent);

	return SButton::OnMouseMove(MyGeometry, MouseEvent);
}

void SButtonEx::OnFocusLost(const FFocusEvent& InFocusEvent)
{
	SButton::OnFocusLost(InFocusEvent);

	OnMouseFocusLostEx.ExecuteIfBound();
}

void SButtonEx::OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	SButton::OnMouseCaptureLost(CaptureLostEvent);

	OnnMouseCaptureLostEx.ExecuteIfBound();
}

void SButtonEx::OnDragLeave(const FDragDropEvent& DragDropEvent)
{
	SButton::OnDragLeave(DragDropEvent);

	OnMouseDragLeave.ExecuteIfBound();
}

void SButtonEx::OnFinishedLongPress()
{
	Release();
}

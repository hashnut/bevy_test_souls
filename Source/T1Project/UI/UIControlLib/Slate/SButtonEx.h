// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Framework/SlateDelegates.h"
#include "Styling/SlateWidgetStyleAsset.h"
#include "Widgets/Input/SButton.h"


/**
 * 이벤트를 받기위해 SButton을 상속받은 클래스 
 */
class SButtonEx	: public SButton
{
public:
	DECLARE_DELEGATE_TwoParams(FOnButtonPointerEventEx, const FGeometry&, const FPointerEvent&);
	DECLARE_DELEGATE(FOnMouseFocusStateEx);

public:
	FOnButtonPointerEventEx OnMouseButtonDownEx;
	FOnButtonPointerEventEx OnMouseButtonUpEx;
	FOnButtonPointerEventEx OnMouseButtonMoveEx;

	FOnMouseFocusStateEx OnMouseFocusLostEx;
	FOnMouseFocusStateEx OnnMouseCaptureLostEx;
	FOnMouseFocusStateEx OnMouseDragLeave;

public:
	void Construct(const FArguments& InArgs);

public:
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual void OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual void OnDragLeave(const FDragDropEvent& DragDropEvent) override;

	void OnFinishedLongPress();
};

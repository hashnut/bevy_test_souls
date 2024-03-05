// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "GsUIWidgeTouchClose.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIWidgeTouchClose : public UGsUIWidgetBase
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnTouchStartDel);

	FOnTouchStartDel OnTouchStart;
protected:
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
};

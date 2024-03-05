// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/LayOut/SScrollBox.h"

DECLARE_DELEGATE_TwoParams(
FOnTIck,
const FGeometry&, const float);

/**
 * 
 */
class T1PROJECT_API SGsScrollBox : public SScrollBox
{
	using Super = SScrollBox;
public:
	SLATE_BEGIN_ARGS(SGsScrollBox)
		: _Style(&FCoreStyle::Get().GetWidgetStyle<FScrollBoxStyle>("ScrollBox"))
		, _ScrollBarStyle(&FCoreStyle::Get().GetWidgetStyle<FScrollBarStyle>("ScrollBar"))
		, _ExternalScrollbar()
		, _Orientation(Orient_Vertical)
		, _ScrollBarVisibility(EVisibility::Visible)
		, _ScrollBarAlwaysVisible(false)
		, _ScrollBarDragFocusCause(EFocusCause::Mouse)
		, _ScrollBarThickness(FVector2D(9.0f, 9.0f))
		, _ScrollBarPadding(2.0f)
		, _AllowOverscroll(EAllowOverscroll::Yes)
		, _AnimateWheelScrolling(false)
		, _WheelScrollMultiplier(1.f)
		, _NavigationDestination(EDescendantScrollDestination::IntoView)
		, _NavigationScrollPadding(0.0f)
		, _ScrollWhenFocusChanges(EScrollWhenFocusChanges::NoScroll)
		, _OnUserScrolled()
		, _ConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible)
		, _OnNativeTick()
	{
		
	}
	SLATE_SUPPORTS_SLOT(FSlot)

	/** Style used to draw this scrollbox */
	SLATE_STYLE_ARGUMENT(FScrollBoxStyle, Style)

	/** Style used to draw this scrollbox's scrollbar */
	SLATE_STYLE_ARGUMENT(FScrollBarStyle, ScrollBarStyle)

	/** Custom scroll bar */
	SLATE_ARGUMENT(TSharedPtr<SScrollBar>, ExternalScrollbar)

	/** The direction that children will be stacked, and also the direction the box will scroll. */
	SLATE_ARGUMENT(EOrientation, Orientation)

	SLATE_ARGUMENT(EVisibility, ScrollBarVisibility)

	SLATE_ARGUMENT(bool, ScrollBarAlwaysVisible)

	SLATE_ARGUMENT(EFocusCause, ScrollBarDragFocusCause)

	SLATE_ARGUMENT(FVector2D, ScrollBarThickness)

	SLATE_ARGUMENT(FMargin, ScrollBarPadding)

	SLATE_ARGUMENT(EAllowOverscroll, AllowOverscroll);

	SLATE_ARGUMENT(bool, AnimateWheelScrolling);

	SLATE_ARGUMENT(float, WheelScrollMultiplier);

	SLATE_ARGUMENT(EDescendantScrollDestination, NavigationDestination);

	/**
	 * The amount of padding to ensure exists between the item being navigated to, at the edge of the
	 * scrollbox.  Use this if you want to ensure there's a preview of the next item the user could scroll to.
	 */
	SLATE_ARGUMENT(float, NavigationScrollPadding);

	SLATE_ARGUMENT(EScrollWhenFocusChanges, ScrollWhenFocusChanges);

	/** Called when the button is clicked */
	SLATE_EVENT(FOnUserScrolled, OnUserScrolled)

	SLATE_ARGUMENT(EConsumeMouseWheel, ConsumeMouseWheel);
	
	SLATE_EVENT(FOnTIck, OnNativeTick)
	
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

protected:
	FOnTIck OnNativeTick;
	TFunction<void()> ScrollIntoFindWidgetRequest;

private:
	bool _clearScroll{ false };

public:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

public:
	void ScrollWidgetIntoView(const TSharedPtr<SWidget>& WidgetToScrollIntoView, bool InAnimateScroll = true, 
		EDescendantScrollDestination InDestination = EDescendantScrollDestination::IntoView, float Padding = 0);

	void SetClearScroll();

protected:
	FORCEINLINE float GetScrollCompoFromVector(FVector2D Vector) const
	{
		return Orientation == Orient_Vertical ? Vector.Y : Vector.X;
	}
};

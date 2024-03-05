// Fill out your copyright notice in the Description page of Project Settings.


#include "SGsScrollBox.h"
#include "SlateOptMacros.h"
//#include "T1Project.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SGsScrollBox::Construct(const FArguments& InArgs)
{
	Super::FArguments Arg;

	Super::Construct
	(
		Super::FArguments()
		.Style(InArgs._Style)
		.ScrollBarStyle(InArgs._ScrollBarStyle)
		.OnUserScrolled(InArgs._OnUserScrolled)
		.Orientation(InArgs._Orientation)
		.ConsumeMouseWheel(InArgs._ConsumeMouseWheel)
		.AllowOverscroll(InArgs._AllowOverscroll)
		.AnimateWheelScrolling(InArgs._AnimateWheelScrolling)
		.WheelScrollMultiplier(InArgs._WheelScrollMultiplier)
		.NavigationScrollPadding(InArgs._NavigationScrollPadding)
		.NavigationDestination(InArgs._NavigationDestination)
		.ScrollWhenFocusChanges(InArgs._ScrollWhenFocusChanges)
	);

	OnNativeTick = InArgs._OnNativeTick;
}

void SGsScrollBox::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	bool wasScrollEnd = bScrollToEnd;
	const float PreviousScrollOffset = GetScrollOffset();

	Super::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (wasScrollEnd || ScrollIntoFindWidgetRequest) //end로 가 있는지 계속 체크
	{
		const FGeometry ScrollPanelGeometry = FindChildGeometry(AllottedGeometry, ScrollPanel.ToSharedRef());
		const float ContentSize = _clearScroll ? 0.f : GetScrollCompoFromVector(ScrollPanel->GetDesiredSize());

		auto Offset = GetScrollOffset();
		bool isProcess = !FMath::IsNearlyEqual(PreviousScrollOffset, Offset, 0.1f) || FMath::IsNearlyZero(ContentSize);

		if (wasScrollEnd)
		{
			bScrollToEnd = isProcess;
			ScrollIntoFindWidgetRequest = nullptr;
		}
		
		if (ScrollIntoFindWidgetRequest)
		{
			if (isProcess) ScrollIntoFindWidgetRequest();
			else ScrollIntoFindWidgetRequest = nullptr;
		}

		if (bScrollToEnd || ScrollIntoFindWidgetRequest)
		{
			Invalidate(EInvalidateWidget::LayoutAndVolatility);
		}

		//GSLOG(Log, TEXT("*********** SGsScrollBox::Tick [%d] [PrevOffset : %f][Offset : %f] [%f]"), bScrollToEnd, PreviousScrollOffset, Offset, ContentSize);
	}

	_clearScroll = false;
	OnNativeTick.ExecuteIfBound(AllottedGeometry, InDeltaTime);
}

void SGsScrollBox::ScrollWidgetIntoView(const TSharedPtr<SWidget>& WidgetToScrollIntoView, bool InAnimateScroll, EDescendantScrollDestination InDestination, float InScrollPadding)
{	
	ScrollIntoFindWidgetRequest = [this, WidgetToScrollIntoView, InAnimateScroll, InDestination, InScrollPadding]() {
		ScrollDescendantIntoView(WidgetToScrollIntoView, InAnimateScroll, InDestination, InScrollPadding);  
	};

	ScrollDescendantIntoView(WidgetToScrollIntoView, InAnimateScroll, InDestination, InScrollPadding);
}

void SGsScrollBox::SetClearScroll()
{
	_clearScroll = true;
	ScrollIntoFindWidgetRequest = nullptr;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION


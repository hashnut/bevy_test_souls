// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCheckBox.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SCheckBox.h"


void UGsCheckBox::BeginDestroy()
{
	_onVisibleList.Empty();
	_onVisibleNoHitList.Empty();
	_offVisibleList.Empty();
	_offVisibleNoHitList.Empty();

	Super::BeginDestroy();
}

TSharedRef<SWidget> UGsCheckBox::RebuildWidget()
{
	MyCheckbox = SNew(SCheckBox)
		.OnCheckStateChanged(BIND_UOBJECT_DELEGATE(FOnCheckStateChanged, SlateOnCheckStateChangedCallbackEx))
		.Style(&WidgetStyle)
		.HAlign(HorizontalAlignment)
		.IsFocusable(IsFocusable)
		;

	if (GetChildrenCount() > 0)
	{
		MyCheckbox->SetContent(GetContentSlot()->Content ? GetContentSlot()->Content->TakeWidget() : SNullWidget::NullWidget);
	}

	return MyCheckbox.ToSharedRef();
}

void UGsCheckBox::SlateOnCheckStateChangedCallbackEx(ECheckBoxState NewState)
{
	CheckedState = NewState;

	//@TODO: Choosing to treat Undetermined as Checked
	const bool bWantsToBeChecked = NewState != ECheckBoxState::Unchecked;
	OnCheckStateChanged.Broadcast(bWantsToBeChecked);
	
	OnCheckStateChangedEx.Broadcast(_indexInGroup, bWantsToBeChecked);

	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, bWantsToBeChecked);
	}
}

void UGsCheckBox::SetIsSelected(bool bInIsSelected)
{
	SetIsChecked(bInIsSelected);

	if (bInIsSelected)
	{
		SetVisibilityTargetList(_onVisibleList, ESlateVisibility::Visible);
		SetVisibilityTargetList(_onVisibleNoHitList, ESlateVisibility::SelfHitTestInvisible);
		SetVisibilityTargetList(_offVisibleList, ESlateVisibility::Hidden);
		SetVisibilityTargetList(_offVisibleNoHitList, ESlateVisibility::Hidden);
	}
	else
	{
		SetVisibilityTargetList(_onVisibleList, ESlateVisibility::Hidden);
		SetVisibilityTargetList(_onVisibleNoHitList, ESlateVisibility::Hidden);
		SetVisibilityTargetList(_offVisibleList, ESlateVisibility::Visible);
		SetVisibilityTargetList(_offVisibleNoHitList, ESlateVisibility::SelfHitTestInvisible);
	}

	if (OnToggleGroupSelected.IsBound())
	{
		OnToggleGroupSelected.Broadcast(bInIsSelected);
	}
}

bool UGsCheckBox::GetIsSelected() const
{
	return IsChecked();
}

void UGsCheckBox::AddWidgetToggleOnVisible(UWidget* InWidget)
{
	_onVisibleList.Add(InWidget);
}

void UGsCheckBox::AddWidgetToggleOnVisibleNoHit(UWidget* InWidget)
{
	_onVisibleNoHitList.Add(InWidget);
}

void UGsCheckBox::AddWidgetToggleOffVisible(UWidget* InWidget)
{
	_offVisibleList.Add(InWidget);
}

void UGsCheckBox::AddWidgetToggleOffVisibleNoHit(UWidget* InWidget)
{
	_offVisibleNoHitList.Add(InWidget);
}

void UGsCheckBox::SetVisibilityTargetList(TArray<TWeakObjectPtr<UWidget>>& InList, ESlateVisibility InVisibility)\
{
	for (TWeakObjectPtr<UWidget> widget : InList)
	{
		if (widget.IsValid())
		{
			widget->SetVisibility(InVisibility);
		}
	}
}

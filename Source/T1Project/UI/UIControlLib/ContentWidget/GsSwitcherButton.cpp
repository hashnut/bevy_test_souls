// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSwitcherButton.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/Input/SButton.h"
#include "Components/ButtonSlot.h"
#include "Components/WidgetSwitcher.h"


void UGsSwitcherButton::BeginDestroy()
{
	_toggleCallback = nullptr;
	_switcher = nullptr;

	Super::BeginDestroy();
}

TSharedRef<SWidget> UGsSwitcherButton::RebuildWidget()
{
	MyButton = SNew(SButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClickedEx))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressedEx))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleasedEx))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		;

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	_bIsFirstSetSelected = true;
	_switcher = RecursiveFindSwitcher(this);

	_oriClickLockSecond = _clickLockSecond;

	// 리셋
	_lockEndTime = 0;
	_lastClickTime = 0;

	return MyButton.ToSharedRef();
}

FReply UGsSwitcherButton::SlateHandleClickedEx()
{
	FReply result = Super::SlateHandleClickedEx();

	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}

	return result;
}

void UGsSwitcherButton::SetIsSelected(bool bInIsSelected)
{
	// 처음이거나 변경되었을 때만 세팅
	if (_bIsFirstSetSelected || 
		_bIsSelected != bInIsSelected)
	{
		_bIsFirstSetSelected = false;
		
		SetSwitcherIndex(bInIsSelected ? _slotIndexSelected : _slotIndexUnselected);
	}

	_bIsSelected = bInIsSelected;

	if (OnSwitcherButtonSelected.IsBound())
	{
		OnSwitcherButtonSelected.Broadcast(bInIsSelected);
	}
}

bool UGsSwitcherButton::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsSwitcherButton::SetSwitcherIndex(int32 InIndex)
{
	if (_switcher.IsValid())
	{
		_switcher->SetActiveWidgetIndex(InIndex);
	}
}

int32 UGsSwitcherButton::GetSwitcherIndex() const
{
	if (_switcher.IsValid())
	{
		return _switcher->GetActiveWidgetIndex();
	}

	return 0;
}

int32 UGsSwitcherButton::GetNumWidgets()
{
	return _switcher.IsValid() ? _switcher->GetNumWidgets() : 0; 
}

UWidgetSwitcher* UGsSwitcherButton::RecursiveFindSwitcher(UPanelWidget* InPanel) const
{
	for (int32 i = 0, childCount = InPanel->GetChildrenCount(); i < childCount; ++i)
	{
		UWidget* childWidget = InPanel->GetChildAt(i);
		if (childWidget->IsA<UWidgetSwitcher>())
		{
			return Cast<UWidgetSwitcher>(childWidget);
		}
		else
		{
			if (childWidget->IsA<UPanelWidget>())
			{
				if (UWidgetSwitcher* switcher = RecursiveFindSwitcher(Cast<UPanelWidget>(childWidget)))
				{
					return switcher;
				}
			}
		}
	}

	return nullptr;
}

void UGsSwitcherButton::SlateHandleReleasedEx()
{
	if (UWorld* world = GetWorld())
	{
		if (APlayerController* controller = world->GetFirstPlayerController())
		{
			FVector2D location;
			controller->GetMousePosition(location.X, location.Y);
		}
	}

	UButton::SlateHandleReleased();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsToggleButton.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/Input/SButton.h"
#include "Components/ButtonSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


void UGsToggleButton::BeginDestroy()
{
	_toggleCallback = nullptr;

	Super::BeginDestroy();
}

TSharedRef<SWidget> UGsToggleButton::RebuildWidget()
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

	return MyButton.ToSharedRef();
}

FReply UGsToggleButton::SlateHandleClickedEx()
{
	FReply result = Super::SlateHandleClickedEx();

	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}

	return result;
}

void UGsToggleButton::SetIsSelected(bool bInIsSelected)
{
	// 처음이거나 변경되었을 때만 세팅
	if (_bIsFirstSetSelected || 
		_bIsSelected != bInIsSelected)
	{
		_bIsFirstSetSelected = false;

		// 버튼의 스타일 변경
		FButtonStyle& targetStyle = (bInIsSelected) ? WidgetStyle : _uncheckedWidgetStyle;
		SetStyleEx(targetStyle);

		// 버튼 자식 중 UImage, UTextBlock의 색상 변경
		FLinearColor targetColor = (bInIsSelected) ? _checkedColor : _uncheckedColor;
		RecursiveSetColor(this, targetColor);
	}

	_bIsSelected = bInIsSelected;

	if (OnToggleButtonSelected.IsBound())
	{
		OnToggleButtonSelected.Broadcast(bInIsSelected);
	}
}

bool UGsToggleButton::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsToggleButton::SetIsSelectedNoEvent(bool bInIsSelected)
{
	// 버튼의 스타일 변경
	FButtonStyle& targetStyle = (bInIsSelected) ? WidgetStyle : _uncheckedWidgetStyle;
	SetStyleEx(targetStyle);

	// 버튼 자식 중 UImage, UTextBlock의 색상 변경
	FLinearColor targetColor = (bInIsSelected) ? _checkedColor : _uncheckedColor;
	RecursiveSetColor(this, targetColor);
}

void UGsToggleButton::SetStyleEx(const FButtonStyle& InStyle)
{
	if (MyButton.IsValid())
	{
		MyButton->SetButtonStyle(&InStyle);
	}
}

void UGsToggleButton::RecursiveSetColor(UPanelWidget* InPanel, const FLinearColor& InColor)
{
	for (int32 i = 0, childCount = InPanel->GetChildrenCount(); i < childCount; ++i)
	{
		UWidget* childWidget = InPanel->GetChildAt(i);
		if (nullptr == childWidget)
		{
			continue;
		}

		if (childWidget->IsA<UImage>())
		{
			if (UImage* image = Cast<UImage>(childWidget))
			{
				image->SetColorAndOpacity(InColor);
			}
		}
		else if (childWidget->IsA<UTextBlock>())
		{
			if (UTextBlock* textBlock = Cast<UTextBlock>(childWidget))
			{
				textBlock->SetColorAndOpacity(InColor);
			}
		}
		else if (childWidget->IsA<UPanelWidget>())
		{
			RecursiveSetColor(Cast<UPanelWidget>(childWidget), InColor);
		}
	}
}

void UGsToggleButton::SlateHandleReleasedEx()
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITooltip.h"
#include "Components/PanelWidget.h"


void UGsUITooltip::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 포커싱 가능한 위젯으로 만든다
	bIsFocusable = true;
}

void UGsUITooltip::NativeConstruct()
{
	Super::NativeConstruct();	
	
	SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUITooltip::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);

	// 포커스를 잃으면 닫는다. 
	// 위젯 바깥 클릭이 감지되지 않아 이런 식으로 처리.
	Close();
}

void UGsUITooltip::Open()
{
	SetVisibility(ESlateVisibility::Visible);

	// 열자마자 포커스를 둔다
	SetFocus();
}

void UGsUITooltip::Close()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

bool UGsUITooltip::IsOpen() const
{
	ESlateVisibility slateVisibility = GetVisibility();
	if (ESlateVisibility::Hidden == slateVisibility ||
		ESlateVisibility::Collapsed == slateVisibility)
	{
		return false;
	}

	return true;
}
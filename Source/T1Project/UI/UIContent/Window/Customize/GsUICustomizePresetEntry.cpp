// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICustomizePresetEntry.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/TextBlock.h"


void UGsUICustomizePresetEntry::NativeOnInitialized()
{
	_btnNormal->OnClicked.AddDynamic(this, &UGsUICustomizePresetEntry::OnClickNormal);
	_btnSelected->OnClicked.AddDynamic(this, &UGsUICustomizePresetEntry::OnClickSelected);

	Super::NativeOnInitialized();
}

void UGsUICustomizePresetEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_btnNormal->SetVisibility((bInIsSelected) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	_btnSelected->SetVisibility((bInIsSelected) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

bool UGsUICustomizePresetEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUICustomizePresetEntry::OnClickNormal()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, true);
	}
}

void UGsUICustomizePresetEntry::OnClickSelected()
{
	// ToggleGroup에 콜백 전달. 끄기 위해 false 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, false);
	}
}

void UGsUICustomizePresetEntry::SetData(int32 InTempId)
{
	// 두자릿수 표기 01, 02, ...
	_textTempId = FText::FromString(FString::Printf(TEXT("%02d"), InTempId));
}

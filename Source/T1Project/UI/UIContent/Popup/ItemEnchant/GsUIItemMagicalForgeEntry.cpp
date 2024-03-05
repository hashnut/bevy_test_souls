// Fill out your copyright notice in the Description page of Project Settings.

#include "GsUIItemMagicalForgeEntry.h"

#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

#include "UI/UIContent/Popup/ItemEnchant/GsUIItemMagicalForgeTextEntry.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"


void UGsUIItemMagicalForgeEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelperCurrent = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperCurrent->Initialize(_textEntryClassCurrent, _panelCurrent);
	_slotHelperCurrent->OnRefreshEntry.AddDynamic(this, &UGsUIItemMagicalForgeEntry::OnRefreshCurrentEntry);

	_slotHelperExpect = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperExpect->Initialize(_textEntryClassExpect, _panelExpect);
	_slotHelperExpect->OnRefreshEntry.AddDynamic(this, &UGsUIItemMagicalForgeEntry::OnRefreshExpectEntry);
}

void UGsUIItemMagicalForgeEntry::SetTitleText(const FText& InText)
{
	_textBlockTitle->SetText(InText);
}

void UGsUIItemMagicalForgeEntry::SetCurrentText(const FText& InText)
{
	FString strCurrent = InText.ToString();
	_currentLines.Empty();
	strCurrent.ParseIntoArray(_currentLines, TEXT("\n"));

	_isRefreshColor = false;
	_slotHelperCurrent->RefreshAll(_currentLines.Num());
}

void UGsUIItemMagicalForgeEntry::SetCurrentColor(const FLinearColor& InColor)
{
	_currentColor = InColor;

	_isRefreshColor = true;
	_slotHelperCurrent->RefreshAll(_currentLines.Num());
}

void UGsUIItemMagicalForgeEntry::SetExpectText(const FText& InText)
{
	FString strExpect = InText.ToString();
	_expectLines.Empty();
	strExpect.ParseIntoArray(_expectLines, TEXT("\n"));

	_isRefreshColor = false;
	_slotHelperExpect->RefreshAll(_expectLines.Num());
}

void UGsUIItemMagicalForgeEntry::SetExpectColor(const FLinearColor& InColor)
{
	_expectColor = InColor;

	_isRefreshColor = true;
	_slotHelperExpect->RefreshAll(_expectLines.Num());
}

void UGsUIItemMagicalForgeEntry::OnRefreshCurrentEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIItemMagicalForgeTextEntry* textEntry = Cast<UGsUIItemMagicalForgeTextEntry>(InEntry);
	if (nullptr == textEntry)
		return;

	if (false == _currentLines.IsValidIndex(InIndex))
		return;

	if (_isRefreshColor)
	{
		textEntry->SetDescColor(_currentColor);
	}
	else
	{
		textEntry->SetDescText(FText::FromString(_currentLines[InIndex]));
	}
}

void UGsUIItemMagicalForgeEntry::OnRefreshExpectEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIItemMagicalForgeTextEntry* textEntry = Cast<UGsUIItemMagicalForgeTextEntry>(InEntry);
	if (nullptr == textEntry)
		return;

	if (false == _expectLines.IsValidIndex(InIndex))
		return;

	if (_isRefreshColor)
	{
		textEntry->SetDescColor(_expectColor);
	}
	else
	{
		textEntry->SetDescText(FText::FromString(_expectLines[InIndex]));
	}
}
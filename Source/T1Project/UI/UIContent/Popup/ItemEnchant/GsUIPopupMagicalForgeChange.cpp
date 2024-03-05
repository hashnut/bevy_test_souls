// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMagicalForgeChange.h"

#include "Components/TextBlock.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"


void UGsUIPopupMagicalForgeChange::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnCancel)
	{
		_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupMagicalForgeChange::OnClickCancel);
	}
	if (_btnOk)
	{
		_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupMagicalForgeChange::OnClickOk);
	}
}

void UGsUIPopupMagicalForgeChange::NativeConstruct()
{
	Super::NativeConstruct();

	_buttonCallback = nullptr;
}

void UGsUIPopupMagicalForgeChange::NativeDestruct()
{
	_buttonCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupMagicalForgeChange::OnInputOk()
{
	
}

void UGsUIPopupMagicalForgeChange::OnInputCancel()
{
	Close();
}

bool UGsUIPopupMagicalForgeChange::OnBack()
{
	OnInputCancel();

	return true;
}

void UGsUIPopupMagicalForgeChange::SetCurrentText(const FText& InText, const FSlateColor& InColor)
{
	_textBlockCurrent->SetText(InText);
	_textBlockCurrent->SetColorAndOpacity(InColor);
}

void UGsUIPopupMagicalForgeChange::SetExpectText(const FText& InText, const FSlateColor& InColor)
{
	_textBlockExpect->SetText(InText);
	_textBlockExpect->SetColorAndOpacity(InColor);
}

void UGsUIPopupMagicalForgeChange::SetCallback(TFunction<void(bool)> InCallback)
{
	_buttonCallback = InCallback;
}

void UGsUIPopupMagicalForgeChange::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupMagicalForgeChange::OnClickOk()
{
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}

	Close();
}

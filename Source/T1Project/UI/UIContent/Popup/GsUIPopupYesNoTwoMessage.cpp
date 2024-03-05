 // Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupYesNoTwoMessage.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "RichTextBlock.h"


void UGsUIPopupYesNoTwoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnYes->OnClicked.AddDynamic(this, &UGsUIPopupYesNoTwoMessage::OnClickYes);
	_btnNo->OnClicked.AddDynamic(this, &UGsUIPopupYesNoTwoMessage::OnClickNo);
}

void UGsUIPopupYesNoTwoMessage::NativeDestruct()
{
	_buttonCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupYesNoTwoMessage::OnInputOk()
{
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}
}

void UGsUIPopupYesNoTwoMessage::OnInputCancel()
{
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}
}

void UGsUIPopupYesNoTwoMessage::SetData(const FText& InFirstMsg, const FText& InSecondMsg,
	TFunction<void(bool)> InCallback)
{
	_richTextBlockFirstMsg->SetText(InFirstMsg);
	_richTextBlockSecondMsg->SetText(InSecondMsg);

	FText findText;
	FText::FindText(TEXT("UICommonText"), TEXT("DefaultPopupButtonOK"), findText);
	_textBlockYesButton->SetText(findText);

	_buttonCallback = InCallback;
}

void UGsUIPopupYesNoTwoMessage::SetData(const FText& InFirstMsg, const FText& InYesButton, const FText& InSecondMsg, 
	TFunction<void(bool)> InCallback)
{
	_richTextBlockFirstMsg->SetText(InFirstMsg);
	_richTextBlockSecondMsg->SetText(InSecondMsg);
	_textBlockYesButton->SetText(InYesButton);

	_buttonCallback = InCallback;
}

void UGsUIPopupYesNoTwoMessage::OnClickYes()
{
	OnInputOk();
}

void UGsUIPopupYesNoTwoMessage::OnClickNo()
{
	OnInputCancel();
}

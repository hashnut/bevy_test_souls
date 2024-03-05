// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupOKTwoMessage.h"
#include "RichTextBlock.h"
#include "TextBlock.h"


void UGsUIPopupOKTwoMessage::NativeDestruct()
{
	_okCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupOKTwoMessage::OnInputOk()
{
	Close();

	if (nullptr != _okCallback)
	{
		_okCallback();
	}
}

void UGsUIPopupOKTwoMessage::SetData(const FText& InFirstMsg, const FText& InSecondMsg,
	TFunction<void()> InCallback)
{
	FText findText;
	FText::FindText(TEXT("UICommonText"), TEXT("DefaultPopupButtonOK"), findText);
	_buttonText->SetText(findText);

	_richTextBlockFirstMsg->SetText(InFirstMsg);
	_richTextBlockSecondMsg->SetText(InSecondMsg);

	_okCallback = InCallback;
}

void UGsUIPopupOKTwoMessage::SetData(const FText& InFirstMsg, const FText& InSecondMsg, const FText& InButton,
	TFunction<void()> InOKCallback)
{
	_buttonText->SetText(InButton);
	_richTextBlockFirstMsg->SetText(InFirstMsg);
	_richTextBlockSecondMsg->SetText(InSecondMsg);	

	_okCallback = InOKCallback;
}

void UGsUIPopupOKTwoMessage::OnClickOk()
{
	OnInputOk();
}

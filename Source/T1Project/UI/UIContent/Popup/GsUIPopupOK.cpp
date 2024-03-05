// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupOK.h"
#include "RichTextBlock.h"
#include "TextBlock.h"


void UGsUIPopupOK::NativeDestruct()
{
	_okCallback = nullptr;

	Super::NativeDestruct();
}

bool UGsUIPopupOK::OnBack()
{
	if (nullptr != _okCallback)
	{
		_okCallback();
	}

	Close();

	return true;
}

void UGsUIPopupOK::OnInputOk()
{
	if (nullptr != _okCallback)
	{
		_okCallback();
	}

	Close();
}

void UGsUIPopupOK::OnInputCancel()
{
	if (nullptr != _okCallback)
	{
		_okCallback();
	}

	Close();
}

void UGsUIPopupOK::SetData(const FText& InContent, const FText& InButton, TFunction<void()> InOKCallback)
{
	_contentRichText->SetText(InContent);
	_buttonText->SetText(InButton);
	_okCallback = InOKCallback;
}

void UGsUIPopupOK::OnClickOk()
{
	OnInputOk();
}

void UGsUIPopupOK::SetData(const FText& InContent, TFunction<void()> InOKCallback)
{
	FText btnText;
	FText::FindText(TEXT("LauncherText"), TEXT("DefaultPopupButtonOK"), btnText); // TEXT: 확인

	SetData(InContent, btnText, InOKCallback);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupYesNo.h"
#include "RichTextBlock.h"

#include "Management/ScopeGame/GsEventProgressManager.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

void UGsUIPopupYesNo::NativeDestruct()
{
	_buttonCallback = nullptr;

	if (nullptr != _destructCallback)
	{
		_destructCallback(_isCallEventProcessFinish);
		_destructCallback = nullptr;
	}

	Super::NativeDestruct();
}

bool UGsUIPopupYesNo::OnBack()
{
	// 취소했을 때와 동일한 로직을 타도록 함
	_isCallEventProcessFinish = true;
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}

	Close(true);

	return true;
}

void UGsUIPopupYesNo::OnInputOk()
{
	_isCallEventProcessFinish = false;
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}

	Close();
}

void UGsUIPopupYesNo::OnInputCancel()
{
	_isCallEventProcessFinish = true;
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}

	Close(true);
}

void UGsUIPopupYesNo::OnClickYes()
{
	OnInputOk();
}

void UGsUIPopupYesNo::OnClickNo()
{
	OnInputCancel();
}

void UGsUIPopupYesNo::SetData(const FText& InContent, const FText& InYesButton, const FText& InNoButton,
	TFunction<void(bool)> InCallback,
	TFunction<void(bool)> InDestructCallback)
{
	_contentRichText->SetText(InContent);
	_yesButtonText = InYesButton;
	_noButtonText = InNoButton;
	_buttonCallback = InCallback;

	_isCallEventProcessFinish = true;

	_destructCallback = InDestructCallback;
}

void UGsUIPopupYesNo::SetData(const FText& InContent, TFunction<void(bool)> InCallback,
	TFunction<void(bool)> InDestructCallback)
{
	FText yesText;
	FText noText;
	FText::FindText(TEXT("LauncherText"), TEXT("DefaultPopupButtonOK"), yesText); // TEXT: 확인
	FText::FindText(TEXT("LauncherText"), TEXT("DefaultPopupButtonNo"), noText); // TEXT: 취소

	SetData(InContent, yesText, noText, InCallback, InDestructCallback);
}

void UGsUIPopupYesNo::SetData(TFunction<void(bool)> InCallback,
	TFunction<void(bool)> InDestructCallback)
{
	_buttonCallback = InCallback;
	_destructCallback = InDestructCallback;
}

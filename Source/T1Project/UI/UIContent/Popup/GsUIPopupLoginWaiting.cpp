// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupLoginWaiting.h"
#include "RichTextBlock.h"
#include "Components/TextBlock.h"

#include "Management/ScopeGame/GsEventProgressManager.h"
#include "EventProgress/GsEventProgressDefine.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIPopupLoginWaiting::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupLoginWaiting::OnClickCancel);
}

void UGsUIPopupLoginWaiting::NativeDestruct()
{
	_loginWaitingCount = 0;
	_cancelCallback = nullptr;

	Super::NativeDestruct();
}

bool UGsUIPopupLoginWaiting::OnBack()
{
	if (nullptr != _cancelCallback)
	{
		_cancelCallback();
	}

	Close(true);

	return true;
}

void UGsUIPopupLoginWaiting::OnClickCancel()
{
	if (nullptr != _cancelCallback)
	{
		_cancelCallback();
	}

	Close(true);
}

void UGsUIPopupLoginWaiting::SetData(int InWaitingCount,
	TFunction<void()> InCancelCallback)
{
	_loginWaitingCount = InWaitingCount;
	_cancelCallback = InCancelCallback;

	FText findText;
	FText::FindText(TEXT("LauncherText"), TEXT("ServerWaitingCount"), findText);

	if (_loginWaitingText)
	{
		_loginWaitingText->SetText(FText::Format(findText, InWaitingCount));		
	}	
}

void UGsUIPopupLoginWaiting::OnInputCancel()
{
	OnClickCancel();
}
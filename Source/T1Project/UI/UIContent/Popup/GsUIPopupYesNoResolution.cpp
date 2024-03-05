 // Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupYesNoResolution.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "RichTextBlock.h"
#include "TimerManager.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "DateTime.h"

void UGsUIPopupYesNoResolution::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnYes->OnClicked.AddDynamic(this, &UGsUIPopupYesNoResolution::OnClickYes);
	_btnNo->OnClicked.AddDynamic(this, &UGsUIPopupYesNoResolution::OnClickNo);
}

void UGsUIPopupYesNoResolution::NativeDestruct()
{
	_buttonCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupYesNoResolution::OnInputOk()
{
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}

	if (_resolutionTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_resolutionTimerHandle);
	}

	Close();
}

void UGsUIPopupYesNoResolution::OnInputCancel()
{	
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}

	if (_resolutionTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_resolutionTimerHandle);
	}

	Close();
}

void UGsUIPopupYesNoResolution::SetData(const FText& InFirstMsg, const FText& InSecondMsg,
	TFunction<void(bool)> InCallback)
{
	_richTextBlockFirstMsg->SetText(InFirstMsg);
	_richTextBlockSecondMsg->SetText(InSecondMsg);

	_buttonCallback = InCallback;

	_timer = FDateTime::Now();
	_countdown = 10;
	GetWorld()->GetTimerManager().SetTimer(_resolutionTimerHandle, this, &UGsUIPopupYesNoResolution::UpdateTime, 1.f, true);
}

void UGsUIPopupYesNoResolution::SetData(const FText& InFirstMsg, const FText& InYesButton, const FText& InSecondMsg,
	TFunction<void(bool)> InCallback)
{
	_richTextBlockFirstMsg->SetText(InFirstMsg);
	_richTextBlockSecondMsg->SetText(InSecondMsg);
	_textBlockYesButton->SetText(InYesButton);

	_buttonCallback = InCallback;

	_timer = FDateTime::Now();		
	_countdown = 10;
	GetWorld()->GetTimerManager().SetTimer(_resolutionTimerHandle, this, &UGsUIPopupYesNoResolution::UpdateTime, 1.f, true);
}

void UGsUIPopupYesNoResolution::UpdateTime()
{
	FTimespan time = FDateTime::Now() - _timer;

	if (_countdown <= time.GetSeconds())
	{
		OnClickNo();
	}
	else
	{
		FText findText;
		FText::FindText(TEXT("OptionText"), TEXT("Resolution_ChangePopUp_Nsecond"), findText);

		_richTextBlockSecondMsg->SetText(FText::Format(findText, 10 - time.GetSeconds()));
	}
}

void UGsUIPopupYesNoResolution::OnClickYes()
{
	OnInputOk();
}

void UGsUIPopupYesNoResolution::OnClickNo()
{
	OnInputCancel();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowIDPassword.h"
#include "EditableTextBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

void UGsUIWindowIDPassword::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr != _btnIDPassword)
		_btnIDPassword->OnClicked.AddDynamic(this, &UGsUIWindowIDPassword::OnClickIDPassword);

	if (nullptr != _textBlockNotice)
	{
#if WITH_EDITOR
		((UWidget*)_textBlockNotice)->SetVisibility(ESlateVisibility::Visible);
#else
		((UWidget*)_textBlockNotice)->SetVisibility(ESlateVisibility::Hidden);
#endif
	}
}

void UGsUIWindowIDPassword::NativeConstruct()
{
	Super::NativeConstruct();	
}

void UGsUIWindowIDPassword::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UGsUIWindowIDPassword::OnBack()
{
	return false;
}

void UGsUIWindowIDPassword::OnClickIDPassword()
{
	OnClickButton.ExecuteIfBound(*_editID->GetText().ToString(), *_editPassword->GetText().ToString());
}


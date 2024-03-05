// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCharacterDeleteCancel.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "TextBlock.h"

void UGsUIPopupCharacterDeleteCancel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCharacterDeleteCancel::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCharacterDeleteCancel::OnClickCancel);
}

void UGsUIPopupCharacterDeleteCancel::OnClickOk()
{
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}
}

void UGsUIPopupCharacterDeleteCancel::OnClickCancel()
{
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}
}

void UGsUIPopupCharacterDeleteCancel::SetData(const FString& InName, TFunction<void(bool)> InCallback)
{
	_buttonCallback = InCallback;
	_textName->SetText(FText::FromString(InName));
}
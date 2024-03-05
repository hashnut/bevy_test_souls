// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupAccountDelete.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "EditableTextBox.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

void UGsUIPopupAccountDelete::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupAccountDelete::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupAccountDelete::OnClickCancel);

	_editText->SetText(FText::FromString(""));
}

void UGsUIPopupAccountDelete::NativeDestruct()
{
	_buttonCallback = nullptr;

	Super::NativeDestruct();	
}

void UGsUIPopupAccountDelete::OnClickOk()
{
	// 계정 삭제
	FText findText;
	FText::FindText(TEXT("OptionText"), TEXT("AccountDeletePopup_Text2"), findText);
	// 입력하신 글자가 맞지 않습니다.
	FText errorText;
	FText::FindText(TEXT("OptionText"), TEXT("AccountDelete_TextMismatch"), errorText);

	if (nullptr != _editText)
	{
		FString strAccountDelete = _editText->GetText().ToString();
		if (strAccountDelete.IsEmpty())
		{			
			FGsUIHelper::TrayMessageTicker(errorText);
		}
		else
		{
			if (strAccountDelete.Equals(findText.ToString()))
			{
				if (nullptr != _buttonCallback)
				{
					_buttonCallback(true);
					Close();
				}
			}
			else
			{
				FGsUIHelper::TrayMessageTicker(errorText);
			}
		}
	}
}

void UGsUIPopupAccountDelete::OnClickCancel()
{
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}

	Close();	
}

void UGsUIPopupAccountDelete::SetData(TFunction<void(bool)> InCallback)
{
	// 창 재사용 시 이전 입력내용이 남아있지 않게 하기 위함.
	_editText->SetText(FText::GetEmpty());
	_buttonCallback = InCallback;
}

bool UGsUIPopupAccountDelete::OnBack()
{
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}

	Close();

	return true;
}

void UGsUIPopupAccountDelete::OnInputOk()
{
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(true);
	}

	Close();
}

void UGsUIPopupAccountDelete::OnInputCancel()
{
	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}

	Close();
}
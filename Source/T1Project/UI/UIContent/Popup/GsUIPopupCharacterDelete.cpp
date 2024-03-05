// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCharacterDelete.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "EditableTextBox.h"
#include "TextBlock.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

void UGsUIPopupCharacterDelete::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCharacterDelete::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCharacterDelete::OnClickCancel);

	_editName->SetText(FText::FromString(""));
}

void UGsUIPopupCharacterDelete::OnClickOk()
{
	if (nullptr != _editName)
	{
		FString userName = _editName->GetText().ToString();
		if (userName.IsEmpty())
		{
			// 삭제하려는 캐릭터의 이름을 입력하세요
			FText findText;
			FText::FindText(TEXT("LobbyText"), TEXT("Delete_2ndPopup_Text"), findText);
			FGsUIHelper::TrayMessageTicker(findText);			
		}
		else
		{
			if (userName.Equals(_name))
			{
				if (nullptr != _buttonCallback)
				{
					_buttonCallback(true);
					Close();
				}
			}
			else
			{
				// 삭제하려는 캐릭터의 이름이 다릅니다.
				FText findText;
				FText::FindText(TEXT("LobbyText"), TEXT("WrongCharacterName"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
			}
		}
	}
}

void UGsUIPopupCharacterDelete::OnClickCancel()
{
	Close();

	if (nullptr != _buttonCallback)
	{
		_buttonCallback(false);
	}
}

void UGsUIPopupCharacterDelete::SetData(const FString& InName, TFunction<void(bool)> InCallback)
{	
	// 창 재사용 시 이전 입력내용이 남아있지 않게 하기 위함.
	_editName->SetText(FText::GetEmpty());
	_name = InName;	
	_buttonCallback = InCallback;

	_textName->SetText(FText::FromString(_name));
}

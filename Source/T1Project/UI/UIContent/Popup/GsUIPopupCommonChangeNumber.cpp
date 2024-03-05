// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCommonChangeNumber.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/RichTextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Net/GsNetSendServiceGuild.h"
#include "UTIL/GsText.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "Misc/CString.h"


void UGsUIPopupCommonChangeNumber::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCommonChangeNumber::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCommonChangeNumber::OnClickCancel);

	_inputBoxMsg->OnTextChanged.AddDynamic(this, &UGsUIPopupCommonChangeNumber::OnTextChangedMsg);
}

void UGsUIPopupCommonChangeNumber::NativeDestruct()
{
	_minNumber = 0;
	_maxNumber = 0;
	_callbackFunc = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupCommonChangeNumber::SetData(int32 InMinNumber, int32 InMaxNumber, TFunction<void(int32)> InFunc)
{
	_minNumber = InMinNumber;
	_maxNumber = InMaxNumber;
	_callbackFunc = InFunc;

	_inputBoxMsg->SetText(FText::GetEmpty());

	FString strTemp = FString::Format(TEXT("{0}~{1}"), { _minNumber, _maxNumber });
	_inputBoxMsg->SetHintText(FText::FromString(strTemp));
}

void UGsUIPopupCommonChangeNumber::SetMessageText(const FText& InText)
{
	_richTextMessage->SetText(InText);
}

void UGsUIPopupCommonChangeNumber::OnClickOk()
{
	FString strNum = _inputBoxMsg->GetText().ToString();
	if (strNum.IsEmpty())
	{
		return;
	}

	int32 number = FCString::Atoi(*strNum);

	// 제한 숫자를 넘어버리면 값 강제 세팅
	if (number > _maxNumber)
	{
		number = _maxNumber;
	}

	if (number < _minNumber)
	{
		number = _minNumber;
	}

	if (_callbackFunc)
	{
		_callbackFunc(number);
	}

	Close();
}

void UGsUIPopupCommonChangeNumber::OnClickCancel()
{
	Close();
}

void UGsUIPopupCommonChangeNumber::OnTextChangedMsg(const FText& InText)
{
	FString strTemp = InText.ToString();
	FString strNum;

	if (false == strTemp.IsNumeric())
	{
		for (int i = 0; i < strTemp.Len(); ++i)
		{
			FString tempCh;
			tempCh.AppendChar(strTemp[i]);

			if (FCString::IsNumeric(*tempCh))
			{
				strNum.AppendChar(strTemp[i]);
			}
		}
	}
	else
	{
		strNum = strTemp;
	}
	
	if (strNum.IsEmpty())
	{
		_inputBoxMsg->SetText(FText::GetEmpty());
		return;
	}

	int32 number = FCString::Atoi(*strNum);

	// 제한 숫자를 넘어버리면 최대값으로 강제 세팅
	number = FMath::Clamp(number, _minNumber, _maxNumber);

	_inputBoxMsg->SetText(FText::AsNumber(number));
}

void UGsUIPopupCommonChangeNumber::OnInputCancel()
{
	OnClickCancel();
}
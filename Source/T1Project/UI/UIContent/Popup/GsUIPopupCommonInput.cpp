// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCommonInput.h"

#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UTIL/GsUIUtil.h"
#include "UTIL/GsText.h"




void UGsUIPopupCommonInput::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCommonInput::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCommonInput::OnClickCancel);

	_editableTextBox->OnTextChanged.AddDynamic(this, &UGsUIPopupCommonInput::OnTextChanged);
}

void UGsUIPopupCommonInput::NativeConstruct()
{
	Super::NativeConstruct();

	_textBlockDesc->SetText(FText::GetEmpty());
	_textBlockWarning->SetText(FText::GetEmpty());
	_textBlockDesc->SetText(FText::GetEmpty());
	_editableTextBox->SetText(FText::GetEmpty());
	_editableTextBox->SetHintText(FText::GetEmpty());

	_callbackFunc = nullptr;
	_maxTextLength = 0;
}

void UGsUIPopupCommonInput::NativeDestruct()
{
	_callbackFunc = nullptr;
	_maxTextLength = 0;
	
	Super::NativeDestruct();
}

void UGsUIPopupCommonInput::OnInputOk()
{
	OnClickOk();
}

void UGsUIPopupCommonInput::OnInputCancel()
{
	OnClickCancel();
}

void UGsUIPopupCommonInput::SetTitleText(const FText& InTitleText)
{
	_titleText = InTitleText;
}

void UGsUIPopupCommonInput::SetDescText(const FText& InDescText)
{
	_textBlockDesc->SetText(InDescText);
}

void UGsUIPopupCommonInput::SetWarningText(const FText& InWarningText)
{
	_textBlockWarning->SetText(InWarningText);
}

void UGsUIPopupCommonInput::SetHintText(const FText& InHintText)
{
	_editableTextBox->SetHintText(InHintText);
}

void UGsUIPopupCommonInput::SetText(const FText& InText)
{
	_editableTextBox->SetText(InText);
}

void UGsUIPopupCommonInput::SetCallbackFunc(TFunction<void(const FString&)> InFunc)
{
	_callbackFunc = InFunc;
}

void UGsUIPopupCommonInput::SetMaxTextLength(int32 InMaxLength, bool bLengthCheckNameType)
{ 
	_maxTextLength = InMaxLength; 
	_bLengthCheckNameType = bLengthCheckNameType;
}

void UGsUIPopupCommonInput::ProcessCallback()
{
	_callbackFunc(_editableTextBox->GetText().ToString());
}

void UGsUIPopupCommonInput::OnClickOk()
{
	FString strInput = _editableTextBox->GetText().ToString();
	int32 length = (_bLengthCheckNameType) ? 
		FGsTextUtil::GetStringLengthNameType(strInput) : FGsTextUtil::GetStringLengthRegex(strInput);
	
	if (_maxTextLength < length)
	{
		// TEXT: 글자 수 조건이 맞지 않습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongTextLength"), textMsg);
		FGsUIHelper::TrayMessageTicker(textMsg);
		return;
	}

	// 숫자와 지원 언어 외의 문자가 포함되었는지 체크(공백, 특수문자, 이모티콘, 미지원언어)
	if (false == FGsTextUtil::IsValidNameType(strInput))
	{
		// TEXT: 지원하지 않는 문자가 포함 되어있습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongLanguage"), textMsg);
		FGsUIHelper::TrayMessageTicker(textMsg);

		return;
	}

	auto thiz = MakeWeakObjectPtr(this);
	// 금칙어 체크
	FGsTextUtil::IsBanWord(strInput, BannedWordCheckUsage::name, [thiz](bool success, FString inText, FString outText) {
		if (success)
		{
			if (thiz.IsValid())
			{
				thiz->ProcessCallback();
				thiz->Close();
			}
		}
		else
		{
			if (GLocalization()->GetBannedWordProcessErrorCode() < 0)
			{
				FText bannedProcError;
				if (FText::FindText(TEXT("ChatText"), TEXT("ChatError1"), bannedProcError))
				{
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, bannedProcError);
				}
				return;
			}

			FText textMsg;
			FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongLetter"), textMsg);
			FGsUIHelper::TrayMessageTicker(textMsg);
		}
		});
}

void UGsUIPopupCommonInput::OnClickCancel()
{
	Close();
}

void UGsUIPopupCommonInput::OnTextChanged(const FText& InText)
{
	// 글자 수 제한 확인
	FString strInput = InText.ToString();
	int32 length = (_bLengthCheckNameType) ?
		FGsTextUtil::GetStringLengthNameType(strInput) : FGsTextUtil::GetStringLengthRegex(strInput);
	
	// 지정 글자수 초과 입력하지 못하게 막음
	if (_maxTextLength < length)
	{
		strInput.RemoveAt(strInput.Len() - 1);
		_editableTextBox->SetText(FText::FromString(strInput));
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupChangeNickName.h"
#include "Components/EditableTextBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "UTIL/GsText.h"
#include "Input/GsInputEventMsgBase.h"
#include "DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"


void UGsUIPopupChangeNickName::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupChangeNickName::OnClickOK);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupChangeNickName::OnClickCancel);

	_editNickName->OnTextChanged.AddDynamic(this, &UGsUIPopupChangeNickName::OnTextChangedNickName);
}

void UGsUIPopupChangeNickName::NativeConstruct()
{
	Super::NativeConstruct();

	_editNickName->SetText(FText::GetEmpty());	

	if (FGsMessageHolder* msgMgr = GMessage())
	{
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_COMMON_POPUP, true);
		msgMgr->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}
}

void UGsUIPopupChangeNickName::NativeDestruct()
{
	if (FGsMessageHolder* msgMgr = GMessage())
	{
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_COMMON_POPUP, false);
		msgMgr->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}

	Super::NativeDestruct();
}

void UGsUIPopupChangeNickName::OnInputOk()
{
	FString strText = _editNickName->GetText().ToString();
	int32 length = FGsTextUtil::GetStringLengthNameType(strText);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (MIN_CHAR_NAME_LEN > length ||
		MAX_CHAR_NAME_LEN < length)
	{
		// TEXT: 글자 수 조건이 맞지 않습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongTextLength"), textMsg);
		FGsUIHelper::TrayMessageTicker(textMsg);

		return;
	}

	// 숫자와 지원 언어 외의 문자가 포함되었는지 체크(공백, 특수문자, 이모티콘, 미지원언어)
	if (false == FGsTextUtil::IsValidNameType(strText))
	{
		// TEXT: 지원하지 않는 문자가 포함 되어있습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongLanguage"), textMsg);
		FGsUIHelper::TrayMessageTicker(textMsg);

		return;
	}

	auto thiz = MakeWeakObjectPtr(this);
	// 금칙어 체크
	FGsTextUtil::IsBanWord(strText, BannedWordCheckUsage::name, [thiz](bool success, FString inText, FString outText) {
		if (success)
		{
			if (thiz.IsValid())
			{
				thiz->CallOkCallback(outText);
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

void UGsUIPopupChangeNickName::OnInputCancel()
{
	Close();
}

void UGsUIPopupChangeNickName::CallOkCallback(const FString& InStr)
{
	FGsPrimitiveString param(InStr);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::CHANGE_NICKNAME_CHECK_REQUEST, &param);
	// 패킷 보내기 로직 요청. 팝업은 결과를 받은 후 외부에서 닫을거라 닫지 않음
}

void UGsUIPopupChangeNickName::OnTextChangedNickName(const FText& InText)
{
	FString strText = InText.ToString();
	int32 length = FGsTextUtil::GetStringLengthNameType(strText);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (MAX_CHAR_NAME_LEN < length)
	{
		strText.RemoveAt(strText.Len() - 1);
		_editNickName->SetText(FText::FromString(strText));
	}
}

void UGsUIPopupChangeNickName::OnClickOK()
{
	OnInputOk();
}

void UGsUIPopupChangeNickName::OnClickCancel()
{
	OnInputCancel();
}

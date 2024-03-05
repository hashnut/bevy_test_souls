// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildChangeMessage.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableText.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Net/GsNetSendServiceGuild.h"
#include "UTIL/GsText.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"


void UGsUIPopupGuildChangeMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupGuildChangeMessage::OnClickOk);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildChangeMessage::OnClickCancel);

	_inputBoxMsg->OnTextChanged.AddDynamic(this, &UGsUIPopupGuildChangeMessage::OnTextChangedMsg);
}

void UGsUIPopupGuildChangeMessage::NativeConstruct()
{
	Super::NativeConstruct();

	// 비우기
	_inputBoxMsg->SetText(FText::GetEmpty());
}

void UGsUIPopupGuildChangeMessage::OnInputOk()
{
	FString textMsg = _inputBoxMsg->GetText().ToString();

	// TODO: 메시지 길이, 적합성 검사?

	switch (_msgType)
	{
	case EGsGuildMsgType::INTRODUCTION:
	{
		FGsNetSendServiceGuild::SendReqChangeIntroduction(textMsg);
	}
	break;
	case EGsGuildMsgType::NOTICE:
	{
		FGsNetSendServiceGuild::SendReqChangeNotice(textMsg);
	}
	break;
	}

	Close();
}

void UGsUIPopupGuildChangeMessage::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildChangeMessage::SetMessageType(EGsGuildMsgType InType)
{
	_msgType = InType;
	
	FText hintText;

	switch (InType)
	{
	case EGsGuildMsgType::INTRODUCTION:
	{
		_maxMsgCount = MAX_GUILD_DESC_LEN;

		// TEXT: 소개글을 입력해 주세요.\n 한글 {0}자, 영문 {1}자 이내로 가능합니다.
		FText textFormat;
		FText::FindText(TEXT("GuildText"), TEXT("UI_IntroductionChange_Text"), textFormat);
		hintText = FText::Format(textFormat, { _maxMsgCount / 2, _maxMsgCount });
	}
	break;
	case EGsGuildMsgType::NOTICE:
	{
		_maxMsgCount = MAX_GUILD_NOTICE_LEN;

		// TEXT: 공지사항을 입력해 주세요.\n 한글 {0}자, 영문 {1}자 이내로 가능합니다.
		FText textFormat;
		FText::FindText(TEXT("GuildText"), TEXT("UI_NoticeChange_Text"), textFormat);
		hintText = FText::Format(textFormat, { _maxMsgCount / 2, _maxMsgCount });
	}
	break;
	}

	_inputBoxMsg->SetHintText(hintText);
}

void UGsUIPopupGuildChangeMessage::OnClickOk()
{
	OnInputOk();
}

void UGsUIPopupGuildChangeMessage::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildChangeMessage::OnTextChangedMsg(const FText& InText)
{
	FString strInText = InText.ToString();
	FString strText;

	const TCHAR* LineEndingR = TEXT("\r");
	const TCHAR* LineEndingN = TEXT("\n");

	if (EGsGuildMsgType::INTRODUCTION == _msgType)
	{
		FString strTemp = strInText.Replace(LineEndingR, TEXT(""));
		strText = strTemp.Replace(LineEndingN, TEXT(""));
	}
	else
	{
		const int maxLineCount = 10;
		int countR = 0;
		int countN = 0;
		FString strTemp;

		for (int i = 0; i < strInText.Len(); ++i)
		{
			FString strChar;
			strChar.AppendChar(strInText[i]);

			if (strChar.Equals(LineEndingR))
			{
				countR += 1;
				if (maxLineCount > countR)
				{
					strTemp.Append(strChar);
				}
			}
			else if (strChar.Equals(LineEndingN))
			{
				countN += 1;
				if (maxLineCount > countN)
				{
					strTemp.Append(strChar);
				}
			}
			else
			{
				strTemp.Append(strChar);
			}
		}

		strText = strTemp;
	}
	
	int32 length = FGsTextUtil::GetStringLengthRegex(strText);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (_maxMsgCount < length)
	{
		strText.RemoveAt(strText.Len() - 1);
	}

	// 텍스트가 달라졌을 경우에만 Set 한다
	if (false == strInText.Equals(strText))
	{
		_inputBoxMsg->SetText(FText::FromString(strText));
	}
}

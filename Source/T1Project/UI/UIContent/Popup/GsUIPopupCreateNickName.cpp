// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCreateNickName.h"
#include "EditableTextBox.h"
#include "UTIL/GsText.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Management/ScopeGlobal/GsNetManager.h"


void UGsUIPopupCreateNickName::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_editNickName->OnTextChanged.AddDynamic(this, &UGsUIPopupCreateNickName::OnTextChangedNickName);
}

void UGsUIPopupCreateNickName::NativeConstruct()
{
	// 창 재사용 시 이전 입력내용이 남아있지 않게 하기 위함.
	_editNickName->SetText(FText::GetEmpty());

	Super::NativeConstruct();
}

void UGsUIPopupCreateNickName::NativeDestruct()
{
	_okCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIPopupCreateNickName::OnInputOk()
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
				FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("CreateNickName")));
				thiz->CallOkCallback();
				/**
				 * https://jira.com2us.com/jira/browse/C2URWQ-3626
				 * 2023/4/4 PKT - 이슈 관련해서 정상적으로 케릭이 생성 된 후 이름 생성 창을 닫도록 한다.
				 */
				//thiz->Close();
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

void UGsUIPopupCreateNickName::OnInputCancel()
{
	Close();
}

void UGsUIPopupCreateNickName::SetOkCallback(TFunction<void(const FString&)> InOkCallback)
{
	FString advReservationUserName = GNet()->GetAdvReservationUserName();
	if (!advReservationUserName.IsEmpty())
	{
		_editNickName->SetText(FText::FromString(advReservationUserName));
		advReservationUserName = TEXT("");
		GNet()->SetAdvReservationUserName(advReservationUserName);
	}

	_okCallback = InOkCallback;
}

void UGsUIPopupCreateNickName::CallOkCallback()
{
	if (_okCallback)
	{
		_okCallback(_editNickName->GetText().ToString());
	}
}

void UGsUIPopupCreateNickName::OnTextChangedNickName(const FText& InText)
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

void UGsUIPopupCreateNickName::OnClickOK()
{	
	OnInputOk();	
}

void UGsUIPopupCreateNickName::OnClickCancel()
{
	OnInputCancel();
}

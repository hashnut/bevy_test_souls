// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupChangeNameMigration.h"
#include "UTIL/GsText.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/EditableTextBox.h"

void UGsUIPopupChangeNameMigration::BeginDestroy()
{
	if (_btnOk)
	{
		_btnOk->OnClicked.RemoveDynamic(this, &UGsUIPopupChangeNameMigration::OnClickOK);
	}
	if (_btnCancel)
	{
		_btnCancel->OnClicked.RemoveDynamic(this, &UGsUIPopupChangeNameMigration::OnClickCancel);
	}
	if (_editName)
	{
		_editName->OnTextChanged.RemoveDynamic(this, &UGsUIPopupChangeNameMigration::OnTextChangedName);
	}

	Super::BeginDestroy();
}

void UGsUIPopupChangeNameMigration::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UGsUIPopupChangeNameMigration::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupChangeNameMigration::OnClickOK);
	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupChangeNameMigration::OnClickCancel);

	_editName->OnTextChanged.AddDynamic(this, &UGsUIPopupChangeNameMigration::OnTextChangedName);
}

void UGsUIPopupChangeNameMigration::NativeConstruct()
{
	Super::NativeConstruct();

	_btnOk->SetIsEnabled(true);
	_endCheckTime = 0.f;
	_editName->SetText(FText::GetEmpty());
}

void UGsUIPopupChangeNameMigration::NativeDestruct()
{
	OnClosedPopup.ExecuteIfBound();

	Super::NativeDestruct();

	OnClosedPopup.Unbind();
	OnChangedName.Unbind();
}

void UGsUIPopupChangeNameMigration::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (_endCheckTime > 0.f && _endCheckTime <= FApp::GetCurrentTime())
	{
		_btnOk->SetIsEnabled(true);
		_endCheckTime = 0.f;
	}
}

void UGsUIPopupChangeNameMigration::OnInputOk()
{
	OnClickOK();
}

void UGsUIPopupChangeNameMigration::OnInputCancel()
{
	Close();
}

void UGsUIPopupChangeNameMigration::OnTextChangedName(const FText& InText)
{
	FString strText = InText.ToString();
	int32 length = FGsTextUtil::GetStringLengthNameType(strText);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (MAX_CHAR_NAME_LEN < length)
	{
		strText.RemoveAt(strText.Len() - 1);
		_editName->SetText(FText::FromString(strText));
	}
}

void UGsUIPopupChangeNameMigration::OnClickOK()
{
	if (_endCheckTime > 0.f) return;

	_btnOk->SetIsEnabled(false);
	_endCheckTime = FApp::GetCurrentTime() + _btnBlockTime;

	FText changedName = _editName->GetText();
	FString strName = changedName.ToString();
	int32 length = FGsTextUtil::GetStringLengthNameType(strName);

	// 지정 글자수 이상 입력하지 못하게 막음
	if (MIN_CHAR_NAME_LEN > length || MAX_CHAR_NAME_LEN < length)
	{
		// TEXT: 글자 수 조건이 맞지 않습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongTextLength"), textMsg);
		FGsUIHelper::TrayMessageTicker(textMsg);

		return;
	}

	// 숫자와 지원 언어 외의 문자가 포함되었는지 체크(공백, 특수문자, 이모티콘, 미지원언어)
	if (false == FGsTextUtil::IsValidNameType(strName))
	{
		// TEXT: 지원하지 않는 문자가 포함 되어있습니다.
		FText textMsg;
		FText::FindText(TEXT("GuildText"), TEXT("Alert_WrongLanguage"), textMsg);
		FGsUIHelper::TrayMessageTicker(textMsg);

		return;
	}

	auto thiz = MakeWeakObjectPtr(this);
	
	// 금칙어 체크
	FGsTextUtil::IsBanWord(strName, BannedWordCheckUsage::name, [thiz, changedName](bool success, FString inText, FString outText) {
			if (success)
			{
				if (thiz.IsValid())
				{
					thiz->OnChangedName.ExecuteIfBound(changedName);
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

void UGsUIPopupChangeNameMigration::OnClickCancel()
{
	Close();
}

void UGsUIPopupChangeNameMigration::SetInit(InitParam* param /*= nullptr*/)
{
	if (param)
	{
		if (param->titleText.IsEmpty() == false)	
			_textTitle = param->titleText;

		if (param->descText.IsEmpty() == false)		
			_textDesc = param->descText;

		if (param->hintText.IsEmpty() == false)
			_textHint = param->hintText;
	}
}

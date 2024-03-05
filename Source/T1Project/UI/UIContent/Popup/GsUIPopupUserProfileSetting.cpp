// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupUserProfileSetting.h"
#include "Components/MultiLineEditableText.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Net/GsNetSendService.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UTIL/GsText.h"


void UGsUIPopupUserProfileSetting::BeginDestroy()
{
	if (_btnCancel)
	{
		_btnCancel->OnClicked.RemoveDynamic(this, &UGsUIPopupUserProfileSetting::OnClickCancel);
	}
	if (_btnOk)
	{
		_btnOk->OnClicked.RemoveDynamic(this, &UGsUIPopupUserProfileSetting::OnClickOk);
	}
	if (_inputBoxIntroduction)
	{
		_inputBoxIntroduction->OnTextChanged.RemoveDynamic(this, &UGsUIPopupUserProfileSetting::OnTextChangedIntroduction);
	}

	Super::BeginDestroy();
}

void UGsUIPopupUserProfileSetting::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupUserProfileSetting::OnClickCancel);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupUserProfileSetting::OnClickOk);

	_inputBoxIntroduction->OnTextChanged.AddDynamic(this, &UGsUIPopupUserProfileSetting::OnTextChangedIntroduction);
}

void UGsUIPopupUserProfileSetting::NativeConstruct()
{
	Super::NativeConstruct();

	// 자기소개 영역
	FText formatText;
	FText::FindText(TEXT("SocialText"), TEXT("introduction_inputGuide"), formatText);
	FText hintText = FText::Format(formatText, { MAX_PROFILE_INTRO_LEN / 2, MAX_PROFILE_INTRO_LEN });
	// 자기소개를 입력해주세요. 한글 {0}자, 영문 {1}자 이내로 가능합니다.
	_inputBoxIntroduction->SetHintText(hintText);
	_inputBoxIntroduction->SetText(FText::FromString(GSUserProfile()->GetIntroductionText()));
}

void UGsUIPopupUserProfileSetting::OnClickOk()
{
	FString introductionText = _inputBoxIntroduction->GetText().ToString();
	GLocalization()->IsBannedWordInclude(introductionText, TEXT("all"), [this](bool success, FString inText, FString outText) 
		{
			if (success)
			{
				if (inText != GSUserProfile()->GetIntroductionText())
				{
					FGsNetSendService::SendReqUserProfileIntroductionUpdate(inText);
				}

				Close();
			}
			else
			{
				FText tickerText;
				FText::FindText(TEXT("SocialText"), TEXT("ticker_introduction_badWord"), tickerText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, tickerText);
			}
		});
}

void UGsUIPopupUserProfileSetting::OnClickCancel()
{
	Close();
}

void UGsUIPopupUserProfileSetting::OnTextChangedIntroduction(const FText& InText)
{
	FString changedString;
	bool isTextChanged = FGsUIStringHelper::GetFormatedEditableText(InText.ToString(), changedString, MAX_PROFILE_INTRO_LEN, false, false, false);
	if (isTextChanged)
	{
		_inputBoxIntroduction->SetText(FText::FromString(changedString));
	}
}

void UGsUIPopupUserProfileSetting::OnInputCancel()
{
	OnClickCancel();
}
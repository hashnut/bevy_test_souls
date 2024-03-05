// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupCommentWrite.h"
#include "Components/MultiLineEditableText.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Net/GsNetSendService.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsText.h"


void UGsUIPopupCommentWrite::BeginDestroy()
{
	if (_btnCancel)
	{
		_btnCancel->OnClicked.RemoveDynamic(this, &UGsUIPopupCommentWrite::OnClickCancel);
	}
	if (_btnOk)
	{
		_btnOk->OnClicked.RemoveDynamic(this, &UGsUIPopupCommentWrite::OnClickOk);
	}
	if (_inputBoxComment)
	{
		_inputBoxComment->OnTextChanged.RemoveDynamic(this, &UGsUIPopupCommentWrite::OnTextChangedComment);
	}

	Super::BeginDestroy();
}

void UGsUIPopupCommentWrite::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupCommentWrite::OnClickCancel);
	_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupCommentWrite::OnClickOk);

	_inputBoxComment->OnTextChanged.AddDynamic(this, &UGsUIPopupCommentWrite::OnTextChangedComment);
}

void UGsUIPopupCommentWrite::NativeConstruct()
{
	Super::NativeConstruct();

	FText formatText;
	FText::FindText(TEXT("SocialText"), TEXT("comment_inputGuide"), formatText);
	FText hintText = FText::Format(formatText, { MAX_PROFILE_COMMENT_LEN / 2, MAX_PROFILE_COMMENT_LEN });
	// 한마디를 입력해주세요. 한글 {0}자, 영문 {1}자 이내로 가능합니다.
	_inputBoxComment->SetHintText(hintText);
	_inputBoxComment->SetText(FText::GetEmpty());
}

void UGsUIPopupCommentWrite::OnClickOk()
{
	FString commentText = _inputBoxComment->GetText().ToString();
	GLocalization()->IsBannedWordInclude(commentText, TEXT("all"), [this, commentText](bool success, FString inText, FString outText) 
		{
			if (success)
			{
				FGsNetSendService::SendReqUserProfileCommentInput(GSUserProfile()->GetTargetUserDBID(), commentText);
				Close();
			}
			else
			{
				if (GLocalization()->GetBannedWordProcessErrorCode() == -1)
				{
					return;
				}

				FText tickerText;
				FText::FindText(TEXT("SocialText"), TEXT("ticker_comment_badWord"), tickerText);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, tickerText);
			}
		}); //return 값은 현재 handler 생성 유무
}

void UGsUIPopupCommentWrite::OnClickCancel()
{
	Close();
}

void UGsUIPopupCommentWrite::OnTextChangedComment(const FText& InText)
{
	FString strText = InText.ToString();
	bool enterExisted = INDEX_NONE != strText.Find(TEXT("\n")) ||
						INDEX_NONE != strText.Find(TEXT("\r"));
	if (enterExisted)
	{
		strText = strText.Replace(TEXT("\n"), TEXT(""));
		strText = strText.Replace(TEXT("\r"), TEXT(""));
	}

	int32 length = FGsTextUtil::GetStringLengthRegex(strText);
	bool textRemoved = false;
	// 지정 글자수 이상 입력하지 못하게 막음
	if (MAX_PROFILE_COMMENT_LEN < length)
	{
		strText.RemoveAt(strText.Len() - 1);
		textRemoved = true;
	}

	if (enterExisted || textRemoved)
	{
		_inputBoxComment->SetText(FText::FromString(strText));
	}
}

void UGsUIPopupCommentWrite::OnInputCancel()
{
	OnClickCancel();
}
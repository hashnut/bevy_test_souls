// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIUserProfileCommentListEntry.h"
#include "Components/TextBlock.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGame/GsUserProfileManager.h"
#include "Net/GsNetSendService.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsTimeSyncUtil.h"


void UGsUIUserProfileCommentListEntry::BeginDestroy()
{
	if (_btnDelete)
	{
		_btnDelete->OnClicked.RemoveDynamic(this, &UGsUIUserProfileCommentListEntry::OnClickDelete);
	}
	if (_btnUserName)
	{
		_btnUserName->OnClicked.RemoveDynamic(this, &UGsUIUserProfileCommentListEntry::OnClickUserName);
	}

	Super::BeginDestroy();
}

void UGsUIUserProfileCommentListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnDelete->OnClicked.AddDynamic(this, &UGsUIUserProfileCommentListEntry::OnClickDelete);
	_btnUserName->OnClicked.AddDynamic(this, &UGsUIUserProfileCommentListEntry::OnClickUserName);
}

void UGsUIUserProfileCommentListEntry::SetData(const UserComment& InUserComment)
{
	_userDBId = InUserComment._userDBId;
	_textBlockUserName->SetText(FText::FromString(InUserComment._userName));

	FString inCommentText = InUserComment._commentText;
	_textBlockComment->SetText(FText::GetEmpty());
	auto thiz = MakeWeakObjectPtr(this);
	GLocalization()->ChangeBannedSentence(inCommentText, [thiz](bool success, FString inText, FString outText) {

		if(thiz.IsValid())
		{
			thiz->_textBlockComment->SetText(FText::FromString(outText));
		}
	});

	SetTimeText(InUserComment._writeTime);

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	if (GSUserProfile()->IsMyProfile() || _userDBId == userData->mUserDBId)
	{
		_btnDelete->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		_btnDelete->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIUserProfileCommentListEntry::SetTimeText(time_t InTime)
{
	FText textTimes;
	FText::FindText(TEXT("ChatText"), TEXT("ChatTime1"), textTimes);

	FDateTime writeTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(InTime);
	FTimespan passTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC() - writeTime;

	int32 year = 0;
	int32 month = 0;
	int32 day = 0;
	writeTime.GetDate(year, month, day);

	int32 days = passTime.GetTotalDays();
	int32 hours = 0;
	int32 minutes = 0;

	if (days > 0)
	{
		FString stringTimes = FString::Printf(TEXT("%4d/%02d/%02d"), year, month, day);
		textTimes = FText::FromString(stringTimes);
	}
	else
	{
		hours = static_cast<int32>(passTime.GetTotalHours());
		if (hours > 0)
		{
			if (FText::FindText(TEXT("ChatText"), TEXT("ChatTime3"), textTimes))
			{
				textTimes = FText::Format(textTimes, hours);
			}
		}
		else
		{
			if ((minutes = static_cast<int32>(passTime.GetTotalMinutes())) > 0)
			{
				if (FText::FindText(TEXT("ChatText"), TEXT("ChatTime2"), textTimes))
				{
					textTimes = FText::Format(textTimes, minutes);
				}
			}
		}
	}
	_textBlockWriteTime->SetText(textTimes);
}

void UGsUIUserProfileCommentListEntry::OnClickDelete()
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;
	
	FText formatText;
	if (GSUserProfile()->IsMyProfile())
	{
		FText::FindText(TEXT("SocialText"), TEXT("comment_deletePopup_MyProfile"), formatText);
	}
	else
	{
		FText::FindText(TEXT("SocialText"), TEXT("comment_deletePopup_OthersProfile"), formatText);
	}

	FString userName = _textBlockUserName->GetText().ToString();
	FText deleteText = FText::Format(formatText, FText::FromString(userName));

	UserDBId userDBId = _userDBId;
	FGsUIHelper::PopupYesNo(deleteText, [userDBId](bool bIsOk)
		{
			if (bIsOk)
			{
				GSUserProfile()->DeleteComment(userDBId);
			}
		});
}

void UGsUIUserProfileCommentListEntry::OnClickUserName()
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	if (userData->mUserDBId != _userDBId)
	{
		FGsNetSendService::SendReqUserProfile(_userDBId);
	}
}
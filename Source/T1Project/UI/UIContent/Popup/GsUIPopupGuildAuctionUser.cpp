// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildAuctionUser.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Popup/Guild/GuildAuction/GsUIGuildAuctionUserEntry.h"

#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Net/GsNetSendServiceGuild.h"


void UGsUIPopupGuildAuctionUser::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_entryWidgetClass, _scrollBoxUser);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupGuildAuctionUser::OnRefreshEntryUser);
	}

	if (_btnOk)
	{
		_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuctionUser::OnClickOk);
	}
}

void UGsUIPopupGuildAuctionUser::NativeDestruct()
{
	_userNameList.Empty();
	_divisionItemDBId = INVALID_ITEM_DB_ID;

	Super::NativeDestruct();
}

void UGsUIPopupGuildAuctionUser::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildAuctionUser::OnInputOk()
{
	Close();
}

void UGsUIPopupGuildAuctionUser::OnClickOk()
{
	Close();
}

void UGsUIPopupGuildAuctionUser::OnRefreshEntryUser(int32 InIndex, class UWidget* InEntry)
{
	if (false == _userNameList.IsValidIndex(InIndex))
		return;

	UGsUIGuildAuctionUserEntry* entry = Cast<UGsUIGuildAuctionUserEntry>(InEntry);
	if (nullptr == entry)
		return;

	entry->SetData(_userNameList[InIndex]);
}

void UGsUIPopupGuildAuctionUser::SetData(ItemDBId InDivisionItemDBId)
{
	_divisionItemDBId = InDivisionItemDBId;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(_divisionItemDBId);
	if (nullptr == divisionItem)
	{
		divisionItem = auctionData->GetCalculateItem(_divisionItemDBId);
		if (nullptr == divisionItem)
		{
			Close();
			return;
		}
	}

	// 참여자({0})
	FText findText;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Participant_PopUp_Participant"), findText);
	FText formatText = FText::Format(findText, divisionItem->GetParticipantListCount());
	_textBlockTitle->SetText(formatText);

	// 획득자 이름 설정
	FString acquisitionUserName = divisionItem->GetAcquisitionUserName();
	_uiMemberAcquirer->SetData(acquisitionUserName);

	if (divisionItem->GetParticipantList().Num() <= 0)
	{
		// 정보가 없을 경우 서버에 요청한다.
		FGsNetSendServiceGuild::SendReqGuildAuctionDetail(divisionItem->GetItemDBId());
	}

	InvalidateList();
}

void UGsUIPopupGuildAuctionUser::InvalidateList()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(_divisionItemDBId);
	if (nullptr == divisionItem)
	{
		divisionItem = auctionData->GetCalculateItem(_divisionItemDBId);
		if (nullptr == divisionItem)
		{
			return;
		}
	}

	const TArray<UserDBId>& userDBIdList = divisionItem->GetParticipantList();
	// 참여자 목록 표시 : 캐릭터명 오름차순 정렬된 정보
	_userNameList.Empty();
	for (const UserDBId& userDBId : userDBIdList)
	{
		_userNameList.Emplace(auctionData->GetUserName(userDBId));
	}

	_userNameList.Sort([](const FString& lhs, const FString& rhs)
		{
			return lhs.Compare(rhs) < 0;
		});

	_scrollBoxHelper->RefreshAll(_userNameList.Num());
}

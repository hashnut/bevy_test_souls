// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildAuctionResult.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Popup/Guild/GuildAuction/GsUIGuildAuctionResultEntry.h"

#include "Guild/GsGuildHelper.h"

#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Net/GsNetSendServiceGuild.h"


void UGsUIPopupGuildAuctionResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_entryWidgetClass, _scrollBoxUser);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupGuildAuctionResult::OnRefreshEntryUser);
	}

	if (_btnClose)
	{
		_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuctionResult::OnClickClose);
	}
}

void UGsUIPopupGuildAuctionResult::NativeConstruct()
{
	Super::NativeConstruct();

	_itemDBId = INVALID_ITEM_DB_ID;
	_auctionId = INVALID_GUILD_AUCTION_ID;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	/* 경매 수수료 설정 */
	FText findText;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Charge_PopUp_Charge_Description"), findText);
	FNumberFormattingOptions option;
	option.SetMaximumFractionalDigits(2);
	option.SetMinimumFractionalDigits(0);

	float commisionRate = (float)auctionData->GetCommissionRate() / 100.0f;
	FText textCommission = FText::Format(findText, FText::AsNumber(commisionRate, &option));
	_textBlockCommission->SetText(textCommission);
}

void UGsUIPopupGuildAuctionResult::OnInputOk()
{
	Close();
}

void UGsUIPopupGuildAuctionResult::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildAuctionResult::OnClickClose()
{
	OnInputCancel();
}

void UGsUIPopupGuildAuctionResult::OnRefreshEntryUser(int32 InIndex, class UWidget* InEntry)
{
	if (false == _userInfoList.IsValidIndex(InIndex))
		return;

	UGsUIGuildAuctionResultEntry* entry = Cast<UGsUIGuildAuctionResultEntry>(InEntry);
	if (nullptr == entry)
		return;

	FText gradeNameText;
	FGsGuildHelper::GetMemberGradeText(_userInfoList[InIndex].Key, gradeNameText);
	FText userNameText = FText::FromString(_userInfoList[InIndex].Value);
	entry->SetData(gradeNameText, userNameText);
}

void UGsUIPopupGuildAuctionResult::SetDivision(const FGsGuildDivisionItem* InDivisionItem)
{
	_openType = EGsAuctionResultOpenType::DIVISION;
	_itemDBId = InDivisionItem->GetItemDBId();

	if (nullptr != InDivisionItem && InDivisionItem->GetResultList().Num() <= 0)
	{
		// 정보가 없을 경우 서버에 요청한다.
		FGsNetSendServiceGuild::SendReqGuildAuctionDetail(_itemDBId);
	}

	InvalidateList();
}

void UGsUIPopupGuildAuctionResult::SetAuction(const FGsGuildAuctionItem* InAuctionItem)
{
	_openType = EGsAuctionResultOpenType::AUCTION;
	
	if (nullptr != InAuctionItem)
	{
		// 경매는 정보가 있어도 재요청한다.
		_auctionId = InAuctionItem->GetAuctionId();
		FGsNetSendServiceGuild::SendReqGuildAuctionDetail(InAuctionItem->GetItemData().mItemDBId);
	}

	InvalidateList();
}

void UGsUIPopupGuildAuctionResult::SetCalculate(const FGsGuildDivisionItem* InCalculateItem)
{
	_openType = EGsAuctionResultOpenType::CALCULATE;
	_itemDBId = InCalculateItem->GetItemDBId();

	if (nullptr != InCalculateItem && InCalculateItem->GetResultList().Num() <= 0)
	{
		// 정보가 없을 경우 서버에 요청한다.
		FGsNetSendServiceGuild::SendReqGuildAuctionDetail(_itemDBId);
	}

	InvalidateList();
}

void UGsUIPopupGuildAuctionResult::InvalidateList()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const TArray<TPair<GuildMemberGradeType, UserDBId>>* userInfoList = nullptr;
	Currency resultPrice = 0;

	switch (_openType)
	{
	case UGsUIPopupGuildAuctionResult::EGsAuctionResultOpenType::DIVISION:
	{
		if (INVALID_ITEM_DB_ID == _itemDBId)
		{
			_scrollBoxHelper->RefreshAll(0);
			_textBlockResult->SetText(FText::AsNumber(0));
			return;
		}

		if (const FGsGuildDivisionItem* item = auctionData->GetDivisionItem(_itemDBId))
		{
			userInfoList = &item->GetResultList();
			resultPrice = item->GetDistributedPrice();
		}
		break;
	}
	case UGsUIPopupGuildAuctionResult::EGsAuctionResultOpenType::AUCTION:
	{
		if (INVALID_GUILD_AUCTION_ID == _auctionId)
		{
			_scrollBoxHelper->RefreshAll(0);
			_textBlockResult->SetText(FText::AsNumber(0));
			return;
		}

		if (const FGsGuildAuctionItem* item = auctionData->GetAuctionItem(_auctionId))
		{
			userInfoList = &item->GetResultList();
			resultPrice = item->GetExpectedResultPrice();
		}
		break;
	}
	case UGsUIPopupGuildAuctionResult::EGsAuctionResultOpenType::CALCULATE:
	{
		if (INVALID_ITEM_DB_ID == _itemDBId)
		{
			_scrollBoxHelper->RefreshAll(0);
			_textBlockResult->SetText(FText::AsNumber(0));
			return;
		}

		if (const FGsGuildDivisionItem* item = auctionData->GetCalculateItem(_itemDBId))
		{
			userInfoList = &item->GetResultList();
			resultPrice = item->GetDistributedPrice();
		}
		break;
	}
	default:
		break;
	}

	if (nullptr == userInfoList)
	{
		_scrollBoxHelper->RefreshAll(0);
		_textBlockResult->SetText(FText::AsNumber(0));
		return;
	}

	TMap<GuildMemberGradeType, TArray<FString>> userInfoListMap;
	// 유저 정보 설정
	for (const TPair<GuildMemberGradeType, UserDBId>& userInfo : *userInfoList)
	{
		if (userInfoListMap.Contains(userInfo.Key))
		{
			userInfoListMap[userInfo.Key].Emplace(auctionData->GetUserName(userInfo.Value));
		}
		else
		{
			TArray<FString> userNameList{auctionData->GetUserName(userInfo.Value)};
			userInfoListMap.Emplace(userInfo.Key, userNameList);
		}
	}

	// 유저 정보 정렬 : 이름 오름차순
	for (TPair<GuildMemberGradeType, TArray<FString>>& userInfoListData : userInfoListMap)
	{
		TArray<FString>& userNameList = userInfoListData.Value;
		userNameList.Sort();
	}

	_userInfoList.Empty();
	for (uint8 grade = (uint8)GuildMemberGradeType::CAPTAIN; grade > (uint8)GuildMemberGradeType::UNKNOWN; --grade)
	{
		if (userInfoListMap.Contains((GuildMemberGradeType)grade))
		{
			for (const FString& userName : userInfoListMap[(GuildMemberGradeType)grade])
			{
				_userInfoList.Emplace((GuildMemberGradeType)grade, userName);
			}
		}
	}

	_scrollBoxHelper->RefreshAll(_userInfoList.Num());

	// 개별 정산금 설정
	resultPrice = _userInfoList.Num() > 0 ? resultPrice : 0;
	_textBlockResult->SetText(FText::AsNumber(resultPrice));
}

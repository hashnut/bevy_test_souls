// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildAuctionBegin.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

#include "Guild/GsGuildHelper.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Net/GsNetSendServiceGuild.h"

#include "Item/GsItemManager.h"


void UGsUIPopupGuildAuctionBegin::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_scrollBoxHelper->Initialize(_iconSelector);
		_scrollBoxHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPopupGuildAuctionBegin::OnRefreshEntryItem);
	}

	if (_btnOk)
	{
		_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuctionBegin::OnClickOk);
	}

	if (_btnCancel)
	{
		_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuctionBegin::OnClickCancel);
	}
}

void UGsUIPopupGuildAuctionBegin::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildAuctionBegin::OnClickOk()
{
	// 1. 아이템이 이미 경매에 등록된 경우 - 서버 & 클라 체크
	// 2. 기사단 창고 권한을 박탈당한 경우 - 서버 & 클라 체크
	// 3. 경매 동시 진행 최대치가 초과된 경우 - 서버 체크 

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	bool canStartDivision = false;
	do
	{
		uint32 myAuthority = guildManager->GetAuthority();
		if (false == FGsGuildHelper::CanDivision(myAuthority))
		{
			// 기사단 분배 권한이 없습니다.
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Authority_Notify"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			break;
		}

		if (0 >= _itemDBIdList.Num())
		{
			// 선택된 획득 내역이 없어 분배를 시작할 수 없습니다.
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_NoCheck_Notify"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			break;
		}

		// 경매 동시 진행 최대치가 초과된 경우 - 서버 체크가 되었는지 테스트 필요하다.

		canStartDivision = true;
	} while (0);

	if (false == canStartDivision)
	{
		Close();
		return;
	}

	TArray<ItemDBId> itemDBIdList;
	TArray<UserDBId> emptyReceiverDBIdList;

	for (ItemDBId divisionItemDBId : _itemDBIdList)
	{
		const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(divisionItemDBId);
		if (nullptr == divisionItem)
		{
			// 경매 가능한 아이템이 없습니다. - 이미 경매에 등록된 경우
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Empty_Notify"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			Close();
			return;
		}

		const ItemDataEquip& itemData = divisionItem->GetItemData();

		if (divisionItem->GetDivisionState() != GuildDivisionState::STORAGE)
		{
			// 경매 가능한 아이템이 없습니다. - 이미 경매에 등록된 경우
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Empty_Notify"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			Close();
			return;
		}

		itemDBIdList.Emplace(divisionItemDBId);
	}

	if (0 >= itemDBIdList.Num())
	{
		// 선택된 획득 내역이 없어 분배를 시작할 수 없습니다.
		FText errorText;
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_NoCheck_Notify"), errorText);
		FGsUIHelper::TrayMessageTicker(errorText);
		Close();
		return;
	}

	TArray<Currency> currencyList;
	FGsNetSendServiceGuild::SendReqGuildAuctionRegister(itemDBIdList, _auctionType, _resultType, emptyReceiverDBIdList, CurrencyType::DIA, currencyList);
	Close();
}

void UGsUIPopupGuildAuctionBegin::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildAuctionBegin::OnRefreshEntryItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	if (false == _itemDBIdList.IsValidIndex(InIndex))
		return;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(_itemDBIdList[InIndex]);
	if (nullptr == divisionItem)
		return;

	// 롱프레스 이벤트 등록
	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupGuildAuctionBegin::OnLongPressIcon);
	}

	ItemId itemId = divisionItem->GetItemData().mTemplateId;
	ItemDBId itemDBId = divisionItem->GetItemData().mItemDBId;
	ItemAmount itemAmount = divisionItem->GetItemData().mItemAmount;
	Level itemLevel = divisionItem->GetItemData().mLevel;
	int32 enchantShield = divisionItem->GetItemData().mEnchantShield;
	int32 enchantBonusIndex = divisionItem->GetItemData().mEnchantBonusIndex;

	if (ItemType::EQUIP == itemManager->GetItemTypeByTID(itemId))
	{
		itemIcon->SetNotOwnItem(itemId, itemAmount, itemLevel, enchantShield, enchantBonusIndex);
	}
	else
	{
		itemIcon->SetNotOwnItem(itemId, itemAmount, itemLevel);
	}

	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetDimmedImage(false);
	itemIcon->SetSlotSelectionImage(false);
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetItemNameToolTipHoverMode(true);
	itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
}

void UGsUIPopupGuildAuctionBegin::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	FGsItem* item = itemIcon->GetItemIconData();
	if (nullptr == item)
		return;

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = true;
	option._bHideBG = false;
	option._bHideButtonsBottom = true;
	option._bUseLock = false;
	option._bUseCollectionMarking = false;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), FGsItemDetailOpenMessageParamEX::PosDir::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void UGsUIPopupGuildAuctionBegin::SetData(TArray<ItemDBId> InItemList, GuildAuctionType InAuctionType, GuildAuctionResultType InResultType)
{
	_auctionType = InAuctionType;
	_resultType = InResultType;	

	_itemDBIdList.Empty();
	for (ItemDBId divisionItemDBid : InItemList)
	{
		_itemDBIdList.Emplace(divisionItemDBid);
	}

	// 가장 오래된 아이템 ~ 최신 아이템 정렬
	_itemDBIdList.Sort([](ItemDBId lhs, ItemDBId rhs)
		{
			return lhs < rhs;
		});

	FText biddingTypeText;

	if (InAuctionType == GuildAuctionType::NORMAL)
	{
		// 일반 경매
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Division_Type_Common"), biddingTypeText);
	}
	else if (InAuctionType == GuildAuctionType::PARTICIPANTS)
	{
		// 참여자 경매
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Division_Type_Acquisition"), biddingTypeText);
	}

	FText findText;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_PopUp_Description"), findText);
	FText formatText = FText::Format(findText, FText::AsNumber(_itemDBIdList.Num()), biddingTypeText);

	_textBlockStart->SetText(formatText);

	_scrollBoxHelper->RefreshAll(_itemDBIdList.Num());
}

void UGsUIPopupGuildAuctionBegin::CheckItemValid()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	bool completeFind = true;
	time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
	const TArray<FGsGuildDivisionItem>& divisionDataList = auctionData->GetDivisionList();
	for (ItemDBId itemDBId : _itemDBIdList)
	{
		bool isExist = false;
		for (int32 i = 0; i < divisionDataList.Num(); ++i)
		{
			// 분배가 되지 않은 보관중인 아이템만 표시한다.
			if (divisionDataList[i].GetDivisionState() != GuildDivisionState::STORAGE)
				continue;

			if (divisionDataList[i].GetExpiredDateTime() < currentTime)
				continue;

			if (divisionDataList[i].GetItemDBId() == itemDBId)
			{
				isExist = true;
				break;
			}
		}

		if (false == isExist)
		{
			completeFind = false;
			break;
		}
	}

	if (false == completeFind)
	{
		Close();
	}
}

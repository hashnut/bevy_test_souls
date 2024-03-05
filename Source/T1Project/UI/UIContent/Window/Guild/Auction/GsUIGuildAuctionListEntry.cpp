// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildAuctionListEntry.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"

#include "Currency/GsCurrencyHelper.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionResult.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionBidding.h"
#include "UI/UIContent/Window/Guild/Auction/GsGuildAuctionListItem.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"



void UGsUIGuildAuctionListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnResult)
	{
		_btnResult->OnClicked.AddDynamic(this, &UGsUIGuildAuctionListEntry::OnClickResult);
	}
	if (_btnBiddingNormalOn)
	{
		_btnBiddingNormalOn->OnClicked.AddDynamic(this, &UGsUIGuildAuctionListEntry::OnClickBidding);
	}
	if (_btnBiddingNormalOff)
	{
		_btnBiddingNormalOff->OnClicked.AddDynamic(this, &UGsUIGuildAuctionListEntry::OnClickBiddingOff);
	}
	if (_btnBiddingAppointmentOn)
	{
		_btnBiddingAppointmentOn->OnClicked.AddDynamic(this, &UGsUIGuildAuctionListEntry::OnClickBidding);
	}
	if (_btnBiddingAppointmentOff)
	{
		_btnBiddingAppointmentOff->OnClicked.AddDynamic(this, &UGsUIGuildAuctionListEntry::OnClickBiddingOff);
	}
}

void UGsUIGuildAuctionListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	// TEXT: {0} 남음
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Status_Nhours"), _remainTimeText);

	_colorNormal = FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_AUCTION_NORMAL);
	_colorAppointment = FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_AUCTION_APPOINTMENT);
}

void UGsUIGuildAuctionListEntry::NativeDestruct()
{
	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
		_uiIcon = nullptr;
	}

	Super::NativeDestruct();
}

void UGsUIGuildAuctionListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UGsGuildAuctionListItem* listItem = Cast<UGsGuildAuctionListItem>(ListItemObject);
	if (nullptr == listItem)
		return;

	_auctionId = listItem->_auctionId;

	InvalidateAll();
}

void UGsUIGuildAuctionListEntry::SetIsSelected(bool bInIsSelected)
{
	_imgSelected->SetVisibility((bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIGuildAuctionListEntry::OnClickResult()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildAuctionItem* auctionItem = auctionData->GetAuctionItem(_auctionId);
	if (nullptr == auctionItem)
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildAuctionCalculate"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildAuctionResult* popup = Cast<UGsUIPopupGuildAuctionResult>(widget.Get()))
		{
			popup->SetAuction(auctionItem);
		}
	}
}

void UGsUIGuildAuctionListEntry::OnClickBidding()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildAuctionItem* auctionItem = auctionData->GetAuctionItem(_auctionId);
	if (nullptr == auctionItem)
		return;

	// 입찰 가능 여부 1차 판단 후 팝업 노출
	Currency bidPrice = auctionData->GetNumpadBidPrice();
	if (auctionItem->CheckBid(bidPrice))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildBidding"));
		if (widget.IsValid())
		{
			if (UGsUIPopupGuildAuctionBidding* popup = Cast<UGsUIPopupGuildAuctionBidding>(widget.Get()))
			{
				popup->SetData(auctionItem, bidPrice);
			}
		}
	}
}

void UGsUIGuildAuctionListEntry::OnClickBiddingOff()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildAuctionItem* auctionItem = auctionData->GetAuctionItem(_auctionId);
	if (nullptr == auctionItem)
		return;

	Currency bidPrice = auctionData->GetNumpadBidPrice();
	auctionItem->CheckBid(bidPrice);
}

void UGsUIGuildAuctionListEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
		return;

	FGsItem* item = itemIcon->GetItemIconData();
	if (nullptr == item)
		return;

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideButtons = false;
	option._bHideBG = false;
	option._bHideButtonsBottom = true;
	option._bUseLock = false;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), FGsItemDetailOpenMessageParamEX::PosDir::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

void UGsUIGuildAuctionListEntry::InvalidateAll()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildAuctionItem* auctionItem = auctionData->GetAuctionItem(_auctionId);
	if (nullptr == auctionItem)
		return;

	/*레드닷*/
	bool isNew = auctionItem->GetAuctionDateTime() > auctionData->GetPrevCheckTimeAuction();
	_uiRedDot->SetVisibility(isNew ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	/*획득 정보*/
	FDateTime acquisitionTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(auctionItem->GetAcquisitionDateTime());
	FString strAcquisitionTime = acquisitionTime.ToString(TEXT("%Y.%m.%d"));

	FText historyText;
	auctionItem->GetRootText(historyText);
	FString strHistory = historyText.ToString();

	FString strFullHistory = FString::Printf(TEXT("%s\n%s"), *strAcquisitionTime, *strHistory);
	_textBlockDate->SetText(FText::FromString(strFullHistory));

	/*분배 타입에 대한 설정*/
	const int32 SWITCHER_INDEX_NORMAL = 0;
	const int32 SWITCHER_INDEX_APPOINTMENT = 1;
	switch (auctionItem->GetBiddingType())
	{
	case GuildAuctionType::NORMAL:
	case GuildAuctionType::PARTICIPANTS:
		_textBlockBidderName->SetColorAndOpacity(_colorNormal);
		_switcherBiddingType->SetActiveWidgetIndex(SWITCHER_INDEX_NORMAL);
		break;
	case GuildAuctionType::APPOINTMENT:
		_textBlockBidderName->SetColorAndOpacity(_colorAppointment);
		_switcherBiddingType->SetActiveWidgetIndex(SWITCHER_INDEX_APPOINTMENT);
		break;
	}

	SetUIItem(auctionItem);
	SetUIBid(auctionItem);
	SetUIState(auctionItem);
	CheckAndInvalidateRemainTime();

	SetIsSelected(auctionData->GetSelectedAuctionId() == auctionItem->GetAuctionId());
}

void UGsUIGuildAuctionListEntry::SetUIItem(const FGsGuildAuctionItem* InAuctionItem)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	if (nullptr == _uiIcon)
	{
		_uiIcon = Cast<UGsUIIconItemInventory>(_iconSelector->CreateIcon());
		_iconSelector->AddChild(_uiIcon);
	}

	if (nullptr == _uiIcon)
		return;

	if (false == _uiIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		_uiIcon->OnFinishedLongPressEvent.Clear();
		_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIGuildAuctionListEntry::OnLongPressIcon);
	}

	ItemId itemId = InAuctionItem->GetItemData().mTemplateId;
	ItemDBId itemDBId = InAuctionItem->GetItemData().mItemDBId;
	ItemAmount itemAmount = InAuctionItem->GetItemData().mItemAmount;
	Level itemLevel = InAuctionItem->GetItemData().mLevel;
	int32 enchantShield = InAuctionItem->GetItemData().mEnchantShield;
	int32 enchantBonusIndex = InAuctionItem->GetItemData().mEnchantBonusIndex;

	if (ItemType::EQUIP == itemManager->GetItemTypeByTID(itemId))
	{
		_uiIcon->SetNotOwnItem(itemId, itemAmount, itemLevel, enchantShield, enchantBonusIndex);
	}
	else
	{
		_uiIcon->SetNotOwnItem(itemId, itemAmount, itemLevel);
	}

	_uiIcon->SetActiveIconBg(true);
	_uiIcon->SetItemNameVisibility(false);
	_uiIcon->SetDimmedImage(false);
	_uiIcon->SetSlotSelectionImage(false);
	_uiIcon->SetItemNameToolTipFlag(true);
	_uiIcon->SetItemNameToolTipHoverMode(true);
	_uiIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
	_uiIcon->SetItemCollectionMarking(_uiIcon->GetItemIconData(), true);

	if (const FGsSchemaItemCommon* itemCommon = itemManager->GetItemTableDataByTID(itemId))
	{
		_textBlockItemName->SetText(itemCommon->name);
		_textBlockItemName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(itemCommon->grade));
	}
}

void UGsUIGuildAuctionListEntry::SetUIBid(const FGsGuildAuctionItem* InAuctionItem)
{
	const int32 SWITCHER_INDEX_EXIST = 0;
	const int32 SWITCHER_INDEX_NONE = 1;

	const FString& bidderName = InAuctionItem->GetBidderName();
	if (bidderName.IsEmpty())
	{
		_switcherBidder->SetActiveWidgetIndex(SWITCHER_INDEX_NONE);
	}
	else
	{
		_switcherBidder->SetActiveWidgetIndex(SWITCHER_INDEX_EXIST);
		_textBlockBidderName->SetText(FText::FromString(bidderName));
		_textBlockPrice->SetText(FText::AsNumber(InAuctionItem->GetBidPrice()));
	}
}

void UGsUIGuildAuctionListEntry::SetUIState(const FGsGuildAuctionItem* InAuctionItem)
{
	switch (InAuctionItem->GetAuctionState())
	{
	case GuildDivisionState::AUCTION: // 경매 진행중
		SetIsEnabled(true);
		break;
	case GuildDivisionState::STORAGE: // 분배 안된 상태
	case GuildDivisionState::DONE: // 경매 완료
	case GuildDivisionState::FAILED: // 유찰
		SetIsEnabled(false);
		// 목록에 보이지 않아야하지만 보일 경우 반응하지 않도록 처리한다.
		break;
	}

	const int32 CAN_BID = 0;
	const int32 CANT_BID = 1;

	_switcherBiddingNormal->SetActiveWidgetIndex(InAuctionItem->CanBid() ? CAN_BID : CANT_BID);
	_switcherBiddingAppointment->SetActiveWidgetIndex(InAuctionItem->CanBid() ? CAN_BID : CANT_BID);
}

bool UGsUIGuildAuctionListEntry::CheckAndInvalidateRemainTime()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return false;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return false;

	const FGsGuildAuctionItem* auctionItem = auctionData->GetAuctionItem(_auctionId);
	if (nullptr == auctionItem)
		return false;

	bool needRefreshList = false;

	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
	time_t expiredTimeSec = auctionItem->GetExpiredDateTime();
	time_t remainSec = expiredTimeSec - serverNowSec;

	if (remainSec <= 0)
	{
		needRefreshList = true;
		remainSec = 0;
	}

	FString strRemainTime;
	FGsTimeStringHelper::GetTimeStringFloor(FTimespan::FromSeconds(remainSec), strRemainTime);
	FText formatText = FText::Format(_remainTimeText, FText::FromString(strRemainTime));
	_textBlockRemainTime->SetText(formatText);

	return needRefreshList;
}
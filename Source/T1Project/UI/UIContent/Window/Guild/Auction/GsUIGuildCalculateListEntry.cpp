// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildCalculateListEntry.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Window/Guild/Auction/GsGuildDivisionListItem.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionUser.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionResult.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"


void UGsUIGuildCalculateListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnParticipant)
	{
		_btnParticipant->OnClicked.AddDynamic(this, &UGsUIGuildCalculateListEntry::OnClickParticipant);
	}
	if (_btnResult)
	{
		_btnResult->OnClicked.AddDynamic(this, &UGsUIGuildCalculateListEntry::OnClickResult);
	}
}

void UGsUIGuildCalculateListEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIGuildCalculateListEntry::NativeDestruct()
{
	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
		_uiIcon = nullptr;
	}

	_calculateItemDBId = INVALID_ITEM_DB_ID;

	Super::NativeDestruct();
}

void UGsUIGuildCalculateListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UGsGuildDivisionListItem* listItem = Cast<UGsGuildDivisionListItem>(ListItemObject);
	if (nullptr == listItem)
		return;

	_calculateItemDBId = listItem->_divisionItemDBId;
	
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildDivisionItem* calculateItem = auctionData->GetCalculateItem(_calculateItemDBId);
	if (nullptr == calculateItem)
		return;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	/*레드닷*/
	bool isNew = calculateItem->GetExpiredDateTime() > auctionData->GetPrevCheckTimeCalculate();
	_uiRedDot->SetVisibility(isNew ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	/*정산일*/
	FDateTime calculateTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(calculateItem->GetExpiredDateTime());
	_textBlockDate->SetText(FText::FromString(calculateTime.ToString(TEXT("%Y.%m.%d\n%H:%M:%S"))));

	/*획득 정보*/
	FDateTime acquisitionTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(calculateItem->GetAcquisitionDateTime());
	FString strAcquisitionTime = acquisitionTime.ToString(TEXT("%Y.%m.%d"));

	FText historyText;
	calculateItem->GetRootText(historyText);
	FString strHistory = historyText.ToString();

	FString strFullHistory = FString::Printf(TEXT("%s\n%s"), *strAcquisitionTime, *strHistory);
	_textBlockHistory->SetText(FText::FromString(strFullHistory));

	/*참여자*/
	FString acquisitionUserName = calculateItem->GetAcquisitionUserName();
	FString strParticipant = FString::Printf(TEXT("%s (%d)"), *acquisitionUserName, calculateItem->GetParticipantListCount());
	_textBlockParticipant->SetText(FText::FromString(strParticipant));

	/*아이템 정보*/
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
		_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIGuildCalculateListEntry::OnLongPressIcon);
	}

	ItemId itemId = calculateItem->GetItemData().mTemplateId;
	ItemDBId itemDBId = calculateItem->GetItemData().mItemDBId;
	ItemAmount itemAmount = calculateItem->GetItemData().mItemAmount;
	Level itemLevel = calculateItem->GetItemData().mLevel;
	int32 enchantShield = calculateItem->GetItemData().mEnchantShield;
	int32 enchantBonusIndex = calculateItem->GetItemData().mEnchantBonusIndex;

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

	if (const FGsSchemaItemCommon* itemCommon = itemManager->GetItemTableDataByTID(itemId))
	{
		_textBlockItemName->SetText(itemCommon->name);
		_textBlockItemName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(itemCommon->grade));
	}

	/*낙찰*/
	_textBlockBidderName->SetText(FText::FromString(calculateItem->GetWinningBidder()));
	bool isDone = calculateItem->GetDivisionState() == GuildDivisionState::DONE;
	_textBlockPrice->SetText(isDone ? FText::AsNumber(calculateItem->GetWinningBidPrice()) : FText::AsNumber(0));
	_panelPrice->SetVisibility(isDone ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	/*정산*/
	_btnResult->SetIsEnabled(isDone);

	/*상태*/
	GuildDivisionState state = calculateItem->GetDivisionState();
	switch (state)
	{
	case GuildDivisionState::DONE: // 경매 완료
	{
		GuildAuctionType auctionType = calculateItem->GetDivisionType();
		FText findText;
		switch (auctionType)
		{
		case GuildAuctionType::NONE:
		case GuildAuctionType::NORMAL:
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Complete_Common"), findText);
			break;
		case GuildAuctionType::PARTICIPANTS:
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Complete_Acquisition"), findText);
			break;
		case GuildAuctionType::APPOINTMENT:
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Complete_Appointment"), findText);
			break;
		case GuildAuctionType::DIRECT_DISTRIBUTION:
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Complete_Appointment_Provide"), findText);
			break;
		default:
			break;
		}
		_textBlockState->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_textBlockState->SetText(findText);
		break;
	}
	case GuildDivisionState::FAILED: // 유찰
	{
		FText findText;
		if (calculateItem->GetDivisionType() == GuildAuctionType::DIRECT_DISTRIBUTION)
		{
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Complete_Appointment_Provide"), findText);
		}
		else
		{
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Bid_Failure"), findText);
		}
		
		_textBlockState->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		_textBlockState->SetText(findText);
		break;
	}
	default:
		_textBlockState->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
}

void UGsUIGuildCalculateListEntry::OnClickParticipant()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildDivisionJoin"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildAuctionUser* popup = Cast<UGsUIPopupGuildAuctionUser>(widget.Get()))
		{
			popup->SetData(_calculateItemDBId);
		}
	}
}

void UGsUIGuildCalculateListEntry::OnClickResult()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildDivisionItem* calculateItem = auctionData->GetCalculateItem(_calculateItemDBId);
	if (nullptr == calculateItem)
		return;

	// 정산이 끝났는가?
	if (calculateItem->GetDivisionState() != GuildDivisionState::DONE)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildAuctionCalculate"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildAuctionResult* popup = Cast<UGsUIPopupGuildAuctionResult>(widget.Get()))
		{
			popup->SetCalculate(calculateItem);
		}
	}
}

void UGsUIGuildCalculateListEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
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
	option._bUseCollectionMarking = false;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), FGsItemDetailOpenMessageParamEX::PosDir::CenterPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
}

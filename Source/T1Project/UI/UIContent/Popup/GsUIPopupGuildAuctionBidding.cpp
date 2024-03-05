// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildAuctionBidding.h"

#include "Components/TextBlock.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "Item/GsItemManager.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Net/GsNetSendServiceGuild.h"


void UGsUIPopupGuildAuctionBidding::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnOk)
	{
		_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuctionBidding::OnClickOk);
	}
	if (_btnCancel)
	{
		_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuctionBidding::OnClickCancel);
	}
}

void UGsUIPopupGuildAuctionBidding::NativeConstruct()
{
	Super::NativeConstruct();

	_auctionId = INVALID_GUILD_AUCTION_ID;
	_bidPrice = 0;
}

void UGsUIPopupGuildAuctionBidding::NativeDestruct()
{
	_auctionId = INVALID_GUILD_AUCTION_ID;
	_bidPrice = 0;

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
		_uiIcon = nullptr;
	}

	Super::NativeDestruct();
}

void UGsUIPopupGuildAuctionBidding::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildAuctionBidding::OnClickOk()
{
	if (INVALID_GUILD_AUCTION_ID == _auctionId)
		return;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildAuctionItem* auctionItem = auctionData->GetAuctionItem(_auctionId);
	if (nullptr == auctionItem)
	{
		Close();
		return;
	}

	if (auctionItem->CheckBid(_bidPrice))
	{
		FGsNetSendServiceGuild::SendReqGuildAuctionBidder(auctionItem->GetAuctionId(), CurrencyType::DIA, _bidPrice);
	}

	Close();
}

void UGsUIPopupGuildAuctionBidding::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildAuctionBidding::OnLongPressIcon(UGsUIIconItem& InIcon)
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

void UGsUIPopupGuildAuctionBidding::SetData(const FGsGuildAuctionItem* InAuctionItem, Currency InPrice)
{
	_auctionId = InAuctionItem->GetAuctionId();

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
		_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupGuildAuctionBidding::OnLongPressIcon);
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
	_uiIcon->SetItemNameToolTipFlag(false);
	_uiIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
	_uiIcon->SetItemCollectionMarking(_uiIcon->GetItemIconData(), true);

	if (const FGsSchemaItemCommon* itemCommon = itemManager->GetItemTableDataByTID(itemId))
	{
		_textBlockItemName->SetText(itemCommon->name);
		_textBlockItemName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(itemCommon->grade));
	}

	// 입찰금
	_textBlockPrice->SetText(FText::AsNumber(InPrice));
	_bidPrice = InPrice;
}

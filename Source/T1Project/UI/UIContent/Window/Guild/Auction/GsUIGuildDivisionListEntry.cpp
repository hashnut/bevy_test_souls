// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDivisionListEntry.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/PanelWidget.h"
#include "Components/Image.h"

#include "Guild/GsGuildHelper.h"
#include "Guild/GsGuildAuctionData.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Window/Guild/Auction/GsGuildDivisionListItem.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionUser.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionResult.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"


void UGsUIGuildDivisionListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnParticipant)
	{
		_btnParticipant->OnClicked.AddDynamic(this, &UGsUIGuildDivisionListEntry::OnClickParticipant);
	}
	if (_btnCheck)
	{
		_btnCheck->OnClicked.AddDynamic(this, &UGsUIGuildDivisionListEntry::OnClickCheck);
	}
}

void UGsUIGuildDivisionListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	// TEXT: {0} 남음
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Acquisition_Status_Nhours"), _remainTimeText);

	_colorNormal = FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_DIVISION_STATE_NORMAL);
	_colorEnough = FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_DIVISION_STATE_ENOUGH);
	_colorNotEnough = FGsUIColorHelper::GetColor(EGsUIColorType::GUILD_DIVISION_STATE_NOT_ENOUGH);

	_currentStateColor = _colorNormal;
	_textBlockStateUp->SetColorAndOpacity(_colorNormal);

	_divisionItemDBId = INVALID_ITEM_DB_ID;
}

void UGsUIGuildDivisionListEntry::NativeDestruct()
{
	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
		_uiIcon = nullptr;
	}

	Super::NativeDestruct();
}

void UGsUIGuildDivisionListEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	UGsGuildDivisionListItem* listItem = Cast<UGsGuildDivisionListItem>(ListItemObject);
	if (nullptr == listItem)
		return;

	_divisionItemDBId = listItem->_divisionItemDBId;

	InvalidateAll();
}

void UGsUIGuildDivisionListEntry::OnClickParticipant()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGuildDivisionJoin"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildAuctionUser* popup = Cast<UGsUIPopupGuildAuctionUser>(widget.Get()))
		{
			popup->SetData(_divisionItemDBId);
		}
	}
}

void UGsUIGuildDivisionListEntry::OnClickCheck()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(_divisionItemDBId);
	if (nullptr == divisionItem)
		return;

	uint32 myAuthority = guildManager->GetAuthority();
	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
	time_t expiredTimeSec = divisionItem->GetExpiredDateTime();
	time_t remainSec = expiredTimeSec - serverNowSec;
	// 분배 권한이 있고 분배 가능한 아이템일 때 체크박스 선택 가능
	bool canDivision = FGsGuildHelper::CanDivision(myAuthority) && divisionItem->GetDivisionState() == GuildDivisionState::STORAGE && remainSec > 0;
	if (false == canDivision)
	{
		InvalidateCheck();
		return;
	}

	if (_btnCheck->GetSwitcherIndex() == _btnCheck->_slotIndexSelected)
	{
		auctionData->RemoveCheckedItem(_divisionItemDBId);
	}
	else
	{
		auctionData->AddCheckedItem(_divisionItemDBId);
	}
	GMessage()->GetGuild().SendMessage(MessageContentGuild::DIVISION_LIST_CHECKED, nullptr);
}

void UGsUIGuildDivisionListEntry::OnLongPressIcon(UGsUIIconItem& InIcon)
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

void UGsUIGuildDivisionListEntry::SetStateColor(const FLinearColor InColor)
{
	if (_currentStateColor != InColor)
	{
		_currentStateColor = InColor;
		_textBlockStateUp->SetColorAndOpacity(InColor);
	}
}

void UGsUIGuildDivisionListEntry::InvalidateAll()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(_divisionItemDBId);
	if (nullptr == divisionItem)
		return;

	/*레드닷*/
	bool isNew = divisionItem->GetAcquisitionDateTime() > auctionData->GetPrevCheckTimeDivision();
	_uiRedDot->SetVisibility(isNew ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	/*획득 정보*/
	FDateTime acquisitionTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(divisionItem->GetAcquisitionDateTime());
	FString strAcquisitionTime = acquisitionTime.ToString(TEXT("%Y.%m.%d"));

	FText historyText;
	divisionItem->GetRootText(historyText);
	FString strHistory = historyText.ToString();

	FString strFullHistory = FString::Printf(TEXT("%s\n%s"), *strAcquisitionTime, *strHistory);
	_textBlockDate->SetText(FText::FromString(strFullHistory));

	/*참여자*/
	FString acquisitionUserName = divisionItem->GetAcquisitionUserName();
	FString strParticipant = FString::Printf(TEXT("%s (%d)"), *acquisitionUserName, divisionItem->GetParticipantListCount());
	_textBlockParticipant->SetText(FText::FromString(strParticipant));

	InvalidateItem(); // 아이템 정보
	CheckAndInvalidateState(); // 상태
	InvalidateCheck(); // 체크
}

void UGsUIGuildDivisionListEntry::InvalidateItem()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(_divisionItemDBId);
	if (nullptr == divisionItem)
		return;

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
		_uiIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIGuildDivisionListEntry::OnLongPressIcon);
	}

	ItemId itemId = divisionItem->GetItemData().mTemplateId;
	ItemDBId itemDBId = divisionItem->GetItemData().mItemDBId;
	ItemAmount itemAmount = divisionItem->GetItemData().mItemAmount;
	Level itemLevel = divisionItem->GetItemData().mLevel;
	int32 enchantShield = divisionItem->GetItemData().mEnchantShield;
	int32 enchantBonusIndex = divisionItem->GetItemData().mEnchantBonusIndex;

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

bool UGsUIGuildDivisionListEntry::CheckAndInvalidateState()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return false;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return false;

	const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(_divisionItemDBId);
	if (nullptr == divisionItem)
		return false;

	bool needRefreshList = false;

	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
	time_t expiredTimeSec = divisionItem->GetExpiredDateTime();
	time_t remainSec = expiredTimeSec - serverNowSec;

	_imageBackGround->SetIsEnabled(remainSec >= 0);

	if (remainSec < 0)
	{
		needRefreshList = true;
		remainSec = 0;
		InvalidateCheck();
	}

	if (remainSec >= 3600) // 1 HOUR
	{
		SetStateColor(_colorEnough);
	}
	else // remainSec < 3600
	{
		SetStateColor(_colorNotEnough);
	}

	FString strRemainTime;
	FGsTimeStringHelper::GetTimeStringFloor(FTimespan::FromSeconds(remainSec), strRemainTime);
	FText formatText = FText::Format(_remainTimeText, FText::FromString(strRemainTime));
	_textBlockStateUp->SetText(formatText);

	return needRefreshList;
}

void UGsUIGuildDivisionListEntry::InvalidateCheck()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(_divisionItemDBId);
	if (nullptr == divisionItem)
		return;

	uint32 myAuthority = guildManager->GetAuthority();
	time_t serverNowSec = FGsTimeSyncUtil::GetServerNowSecond();
	time_t expiredTimeSec = divisionItem->GetExpiredDateTime();
	time_t remainSec = expiredTimeSec - serverNowSec;
	// 분배 권한이 있고 분배 가능한 아이템일 때 체크박스 표시
	bool canDivision = FGsGuildHelper::CanDivision(myAuthority) && divisionItem->GetDivisionState() == GuildDivisionState::STORAGE && remainSec > 0;
	_switcherCheck->SetActiveWidgetIndex(0); // index : 체크박스(0), 거래취소(1)
	_switcherCheck->SetVisibility(canDivision ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

	bool isChecked = auctionData->IsCheckedItem(_divisionItemDBId);
	_btnCheck->SetSwitcherIndex(isChecked ? _btnCheck->_slotIndexSelected : _btnCheck->_slotIndexUnselected);
}
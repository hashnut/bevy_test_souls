// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildAuctionDirect.h"

#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"

#include "Guild/GsGuildHelper.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"

#include "UI/UIContent/Popup/Guild/GuildAuction/GsUIGuildAuctionAppointmentEntry.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Net/GsNetSendServiceGuild.h"

#include "Item/GsItemEquipment.h"
#include "Item/GsItemManager.h"

#include "UTIL/GsTimeSyncUtil.h"


void UGsUIPopupGuildAuctionDirect::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_entryWidgetClass, _panelUserList);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupGuildAuctionDirect::OnRefreshEntryUserList);
	}

	if (nullptr == _scrollBoxItemHelper)
	{
		_scrollBoxItemHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_scrollBoxItemHelper->Initialize(_iconSelector);
		_scrollBoxItemHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPopupGuildAuctionDirect::OnRefreshIcon);
	}

	if (_btnOk)
	{
		_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuctionDirect::OnClickOk);
	}
	if (_btnCancel)
	{
		_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuctionDirect::OnClickCancel);
	}

	_toggleGroupUserList.OnSlotClicked.BindUObject(this, &UGsUIPopupGuildAuctionDirect::OnClickUserName);
}

void UGsUIPopupGuildAuctionDirect::NativeConstruct()
{
	Super::NativeConstruct();

	_selectedUserDBId = INVALID_USER_DB_ID;
	_userInfoList.Empty();
	_itemDBIdList.Empty();
	InvalidateButton();
}

void UGsUIPopupGuildAuctionDirect::NativeDestruct()
{
	_selectedUserDBId = INVALID_USER_DB_ID;
	_userInfoList.Empty();
	_itemDBIdList.Empty();

	if (nullptr != _scrollBoxItemHelper)
	{
		_scrollBoxItemHelper->Clear();
	}

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::NativeDestruct();
}

void UGsUIPopupGuildAuctionDirect::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildAuctionDirect::OnClickOk()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
	{
		Close();
		return;
	}

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
	{
		Close();
		return;
	}

	uint32 myAuthority = guildManager->GetAuthority();
	if (false == FGsGuildHelper::CanDivision(myAuthority))
	{
		// 기사단 분배 권한이 없습니다.
		FText errorText;
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Authority_Notify"), errorText);
		FGsUIHelper::TrayMessageTicker(errorText);
		return;
	}

	// 버튼 비활성화 상태어야 한다.
	if (INVALID_USER_DB_ID == _selectedUserDBId)
	{
		InvalidateButton();
		return;
	}

	if (0 >= _itemDBIdList.Num())
	{
		Close();
		return;
	}

	for (ItemDBId itemDBId : _itemDBIdList)
	{
		const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(itemDBId);

		if (INVALID_ITEM_DB_ID == itemDBId || nullptr == divisionItem)
		{
			// 선택된 획득 내역이 없어 분배를 시작할 수 없습니다.
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_NoCheck_Notify"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			Close();
			return;
		}

		if (divisionItem->GetDivisionState() != GuildDivisionState::STORAGE)
		{
			// 경매 가능한 아이템이 없습니다. - 이미 경매에 등록된 경우
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Empty_Notify"), errorText);
			FGsUIHelper::TrayMessageTicker(errorText);
			Close();
			return;
		}
	}

	TArray<UserDBId> selectedUserDBIdList{_selectedUserDBId};
	TArray<Currency> currencyList;
	FGsNetSendServiceGuild::SendReqGuildAuctionRegister(_itemDBIdList, GuildAuctionType::DIRECT_DISTRIBUTION, GuildAuctionResultType::NORMAL, selectedUserDBIdList, CurrencyType::DIA, currencyList);
	Close();
}

void UGsUIPopupGuildAuctionDirect::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildAuctionDirect::OnClickUserName(int32 InIndex, bool bInIsSelected)
{
	if (false == _userInfoList.IsValidIndex(InIndex))
		return;

	_selectedUserDBId = _userInfoList[InIndex].Key;
	InvalidateButton();
}

void UGsUIPopupGuildAuctionDirect::OnLongPressIcon(UGsUIIconItem& InIcon)
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

void UGsUIPopupGuildAuctionDirect::OnRefreshEntryUserList(int32 InIndex, UWidget* InEntry)
{
	if (false == _userInfoList.IsValidIndex(InIndex))
		return;

	UGsUIGuildAuctionAppointmentEntry* entry = Cast<UGsUIGuildAuctionAppointmentEntry>(InEntry);
	if (nullptr == entry)
		return;

	entry->SetData(_userInfoList[InIndex].Key, FText::FromString(_userInfoList[InIndex].Value));
}

void UGsUIPopupGuildAuctionDirect::OnRefreshIcon(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	if (false == _itemDBIdList.IsValidIndex(InIndex))
		return;

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
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

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupGuildAuctionDirect::OnLongPressIcon);
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

void UGsUIPopupGuildAuctionDirect::InvalidateButton()
{
	_btnOk->SetIsEnabled(_selectedUserDBId != INVALID_USER_DB_ID);
}

void UGsUIPopupGuildAuctionDirect::SetData(const TArray<ItemDBId>& InDivisionItemDBIdList)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	_itemDBIdList = InDivisionItemDBIdList;

	/* 아이템 목록 위 문구 설정 */
	FText titleText;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Appointment_Provide_PopUp_Title"), titleText);

	FText findText;
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_PopUp_Description"), findText);
	FText formatText = FText::Format(findText, FText::AsNumber(_itemDBIdList.Num()), titleText);

	_textBlockStart->SetText(formatText);

	_scrollBoxItemHelper->RefreshAll(_itemDBIdList.Num());

	/* 기사단원 캐릭터명 표시  */
	time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
	TArray<const FGsGuildMemberData*> sortMemberList;
	TArray<FGsGuildMemberData*> memberList = guildManager->GetMemberList();
	for (const FGsGuildMemberData* memberData : memberList)
	{
		if (nullptr == memberData)
			continue;

		// 기사단 가입 후 경과한 시간이 경매 참여 가능 시간보다 작으면 표시하지 않는다.
		time_t elapsedTime = currentTime - memberData->_joinedTime; // 경과 시간 = 현재 - 과거
		if (auctionData->GetAuctionStandby() > elapsedTime)
			continue;

		if ((uint8)memberData->_memberGrade < (uint8)GuildMemberGradeType::SECONDKNIGHT)
			continue;

		sortMemberList.Emplace(memberData);
	}

	// 이름 오름차순 정렬
	sortMemberList.Sort([](const FGsGuildMemberData& lhs, const FGsGuildMemberData& rhs)
		{
			return lhs._userName.Compare(rhs._userName) < 0;
		});

	_userInfoList.Empty();
	for (const FGsGuildMemberData* memberData : sortMemberList)
	{
		_userInfoList.Emplace(memberData->_userDBId, memberData->_userName);
	}

	_selectedUserDBId = INVALID_USER_DB_ID;
	_scrollBoxHelper->RefreshAll(_userInfoList.Num());

	_toggleGroupUserList.Clear();
	_toggleGroupUserList.AddToggleWidgetByParentPanel(_panelUserList);
	_toggleGroupUserList.SetSelectedIndex(-1);
}

void UGsUIPopupGuildAuctionDirect::CheckItemValid()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	bool isExist = true;
	time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();

	for (ItemDBId itemDBId : _itemDBIdList)
	{
		const FGsGuildDivisionItem* divisionData = auctionData->GetDivisionItem(itemDBId);
		if (nullptr == divisionData)
		{
			isExist = false;
			break;
		}

		if (divisionData->GetDivisionState() != GuildDivisionState::STORAGE)
		{
			isExist = false;
			break;
		}

		if (divisionData->GetExpiredDateTime() < currentTime)
		{
			isExist = false;
			break;
		}
	}

	if (false == isExist)
	{
		Close();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupGuildAuctionAppointment.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"

#include "Guild/GsGuildAuctionData.h"
#include "Guild/GsGuildHelper.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Common/Numpad/GsUINumpadDefault.h"

#include "UI/UIContent/Popup/Guild/GuildAuction/GsUIGuildAuctionAppointmentEntry.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Net/GsNetSendServiceGuild.h"

#include "Item/GsItemEquipment.h"
#include "Item/GsItemManager.h"


void UGsUIPopupGuildAuctionAppointment::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _scrollBoxHelper)
	{
		_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_scrollBoxHelper->Initialize(_entryWidgetClass, _panelUserList);
		_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupGuildAuctionAppointment::OnRefreshEntryUserList);
	}

	if (nullptr == _scrollBoxItemHelper)
	{
		_scrollBoxItemHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_scrollBoxItemHelper->Initialize(_iconSelectorMulti);
		_scrollBoxItemHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPopupGuildAuctionAppointment::OnRefreshIcon);
	}

	if (_btnOk)
	{
		_btnOk->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuctionAppointment::OnClickOk);
	}
	if (_btnCancel)
	{
		_btnCancel->OnClicked.AddDynamic(this, &UGsUIPopupGuildAuctionAppointment::OnClickCancel);
	}
	if (_uiNumpad)
	{
		_uiNumpad->OnChangeNumber().BindUObject(this, &UGsUIPopupGuildAuctionAppointment::OnChangeNumpadNumber);
	}
}

void UGsUIPopupGuildAuctionAppointment::NativeConstruct()
{
	Super::NativeConstruct();

	_itemDBIdList.Empty();
	_userInfoList.Empty();
	_selectedUserList.Empty();
	_biddingStartPrice = 0;
	_minPrice = 0;
	InvalidateButton();
}

void UGsUIPopupGuildAuctionAppointment::NativeDestruct()
{
	_itemDBIdList.Empty();
	_userInfoList.Empty();
	_selectedUserList.Empty();
	_biddingStartPrice = 0;
	_minPrice = 0;

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}
	_uiIcon = nullptr;

	if (nullptr != _scrollBoxItemHelper)
	{
		_scrollBoxItemHelper->Clear();
	}
	if (nullptr != _iconSelectorMulti)
	{
		_iconSelectorMulti->RemoveAllChildren();
	}

	Super::NativeDestruct();
}

void UGsUIPopupGuildAuctionAppointment::OnInputCancel()
{
	Close();
}

void UGsUIPopupGuildAuctionAppointment::OnClickOk()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

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
	if (0 >= _selectedUserList.Num())
	{
		InvalidateButton();
		return;
	}

	if (0 >= _itemDBIdList.Num())
	{
		Close();
		return;
	}

	if (1 == _itemDBIdList.Num())
	{
		if (_minPrice > _biddingStartPrice)
		{
			// TEXT: 경매 시작 금액은 최소 {0} 이상이어야 합니다.
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Appointment_PopUp_BeginPriceLack"), errorText);
			errorText = FText::Format(errorText, _minPrice);
			FGsUIHelper::TrayMessageTicker(errorText);
			return;
		}
	}

	for (ItemDBId itemDBId : _itemDBIdList)
	{
		const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(itemDBId);

		if (INVALID_ITEM_DB_ID == itemDBId || nullptr == divisionItem)
		{
			// 경매 가능한 아이템이 없습니다. - 이미 경매에 등록된 경우
			FText errorText;
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_Empty_Notify"), errorText);
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

	TArray<Currency> currencyList;
	if (1 == _itemDBIdList.Num())
	{
		currencyList.Emplace(_biddingStartPrice);
	}
	else if (1 < _itemDBIdList.Num())
	{
		for (ItemDBId itemDBId : _itemDBIdList)
		{
			const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(itemDBId);
			if (nullptr == divisionItem)
				return;

			ItemGrade itemGrade = ItemGrade::NONE;
			if (const FGsSchemaItemCommon* itemCommon = itemManager->GetItemTableDataByTID(divisionItem->GetItemData().mTemplateId))
			{
				itemGrade = itemCommon->grade;
			}

			Currency minAuctionPrice = auctionData->GetMinAuctionPrice(itemGrade);
			currencyList.Emplace(minAuctionPrice);
		}
	}

	FGsNetSendServiceGuild::SendReqGuildAuctionRegister(_itemDBIdList, GuildAuctionType::APPOINTMENT, _resultType, _selectedUserList, CurrencyType::DIA, currencyList);
	Close();
}

void UGsUIPopupGuildAuctionAppointment::OnClickCancel()
{
	OnInputCancel();
}

void UGsUIPopupGuildAuctionAppointment::OnClickedUserName(UserDBId InUserDBId, bool InIsOn)
{
	if (InIsOn)
	{
		if (false == _selectedUserList.Contains(InUserDBId))
		{
			_selectedUserList.Emplace(InUserDBId);
		}
	}
	else
	{
		if (_selectedUserList.Contains(InUserDBId))
		{
			_selectedUserList.Remove(InUserDBId);
		}
	}
	InvalidateBidderCount();
	InvalidateButton();
}

void UGsUIPopupGuildAuctionAppointment::OnLongPressIcon(UGsUIIconItem& InIcon)
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

void UGsUIPopupGuildAuctionAppointment::OnRefreshEntryUserList(int32 InIndex, class UWidget* InEntry)
{
	if (false == _userInfoList.IsValidIndex(InIndex))
		return;

	UGsUIGuildAuctionAppointmentEntry* entry = Cast<UGsUIGuildAuctionAppointmentEntry>(InEntry);
	if (nullptr == entry)
		return;

	entry->SetData(_userInfoList[InIndex].Key, FText::FromString(_userInfoList[InIndex].Value));

	if (entry->OnClickUserName.IsBound())
	{
		entry->OnClickUserName.Unbind();
	}
	entry->OnClickUserName.BindUObject(this, &UGsUIPopupGuildAuctionAppointment::OnClickedUserName);
}

void UGsUIPopupGuildAuctionAppointment::OnRefreshIcon(int32 InIndex, class UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
		return;

	if (false == _itemDBIdList.IsValidIndex(InIndex))
		return;

	SetUIIcon(itemIcon, _itemDBIdList[InIndex]);
}

void UGsUIPopupGuildAuctionAppointment::InvalidateBidderCount()
{
	/* 입찰자 인원 : {0}명 */
	FText findText;
	FText::FindText(TEXT("SystemText"), TEXT("NPersons"), findText);
	FText formatText =  FText::Format(findText, _selectedUserList.Num());

	_textBlockBidderCount->SetText(formatText);
}

void UGsUIPopupGuildAuctionAppointment::InvalidateButton()
{
	_btnOk->SetIsEnabled(_selectedUserList.Num() > 0);
}

void UGsUIPopupGuildAuctionAppointment::OnChangeNumpadNumber(int32 InNumber)
{
	_biddingStartPrice = InNumber;
	_textBlockPrice->SetText(FText::AsNumber(InNumber));
}

void UGsUIPopupGuildAuctionAppointment::SetUIIcon(UGsUIIconItemInventory* InIcon, ItemDBId InItemDBId)
{
	if (nullptr == InIcon)
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

	const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(InItemDBId);
	if (nullptr == divisionItem)
		return;

	if (false == InIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		InIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupGuildAuctionAppointment::OnLongPressIcon);
	}

	ItemId itemId = divisionItem->GetItemData().mTemplateId;
	ItemDBId itemDBId = divisionItem->GetItemData().mItemDBId;
	ItemAmount itemAmount = divisionItem->GetItemData().mItemAmount;
	Level itemLevel = divisionItem->GetItemData().mLevel;
	int32 enchantShield = divisionItem->GetItemData().mEnchantShield;
	int32 enchantBonusIndex = divisionItem->GetItemData().mEnchantBonusIndex;

	if (ItemType::EQUIP == itemManager->GetItemTypeByTID(itemId))
	{
		InIcon->SetNotOwnItem(itemId, itemAmount, itemLevel, enchantShield, enchantBonusIndex);
	}
	else
	{
		InIcon->SetNotOwnItem(itemId, itemAmount, itemLevel);
	}

	InIcon->SetActiveIconBg(true);
	InIcon->SetItemNameVisibility(false);
	InIcon->SetDimmedImage(false);
	InIcon->SetSlotSelectionImage(false);
	InIcon->SetItemNameToolTipFlag(true);
	InIcon->SetItemNameToolTipHoverMode(true);
	InIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
}

void UGsUIPopupGuildAuctionAppointment::SetData(const TArray<ItemDBId>& InDivisionItemDBIdList, GuildAuctionResultType InResultType)
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

	/* 아이템 아이콘 설정 */
	if (nullptr == _uiIcon)
	{
		_uiIcon = Cast<UGsUIIconItemInventory>(_iconSelector->CreateIcon());
		_iconSelector->AddChild(_uiIcon);
	}

	if (nullptr == _uiIcon)
		return;

	_itemDBIdList = InDivisionItemDBIdList;
	_resultType = InResultType;

	if (1 == _itemDBIdList.Num())
	{
		_switcherCount->SetActiveWidgetIndex(0);
		const FGsGuildDivisionItem* divisionItem = auctionData->GetDivisionItem(_itemDBIdList[0]);
		if (nullptr == divisionItem)
			return;

		/* 아이템 이름 설정 */
		ItemGrade itemGrade = ItemGrade::NONE;
		if (const FGsSchemaItemCommon* itemCommon = itemManager->GetItemTableDataByTID(divisionItem->GetItemData().mTemplateId))
		{
			itemGrade = itemCommon->grade;
			_textBlockItemName->SetText(itemCommon->name);
			_textBlockItemName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(itemCommon->grade));
		}

		SetUIIcon(_uiIcon, _itemDBIdList[0]);

		/* 계산기 설정 */
		Currency minAuctionPrice = auctionData->GetMinAuctionPrice(itemGrade);

		_biddingStartPrice = minAuctionPrice;
		_minPrice = minAuctionPrice;

		_uiNumpad->SetMaxNumber(auctionData->GetMaxAuctionBeginPrice());
		_uiNumpad->SetNumber(minAuctionPrice);

		_textBlockPrice->SetText(FText::AsNumber(minAuctionPrice));
	}
	else
	{
		_switcherCount->SetActiveWidgetIndex(1);

		FText titleText;
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Appointment_PopUp_Title"), titleText);

		FText findText;
		FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Begin_PopUp_Description"), findText);
		FText formatText = FText::Format(findText, FText::AsNumber(_itemDBIdList.Num()), titleText);

		_textBlockStart->SetText(formatText);
		_scrollBoxItemHelper->RefreshAll(_itemDBIdList.Num());
	}

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

	_scrollBoxHelper->RefreshAll(_userInfoList.Num());

	_selectedUserList.Empty();
	InvalidateBidderCount();
}

void UGsUIPopupGuildAuctionAppointment::CheckItemValid()
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

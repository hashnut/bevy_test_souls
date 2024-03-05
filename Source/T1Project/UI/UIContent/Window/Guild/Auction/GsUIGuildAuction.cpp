// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildAuction.h"

#include "Components/WidgetSwitcher.h"
#include "Components/ListView.h"
#include "Components/EditableText.h"

#include "Currency/GsCurrencyHelper.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"

#include "UI/UIContent/Common/Numpad/GsUINumpadDefault.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionBidding.h"

#include "UI/UIContent/Window/Guild/Auction/GsUIGuildAuctionChat.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildAuctionChatMacro.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildAuctionListEntry.h"
#include "UI/UIContent/Window/Guild/Auction/GsGuildAuctionListItem.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildStorageComboBox.h"

#include "UTIL/GsTimeSyncUtil.h"


void UGsUIGuildAuction::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_listView)
	{
		_listView->OnItemClicked().AddUObject(this, &UGsUIGuildAuction::OnClickAuctionEntry);
		_listView->OnListViewScrolled().AddUObject(this, &UGsUIGuildAuction::OnListViewScrolled);
	}

	if (_checkBoxNumpad)
	{
		_checkBoxNumpad->OnCheckStateChangedEx.AddDynamic(this, &UGsUIGuildAuction::OnChangeCheckNumpad);
	}

	if (_inputBidPrice)
	{
		_inputBidPrice->OnTextChanged.AddDynamic(this, &UGsUIGuildAuction::OnTextChangedBidPrice);
		_inputBidPrice->OnTextCommitted.AddDynamic(this, &UGsUIGuildAuction::OnTextCommittedBidPrice);
	}

	if (_uiNumpad)
	{
		_uiNumpad->OnChangeNumber().BindUObject(this, &UGsUIGuildAuction::OnChangeNumpadNumber);
	}

	if (_uiChat)
	{
		_uiChat->OnClickedMacro.BindUObject(this, &UGsUIGuildAuction::OnClickedMacro);
	}
	if (_uiChatMacro)
	{
		_uiChatMacro->OnClickedMacroEntry.BindUObject(this, &UGsUIGuildAuction::OnClickedMacroEntry);
	}

	_comboItemCategory->OnClickComboMenu.BindUObject(this, &UGsUIGuildAuction::OnClickComboMenuCategory);
	_comboItemCategory->OnSelectComboBoxSub.BindUObject(this, &UGsUIGuildAuction::OnSelectComboBoxCategory);
	_comboBid->OnClickComboMenu.BindUObject(this, &UGsUIGuildAuction::OnClickComboMenuBid);
	_comboBid->OnSelectComboBoxMain.BindUObject(this, &UGsUIGuildAuction::OnSelectComboBoxBid);
}

void UGsUIGuildAuction::NativeConstruct()
{
	Super::NativeConstruct();

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	_cachedAuctionData = auctionData;

	auctionData->MakeComboBoxCategory(_comboItemCategory);
	FGsGuildAuctionData::MakeComboBoxBid(_comboBid);
}

void UGsUIGuildAuction::NativeDestruct()
{
	_cachedAuctionData = nullptr;

	_auctionIdList.Empty();

	Super::NativeDestruct();
}

void UGsUIGuildAuction::Show(bool InIsStackRecoverFlag)
{
	if (nullptr == _cachedAuctionData)
		return;

	_isStackRecoverFlag = InIsStackRecoverFlag;

	SendRequestPacket();

	if (FGsGuildManager* guildManager = GGuild())
	{
		/*레드닷*/
		_cachedAuctionData->SetPrevCheckTimeAuction(guildManager->GetRedDotCheckData()._auctionCheckTime);
		guildManager->GetRedDotCheckData().UpdateAuctionCheckTime();
		guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Auction);
	}

	/*기사단 채팅*/
	const int32 ENABLE_CHAT = 0;
	_switcherChat->SetActiveWidgetIndex(ENABLE_CHAT);
	_uiChat->Show();
	_uiChatMacro->Hide();

	/*입찰금 & 숫자패드*/
	_checkBoxNumpad->SetIsSelected(false);
	_maxAuctionPrice = _cachedAuctionData->GetMaxAuctionPrice();
	_uiNumpad->SetMaxNumber(_maxAuctionPrice);
	_uiNumpad->SetNumber(0, true);
	SetBidPrice(0);

	/*스크롤 감추기 및 상단 이동*/
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_listView->ScrollToTop();

	/*필터 상태 초기화*/
	TPair<int32, int32> filterCategory = _cachedAuctionData->GetFilterCategory(EGsGuildAuctionFilterType::AUCTION_CATEGORY);

	_comboItemCategory->SetSelectedIndex(filterCategory.Key, filterCategory.Value);
	_comboBid->SetSelectedIndex(_cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::AUCTION_CAN_BID) ? 1 : 0);

	/*경매 목록*/
	_cachedAuctionData->SetSelectedAuctionId(INVALID_GUILD_AUCTION_ID);
	InvalidateList();

	_uiContent->OnShowAuction();
}

void UGsUIGuildAuction::Hide()
{
	if (nullptr == _cachedAuctionData)
		return;

	//_cachedAuctionData->SetPrevCheckTimeAuction(FGsTimeSyncUtil::GetServerNowSecond());

	_cachedAuctionData->ResetAuctionList();

	_uiContent->OnHideAuction();

	_comboItemCategory->Close();
	_comboBid->Close();
}

void UGsUIGuildAuction::InvalidateTick()
{
	if (_switcherList->GetActiveWidgetIndex() == AUCTION_EXIST)
	{
		bool needRefreshList = false;
		const TArray<UUserWidget*>& entries = _listView->GetDisplayedEntryWidgets();
		for (int32 i = 0; i < entries.Num(); ++i)
		{
			UGsUIGuildAuctionListEntry* entry = Cast<UGsUIGuildAuctionListEntry>(entries[i]);
			needRefreshList |= entry->CheckAndInvalidateRemainTime();
		}

		if (needRefreshList)
		{
			GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_LIST, nullptr);
		}
	}
}

void UGsUIGuildAuction::InvalidateList()
{
	_requestList = false;

	if (nullptr == _cachedAuctionData)
		return;

	bool filterCanBid = _cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::AUCTION_CAN_BID);
	TArray<MarketCategory> filterCategoryList = _cachedAuctionData->GetFilterCategoryList(EGsGuildAuctionFilterType::AUCTION_CATEGORY);

	// 데이터 갱신
	_realDataCount = 0;
	time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
	_auctionIdList.Empty();
	const TArray<FGsGuildAuctionItem>& auctionDataList = _cachedAuctionData->GetAuctionList();
	for (int32 i = 0; i < auctionDataList.Num(); ++i)
	{
		const FGsGuildAuctionItem& auctionData = auctionDataList[i];

		// 경매진행중이 아닌 항목은 노출시키지 않는다.
		if (auctionData.GetAuctionState() != GuildDivisionState::AUCTION)
			continue;

		// 만료 예정 시각을 지났다면 노출시키지 않는다.
		if (auctionData.GetExpiredDateTime() < currentTime)
			continue;

		++_realDataCount;

		// 아이템 정보 필터
		if (filterCategoryList.Num() > 0 && false == filterCategoryList.Contains(auctionData.GetMarketCategory()))
			continue;

		// 입찰 가능 필터
		if (filterCanBid && false == auctionData.CanBid())
			continue;

		_auctionIdList.Emplace(auctionData.GetAuctionId());
	}
	_auctionIdList.Sort([](GuildAuctionId lhs, GuildAuctionId rhs)
		{
			return lhs > rhs;
		});

	TArray<UGsGuildAuctionListItem*> auctionList;
	for (int32 i = 0; i < _auctionIdList.Num(); ++i)
	{
		UGsGuildAuctionListItem* listItem = NewObject<UGsGuildAuctionListItem>(this);
		listItem->SetData(_auctionIdList[i]);
		auctionList.Emplace(listItem);
	}

	// 경매 내역이 존재한다.
	if (auctionList.Num() > 0)
	{
		_switcherList->SetActiveWidgetIndex(AUCTION_EXIST);
		_listView->SetListItems(auctionList);

		if (_isStackRecoverFlag)
		{
			_listView->SetScrollOffset(_cachedAuctionData->GetScrollOffset());
		}
	}
	// 경매 내역이 존재하지 않는다.
	else
	{
		_switcherList->SetActiveWidgetIndex(AUCTION_NONE);
		_listView->ClearListItems();
	}

	_uiContent->InvalidateAuctionCount(_cachedAuctionData->GetAuctionListCount(), _cachedAuctionData->GetMaxAuctionCount());
}

void UGsUIGuildAuction::OnReceiveAuctionList()
{
	InvalidateList();
}

void UGsUIGuildAuction::OnUpdatedAuctionItem(uint64 InAuctionId)
{
	const TArray<UUserWidget*>& entries = _listView->GetDisplayedEntryWidgets();
	for (int32 i = 0; i < entries.Num(); ++i)
	{
		if (UGsUIGuildAuctionListEntry* entry = Cast<UGsUIGuildAuctionListEntry>(entries[i]))
		{
			if (entry->GetAuctionId() == InAuctionId)
			{
				entry->InvalidateAll();
			}
		}
	}

	if (nullptr == _cachedAuctionData)
		return;

	const FGsGuildAuctionItem* auctionItem = _cachedAuctionData->GetAuctionItem(InAuctionId);
	if (nullptr == auctionItem)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("PopupGuildBidding"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildAuctionBidding* popup = Cast<UGsUIPopupGuildAuctionBidding>(widget.Get()))
		{
			// 내가 입찰 대기중인 금액보다 다른 사람이 높게 입찰했을 때 팝업 닫기
			if (InAuctionId == popup->GetAuctionId() && auctionItem->GetBidPrice() > popup->GetBidPrice())
			{
				uiManager->CloseByKeyName(TEXT("PopupGuildBidding"));
			}
		}
	}
}

void UGsUIGuildAuction::OnRemovedAuctionItem(uint64 InAuctionId)
{
	if (nullptr == _cachedAuctionData)
		return;

	if (_cachedAuctionData->GetSelectedAuctionId() == InAuctionId)
	{
		_cachedAuctionData->SetSelectedAuctionId(INVALID_GUILD_AUCTION_ID);
	}

	InvalidateList();

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("PopupGuildBidding"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildAuctionBidding* popup = Cast<UGsUIPopupGuildAuctionBidding>(widget.Get()))
		{
			// 내가 입찰 대기중이던 아이템이 경매 종료되었을 때 팝업 닫기
			if (popup->GetAuctionId() == InAuctionId)
			{
				uiManager->CloseByKeyName(TEXT("PopupGuildBidding"));
			}
		}
	}
}

void UGsUIGuildAuction::SendRequestPacket()
{
	if (nullptr == _cachedAuctionData)
		return;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

//#if WITH_EDITOR
//	// 에디터에서는 들어갈 때마다 정보 초기화 -> 이후 재요청 로직
//	_cachedAuctionData->ResetAuctionList();
//#endif

	// 신규 내역이 존재한다면 데이터 초기화 후 다시 요청한다.
	bool isRedDot = guildManager->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Auction);
	if (isRedDot)
	{
		_cachedAuctionData->ResetAuctionList();
		_cachedAuctionData->SendReqAuctionList(0);
		return;
	}

	// 비어있을 경우 정보를 요청한다.
	bool isEmpty = _cachedAuctionData->GetAuctionList().Num() <= 0;
	if (isEmpty)
	{
		_cachedAuctionData->ResetAuctionList();
		_cachedAuctionData->SendReqAuctionList(0);
		return;
	}
}

void UGsUIGuildAuction::OnChangeCheckNumpad(int32 InIndexGroup, bool InIsChecked)
{
	const int32 ENABLE_CHAT = 0;
	const int32 ENABLE_NUMPAD = 1;

	_switcherChat->SetActiveWidgetIndex(InIsChecked ? ENABLE_NUMPAD : ENABLE_CHAT);
	_uiChatMacro->Hide();
}

void UGsUIGuildAuction::OnTextChangedBidPrice(const FText& InText)
{
	Currency price = GetPriceFromText(InText);
	SetBidPriceWithNumpad(price);
}

void UGsUIGuildAuction::OnTextCommittedBidPrice(const FText& InText, ETextCommit::Type InType)
{
	if (ETextCommit::OnEnter == InType)
	{
		Currency price = GetPriceFromText(InText);
		SetBidPriceWithNumpad(price);
	}
	else if (ETextCommit::OnEnter == InType)
	{
		SetBidPriceWithNumpad(0);
	}
}

void UGsUIGuildAuction::OnClickAuctionEntry(UObject* InListItemData)
{
	UGsGuildAuctionListItem* listItem = Cast<UGsGuildAuctionListItem>(InListItemData);
	if (nullptr == listItem)
		return;

	if (nullptr == _cachedAuctionData)
		return;

	const FGsGuildAuctionItem* auctionItem = _cachedAuctionData->GetAuctionItem(listItem->_auctionId);
	if (nullptr == auctionItem)
		return;

	_cachedAuctionData->SetSelectedAuctionId(listItem->_auctionId);

	const TArray<UUserWidget*>& entries = _listView->GetDisplayedEntryWidgets();
	for (int32 i = 0; i < entries.Num(); ++i)
	{
		if (UGsUIGuildAuctionListEntry* entry = Cast<UGsUIGuildAuctionListEntry>(entries[i]))
		{
			entry->InvalidateAll();
		}
	}

	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->PlayPressSound();
	}

	if (auctionItem->GetBidderName().IsEmpty())
	{
		SetBidPriceWithNumpad(auctionItem->GetBidPrice());
	}
	else
	{
		Currency autoAddedBidPrice = auctionItem->GetBidPrice() + _cachedAuctionData->GetAutoAddedBid();
		SetBidPriceWithNumpad(autoAddedBidPrice);
	}
}

void UGsUIGuildAuction::OnClickedMacro()
{
	if (_uiChatMacro->IsVisible())
	{
		_uiChatMacro->Hide();
	}
	else
	{
		_uiChatMacro->Show();
	}
}

void UGsUIGuildAuction::OnClickedMacroEntry(const FText& InMacroText)
{
	_uiChatMacro->Hide();
	_uiChat->OnInputChat(InMacroText);
}

void UGsUIGuildAuction::OnChangeNumpadNumber(int32 InNumber)
{
	Currency changedBidPrice = InNumber;
	Currency myDiaAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::DIA);
	if (changedBidPrice > myDiaAmount)
	{
		changedBidPrice = myDiaAmount;
	}

	changedBidPrice = FMath::Clamp<Currency>(changedBidPrice, 0, _maxAuctionPrice);
	if (changedBidPrice != static_cast<Currency>(InNumber))
	{
		_uiNumpad->SetNumber(changedBidPrice, false);
	}

	SetBidPrice(changedBidPrice);
}

void UGsUIGuildAuction::OnListViewScrolled(float InOffsetInItems, float InDistanceRemaining)
{
	if (_requestList)
		return;

	if (0 >= _listView->GetDisplayedEntryWidgets().Num())
		return;

	if (nullptr == _cachedAuctionData)
		return;

	_cachedAuctionData->SetScrollOffset(InOffsetInItems);

	float listViewHeight = _listView->GetCachedGeometry().GetLocalSize().Y;
	float entryHeight = _listView->GetDisplayedEntryWidgets()[0]->GetCachedGeometry().GetLocalSize().Y;
	float displayedCount = listViewHeight / entryHeight;

	int32 bottomIndex = FMath::FloorToInt(InOffsetInItems + displayedCount * 1.5f);
	if (_realDataCount < bottomIndex)
	{
		if (_realDataCount < _cachedAuctionData->GetAuctionListCount())
		{
			_requestList = true;
			_cachedAuctionData->SendReqAuctionList(_auctionIdList.Last(), true);
		}
	}
}

void UGsUIGuildAuction::OnClickComboMenuCategory()
{
	_comboBid->Close();
}

void UGsUIGuildAuction::OnClickComboMenuBid()
{
	_comboItemCategory->Close();
}

void UGsUIGuildAuction::OnSelectComboBoxCategory(int32 InMainIndex, int32 InSubIndex)
{
	if (nullptr == _cachedAuctionData)
		return;

	_cachedAuctionData->AddOrUpdateFilterCategory(EGsGuildAuctionFilterType::AUCTION_CATEGORY, InMainIndex, InSubIndex);

	InvalidateList();
}

void UGsUIGuildAuction::OnSelectComboBoxBid(int32 InIndex)
{
	if (nullptr == _cachedAuctionData)
		return;

	if (0 == InIndex)
	{
		_cachedAuctionData->RemoveFilter(EGsGuildAuctionFilterType::AUCTION_CAN_BID);
	}
	else
	{
		_cachedAuctionData->AddFilter(EGsGuildAuctionFilterType::AUCTION_CAN_BID);
	}

	InvalidateList();
}

void UGsUIGuildAuction::SetBidPrice(Currency InPrice)
{
	FString strNumber = FString::Printf(TEXT("%llu"), InPrice);
	_inputBidPrice->SetText(FText::FromString(strNumber));
	_bidPrice = InPrice;

	if (_cachedAuctionData)
	{
		_cachedAuctionData->SetNumpadBidPrice(InPrice);
	}
}

void UGsUIGuildAuction::SetBidPriceWithNumpad(Currency InPrice)
{
	SetBidPrice(InPrice);
	_uiNumpad->SetNumber(InPrice, false);
}

Currency UGsUIGuildAuction::GetPriceFromText(const FText& InText)
{
	const int32 DEFAULT_PRICE = 0;

	FString strTemp = InText.ToString();
	strTemp = strTemp.Replace(TEXT(","), TEXT(""));
	strTemp = strTemp.Replace(TEXT("+"), TEXT(""));
	FString strBidPrice = strTemp.Replace(TEXT("-"), TEXT(""));

	if (strBidPrice.IsEmpty())
	{
		return 0;
	}

	if (false == strBidPrice.IsNumeric())
	{
		return _bidPrice;
	}

	// 최대 _maxAuctionPrice 까지만 입력 가능하다.
	int32 length = strBidPrice.Len();
	FString maxPriceLength = FString::FromInt(_maxAuctionPrice);
	if (length > maxPriceLength.Len())
	{
		strBidPrice = maxPriceLength;
	}

	// 보유한 다이아 금액까지만 입력 가능하다.
	Currency changedBidPrice = FCString::Atoi(*strBidPrice);
	Currency myDiaAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::DIA);
	if (changedBidPrice > myDiaAmount)
	{
		changedBidPrice = myDiaAmount;
	}

	return (changedBidPrice < 0) ? 0 : (changedBidPrice < _maxAuctionPrice) ? changedBidPrice : _maxAuctionPrice;
}

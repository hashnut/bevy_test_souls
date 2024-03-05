// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildCalculate.h"

#include "Components/WidgetSwitcher.h"
#include "Components/ListView.h"

#include "Management/ScopeGlobal/GsGuildManager.h"

#include "UI/UIContent/Window/Guild/Auction/GsGuildDivisionListItem.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildStorageComboBox.h"

#include "T1Project.h"


void UGsUIGuildCalculate::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_listView->OnListViewScrolled().AddUObject(this, &UGsUIGuildCalculate::OnListViewScrolled);

	_comboParticipant->OnClickComboMenu.BindUObject(this, &UGsUIGuildCalculate::OnClickComboMenuParticipant);
	_comboParticipant->OnSelectComboBoxMain.BindUObject(this, &UGsUIGuildCalculate::OnSelectComboBoxParticipant);
	_comboItemCategory->OnClickComboMenu.BindUObject(this, &UGsUIGuildCalculate::OnClickComboMenuCategory);
	_comboItemCategory->OnSelectComboBoxSub.BindUObject(this, &UGsUIGuildCalculate::OnSelectComboBoxCategory);
	_comboBid->OnClickComboMenu.BindUObject(this, &UGsUIGuildCalculate::OnClickComboMenuBid);
	_comboBid->OnSelectComboBoxMain.BindUObject(this, &UGsUIGuildCalculate::OnSelectComboBoxBid);
}

void UGsUIGuildCalculate::NativeConstruct()
{
	Super::NativeConstruct();

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	_cachedAuctionData = auctionData;

	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Participant_Popup_Participant"), _filterParticipantText);
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Acquisition_ItemName2"), _filterCategoryText);
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Auction_Calculate_SuccessfulBid2"), _filterBidText);

	FGsGuildAuctionData::MakeComboBoxParticipant(_comboParticipant);
	auctionData->MakeComboBoxCategory(_comboItemCategory);
	FGsGuildAuctionData::MakeComboBoxSuccessBid(_comboBid);
}

void UGsUIGuildCalculate::NativeDestruct()
{
	_cachedAuctionData = nullptr;

	Super::NativeDestruct();
}

void UGsUIGuildCalculate::Show(bool InIsStackRecoverFlag)
{
	if (nullptr == _cachedAuctionData)
		return;

	_isStackRecoverFlag = InIsStackRecoverFlag;

	SendRequestPacket();
	
	if (FGsGuildManager* guildManager = GGuild())
	{
		/*레드닷*/
		_cachedAuctionData->SetPrevCheckTimeCalculate(guildManager->GetRedDotCheckData()._calculateCheckTime);
		guildManager->GetRedDotCheckData().UpdateCalculateCheckTime();
		guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::GuildCalculate);
	}

	/*스크롤 감추기 및 상단 이동*/
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_listView->ScrollToTop();

	/*필터 상태 초기화*/
	TPair<int32, int32> filterCategory = _cachedAuctionData->GetFilterCategory(EGsGuildAuctionFilterType::CALCULATE_CATEGORY);

	_comboParticipant->SetSelectedIndex(_cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::CALCULATE_PARTICIPANT) ? 1 : 0);
	_comboItemCategory->SetSelectedIndex(filterCategory.Key, filterCategory.Value);
	_comboBid->SetSelectedIndex(_cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::CALCULATE_SUCCESS_BID) ? 1 : 0);

	/*목록 갱신*/
	InvalidateList();
}

void UGsUIGuildCalculate::Hide()
{
	if (nullptr == _cachedAuctionData)
		return;

	_comboParticipant->Close();
	_comboItemCategory->Close();
	_comboBid->Close();
	//_cachedAuctionData->SetPrevCheckTimeCalculate(FGsTimeSyncUtil::GetServerNowSecond());
}

void UGsUIGuildCalculate::SendRequestPacket()
{
	if (nullptr == _cachedAuctionData)
		return;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

//#if WITH_EDITOR
//	// 에디터에서는 들어갈 때마다 정보 초기화 -> 이후 재요청 로직
//	_cachedAuctionData->ResetCalculateList();
//#endif

	// 신규 내역이 존재한다면 데이터 초기화 후 다시 요청한다.
	bool isRedDot = guildManager->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::GuildCalculate);
	if (isRedDot)
	{
		_cachedAuctionData->ResetCalculateList();
		_cachedAuctionData->SendReqCalculateList(0);
		return;
	}

	// 비어있을 경우 정보를 요청한다.
	bool isEmpty = _cachedAuctionData->GetCalculateList().Num() <= 0;
	if (isEmpty)
	{
		_cachedAuctionData->SendReqCalculateList(0);
		return;
	}
}

void UGsUIGuildCalculate::InvalidateList()
{
	_requestList = false;

	if (nullptr == _cachedAuctionData)
		return;

	bool filterParticipant = _cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::CALCULATE_PARTICIPANT);
	bool filterSuccessBid = _cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::CALCULATE_SUCCESS_BID);
	TArray<MarketCategory> filterCategoryList = _cachedAuctionData->GetFilterCategoryList(EGsGuildAuctionFilterType::CALCULATE_CATEGORY);

	_realDataCount = 0;
	_calculateIdList.Empty();
	TArray<const FGsGuildDivisionItem*> calculateItemList;
	const TArray<FGsGuildDivisionItem>& calculateDataList = _cachedAuctionData->GetCalculateList();
	for (int32 i = 0; i < calculateDataList.Num(); ++i)
	{
		const FGsGuildDivisionItem& calculateData = calculateDataList[i];

		// 완료도 유찰도 아닌 항목은 노출시키지 않는다.
		if (calculateData.GetDivisionState() != GuildDivisionState::DONE &&
			calculateData.GetDivisionState() != GuildDivisionState::FAILED)
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("UGsUIGuildCalculate::InvalidateList ItemDBId: %llu, DivisionState: %u"), calculateData.GetItemDBId(), static_cast<uint8>(calculateData.GetDivisionState()));
#endif
			continue;
		}

		++_realDataCount;

		// 참여자 필터
		if (filterParticipant && false == calculateData.IsInParticipantList())
			continue;

		// 낙찰 필터
		if (filterSuccessBid && false == calculateData.IsMyItem())
			continue;

		// 아이템 정보 필터
		if (filterCategoryList.Num() > 0 && false == filterCategoryList.Contains(calculateData.GetMarketCategory()))
			continue;

		calculateItemList.Emplace(&calculateDataList[i]);
	}

	calculateItemList.Sort([](const FGsGuildDivisionItem& lhs, const FGsGuildDivisionItem& rhs)
		{
			return lhs.GetAuctionDoneId() > rhs.GetAuctionDoneId();
		});

	TArray<UGsGuildDivisionListItem*> calculateList;
	for (int32 i = 0; i < calculateItemList.Num(); ++i)
	{
		UGsGuildDivisionListItem* listItem = NewObject<UGsGuildDivisionListItem>(this);
		listItem->SetData(calculateItemList[i]->GetItemDBId());
		calculateList.Emplace(listItem);
		_calculateIdList.Emplace(calculateItemList[i]->GetAuctionDoneId());
	}

	const int32 CALCULATE_EXIST = 0;
	const int32 CALCULATE_NONE = 1;
	// 정산 내역이 존재한다.
	if (calculateList.Num() > 0)
	{
		_switcherList->SetActiveWidgetIndex(CALCULATE_EXIST);
		_listView->SetListItems(calculateList);

		if (_isStackRecoverFlag)
		{
			_listView->SetScrollOffset(_cachedAuctionData->GetScrollOffset());
		}
	}
	// 정산 내역이 존재하지 않는다.
	else
	{
		_switcherList->SetActiveWidgetIndex(CALCULATE_NONE);
		_listView->ClearListItems();
	}
}

void UGsUIGuildCalculate::OnListViewScrolled(float InOffsetInItems, float InDistanceRemaining)
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
		if (_realDataCount < _cachedAuctionData->GetCalculateListCount())
		{
			_requestList = true;
			_cachedAuctionData->SendReqCalculateList(_calculateIdList.Last(), true);
		}
	}
}

void UGsUIGuildCalculate::OnClickComboMenuParticipant()
{
	_comboItemCategory->Close();
	_comboBid->Close();
}

void UGsUIGuildCalculate::OnClickComboMenuCategory()
{
	_comboParticipant->Close();
	_comboBid->Close();
}

void UGsUIGuildCalculate::OnClickComboMenuBid()
{
	_comboParticipant->Close();
	_comboItemCategory->Close();
}

void UGsUIGuildCalculate::OnSelectComboBoxParticipant(int32 InIndex)
{
	if (nullptr == _cachedAuctionData)
		return;

	if (0 == InIndex)
	{
		_cachedAuctionData->RemoveFilter(EGsGuildAuctionFilterType::CALCULATE_PARTICIPANT);
	}
	else
	{
		_cachedAuctionData->AddFilter(EGsGuildAuctionFilterType::CALCULATE_PARTICIPANT);
	}
	
	InvalidateList();
}

void UGsUIGuildCalculate::OnSelectComboBoxCategory(int32 InMainIndex, int32 InSubIndex)
{
	if (nullptr == _cachedAuctionData)
		return;

	_cachedAuctionData->AddOrUpdateFilterCategory(EGsGuildAuctionFilterType::CALCULATE_CATEGORY, InMainIndex, InSubIndex);

	InvalidateList();
}

void UGsUIGuildCalculate::OnSelectComboBoxBid(int32 InIndex)
{
	if (nullptr == _cachedAuctionData)
		return;

	if (0 == InIndex)
	{
		_cachedAuctionData->RemoveFilter(EGsGuildAuctionFilterType::CALCULATE_SUCCESS_BID);
	}
	else
	{
		_cachedAuctionData->AddFilter(EGsGuildAuctionFilterType::CALCULATE_SUCCESS_BID);
	}

	InvalidateList();
}

void UGsUIGuildCalculate::OnReceiveCalculateList()
{
	InvalidateList();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDivision.h"

#include "Components/WidgetSwitcher.h"
#include "Components/ListView.h"

#include "Guild/GsGuildHelper.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

#include "UI/UIContent/Window/Guild/Auction/GsGuildDivisionListItem.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildDivisionListEntry.h"
#include "UI/UIContent/Window/Guild/Auction/GsUIGuildStorageComboBox.h"

#include "UI/UIContent/Popup/GsUIPopupGuildAuctionBegin.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionAppointment.h"
#include "UI/UIContent/Popup/GsUIPopupGuildAuctionDirect.h"

#include "UTIL/GsTimeSyncUtil.h"


void UGsUIGuildDivision::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnCheckAll)
	{
		_btnCheckAll->OnClicked.AddDynamic(this, &UGsUIGuildDivision::OnClickCheckAll);
	}

	if (_listView)
	{
		_listView->OnListViewScrolled().AddUObject(this, &UGsUIGuildDivision::OnListViewScrolled);
	}

	_comboParticipant->OnClickComboMenu.BindUObject(this, &UGsUIGuildDivision::OnClickComboMenuParticipant);
	_comboParticipant->OnSelectComboBoxMain.BindUObject(this, &UGsUIGuildDivision::OnSelectComboBoxParticipant);
	_comboItemCategory->OnClickComboMenu.BindUObject(this, &UGsUIGuildDivision::OnClickComboMenuCategory);
	_comboItemCategory->OnSelectComboBoxSub.BindUObject(this, &UGsUIGuildDivision::OnSelectComboBoxCategory);
}

void UGsUIGuildDivision::NativeConstruct()
{
	Super::NativeConstruct();

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	_cachedAuctionData = auctionData;

	FGsGuildAuctionData::MakeComboBoxParticipant(_comboParticipant);
	auctionData->MakeComboBoxCategory(_comboItemCategory);
}

void UGsUIGuildDivision::NativeDestruct()
{
	_cachedAuctionData = nullptr;

	Super::NativeDestruct();
}

void UGsUIGuildDivision::Show(bool InIsStackRecoverFlag)
{
	if (nullptr == _cachedAuctionData)
		return;

	_isStackRecoverFlag = InIsStackRecoverFlag;

	SendRequestPacket();

	if (FGsGuildManager* guildManager = GGuild())
	{
		/*레드닷*/
		_cachedAuctionData->SetPrevCheckTimeDivision(guildManager->GetRedDotCheckData()._divisionCheckTime);
		guildManager->GetRedDotCheckData().UpdateDivisionCheckTime();
		guildManager->CheckRedDotByType(FGsGuildManager::EGsGuildRedDotType::Division);
	}

	/*데이터 초기화*/
	_cachedAuctionData->SetCurrentDivisionType(GuildAuctionType::NORMAL);
	_cachedAuctionData->SetCurrentResultType(GuildAuctionResultType::NORMAL);

	_cachedAuctionData->RemoveCheckedItemAll();

	/*스크롤 감추기 및 상단 이동*/
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_listView->ScrollToTop();

	/*전체 선택/해제 체크 버튼 갱신*/
	InvalidateCheckAll();

	/*필터 상태 초기화*/
	TPair<int32, int32> filterCategory = _cachedAuctionData->GetFilterCategory(EGsGuildAuctionFilterType::DIVISION_CATEGORY);

	_comboParticipant->SetSelectedIndex(_cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::DIVISION_PARTICIPANT) ? 1 : 0);
	_comboItemCategory->SetSelectedIndex(filterCategory.Key, filterCategory.Value);

	/*목록 갱신*/
	InvalidateList();

	/*하단 갱신*/
	_uiContent->OnShowDivision();
}

void UGsUIGuildDivision::Hide()
{
	if (nullptr == _cachedAuctionData)
		return;

	//_cachedAuctionData->SetPrevCheckTimeDivision(FGsTimeSyncUtil::GetServerNowSecond());

	_cachedAuctionData->SetCurrentDivisionType(GuildAuctionType::NORMAL);
	_cachedAuctionData->SetCurrentResultType(GuildAuctionResultType::NORMAL);

	_cachedAuctionData->RemoveCheckedItemAll();

	_uiContent->OnHideDivision();

	_comboParticipant->Close();
	_comboItemCategory->Close();
}

void UGsUIGuildDivision::InvalidateTick()
{
	if (_switcherList->GetActiveWidgetIndex() == DIVISION_EXIST)
	{
		bool needRefreshList = false;
		const TArray<UUserWidget*>& entries = _listView->GetDisplayedEntryWidgets();
		for (int32 i = 0; i < entries.Num(); ++i)
		{
			UGsUIGuildDivisionListEntry* entry = Cast<UGsUIGuildDivisionListEntry>(entries[i]);
			needRefreshList |= entry->CheckAndInvalidateState();
		}

		if (needRefreshList)
		{
			GMessage()->GetGuild().SendMessage(MessageContentGuild::STORAGE_INVALIDATE_LIST, nullptr);
		}
	}
}

void UGsUIGuildDivision::SendRequestPacket()
{
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	guildManager->GetSendController().SendRequestMemberList();

//#if WITH_EDITOR
//	// 에디터에서는 들어갈 때마다 정보 초기화 -> 이후 재요청 로직
//	_cachedAuctionData->ResetDivisionList();
//#endif

	// 신규 내역이 존재한다면 데이터 초기화 후 다시 요청한다.
	bool isRedDot = guildManager->IsRedDotByType(FGsGuildManager::EGsGuildRedDotType::Division);
	if (isRedDot)
	{
		auctionData->ResetDivisionList();
		auctionData->SendReqDivisionList(0);
		return;
	}

	// 비어있을 경우 정보를 요청한다.
	bool isEmpty = _cachedAuctionData->GetDivisionList().Num() <= 0;
	if (isEmpty)
	{
		auctionData->ResetDivisionList();
		auctionData->SendReqDivisionList(0);
		return;
	}
}

void UGsUIGuildDivision::InvalidateCheckAll()
{
	if (nullptr == _cachedAuctionData)
		return;

	GuildAuctionType currentDivisionType = _cachedAuctionData->GetCurrentDivisionType();
	// 지정 경매일 경우 일괄 체크 기능을 사용할 수 없다. (딤드)
	if (currentDivisionType == GuildAuctionType::APPOINTMENT || currentDivisionType == GuildAuctionType::DIRECT_DISTRIBUTION)
	{
		_btnCheckAll->SetIsEnabled(false);
		_btnCheckAll->SetIsSelected(false);
	}
	else
	{
		_btnCheckAll->SetIsEnabled(true);
		_btnCheckAll->SetIsSelected(_cachedAuctionData->IsCheckedAllItem());
	}
}

void UGsUIGuildDivision::InvalidateList()
{
	_requestList = false;

	// 분배 목록 정보를 받기 전에는 빈 목록이 보여진다.
	// 분배 목록 정보를 다 받았을 때부터 목룍이 표시된다.
	if (nullptr == _cachedAuctionData)
		return;

	bool filterParticipant = _cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::DIVISION_PARTICIPANT);
	TArray<MarketCategory> filterCategoryList = _cachedAuctionData->GetFilterCategoryList(EGsGuildAuctionFilterType::DIVISION_CATEGORY);

	time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();

	_realDataCount = 0;
	_divisionItemDBIdList.Empty();
	const TArray<FGsGuildDivisionItem>& divisionDataList = _cachedAuctionData->GetDivisionList();
	for (int32 i = 0; i < divisionDataList.Num(); ++i)
	{
		const FGsGuildDivisionItem& divisionData = divisionDataList[i];

		// 분배가 되지 않은 보관중인 아이템만 표시한다.
		if (divisionData.GetDivisionState() != GuildDivisionState::STORAGE)
			continue;

		// 만료 예정 시각을 지났다면 노출시키지 않는다.
		if (divisionData.GetExpiredDateTime() < currentTime)
			continue;

		++_realDataCount;

		// 참여자 필터
		if (filterParticipant && false == divisionData.IsInParticipantList())
			continue;

		// 아이템 정보 필터
		if (filterCategoryList.Num() > 0 && false == filterCategoryList.Contains(divisionData.GetMarketCategory()))
			continue;

		_divisionItemDBIdList.Emplace(divisionData.GetItemDBId());
	}

	_divisionItemDBIdList.Sort([](ItemDBId lhs, ItemDBId rhs)
		{
			return lhs > rhs;
		});

	TArray<UGsGuildDivisionListItem*> divisionList;
	for (int32 i = 0; i < _divisionItemDBIdList.Num(); ++i)
	{
		UGsGuildDivisionListItem* listItem = NewObject<UGsGuildDivisionListItem>(this);
		listItem->SetData(_divisionItemDBIdList[i]);
		divisionList.Emplace(listItem);
	}

	// 획득 내역이 존재한다.
	if (divisionList.Num() > 0)
	{
		_switcherList->SetActiveWidgetIndex(DIVISION_EXIST);
		_listView->SetListItems(divisionList);
		
		if (_isStackRecoverFlag)
		{
			_listView->SetScrollOffset(_cachedAuctionData->GetScrollOffset());
		}
	}
	// 획득 내역이 존재하지 않는다.
	else
	{
		_switcherList->SetActiveWidgetIndex(DIVISION_NONE);
		_listView->ClearListItems();
	}

	if (_uiContent)
	{
		// 획득 내역 수 갱신
		_uiContent->InvalidateDivisionCount(_cachedAuctionData->GetDivisionListCount(), _cachedAuctionData->GetMaxDivisionCount());
	}

	OnInvalidateList();
}

void UGsUIGuildDivision::InvalidateListEntry()
{
	const TArray<UUserWidget*>& entries = _listView->GetDisplayedEntryWidgets();
	for (int32 i = 0; i < entries.Num(); ++i)
	{
		UGsUIGuildDivisionListEntry* entry = Cast<UGsUIGuildDivisionListEntry>(entries[i]);
		entry->InvalidateCheck();
	}
}

void UGsUIGuildDivision::OnInvalidateList()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("PopupGuildAuction"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildAuctionAppointment* popup = Cast<UGsUIPopupGuildAuctionAppointment>(widget.Get()))
		{
			popup->CheckItemValid();
		}
	}

	widget = uiManager->GetWidgetByKey(TEXT("PopupGuildNormalAuction"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildAuctionBegin* popup = Cast<UGsUIPopupGuildAuctionBegin>(widget.Get()))
		{
			popup->CheckItemValid();
		}
	}

	widget = uiManager->GetWidgetByKey(TEXT("PopupGuildAuctionDirect"));
	if (widget.IsValid())
	{
		if (UGsUIPopupGuildAuctionDirect* popup = Cast<UGsUIPopupGuildAuctionDirect>(widget.Get()))
		{
			popup->CheckItemValid();
		}
	}
}

void UGsUIGuildDivision::OnClickCheckAll()
{
	if (nullptr == _cachedAuctionData)
		return;

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	uint32 myAuthority = guildManager->GetAuthority();
	if (false == FGsGuildHelper::CanDivision(myAuthority))
	{
		_cachedAuctionData->RemoveCheckedItemAll();
		OnChangedDivisionListCheck();
		return;
	}

	// 지정 경매라면 다중 선택 불가능 & 일괄체크 비활성화 및
	GuildAuctionType currentDivisionType = _cachedAuctionData->GetCurrentDivisionType();
	if (currentDivisionType == GuildAuctionType::APPOINTMENT || currentDivisionType == GuildAuctionType::DIRECT_DISTRIBUTION)
		return;

	if (_btnCheckAll->GetIsSelected())
	{
		_cachedAuctionData->RemoveCheckedItemAll();
	}
	else
	{
		bool filterParticipant = _cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::DIVISION_PARTICIPANT);
		TArray<MarketCategory> filterCategoryList = _cachedAuctionData->GetFilterCategoryList(EGsGuildAuctionFilterType::DIVISION_CATEGORY);
		
		if (filterParticipant || 0 < filterCategoryList.Num())
		{
			// 필터가 있으면
			for (ItemDBId displayedItemDBId : _divisionItemDBIdList)
			{
				_cachedAuctionData->AddCheckedItem(displayedItemDBId);
			}
		}
		else
		{
			// 필터가 없으면
			_cachedAuctionData->AddCheckedItemAll();
		}
	}

	OnChangedDivisionListCheck();
}

void UGsUIGuildDivision::OnListViewScrolled(float InOffsetInItems, float InDistanceRemaining)
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
		if (_realDataCount < _cachedAuctionData->GetDivisionListCount())
		{
			_requestList = true;
			_cachedAuctionData->SendReqDivisionList(_divisionItemDBIdList.Last(), true);
		}
	}
}

void UGsUIGuildDivision::OnClickComboMenuParticipant()
{
	_comboItemCategory->Close();
}

void UGsUIGuildDivision::OnClickComboMenuCategory()
{
	_comboParticipant->Close();
}

void UGsUIGuildDivision::OnSelectComboBoxParticipant(int32 InIndex)
{
	if (nullptr == _cachedAuctionData)
		return;

	bool isChanged = false;
	if (0 == InIndex)
	{
		if (_cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::DIVISION_PARTICIPANT))
		{
			_cachedAuctionData->RemoveFilter(EGsGuildAuctionFilterType::DIVISION_PARTICIPANT);
			isChanged = true;
		}
	}
	else
	{
		if (false == _cachedAuctionData->GetFilter(EGsGuildAuctionFilterType::DIVISION_PARTICIPANT))
		{
			_cachedAuctionData->AddFilter(EGsGuildAuctionFilterType::DIVISION_PARTICIPANT);
			isChanged = true;
		}
	}

	if (isChanged)
	{
		_cachedAuctionData->RemoveCheckedItemAll();
		InvalidateList();
		OnChangedDivisionListCheck();
	}
}

void UGsUIGuildDivision::OnSelectComboBoxCategory(int32 InMainIndex, int32 InSubIndex)
{
	if (nullptr == _cachedAuctionData)
		return;

	TArray<MarketCategory> prevFilterCategoryList = _cachedAuctionData->GetFilterCategoryList(EGsGuildAuctionFilterType::DIVISION_CATEGORY);
	bool prevFiltered = prevFilterCategoryList.Num() > 0;

	TPair<int32, int32> prevCategoryIndex = _cachedAuctionData->GetFilterCategory(EGsGuildAuctionFilterType::DIVISION_CATEGORY);

	_cachedAuctionData->AddOrUpdateFilterCategory(EGsGuildAuctionFilterType::DIVISION_CATEGORY, InMainIndex, InSubIndex);

	TArray<MarketCategory> currFilterCategoryList = _cachedAuctionData->GetFilterCategoryList(EGsGuildAuctionFilterType::DIVISION_CATEGORY);
	bool currFiltered = currFilterCategoryList.Num() > 0;

	if (!(prevCategoryIndex.Key == InMainIndex && prevCategoryIndex.Value == InSubIndex) ||	prevFiltered != currFiltered)
	{
		_cachedAuctionData->RemoveCheckedItemAll();
		InvalidateList();
		OnChangedDivisionListCheck();
	}
}

void UGsUIGuildDivision::OnNotifyJoinMember()
{
	// 해당 멤버의 정보를 얻기 위해 리스트 요청
	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	guildManager->GetSendController().ResetCoolTime(FGsGuildSendCoolTimeController::EGsCoolTimeType::MemberList);
	guildManager->GetSendController().SendRequestMemberList();
}

void UGsUIGuildDivision::OnReceiveDivisionList()
{
	InvalidateList();
	InvalidateCheckAll();
}

void UGsUIGuildDivision::OnChangedDivisionListCheck()
{
	InvalidateCheckAll();
	InvalidateListEntry();
	_uiContent->InvalidateDivisionButton();
}

void UGsUIGuildDivision::OnUpdatedDivisionItem()
{
	InvalidateList();
	OnChangedDivisionListCheck();

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	uiManager->CloseByKeyName(TEXT("PopupGuildAuctionCalculate"));
	uiManager->CloseByKeyName(TEXT("PopupGuildDivisionJoin"));
	uiManager->CloseByKeyName(TEXT("PopupGuildNormalAuction"));
	uiManager->CloseByKeyName(TEXT("PopupGuildAuction"));
	uiManager->CloseByKeyName(TEXT("PopupGuildAuctionDirect"));
}

void UGsUIGuildDivision::OnChangedComboBox()
{
	OnChangedDivisionListCheck();
	//InvalidateList();
}

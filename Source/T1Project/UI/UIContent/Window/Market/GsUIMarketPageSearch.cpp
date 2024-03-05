// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIMarketPageSearch.h"
#include "Components/ListView.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/Market/GsUIMarketCategoryListEntry.h"
#include "UI/UIContent/Window/Market/GsUIMarketSubCategoryListEntry.h"
#include "UI/UIContent/Window/Market/GsUIMarketItemListEntryBase.h"
#include "UI/UIContent/Window/Market/List/GsMarketItemGroupListItem.h"
#include "UI/UIContent/Window/Market/List/GsMarketItemListItem.h"
#include "UI/UIContent/Window/Market/List/GsMarketListViewControlItem.h"
#include "UI/UIContent/Window/Market/List/GsMarketListViewControlItemGroup.h"
#include "UI/UIContent/Popup/GsUIPopupMarketPurchase.h"
#include "UI/UIContent/Popup/GsUIPopupMarketMultiplePurchase.h"
#include "UI/UIContent/Tray/GsUITrayMarketFilter.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsMarketManager.h"
#include "Net/GsNetSendServiceMarket.h"
#include "DataSchema/Market/GsSchemaMarketConfig.h"
#include "DataSchema/Market/GsSchemaMarketCategoryData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Market/GsMarketHelper.h"
#include "UTIL/GsTableUtil.h"

const FName PopupPurchaseName(TEXT("PopupMarketPurchase"));
const FName PopupMultiplePurchaseName(TEXT("PopupMarketMultiplePurchase"));
const FName PopupMultiplePurchaseResultName(TEXT("PopupMarketMultiplePurchaseResult"));

//----------------------------------------------------------------------------------------------------------------------
void UGsUIMarketPageSearch::FGsMarketCategoryData::InitData()
{
	_tableData = GetTableData();

	if (_tableData && _slot)
	{
		_slot->SetUIName(_tableData->categoryText);
		_slot->SetUISubName(FText::GetEmpty());
	}
}

void UGsUIMarketPageSearch::FGsMarketCategoryData::SetSubCategory(int32 InIndex)
{
	if (_slot)
	{
		if (const FGsSchemaMarketSubCategoryData* subData = GetSubCategoryData(InIndex))
		{
			_slot->SetUISubName(subData->marketCategoryText);
		}
		else
		{
			_slot->SetUISubName(FText::GetEmpty());
		}
	}
}

bool UGsUIMarketPageSearch::FGsMarketCategoryData::IsSubCategory() const
{
	if (_tableData)
	{
		return (0 < _tableData->subCategoryList.Num()) ? true : false;
	}

	return false;
}

const FGsSchemaMarketCategoryData* UGsUIMarketPageSearch::FGsMarketCategoryData::GetTableData() const
{
	return UGsTableUtil::FindRowById<UGsTableMarketCategoryData, FGsSchemaMarketCategoryData>(_uiCategory);
}

const FGsSchemaMarketSubCategoryData* UGsUIMarketPageSearch::FGsMarketCategoryData::GetSubCategoryData(int32 InSubIndex) const
{
	if (_tableData)
	{
		if (_tableData->subCategoryList.IsValidIndex(InSubIndex))
		{
			return &_tableData->subCategoryList[InSubIndex];
		}
	}

	return nullptr;
}
//----------------------------------------------------------------------------------------------------------------------
void UGsUIMarketPageSearch::BeginDestroy()
{
	if (_slotHelperSubCategory)
	{
		_slotHelperSubCategory->OnRefreshEntry.RemoveDynamic(this, &UGsUIMarketPageSearch::OnRefreshEntrySubCategory);
	}
	_slotHelperSubCategory = nullptr;

	for (int32 i = 0; i < _listViewControlList.Num(); ++i)
	{
		delete _listViewControlList[i];
	}
	_listViewControlList.Empty();

	if (_poolItemGroup)
	{
		_poolItemGroup->RemovePool();
		_poolItemGroup = nullptr;
	}

	if (_poolItem)
	{
		_poolItem->RemovePool();
		_poolItem = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIMarketPageSearch::NativeOnInitialized()
{
	Super::NativeOnInitialized();	

	// 카테고리 설정
	_slotHelperCategory.Initialize(_panelCategory);

	_toggleGroupCategory.OnSlotClicked.BindUObject(this, &UGsUIMarketPageSearch::OnClickCategory);
	_toggleGroupCategory.AddToggleWidgetByParentPanel(_panelCategory);	
	
	// 서브 카테고리 설정
	_slotHelperSubCategory = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperSubCategory->Initialize(_subCategoryEntryClass, _panelSubCategory);
	_slotHelperSubCategory->OnRefreshEntry.AddDynamic(this, &UGsUIMarketPageSearch::OnRefreshEntrySubCategory);

	_toggleGroupSubCategory.OnSlotClicked.BindUObject(this, &UGsUIMarketPageSearch::OnClickSubCategory);

	// 리스트뷰 설정. 정해진 페이지의 반 값을 offset으로 사용한다
	int32 listItemCountPerPage = 10;
	if (const FGsSchemaMarketConfig* config = FGsMarketHelper::GetMarketConfig())
	{
		listItemCountPerPage = IsWorldMarket() ? config->wdListItemCountPerPage : config->listItemCountPerPage;
	}
	// 4분의1 지점쯤 스크롤 했을 때 다음 정보 요청
	int32 offsetIndex = FMath::RoundToInt(static_cast<float>(listItemCountPerPage) * 0.75);

	// ListItem Pool
	_poolItemGroup = NewObject<UGsPoolUObject>(this); // UGsMarketItemGroupListItem
	_poolItem = NewObject<UGsPoolUObject>(this); // UGsMarketItemListItem

	_listViewControlList.Emplace(new FGsMarketListViewControlItemGroup(_listViewItemGroup, offsetIndex, _poolItemGroup));
	_listViewControlList.Emplace(new FGsMarketListViewControlItem(_listViewInstackableItem, offsetIndex, _poolItem));
	_listViewControlList.Emplace(new FGsMarketListViewControlItem(_listViewStackableItem, offsetIndex, _poolItem));
	
	_listViewItemGroup->OnItemClicked().AddUObject(this, &UGsUIMarketPageSearch::OnClickListViewItemGroup);
	_listViewInstackableItem->OnItemClicked().AddUObject(this, &UGsUIMarketPageSearch::OnClickListViewItem);
	_listViewStackableItem->OnItemClicked().AddUObject(this, &UGsUIMarketPageSearch::OnClickListViewItem);

	_listViewItemGroup->OnListViewScrolled().AddUObject(this, &UGsUIMarketPageSearch::OnScrollListViewItemGroup);
	_listViewInstackableItem->OnListViewScrolled().AddUObject(this, &UGsUIMarketPageSearch::OnScrollListViewItem);
	_listViewStackableItem->OnListViewScrolled().AddUObject(this, &UGsUIMarketPageSearch::OnScrollListViewItem);

	// 소팅 버튼 설정
	_sBtnSortByPriceInstackable->OnClicked.AddDynamic(this, &UGsUIMarketPageSearch::OnClickSortByPriceInstackable);
	_sBtnSortByPricePerUnit->OnClicked.AddDynamic(this, &UGsUIMarketPageSearch::OnClickSortByPricePerUnit);
	_sBtnSortByPriceStackable->OnClicked.AddDynamic(this, &UGsUIMarketPageSearch::OnClickSortByPriceStackable);

	// 검색 버튼쪽 설정
	_btnPrev->OnClicked.AddDynamic(this, &UGsUIMarketPageSearch::OnClickPrev);
	_btnFilter->OnClicked.AddDynamic(this, &UGsUIMarketPageSearch::OnClickFilter);
	_btnSearch->OnClicked.AddDynamic(this, &UGsUIMarketPageSearch::OnClickSearch);
	_btnReset->OnClicked.AddDynamic(this, &UGsUIMarketPageSearch::OnClickReset);
	_btnRefresh->OnClicked.AddDynamic(this, &UGsUIMarketPageSearch::OnClickRefresh);
	_btnSelectTopItems->OnClicked.AddDynamic(this, &UGsUIMarketPageSearch::OnClickSelectTopItems);
	_btnMultiplePurchase->OnClicked.AddDynamic(this, &UGsUIMarketPageSearch::OnClickMultiplePurchase);

	_checkCooltime.SetIntervalSecond(0.5f);

	InitializeCategoryData();
}

void UGsUIMarketPageSearch::NativeConstruct()
{
	Super::NativeConstruct();

	// 포인터가 깨질 수도 있으니 창 열때마다 새로 넣어준다
	for (TPair<EGsMarketUICategory, FGsMarketCategoryData>& itPair : _categoryDataMap)
	{
		itPair.Value.InitData();
	}

	// 정렬버튼 리셋
	_sBtnSortByPriceInstackable->SetSwitcherIndex(0);
	_sBtnSortByPricePerUnit->SetSwitcherIndex(0);
	_sBtnSortByPriceStackable->SetSwitcherIndex(0);

	// 기본값 세팅
	SetCategoryInter(EGsMarketUICategory::BOOKMARK);
	SetVisibleSubCategory(false);
	InvalidateTab();
	// InvalidateList는 Show 시 수행

	_bIsFirstOpen = true;
}

void UGsUIMarketPageSearch::NativeDestruct()
{
	_bIsFirstOpen = false;
	_prevClickedCategory = EGsMarketUICategory::NONE;

	if (FGsMarketManager* marketMgr = GSMarket())
	{
		// 다중 구매 리스트 리셋
		marketMgr->ClearMultiplePurchaseList();
	}

	Super::NativeDestruct();
}

void UGsUIMarketPageSearch::Show(bool bIsWorldMarket)
{
	if (IsWorldMarket() != bIsWorldMarket)
	{
		if (EGsMarketUICategory::NONE == _currCategory ||
			EGsMarketUICategory::BOOKMARK == _currCategory)
		{
			_bIsFirstOpen = true;
		}
	}

	// 여기서 월드마켓 여부를 저장하므로 주의
	Super::Show(bIsWorldMarket);

	FGsMarketManager* marketMgr = GSMarket();

	// 다중 구매 리스트 리셋
	marketMgr->ClearMultiplePurchaseList();

	// 뎁스나 아이템 선택 상태들은 리셋 시킨다
	SetCategoryInter(_currCategory, _currSubCategoryIndex);	

	// 북마크 정보는 다 받아야 함
	marketMgr->SendReqBookmarkList(IsWorldMarket());

	FString strSearch = marketMgr->GetSearchString(IsWorldMarket());
	if (strSearch.IsEmpty())
	{
		if (_bIsFirstOpen)
		{
			SetCategoryInter(EGsMarketUICategory::BOOKMARK);
		}
		else
		{
			marketMgr->SendReqItemGroupList(IsWorldMarket(), GetMarketCategory(_currCategory, _currSubCategoryIndex), true);
		}
	}
	else
	{
		_bIsFirstOpen = false;

		ItemId shortcutItemId = marketMgr->GetShortcutItemId();
		if (INVALID_ITEM_ID != shortcutItemId)
		{
			SetCategoryInter(EGsMarketUICategory::ALL);
		}
	}

	SetVisibleSubCategory(false);
	InvalidateTab();
	InvalidateList(true);
	SetUISearchString(strSearch);
}

void UGsUIMarketPageSearch::OnUpdateItemList()
{
	InvalidateList(false);
}

void UGsUIMarketPageSearch::OnUpdateBookmark(ItemId InItemId)
{
	FGsMarketManager* marketMgr = GSMarket();
	FGsMarketControllerBase* controller = marketMgr->GetController(IsWorldMarket());
	if (nullptr == controller)
	{
		return;
	}

	if (IsBookmarkCategory())
	{
		if (FGsMarketListViewControlBase* listViewControl = GetListViewControl(_switcherList->GetActiveWidgetIndex()))
		{
			if (controller->IsBookmarkedItem(InItemId))
			{
				if (const FGsMarketItemGroupData* itemGroupData = controller->GetItemGroupData(InItemId, true))
				{
					UGsMarketItemGroupListItem* listItem = _poolItemGroup->Claim<UGsMarketItemGroupListItem>();
					listItem->SetData(IsWorldMarket(), itemGroupData);

					listViewControl->AddData(listItem);
				}
			}
			else
			{
				listViewControl->RemoveData(InItemId);
			}

			listViewControl->RefreshAll();
			SetVisibleEmptyList(listViewControl->IsEmptyList());
		}
		
		return;
	}	
	
	if (EMarketListDepth::FIRST == _currDepth)
	{
		// 하나만 갱신하고 싶어서 만들긴 했는데, 안될지도 모름.
		if (const FGsMarketItemGroupData* itemGroupData = controller->GetItemGroupData(InItemId, false))
		{
			if (FGsMarketListViewControlBase* listViewControl = GetListViewControl(_switcherList->GetActiveWidgetIndex()))
			{
				int32 index = listViewControl->FindDataIndex(InItemId);
				if (0 <= index)
				{
					UGsMarketItemGroupListItem* listItem = _poolItemGroup->Claim<UGsMarketItemGroupListItem>();
					listItem->SetData(IsWorldMarket(), itemGroupData);

					listViewControl->ChangeData(index, listItem);
				}

				listViewControl->RefreshAll();
				SetVisibleEmptyList(listViewControl->IsEmptyList());
			}
		}
	}
	else if (EMarketListDepth::SECOND == _currDepth)
	{
		InvalidateList(false);
	}
}

void UGsUIMarketPageSearch::OnRcvBuySuccess(MarketTransactionId InTransactionId)
{
	InvalidateList(false);
}

void UGsUIMarketPageSearch::OnRcvBuyFail()
{
	InvalidateList(false);
}

void UGsUIMarketPageSearch::OnUpdateSearch(const FString& InStr)
{
	// 검색창에서 스트링 입력완료 시 들어오는 부분	
	// SendSearchedItemList 함수에서 비어있으면 패킷을 보내지 않음
	GSMarket()->SendSearchedItemList(IsWorldMarket(), InStr, _currCategory, _currSubCategoryIndex);

	if (InStr.IsEmpty())
	{
		// 패킷을 안보내 갱신되지 않으므로 강제 갱신
		InvalidateTab();
		InvalidateList(true);
		SetUISearchString(InStr);
	}
	else
	{
		// 검색은 1뎁스에서만 일어나므로 강제로 1뎁스로 보냄(기본값)
		SetCategoryInter(_currCategory, _currSubCategoryIndex/*, INVALID_ITEM_ID, 0, EMarketListDepth::FIRST*/);

		// 리스트는 패킷 응답 받고 갱신
	}
}

void UGsUIMarketPageSearch::OnUpdateFilter(bool bIsGradeChanged)
{
	// 필터 창에서 필터 입력 완료시 들어옴
	FGsMarketManager* marketMgr = GSMarket();
	if (nullptr == marketMgr)
	{
		return;
	}

	// 다중 구매 체크 리셋
	marketMgr->ClearMultiplePurchaseList();

	FGsMarketFilterData& filter = marketMgr->GetFilterData();

	// ItemGrade에 변화가 있으면 서버에 보내서 갱신해야 함.
	if (bIsGradeChanged)
	{
		// 즐겨찾기가 아니고, 1뎁스 정보일 때만 페이지 단위로 서버에 정보 요청
		if (false == IsBookmarkCategory() &&
			EMarketListDepth::FIRST == _currDepth)
		{
			GSMarket()->SendReqItemGroupList(IsWorldMarket(), GetMarketCategory(_currCategory, _currSubCategoryIndex), true);

			return;
		}
	}

	InvalidateList(true);
}

void UGsUIMarketPageSearch::OnRcvSearchResult()
{
	// 검색 결과를 받음
	FGsMarketManager* marketMgr = GSMarket();

	// 다중 구매 체크 리셋
	marketMgr->ClearMultiplePurchaseList();

	const TPair<ItemId, bool>& serchInfo = marketMgr->GetSearchItemInfo();
	if (INVALID_ITEM_ID != serchInfo.Key)
	{
		SetCategoryInter(EGsMarketUICategory::ALL);
	}

	InvalidateTab();
	InvalidateList(true);
	SetUISearchString(marketMgr->GetSearchString(IsWorldMarket()));
}

void UGsUIMarketPageSearch::OnReqRefresh()
{	
	SendRefresh();
}

void UGsUIMarketPageSearch::OnRcvBookmarkList()
{
	// 처음 열었는데
	if (_bIsFirstOpen)
	{
		FGsMarketControllerBase* controller = GSMarket()->GetController(IsWorldMarket());
		if (nullptr == controller)
		{
			return;
		}

		// 북마크 정보가 없다면 ALL탭으로 변경
		if (false == controller->IsBookmark())
		{			
			SetCategoryInter(EGsMarketUICategory::ALL);			
			GSMarket()->SendReqItemGroupList(IsWorldMarket(), GetMarketCategory(_currCategory, _currSubCategoryIndex), true);
			InvalidateTab();
		}

		InvalidateList(true);
	}
	else
	{
		// 처음 연 경우가 아닐 경우 북마크 탭일 때만 갱신
		if (IsBookmarkCategory())
		{
			InvalidateList(false);
		}
	}

	// 북마크 정보 받으면 무조건 꺼준다
	_bIsFirstOpen = false;
}

void UGsUIMarketPageSearch::OnAddToPurchaseList(MarketTransactionId InTransactionId)
{
	if (INVALID_MARKET_TRANSACTION_ID == InTransactionId)
	{
		return;
	}

	// 이미 추가되어있는 경우 false
	bool bIsSuccess = GSMarket()->AddMultiplePurchaseList(IsWorldMarket(), InTransactionId);
	if (bIsSuccess)
	{
		InvalidateMultiplePurchaseInfo();
	}
}

void UGsUIMarketPageSearch::OnRemoveFromPurchaseList(MarketTransactionId InTransactionId)
{
	if (INVALID_MARKET_TRANSACTION_ID == InTransactionId)
	{
		return;
	}

	// 없을 경우 false
	bool bIsSuccess = GSMarket()->RemoveMultiplePurchaseList(IsWorldMarket(), InTransactionId);
	if (bIsSuccess)
	{
		InvalidateMultiplePurchaseInfo();
	}
}

void UGsUIMarketPageSearch::InitializeCategoryData()
{
	const UGsTableMarketCategoryData* table = Cast<const UGsTableMarketCategoryData>(
		FGsSchemaMarketCategoryData::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}
	
	_categoryDataMap.Empty();
	
	int32 count = _panelCategory->GetChildrenCount();
	for (int32 i = 0; i < count; ++i)
	{
		UGsUIMarketCategoryListEntry* slot = Cast<UGsUIMarketCategoryListEntry>(_panelCategory->GetChildAt(i));
		if (nullptr == slot)
		{
			continue;
		}

		// EGsMarketUICategory 의 0값은 NONE 이므로, 1을 더함
		EGsMarketUICategory category = ConvertIndexToCategory(i);
		_categoryDataMap.Emplace(category, FGsMarketCategoryData(i, slot));
	}
}

void UGsUIMarketPageSearch::SetCategory(EGsMarketUICategory InCategory)
{
	SetCategoryInter(InCategory);
	SetVisibleSubCategory(false);

	InvalidateTab();
	InvalidateList(true);
}

void UGsUIMarketPageSearch::ClearSearchData()
{
	// 카테고리 바뀔 때, 탭 바뀔 때, 창 닫을 때 리셋
	FGsMarketManager* marketMgr = GSMarket();
	marketMgr->ClearSearchData();

	SetUISearchString(TEXT(""));
}

void UGsUIMarketPageSearch::SetCategoryInter(EGsMarketUICategory InCategory, int32 InSubCategoryIndex,
	ItemId InSelectedItemId, int32 InSelectedEnchantLevel, EMarketListDepth InListDepth)
{
	_currCategory = InCategory;
	_currSubCategoryIndex = InSubCategoryIndex;
	_selectedItemId = InSelectedItemId;
	_selectedEnchantLevel = InSelectedEnchantLevel;
	_currDepth = InListDepth;

	// 카테고리가 확정될 경우
	_prevClickedCategory = InCategory;
}

void UGsUIMarketPageSearch::InvalidateTab()
{
	// 메인탭
	if (FGsMarketCategoryData* categoryData = GetCategoryData(_currCategory))
	{
		_toggleGroupCategory.SetSelectedIndex(categoryData->_index);
	}

	for (TPair<EGsMarketUICategory, FGsMarketCategoryData>& itPair : _categoryDataMap)
	{
		if (_currCategory == itPair.Key)
		{
			itPair.Value.SetSubCategory(_currSubCategoryIndex);
		}
		else
		{
			// 없애기 위함
			itPair.Value.SetSubCategory(-1);
		}
	}
}

void UGsUIMarketPageSearch::InvalidateList(bool bResetScroll)
{
	ESlateVisibility prevBtnVisibility = ESlateVisibility::Visible;
	switch (_currDepth)
	{
	case UGsUIMarketPageSearch::FIRST:
		SetListDataFirstDepth(bResetScroll);
		prevBtnVisibility = ESlateVisibility::Hidden;
		break;
	case UGsUIMarketPageSearch::SECOND:
		SetListDataSecondDepth(bResetScroll);
		break;
	case UGsUIMarketPageSearch::THIRD:
		SetListDataThirdDepth(bResetScroll);
		break;
	default:
		break;
	}

	_btnPrev->SetVisibility(prevBtnVisibility);

	InvalidateMultiplePurchaseInfo();
}

void UGsUIMarketPageSearch::InvalidateMultiplePurchaseInfo()
{
	if (UGsUIMarketPageSearch::THIRD != _currDepth)
	{
		_btnSelectTopItems->SetVisibility(ESlateVisibility::Collapsed);
		_btnMultiplePurchase->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	int32 count = GSMarket()->GetMultiplePurchaseCount(IsWorldMarket());
	FText textResult;

	// TEXT: 일괄 구매 {0}/{1}
	FText textFormat;
	if (FText::FindText(TEXT("MarketText"), TEXT("UI_Market_MultiPurchase"), textFormat))
	{
		textResult = FText::Format(textFormat, count, FGsMarketHelper::GetMaxMultiplePurchaseCount());
	}
	else
	{
		// 못찾으면 그냥 갯수로 출력
		FString strNum = FString::Format(TEXT("{0}/{1}"), { count, FGsMarketHelper::GetMaxMultiplePurchaseCount() });
		textResult = FText::FromString(strNum);
	}
	
	_textBlockMultiplePurchase->SetText(textResult);

	_btnSelectTopItems->SetVisibility(ESlateVisibility::Visible);
	_btnMultiplePurchase->SetVisibility(ESlateVisibility::Visible);
	_btnMultiplePurchase->SetIsEnabled((0 < count) ? true : false);
}

void UGsUIMarketPageSearch::SetListDataFirstDepth(bool bResetScroll)
{
	FGsMarketManager* marketMgr = GSMarket();
	FGsMarketControllerBase* controller = marketMgr->GetController(IsWorldMarket());
	if (nullptr == controller)
	{
		return;
	}

	// 다중 구매 리스트 리셋
	marketMgr->ClearMultiplePurchaseList();

	// 순서에 유의(GetCurrentListViewControl 보다 먼저 수행)
	_switcherList->SetActiveWidgetIndex(EMarketListType::ITEM_GROUP);	

	// 데이터 리스트 얻어오기(필터링, 정렬 끝난 값이 리턴되므로 추가 작업 필요없음)
	TArray<const FGsMarketItemGroupData*> itemGroupList;
	controller->GetItemGroupListByCategory(true, _currCategory, _currSubCategoryIndex, itemGroupList);
	int32 lastRcvIndex = controller->GetServerLastRcvIndex();

	// 리스트 담기
	FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex());
	if (listControl)
	{
		// 데이터 리셋, 풀에 반납
		listControl->ClearData();

		for (const FGsMarketItemGroupData* iter : itemGroupList)
		{
			UGsMarketItemGroupListItem* listItem = _poolItemGroup->Claim<UGsMarketItemGroupListItem>();
			listItem->SetData(IsWorldMarket(), iter);

			listControl->SetData(listItem);
		}		

		// 해당 스크롤 위치까지 다 받았는지 검사
		if (false == listControl->UpdateList(bResetScroll, lastRcvIndex))
		{
			if (false == IsBookmarkCategory())
			{
				// 리스트를 다 받지 않았으면 요청 보냄
				if (false == controller->IsItemGroupListComplete())
				{
					marketMgr->SendReqItemGroupList(IsWorldMarket(), GetMarketCategory(_currCategory, _currSubCategoryIndex), false);
				}
			}
		}

		listControl->RefreshAll();

		SetVisibleEmptyList(listControl->IsEmptyList());
	}
}

void UGsUIMarketPageSearch::SetListDataSecondDepth(bool bResetScroll)
{
	_switcherList->SetActiveWidgetIndex(EMarketListType::ITEM_GROUP);

	FGsMarketManager* marketMgr = GSMarket();
	FGsMarketControllerBase* controller = marketMgr->GetController(IsWorldMarket());
	if (nullptr == controller)
	{
		return;
	}

	// 다중 구매 리스트 리셋
	marketMgr->ClearMultiplePurchaseList();

	const FGsMarketItemGroupData* itemGroup = controller->GetItemGroupData(_selectedItemId, IsBookmarkCategory());
	if (nullptr == itemGroup)
	{
		return;
	}

	FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex());
	if (listControl)
	{
		// 데이터 리셋, 풀에 반납
		listControl->ClearData();
		
		// 필터링. 2뎁스는 ItemGrade와 EnchantLevel을 검사한다
		FGsMarketFilterData& filter = marketMgr->GetFilterData();
		if (filter.CheckByGrade(itemGroup->_table->grade))
		{
			for (const TPair<int32, TArray<FGsMarketItemData*>>& itPair : itemGroup->_itemLevelMap)
			{
				if (filter.CheckByLevel(itPair.Key))
				{
					UGsMarketItemGroupListItem* listItem = _poolItemGroup->Claim<UGsMarketItemGroupListItem>();
					listItem->SetData(IsWorldMarket(), itemGroup, true, itPair.Key);

					listControl->SetData(listItem);
				}
			}
		}

		// 강화도 높은순으로 정렬
		listControl->SortByEnchantLevel();
		listControl->UpdateList(bResetScroll);
		listControl->RefreshAll();

		SetVisibleEmptyList(listControl->IsEmptyList());
	}
}
void UGsUIMarketPageSearch::SetListDataThirdDepth(bool bResetScroll)
{
	FGsMarketManager* marketMgr = GSMarket();
	FGsMarketControllerBase* controller = marketMgr->GetController(IsWorldMarket());
	if (nullptr == controller)
	{
		return;
	}

	const FGsMarketItemGroupData* itemGroup = controller->GetItemGroupData(_selectedItemId, IsBookmarkCategory());
	if (nullptr == itemGroup)
	{
		return;
	}

	_switcherList->SetActiveWidgetIndex((itemGroup->IsStackableItem()) ?
		EMarketListType::STACKABLE_ITEM : EMarketListType::INSTACKABLE_ITEM);

	FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex());
	if (listControl)
	{
		// 정보리셋
		listControl->ClearData();

		if (const TArray<FGsMarketItemData*>* listPtr = itemGroup->_itemLevelMap.Find(_selectedEnchantLevel))
		{
			FGsMarketFilterData& filter = marketMgr->GetFilterData();
			bool bIsFilter = filter.IsFilter();

			for (const FGsMarketItemData* iter : *listPtr)
			{
				bool bIsTarget = true;

				// 필터링
				if (bIsFilter)
				{
					bIsTarget = filter.CheckItem(iter->_item);
				}

				if (bIsTarget)
				{
					UGsMarketItemListItem* listItem = _poolItem->Claim<UGsMarketItemListItem>();
					listItem->SetData(IsWorldMarket(), iter);

					listControl->SetData(listItem);
				}
			}
		}

		// 정렬 체크
		if (itemGroup->IsStackableItem())
		{
			int32 sortIndexPerUnit = _sBtnSortByPricePerUnit->GetSwitcherIndex();
			int32 sortIndexPrice = _sBtnSortByPriceStackable->GetSwitcherIndex();

			if (0 == sortIndexPerUnit)
			{
				if (0 == sortIndexPrice)
				{
					// 별도의 정렬 없으면 기본정렬(개당 판매가격 오름차순)
					listControl->SortByPricePerUnit(false);
				}
				else
				{
					listControl->SortByPrice((2 == sortIndexPrice) ? true : false);
				}
			}
			else
			{
				listControl->SortByPricePerUnit((2 == sortIndexPerUnit) ? true : false);
			}
		}
		else
		{
			// 별도의 정렬 없으면 판매가격 오름차순 SortByPrice(false)
			int32 sortIndexPrice = _sBtnSortByPriceInstackable->GetSwitcherIndex();
			listControl->SortByPrice((2 == sortIndexPrice) ? true : false);
		}

		listControl->UpdateList(bResetScroll);
		listControl->RefreshAll();

		SetVisibleEmptyList(listControl->IsEmptyList());
	}
}

void UGsUIMarketPageSearch::OnRefreshEntrySubCategory(int32 InIndex, UWidget* InEntry)
{
	EGsMarketUICategory category = ConvertIndexToCategory(_toggleGroupCategory.GetSelectedIndex());
	FGsMarketCategoryData* categoryData = GetCategoryData(category);
	if (categoryData)
	{
		if (const FGsSchemaMarketSubCategoryData* subData = categoryData->GetSubCategoryData(InIndex))
		{
			if (UGsUIMarketSubCategoryListEntry* entry = Cast<UGsUIMarketSubCategoryListEntry>(InEntry))
			{
				entry->SetUIName(subData->marketCategoryText);
			}
		}
	}
}

void UGsUIMarketPageSearch::OnClickListViewItemGroup(UObject* InListItem)
{
	UGsMarketItemGroupListItem* listItemData = Cast<UGsMarketItemGroupListItem>(InListItem);
	if (nullptr == listItemData)
	{
		return;
	}

	UGsUIMarketItemListEntryBase* entryBase = 
		_listViewItemGroup->GetEntryWidgetFromItem<UGsUIMarketItemListEntryBase>(InListItem);
	if (entryBase)
	{
		if (entryBase->IsSlotLongPressed())
		{
			return;
		}		
	}

	// ListView는 버튼이 안먹기 때문에 별도 사운드 부착 
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->PlayPressSound();
	}

	const FGsMarketItemGroupData* itemGroupData = listItemData->_data;
	if (nullptr == itemGroupData)
	{
		return;
	}

	// 매물이 없으면 다음 뎁스로 넘기지 않음
	if (false == itemGroupData->IsSaleItemExist())
	{
		return;
	}

	// 다음 뎁스로 넘어갈 수있게 세팅
	EMarketListDepth targetDepth = EMarketListDepth::THIRD;
	
	// 1 -> 2뎁스인 경우(쌓을 수 없는 아이템, 1뎁스 상황)
	if (false == itemGroupData->IsStackableItem())
	{
		if (false == listItemData->_bIsSecondDepth)
		{
			targetDepth = EMarketListDepth::SECOND;
		}
	}

	// 2 -> 3뎁스는 리스트를 다시 받을 필요가 없으므로 제외하고 요청
	if (EMarketListDepth::SECOND != _currDepth)
	{
		bool bResetTime = IsBookmarkCategory() ? true : false;
		GSMarket()->SendReqItemList(IsWorldMarket(), itemGroupData->GetItemId(), bResetTime);
	}

	SetCategoryInter(_currCategory, _currSubCategoryIndex,
		itemGroupData->GetItemId(), listItemData->_enchantLevel, targetDepth);

	InvalidateList(true);

	// 열려있다면 닫음
	SetVisibleSubCategory(false);
}

void UGsUIMarketPageSearch::OnClickListViewItem(UObject* InListItem)
{
	if (false == _checkCooltime.IsTimeOver())
	{
		return;
	}
	_checkCooltime.UpdateTime();

	UGsMarketItemListItem* itemData = Cast<UGsMarketItemListItem>(InListItem);
	if (nullptr == itemData ||
		nullptr == itemData->_data)
	{
		return;
	}

	UGsUIMarketItemListEntryBase* entryBase =
		_listViewItemGroup->GetEntryWidgetFromItem<UGsUIMarketItemListEntryBase>(InListItem);
	if (entryBase)
	{
		if (entryBase->IsSlotLongPressed())
		{
			return;
		}
	}

	UGsUIManager* uiMgr = GUI();

	// ListView는 버튼이 안먹기 때문에 별도 사운드 부착 
	uiMgr->PlayPressSound();

	// 이미 팝업이 켜져있을 수도 있으므로 체크해본다
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->GetWidgetByKey(PopupPurchaseName);
	if (false == widget.IsValid())
	{
		widget = uiMgr->OpenAndGetWidget(PopupPurchaseName);
	}

	if (widget.IsValid())
	{
		if (UGsUIPopupMarketPurchase* popup = Cast<UGsUIPopupMarketPurchase>(widget.Get()))
		{
			popup->SetData(IsWorldMarket(), itemData->_data);
		}
	}

	// 열려있다면 닫음
	SetVisibleSubCategory(false);
}

void UGsUIMarketPageSearch::OnScrollListViewItemGroup(float InOffsetInItems, float InDistanceRemaining)
{
	if (FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex()))
	{
		bool bNeedNextList = listControl->OnScrolled(InOffsetInItems);
		if (bNeedNextList)
		{
			// 즐겨찾기가 아니고, 1뎁스 정보일 때만 페이지 단위로 서버에 정보 요청
			if (false == IsBookmarkCategory() &&
				EMarketListDepth::FIRST == _currDepth)
			{
				GSMarket()->SendReqItemGroupList(IsWorldMarket(), GetMarketCategory(_currCategory, _currSubCategoryIndex), false, true);
			}
		}
	}
}

void UGsUIMarketPageSearch::OnScrollListViewItem(float InOffsetInItems, float InDistanceRemaining)
{
	// 3뎁스 일때만 들어옴
	if (FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex()))
	{
		listControl->OnScrolled(InOffsetInItems);
	}
}

void UGsUIMarketPageSearch::SetVisibleSubCategory(bool bInShow)
{
	_panelSubCategoryRoot->SetVisibility(bInShow ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIMarketPageSearch::SetVisibleEmptyList(bool bIsEmpty)
{
	_panelEmptyList->SetVisibility(bIsEmpty ?
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIMarketPageSearch::SetUISearchString(const FString& InStr)
{
	_editableSearch->SetText(FText::FromString(InStr));
}

void UGsUIMarketPageSearch::SendRefresh()
{
	FGsMarketManager* marketMgr = GSMarket();

	if (INVALID_ITEM_ID != _selectedItemId)
	{
		bool bResetTime = IsBookmarkCategory() ? true : false;
		marketMgr->SendReqItemList(IsWorldMarket(), _selectedItemId, bResetTime);
	}
	else
	{
		// 검색어 있으면 검색 항목 정보 요청
		FString strSearch = marketMgr->GetSearchString(IsWorldMarket());
		if (strSearch.IsEmpty())
		{
			if (IsBookmarkCategory())
			{
				GSMarket()->SendReqBookmarkList(IsWorldMarket());
			}
			else
			{
				marketMgr->SendReqItemGroupList(IsWorldMarket(), GetMarketCategory(_currCategory, _currSubCategoryIndex), true);
			}
		}
		else
		{
			marketMgr->SendSearchedItemList(IsWorldMarket());
		}
	}

	// 다중 구매 체크 리셋
	marketMgr->ClearMultiplePurchaseList();
	InvalidateMultiplePurchaseInfo();

	// 스크롤 맨 위로
	if (FGsMarketListViewControlBase* listView = GetListViewControl(_switcherList->GetActiveWidgetIndex()))
	{
		listView->ScrollToTop();

		// 체크 갱신때문에 그대로 갱신한번 수행
		listView->RefreshAll();
	}
}

void UGsUIMarketPageSearch::CloseAllMarketPopups()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->CloseByKeyName(PopupPurchaseName);
		uiMgr->CloseByKeyName(PopupMultiplePurchaseName);
		uiMgr->CloseByKeyName(PopupMultiplePurchaseResultName);
	}
}

void UGsUIMarketPageSearch::OnClickCategory(int32 InIndex, bool bIsSelected)
{
	EGsMarketUICategory category = ConvertIndexToCategory(InIndex);

	// _currCategory는 현재 리스트가 표시되고있는 카테고리이므로 서브탭 때문에 확정이 아닌 상황엔 쓰지 않음
	if (category == _prevClickedCategory)
	{
		// SubCategory가 켜져있으면 끈다.
		if (_panelSubCategoryRoot->IsVisible())
		{
			SetVisibleSubCategory(false);
			return;
		}

		// 하위 탭 없는경우, 동일탭 클릭 시 리턴
		if (FGsMarketCategoryData* categoryData = GetCategoryData(category))
		{
			if (false == categoryData->IsSubCategory())
			{
				return;
			}
		}
	}

	_prevClickedCategory = category;

	// 서브 카테고리 표시
	if (FGsMarketCategoryData* categoryData = GetCategoryData(category))
	{
		if (categoryData->IsSubCategory())
		{
			// 서브 카테고리에서 SetCategoryInter 를 진행
			_slotHelperSubCategory->RefreshAll(categoryData->_tableData->subCategoryList.Num());
			_toggleGroupSubCategory.Clear();
			_toggleGroupSubCategory.AddToggleWidgetByParentPanel(_panelSubCategory);
			
			// 데이터 없는 항목은 감춤. 전체는 무조건 표시
			// 주의: AddToggleWidgetByParentPanel 전에 Collapsed하면 없는 슬롯이 되어 인덱스가 꼬이므로 그 이후에 수행함
			if (FGsMarketManager* marketMgr = GSMarket())
			{
				for (int i = 0; i < categoryData->_tableData->subCategoryList.Num(); ++i)
				{
					bool bIsExistData = false;
					if (categoryData->_tableData->subCategoryList[i].isAll)
					{
						bIsExistData = true;
					}
					else
					{
						bIsExistData = marketMgr->IsExistData(IsWorldMarket(), 
							categoryData->_tableData->subCategoryList[i].marketCategory);
					}

					if (UWidget* widget = _panelSubCategory->GetChildAt(i))
					{
						widget->SetVisibility(bIsExistData ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
					}
				}
			}

			SetVisibleSubCategory(true);
		}
		else
		{
			// 바로 변경이므로 카테고리 저장
			SetCategoryInter(category);

			ClearSearchData();

			// 주의: GetCurrentMarketCategory 때문에 SetCategoryInter 후에 부를 것
			if (IsBookmarkCategory())
			{
				GSMarket()->SendReqBookmarkList(IsWorldMarket());
			}
			else
			{
				GSMarket()->SendReqItemGroupList(IsWorldMarket(), GetMarketCategory(_currCategory, _currSubCategoryIndex), true);
			}

			// 즐겨찾기, 전체탭 처럼 서브 카테고리가 없는 경우 
			SetVisibleSubCategory(false);

			InvalidateList(true);

			// 주의: 순서에 유의. Invelidate 후 수행
			if (FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex()))
			{
				listControl->ScrollToTop();
			}

			// 이전에 열려있던 팝업 닫기
			CloseAllMarketPopups();
		}
	}
}

void UGsUIMarketPageSearch::OnClickSubCategory(int32 InIndex, bool bIsSelected)
{
	EGsMarketUICategory category = ConvertIndexToCategory(_toggleGroupCategory.GetSelectedIndex());
	bool bIsCategoryChanged = (category != _currCategory) ? true : false;
	int32 prevSubCategoryIndex = _currSubCategoryIndex;

	SetCategoryInter(category, InIndex);

	// 카테고리 하위 텍스트 표시
	if (FGsMarketCategoryData* categoryData = GetCategoryData(_currCategory))
	{
		categoryData->SetSubCategory(InIndex);
	}

	// 주의: 순서에 유의. Invelidate 후 수행
	if (FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex()))
	{
		listControl->ScrollToTop();
	}

	// 주의: GetCurrentMarketCategory 때문에 SetCategoryInter 후에 부를 것.
	if (bIsCategoryChanged ||
		prevSubCategoryIndex != _currSubCategoryIndex)
	{
		ClearSearchData();

		GSMarket()->SendReqItemGroupList(IsWorldMarket(), GetMarketCategory(_currCategory, _currSubCategoryIndex), true);
	}

	SetVisibleSubCategory(false);
	InvalidateList(true);

	// 이전에 열려있던 팝업 닫기
	CloseAllMarketPopups();
}

void UGsUIMarketPageSearch::OnClickSortByPriceInstackable()
{
	// 0: 없음, 1: 오름차순, 2: 내림차순
	// 다음 상태로 바꾸기 위해 +1 처리.
	int32 index = (_sBtnSortByPriceInstackable->GetSwitcherIndex() + 1) % 3;	
	if (0 == index)
	{
		index = 1; // 클릭 시엔 없음 상태로 돌아가지 않음 
	}
	_sBtnSortByPriceInstackable->SetSwitcherIndex(index);

	FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex());
	if (listControl)
	{
		listControl->SortByPrice((2 == index) ? true : false);
	}
}

void UGsUIMarketPageSearch::OnClickSortByPricePerUnit()
{
	// 0: 없음, 1: 오름차순, 2: 내림차순
	// 다음 상태로 바꾸기 위해 +1 처리.
	int32 index = (_sBtnSortByPricePerUnit->GetSwitcherIndex() + 1) % 3;
	if (0 == index)
	{
		index = 1; // 클릭 시엔 없음 상태로 돌아가지 않음 
	}
	_sBtnSortByPricePerUnit->SetSwitcherIndex(index);

	FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex());
	if (listControl)
	{
		listControl->SortByPricePerUnit((2 == index) ? true : false);
	}

	// 다른쪽 버튼은 원상복구
	_sBtnSortByPriceStackable->SetSwitcherIndex(0);
}

void UGsUIMarketPageSearch::OnClickSortByPriceStackable()
{
	// 0: 없음, 1: 오름차순, 2: 내림차순
	// 다음 상태로 바꾸기 위해 +1 처리.
	int32 index = (_sBtnSortByPriceStackable->GetSwitcherIndex() + 1) % 3;
	if (0 == index)
	{
		index = 1; // 클릭 시엔 없음 상태로 돌아가지 않음 
	}
	_sBtnSortByPriceStackable->SetSwitcherIndex(index);

	FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex());
	if (listControl)
	{
		listControl->SortByPrice((2 == index) ? true : false);
	}

	// 다른쪽 버튼은 원상복구
	_sBtnSortByPricePerUnit->SetSwitcherIndex(0);
}

void UGsUIMarketPageSearch::OnClickPrev()
{
	// 이전에 열려있던 팝업 닫기
	CloseAllMarketPopups();

	// 2뎁스로 가는 경우를 구분
	if (EMarketListDepth::THIRD == _currDepth)
	{
		FGsMarketControllerBase* controller = GSMarket()->GetController(IsWorldMarket());
		if (nullptr == controller)
		{
			return;
		}

		if (const FGsMarketItemGroupData* itemGroup = controller->GetItemGroupData(_selectedItemId, IsBookmarkCategory()))
		{
			if (false == itemGroup->IsStackableItem())
			{
				SetCategoryInter(_currCategory, _currSubCategoryIndex, _selectedItemId, 0, EMarketListDepth::SECOND);
				InvalidateList(false);

				return;
			}
		}
	}

	// 1뎁스로 보냄
	SetCategoryInter(_currCategory, _currSubCategoryIndex);

	InvalidateTab();
	InvalidateList(false);

	if (_bNeedRefresh)
	{
		_bNeedRefresh = false;
		SendRefresh();
	}
}

void UGsUIMarketPageSearch::OnClickFilter()
{
	// 이전에 열려있던 팝업 닫기
	CloseAllMarketPopups();

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayMarketFilter"));
	if (widget.IsValid())
	{
		if (UGsUITrayMarketFilter* trayFilter = Cast<UGsUITrayMarketFilter>(widget.Get()))
		{
			trayFilter->SetData(IsWorldMarket());
		}
	}
}

void UGsUIMarketPageSearch::OnClickSearch()
{
	// 이전에 열려있던 팝업 닫기
	CloseAllMarketPopups();

	GUI()->Open(TEXT("TrayMarketSearch"));
}

void UGsUIMarketPageSearch::OnClickReset()
{
	// 이전에 열려있던 팝업 닫기
	CloseAllMarketPopups();

	FGsMarketManager* marketMgr = GSMarket();
	marketMgr->ClearFilterData();
	marketMgr->ClearSearchData();

	SetUISearchString(marketMgr->GetSearchString(IsWorldMarket()));

	SendRefresh();

	// 앞으로 돌아올 때 리스트 다시 받아야 함
	if (EMarketListDepth::FIRST != _currDepth)
	{
		_bNeedRefresh = true;
	}
}

void UGsUIMarketPageSearch::OnClickRefresh()
{
	// 이전에 열려있던 팝업 닫기
	CloseAllMarketPopups();

	SendRefresh();
}

void UGsUIMarketPageSearch::OnClickSelectTopItems()
{
	FGsMarketManager* marketMgr = GSMarket();

	// 기존 선택 비우기
	marketMgr->ClearMultiplePurchaseList();

	// 상위 5개 얻어오기
	TArray<MarketTransactionId> idList;
	FGsMarketListViewControlBase* listControl = GetListViewControl(_switcherList->GetActiveWidgetIndex());
	if (listControl)
	{
		listControl->GetTopNTransactionIdList(FGsMarketHelper::GetMaxMultiplePurchaseCount(), idList);

		// 얻어온 값 저장
		for (int i = 0; i < idList.Num(); ++i)
		{
			marketMgr->AddMultiplePurchaseList(IsWorldMarket(), idList[i]);
		}

		// 체크 갱신차 UI 보이는 항목들 갱신
		listControl->RefreshAll();
	}

	// 하단 버튼 카운트 갱신
	InvalidateMultiplePurchaseInfo();
}

void UGsUIMarketPageSearch::OnClickMultiplePurchase()
{
	TArray<MarketTransactionId> idList = GSMarket()->GetMultiplePurchaseList(IsWorldMarket());
	if (0 == idList.Num())
	{
		return;
	}

	UGsUIManager* uiMgr = GUI();

	// 열려있던 1개짜리 구매 팝업이 있다면 제거
	uiMgr->CloseByKeyName(PopupPurchaseName);

	// 이미 팝업이 켜져있을 수도 있으므로 체크해본다
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(PopupMultiplePurchaseName);
	if (false == widget.IsValid())
	{
		widget = uiMgr->OpenAndGetWidget(PopupMultiplePurchaseName);
	}

	if (widget.IsValid())
	{
		if (UGsUIPopupMarketMultiplePurchase* popup = Cast<UGsUIPopupMarketMultiplePurchase>(widget.Get()))
		{	
			popup->SetData(IsWorldMarket(), idList);
		}
	}
}

EGsMarketUICategory UGsUIMarketPageSearch::ConvertIndexToCategory(int32 InIndex)
{
	// EGsMarketUICategory 의 0값은 NONE 이므로, 1을 더함
	return static_cast<EGsMarketUICategory>(InIndex + 1);
}

UGsUIMarketPageSearch::FGsMarketCategoryData* UGsUIMarketPageSearch::GetCategoryData(EGsMarketUICategory InCategory)
{
	return _categoryDataMap.Find(InCategory);
}

FGsMarketListViewControlBase* UGsUIMarketPageSearch::GetListViewControl(int32 InActiveIndex)
{
	if (_listViewControlList.IsValidIndex(InActiveIndex))
	{
		return _listViewControlList[InActiveIndex];
	}

	return nullptr;
}

MarketCategory UGsUIMarketPageSearch::GetMarketCategory(EGsMarketUICategory InCategory, int32 InSubCategoryIndex)
{
	if (FGsMarketCategoryData* categoryData = GetCategoryData(InCategory))
	{
		if (const FGsSchemaMarketSubCategoryData* subData = categoryData->GetSubCategoryData(InSubCategoryIndex))
		{
			return subData->marketCategory;
		}
	}

	return MarketCategory::MARKET_CATEGORY_ALL;
}

bool UGsUIMarketPageSearch::IsBookmarkCategory() const
{
	return (EGsMarketUICategory::BOOKMARK == _currCategory) ? true : false;
}

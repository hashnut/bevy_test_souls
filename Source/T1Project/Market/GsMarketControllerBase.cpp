// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketControllerBase.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Market/GsSchemaMarketCategoryData.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Management/GsMessageHolder.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Market/GsMarketHelper.h"


void FGsMarketControllerBase::Initialize()
{
	// 주의: 즐겨찾기의 경우, itemGroupMap을 같이 사용하므로, 
	// 카테고리 변경 시 즐겨찾기로 받은 최소금액, 개수 정보가 지워질 수 있다.
	// 따라서 쿨타임을 아주 짧게 광클만 불가능할 정도로 설정한다.
	_checkerBookmark.SetIntervalSecond(0.2f);
	_checkerSellList.SetIntervalSecond(5.f);
	_checkerSettlementList.SetIntervalSecond(5.f);
}

void FGsMarketControllerBase::InitializeItemGroup(const FGsSchemaItemCommon* InData)
{
	if (false == IsTradableItem(InData))
	{
		return;
	}

	FGsMarketItemGroupData* data = _poolItemGroupData.Claim();
	data->Set(InData);

	// 원본 저장
	_itemGroupList.Emplace(data); // 원본저장

	// 편의를 위해 포인터만 채운다
	_itemGroupMap.Emplace(data->_itemId, data);

	// 편의를 위해 포인터만 채운다
	MarketCategory marketCategory = SharedFunc::GetMarketCategory(InData->categorySub);
	if (DefItemGroupList* list = _itemGroupMapByCategory.Find(marketCategory))
	{
		list->Emplace(data);
	}
	else
	{
		DefItemGroupList newList;
		newList.Emplace(data);
		_itemGroupMapByCategory.Emplace(marketCategory, newList);
	}
}

void FGsMarketControllerBase::Finalize()
{
	_strSearch.Empty();
	_searchedItemGroupList.Empty();

	_itemGroupMap.Empty();
	_itemGroupMapByCategory.Empty();
	_sellItemList.Empty();
	_serverItemGroupList.Empty();
	_serverSearchedItemGroupList.Empty();
	_multiplePurchaseList.Empty();

	_itemMap.Empty();
	_itemGroupList.Empty();
	_bookmarkList.Empty();
	_bookmarkItemGroupList.Empty();
	_poolItemGroupData.RemovePool();
	_poolItemData.RemovePool();

	_settlementList.Empty();
	_settlementLogList.Empty();
	_settlementMap.Empty();
	_poolSettlementData.RemovePool();

	_lastUpdateCategory = MarketCategory::MAX;
	_lastUpdateGradeList.Empty();
	_lastUpdateItemId = INVALID_ITEM_ID;
	_lastUpdateTimeMap.Empty();
	_lastUpdateLogPair.Key = INVALID_MARKET_TRANSACTION_ID;
	_lastUpdateLogPair.Value = 0;
	_bIsItemGroupListComplete = false;
	_bIsLogListComplete = false;

	_logTotalCount = 0;
	_logTotalPrice = 0;

	_filterData.Clear();
}

void FGsMarketControllerBase::SetBookmark(ItemId InItemId, bool bIsBookmarked)
{
	if (bIsBookmarked)
	{
		if (false == _bookmarkList.Contains(InItemId))
		{
			_bookmarkList.Emplace(InItemId);
		}
	}
	else
	{
		if (_bookmarkList.Contains(InItemId))
		{
			_bookmarkList.Remove(InItemId);
		}

		// 아이템 정보보기 후 뒤로가기 시 리스트를 다시 받지 않아 데이터가 남아있는 문제가 있어 제거
		for (FGsMarketItemGroupData* itemGroup : _bookmarkItemGroupList)
		{
			if (itemGroup->GetItemId() == InItemId)
			{
				// 리스트에서 제거
				_bookmarkItemGroupList.Remove(itemGroup);

				// 정보 Clear
				itemGroup->ClearItemGroupData();
				itemGroup->ClearItemData();

				// 풀에 반납
				_poolItemGroupData.Release(itemGroup);

				break;
			}
		}
	}
}

void FGsMarketControllerBase::RemoveItem(MarketTransactionId InTransitionId)
{
	// 각종 포인터 삭제
	if (FGsMarketItemData** itemPtr = _itemMap.Find(InTransitionId))
	{
		FGsMarketItemData* item = *itemPtr;

		// 편의를 위해 포인터 저장해놨던 정보들 삭제
		if (FGsMarketItemGroupData* itemGroup = GetItemGroupDataInter(item->GetItemId()))
		{
			itemGroup->RemoveItemData(InTransitionId);
		}

		if (FGsMarketItemGroupData* bookmarkItemGroup = GetBookmarkItemGroupDataInter(item->GetItemId()))
		{
			bookmarkItemGroup->RemoveItemData(InTransitionId);
		}

		for (int32 i = 0; i < _sellItemList.Num(); ++i)
		{
			if (InTransitionId == _sellItemList[i]->_transactionId)
			{
				_sellItemList.RemoveAt(i);
				break;
			}
		}

		// 풀 반납, 원본 삭제
		item->Clear();
		_poolItemData.Release(item);

		_itemMap.Remove(InTransitionId);
	}
}

void FGsMarketControllerBase::RemoveSettlementItem(MarketTransactionId InTransitionId)
{
	// 포인터 삭제
	for (int32 i = 0; i < _settlementList.Num(); ++i)
	{
		if (InTransitionId == _settlementList[i]->_transactionId)
		{
			_settlementList.RemoveAt(i);
		}
	}

	// 풀 반납 및 원본 삭제
	if (FGsMarketSettlementData** dataPtr = _settlementMap.Find(InTransitionId))
	{
		FGsMarketSettlementData* data = *dataPtr;
		data->Clear();
		_poolSettlementData.Release(data);

		_settlementMap.Remove(InTransitionId);
	}
}

void FGsMarketControllerBase::RemoveAllSettlementItems()
{
	// 풀 반납
	for (FGsMarketSettlementData* data : _settlementList)
	{
		data->Clear();
		_poolSettlementData.Release(data);
	}

	// 모두 비우기
	_settlementList.Empty();
	_settlementMap.Empty();
}

void FGsMarketControllerBase::ClearSearchData()
{
	_strSearch.Empty();
	_searchedItemGroupList.Empty();
	_serverSearchedItemGroupList.Empty();
}

void FGsMarketControllerBase::ClearFilterData()
{
	_filterData.Clear();
}

bool FGsMarketControllerBase::AddMultiplePurchaseList(MarketTransactionId InTransactionId)
{
	if (_multiplePurchaseList.Contains(InTransactionId))
	{
		return false;
	}

	_multiplePurchaseList.Emplace(InTransactionId);

	return true;
}

bool FGsMarketControllerBase::RemoveMultiplePurchaseList(MarketTransactionId InTransactionId)
{
	if (false == _multiplePurchaseList.Contains(InTransactionId))
	{
		return false;
	}

	_multiplePurchaseList.Remove(InTransactionId);

	return true;
}

bool FGsMarketControllerBase::IsMultiplePurchaseTarget(MarketTransactionId InTransactionId) const
{
	return _multiplePurchaseList.Contains(InTransactionId);
}

void FGsMarketControllerBase::ClearMultiplePurchaseList()
{
	_multiplePurchaseList.Empty();
}

void FGsMarketControllerBase::SendReqBookmarkList()
{
	if (_checkerBookmark.IsTimeOver())
	{
		FGsNetSendServiceMarket::SendReqBookmarkList(IsWorldMarket());
		_checkerBookmark.UpdateTime();
	}
	else
	{
		GMessage()->GetMarket().SendMessage(MessageContentMarket::RCV_BOOKMARK_LIST, nullptr);
	}
}

void FGsMarketControllerBase::SendReqItemGroupList(MarketCategory InCategory, bool bInResetScroll, bool bInIsScrolled)
{
	// 스크롤 리셋일 경우 마지막 받은 itemId 리셋
	if (bInResetScroll)
	{
		_lastUpdateItemId = INVALID_ITEM_ID;
	}
	else
	{
		// 카테고리가 달라졌거나, filter의 grade 정보가 달라졌으면, 마지막 받은 itemId 리셋
		if (InCategory == _lastUpdateCategory)
		{
			bool bIsSameGrade = _filterData.IsSameGradeFilter(_lastUpdateGradeList);
			if (false == bIsSameGrade)
			{
				_lastUpdateItemId = INVALID_ITEM_ID;
			}
		}
		else
		{
			_lastUpdateItemId = INVALID_ITEM_ID;
		}
	}

	// _lastUpdateItemId 값이 있는데 빈 리스트가 온 경우엔, 완전히 비었거나 더 받을 수 없거나이다.
	if (INVALID_ITEM_ID == _lastUpdateItemId)
	{
		// 서버 부하 시 이전 리스트가 미리 보이지 않도록 먼저 지워버림 
		_serverItemGroupList.Empty();

		for (TPair<ItemId, FGsMarketItemGroupData*> itPair : _itemGroupMap)
		{
			itPair.Value->ClearItemGroupData();
		}
	}

	_lastUpdateCategory = InCategory;
	_lastUpdateGradeList.Empty();
	_lastUpdateGradeList.Append(_filterData._gradeList);

	// 스크롤 상황에서는 blockUI를 켜지 않기 위해 !bInIsScrolled 를 넘김
	FGsNetSendServiceMarket::SendReqSalesItemList(IsWorldMarket(), _lastUpdateCategory, _lastUpdateGradeList, _lastUpdateItemId,
		!bInIsScrolled);
}

void FGsMarketControllerBase::SendReqItemList(ItemId InItemId, bool bResetTime)
{
	time_t lastUpdateTime = 0;
	if (false == bResetTime)
	{
		if (time_t* timePtr = _lastUpdateTimeMap.Find(InItemId))
		{
			lastUpdateTime = *timePtr;
		}
	}

	if (FGsMarketItemGroupData* groupData = GetItemGroupDataInter(InItemId))
	{
		if (groupData->IsStackableItem())
		{
			FGsNetSendServiceMarket::SendReqTransactionListStackable(IsWorldMarket(), InItemId, lastUpdateTime);
		}
		else
		{
			FGsNetSendServiceMarket::SendReqTransactionListInstackable(IsWorldMarket(), InItemId, lastUpdateTime);
		}
	}
}

void FGsMarketControllerBase::SendReqSellItemList(bool bResetCoolTime)
{
	if (bResetCoolTime)
	{
		_checkerSellList.Reset();
	}

	if (_checkerSellList.IsTimeOver())
	{
		FGsNetSendServiceMarket::SendReqMyTransactionList(IsWorldMarket());
		_checkerSellList.UpdateTime();
	}
}

void FGsMarketControllerBase::SendReqSettlementList()
{
	if (_checkerSettlementList.IsTimeOver())
	{
		FGsNetSendServiceMarket::SendReqCalculateList(IsWorldMarket());
		_checkerSettlementList.UpdateTime();
	}
}

void FGsMarketControllerBase::SendReqLogList(bool bResetScroll, bool bInIsScrolled)
{
	if (bResetScroll)
	{
		_lastUpdateLogPair.Key = INVALID_MARKET_TRANSACTION_ID;
		_lastUpdateLogPair.Value = 0;
	}

	// 스크롤 상황에서는 BlockUI를 켜지 않기 위해 !bInIsScrolled를 넘김
	FGsNetSendServiceMarket::SendReqCalculatedList(IsWorldMarket(), _lastUpdateLogPair.Key, _lastUpdateLogPair.Value, !bInIsScrolled);
}

FString FGsMarketControllerBase::SendSearchItem(ItemId InItemId)
{
	_strSearch.Empty();
	_searchedItemGroupList.Empty();

	if (FGsMarketItemGroupData* itemGroupData = GetItemGroupDataInter(InItemId))
	{
		_strSearch = itemGroupData->GetItemNameString();
		_searchedItemGroupList.Emplace(itemGroupData);
	}

	SendSearchedItemList();

	return _strSearch;
}

void FGsMarketControllerBase::SendSearchedItemList(const FString& InStr, EGsMarketUICategory InCategory, int32 InIndex)
{
	_strSearch = InStr;
	_serverSearchedItemGroupList.Empty();

	if (_strSearch.IsEmpty())
	{
		_searchedItemGroupList.Empty();
		return;
	}

	DefItemGroupListConst targetList;
	GetItemGroupListByCategory(false, InCategory, InIndex, targetList);

	_searchedItemGroupList.Empty();
	for (const FGsMarketItemGroupData* data : targetList)
	{
		if (INDEX_NONE != data->GetItemNameString().Find(_strSearch))
		{
			_searchedItemGroupList.Emplace(const_cast<FGsMarketItemGroupData*>(data));
		}
	}
	SendSearchedItemList();
}

void FGsMarketControllerBase::SendSearchedItemList()
{
	TArray<ItemId> itemIdList;
	for (FGsMarketItemGroupData* itemGroupData : _searchedItemGroupList)
	{
		itemIdList.Emplace(itemGroupData->GetItemId());
	}

	FGsNetSendServiceMarket::SendReqItemSearch(IsWorldMarket(), itemIdList);
}

bool FGsMarketControllerBase::IsBookmark() const
{
	return (0 < _bookmarkList.Num()) ? true : false;
}

bool FGsMarketControllerBase::IsBookmarkedItem(ItemId InItemId) const
{
	return _bookmarkList.Contains(InItemId);
}

int32 FGsMarketControllerBase::GetBookmarkCount() const
{
	return _bookmarkList.Num();
}

const FGsMarketItemGroupData* FGsMarketControllerBase::GetItemGroupData(ItemId InItemId, bool bIsBookmark) const
{
	if (bIsBookmark)
	{
		return GetBookmarkItemGroupDataInter(InItemId);
	}

	return GetItemGroupDataInter(InItemId);
}

const FGsMarketItemData* FGsMarketControllerBase::GetItemData(MarketTransactionId InTransactionId) const
{
	return GetItemDataInter(InTransactionId);
}

const FGsMarketSettlementData* FGsMarketControllerBase::GetSettlementData(MarketTransactionId InTransactionId) const
{
	return GetSettlementDataInter(InTransactionId);
}

void FGsMarketControllerBase::GetItemGroupListByCategory(bool bUseFilterRule, EGsMarketUICategory InCategory, int32 InIndex,
	OUT DefItemGroupListConst& OutList)
{
	if (EGsMarketUICategory::BOOKMARK == InCategory)
	{
		GetItemGroupListBookmark(bUseFilterRule, OutList);
	}
	else if (EGsMarketUICategory::ALL == InCategory)
	{
		GetItemGroupListAll(bUseFilterRule, OutList);
	}
	else
	{
		GetItemGroupListSubCategory(bUseFilterRule, InCategory, InIndex, OutList);
	}
}

int32 FGsMarketControllerBase::GetServerLastRcvIndex()
{
	return (_strSearch.IsEmpty()) ? _serverItemGroupList.Num() : _serverSearchedItemGroupList.Num();
}

bool FGsMarketControllerBase::IsExistData(MarketCategory InCategory) const
{
	return _itemGroupMapByCategory.Contains(InCategory);
}

void FGsMarketControllerBase::SetFilterData(FGsMarketFilterData& InData)
{
	_filterData.Set(InData);
}

FGsMarketItemGroupData* FGsMarketControllerBase::GetItemGroupDataInter(ItemId InItemId) const
{
	if (FGsMarketItemGroupData* const* data = _itemGroupMap.Find(InItemId))
	{
		return *data;
	}

	return nullptr;
}

FGsMarketItemGroupData* FGsMarketControllerBase::GetBookmarkItemGroupDataInter(ItemId InItemId) const
{
	for (FGsMarketItemGroupData* iter : _bookmarkItemGroupList)
	{
		if (iter->_itemId == InItemId)
		{
			return iter;
		}
	}

	return nullptr;
}

FGsMarketItemData* FGsMarketControllerBase::GetItemDataInter(MarketTransactionId InTransactionId) const
{
	if (FGsMarketItemData* const* data = _itemMap.Find(InTransactionId))
	{
		return *data;
	}

	return nullptr;
}

FGsMarketSettlementData* FGsMarketControllerBase::GetSettlementDataInter(MarketTransactionId InTransactionId) const
{
	if (FGsMarketSettlementData* const* data = _settlementMap.Find(InTransactionId))
	{
		return *data;
	}

	return nullptr;
}


void FGsMarketControllerBase::GetItemGroupListBookmark(bool bUseFilterRule, OUT DefItemGroupListConst& OutList)
{
	// 필터 룰 사용하지 않을 경우
	if (false == bUseFilterRule)
	{
		OutList.Append(_bookmarkItemGroupList);

		return;
	}

	if (_strSearch.IsEmpty())
	{
		// 검색어가 없는 경우
		FilteringByGradeItemGroupList(_bookmarkItemGroupList, OutList);
	}
	else
	{
		// 검색어가 있으면, 검색된 결과 내에서 필터링
		for (FGsMarketItemGroupData* itemGroup : _bookmarkItemGroupList)
		{
			for (FGsMarketItemGroupData* searchedItemGroup : _searchedItemGroupList)
			{
				if (itemGroup->_itemId == searchedItemGroup->_itemId)
				{
					if (_filterData.CheckByGrade(itemGroup->GetItemGrade()))
					{
						OutList.Emplace(itemGroup);
					}
				}
			}
		}
	}

	// 소팅
	OutList.Sort([this](const FGsMarketItemGroupData& A, const FGsMarketItemGroupData& B)
		{
			if (A.IsSaleItemExist() && false == B.IsSaleItemExist())
			{
				return true;
			}
			else if (A.IsSaleItemExist() == B.IsSaleItemExist())
			{
				int32 resultGrade = SortFuncGrade(A, B);
				if (0 < resultGrade)
				{
					return true;
				}
				else if (0 == resultGrade)
				{
					int32 resultCategory = SortFuncCategory(A, B);
					if (0 < resultCategory)
					{
						return true;
					}
					else if (0 == resultCategory)
					{
						if (A.GetItemId() <= B.GetItemId())
						{
							return true;
						}
					}
				}
			}

			return false;
		});
}

void FGsMarketControllerBase::GetItemGroupListAll(bool bUseFilterRule, OUT DefItemGroupListConst& OutList)
{
	// 필터 룰 사용하지 않을 경우
	if (false == bUseFilterRule)
	{
		OutList.Append(_itemGroupList);
		return;
	}

	if (_strSearch.IsEmpty())
	{
		// 검색어가 없는 경우
		DefItemGroupListConst tempList;
		FilteringTargetItemGroupList(_itemGroupList, _serverItemGroupList, tempList);

		SortItemGroupDefault(tempList);

		DefItemGroupListConst tempServerList;
		FilteringByGradeItemGroupList(_serverItemGroupList, tempServerList);

		OutList.Append(tempServerList);
		OutList.Append(tempList);
	}
	else
	{
		// 검색어가 있으면, 검색된 결과 내에서 필터링
		DefItemGroupListConst tempList;
		FilteringTargetItemGroupList(_searchedItemGroupList, _serverSearchedItemGroupList, tempList);

		SortItemGroupDefault(tempList);

		DefItemGroupListConst tempServerList;
		FilteringByGradeItemGroupList(_serverSearchedItemGroupList, tempServerList);

		OutList.Append(_serverSearchedItemGroupList);
		OutList.Append(tempList);
	}
}

void FGsMarketControllerBase::GetItemGroupListSubCategory(bool bUseFilterRule, EGsMarketUICategory InCategory, int32 InIndex,
	OUT DefItemGroupListConst& OutList)
{
	// 필터 룰 사용하지 않을 경우
	if (false == bUseFilterRule)
	{
		DefItemGroupList tempList;
		GetItemGroupListBySubCateogryInter(InCategory, InIndex, tempList);
		OutList.Append(tempList);
		return;
	}

	if (_strSearch.IsEmpty())
	{
		// 검색어가 없는 경우

		// 카테고리 정보 가져옴
		DefItemGroupList cateogryItemGroupList;
		GetItemGroupListBySubCateogryInter(InCategory, InIndex, cateogryItemGroupList);

		DefItemGroupListConst tempList;
		FilteringTargetItemGroupList(cateogryItemGroupList, _serverItemGroupList, tempList);

		SortItemGroupDefault(tempList);

		OutList.Append(_serverItemGroupList);
		OutList.Append(tempList);
	}
	else
	{
		// 검색어가 있으면, 검색된 결과 내에서 필터링

		// 카테고리 정보 가져옴
		DefItemGroupList cateogryItemGroupList;
		GetItemGroupListBySubCateogryInter(InCategory, InIndex, cateogryItemGroupList);

		// 검색된 항목만 모음
		DefItemGroupList searchedList;
		for (FGsMarketItemGroupData* itemGroup : _searchedItemGroupList)
		{
			if (cateogryItemGroupList.Contains(itemGroup))
			{
				searchedList.Emplace(itemGroup);
			}
		}

		DefItemGroupListConst tempList;
		FilteringTargetItemGroupList(searchedList, _serverSearchedItemGroupList, tempList);

		SortItemGroupDefault(tempList);

		OutList.Append(_serverSearchedItemGroupList);
		OutList.Append(tempList);
	}
}

void FGsMarketControllerBase::GetItemGroupListBySubCateogryInter(EGsMarketUICategory InCategory, int32 InIndex,
	OUT DefItemGroupList& OutList)
{
	const FGsSchemaMarketCategoryData* categoryData = FGsMarketHelper::GetMarketCategoryData(InCategory);
	if (nullptr == categoryData)
	{
		return;
	}

	const FGsSchemaMarketSubCategoryData* subCategoryData = nullptr;
	if (categoryData->subCategoryList.IsValidIndex(InIndex))
	{
		subCategoryData = &categoryData->subCategoryList[InIndex];
	}

	if (nullptr == subCategoryData)
	{
		return;
	}

	// SubCategory가 전체일 경우 하위 항목 모두 얻어오기
	if (subCategoryData->isAll)
	{
		for (int32 i = 0; i < categoryData->subCategoryList.Num(); ++i)
		{
			const FGsSchemaMarketSubCategoryData* subData = &categoryData->subCategoryList[i];
			if (subData &&
				false == subData->isAll)
			{
				if (DefItemGroupList* listPtr = _itemGroupMapByCategory.Find(subData->marketCategory))
				{
					OutList.Append(*listPtr);
				}
			}
		}
	}
	else
	{
		if (DefItemGroupList* listPtr = _itemGroupMapByCategory.Find(subCategoryData->marketCategory))
		{
			OutList.Append(*listPtr);
		}
	}
}

void FGsMarketControllerBase::FilteringTargetItemGroupList(DefItemGroupList& InList, DefItemGroupList& InServerRcvList,
	OUT DefItemGroupListConst& OutList)
{
	for (FGsMarketItemGroupData* itemGroup : InList)
	{
		// 서버에서 준 값은 제외
		if (false == InServerRcvList.Contains(itemGroup))
		{
			// ItemGrade 필터링
			if (_filterData.CheckByGrade(itemGroup->GetItemGrade()))
			{
				OutList.Emplace(itemGroup);
			}
		}
	}
}

void FGsMarketControllerBase::FilteringByGradeItemGroupList(DefItemGroupList& InList, OUT DefItemGroupListConst& OutList)
{
	for (FGsMarketItemGroupData* itemGroup : InList)
	{
		// ItemGrade 필터링
		if (_filterData.CheckByGrade(itemGroup->GetItemGrade()))
		{
			OutList.Emplace(itemGroup);
		}
	}
}

void FGsMarketControllerBase::SortItemGroupDefault(OUT DefItemGroupListConst& OutList)
{
	// 1순위 ItemGrade 내림차순
	// 2순위 MarketCategory 오름차순
	// 3순위 ItemId 오름차순

	OutList.Sort([this](const FGsMarketItemGroupData& A, const FGsMarketItemGroupData& B)
		{
			int32 resultGrade = SortFuncGrade(A, B);
			if (0 < resultGrade)
			{
				return true;
			}
			else if (0 == resultGrade)
			{
				int32 resultCategory = SortFuncCategory(A, B);
				if (0 < resultCategory)
				{
					return true;
				}
				else if (0 == resultCategory)
				{
					if (A.GetItemId() <= B.GetItemId())
					{
						return true;
					}
				}
			}

			return false;
		});
}

int32 FGsMarketControllerBase::SortFuncGrade(const FGsMarketItemGroupData& InA, const FGsMarketItemGroupData& InB)
{
	if (InA.GetItemGrade() > InB.GetItemGrade())
	{
		return 1;
	}
	else if (InA.GetItemGrade() == InB.GetItemGrade())
	{
		return 0;
	}

	return -1;
}

int32 FGsMarketControllerBase::SortFuncCategory(const FGsMarketItemGroupData& InA, const FGsMarketItemGroupData& InB)
{
	if (InA._marketCategory < InB._marketCategory)
	{
		return 1;
	}
	else if (InA._marketCategory == InB._marketCategory)
	{
		return 0;
	}

	return -1;
}

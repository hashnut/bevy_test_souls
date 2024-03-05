// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Classes/GsPool.h"
#include "Market/GsMarketData.h"
#include "Market/GsMarketItemData.h"
#include "Market/GsMarketItemGroupData.h"
#include "Market/GsMarketFilterData.h"
#include "UTIL/GsIntervalChecker.h"

struct FGsSchemaItemCommon;

/**
 * 
 */
class T1PROJECT_API FGsMarketControllerBase
{
public:
	using DefItemGroupList = TArray<FGsMarketItemGroupData*>;
	using DefItemGroupListConst = TArray<const FGsMarketItemGroupData*>;
	using DefItemList = TArray<FGsMarketItemData*>;
	using DefSettlementList = TArray<FGsMarketSettlementData*>;

protected:
	// 데이터 풀
	TGsPoolDefault<FGsMarketItemGroupData> _poolItemGroupData;
	TGsPoolDefault<FGsMarketItemData> _poolItemData;
	TGsPoolDefault<FGsMarketSettlementData> _poolSettlementData;

	// 데이터 원본
	DefItemGroupList _itemGroupList; // 지우지 않고 내용 갱신만 수행한다
	TMap<MarketTransactionId, FGsMarketItemData*> _itemMap; // 데이터 원본
	TMap<MarketTransactionId, FGsMarketSettlementData*> _settlementMap; // 데이터 원본
	DefSettlementList _settlementLogList; // 데이터 원본(Key값이 없음... 논의 필요할까)

	TArray<ItemId> _bookmarkList; // 즐겨찾기 id만 저장. itemGroup쪽에서 저장하는 상황 대응 위함
	DefItemGroupList _bookmarkItemGroupList; // 즐겨찾기 리스트는 따로 데이터를 가짐

	int32 _logTotalCount; // 정산내역 총 개수
	MarketPrice _logTotalPrice; // 정산내역 총 가격

	// 리스트를 연결해서 받기 위해 저장한 정보들
	MarketCategory _lastUpdateCategory; // 1뎁스용
	TArray<ItemGrade> _lastUpdateGradeList; // 1뎁스용
	ItemId _lastUpdateItemId; // 1뎁스용
	TMap<ItemId, time_t> _lastUpdateTimeMap; // 2, 3뎁스용 
	TPair<MarketTransactionId, time_t> _lastUpdateLogPair; // 정산로그용
	bool _bIsItemGroupListComplete = false; // 서버에서 아이템 그룹 리스트를 다 받았음을 체크
	bool _bIsLogListComplete = false; // 서버에서 정산내역을 다 받았음을 체크

protected:
	// 필요할 때만 저장하여 사용하는 정보
	FGsMarketItemPriceData _priceData;
	// 거래소 다중 구매 시 활용
	TArray<MarketTransactionId> _multiplePurchaseList;

	// 편의를 위해 만들었고, 외부에선 FGsMarketManager의 값을 사용
	FGsMarketFilterData _filterData;
	FString _strSearch;

protected:
	// 편의에 따라 나누어 둔 데이터(포인터만 저장)
	TMap<ItemId, FGsMarketItemGroupData*> _itemGroupMap;
	TMap<MarketCategory, DefItemGroupList> _itemGroupMapByCategory;
	DefItemList _sellItemList;
	DefSettlementList _settlementList;
	DefItemGroupList _searchedItemGroupList; // 찾아놓은것

protected:
	DefItemGroupList _serverSearchedItemGroupList; // 서버에서 찾아놓은거에 대한 정보 준 것
	DefItemGroupList _serverItemGroupList; // 서버에서 받은 아이템 그룹 리스트

protected:
	// 패킷 최적화를 위한 쿨타임 체커
	FGsIntervalChecker _checkerBookmark;
	FGsIntervalChecker _checkerSellList;
	FGsIntervalChecker _checkerSettlementList;
	
public:
	FGsMarketControllerBase() = default;
	virtual ~FGsMarketControllerBase() {}

public:
	virtual void Initialize();
	virtual void InitializeItemGroup(const FGsSchemaItemCommon* InData);
	virtual void Finalize();

protected:	
	virtual bool IsTradableItem(const FGsSchemaItemCommon* InRow) const { return false; }
	virtual bool IsWorldMarket() const { return false; }

public:
	void SetBookmark(ItemId InItemId, bool bIsBookmarked);

public:
	void RemoveItem(MarketTransactionId InTransitionId);
	void RemoveSettlementItem(MarketTransactionId InTransitionId);
	void RemoveAllSettlementItems();
	void ClearSearchData();
	void ClearFilterData();

	// 거래소 다중 구매 리스트 관리
	bool AddMultiplePurchaseList(MarketTransactionId InTransactionId);
	bool RemoveMultiplePurchaseList(MarketTransactionId InTransactionId);
	bool IsMultiplePurchaseTarget(MarketTransactionId InTransactionId) const;
	void ClearMultiplePurchaseList();
	int32 GetMultiplePurchaseCount() const { return _multiplePurchaseList.Num(); }
	TArray<MarketTransactionId> GetMultiplePurchaseList() const { return _multiplePurchaseList; }
	
	// Send Request
public:
	void SendReqBookmarkList();
	void SendReqItemGroupList(MarketCategory InCategory, bool bInResetScroll = true, bool bInIsScrolled = false);
	void SendReqItemList(ItemId InItemId, bool bResetTime);
	void SendReqSellItemList(bool bResetCoolTime);
	void SendReqSettlementList();
	void SendReqLogList(bool bResetScroll, bool bInIsScrolled = false);
	FString SendSearchItem(ItemId InItemId);
	void SendSearchedItemList(const FString& InStr, EGsMarketUICategory InCategory, int32 InIndex);
	void SendSearchedItemList();

public:
	bool IsBookmark() const;
	bool IsBookmarkedItem(ItemId InItemId) const;
	int32 GetBookmarkCount() const;

	const FGsMarketItemGroupData* GetItemGroupData(ItemId InItemId, bool bIsBookmark) const;
	const FGsMarketItemData* GetItemData(MarketTransactionId InTransactionId) const;
	const FGsMarketSettlementData* GetSettlementData(MarketTransactionId InTransactionId) const;

	const FGsMarketItemPriceData& GetItemPriceData() const { return _priceData; }
	DefItemGroupList GetItemGroupList() { return _itemGroupList; }
	DefItemList GetSellItemList() const { return _sellItemList; }
	DefSettlementList GetSettlementList() const { return _settlementList; }
	int32 GetSettlementCount() const { return _settlementList.Num(); }
	DefSettlementList GetSettlementLogList() const { return _settlementLogList; }
	int32 GetLogTotalCount() const { return _logTotalCount; }
	MarketPrice GetLogTotalPrice() const { return _logTotalPrice; }

	// 서버에서 받은 정보(구매 가능한, 서버에서 이미 정렬/필터링 해서 보내는 정보)를 가장 먼저 넣어주고,
	// 나머지 정보를 필터링 후 정렬하여 넣어준다
	void GetItemGroupListByCategory(bool bUseFilterRule, EGsMarketUICategory InCategory, int32 InIndex,
		OUT DefItemGroupListConst& OutList);
	int32 GetServerLastRcvIndex();
	bool IsItemGroupListComplete() const { return _bIsItemGroupListComplete; }
	bool IsLogListComplete() const { return _bIsLogListComplete; }

	bool IsExistData(MarketCategory InCategory) const;

	void SetFilterData(FGsMarketFilterData& InData);

protected:
	FGsMarketItemGroupData* GetItemGroupDataInter(ItemId InItemId) const;
	FGsMarketItemGroupData* GetBookmarkItemGroupDataInter(ItemId InItemId) const;
	FGsMarketItemData* GetItemDataInter(MarketTransactionId InTransactionId) const;
	FGsMarketSettlementData* GetSettlementDataInter(MarketTransactionId InTransactionId) const;


	void GetItemGroupListBookmark(bool bUseFilterRule, OUT DefItemGroupListConst& OutList);
	void GetItemGroupListAll(bool bUseFilterRule, OUT DefItemGroupListConst& OutList);
	void GetItemGroupListSubCategory(bool bUseFilterRule, EGsMarketUICategory InCategory, int32 InIndex,
		OUT DefItemGroupListConst& OutList);
	void GetItemGroupListBySubCateogryInter(EGsMarketUICategory InCategory, int32 InIndex,
		OUT DefItemGroupList& OutList);

	// 서버에서 받은 값은 제외하고, FilterData에 들어있는 ItemGrade 값으로 필터링
	void FilteringTargetItemGroupList(DefItemGroupList& InList, DefItemGroupList& InServerRcvList,
		OUT DefItemGroupListConst& OutList);
	// 대상 리스트를 Grade로 필터링
	void FilteringByGradeItemGroupList(DefItemGroupList& InList, OUT DefItemGroupListConst& OutList);

	void SortItemGroupDefault(OUT DefItemGroupListConst& OutList);
	int32 SortFuncGrade(const FGsMarketItemGroupData& InA, const FGsMarketItemGroupData& InB);
	int32 SortFuncCategory(const FGsMarketItemGroupData& InA, const FGsMarketItemGroupData& InB);
};

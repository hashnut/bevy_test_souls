// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Classes/GsManager.h"
#include "Classes/GsPool.h"
#include "Management/GsScopeHolder.h"
#include "Market/GsMarketData.h"
#include "Market/GsMarketItemData.h"
#include "Market/GsMarketItemGroupData.h"
#include "Market/GsMarketFilterData.h"
#include "Market/GsMarketCollectionData.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedMarketEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Market/GsMarketControllerBase.h"
#include "DataSchema/GsSchemaEnums.h"
#include "UTIL/GsIntervalChecker.h"
#include "RedDot/GsRedDotCheck.h"



namespace PD
{
	namespace GC
	{
		struct PKT_GC_ACK_MARKET_SALES_ITEM_LIST_READ;
		struct PKT_GC_ACK_MARKET_TRANSACTION_LIST_INSTACKABLE_READ;
		struct PKT_GC_ACK_MARKET_TRANSACTION_LIST_STACKABLE_READ;
		struct PKT_GC_ACK_MARKET_MY_TRANSACTION_LIST_READ;
		struct PKT_GC_ACK_MARKET_CALCULATE_LIST_READ;
		struct PKT_GC_ACK_MARKET_ITEM_PRICE_READ;
		struct PKT_GC_ACK_MARKET_ITEM_SEARCH_READ;
		struct PKT_GC_ACK_MARKET_TRANSACTION_DETAIL_EQUIP_READ;
		struct PKT_GC_ACK_MARKET_BOOKMARK_LIST_READ;
		struct PKT_GC_ACK_MARKET_CALCULATED_LIST_READ;
		struct PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ;
	}
	namespace SC
	{
		struct PKT_SC_ACK_MARKET_TRANSACTION_REGISTER_READ;
	}
}

//class FGsMarketControllerBase;
class FGsMarketControllerNormal;
class FGsMarketControllerWorld;

/**
 * 거래소 데이터 매니저
 * - 즐겨찾기 리스트: 전부 받음
 * - 아이템 리스트:
 *	 - 1뎁스 정보: n개씩 나눠 받음. 마지막으로 받은 정보 보내서 이후로 이어받음.
 *   - 2,3뎁스 정보: 전부 받음. 마지막 변경시간을 보내서 변경이 있을 때만 다시 받음.
 * - 정산 리스트: 전부 받음
 * - 정산내역 리스트: n개씩 나눠받음. 마지막으로 받은 정보 보내서 이후로 이어받음.
 */
class T1PROJECT_API FGsMarketManager :
	public IGsManager,
	public IGsRedDotCheck
{
public:
	using DefItemGroupList = TArray<FGsMarketItemGroupData*>;
	using DefItemGroupListConst = TArray<const FGsMarketItemGroupData*>;
	using DefItemList = TArray<FGsMarketItemData*>;
	using DefSettlementList = TArray<FGsMarketSettlementData*>;


	struct FGsMarketPurchaseResult
	{
		bool _bIsWorldMarket = false;
		MarketTransactionId _transactionId;
		int32 _result = 0;

		FGsMarketPurchaseResult(bool bIsWorldMarket, MarketTransactionId InTransactionId, int32 InResult)
			: _bIsWorldMarket(bIsWorldMarket), _transactionId(InTransactionId), _result(InResult)
		{
		}
	};

private:
	FGsMarketControllerNormal* _controllerNormal = nullptr;
	FGsMarketControllerWorld* _controllerWorld = nullptr;
	TArray<FGsMarketControllerBase*> _controllerList; //0:노멀 1:월드

private:
	// 검색 관련
	TPair<ItemId, bool> _serchItemInfo;
	FString _strSearch;
	FGsMarketFilterData _filterData;
	
	// 거래소 구매 결과
	TArray<FGsMarketPurchaseResult> _purchaseResultList;

	// 컬렉션 관련
	TGsPoolDefault<FGsMarketCollectionData> _poolCollectionData;
	TMap<CollectionId, FGsMarketCollectionData*> _collectionDataMap;

public:
	FGsMarketManager() = default;
	virtual ~FGsMarketManager() = default;

public:
	// IGsManager override
	virtual void Initialize() override;
	virtual void Finalize() override;

	// IGsIsRedDotCheck override
	virtual bool CheckRedDot(bool bInUpdate = true) override;

public:
	bool CheckRedDoyByServer(bool bIsWorldMarket);

public:
	// 구매 결과 저장(거래소, 컬렉션)
	void SetPurchaseResult(bool bIsWorldMarket, MarketTransactionId InTransactionId, int32 InResult);
	void ClearPurchaseResult();
	
public:
	void RemoveItem(bool bIsWorldMarket, MarketTransactionId InTransitionId);
	void RemoveSettlementItem(bool bIsWorldMarket, MarketTransactionId InTransitionId);
	void RemoveAllSettlementItems(bool bIsWorldMarket);
	void ClearSearchData(); // 서버/월드 거래소 정보 둘다지움
	void ClearFilterData(); // 서버/월드 거래소 정보 둘다지움

	// 거래소 다중 구매 리스트 관리
	bool AddMultiplePurchaseList(bool bIsWorldMarket, MarketTransactionId InTransactionId);
	bool RemoveMultiplePurchaseList(bool bIsWorldMarket, MarketTransactionId InTransactionId);
	bool IsMultiplePurchaseTarget(bool bIsWorldMarket, MarketTransactionId InTransactionId) const;
	void ClearMultiplePurchaseList();  // 주의: 이건 둘다 지워야 할 것 같은데
	int32 GetMultiplePurchaseCount(bool bIsWorldMarket) const;
	TArray<MarketTransactionId> GetMultiplePurchaseList(bool bIsWorldMarket) const;

	// 컬렉션 관련
public:
	void ClearCollectionData();
	void SetCollectionData(PD::GC::PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ* InPacket);
	FGsMarketCollectionData* CreateOrGetCollectionData(CollectionId InCollectionId);
	FGsMarketCollectionData* GetCollectionData(CollectionId InCollectionId) const;

public: 
	void SendReqBookmarkList(bool bIsWorldMarket);	
	void SendReqItemGroupList(bool bIsWorldMarket, MarketCategory InCategory, bool bInResetScroll = true, bool bInIsScrolled = false);
	void SendReqItemList(bool bIsWorldMarket, ItemId InItemId, bool bResetTime);
	void SendReqSellItemList(bool bIsWorldMarket, bool bResetCoolTime);
	void SendReqSettlementList(bool bIsWorldMarket);
	void SendReqLogList(bool bIsWorldMarket, bool bResetScroll, bool bInIsScrolled = false);	
	void SendSearchItem(bool bIsWorldMarket, ItemId InItemId);
	void SendSearchedItemList(bool bIsWorldMarket, const FString& InStr, EGsMarketUICategory InCategory, int32 InIndex);
	void SendSearchedItemList(bool bIsWorldMarket);

public:
	FGsMarketControllerNormal* GetNormalController() { return _controllerNormal; }
	FGsMarketControllerWorld* GetWorldController() { return _controllerWorld; }
	FGsMarketControllerBase* GetController(bool bIsWorldMarket) const;

public:
	const FString GetSearchString(bool bIsWorldMarket) const { return _strSearch; }
	const TPair<ItemId, bool>& GetSearchItemInfo() const { return _serchItemInfo; }
	FGsMarketFilterData& GetFilterData() { return _filterData; }
	void SetFilterData(FGsMarketFilterData& InData);
	bool IsExistData(bool bIsWorldMarket, MarketCategory InCategory) const;

	int32 GetPurchaseSuccessCount() const;
	// 값이 없으면 -1을 반환
	int32 GetPurchaseResult(bool bIsWorldMarket, MarketTransactionId InTransactionId);

	ItemId GetShortcutItemId() const { return _serchItemInfo.Key; }
};

#define GSMarket() UGsScopeHolder::GetGameManagerFType<FGsMarketManager>(UGsScopeGame::EManagerType::Market)

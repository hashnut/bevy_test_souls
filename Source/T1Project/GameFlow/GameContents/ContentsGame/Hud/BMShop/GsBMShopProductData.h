// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataSchema/BMShop/GsSchemaBMShopProductData.h"
#include "DataSchema/GsSchemaEnums.h"
#include "T1Project/GameObject/Define/GsGameObjectDefine.h"
#include "HIVE_IAPV4.h"

#include "GameObject/Define/EBMShopProductTagType.h"
#include "GameObject/Define/EBMShopHighlightType.h"
#include "GameObject/Define/EBMShopProductDetailPopupType.h"

using namespace hive;


class FGsBMShopCostInfo final
{
private:
	CostType _costType = CostType::MAX;
	CurrencyType _currencyType = CurrencyType::NONE;
	uint32 _currencyItemTid = 0;
	Currency _amount = 0;
	bool _isItemCurrencyType = false;

public:
	FGsBMShopCostInfo() = default;
	explicit FGsBMShopCostInfo(CostType costType, CurrencyType currencyType, uint32 currencyItemTid, Currency amount, bool isItemCurrencyType)
		:
		_costType(costType), _currencyType(currencyType), _currencyItemTid(currencyItemTid), _amount(amount), _isItemCurrencyType(isItemCurrencyType)
	{
	}

public:
	const bool IsItemCurrencyType()
	{
		return _isItemCurrencyType;
	}
	const CostType GetCostType() { return _costType;}
	const CurrencyType GetCurrencyType() { return _currencyType; }
	const uint32 GetCurrencyItemTid() { return _currencyItemTid; }
	const Currency GetAmount() { return _amount; }

};

/**
 * BM 상점 상품 정보
 */
class FGsBMShopProductData final
{
public:
	struct HiveProductInfo final
	{
	public:
		std::string _productTypeString;
		std::string _marketPID;
		std::string _currency;
		double _price;
		std::string _displayPrice;
		std::string _title;
		std::string _productDescription;

		//Price before Promotion Sale
		std::string _displayOriginalPrice;
		double _originalPrice;

		/**
		 * 32-bit PNG (with alpha)
		 *
		 * Dimensions: 512px by 512px
		 *
		 * Maximum file size: 1024KB
		 */
		std::string _iconURL;
		int32 _coinsReward;

	public:
		HiveProductInfo(const IAPV4Product& InIAPV4Product)
			: _productTypeString(InIAPV4Product.productType)
			, _marketPID(InIAPV4Product.marketPid)
			, _currency(InIAPV4Product.currency)
			, _price(InIAPV4Product.price)
			, _displayPrice(InIAPV4Product.displayPrice)
			, _title(InIAPV4Product.title)
			, _productDescription(InIAPV4Product.productDescription)
			, _displayOriginalPrice(InIAPV4Product.displayOriginalPrice)
			, _originalPrice(InIAPV4Product.originalPrice)
			, _iconURL(InIAPV4Product.iconURL)
			, _coinsReward(InIAPV4Product.coinsReward)
		{
		}
	};


private:
	const BMProductId _productID = 0;
	uint16	_purchasedQuantiy = 0;  // 구입한 수량
	FDateTime* _startSaleTime = nullptr;
	FDateTime* _endSaleTime = nullptr;
	TUniquePtr<const HiveProductInfo> _hiveProductInfo;
	FGsBMShopCostInfo* _costInfo = nullptr;

	// 해당 맴버들은 child 상품에 따라서 동적으로 변하는 데이터 이므로
	// 맴버로 갖고 있음
private:
	int32 _itemId = 0;
	CurrencyType _rebateCurrencyType = CurrencyType::NONE;
	Currency _rebateCurrencyPrice = 0;
	int32 _pcLevelLimit = 0;
	uint16 _purchaseQuantityLimit = 0;
	BMPurchaseLimitBuyerType _limitBuyerType = BMPurchaseLimitBuyerType::NONE;
	ResetTimeTicket _resetTimeTicketType = ResetTimeTicket::RTT_NONE;
	TArray<int32> _rewardGroupIdList;
	int32 _displaySaleEndCountdownTimeValue = 0;
	int32 _guildLevelLimit = 0;
	bool _isGuildGiftProduct = false;
	const FSoftObjectPath* _iconPath = nullptr;

private:
	// 이 변수는 서버에서 긴급하게 상품 노출에 대하여 패킷 주는 경우에만 사용 (현금 결제 상품 전용)
	bool _isRemoveDisplay = false; 

private:
	const FGsSchemaBMShopProductData* _tableData = nullptr;

public:
	FGsBMShopProductData() = default;
	explicit FGsBMShopProductData(const int32 InProductID, const IAPV4Product* InHiveProductInfo);
	~FGsBMShopProductData();

public:
	void InitCostInfo(const FGsSchemaBMShopProductData* InTableData);

public:
	void UpdateHiveProductInfo(const IAPV4Product* InHiveProductInfo);
	void UpdateTableData(const int32 InProductID);
	void UpdateChildProductData(FGsBMShopProductData* InParentData);

public:
	bool IsHiveProduct() const;

public:
	//const bool IsDisplayRemainTimeAfterSale();
	const bool IsExpireSale() const;
	const bool IsSlodOut() const;
	const bool IsRetryBuyPossible() const;
	const bool IsEnoughLevelLimit() const;
	const bool IsEnoughGuildGiftLevelLimit() const;
	const bool IsBeforSale() const;
	const bool IsCompleteSteupCondition() const;
	const bool IsItemCurrencyType() const;
	const CostType GetPurchaseCostType() const;
	const uint32 GetPurchaseCurrencyItemId() const;
	const CurrencyType GetPurchaseCurrencyType() const;
	const Currency GetPurchasePrice() const;
	const bool IsCashProductItem() const;
	const bool IsCashRebate() const;

	/** 단일 구매로 구매 가능한 최대 구매 갯수.*/
	uint32 GetMaxCountSinglePurchase() const;
	/** 출력용 상품명. 하이브 상품은 하이브 상품명을 출력함. */
	FText GetProductDisplayName() const;
	/** 출력용 가격. 하이브 상품은 하이브 가격을 출력함. */
	FText GetProductDisplayPrice() const;
	/** 상품의 통화 코드. 하이브 상품은 하이브의 통화 코드를 기반으로 나타내고, 유효하지 않을 경우, GlobalConstant 의 디폴트 통화 코드를 반환함. */
	EGsBMShopCashCurrency GetHiveCurrencyCode() const;

public:
	void SetPurchaseQuantiy(uint16 InCount) { _purchasedQuantiy = InCount; }
	void SetIsRemoveDisplay(const bool InIsRemove) { _isRemoveDisplay = InIsRemove; } // 이 함수는 서버에서 긴급하게 상품 노출에 대하여 패킷 주는 경우에만 사용


	// Table Data Info
public:
	const EBMShopProductTagType GetTagType() const;
	const EBMShopHighlightType GetHighLightType() const;
	const FString GetDisplayTime() const;
	const bool GetSaleStartTime(FDateTime& outData) const;
	const bool GetSaleEndTime(FDateTime& outData) const;
	const bool GetSaleDispalyStartTime(FDateTime& outData) const;
	const bool GetSaleDispalyEndTime(FDateTime& outData) const;
	void GetAllActiveDisplayText(OUT TMap<FString, FString>& outDisplayStringMap) const;
	const FString GetLimitSaleTimeText() const;
	const BMProductId GetProductID() const { return _productID; }
	const int32 GetItemTID() const { return _itemId; }
	const FSoftObjectPath* GetIconPath() const;
	const uint16 GetPurchaseQuantiy() const { return _purchasedQuantiy; }
	const uint16 GetPurchaseQuantiyLimit() const { return _purchaseQuantityLimit; }
	const int32 GetParentTabID() const { return _tableData->bmShopParentTabId; }
	const int32 GetSubTabID() const { return _tableData->bmShopSubTabId; }
	FGsBMShopCostInfo* GetProductCostInfo() { return _costInfo; }
	const CurrencyType GetRebateCurrencyType() const { return _rebateCurrencyType; }
	const int32 GetRebateAmount() const { return _rebateCurrencyPrice; }
	const int32 GetDisplayOrder() { return _tableData->displayOrder; }
	const int32 GetLimitLevel() const { return _pcLevelLimit; }
	const int32 GetGuildGiftLimitLevel() const { return _guildLevelLimit; }
	const ResetTimeTicket GetResetTimeType() const { return _resetTimeTicketType; }
	const BMPurchaseLimitBuyerType  GetLimitBuyerType() const { return _limitBuyerType; }
	const EBMShopProductDetailPopupType GetPopupDetailType() const { return _tableData->detailPopupType; }
	const bool IsGuildGift() const { return _isGuildGiftProduct; }
	const bool IsBulkSale() const { return _tableData->isBulkSale; }
	const TArray<int32> GetRewardIdList() const { return _rewardGroupIdList; }
	const int32 GetBannerDisplayOrder() const { return _tableData->bannerDisplayOrder; }
	const int32 GetStepUpGroupID() const { return _tableData->stepGroupID; }
	const int32 GetStepUpLevel() const { return _tableData->stepLevel; }
	const int32 GetStepUpBuyLimitCount() const { return _tableData->stepBuyLimitCount; }
	const FString GetBannerImgPath() const { return _tableData->bannerImgPath; }
	const FText& GetName() const { return _tableData->titleText; }
	const FText& GetDesc() const { return _tableData->desc; }
	const FText& GetExternalDesc() const { return _tableData->externalDesc; }
	const bool IsStepUpProduct() const { return 0 < _tableData->stepGroupID; }
	const bool IsPopupShopProduct() const { return 0 < _tableData->popupConditionId;}
	const bool IsExistParentProductID() const{ return 0 < _tableData->parentProductId; }
	const int32 GetPopupShopHiveBannerID() const { return _tableData->popuphiveBannerId; }
	const int32 GetParentProductID() const { return _tableData->parentProductId; }
	const int32 GetDisplaySaleEndCountdownTimeValue() const { return _displaySaleEndCountdownTimeValue; }
	const bool IsCouponProduct() const;
	void GetCouponGroupID(OUT TArray<int32>& outGroupIdList) const;
	const bool IsRemoveDisplay() const { return _isRemoveDisplay; } // 이 함수는 서버에서 긴급하게 상품 노출에 대하여 패킷 주는 경우에만 사용
	const bool IsOOAP() const { return _tableData->isOOAP; }
	const FGsSchemaBMShopProductData* GetTableData() { return _tableData; }
	const bool IsImmediatelyUseProduct() { return _tableData->immediatelyUse; }

	// Hive Info
public:
	std::string GetHiveMarketPID();
	FString GetConvertHiveMarketPID();
private:
	FString GetHiveCurrency() const;
	FString GetConvertHiveTitle() const;
	FString GetConvertHiveDisplayPrice() const;

private:
	const HiveProductInfo* GetHiveProduct() const { return _hiveProductInfo.Get(); } // nullptr 인 상품도 있음

private:
	const FGsSchemaBMShopProductData* FindTableData();

public:
	//For Test
	void PrintHiveProductInfo() const;
};

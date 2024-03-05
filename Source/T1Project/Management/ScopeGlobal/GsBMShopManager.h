// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "GameObject/Define/GsGameObjectDefine.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/BMShop/GsSchemaBMShopTabInfo.h"
#include "DataSchema/Item/GsSchemaItemEffectDiscountCouponData.h"

#include "Message/MessageParam/GsBMShopMessageParam.h"

#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsIAPPurchaseBase.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateHandler.h"

#include "Management/GsScopeGlobal.h"
#include "Management/GsScopeHolder.h"

#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageContents.h"

#include "Reward/GsRewardHelper.h"

#include "RedDot/GsRedDotCheck.h"
#include "HIVE_IAPV4.h"


using namespace hive;

struct FGsSchemaBMShopProductData;

class FGsItem;
class FGsBMShopProductData;
class FGsBMShopStorageItemData;

/**
 *  BMShop 패킷 데이터 및 테이블 데이터 관리 매니저
 */
class FGsBMShopManager final :
public IGsManager,
public IGsRedDotCheck
{
public:
	enum RemainTimeUnitType : uint8
	{
		Day_Type = 0,
		Hour_Type,
		Min_Type,
		Sec_Type,

		Type_Max,
	};

	enum StepUpState : uint8
	{
		State_None = 0,
		State_IsOpen,
		State_IsLock,
		State_Complete,
		State_IsSoldOut,

		State_Max,
	};

	enum BMShopSoundType : uint8
	{
		Sound_Purchase = 0,
		Sound_ReceiveStorageItem,

		Sound_Max,
	};

private:
	/**
	 * 자동순차결제봇
	 */
	class FGsPurchaseSequentially final
	{
	private:
		/** 자동순차결제에 사용할 구매 정보 */
		TSharedRef<FGsBMShopCheckValidProduct> _message;

	public:
		/** 자동순차결제 총 횟수 */
		const ItemAmount _limit;

	private:
		/** 자동순차결제 현재 결제 수 */
		ItemAmount _count;

	public:
		FGsPurchaseSequentially(TSharedRef<FGsBMShopCheckValidProduct> InBMShopCheckValidProduct, const ItemAmount InLimit)
			: _message(InBMShopCheckValidProduct)
			, _limit(FMath::Clamp<ItemAmount>(InLimit, 0, TNumericLimits<ItemAmount>::Max()))
			, _count(0)
		{
		}
		
		/** 카운트 리셋 */
		void ResetCount()
		{
			_count = 0;
		}

		/** 카운트 업. */
		void CountUp()
		{
			_count = FMath::Clamp<ItemAmount>(_count + 1, 0, _limit);
		}

		/** 모든 결제 완료 */
		bool IsCompleted() const
		{
			return _limit <= _count;
		}

		/** 현재 남은 결제 횟수 */
		ItemAmount RemainCount() const
		{
			return _limit - _count;
		}

		/**
		 * this->_limit 가 1인 경우, 1회 결제만 수행하면 되므로, 저장한 메세지를 그대로 반환함.
		 * 1을 초과하는 경우, 
		 */
		TSharedRef<FGsBMShopCheckValidProduct> MakeMessage()
		{
			if (1 == _limit)
			{
				// 1회 구매인 경우, 저장된 메세지 그대로 반환.
				return _message;
			}
			else
			{
				return MakeShared<FGsBMShopCheckValidProduct>(TArray<BMProductIdAmountPair> { BMProductIdAmountPair(_message->_pairList[0].mBMProductId, 1) }, _message->_isBulkSale, _message->_isUseDiscount, _message->_discountCurrencyAmount);
			}
		}
	};

private:
	TUniquePtr<FGsBMShopHandler> _bmShopHandler;
	TUniquePtr<FGsServerMigrateHandler> _ServerMigrateHandler;

private:
	// 테이블 데이터 기반 컨테이터
	// TMap<ProductID, FGsBMShopProductData*>
	TMap<BMProductId, FGsBMShopProductData*> _bmShopProductDataMap;

	// 부모 상품 id 를 물고있는 자식 상품들 관리 컨테이너
	// 동일한 상품이 다른 탭에서 출력해야 하는 상황 처리
	TMap<BMProductId, TArray<BMProductId>> _bmShopChildProductIdMap;

	// StepUp 상품들 데이터
	// TMap<StepUpGroupID, TArray<FGsBMShopProductData*>>
	TMap<BMProductId, TArray<FGsBMShopProductData*>> _stepUpProductDataMap;
	
	// ui tab 구성 컨테이너
	TArray<const FGsSchemaBMShopTabInfo*> _mainTabDataList;
	TArray<const FGsSchemaBMShopTabInfo*> _subTabDataList;

	// 보관함 관리 컨테이너
	TMap<uint64 , FGsBMShopStorageItemData*> _storageItemDataMap;

	// 상품 판매 시간 관리 컨테이터
	// 상품 ID , 계산할 target 시간
	TMap<BMProductId, FDateTime> _saleRemainTimeManageMap;
	// 상품 ID , 남은 시간
	TMap<BMProductId, FTimespan> _saleRemainTimeSpanMap;
	TArray<BMProductId> _removeTimeManageList;

	// 일괄구매 저장 상품들
	// productID , TPair<state,amount>
	TMap<BMProductId, TPair<bool,uint16>> _bulkSaleProductDataMap;

	// 팝업상점 상품 정보
	// 상품 ID , 남은 시간(패킷시간)
	TArray<TPair<BMProductId, FDateTime>> _popupShopProductDataList;

	// BM 상점에서 사용될 쿠폰 관련 데이터 캐싱처리
	// <conditionID, TMap<쿠폰ID, Schema*>>
	TMap<int32, TMap<int32, const FGsSchemaItemEffectDiscountCouponData*>> _couponDataMap;

private:
	//bool _isPopupZpay = false;
	//bool _isUseZpayFlag = false;
	//bool _isCanUseZpay = false;
	// bool _isStartInAppPurchase = false;
	bool _isSuccessIAPV4CheckOnce = false;
	uint64 _zPayUserDbid = 0;
	Level _zPayUserLevel = 0;

private:
	TMap<BMShopSoundType, uint64> _bmShopSoundIdMap;

private:
	BMProductId _analyticsProductId = 0;
	/** 참이면, PKT_SC_SPAWN_ME_COMPLETE_WRITE 수신 시 PgUnconsumedPurchase 를 서버로 송신하고 false 로 변경함. */
	bool _waitForReqPgUnconsumedPurchase = false;

private:
	bool _isUseIAP = true;
	FString _zPayModeUserServerName = "";
	FString _zPayModeUserName = "";

private:
	TArray<const FGsSchemaBMShopProductData*> _bmShopProductTableData;
	BMProductId _reserveHivePromoProductID = 0;
	BMProductId _reserveBannerProductID = 0;
	uint64 _reserveCouponDBID = 0;
	uint8 _retryCount = 0;
	bool _isActiveIAPV4Log = true;

	// 에디터에 한하여 인앱결제 상품을 테스트용으로 구매할지에 대한 flag
public:
	bool _editorZpay = false;
	bool _usecheatFlag = false;

private:
	TPair<MessageContentTutorial, FDelegateHandle> _tutorialMsgHandler;
	MsgGameObjHandle _gameobjectMsgHandler;
	MsgUIHandleArray _uiMsgHandlerList;

	TUniquePtr<FGsPurchaseSequentially> _purchaseSequentially;

	//private:
		//PhoneInfo

public:
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float inTick) override;

public:
	virtual void InitializeAfterResourceDownload() override;

public:
	void InitHiveProductList(TArray<IAPV4Product>& InHiveInfoList , const DevicePlatformType InCurPlatformType);

public:
	void ResetShopData();

public:
	void UpdatePurchaseQuantity(const BMProductId InProductID , const uint16 InAmount);
	void UpdateProduct(TArray<struct BMShopProductInfo>& InServerDataList);
	void UpdateProduct(TArray<struct BMProductIdAmountPair>& InServerDataList);
	void UpdateBlockList(TArray<FString>& InServerBlockList);
	void UpdateBlockProductNotify(FString InPid, bool InIsRemove);


public:
	void UpdatePopupShopInfo();
	void UpdatePopupShopPacketInfo(TArray<TPair<BMProductId,time_t>> InServerDataList);
	void UpdatePopupShopNotifyPacketInfo(TArray<TPair<BMProductId, time_t>> InServerDataList);

	void UpdateStorageItems(TArray<struct BMDepotDetail>& InServerDataList);
	void RemoveStorageItems(TArray<BMItemDBId>& InServerDataList);

	void UpdateBulkSaleProductList(TArray<BMProductBulkSale> InPairList);
	void ResetBulkSaleSettingData();

	void UpdateChildProductData(FGsBMShopProductData* InParentData);

public:
	bool IsReceivedHiveCallback(const FGsIAPPurchaseBase::CallbackResultType InType);


public:
	static void DisplayBuyPopup(const BMProductId InProductID);
	void FinishedPurchaseItem(TArray<TPair<BMProductId, ItemAmount>> InProductIdList, bool InIsBulkSale, const bool InNotify);
	static void FinishedUpdateList();
	static void CheckFairySummonWarnPopup();
	static void CheckCostumeSummonWarnPopup();
	bool IsSameTypeProductBySubCategory(const BMProductId InProductID , const ItemCategorySub InSubCateogry);
	bool IsSummonTypeProduct(const BMProductId InProductID);
	bool IsExistTabID(int32 InTabID) const;
	bool IsMainTabID(int32 InTabID) const;
	bool hasChildTab(int32 InTabID) const;
	bool IsOpenedPopupShopProduct(const BMProductId InProductID) const;
	StepUpState GetStepUpProductState(const BMProductId InProductID);
	static void SendPopupShopProduct();
	bool IsUseIAPSystem() const { return _isUseIAP; }
	void ValidCheckCashProduct(OUT TArray<BMProductId>& InProductIdList);

public:
	static void GuildGiftNotice(const FString InUerName, const int32 InItemTID, const uint64 InBuyerDBID);

public:
	FGsBMShopProductData* FindProduct(const BMProductId InProductID);
	FGsBMShopProductData* FindProduct(FString InMarketPID);
	FGsBMShopStorageItemData* FindStorageItems(const uint64 InStorageItemDBID);
	const BMProductId FindCurOpenStepUpProductId(const BMProductId InCurProductID);
	void FindProductListByTabInfo(const int32 InMainTabID, OUT TArray<FGsBMShopProductData*>& outDataList);
	void FindProductListByTabInfo(const int32 InMainTabID, const int32 InSubTabID, OUT TArray<FGsBMShopProductData*>& outDataList);
	void FindProductIDListByTabInfo(const int32 InMainTabID, const int32 InSubTabID, OUT TArray<BMProductId>& outDataList);
	void FindProductIDListByTabInfo(const int32 InMainTabID, OUT TArray<BMProductId>& outDataList);
	void FindMainTabData(bool IsUseZpayMode, OUT TArray<const FGsSchemaBMShopTabInfo*>& outDataList);
	void FindSubTabDataByMainID(const int32 InMainTabID, OUT TArray<const FGsSchemaBMShopTabInfo*>& outIDList);
	void FindStepUpProduct(TArray<BMProductId> InProductIdList, OUT TArray<FGsBMShopProductData*>& outDataList);
	const FGsSchemaBMShopTabInfo* FindMainTabInfoByID(const int32 InMainTabID) const;
	const FGsSchemaBMShopTabInfo* FindSubTabInfoByID(const int32 InSubTabID) const;
	const FGsSchemaBMShopTabInfo* FindSpecialTabInfo(const EGsBMShopSpecialTab InType) const;
	bool FindProductGroupInfoByProductID(const BMProductId InProductID, OUT TArray<FGsRewardUnboxingData>& outRewardDataList);
	bool FindCouponDataByProductID(const BMProductId InProductID, OUT TMap<TWeakPtr<FGsItem> , const FGsSchemaItemEffectDiscountCouponData*>& outDataMap);

public:
	void PlayBMShopSound(const BMShopSoundType InType);


public:
	void SetHivePromotionCallbackData(const BMProductId InProductID);
	void SetProductRemainTime(const BMProductId InProductId, FDateTime InTargetTime);
	void SetReserveCouponItemDBID(const uint64 InItemDBID) { _reserveCouponDBID = InItemDBID; }
	void SetRetryHiveCallbackCount(const uint8 InCount) { _retryCount = InCount; };
	void SetIsUseIAPV4SheetsLogFlag(const bool InIsUse) { _isActiveIAPV4Log = InIsUse; }
	// void SetIsPopupZpay(const bool InShow) { _isPopupZpay = InShow; }
	//void SetIsUseZpay(const bool InUse) { _isUseZpayFlag = InUse; }
	//void SetIsCanUseZpay(const bool InCanUse) { _isCanUseZpay = InCanUse; }
	void SetZpayModeUserServerName(FString InServerName) { _zPayModeUserServerName = InServerName; }
	void SetZpayModeUserName(FString InName) { _zPayModeUserName = InName; }
	// void SetIsStartInAppPurchase(const bool InIsStart) { _isStartInAppPurchase = InIsStart; }
	void SetZpayUserDbid(uint64 InDbid) { _zPayUserDbid = InDbid; }
	void SetZpayUserLevel(Level InLevel) { _zPayUserLevel = InLevel; }

public:
	void OnIAPV4SheetsLog(FString InLogDescText) const;

public:
	const FGsSchemaBMShopTabInfo* GetTabTableDataByTabID(const int32 InTabID) const;
	static FString GetCalcRemainTime(FTimespan InTimeSpan);
	bool GetPopupShopLessRemainTimeProduct(OUT FDateTime& outDateTime) const;
	BMProductId GetPopupShopLessRemainTimeProductID() const;
	bool GetPopupShopRemainTime(const BMProductId InProductID, OUT struct FDateTime& outDateTime) const;
	bool GetDisplayRemainTime(BMProductId InProductID, FString& outString) const;
	void GetAllStorageItemsDBID(OUT TArray<uint64>& outDBIDList) const;
	static bool GetMinHourDateTime(FDateTime& InTargetDataTime, const uint8 InAddedHour, FDateTime& outDateTime);
	EGsBMShopDisPlayType GetProductDisplayType(const int32 InMainTabID, const int32 InSubTabID);
	bool HasBulkSaleProduct(TArray<BMProductId> InProductIdList);
	int32 GetMaxStepByGruopID(const int32 InStepUpGroupID) const;
	static int32 GetDiscountPrice(FGsBMShopProductData* InProductData, FGsItem* InCouponItemData);
	const FGsBMShopProductData* GetBeforeStepUpProductData(const BMProductId InCurProductID);
	void CheckPopupShopSoldOut(const BMProductId InProductID);
	uint16 GetBulkSaleAmount(const BMProductId InProductID) const;
	bool GetBulkSaleState(const BMProductId InProductID) const;
	bool IsAllowBuyCashItem() const;
	// const bool IsStartInAppPurchase() { return _isStartInAppPurchase; };
	uint8 GetOpenPopupShopProductCount() const { return _popupShopProductDataList.Num(); }
	uint64 GetZpayUserDbid() const { return _zPayUserDbid; }
	Level GetZpayUserLevel() const { return _zPayUserLevel; }
	static FString GetSoundResName(const BMShopSoundType InType);

	// 자동순차구매봇 기능 관련
public:
	/** 자동순차구매봇 삭제 */
	void DeletePurchaseSequentially();

	/** 자동순차구매봇 리셋 */
	void StartPurchase(TSharedRef<FGsBMShopCheckValidProduct> InBMShopCheckValidProduct);

	/** 자동순차구매봇 횟수 증가 */
	void CountUpPurchaseSequentially();

	/** 자동순차구매봇 횟수 반환 */
	TOptional<ItemAmount> GetRemainCountPurchaseSequentially() const;

	/**
	 * 자동순차구매봇 1회 결제 완료될 때 마다 실행시킴.
	 * 여기서 카운트업을 하지 않음. 카운트업과 후처리 시점이 다를 수 있으므로 분리함.
	 * - 횟수가 남은 경우, 구매를 다시 실행하면서 true 반환.
	 * - 횟수가 0이 된 경우, 아무것도 하지 않고 false 반환.
	 * - 자동순차구매중이 아닌 경우, 아무것도 하지 않고 false 반환.
	 */
	bool RunNextPurchaseSequentially();

public:
	void OnReconectionEndBmShop();

public:
	void AddBMShopHandlerItemDetailMsg();
	void RemoveBMShopHandlerItemDetailMsg();

public:
	uint64 GetReserveCouponItemDBID() const	{ return _reserveCouponDBID; }
	uint8 GetRetryHiveCallbackCount() const { return _retryCount; }
	bool GetIsUseIAPV4SheetsLogFlag() const { return _isActiveIAPV4Log; }
	FString GetZpayModeUserServerName() const { return _zPayModeUserServerName; }
	FString GetZpayModeUserName() const { return _zPayModeUserName; }

private:
	void InitTableData();
	void InitProductTableData();
	void InitChildProductData();
	void InitStepUpProductData();
	void InitTabData();
	void LoadProductTableData();
	void BeginPromotionProc();
	void InitCouponData();

public:
	void CheckHiveProductData(const bool InIsFromBMShopEnter = false) const;

private:
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);
	void OnEmptyReservedQueueUI(const struct IGsMessageParam* InParam);
	void OnClickTitleStart(const struct IGsMessageParam* InParam);
	void OnEmptyReservedTutorialQueueUI(const struct IGsMessageParam* InParam);

private:
	void SortTab(OUT TArray<const FGsSchemaBMShopTabInfo*>& outTabInfo);
	static const FGsSchemaBMShopTabInfo* GetTabInfo(const TArray<const FGsSchemaBMShopTabInfo*>& InTabInfoList, const int32 InTabID);

private:
	static bool GetRemainTimeUnit(RemainTimeUnitType _type, FTimespan InTimeSpan, FString& outText);

public:
	bool IsExpiredCouponItem();

	
public:
	// IGsIsRedDotCheck override
	virtual bool CheckRedDot(bool bInUpdate = true) override;

public:
	void PrintHiveProductInfo() const;

public:
	bool IsValidHiveProduct() const;
	bool IsUseHiveProductPlatform() const;


public:
	/** BM 상점 상품 구매 시, 하이브로 유저 추적용 이벤트 전송 */
	void SendPurchaseEventToHive(const TArray<BMProductId>& InProductIdList);

	/** BM 상점 상품 복구 시, 하이브로 유저 추적용 이벤트 전송 (복구를 하면 구매가 진행된다고 함) */
	void SendRestoreEventToHive(const FString&& InMarketPID);

	bool OnClickUserPgComplete(const bool InForce = false);

	void SetWaitForReqPgUnconsumedPurchase(const bool InWait);

#if WITH_EDITOR
	/** 치트에서 사용하는 함수로 하이브상품 시뮬레이션을 위해, 하이브상품목록을 클리어함. */
	void EditorClearHiveProductInfos();
#endif


	// 2023/10/26 PKT - 서버 이전 Handler
public:
	FGsServerMigrateHandler* GetServerMigrateHandler() const;
};

#define GBMShop() UGsScopeHolder::GetGlobalManagerFType<FGsBMShopManager>(UGsScopeGlobal::EManagerType::BMShop)
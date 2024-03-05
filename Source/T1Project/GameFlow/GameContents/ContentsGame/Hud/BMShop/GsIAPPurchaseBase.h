// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HIVE_IAPV4.h"
#include "HIVE_Promotion.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsMessageParam.h"

using namespace std;
using namespace hive;

struct IGsMessageParam;
struct FGsSharedParam;

class FGsIAPPurchaseBase
{
public:
	enum HiveFailedCallbackType : uint8
	{
		None_Failed = 0,
		MarketInit_Failed,
		GetProductInfo_Failed,
		Restore_Failed,
		Purchase_Failed,
		TransactionFinishMulti_Failed,
		TransactionFinishSingle_Failed,
	};
	enum CallbackResultType : uint8
	{
		Callback_MarketInit = 0 ,
		Callback_GetProductInfo,

		Callback_Max,
	};

	/** 결제 진행 도중에 화면 블럭 타입 */
	enum EGsScreenOnPaying : uint8
	{
		// 화면을 막고, 로딩창을 띄움.
		Block,
		// 확인 버튼을 출력하고, 확인 클릭 시, 서버로 결제 확인 요청
		Popup,
	};

protected:
	TArray<TPair<MessageContentStoreBM, FDelegateHandle>> _bmshopDelegates;
	TArray<IAPV4Type> _purchaseMarketTypeList;
	TArray<IAPV4Product> _hiveProductList;
	TMap<CallbackResultType, bool> _hiveCallbackResultMap;

public:
	int32 _hiveErrorCode = 0;
	int64 _hiveCode = 0;
	HiveFailedCallbackType _failedCallbackType = HiveFailedCallbackType::None_Failed;
	FString _hiveErrorDesc = "";
	bool _isUserCallRestoreFlag = false;

#if PLATFORM_ANDROID || PLATFORM_IOS
	const EGsScreenOnPaying _screenOnPaying = EGsScreenOnPaying::Block;
	const bool _sendPgPurchaseWnd = false;
#elif PLATFORM_WINDOWS
	const EGsScreenOnPaying _screenOnPaying = EGsScreenOnPaying::Popup;
	const bool _sendPgPurchaseWnd = true;
#else
	const EGsScreenOnPaying _screenOnPaying = EGsScreenOnPaying::Popup;
	const bool _sendPgPurchaseWnd = true;
#endif

protected:
	DevicePlatformType _ePlatformType;

public:
	FGsIAPPurchaseBase();
	virtual ~FGsIAPPurchaseBase();

protected:
	void FailedHiveCallback(const int32 InErrorCode, const int64 InCode, HiveFailedCallbackType InFailedType, FString InDesc = "");
	void FailedHiveCallback(TArray<int32> InErrorCodeList, TArray<int64> InCodeList, HiveFailedCallbackType InFailedType, FString InDesc = "");
	//void RestoreProc(TArray<FString> InMarketPIDList, TArray<std::string> InBypassInfoList);

public:
	void InitHiveIAP();
	void OnBMShopHiveMarketConnect(FGsSharedParam& InMessageParam);
	void GetHiveProductInfo();
	void OnBMShopHiveGetProductInfo(FGsSharedParam& InMessageParam);
	void CheckRestore(bool InIsFromUserCheck);
	void OnBMShopHiveRestore(FGsSharedParam& InMessageParam);
	void CheckRestoreSubscription();
	void OnBMShopHiveRestoreSubscription(FGsSharedParam& InMessageParam);
	void OnTransactionFinishMulti(TArray<std::string> InMarketPIDList);
	void OnTransactionFinishSingle(std::string InPid);
	void GetSubscriptionProductInfo();
	void OnBMShopHiveGetSubscriptionProductInfo(FGsSharedParam& InMessageParam);
	void ShowPopupShopCustomContents(const int32 InHiveBannerID);

public:
	bool IsValidState() const;

public:
	void SetCallbackResultFlag(const CallbackResultType InType, const bool InState);

public:
	void Initialize(const DevicePlatformType InCurrentPlatformType);
	void TryPurchase(const TArray<BMProductIdAmountPair>& InAmountPairList, const bool InBulkSale, const uint64 InCouponItemDbid = 0);

public:
	void TryHivePurchase(TArray<BMProductIdAmountPair> InAmountPairList, const uint64 InIdentifier, const bool InIsBulkSale, uint64 InCouponItemDbid);
	void TryPurchaseSubscription();

public:
	void GetMarketTypeList(OUT TArray<IAPV4Type>& outTypeList);
	void GetHiveProductInfoList(OUT TArray<IAPV4Product>& outProductInfoList);
	bool GetHiveCallbackResultFalg(const CallbackResultType InType) const;

public:
	DevicePlatformType GetCurrenctPlatformType() const { return _ePlatformType; }

#if WITH_EDITOR
public:
	/** 치트에서 사용하는 함수로 하이브상품 시뮬레이션을 위해, 하이브상품목록을 클리어함. */
	void EditorClearHiveProductInfos();
#endif

};

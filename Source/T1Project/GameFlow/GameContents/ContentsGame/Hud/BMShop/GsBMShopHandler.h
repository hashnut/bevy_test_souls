// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HIVE_IAPV4.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"

#include "GsIAPPurchaseBase.h"
#include "HIVE_ResultAPI.h"

using namespace hive;

struct IGsMessageParam;
class FGsBMShopProductData;
struct FGsBMShopCheckValidProduct;
/**
 * BmShop handler
 */
class FGsBMShopHandler final
{
private:
	TArray<TPair<MessageContentStoreBM, FDelegateHandle>> _bmShopMsgHandlerList;

public:
	TUniquePtr<FGsIAPPurchaseBase> _iapPurchaseBase = nullptr;

private:
	TArray<MsgGameObjHandle> _listGameObjHandleDelegate;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDelegate;
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_listItemDetailDelegate;
	TPair<MessageUserInfo, FDelegateHandle> _userInfoMsgHandler;
	TPair<MessageUI, FDelegateHandle> _uiMsgHandler;

public:
	void Init();
	void Close();
	void OnReconnection();

public:
	bool IsValidHiveProduct() const;
	bool IsUseHiveProductPlatform() const;

public:
	void RemoveItemDetailMsg();
	void AddItemDetailMsg();

public:
	bool IsReceivedHiveCallback(const FGsIAPPurchaseBase::CallbackResultType InType) const;

private:
	void InitMsg();
	void RemoveMsg();

private:
	void OpenBuyPopup(FGsSharedParam& InMessageParam);
	void OpenBulkSalePopup(FGsSharedParam& InMessageParam);
	void OpenCouponListPopup(FGsSharedParam& InMessageParam);
	void OpenGroupDetailPopup(FGsSharedParam& InMessageParam);
	void SuccessePurchaseItem(FGsSharedParam& InMessageParam);
	void SendTransactionFinishSingle(FGsSharedParam& InMessageParam);
	void ValidCheck(FGsSharedParam& InMessageParam);
	void GetStorageItem(FGsSharedParam& InMessageParam);
	void RestoreCheck(FGsSharedParam& InMessageParam);
	void UserCheckRestore(FGsSharedParam& InMessageParam);
	void OpenBMShopWithTab(FGsSharedParam& InMessageParam);
	void OpenBMShopWithSpecialTab(FGsSharedParam& InMessageParam);
	void OpenBMShopValidHiveProduct(FGsSharedParam& InMessageParam);
	void OpenBMShopHUD(FGsSharedParam& InMessageParam);
	void OpenPopupShop(FGsSharedParam& InMessageParam);
	void OpenPopupShopPromotion(FGsSharedParam& InMessageParam);
	void NotingToRestore(FGsSharedParam& InMessageParam);
	void DisplayNotingToRestore(FGsSharedParam& InMessageParam);
	void AckSearchZpayUser(FGsSharedParam& InMessageParam);
	void NotifySearchZpayUser(FGsSharedParam& InMessageParam);
	void ServerValidCheckResult(FGsSharedParam& InMessageParam);
	void MarketConnection(FGsSharedParam& InMessageParam);
	void HiveProductInfo(FGsSharedParam& InMessageParam);
	void OnHiveCallbackBlockUI(FGsSharedParam& InMessageParam);
	void OnHiveCallbackCloseBlockUI(FGsSharedParam& InMessageParam);
	// void TransactionFinish(FGsSharedParam& InMessageParam);
	void OnFailedHiveCallback(FGsSharedParam& InMessageParam);
	void OnFailedHiveCallbackMulti(FGsSharedParam& InMessageParam);
	void FinishedPeriodItem(const struct IGsMessageParam* InMessageParam);
	void OnLocalSpawnMeComplete(const struct IGsMessageParam* InMessageParam);
	void SendAnlyticsEvent(FGsSharedParam& InParam);
	void HiveNeedExitCallback(FGsSharedParam& InParam);
	void CloseShopByBlock(FGsSharedParam& InParam);

private:
	void OpenItemDetail(const struct IGsMessageParam* InMessageParam);
	void OpenItemDetailTID(const struct IGsMessageParam* InMessageParam);

private:
	bool ValidCheckInter(const FGsBMShopCheckValidProduct& InParam);
	// bool IsBackToTitleError();

private:
	void OnAddItemList(struct FGsItemMessageParamBase& InParam);
	void OnUpdateItem(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItem(struct FGsItemMessageParamBase& InParam);

private:
	void UseCouponCallback(const TArray<BMProductIdAmountPair>& InPairList, bool InBulkSale, const uint64 InCouponItemDbid);

private:
	bool CouponValidCheck();
	void CouponValidCheck(FGsSharedParam& InMessageParam);
	void OpenConfirmCouponPopup(const uint64 InCouponItemDbid, const FGsBMShopCheckValidProduct* InParamData);


private:
	void OnUpdateCurrency(uint64 InData);
	void OnUpdateUserLevel(const struct IGsMessageParam* inParam);


private:
	void OnShowPurchaseFailPopup(FGsBMShopProductData* InProductData, uint64 InTotalCost, Currency InOwnedAmount);
	void MoveToTargetTab();

private:
	void OnShowHiveErrorPopup(const FGsIAPPurchaseBase::HiveFailedCallbackType InHiveFailedCallbackType, const hive::ResultAPI::Code InCode, const int32 InErrorCode);

};

FString LexToString(const struct FGsBMShopSalesTimeOver& InParam);
FString LexToString(const struct FGsBMShopOpenPopupBuy& InParam);
FString LexToString(const struct FGsBMShopOpenPopupGroupDetail& InParam);
//FString LexToString(const struct FGsBMShopInvalidateProduct& InParam);
//FString LexToString(const struct FGsBMShopInvalidateProductList& InParam);
//FString LexToString(const struct FGsBMShopOpenBMShopHud& InParam);
//FString LexToString(const struct FGsBMShopPurchaseRestoreCheck& InParam);
//FString LexToString(const struct FGsBMShopKickZpayUser& InParam);
//FString LexToString(const struct FGsBMShopInvalidateStorage& InParam);
//FString LexToString(const struct FGsBMShopInvalidatePopupShopProduct& InParam);
//FString LexToString(const struct FGsBMShopSearchZpayUserNotify& InParam);
//FString LexToString(const struct FGsBMShopDisplayNothingToRestore& InParam);
//FString LexToString(const struct FGsBMShopHiveBlockUiClose& InParam);
//FString LexToString(const struct FGsBMShopCloseBMShopByBlock& InParam);
//FString LexToString(const struct FGsBMShopUserCheckRestore& InParam);
//FString LexToString(const struct FGsBMShopNothingToRestore& InParam);
//FString LexToString(const struct FGsBMShopHiveMarketConnection& InParam);
//FString LexToString(const struct FGsBMShopHiveProudctListInfo& InParam);
//FString LexToString(const struct FGsBMShopHiveBlockUiOpen& InParam);
FString LexToString(const struct FGsBMShopHiveError& InParam);
FString LexToString(const struct FGsBMShopHiveErrors& InParam);
//FString LexToString(const struct FGsBMShopSendAnalyticsEvent& InParam);
//FString LexToString(const struct FGsBMShopHiveNeedExit& InParam);
//FString LexToString(const struct FGsBMShopInvalidateServerMigrage& InParam);
//FString LexToString(const struct FGsBMShopPopupProductTimeOver& InParam);
FString LexToString(const struct FGsBMShopCheckValidProduct& InParam);
FString LexToString(const struct FGsBMShopCheckValidCoupon& InParam);
FString LexToString(const struct FGsBMShopAcceptStorage& InParam);
FString LexToString(const struct FGsBMShopChangeTab& InParam);
FString LexToString(const struct FGsBMShopOpenSpecialTab& InParam);
FString LexToString(const struct FGsBMShopOpenTab& InParam);
FString LexToString(const struct FGsBMShopChangeTabFromId& InParam);
FString LexToString(const struct FGsBMShopChangeFocusingProduct& InParam);
FString LexToString(const struct FGsBMShopPurchaseSuccess& InParam);
FString LexToString(const struct FGsBMShopSendTransactionFinishSingle& InParam);
FString LexToString(const struct FGsBMShopOpenBMShopValidHive& InParam);
FString LexToString(const struct FGsBMShopCheckValidProductResult& InParam);
FString LexToString(const struct FGsBMShopOpenBulkSale& InParam);
FString LexToString(const struct FGsBMShopOpenPopupShop& InParam);
FString LexToString(const struct FGsBMShopOpenBMShopPromotion& InParam);
FString LexToString(const struct FGsBMShopOpenCouponList& InParam);
FString LexToString(const struct FGsBMShopZpayUserAck& InParam);
FString LexToString(const struct FGsBMShopHiveMarketConnect& InParam);
FString LexToString(const struct FGsBMShopHiveGetProductInfo& InParam);
FString LexToString(const struct FGsBMShopHiveGetSubscriptionProductInfo& InParam);
FString LexToString(const struct FGsBMShopHiveRestore& InParam);
FString LexToString(const struct FGsBMShopHiveRestoreSubscription& InParam);
FString LexToString(const struct FGsBMShopHivePurchase& InParam);

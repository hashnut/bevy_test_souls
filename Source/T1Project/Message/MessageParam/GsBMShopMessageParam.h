#pragma once

#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project/GameFlow/GameContents/ContentsGame/Hud/BMShop/GsIAPPurchaseBase.h"
#include "HIVE_IAPV4.h"
#include "T1Project/GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopHandler.h"
#include "Message/GsMessageContents.h"

template<MessageContentStoreBM InMessageId>
struct TGsBMShopMessageParamBase : public FGsSharedParamData
{
	static constexpr MessageContentStoreBM MESSAGE_ID = InMessageId;

public:
	TGsBMShopMessageParamBase()
	{
	}

	virtual ~TGsBMShopMessageParamBase()
	{
	}

public:
	/** 실제 메세지 발생은 이 함수를 호출해야 발생함. */
	virtual bool Send()
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return false;
		}

		// GSLOG(Log, TEXT("%s"), *LexToString(*this));

		FGsSharedParam SharedParam(AsShared());
		MessageHolder->GetBMShop().SendMessage(MESSAGE_ID, MoveTemp(SharedParam));
		return true;
	}

	bool SendAsync()
	{
		GSLOG(Log, TEXT("SendAsync()"));

		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return false;
		}

		GSLOG(Log, TEXT("FGsSharedParam SharedParam(AsShared());"));

		FGsSharedParam SharedParam(AsShared());

		GSLOG(Log, TEXT("MessageHolder->GetBMShop().SendMessageAsync();"));

		MessageHolder->GetBMShop().SendMessageAsync(MESSAGE_ID, MoveTemp(SharedParam));
		return true;
	}

	template<typename TMessageParam>
	static bool StaticSendAsync(TSharedRef<TMessageParam> InMessageParam)
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr == MessageHolder)
		{
			GSLOG(Error, TEXT("nullptr == MessageHolder"));
			return false;
		}

		MessageHolder->GetBMShop().SendMessageAsync(MESSAGE_ID, FGsSharedParam(InMessageParam));
		return true;
	}
};

struct FGsBMShopSalesTimeOver final : public TGsBMShopMessageParamBase<MessageContentStoreBM::SALES_TIME_OVER>
{
	BMProductId _productID;

	explicit FGsBMShopSalesTimeOver(BMProductId InProductID)
		: _productID(InProductID)
	{
	}
};

// Open
struct FGsBMShopOpenPopupBuy final : public TGsBMShopMessageParamBase<MessageContentStoreBM::OPEN_POPUP_BUY>
{
	BMProductId _productID;

	explicit FGsBMShopOpenPopupBuy(BMProductId InProductID)
		: _productID(InProductID)
	{
	}
};

struct FGsBMShopOpenPopupGroupDetail final : public TGsBMShopMessageParamBase<MessageContentStoreBM::OPEN_POPUP_GROUP_DETAIL>
{
	ItemId _itemId = 0;

	explicit FGsBMShopOpenPopupGroupDetail(ItemId itemId)
		: _itemId(itemId)
	{
	}
};

using FGsBMShopInvalidateProduct = TGsBMShopMessageParamBase<MessageContentStoreBM::INVALIDATE_PRODUCT>;
using FGsBMShopInvalidateProductList = TGsBMShopMessageParamBase<MessageContentStoreBM::INVALIDATE_PRODUCT_LIST>;
using FGsBMShopOpenBMShopHud = TGsBMShopMessageParamBase<MessageContentStoreBM::OPEN_BMSHOP_HUD>;
using FGsBMShopPurchaseRestoreCheck = TGsBMShopMessageParamBase<MessageContentStoreBM::PURCHASE_RESTORE_CHECK>;
using FGsBMShopKickZpayUser = TGsBMShopMessageParamBase<MessageContentStoreBM::KICK_Z_PAY_USER>;
using FGsBMShopInvalidateStorage = TGsBMShopMessageParamBase<MessageContentStoreBM::INVALIDATE_STORAGE>;
using FGsBMShopInvalidatePopupShopProduct = TGsBMShopMessageParamBase<MessageContentStoreBM::INVALIDATE_POPUP_SHOP_PRODUCT>;
using FGsBMShopSearchZpayUserNotify = TGsBMShopMessageParamBase<MessageContentStoreBM::SEARCH_Z_PAY_USER_NOTIFY>;
using FGsBMShopDisplayNothingToRestore = TGsBMShopMessageParamBase<MessageContentStoreBM::DISPLAY_NOTHING_TO_RESTORE>;
using FGsBMShopHiveBlockUiClose = TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_BLOCK_UI_CLOSE>;
using FGsBMShopCloseBMShopByBlock = TGsBMShopMessageParamBase<MessageContentStoreBM::CLOSE_BMSHOP_BY_BLOCK>;
using FGsBMShopUserCheckRestore = TGsBMShopMessageParamBase<MessageContentStoreBM::USER_CHECK_RESTORE>;
using FGsBMShopNothingToRestore = TGsBMShopMessageParamBase<MessageContentStoreBM::NOTHING_TO_RESTORE>;
using FGsBMShopHiveMarketConnection = TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_MARKET_CONNECTION>;
using FGsBMShopHiveProudctListInfo = TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_PRODUCT_LIST_INFO>;
using FGsBMShopHiveBlockUiOpen = TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_BLOCK_UI_OPEN>;

struct FGsBMShopHiveError final : public TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_ERROR>
{
	const DevicePlatformType _devicePlatformType;
	const hive::ResultAPI::Code _code;
	const int32 _errorCode;
	const FGsIAPPurchaseBase::HiveFailedCallbackType _hiveFailedCallbackType;
	const FString _desc;

	explicit FGsBMShopHiveError(DevicePlatformType InDevicePlatformType, hive::ResultAPI::Code InCode, int32 InErrorCode, FGsIAPPurchaseBase::HiveFailedCallbackType InHiveFailedCallbackType, FString InDesc = TEXT(""))
		: _devicePlatformType(InDevicePlatformType)
		, _code(InCode)
		, _errorCode(InErrorCode)
		, _hiveFailedCallbackType(InHiveFailedCallbackType)
		, _desc(MoveTemp(InDesc))
	{
	}
};

struct FGsBMShopHiveErrors final : public TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_ERRORS>
{
	const DevicePlatformType _devicePlatformType;
	const hive::ResultAPI::Code _code;
	const int32 _errorCode;
	const FGsIAPPurchaseBase::HiveFailedCallbackType _hiveFailedCallbackType;

	explicit FGsBMShopHiveErrors(DevicePlatformType InDevicePlatformType, hive::ResultAPI::Code InCode, int32 InErrorCode, FGsIAPPurchaseBase::HiveFailedCallbackType InHiveFailedCallbackType)
		: _devicePlatformType(InDevicePlatformType)
		, _code(InCode)
		, _errorCode(InErrorCode)
		, _hiveFailedCallbackType(InHiveFailedCallbackType)
	{
	}
};

using FGsBMShopSendAnalyticsEvent = TGsBMShopMessageParamBase<MessageContentStoreBM::SEND_ANLYTICS_EVENT>;
using FGsBMShopHiveNeedExit = TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_NEED_EXIT>;
using FGsBMShopInvalidateServerMigrage = TGsBMShopMessageParamBase<MessageContentStoreBM::INVALIDATE_SERVERMIGRATE>;
using FGsBMShopPopupProductTimeOver = TGsBMShopMessageParamBase<MessageContentStoreBM::POPUP_PRODUCT_TIME_OVER>;

struct FGsBMShopCheckValidProduct final : public TGsBMShopMessageParamBase<MessageContentStoreBM::CHECK_VALID_PRODUCT>
{
	TArray<BMProductIdAmountPair> _pairList;
	bool _isBulkSale;
	bool _isUseDiscount;
	Currency _discountCurrencyAmount;

	explicit FGsBMShopCheckValidProduct(TArray<BMProductIdAmountPair> pairList, const bool isBulkSale, bool isUseDiscount, Currency discountCurrencyAmount)
		: _pairList(MoveTemp(pairList))
		, _isBulkSale(isBulkSale)
		, _isUseDiscount(isUseDiscount)
		, _discountCurrencyAmount(discountCurrencyAmount)
	{
	}
};

struct FGsBMShopCheckValidCoupon final : public TGsBMShopMessageParamBase<MessageContentStoreBM::CHECK_VALID_COUPON>
{
	BMProductId _productID;

	explicit FGsBMShopCheckValidCoupon(BMProductId productID)
		: _productID(productID)
	{
	}
};

struct FGsBMShopAcceptStorage final : public TGsBMShopMessageParamBase<MessageContentStoreBM::ACCEPT_STORAGE>
{
	uint64 _storageItemDBID;

	explicit FGsBMShopAcceptStorage(const uint64 storageItemDBID)
		: _storageItemDBID(storageItemDBID)
	{
	}
};

struct FGsBMShopChangeTab final : public TGsBMShopMessageParamBase<MessageContentStoreBM::CHANGE_TAB>
{
	EGsBMShopSpecialTab _tabType;

	explicit FGsBMShopChangeTab(const EGsBMShopSpecialTab tabType)
		: _tabType(tabType)
	{
	}
};

struct FGsBMShopOpenSpecialTab final : public TGsBMShopMessageParamBase<MessageContentStoreBM::OPEN_SPECIAL_TAB>
{
	EGsBMShopSpecialTab _tabType;

	explicit FGsBMShopOpenSpecialTab(const EGsBMShopSpecialTab tabType)
		: _tabType(tabType)
	{
	}
};

struct FGsBMShopOpenTab final : public TGsBMShopMessageParamBase<MessageContentStoreBM::OPEN_TAB>
{
	int32 _tabID;

	explicit FGsBMShopOpenTab(int32 tabID) : _tabID(tabID)
	{
	}
};

struct FGsBMShopChangeTabFromId final : public TGsBMShopMessageParamBase<MessageContentStoreBM::CHANGE_TAB_FROM_ID>
{
	int32 _tabID;

	explicit FGsBMShopChangeTabFromId(int32 tabID) : _tabID(tabID)
	{
	}
};

struct FGsBMShopChangeFocusingProduct final : public TGsBMShopMessageParamBase<MessageContentStoreBM::CHANGE_FOCUSING_PRODUCT>
{
	int32 _mainTabID;
	int32 _subTabID;
	BMProductId _productID;

	explicit FGsBMShopChangeFocusingProduct(int32 mainTabID, int32 subTabID, BMProductId productID)
		: _mainTabID(mainTabID)
		, _subTabID(subTabID)
		, _productID(productID)
	{
	}
};

struct FGsBMShopPurchaseSuccess final : public TGsBMShopMessageParamBase<MessageContentStoreBM::PURCHASE_SUCCESS>
{
	TArray<TPair<BMProductId, ItemAmount>> _productIdList;
	bool _isBulkSale;

	explicit FGsBMShopPurchaseSuccess(TArray<TPair<BMProductId, ItemAmount>> productIdList, bool isBulkSale)
		: _productIdList(MoveTemp(productIdList))
		, _isBulkSale(isBulkSale)
	{
	}
};

struct FGsBMShopSendTransactionFinishSingle final : public TGsBMShopMessageParamBase<MessageContentStoreBM::SEND_TRANSACTION_FINISH_SINGLE>
{
	FString _pid;

	explicit FGsBMShopSendTransactionFinishSingle(FString pid)
		: _pid(pid)
	{
	}
};

struct FGsBMShopOpenBMShopValidHive final : public TGsBMShopMessageParamBase<MessageContentStoreBM::OPEN_BMSHOP_VALID_HIVE>
{
	bool _valueFlag;

	explicit FGsBMShopOpenBMShopValidHive(bool valueFlag)
		: _valueFlag(valueFlag)
	{
	}
};

struct FGsBMShopCheckValidProductResult final : public TGsBMShopMessageParamBase<MessageContentStoreBM::CHECK_VALID_RRODUCT_RESULT>
{
	TArray<BMProductIdAmountPair> _amountPairList;
	uint64 _identifier;
	bool _isBulkSale;
	uint64 _couponItemDbid;

	explicit FGsBMShopCheckValidProductResult(TArray<BMProductIdAmountPair> amountPairList, const uint64 identifier, bool isBulkSale, uint64 couponItemDbid)
		: _amountPairList(MoveTemp(amountPairList))
		, _identifier(identifier)
		, _isBulkSale(isBulkSale)
		, _couponItemDbid(couponItemDbid)
	{
	}
};

struct FGsBMShopOpenBulkSale final : public TGsBMShopMessageParamBase<MessageContentStoreBM::OPEN_BULK_SALE>
{
	TArray<BMProductIdAmountPair> _bulkSaleProductInfoList;

	explicit FGsBMShopOpenBulkSale(TArray<BMProductIdAmountPair> bulkSaleProductInfoList)
		: _bulkSaleProductInfoList(MoveTemp(bulkSaleProductInfoList))
	{
	}
};

struct FGsBMShopOpenPopupShop final : public TGsBMShopMessageParamBase<MessageContentStoreBM::OPEN_POPUP_SHOP>
{
	BMProductId _productID;

	explicit FGsBMShopOpenPopupShop(BMProductId productID)
		: _productID(productID)
	{
	}
};

struct FGsBMShopOpenBMShopPromotion final : public TGsBMShopMessageParamBase<MessageContentStoreBM::OPEN_BMSHOP_PROMOTION>
{
	BMProductId _productID;

	explicit FGsBMShopOpenBMShopPromotion(BMProductId productId)
		: _productID(productId)
	{
	}
};

struct FGsBMShopOpenCouponList final : public TGsBMShopMessageParamBase<MessageContentStoreBM::OPEN_COUPON_LIST>
{
	BMProductId _productID;

	explicit FGsBMShopOpenCouponList(const BMProductId productID)
		: _productID(productID)
	{
	}
};

struct FGsBMShopZpayUserAck final : public TGsBMShopMessageParamBase<MessageContentStoreBM::SEARCH_Z_PAY_USER_ACK>
{
	bool _isInZpayMode;

	explicit FGsBMShopZpayUserAck(const bool isInZpayMode)
		: _isInZpayMode(isInZpayMode)
	{
	}
};

struct FGsBMShopHiveMarketConnect final : public TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_MARKET_CONNECT>
{
	const bool _isSuccess;
	const hive::ResultAPI::Code _code;
	const bool _needExit;
	const int _errorCode;
	const TArray<hive::IAPV4Type> _marketIds;

	explicit FGsBMShopHiveMarketConnect(bool InIsSuccess, hive::ResultAPI::Code InCode, bool InNeedExit, int InErrorCode, TArray<hive::IAPV4Type>&& InMarketIds)
		: _isSuccess(InIsSuccess)
		, _code(InCode)
		, _needExit(InNeedExit)
		, _errorCode(InErrorCode)
		, _marketIds(MoveTemp(InMarketIds))
	{
	}
};

struct FGsBMShopHiveGetProductInfo final : public TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_GET_PRODUCT_INFO>
{
	const bool _isSuccess;
	const hive::ResultAPI::Code _code;
	const bool _needExit;
	const int _errorCode;
	const TArray<hive::IAPV4Product> _ipa4Products;

	explicit FGsBMShopHiveGetProductInfo(bool InIsSuccess, hive::ResultAPI::Code InCode, bool InNeedExit, int InErrorCode, TArray<hive::IAPV4Product>&& InIpa4Products)
		: _isSuccess(InIsSuccess)
		, _code(InCode)
		, _needExit(InNeedExit)
		, _errorCode(InErrorCode)
		, _ipa4Products(MoveTemp(InIpa4Products))
	{
	}
};

struct FGsBMShopHiveGetSubscriptionProductInfo final : public TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_GET_SUBSCRIPTION_PRODUCT_INFO>
{
	const bool _isSuccess;
	const hive::ResultAPI::Code _code;
	const int _errorCode;
	const TArray<hive::IAPV4Product> _ipa4Products;

	explicit FGsBMShopHiveGetSubscriptionProductInfo(bool InIsSuccess, hive::ResultAPI::Code InCode, int InErrorCode, TArray<hive::IAPV4Product> InIpa4Products)
		: _isSuccess(InIsSuccess)
		, _code(InCode)
		, _errorCode(InErrorCode)
		, _ipa4Products(InIpa4Products)
	{
	}
};

struct FGsBMShopHiveRestore final : public TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_RESTORE>
{
	const bool _isSuccess;
	const hive::ResultAPI::Code _code;
	const bool _needExit;
	const int _errorCode;
	const TArray<TPair<FString, std::string>> _marketIdAndBypassInfos;

	explicit FGsBMShopHiveRestore(bool InIsSuccess, hive::ResultAPI::Code InCode, bool InNeedExit, int InErrorCode, TArray<TPair<FString, std::string>> InMarketIdAndBypassInfos)
		: _isSuccess(InIsSuccess)
		, _code(InCode)
		, _needExit(InNeedExit)
		, _errorCode(InErrorCode)
		, _marketIdAndBypassInfos(InMarketIdAndBypassInfos)
	{
	}
};

struct FGsBMShopHiveRestoreSubscription final : public TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_RESTORE_SUBSCRIPTION>
{
	const bool _isSuccess;
	const hive::ResultAPI::Code _code;
	const bool _needExit;
	const int _errorCode;
	const TArray<TPair<FString, std::string>> _marketIdAndBypassInfos;

	explicit FGsBMShopHiveRestoreSubscription(bool InIsSuccess, hive::ResultAPI::Code InCode, bool InNeedExit, int InErrorCode, TArray<TPair<FString, std::string>> InMarketIdAndBypassInfos)
		: _isSuccess(InIsSuccess)
		, _code(InCode)
		, _needExit(InNeedExit)
		, _errorCode(InErrorCode)
		, _marketIdAndBypassInfos(InMarketIdAndBypassInfos)
	{
	}
};

struct FGsBMShopHivePurchase final : public TGsBMShopMessageParamBase<MessageContentStoreBM::HIVE_PURCHASE>
{
	bool _isSuccess;
	hive::ResultAPI::Code _code;
	bool _needExit;
	int _errorCode;
	FString _currentPid;
	uint64 _identifier;
	std::string _bypassInfo;

	/*
	explicit FGsBMShopHivePurchase(bool InIsSuccess, hive::ResultAPI::Code InCode, bool InNeedExit, int InErrorCode, const FString InCurrentPid, uint64 InIdentifier, const std::string InBypassInfo)
		: _isSuccess(InIsSuccess)
		, _code(InCode)
		, _needExit(InNeedExit)
		, _errorCode(InErrorCode)
		, _currentPid(InCurrentPid)
		, _identifier(InIdentifier)
		, _bypassInfo(InBypassInfo)
	{
	}
	*/

	void Set(const bool InIsSuccess, const hive::ResultAPI::Code InCode, const bool InNeedExit, const int InErrorCode, const FString& InCurrentPid, const uint64& InIdentifier, const std::string& InBypassInfo)
	{
		_isSuccess = InIsSuccess;
		_code = InCode;
		_needExit = InNeedExit;
		_errorCode = InErrorCode;
		_currentPid = InCurrentPid;
		_identifier = InIdentifier;
		_bypassInfo = InBypassInfo;
	}
};

struct FGsMigrationNameChangeMessageParam final : public IGsMessageParam
{
	enum class Type { UserName, GuildName };
	Type _type{ Type::UserName };
	UserDBId _userDbId{ 0 };
	FText _curName;

	FGsMigrationNameChangeMessageParam() = default;
	virtual ~FGsMigrationNameChangeMessageParam() = default;

	explicit FGsMigrationNameChangeMessageParam(const UserDBId dbId, const FText& curName, Type type = Type::UserName)
		:_type(type), _userDbId(dbId), _curName(curName)
	{
	}
};

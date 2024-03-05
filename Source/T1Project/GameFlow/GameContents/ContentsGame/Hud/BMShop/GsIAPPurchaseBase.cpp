// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsIAPPurchaseBase.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Message/GsMessageContents.h"

#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsBMShopMessageParam.h"

#include "Net/GsNetSendServiceBMShop.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Hive/GsHiveEventMsg.h"

#include "T1Project.h"
#include "Message/MessageParam/GsMessageParam.h"

FGsIAPPurchaseBase::FGsIAPPurchaseBase()
{
}

FGsIAPPurchaseBase::~FGsIAPPurchaseBase()
{
	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr != MessageHolder)
		{
			for (const TPair<MessageContentStoreBM, FDelegateHandle>& BmshopDelegate : _bmshopDelegates)
			{
				MessageHolder->GetBMShop().Remove(BmshopDelegate);
			}
		}

		_bmshopDelegates.Empty();
	}

	_hiveProductList.Empty();
	_purchaseMarketTypeList.Empty();
	_hiveCallbackResultMap.Empty();
	_ePlatformType = DevicePlatformType::NONE;
}

void FGsIAPPurchaseBase::Initialize(const DevicePlatformType InCurrentPlatformType)
{
	_hiveCallbackResultMap.Empty();
	_hiveCallbackResultMap.Emplace(CallbackResultType::Callback_MarketInit, false);
	_hiveCallbackResultMap.Emplace(CallbackResultType::Callback_GetProductInfo, false);

	_isUserCallRestoreFlag = false;
	_hiveErrorCode = 0;
	_hiveCode = 0;
	_failedCallbackType = HiveFailedCallbackType::None_Failed;
	_ePlatformType = InCurrentPlatformType;

	GSLOG(Log, TEXT("0 _ePlatformType: %d, InCurrentPlatformType: %d"), _ePlatformType, InCurrentPlatformType);

	{
		FGsMessageHolder* MessageHolder = GMessage();
		if (nullptr != MessageHolder)
		{
			_bmshopDelegates.Emplace(MessageHolder->GetBMShop().AddRaw(FGsBMShopHiveMarketConnect::MESSAGE_ID, this, &FGsIAPPurchaseBase::OnBMShopHiveMarketConnect));
			_bmshopDelegates.Emplace(MessageHolder->GetBMShop().AddRaw(FGsBMShopHiveGetProductInfo::MESSAGE_ID, this, &FGsIAPPurchaseBase::OnBMShopHiveGetProductInfo));
			_bmshopDelegates.Emplace(MessageHolder->GetBMShop().AddRaw(FGsBMShopHiveRestore::MESSAGE_ID, this, &FGsIAPPurchaseBase::OnBMShopHiveRestore));
			_bmshopDelegates.Emplace(MessageHolder->GetBMShop().AddRaw(FGsBMShopHiveRestoreSubscription::MESSAGE_ID, this, &FGsIAPPurchaseBase::OnBMShopHiveRestoreSubscription));
			_bmshopDelegates.Emplace(MessageHolder->GetBMShop().AddRaw(FGsBMShopHiveGetSubscriptionProductInfo::MESSAGE_ID, this, &FGsIAPPurchaseBase::OnBMShopHiveGetSubscriptionProductInfo));
		}
	}
}

void FGsIAPPurchaseBase::InitHiveIAP()
{
	GSLOG(Warning, TEXT("[FGsIAPPurchaseBase::InitHiveIAP]"));

	if (_hiveCallbackResultMap.Contains(CallbackResultType::Callback_MarketInit))
	{
		bool flag = _hiveCallbackResultMap[CallbackResultType::Callback_MarketInit];
		if (flag)
		{
			GSLOG(Error, TEXT("[FGsIAPPurchaseBase::InitHiveIAP] - callback result flag true"));
			return;
		}
		else
		{
			GSLOG(Error, TEXT("[FGsIAPPurchaseBase::InitHiveIAP] - flag Change : true "));
			_hiveCallbackResultMap[CallbackResultType::Callback_MarketInit] = true;
		}
	}
	_purchaseMarketTypeList.Empty();

	// FGsUIHelper::ShowBlockUI 호출해도 하이브 콜백 받기 전에 게임쓰레드에서 다른 패킷 응답딴에서 
	// HideBlockUI() 를 호출하기때문에 블록거는게 의미가 없음. ( blockUI 가 개별로 있지않기 떄문에 )
	//FGsUIHelper::ShowBlockUI();

	//GBMShop()->OnIAPV4SheetsLog(TEXT("InitHiveIAP"));

	IAPV4::marketConnect([](ResultAPI const& InResult, std::vector< IAPV4Type > const& InMarketIDs)
		{
			GSLOG(Error, TEXT("[IAPV4::marketConnect] marketConnect Call Result"));

			TArray<hive::IAPV4Type> MarketIds;
			for (const hive::IAPV4Type& It : InMarketIDs)
			{
				MarketIds.Emplace(It);
			}

			TSharedRef<FGsBMShopHiveMarketConnect> BMShopHiveMarketConnect = MakeShared<FGsBMShopHiveMarketConnect>(InResult.isSuccess(), InResult.code, InResult.needExit(), InResult.errorCode, MoveTemp(MarketIds));
			GSLOG(Log, TEXT("%s"), *LexToString(*BMShopHiveMarketConnect));
			FGsBMShopHiveMarketConnect::StaticSendAsync(BMShopHiveMarketConnect); // BMShopHiveMarketConnect->SendAsync();
		});
}

void FGsIAPPurchaseBase::OnBMShopHiveMarketConnect(FGsSharedParam& InMessageParam)
{
	const FGsBMShopHiveMarketConnect* Param = InMessageParam.GetParam<const FGsBMShopHiveMarketConnect>();
	if (nullptr == Param)
	{
		return;
	}

	GSLOG(Log, TEXT("[FGsIAPPurchaseBase::OnBMShopHiveMarketConnect] Param: %s"), *LexToString(*Param));

	if (_hiveCallbackResultMap.Contains(CallbackResultType::Callback_MarketInit))
	{
		_hiveCallbackResultMap[CallbackResultType::Callback_MarketInit] = false;
		GSLOG(Log, TEXT("[hive::IAPV4::marketConnect] Callback Flag Reset"));
	}

	if (Param->_isSuccess)
	{
		GSLOG(Log, TEXT("[hive::IAPV4::marketConnect] marketConnect Success. Param->_marketIds.Num(): %d"), Param->_marketIds.Num());

		if (0 >= Param->_marketIds.Num())
		{
			MakeShared<FGsBMShopHiveError>(_ePlatformType, Param->_code, Param->_errorCode, HiveFailedCallbackType::MarketInit_Failed, TEXT("InResult.isSuccess() but 0 >= Param->_marketIds.Num()"))->Send();
			return;
		}

		_purchaseMarketTypeList.Append(Param->_marketIds);
		MakeShared<FGsBMShopHiveMarketConnection>()->Send();
	}
	else
	{
		if (Param->_needExit)
		{
			MakeShared<FGsBMShopHiveNeedExit>()->Send();
			return;
		}

		GSLOG(Warning, TEXT("[hive::IAPV4::marketConnect] (int32)InResult.code: %d, InResult.errorCode: %d"), Param->_code, Param->_errorCode);

		MakeShared<FGsBMShopHiveError>(_ePlatformType, Param->_code, Param->_errorCode, HiveFailedCallbackType::MarketInit_Failed, TEXT("InResult.isSuccess() but 0 >= Param->_marketIds.Num()"))->Send();
	}
}

void FGsIAPPurchaseBase::GetHiveProductInfo()
{
	GSLOG(Warning, TEXT("[FGsIAPPurchaseBase::GetHiveProductInfo]"));

	if (_hiveCallbackResultMap.Contains(CallbackResultType::Callback_GetProductInfo))
	{
		bool flag = _hiveCallbackResultMap[CallbackResultType::Callback_GetProductInfo];
		if (flag)
		{
			GSLOG(Error, TEXT("[FGsIAPPurchaseBase::GetHiveProductInfo] - callback result flag true"));
			return;
		}
		else
		{
			GSLOG(Error, TEXT("[FGsIAPPurchaseBase::GetHiveProductInfo] - flag Change : true "));
			_hiveCallbackResultMap[CallbackResultType::Callback_GetProductInfo] = true;
		}
	}

	if (0 < _hiveProductList.Num())
	{
		_hiveProductList.Empty();
	}

	// FGsUIHelper::ShowBlockUI 호출해도 하이브 콜백 받기 전에 게임쓰레드에서 다른 패킷 응답딴에서 
	// HideBlockUI() 를 호출하기때문에 블록거는게 의미가 없음. ( blockUI 가 개별로 있지않기 떄문에 )
	//FGsUIHelper::ShowBlockUI();

	//GBMShop()->OnIAPV4SheetsLog(TEXT("GetHiveProductInfo"));

	IAPV4::getProductInfo([&, this](ResultAPI const& InResult, std::vector<class IAPV4Product> const& IniapV4ProductList, unsigned int Inbalance)
		{
			GSLOG(Error, TEXT("[IAPV4::getProductInfo] getProductInfo Call Result"));

			TArray<hive::IAPV4Product> Ipa4Products;
			for (const hive::IAPV4Product& It : IniapV4ProductList)
			{
				Ipa4Products.Emplace(It);
			}

			TSharedRef<FGsBMShopHiveGetProductInfo> BMShopHiveGetProductInfo = MakeShared<FGsBMShopHiveGetProductInfo>(InResult.isSuccess(), InResult.code, InResult.needExit(), InResult.errorCode, MoveTemp(Ipa4Products));
			GSLOG(Log, TEXT("%s"), *LexToString(*BMShopHiveGetProductInfo));
			// BMShopHiveGetProductInfo->SendAsync();
			FGsBMShopHiveGetProductInfo::StaticSendAsync(BMShopHiveGetProductInfo);
		});
}

void FGsIAPPurchaseBase::OnBMShopHiveGetProductInfo(FGsSharedParam& InMessageParam)
{
	const FGsBMShopHiveGetProductInfo* Param = InMessageParam.GetParam<const FGsBMShopHiveGetProductInfo>();
	if (nullptr == Param)
	{
		return;
	}

	GSLOG(Log, TEXT("[FGsIAPPurchaseBase::OnBMShopHiveGetProductInfo] Param: %s"), *LexToString(*Param));

	if (_hiveCallbackResultMap.Contains(CallbackResultType::Callback_GetProductInfo))
	{
		_hiveCallbackResultMap[CallbackResultType::Callback_GetProductInfo] = false;
		GSLOG(Log, TEXT("[hive::IAPV4::getProductInfo] Callback Flag Reset"));
	}

	if (Param->_isSuccess)
	{
		GSLOG(Log, TEXT("[hive::IAPV4::getProductInfo] - Param->_isSuccess, Param->_ipa4Products.Num() : %d"), Param->_ipa4Products.Num());

		if (0 >= Param->_ipa4Products.Num())
		{
			MakeShared<FGsBMShopHiveError>(_ePlatformType, Param->_code, Param->_errorCode, HiveFailedCallbackType::GetProductInfo_Failed)->Send();
			return;
		}

		_hiveProductList.Append(Param->_ipa4Products);
		MakeShared<FGsBMShopHiveProudctListInfo>()->Send();
	}
	else
	{
		if (Param->_needExit)
		{
			MakeShared<FGsBMShopHiveNeedExit>()->Send();
			return;
		}

		GSLOG(Warning, TEXT("[hive::IAPV4::getProductInfo] InResult.code : %d, InResult.errorCode : %d"), Param->_code, Param->_errorCode);
		MakeShared<FGsBMShopHiveError>(_ePlatformType, Param->_code, Param->_errorCode, HiveFailedCallbackType::GetProductInfo_Failed)->Send();
	}
}

void FGsIAPPurchaseBase::GetSubscriptionProductInfo()
{
	GSLOG(Warning, TEXT("[FGsIAPPurchaseBase::SubscriptionProductInfo]"));

	//typedef std::function<void(ResultAPI const & result,std::vector<IAPV4Product> const & iapV4ProductList,unsigned int balance)> onGetProductInfo;
	IAPV4::getSubscriptionProductInfo([&, this](ResultAPI const& InResult, std::vector<class IAPV4Product> const& IniapV4ProductList, unsigned int Inbalance)
		{
			GSLOG(Log, TEXT("[hive::IAPV4::getSubscriptionProductInfo] recieve"));

			TArray<hive::IAPV4Product> Ipa4Products;
			for (const hive::IAPV4Product& It : IniapV4ProductList)
			{
				Ipa4Products.Emplace(It);
			}

			TSharedRef<FGsBMShopHiveGetSubscriptionProductInfo> BMShopHiveGetSubscriptionProductInfo = MakeShared<FGsBMShopHiveGetSubscriptionProductInfo>(InResult.isSuccess(), InResult.code, InResult.errorCode, Ipa4Products);
			GSLOG(Log, TEXT("%s"), *LexToString(*BMShopHiveGetSubscriptionProductInfo));
			FGsBMShopHiveGetSubscriptionProductInfo::StaticSendAsync(BMShopHiveGetSubscriptionProductInfo); // BMShopHiveGetSubscriptionProductInfo->SendAsync();
		});
}

void FGsIAPPurchaseBase::OnBMShopHiveGetSubscriptionProductInfo(FGsSharedParam& InMessageParam)
{
	const FGsBMShopHiveGetSubscriptionProductInfo* Param = InMessageParam.GetParam<const FGsBMShopHiveGetSubscriptionProductInfo>();
	if (nullptr == Param)
	{
		return;
	}

	GSLOG(Log, TEXT("[FGsIAPPurchaseBase::OnBMShopHiveGetSubscriptionProductInfo] Param: %s"), *LexToString(*Param));

	if (Param->_isSuccess)
	{
		_hiveProductList.Append(Param->_ipa4Products);
		FGsBMShopHiveProudctListInfo::StaticSendAsync(MakeShared<FGsBMShopHiveProudctListInfo>()); // MakeShared<FGsBMShopHiveProudctListInfo>()->SendAsync();
	}
	else
	{
		GSLOG(Warning, TEXT("[hive::IAPV4::SubscriptionProductInfo] InResult.code : %d, InResult.errorCode : %d"), Param->_code, Param->_errorCode);
		MakeShared<FGsBMShopHiveError>(_ePlatformType, Param->_code, Param->_errorCode, HiveFailedCallbackType::GetProductInfo_Failed)->Send();
	}
}

void FGsIAPPurchaseBase::CheckRestore(bool InIsFromUserCheck)
{
	GSLOG(Warning, TEXT("[FGsIAPPurchaseBase::CheckRestore]"));

	_isUserCallRestoreFlag = InIsFromUserCheck;

	FGsUIHelper::ShowBlockUI();

	IAPV4::restore([&, this](ResultAPI const& InResult, std::vector<std::reference_wrapper<IAPV4Receipt>> const& iapv4ReceiptList)
		{
			GSLOG(Log, TEXT("[hive::IAPV4::restore]"));

			TArray<TPair<FString, std::string>> MarketIdAndBypassInfos;
			for (const hive::IAPV4Receipt& Iapv4Receipt : iapv4ReceiptList)
			{
				MarketIdAndBypassInfos.Emplace(MakeTuple(FString(UTF8_TO_TCHAR(Iapv4Receipt.product.marketPid.c_str())), Iapv4Receipt.bypassInfo));
				//GSLOG(Log, TEXT("[hive::IAPV4::restore] info.bypassInfo.length() : %d"), info.bypassInfo.length());
			}

			TSharedRef<FGsBMShopHiveRestore> BMShopHiveRestore = MakeShared<FGsBMShopHiveRestore>(InResult.isSuccess(), InResult.code, InResult.needExit(), InResult.errorCode, MarketIdAndBypassInfos);
			GSLOG(Log, TEXT("%s"), *LexToString(*BMShopHiveRestore));
			FGsBMShopHiveRestore::StaticSendAsync(BMShopHiveRestore); // BMShopHiveRestore->SendAsync();
		});
}

void FGsIAPPurchaseBase::OnBMShopHiveRestore(FGsSharedParam& InMessageParam)
{
	const FGsBMShopHiveRestore* Param = InMessageParam.GetParam<const FGsBMShopHiveRestore>();
	if (nullptr == Param)
	{
		return;
	}

	GSLOG(Log, TEXT("[FGsIAPPurchaseBase::OnBMShopHiveRestore] Param: %s"), *LexToString(*Param));

	if (hive::ResultAPI::Code::IAPV4NothingToRestore == Param->_code)
	{
		GSLOG(Log, TEXT("[hive::IAPV4::restore] hive::ResultAPI::Code::IAPV4NothingToRestore == Param->_code"));
		MakeShared<FGsBMShopNothingToRestore>()->Send();
		return;
	}

	if (Param->_needExit)
	{
		GSLOG(Warning, TEXT("[hive::IAPV4::restore] needExit() !!!! "));
		MakeShared<FGsBMShopHiveNeedExit>()->Send();
		return;
	}

	if (Param->_isSuccess)
	{
		if (0 >= Param->_marketIdAndBypassInfos.Num())
		{
			GSLOG(Error, TEXT("[hive::IAPV4::restore] 0 >= Param->_marketIdAndBypassInfos.Num() !!! "));
			MakeShared<FGsBMShopHiveBlockUiClose>()->Send();
			return;
		}

		for (const TPair<FString, std::string>& MarketIdAndBypassInfo : Param->_marketIdAndBypassInfos)
		{
			FGsNetSendServiceBMShop::SendRestoreItem(MarketIdAndBypassInfo.Key, MarketIdAndBypassInfo.Value);
		}
	}
	else
	{
		GSLOG(Warning, TEXT("[hive::IAPV4::CheckRestore] InResult.code : %d, InResult.errorCode : %d"), Param->_code, Param->_errorCode);
		MakeShared<FGsBMShopHiveError>(_ePlatformType, Param->_code, Param->_errorCode, HiveFailedCallbackType::Restore_Failed)->Send();
	}
}

void FGsIAPPurchaseBase::CheckRestoreSubscription()
{
	GSLOG(Warning, TEXT("[FGsIAPPurchaseBase::CheckRestoreSubscription]"));

	IAPV4::restoreSubscription([&, this](ResultAPI const& InResult, std::vector<std::reference_wrapper<IAPV4Receipt>> const& iapv4ReceiptList)
		{
			GSLOG(Log, TEXT("[hive::IAPV4::restoreSubscription]"));

			TArray<TPair<FString, std::string>> MarketIdAndBypassInfos;
			for (const hive::IAPV4Receipt& Iapv4Receipt : iapv4ReceiptList)
			{
				MarketIdAndBypassInfos.Emplace(MakeTuple(FString(UTF8_TO_TCHAR(Iapv4Receipt.product.marketPid.c_str())), Iapv4Receipt.bypassInfo));
			}

			TSharedRef<FGsBMShopHiveRestoreSubscription> BMShopHiveRestoreSubscription = MakeShared<FGsBMShopHiveRestoreSubscription>(InResult.isSuccess(), InResult.code, InResult.needExit(), InResult.errorCode, MarketIdAndBypassInfos);
			GSLOG(Log, TEXT("%s"), *LexToString(*BMShopHiveRestoreSubscription));
			FGsBMShopHiveRestoreSubscription::StaticSendAsync(BMShopHiveRestoreSubscription); // BMShopHiveRestoreSubscription->SendAsync();
		});
}

void FGsIAPPurchaseBase::OnBMShopHiveRestoreSubscription(FGsSharedParam& InMessageParam)
{
	const FGsBMShopHiveRestoreSubscription* Param = InMessageParam.GetParam<const FGsBMShopHiveRestoreSubscription>();
	if (nullptr == Param)
	{
		return;
	}

	GSLOG(Log, TEXT("[FGsIAPPurchaseBase::OnBMShopHiveRestoreSubscription] Param: %s"), *LexToString(*Param));

	if (Param->_isSuccess)
	{
		// nothing to do.
	}
	else
	{
		if (hive::ResultAPI::Code::IAPV4NothingToRestore == Param->_code)
		{
			return;
		}

		GSLOG(Warning, TEXT("[hive::IAPV4::CheckRestoreSubscription] code : %d, errorCode : %d"), Param->_code, Param->_errorCode);
		MakeShared<FGsBMShopHiveError>(_ePlatformType, Param->_code, Param->_errorCode, HiveFailedCallbackType::Restore_Failed)->Send();
	}
}

void FGsIAPPurchaseBase::TryPurchaseSubscription()
{
	std::string pid;
	std::string oldPid;
	std::string additionalInfo;

	IAPV4::purchaseSubscriptionUpdate(pid, oldPid, additionalInfo, [&, pid, oldPid, additionalInfo](ResultAPI const& result, IAPV4Receipt const& iapV4Receipt)
		{

		});

}

//if (IAP_PLATFORM_TYPE::ANDROID_PLATFORM == _ePlatformType || IAP_PLATFORM_TYPE::IOS_PLATFORM == _ePlatformType)
void FGsIAPPurchaseBase::TryPurchase(const TArray<BMProductIdAmountPair>& InAmountPairList, const bool InBulkSale, const uint64 InCouponItemDbid)
{
	FGsNetSendServiceBMShop::SendBMShopProductValidCheck(InAmountPairList, InBulkSale, InCouponItemDbid);
}

// 일괄구매 기능에서는 현금결제 아이템을 구매할 수 없다
void FGsIAPPurchaseBase::TryHivePurchase(TArray<BMProductIdAmountPair> InAmountPairList, const uint64 InIdentifier, const bool InIsBulkSale, uint64 InCouponItemDbid)
{
	if (0 >= InAmountPairList.Num())
		return;

	GSLOG(Error, TEXT("[FGsIAPPurchaseBase::TryPurchase]"));

	if (InIsBulkSale)
	{
		FGsNetSendServiceBMShop::SendBMShopPurchaseItem(InAmountPairList, InIdentifier, InIsBulkSale, InCouponItemDbid);
	}
	else
	{
		FGsBMShopManager* bmShopMgr = GBMShop();
		BMProductIdAmountPair pairData = InAmountPairList[0];
		GSLOG(Log, TEXT("InAmountPairList[0]: mBMProductId: %s, mAmount: %s"), *LexToString(pairData.mBMProductId), *LexToString(pairData.mAmount));
		FGsBMShopProductData* data = bmShopMgr->FindProduct(pairData.mBMProductId);
		if (nullptr == data)
		{
			GSLOG(Warning, TEXT("nullptr == data"));
			return;
		}

		uint64 identifier = InIdentifier;

		// 에디터에 한하여 현금결제 상품 구매 테스트 위한 치트 코드 추가
#if WITH_EDITOR
		if (bmShopMgr->_usecheatFlag && data->IsCashProductItem())
		{
			if (nullptr != data->GetTableData())
			{
				std::string str = TCHAR_TO_UTF8(*FDateTime::Now().ToString());
				FGsNetSendServiceBMShop::SendBMShopPurchaseItemTest(data->GetProductID(), str, identifier);
			}
			return;
		}
#endif

		if (true == data->IsHiveProduct()) // 현금성 아이템
		{
			const FString CurrentPid = data->GetConvertHiveMarketPID();
			if (CurrentPid.IsEmpty())
			{
				GSLOG(Error, TEXT("[FGsIAPPurchaseBase::TryPurchase] data->GetHiveMarketPID is empty - GetProductID : %d"), data->GetProductID());
				return;
			}

			/*
			// 윈도우 플랫폼에서는 현금결제 불가능
			// 씽크페이 사용해야함
			if (DevicePlatformType::WINDOWS == _ePlatformType)
			{
				// 티커 출력필요
				// 윈도우는 인앱상품 결제 불가능
				return;
			}

			// 혹시 몰라서 2차로 예외처리 추가
#if PLATFORM_WINDOWS
			return;
#endif
			*/

			uint64 userDbid = 0;
			const FGsNetUserData* userData = GGameData()->GetUserData();
			if (nullptr != userData)
			{
				uint64 userId = userData->mUserDBId;
				userDbid = userId;
			}

			if (0 >= userDbid)
			{
				FText findText;
				FText::FindText(TEXT("BMShopText"), TEXT("Notice_Error_Restore_NoUserId"), findText);
				FGsUIHelper::PopupSystemMsg(findText);
				return;
			}
			
			GSLOG(Error, TEXT("[FGsIAPPurchaseBase::TryPurchase] userDbid : %llu"), userDbid);

			std::string convertPayload = std::to_string(userDbid);

			//GSLOG(Error, TEXT("[FGsIAPPurchaseBase::TryPurchase] send to hive Purchase - payloadString : %s"), *FString(UTF8_TO_TCHAR(convertPayload.c_str())));
			//GSLOG(Error, TEXT("[FGsIAPPurchaseBase::TryPurchase] send to hive Purchase - identifier : %llu"), identifier);

			if (EGsScreenOnPaying::Block == _screenOnPaying)
			{
				FGsUIHelper::ShowBlockUI();
			}
			else if (EGsScreenOnPaying::Popup == _screenOnPaying)
			{
				if (!FGsUIHelper::TrayPGPayBlock())
				{
					return;
				}
			}

			bmShopMgr->OnIAPV4SheetsLog(TEXT("TryHivePurchase"));

			if (_sendPgPurchaseWnd)
			{
				// https://jira.com2us.com/wiki/x/4edCFw
				// 1. 웹 결제창을 열면서 서버로 창 오픈시(CG_OPEN_PG_PURCHASE_WND)
				FGsOPEN_PG_PURCHASE_WND().Send();
			}

			GSLOG(Log, TEXT("1 _ePlatformType: %d"), _ePlatformType);

			// const DevicePlatformType PlatformTypeCpt = _ePlatformType;

			//GSLOG(Log, TEXT("2 PlatformTypeCpt: %d"), PlatformTypeCpt);

			TFunction<void(hive::ResultAPI const& InResultApi, hive::IAPV4Receipt const& InIapV4Receipt)> OnPurchaseDelegate = [CurrentPid, identifier](hive::ResultAPI const& InResultApi, hive::IAPV4Receipt const& InIapV4Receipt)
			{
				//FGsBMShopHivePurchase2* BMShopHivePurchase = new FGsBMShopHivePurchase2();
				//if (nullptr == BMShopHivePurchase)
				//{
				//	GSLOG(Error, TEXT("nullptr == BMShopHivePurchase"));
				//	return;
				//}

				GSLOG(Log, TEXT("IAPV4Purchase() recieved. errorMessage: %s"), UTF8_TO_TCHAR(InResultApi.errorMessage.c_str()));

				// FGsBMShopHivePurchase* BMShopHivePurchase = new FGsBMShopHivePurchase(InResultApi.isSuccess(), InResultApi.code, InResultApi.needExit(), InResultApi.errorCode, CurrentPid, identifier, InIapV4Receipt.bypassInfo);
				// BMShopHivePurchase->Set(InResultApi.isSuccess(), InResultApi.code, InResultApi.needExit(), InResultApi.errorCode, CurrentPid, identifier, InIapV4Receipt.bypassInfo);

				// GSLOG(Log, TEXT("BMShopHivePurchase: %s"), *LexToString(*BMShopHivePurchase));

				// TSharedRef<FGsBMShopHivePurchase2> BMShopHivePurchase = MakeShared<FGsBMShopHivePurchase2>();

				//FGsBMShopHivePurchase::StaticSendAsync(MoveTemp(BMShopHivePurchase)); // BMShopHivePurchase->SendAsync();
				//BMShopHivePurchase->SendAsync();

				//FGsBMShopHivePurchase2::StaticSendAsync(MoveTemp(BMShopHivePurchase));

				GSLOG(Log, TEXT("FGsIAPPurchaseBase::OnBMShopHivePurchase()"));

				//if (!InMessageParam.IsValid())
				//{
				//	GSLOG(Log, TEXT("FGsIAPPurchaseBase::OnBMShopHivePurchase(), !InMessageParam.IsValid()"));
				//	return;
				//}

				//const FGsBMShopHivePurchase2* Param = InMessageParam->GetParam<const FGsBMShopHivePurchase2>();
				//if (nullptr == Param)
				//{
				//	return;
				//}

				// GSLOG(Log, TEXT("[FGsIAPPurchaseBase::OnBMShopHivePurchase] Param: %s"), *LexToString(*Param));
				//GMessage()->GetBMShop().SendMessage(MessageContentStoreBM::HIVE_BLOCK_UI_CLOSE, nullptr);
				//GBMShop()->SetIsStartInAppPurchase(false);

#if PLATFORM_WINDOWS
				const DevicePlatformType CurrentDevicePlatformType = DevicePlatformType::WINDOWS;

				// GSLOG(Log, TEXT("3 PlatformTypeCpt: %d"), PlatformTypeCpt);
				//if (DevicePlatformType::WINDOWS == PlatformTypeCpt)
				//{
					if (hive::ResultAPI::Code::IAPV4HiveStoreSuccess == InResultApi.code && hive::ResultAPI::ErrorCode::IAPSUCCESS == InResultApi.errorCode)
					{
						GSLOG(Log, TEXT("hive::ResultAPI::Code::IAPV4HiveStoreSuccess == InResultApi.code && hive::ResultAPI::ErrorCode::IAPSUCCESS == InResultApi.errorCode"));
					}
					// https://developers.withhive.com/dev4/billing/pg-billing/
					// hive.IAPV4.purchase(marketPid, null, onIAPV4PurchaseCB);를 호출하면 영수증 정보 없이 onIAPV4PurchaseCB를 통해 ErrorCode:IAPSUCCESS=90와 Code:IAPV4HiveStoreSuccess=-6110000의 응답을 즉시 반환하며,
					// ErrorCode(또는 Code)를 판별하여 게임 내에서 구매 중임을 알리는 팝업을 노출하면 됩니다.

					// windows PG purchase packet
					//FGsNetSendServiceBMShop::SendPG_PurchaseItem();
					GSLOG(Log, TEXT("skip FGsNetSendServiceBMShop::SendPG_PurchaseItem()"));
				//}

#elif PLATFORM_ANDROID || PLATFORM_IOS

				const DevicePlatformType CurrentDevicePlatformType = PLATFORM_ANDROID ? DevicePlatformType::GOOLE_ANDROID : DevicePlatformType::APPLE_IOS;
				if (InResultApi.isSuccess())
				{
					/*
					* @param marketPid            App Store, Google Store 에 등록된 하나의 상품을 지칭하는 새로운 구독 상품의 고유 코드
					* @param oldMarketPid        App Store, Google Store 에 등록된 하나의 상품을 지칭하는 기존 구독 상품의 고유 코드 (Android Only)
					* @param iapPayload    구매 요청에 대한 보충 정보를 포함한 문자열로, 개발자가 지정합니다. (Android Only)
					* @param listener            API 결과 통지
					*/

					/*
					* 지우지 마세여
					if (true == restoreTest)
					{
						verifyf(nullptr, TEXT("restore Test !!!!"));
						return;
					}
					*/

					//GMessage()->GetBMShop().SendMessage(MessageContentStoreBM::SEND_ANLYTICS_EVENT, nullptr);

					FGsNetSendServiceBMShop::SendBMShopPurchaseItem(CurrentPid, InIapV4Receipt.bypassInfo, identifier);
				}
				else
				{
					GSLOG(Log, TEXT("if (InResultApi.needExit())"));

					if (InResultApi.needExit())
					{
						MakeShared<FGsBMShopHiveNeedExit>()->SendAsync();
						return;
					}

					GSLOG(Log, TEXT("if (hive::ResultAPI::Code::IAPV4CancelPayment == InResultApi.code || hive::ResultAPI::Code::IAPV4HiveStoreSuccess == InResultApi.code)"));

					if (hive::ResultAPI::Code::IAPV4CancelPayment == InResultApi.code || hive::ResultAPI::Code::IAPV4HiveStoreSuccess == InResultApi.code)
					{
						MakeShared<FGsBMShopHiveBlockUiClose>()->SendAsync();
						return;
					}

					GSLOG(Log, TEXT("if (hive::ResultAPI::Code::IAPV4NeedRestore == InResultApi.code)"));

					if (hive::ResultAPI::Code::IAPV4NeedRestore == InResultApi.code)
					{
						MakeShared<FGsBMShopPurchaseRestoreCheck>()->SendAsync();
						return;
					}

					GSLOG(Log, TEXT("MakeShared<FGsBMShopHiveError>(CurrentDevicePlatformType, InResultApi.code, InResultApi.errorCode, HiveFailedCallbackType::Purchase_Failed)->SendAsync();"));

					MakeShared<FGsBMShopHiveError>(CurrentDevicePlatformType, InResultApi.code, InResultApi.errorCode, HiveFailedCallbackType::Purchase_Failed)->SendAsync();
				}
#endif

				GSLOG(Log, TEXT("BMShopHivePurchase->SendAsync()"));
			};

#if PLATFORM_ANDROID || PLATFORM_IOS
			GSLOG(Log, TEXT("hive::IAPV4::purchase(\"%s\", \"%s\")"), *CurrentPid, UTF8_TO_TCHAR(convertPayload.c_str()));
			hive::IAPV4::purchase(TCHAR_TO_UTF8(*CurrentPid), convertPayload, OnPurchaseDelegate);
#elif PLATFORM_WINDOWS
			GSLOG(Log, TEXT("IAPV4Purchase(\"%s\", \"%s\")"), *CurrentPid, UTF8_TO_TCHAR(convertPayload.c_str()));
			IAPV4Purchase(TCHAR_TO_UTF8(*CurrentPid), convertPayload.c_str(), OnPurchaseDelegate);
#endif

		}
		else
		{
			FGsNetSendServiceBMShop::SendBMShopPurchaseItem(InAmountPairList, identifier, InIsBulkSale, InCouponItemDbid);
		}
	}
}

//void FGsIAPPurchaseBase::OnBMShopHivePurchase(FGsSharedParam2::ThreadSafeType& InMessageParam)
//{
//	GSLOG(Log, TEXT("FGsIAPPurchaseBase::OnBMShopHivePurchase()"));
//
//	if (!InMessageParam.IsValid())
//	{
//		GSLOG(Log, TEXT("FGsIAPPurchaseBase::OnBMShopHivePurchase(), !InMessageParam.IsValid()"));
//		return;
//	}
//
//	const FGsBMShopHivePurchase2* Param = InMessageParam->GetParam<const FGsBMShopHivePurchase2>();
//	if (nullptr == Param)
//	{
//		return;
//	}
//
//	GSLOG(Log, TEXT("[FGsIAPPurchaseBase::OnBMShopHivePurchase] Param: %s"), *LexToString(*Param));
//	//GMessage()->GetBMShop().SendMessage(MessageContentStoreBM::HIVE_BLOCK_UI_CLOSE, nullptr);
//	//GBMShop()->SetIsStartInAppPurchase(false);
//
//	if (DevicePlatformType::WINDOWS == _ePlatformType)
//	{
//		if (hive::ResultAPI::Code::IAPV4HiveStoreSuccess == Param->_code && hive::ResultAPI::ErrorCode::IAPSUCCESS == Param->_errorCode)
//		{
//			GSLOG(Log, TEXT("hive::ResultAPI::Code::IAPV4HiveStoreSuccess == Param->_code && hive::ResultAPI::ErrorCode::IAPSUCCESS == Param->_errorCode"));
//		}
//		// https://developers.withhive.com/dev4/billing/pg-billing/
//		// hive.IAPV4.purchase(marketPid, null, onIAPV4PurchaseCB);를 호출하면 영수증 정보 없이 onIAPV4PurchaseCB를 통해 ErrorCode:IAPSUCCESS=90와 Code:IAPV4HiveStoreSuccess=-6110000의 응답을 즉시 반환하며,
//		// ErrorCode(또는 Code)를 판별하여 게임 내에서 구매 중임을 알리는 팝업을 노출하면 됩니다.
//
//		// windows PG purchase packet
//		//FGsNetSendServiceBMShop::SendPG_PurchaseItem();
//		GSLOG(Log, TEXT("skip FGsNetSendServiceBMShop::SendPG_PurchaseItem()"));
//	}
//	else if (Param->_isSuccess)
//	{
//		/*
//		* @param marketPid            App Store, Google Store 에 등록된 하나의 상품을 지칭하는 새로운 구독 상품의 고유 코드
//		* @param oldMarketPid        App Store, Google Store 에 등록된 하나의 상품을 지칭하는 기존 구독 상품의 고유 코드 (Android Only)
//		* @param iapPayload    구매 요청에 대한 보충 정보를 포함한 문자열로, 개발자가 지정합니다. (Android Only)
//		* @param listener            API 결과 통지
//		*/
//
//		/*
//		* 지우지 마세여
//		if (true == restoreTest)
//		{
//			verifyf(nullptr, TEXT("restore Test !!!!"));
//			return;
//		}
//		*/
//
//		//GMessage()->GetBMShop().SendMessage(MessageContentStoreBM::SEND_ANLYTICS_EVENT, nullptr);
//
//		FGsNetSendServiceBMShop::SendBMShopPurchaseItem(Param->_currentPid, Param->_bypassInfo, Param->_identifier);
//	}
//	else
//	{
//		if (Param->_needExit)
//		{
//			MakeShared<FGsBMShopHiveNeedExit>()->Send();
//			return;
//		}
//
//		if (hive::ResultAPI::Code::IAPV4CancelPayment == Param->_code || hive::ResultAPI::Code::IAPV4HiveStoreSuccess == Param->_code)
//		{
//			MakeShared<FGsBMShopHiveBlockUiClose>()->Send();
//			return;
//		}
//
//		if (hive::ResultAPI::Code::IAPV4NeedRestore == Param->_code)
//		{
//			MakeShared<FGsBMShopPurchaseRestoreCheck>()->Send();
//			return;
//		}
//
//		MakeShared<FGsBMShopHiveError>(_ePlatformType, Param->_code, Param->_errorCode, HiveFailedCallbackType::Purchase_Failed)->Send();
//	}
//}

void FGsIAPPurchaseBase::OnTransactionFinishMulti(TArray<std::string> InMarketPIDList)
{
	GSLOG(Error, TEXT("[FGsIAPPurchaseBase::OnTransactionFinish]"));

	if (0 >= InMarketPIDList.Num())
		return;

	std::vector< std::string > pidList;
	for (auto& iter : InMarketPIDList)
	{
		pidList.push_back(iter);
	}

	const DevicePlatformType CurrentPlatformType = _ePlatformType;
	hive::IAPV4::transactionMultiFinish(pidList, [CurrentPlatformType](std::vector<hive::ResultAPI> const& resultList, std::vector<std::string> const& marketPidList)
		{
			GSLOG(Log, TEXT("[FGsIAPPurchaseBase::OnTransactionFinish] in transactionMultiFinish"));

			hive::ResultAPI::Code CurrentCode = hive::ResultAPI::Code::Success;
			int32 CurrentErrorCode = hive::ResultAPI::Success;
			bool FoundInvalidResult = false;

			for (const hive::ResultAPI& result : resultList)
			{
				if (!result.isSuccess())
				{
					CurrentCode = result.code;
					CurrentErrorCode = result.errorCode;
					FoundInvalidResult = true;
					break;
				}
			}

			if (FoundInvalidResult)
			{
				//MakeShared<FGsBMShopHiveErrors>(CurrentPlatformType, CurrentCode, CurrentErrorCode, HiveFailedCallbackType::TransactionFinishMulti_Failed)->SendAsync();
				FGsBMShopHiveErrors::StaticSendAsync(MakeShared<FGsBMShopHiveErrors>(CurrentPlatformType, CurrentCode, CurrentErrorCode, HiveFailedCallbackType::TransactionFinishMulti_Failed));
			}
		});
}

void FGsIAPPurchaseBase::OnTransactionFinishSingle(std::string InPid)
{
	GSLOG(Error, TEXT("[FGsIAPPurchaseBase::OnTransactionFinishSingle]"));

	if (InPid.empty())
		return;

	const DevicePlatformType CurrentPlatformType = _ePlatformType;
	hive::IAPV4::transactionFinish(InPid, [CurrentPlatformType](hive::ResultAPI const& result, std::string marketPid)
		{
			GSLOG(Log, TEXT("[hive::IAPV4::transactionFinish] recieved"));

			if (result.isSuccess())
			{
				GSLOG(Log, TEXT("[FGsIAPPurchaseBase::OnTransactionFinishSingle] result.isSuccess()"));
			}
			else
			{
				if (result.needExit())
				{
					FGsBMShopHiveNeedExit::StaticSendAsync(MakeShared<FGsBMShopHiveNeedExit>()); // MakeShared<FGsBMShopHiveNeedExit>()->SendAsync();
					return;
				}

				// MakeShared<FGsBMShopHiveError>(CurrentPlatformType, result.code, result.errorCode, HiveFailedCallbackType::TransactionFinishSingle_Failed)->SendAsync();
				FGsBMShopHiveError::StaticSendAsync(MakeShared<FGsBMShopHiveError>(CurrentPlatformType, result.code, result.errorCode, HiveFailedCallbackType::TransactionFinishSingle_Failed));
			}
		});
}

void FGsIAPPurchaseBase::ShowPopupShopCustomContents(const int32 InHiveBannerID)
{
	// 윈도우 플랫폼은 하이브 crossplay 에서 interwork 기능을 지원하지 않으므로 처리 x
#if PLATFORM_IOS || PLATFORM_ANDROID 
	if (0 >= InHiveBannerID)
		return;

	if (!GHive()->IsSignIn())
		return;

	FString temp = FString::FromInt(InHiveBannerID);
	std::string str = TCHAR_TO_UTF8(*temp);
	if (str.empty())
		return;

	GSLOG(Warning, TEXT("[FGsIAPPurchaseBase::ShowPopupShopCustomContents] - InHiveBannerID : %d"), InHiveBannerID);

	hive::Promotion::showCustomContents(hive::PromotionCustomType::SPOT, str, [](hive::ResultAPI const& result, hive::PromotionEventType promotionEventType)
		{
			if (result.isSuccess())
			{
				// API 호출 성공
				switch (promotionEventType)
				{
					// 프로모션 뷰가 뜸
				case hive::PromotionEventType::OPEN:
					GSLOG(Log, TEXT("hive::PromotionEventType::OPEN"));
					break;

					//프로모션 뷰가 닫힘
				case hive::PromotionEventType::CLOSE:
					GSLOG(Log, TEXT("hive::PromotionEventType::CLOSE"));
					break;

					// 게임 종료 팝업(더 많은 게임 보기)에서 종료하기가 선택됨
				case hive::PromotionEventType::EXIT:
					GSLOG(Log, TEXT("hive::PromotionEventType::EXIT"));
					break;
				}
			}
			else
			{
				if (result.needExit())
				{
					FGsBMShopHiveNeedExit::StaticSendAsync(MakeShared<FGsBMShopHiveNeedExit>()); // MakeShared<FGsBMShopHiveNeedExit>()->SendAsync();
					return;
				}
			}
		});
#endif
}

void FGsIAPPurchaseBase::GetMarketTypeList(OUT TArray<IAPV4Type>& outTypeList)
{
	for (IAPV4Type type : _purchaseMarketTypeList)
	{
		outTypeList.Emplace(type);
	}
}

void FGsIAPPurchaseBase::GetHiveProductInfoList(OUT TArray<IAPV4Product>& outProductInfoList)
{
	for (IAPV4Product productInfo : _hiveProductList)
	{
		outProductInfoList.Emplace(productInfo);
	}
}

// 상품정보를 받는 마켓과 상품 리스트가 정상적으로 담겨있어야 함
bool FGsIAPPurchaseBase::IsValidState() const
{
	return 	(0 < _purchaseMarketTypeList.Num() && 0 < _hiveProductList.Num());
}

void FGsIAPPurchaseBase::SetCallbackResultFlag(const CallbackResultType InType, const bool InState)
{
	if (_hiveCallbackResultMap.Contains(InType))
	{
		_hiveCallbackResultMap[InType] = InState;
	}
}

bool FGsIAPPurchaseBase::GetHiveCallbackResultFalg(const CallbackResultType InType) const
{
	if (_hiveCallbackResultMap.Contains(InType))
	{
		return _hiveCallbackResultMap[InType];
	}

	return false;
}

/*
void FGsIAPPurchaseBase::RestoreProc(TArray<FString> InMarketPIDList , TArray<std::string> InBypassInfoList)
{
	GSLOG(Warning, TEXT("[FGsIAPPurchaseBase::RestoreProc]"));


	int32 pidListCount = InMarketPIDList.Num();
	int32 bypassInfoListCount = InBypassInfoList.Num();
	if (pidListCount != bypassInfoListCount)
	{
		GSLOG(Error, TEXT("[FGsIAPPurchaseBase::RestoreProc] pidListCount != bypassInfoListCount - Error !!!!"));
		GMessage()->GetBMShop().SendMessageAsync(MessageContentStoreBM::HIVE_BLOCK_UI_CLOSE, nullptr);
		return;
	}

	for (int32 i = 0; i < InMarketPIDList.Num(); ++i)
	{
		if (false == InMarketPIDList.IsValidIndex(i) || false == InBypassInfoList.IsValidIndex(i))
		{
			GMessage()->GetBMShop().SendMessageAsync(MessageContentStoreBM::HIVE_BLOCK_UI_CLOSE, nullptr);
			break;
		}

		GSLOG(Error, TEXT("[FGsIAPPurchaseBase::RestoreProc] FGsNetSendServiceBMShop::SendRestoreItem"));
		FGsNetSendServiceBMShop::SendRestoreItem(InMarketPIDList[i], InBypassInfoList[i]);
	}
}
*/

#if WITH_EDITOR
void FGsIAPPurchaseBase::EditorClearHiveProductInfos()
{
	_hiveProductList.Empty();
}
#endif
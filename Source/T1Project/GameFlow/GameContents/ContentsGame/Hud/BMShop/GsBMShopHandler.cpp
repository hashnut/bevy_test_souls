// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopHandler.h"

#include "GsBMShopProductData.h"
#include "GsIAPPurchaseAndroid.h"
#include "GsIAPPurchaseIOS.h"
#include "GsIAPPurchaseWindow.h"
#include "GsIAPPurchaseEditor.h"

#include "Net/GsNetSendServiceBMShop.h"

#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Management/ScopeGlobal/GsPopupSystemManager.h"

#include "UI/UIContent/Window/BMShop/GsUIPopupBMShopProductBuy.h"
#include "UI/UIContent/Window/BMShop/GsUIPopupBMShopGroupDetail.h"
#include "UI/UIContent/Popup/BMShop/GsUIPopupBulkSale.h"
#include "UI/UIContent/Popup/BMShop/GsUIPopupCouponList.h"
#include "UI/UIContent/Popup/BMShop/GsUIPopupConfirmDiscount.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupCommonItemAction.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/item/GsSchemaItemCommon.h"
#include "DataSchema/item/GsSchemaItemConsumeDetail.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "Kismet/GameplayStatics.h"
#include "Currency/GsCurrencyHelper.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "Reward/GsRewardHelper.h"

#include "T1Project.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"

void FGsBMShopHandler::Init()
{
	InitMsg();

	if (!_iapPurchaseBase.IsValid())
	{
		GSLOG(Log, TEXT("FGsBMShopHandler::Init()"));

#if WITH_EDITOR
		GSLOG(Log, TEXT("FGsBMShopHandler::Init() DevicePlatformType::EDITOR"));
		_iapPurchaseBase = MakeUnique<FGsIAPPurchaseEditor>();
		_iapPurchaseBase->Initialize(DevicePlatformType::EDITOR);
#elif PLATFORM_ANDROID 
		GSLOG(Log, TEXT("FGsBMShopHandler::Init() DevicePlatformType::GOOLE_ANDROID"));
		_iapPurchaseBase = MakeUnique<FGsIAPPurchaseAndroid>();
		_iapPurchaseBase->Initialize(DevicePlatformType::GOOLE_ANDROID);
#elif PLATFORM_IOS
		GSLOG(Log, TEXT("FGsBMShopHandler::Init() DevicePlatformType::APPLE_IOS"));
		_iapPurchaseBase = MakeUnique<FGsIAPPurchaseIOS>();
		_iapPurchaseBase->Initialize(DevicePlatformType::APPLE_IOS);
#elif PLATFORM_WINDOWS
		GSLOG(Log, TEXT("FGsBMShopHandler::Init() DevicePlatformType::WINDOWS"));
		_iapPurchaseBase = MakeUnique<FGsIAPPurchaseWindow>();
		_iapPurchaseBase->Initialize(DevicePlatformType::WINDOWS);
#endif
	}

	if (!_iapPurchaseBase.IsValid())
	{
		// 어떤 플랫폼인지 찾지 못함
		// 예외처리 필요

	}

	/*
		FString devicePlatformName = UGameplayStatics::GetPlatformName();
		GSLOG(Warning, TEXT("[FGsBMShopHandler::Init] - DevicePlatformName : %s"), *devicePlatformName);
		if ("android" == devicePlatformName.ToLower())
		{
			_iapPurchaseBase = new FGsIAPPurchaseAndroid();
			_iapPurchaseBase->Initialize(FGsIAPPurchaseBase::IAP_PLATFORM_TYPE::ANDROID_PLATFORM);

		}
		else if ("ios" == devicePlatformName.ToLower())
		{
			_iapPurchaseBase = new FGsIAPPurchaseIOS();
			_iapPurchaseBase->Initialize(FGsIAPPurchaseBase::IAP_PLATFORM_TYPE::IOS_PLATFORM);
		}
		else
		{
			// window
			_iapPurchaseBase = new FGsIAPPurchaseWindow();
			_iapPurchaseBase->Initialize(FGsIAPPurchaseBase::IAP_PLATFORM_TYPE::WINDOW_PLATFORM);
		}
		*/

	//GSLOG(Warning, TEXT("[FGsBMShopHandler::Init] - GDivergence()->GetPlatform() : %s"), *GDivergence()->GetPlatform());
}

void FGsBMShopHandler::Close()
{
	RemoveMsg();

	_iapPurchaseBase.Reset(nullptr);
}

void FGsBMShopHandler::RemoveMsg()
{
	FGsMessageHolder* msg = GMessage();
	for (TPair<MessageContentStoreBM, FDelegateHandle>& elemBmShop : _bmShopMsgHandlerList)
	{
		msg->GetBMShop().Remove(elemBmShop);
	}
	_bmShopMsgHandlerList.Empty();

	for (TPair<MessageItem, FDelegateHandle>& elem : _listItemDelegate)
	{
		msg->GetItem().Remove(elem);
	}
	_listItemDelegate.Empty();

	for (TPair<MessageContentItem, FDelegateHandle>& elem : _listItemDetailDelegate)
	{
		msg->GetItemContents().Remove(elem);
	}
	_listItemDetailDelegate.Empty();

	msg->GetUserBasicInfo().Remove(_userInfoMsgHandler);
	msg->GetUI().Remove(_uiMsgHandler);

	for (MsgGameObjHandle& GameObjHandle : _listGameObjHandleDelegate)
	{
		msg->GetGameObject().Remove(GameObjHandle);
	}
	_listGameObjHandleDelegate.Empty();
}

void FGsBMShopHandler::InitMsg()
{
	FGsMessageHolder* msg = GMessage();

	// InGame Callback
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopOpenPopupBuy::MESSAGE_ID, this, &FGsBMShopHandler::OpenBuyPopup));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopOpenTab::MESSAGE_ID, this, &FGsBMShopHandler::OpenBMShopWithTab));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopOpenSpecialTab::MESSAGE_ID, this, &FGsBMShopHandler::OpenBMShopWithSpecialTab));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopOpenBMShopHud::MESSAGE_ID, this, &FGsBMShopHandler::OpenBMShopHUD));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopOpenPopupShop::MESSAGE_ID, this, &FGsBMShopHandler::OpenPopupShop));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopOpenCouponList::MESSAGE_ID, this, &FGsBMShopHandler::OpenCouponListPopup));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopOpenBMShopPromotion::MESSAGE_ID, this, &FGsBMShopHandler::OpenPopupShopPromotion));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopOpenPopupGroupDetail::MESSAGE_ID, this, &FGsBMShopHandler::OpenGroupDetailPopup));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopPurchaseSuccess::MESSAGE_ID, this, &FGsBMShopHandler::SuccessePurchaseItem));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopCheckValidProduct::MESSAGE_ID, this, &FGsBMShopHandler::ValidCheck));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopCheckValidCoupon::MESSAGE_ID, this, &FGsBMShopHandler::CouponValidCheck));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopAcceptStorage::MESSAGE_ID, this, &FGsBMShopHandler::GetStorageItem));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopPurchaseRestoreCheck::MESSAGE_ID, this, &FGsBMShopHandler::RestoreCheck));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopUserCheckRestore::MESSAGE_ID, this, &FGsBMShopHandler::UserCheckRestore));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopCheckValidProductResult::MESSAGE_ID, this, &FGsBMShopHandler::ServerValidCheckResult));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopOpenBulkSale::MESSAGE_ID, this, &FGsBMShopHandler::OpenBulkSalePopup));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopSendTransactionFinishSingle::MESSAGE_ID, this, &FGsBMShopHandler::SendTransactionFinishSingle));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopOpenBMShopValidHive::MESSAGE_ID, this, &FGsBMShopHandler::OpenBMShopValidHiveProduct));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopNothingToRestore::MESSAGE_ID, this, &FGsBMShopHandler::NotingToRestore));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopDisplayNothingToRestore::MESSAGE_ID, this, &FGsBMShopHandler::DisplayNotingToRestore));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopZpayUserAck::MESSAGE_ID, this, &FGsBMShopHandler::AckSearchZpayUser));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopSearchZpayUserNotify::MESSAGE_ID, this, &FGsBMShopHandler::NotifySearchZpayUser));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopCloseBMShopByBlock::MESSAGE_ID, this, &FGsBMShopHandler::CloseShopByBlock));
	
	// Hive Callback
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopHiveMarketConnection::MESSAGE_ID, this, &FGsBMShopHandler::MarketConnection));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopHiveProudctListInfo::MESSAGE_ID, this, &FGsBMShopHandler::HiveProductInfo));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopHiveBlockUiOpen::MESSAGE_ID, this, &FGsBMShopHandler::OnHiveCallbackBlockUI));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopHiveBlockUiClose::MESSAGE_ID, this, &FGsBMShopHandler::OnHiveCallbackCloseBlockUI));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopHiveError::MESSAGE_ID, this, &FGsBMShopHandler::OnFailedHiveCallback));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopHiveErrors::MESSAGE_ID, this, &FGsBMShopHandler::OnFailedHiveCallbackMulti));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopSendAnalyticsEvent::MESSAGE_ID, this, &FGsBMShopHandler::SendAnlyticsEvent));
	_bmShopMsgHandlerList.Emplace(msg->GetBMShop().AddRaw(FGsBMShopHiveNeedExit::MESSAGE_ID, this, &FGsBMShopHandler::HiveNeedExitCallback));

	_userInfoMsgHandler = msg->GetUserBasicInfo().AddRaw(MessageUserInfo::CURRENCY, this, &FGsBMShopHandler::OnUpdateCurrency);
	_uiMsgHandler = msg->GetUI().AddRaw(MessageUI::LEVEL_UP, this, &FGsBMShopHandler::OnUpdateUserLevel);

	_listGameObjHandleDelegate.Emplace(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_COOLTIME_END, this, &FGsBMShopHandler::FinishedPeriodItem));
	_listGameObjHandleDelegate.Emplace(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this, &FGsBMShopHandler::OnLocalSpawnMeComplete));

	// Item Callback
	_listItemDelegate.Emplace(msg->GetItem().AddRaw(MessageItem::ADDITEMLIST, this, &FGsBMShopHandler::OnAddItemList));
	_listItemDelegate.Emplace(msg->GetItem().AddRaw(MessageItem::UPDATEITEM, this, &FGsBMShopHandler::OnUpdateItem));
	_listItemDelegate.Emplace(msg->GetItem().AddRaw(MessageItem::REMOVEITEM, this, &FGsBMShopHandler::OnRemoveItem));
}

void FGsBMShopHandler::RemoveItemDetailMsg()
{
	FGsMessageHolder* msg = GMessage();
	for (TPair<MessageContentItem, FDelegateHandle>& elem : _listItemDetailDelegate)
	{
		msg->GetItemContents().Remove(elem);
	}
	_listItemDetailDelegate.Empty();
}

void FGsBMShopHandler::AddItemDetailMsg()
{
	if (0 >= _listItemDetailDelegate.Num())
	{
		FGsMessageHolder* msg = GMessage();
		// ItemDetail
		_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_DISPLAY_ITEM_TID,
			this, &FGsBMShopHandler::OpenItemDetailTID));
		_listItemDetailDelegate.Emplace(msg->GetItemContents().AddRaw(MessageContentItem::DETAIL_DISPLAY_ITEM,
			this, &FGsBMShopHandler::OpenItemDetail));
	}
}

void FGsBMShopHandler::SendAnlyticsEvent(FGsSharedParam& InParam)
{
	if (UGsHiveManager* hiveMgr = GHive())
	{
		hiveMgr->AnalyticsSendEvent(TEXT("Purchase"));
	}
}

void FGsBMShopHandler::OpenItemDetail(const struct IGsMessageParam* InMessageParam)
{
	const FGsItemDetailOpenMessageParam* param = InMessageParam->Cast<const FGsItemDetailOpenMessageParam>();
	if (nullptr == param || false == FGsItem::IsValidItem(param->_item))
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (uiManager->IsActiveWidget(TEXT("PopupItemDetail")))
	{
		uiManager->CloseByKeyName(TEXT("PopupItemDetail"), true);
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetail"));
	if (widget.IsValid())
	{
		if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
		{
			popupDetail->SetItemData(param->_item, true, UGsUIPopupItemDetailMain::CenterPosType::CenterPos, false, true, false);
		}
	}
}

void FGsBMShopHandler::OpenItemDetailTID(const struct IGsMessageParam* InMessageParam)
{
	const FGsItemDetailOpenMessageParamTID* param = InMessageParam->Cast<const FGsItemDetailOpenMessageParamTID>();
	if (nullptr == param || 0 >= param->_itemTID)
		return;

	if (UGsUIManager* uiManager = GUI())
	{
		if (uiManager->IsActiveWidget(TEXT("PopupItemDetail")))
		{
			uiManager->CloseByKeyName(TEXT("PopupItemDetail"), true);
		}

		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupItemDetail"));
		if (widget.IsValid())
		{
			if (UGsUIPopupItemDetailMain* popupDetail = Cast<UGsUIPopupItemDetailMain>(widget))
			{
				popupDetail->SetItemData(param->_itemTID, true, UGsUIPopupItemDetailMain::CenterPosType::CenterPos, false);
			}
		}
	}
}

// 게임에 노출되지 말아야할 인앱결제 상품이 노출되는 경우 동적으로 서버로부터 패킷을 받아 처리함
void FGsBMShopHandler::CloseShopByBlock(FGsSharedParam& InParam)
{
	// 상품 오류로 인하여 상점을 종료 합니다
	// 다시 오픈해주세요
	FText msg;
	FText::FindText(TEXT("NetText"), TEXT("BM_SHOP_NOTICE_CLOSED"), msg);

	if (UGsUIManager* uiMgr = GUI())
	{
		if (true == uiMgr->IsActiveWidget(TEXT("WindowBMShop")))
		{
			if (true == uiMgr->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
			{
				uiMgr->CloseByKeyName(UGsUIPopupBMShopProductBuy::ASSET_NAME);
			}
			if (true == uiMgr->IsActiveWidget(TEXT("PopupItemDetail")))
			{
				uiMgr->CloseByKeyName(TEXT("PopupItemDetail"));
			}
			uiMgr->CloseByKeyName(TEXT("WindowBMShop"));
			FGsUIHelper::PopupOK(msg);
		}
	}
}

void FGsBMShopHandler::FinishedPeriodItem(const struct IGsMessageParam* InParam)
{
	const FGsCoolTimeMessageParam* param = InParam->Cast<const FGsCoolTimeMessageParam>();
	if (nullptr == param)
		return;

	if (EGsCoolTime::ItemPeriod != param->_type)
		return;

	TWeakPtr<FGsItem> findItem = GItem()->FindItem(param->_groupId);
	if (false == findItem.IsValid())
		return;

	if (ItemCategorySub::DISCOUNT_COUPON != findItem.Pin()->GetSubCategory())
		return;

	FGsBMShopManager* shopMgr = GBMShop();
	if (findItem.Pin()->GetDBID() == shopMgr->GetReserveCouponItemDBID())
	{
		// 강제로 빼는 처리 필요
		shopMgr->SetReserveCouponItemDBID(0);
		
		UGsUIManager* uiManager = GUI();
		if (nullptr == uiManager)
			return;

		if (true == uiManager->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME);
			if (widget.IsValid())
			{
				UGsUIPopupBMShopProductBuy* popup = Cast<UGsUIPopupBMShopProductBuy>(widget.Get());
				if (nullptr == popup)
				{
					return;
				}
				popup->ExpiredCouponItem();
			}
		}

		/*
		if (true == uiManager->IsActiveWidget(TEXT("PopupCouponList")))
		{

		}
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupCouponList"));
		if (widget.IsValid())
		{
			UGsUIPopupCouponList* popup = Cast<UGsUIPopupCouponList>(widget.Get());
			if (nullptr == popup)
			{
				return;
			}
			popup->SetCouponList(param->_productID);
		}

		if (true == uiManager->IsActiveWidget(TEXT("PopupConfirmDiscount")))
			return;

		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupConfirmDiscount"));
		if (widget.IsValid())
		{
			UGsUIPopupConfirmDiscount* popup = Cast<UGsUIPopupConfirmDiscount>(widget.Get());
			if (nullptr == popup)
			{
				return;
			}
			popup->SetCouponInfo(InCouponItemDbid, InParamData->_pairList, InParamData->_isBulkSale);
			if (false == popup->OnClickOkCoupon.IsBoundToObject(this))
			{
				popup->OnClickOkCoupon.AddRaw(this, &FGsBMShopHandler::UseCouponCallback);
			}
		}

		*/
	}
}

void FGsBMShopHandler::OnLocalSpawnMeComplete(const struct IGsMessageParam* InMessageParam)
{
	FGsBMShopManager* BMShopManager = GBMShop();
	if (nullptr != BMShopManager)
	{
		BMShopManager->OnClickUserPgComplete();
	}
}

void FGsBMShopHandler::OpenBuyPopup(FGsSharedParam& InMessageParam)
{
	const FGsBMShopOpenPopupBuy* param = InMessageParam.GetParam<const FGsBMShopOpenPopupBuy>();
	if (nullptr == param)
		return;

	FGsBMShopProductData* productData = GBMShop()->FindProduct(param->_productID);
	if (nullptr == productData)
	{
		GSLOG(Warning, TEXT("[FGsBMShopHandler::OpenBuyPopup] param->_productID error : %d"), param->_productID);
		return;
	}

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME);
	if (widget.IsValid())
	{
		UGsUIPopupBMShopProductBuy* popup = Cast<UGsUIPopupBMShopProductBuy>(widget.Get());
		if (nullptr == popup)
		{
			return;
		}
		popup->SetProductBuyInfo(productData);
	}
}

void FGsBMShopHandler::OpenGroupDetailPopup(FGsSharedParam& InMessageParam)
{
	const FGsBMShopOpenPopupGroupDetail* param = InMessageParam.GetParam<const FGsBMShopOpenPopupGroupDetail>();
	if (nullptr == param)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(TEXT("PopupGroupDetail")))
		return;

	if (0 >= param->_itemId)
	{
		GSLOG(Warning, TEXT("[FGsBMShopHandler::OpenGroupDetailPopup] param->_itemId error : %d"), param->_itemId);
		return;
	}

	const FGsSchemaItemCommon* commonData = FGsItem::GetTableDataByID(param->_itemId);
	if (nullptr == commonData)
	{
		GSLOG(Warning, TEXT("[FGsBMShopHandler::OpenGroupDetailPopup] FGsItem::GetTableDataByID error : %d"), param->_itemId);
		return;
	}

	if (ItemCategorySub::REWARD_BOX != commonData->categorySub)
		return;

	const UGsTableItemConsumeDetail* table = Cast<UGsTableItemConsumeDetail>(FGsSchemaItemConsumeDetail::GetStaticTable());
	if (nullptr == table)
	{
		return;
	}

	const FGsSchemaItemConsumeDetail* row = nullptr;
	if (false == table->FindRowById(commonData->detailId, row))
	{
		return;
	}

	if (0 >= row->effectIDList.Num())
	{
		return;
	}

	TArray<FGsRewardUnboxingData> dataList;
	for (int32 id : row->effectIDList)
	{
		FGsRewardUnboxingData bagItemData;
		if (FGsRewardHelper::UnboxingRewardData(id, bagItemData))
		{
			dataList.Emplace(bagItemData);
		}
	}

	if (0 >= dataList.Num())
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupGroupDetail"));
	if (widget.IsValid())
	{
		UGsUIPopupBMShopGroupDetail* popup = Cast<UGsUIPopupBMShopGroupDetail>(widget.Get());
		if (nullptr == popup)
		{
			return;
		}

		popup->SetProvideItem(dataList);
	}
}

void FGsBMShopHandler::SuccessePurchaseItem(FGsSharedParam& InMessageParam)
{
	const FGsBMShopPurchaseSuccess* param = InMessageParam.GetParam<const FGsBMShopPurchaseSuccess>();
	if (nullptr == param)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
		return;

	// 보관함 정보 받아놓음
	FGsNetSendServiceBMShop::SendBMShopStorageList();

	if (true == uiManager->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
	{
		uiManager->CloseByKeyName(UGsUIPopupBMShopProductBuy::ASSET_NAME, true);
		return;
	}

	if (0 < param->_productIdList.Num())
	{
		if (param->_isBulkSale)
		{
			FGsUIHelper::TrayTickerBMShopPurchase(TPair<FGsBMShopProductData*, ItemAmount>(nullptr, 0), true, false);
			return;
		}
		else
		{
			bool isSummonType = false;
			TPair<FGsBMShopProductData*, ItemAmount> pairData;
			TPair<BMProductId, ItemAmount> paramPairData = param->_productIdList[0];
			BMProductId id = paramPairData.Key;
			ItemAmount amount = paramPairData.Value;

			FGsBMShopProductData* productData = bmShopMgr->FindProduct(id);
			if (nullptr != productData)
			{
				pairData = TPair<FGsBMShopProductData*, ItemAmount>(productData, amount);

				// Test를 위한 코드
				// 에디터 테스트 완료 후 제거
#if WITH_EDITOR
				bool isCash = productData->IsCashProductItem();
#else
				bool isCash = false;
#endif

				// 인게임 재화 뽑기권 구매 했을때는 티커 출력 안함
				isSummonType = bmShopMgr->IsSummonTypeProduct(productData->GetProductID());
				if (_iapPurchaseBase.IsValid() && productData->IsHiveProduct() && DevicePlatformType::WINDOWS != _iapPurchaseBase->GetCurrenctPlatformType())
				{
					_iapPurchaseBase->OnTransactionFinishSingle(productData->GetHiveMarketPID());
					isCash = true;
				}

				if (false == isSummonType)
				{
					FGsUIHelper::TrayTickerBMShopPurchase(pairData, param->_isBulkSale, isCash);
				}
			}
		}
	}
}

void FGsBMShopHandler::SendTransactionFinishSingle(FGsSharedParam& InMessageParam)
{
	const FGsBMShopSendTransactionFinishSingle* param = InMessageParam.GetParam<const FGsBMShopSendTransactionFinishSingle>();
	if (nullptr == param)
		return;

	if (_iapPurchaseBase.IsValid() && false == param->_pid.IsEmpty() && DevicePlatformType::WINDOWS != _iapPurchaseBase->GetCurrenctPlatformType())
	{
		std::string str = TCHAR_TO_UTF8(*param->_pid);
		_iapPurchaseBase->OnTransactionFinishSingle(str);
	}
}

bool FGsBMShopHandler::CouponValidCheck()
{
	FGsBMShopManager* bmShopMgr = GBMShop();
	uint64 couponItemDbdi = bmShopMgr->GetReserveCouponItemDBID();
	if (0 >= couponItemDbdi)
	{
		FText findText;
		FText::FindText(TEXT("BMShopText"), TEXT("voidErrorBuyCoupon"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return false;
	}

	TWeakPtr<FGsItem> findItem = GItem()->FindItem(couponItemDbdi);
	if (false == findItem.IsValid())
	{
		FText findText;
		FText::FindText(TEXT("BMShopText"), TEXT("voidErrorBuyCoupon"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return false;
	}
	if (findItem.Pin()->IsExpiredTime())
	{
		// 해당 쿠폰의 유효기간이 초과되었습니다
		FText findText;
		FText::FindText(TEXT("BMShopText"), TEXT("expirationErrorBuyCoupon"), findText);
		FGsUIHelper::TrayMessageTicker(findText);

		return false;
	}

	return true;
}

void FGsBMShopHandler::CouponValidCheck(FGsSharedParam& InMessageParam)
{
	const FGsBMShopCheckValidCoupon* param = InMessageParam.GetParam<const FGsBMShopCheckValidCoupon>();
	if (nullptr == param)
		return;

	// 쿠폰 적용을 뺄 수 있어야 하기에 0 == _couponItemDBID 일수도 있음
	FGsBMShopManager* shopMgr = GBMShop();
	TWeakPtr<FGsItem> findItem;
	uint64 couponItemDbid = shopMgr->GetReserveCouponItemDBID();
	if (0 < couponItemDbid)
	{
		 findItem = GItem()->FindItem(couponItemDbid);
		if (false == findItem.IsValid())
		{
			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("voidErrorBuyCoupon"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			
			shopMgr->SetReserveCouponItemDBID(0);
			return;
		}
		if (findItem.Pin()->IsExpiredTime())
		{
			// 해당 쿠폰의 유효기간이 초과되었습니다
			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("expirationErrorBuyCoupon"), findText);
			FGsUIHelper::TrayMessageTicker(findText);

			shopMgr->SetReserveCouponItemDBID(0);
			return;
		}
	}

	FGsBMShopProductData* productData = shopMgr->FindProduct(param->_productID);
	if (nullptr == productData)
	{
		// ProductData Error
		shopMgr->SetReserveCouponItemDBID(0);
		return;
	}

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (false == uiManager->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME);
	if (widget.IsValid())
	{
		UGsUIPopupBMShopProductBuy* popup = Cast<UGsUIPopupBMShopProductBuy>(widget.Get());
		if (nullptr == popup)
		{
			return;
		}
		popup->InvalidateCoupontPrice(productData, false == findItem.IsValid() ? nullptr : findItem.Pin().Get());
	}
}

void FGsBMShopHandler::ValidCheck(FGsSharedParam& InMessageParam)
{
	FGsBMShopManager* BMShopManager = GBMShop();
	if (nullptr == BMShopManager)
	{
		GSLOG(Error, TEXT("nullptr == BMShopManager"));
		return;
	}

	const FGsBMShopCheckValidProduct* Param = InMessageParam.GetParam<const FGsBMShopCheckValidProduct>();
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	if (ValidCheckInter(*Param) && _iapPurchaseBase.IsValid())
	{
		const ItemDBId couponDbid = BMShopManager->GetReserveCouponItemDBID();
		if (0 < couponDbid)
		{
			if (CouponValidCheck())
			{
				OpenConfirmCouponPopup(couponDbid, Param);
			}
		}
		else
		{
			_iapPurchaseBase->TryPurchase(Param->_pairList, Param->_isBulkSale, 0);
		}
	}
	else
	{
		// B1 BM상점 튜토리얼에서 구매 진행 중 실패할 경우 바로 종료
		if (FGsTutorialManager* mgr = GSTutorial())
		{
			if (mgr->IsActiveTutorial())
			{
				GMessage()->GetTutorial().SendMessage(MessageContentTutorial::FORCED_END_TUTORIAL, nullptr);
			}
		}

		BMShopManager->DeletePurchaseSequentially();
	}
}

bool FGsBMShopHandler::ValidCheckInter(const FGsBMShopCheckValidProduct& InParam)
{
	if (0 >= InParam._pairList.Num())
	{
		return false;
	}

	FGsBMShopManager* bmShopMgr = GBMShop();
	for (const BMProductIdAmountPair& pairData : InParam._pairList)
	{
		FGsBMShopProductData* productData = bmShopMgr->FindProduct(pairData.mBMProductId);
		if (nullptr == productData)
		{
			GSLOG(Warning, TEXT("[FGsBMShopHandler::ValidCheck] pairData.mBMProductId error : %d"), pairData.mBMProductId);
			continue;
		}

		// 현금 결제 상품은 윈도우 플랫폼에서 구매 불가능
		if (true == productData->IsCashProductItem() && false == bmShopMgr->IsAllowBuyCashItem())
		{
			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("SyncPay_Buy_Popup_Desc"), findText);
			FGsUIHelper::PopupOK(findText);
			return false;
		}

		// 길드 상품
		if (true == productData->IsGuildGift())
		{
			FGsGuildManager* guildMgr = GGuild();
			if (false == guildMgr->IsGuild())
			{
				// 길드 미가입 상태
				FText findText;
				FText::FindText(TEXT("GuildText"), TEXT("GuildGift_Alert_NotInGuild"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}

			if (guildMgr->GetGuildLevel() < productData->GetGuildGiftLimitLevel())
			{
				// 길드 레벨 부족 상태
				FText findText;
				if (true == FText::FindText(TEXT("GuildText"), TEXT("UI_GuildShop_LevelLimit"), findText))
				{
					FText msg = FText::Format(findText, FText::AsNumber(productData->GetGuildGiftLimitLevel()));
					FGsUIHelper::TrayMessageTicker(msg);
				}
				return false;
			}
		}
		// 팝업상점 상품 시간 체크
		if (productData->IsPopupShopProduct())
		{
			if (false == bmShopMgr->IsOpenedPopupShopProduct(productData->GetProductID()))
			{
				FText findText;
				FText::FindText(TEXT("BMShopText"), TEXT("errorBuyAfterSaleEndTime"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}
		}
		// 구매 횟수 제한 체크
		if (true == productData->IsSlodOut())
		{
			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("errorBuyQuantityLimit"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}

		// pc 레벨
		if (false == productData->IsEnoughLevelLimit())
		{
			// 레벨 제한 티커 출력
			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("errorBuyPcLevelLimit"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}

		// 구매 가능 시간 인지 체크
		if (true == productData->IsExpireSale())
		{
			// 판매 시간 종료 티커 출력
			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("errorBuyAfterSaleEndTime"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}

		/*
		if (false == productData->IsAlwaysSlod())
		{
			// 구매 가능 시간 인지 체크
			if (true == productData->IsExpireSale())
			{
				// 판매 시간 종료 티커 출력
				FText findText;
				FText::FindText(TEXT("BMShopText"), TEXT("errorBuyAfterSaleEndTime"), findText);
				FGsUIHelper::TrayMessageTicker(findText);
				return false;
			}
		}
		*/
		if (true == productData->IsBeforSale())
		{
			// 판매 시작 시간이 아직 안됌 티커 출력
			FText findText;
			FText::FindText(TEXT("BMShopText"), TEXT("errorBuyBeforeSaleStartTime"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return false;
		}
		// 비용 체크 (마일리지도 포함)
		if (CurrencyType::CASH != productData->GetPurchaseCurrencyType())
		{
			Currency amount = 0;

			// 만약 비용이 아이템 타입(젠, 스타 등)일 때
			if (productData->IsItemCurrencyType())
			{
				// 만약 잠겨 있을 경우...
				if (GItem()->IsItemLockByTId(productData->GetPurchaseCurrencyItemId()))
				{
					// 잠긴 아이템은 사용할 수 없습니다. 티커 출력
					FText findText;
					FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Use_Notice_Locked"), findText);
					FGsUIHelper::TrayMessageTicker(findText);
					return false;
				}

				amount = FGsCurrencyHelper::GetItemCurrencyAmount(productData->GetPurchaseCurrencyItemId());
			}
			// 아이템 타입이 아닌 경우
			else
			{
				amount = FGsCurrencyHelper::GetCurrencyAmount(productData->GetPurchaseCurrencyType());
			}

			uint64 price = (InParam._isUseDiscount ? InParam._discountCurrencyAmount : productData->GetPurchasePrice()) * pairData.mAmount;
			if (price > amount)
			{
				OnShowPurchaseFailPopup(productData, price, amount);
				return false;
			}
		}
	}

	return true;
}

void FGsBMShopHandler::OpenConfirmCouponPopup(const uint64 InCouponItemDbid, const FGsBMShopCheckValidProduct* InParamData)
{
	if (0 >= InCouponItemDbid)
		return;

	if (nullptr == InParamData)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(TEXT("PopupConfirmDiscount")))
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupConfirmDiscount"));
	if (widget.IsValid())
	{
		UGsUIPopupConfirmDiscount* popup = Cast<UGsUIPopupConfirmDiscount>(widget.Get());
		if (nullptr == popup)
		{
			return;
		}
		popup->SetCouponInfo(InCouponItemDbid, InParamData->_pairList, InParamData->_isBulkSale);
		if (false == popup->OnClickOkCoupon.IsBoundToObject(this))
		{
			popup->OnClickOkCoupon.AddRaw(this, &FGsBMShopHandler::UseCouponCallback);
		}
	}
}

void FGsBMShopHandler::UseCouponCallback(const TArray<BMProductIdAmountPair>& InPairList, bool InBulkSale, const uint64 InCouponItemDbid)
{
	if (0 >= InPairList.Num())
		return;

	if (0 >= InCouponItemDbid)
		return;

	if (!_iapPurchaseBase.IsValid())
	{
		GSLOG(Error, TEXT("!_iapPurchaseBase.IsValid()"));
		return;
	}

	_iapPurchaseBase->TryPurchase(InPairList, InBulkSale, InCouponItemDbid);
}

void FGsBMShopHandler::OnShowPurchaseFailPopup(FGsBMShopProductData* InProductData, uint64 InTotalCost, Currency InOwnedAmount)
{
	GUI()->CloseByKeyName(UGsUIPopupBMShopProductBuy::ASSET_NAME, true);

	// 재화 부족 팝업 띄움
	const uint64 needCost = InTotalCost - InOwnedAmount;
	if (InProductData->IsItemCurrencyType())
	{
		FGsCurrencyHelper::OpenLackItemPopup(InProductData->GetPurchaseCurrencyItemId(), needCost, false);
		
	}
	else
	{
		FGsCurrencyHelper::OpenLackCurrencyPopup(InProductData->GetPurchaseCurrencyType(), needCost);
	}
}

void FGsBMShopHandler::MoveToTargetTab()
{
	MakeShared<FGsBMShopChangeTab>(EGsBMShopSpecialTab::CurrencyTab)->Send();

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
	{
		uiManager->CloseByKeyName(UGsUIPopupBMShopProductBuy::ASSET_NAME);
	}
}

void FGsBMShopHandler::GetStorageItem(FGsSharedParam& InMessageParam)
{
	const FGsBMShopAcceptStorage* param = InMessageParam.GetParam<const FGsBMShopAcceptStorage>();
	if (nullptr == param)
		return;

	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
		return;	

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupCommonItemAction"));
	if (widget.IsValid())
	{
		UGsUIPopupCommonItemAction* popup = Cast<UGsUIPopupCommonItemAction>(widget.Get());
		if (nullptr == popup)
			return;		

		// 상품 수령 시, 청약 철회가 제한됩니다.
		// 받으시겠습니까 ?
		uint64 storageItemDBID = param->_storageItemDBID;
		FText findText;
		FText::FindText(TEXT("BMShopText"), TEXT("storageGetConfirm"), findText);
		popup->SetDescription(findText);
		popup->SetCallBack([&, storageItemDBID](bool state)
			{
				if (true == state)
				{
					FGsNetSendServiceBMShop::SendReceiveStorageItem(storageItemDBID);
				}
			});
	}
}

void FGsBMShopHandler::RestoreCheck(FGsSharedParam& InMessageParam)
{
#if WITH_EDITOR
	return;
#endif

	FGsUIHelper::HideBlockUI();

	if (!_iapPurchaseBase.IsValid())
	{
		return;
	}

	FGsBMShopManager* shopMgr = GBMShop();
	if (false == shopMgr->IsUseIAPSystem())
		return;

	if (false == _iapPurchaseBase->IsValidState())
	{
		TArray<IAPV4Type> marketTypeList;
		_iapPurchaseBase->GetMarketTypeList(marketTypeList);
		if (0 >= marketTypeList.Num()) // 등록된 마켓 정보 타입이 없음
		{
			_iapPurchaseBase->InitHiveIAP();
			return;
		}
		TArray<IAPV4Product> productInfoList;
		_iapPurchaseBase->GetHiveProductInfoList(productInfoList);
		if (0 >= productInfoList.Num()) // 상품들 정보가 없음
		{
			_iapPurchaseBase->GetHiveProductInfo();
			return;
		}

		if (0 != shopMgr->GetRetryHiveCallbackCount())
		{
			shopMgr->SetRetryHiveCallbackCount(0);
		}

		/*
		UGsPopupSystemManager* popupMgr = GPopupSystem();
		if (nullptr != popupMgr)
		{
			FString displayMsg = "[RestoreCheck] false == _iapPurchaseBase->IsValidState()";
			popupMgr->PopupSystemMsg(FText::FromString(displayMsg), nullptr);
		}
		*/
		return;
	}

#if !PLATFORM_WINDOWS // windows 플랫폼은 Restore 가 없음
	_iapPurchaseBase->CheckRestore(false);
#endif

}

void FGsBMShopHandler::UserCheckRestore(FGsSharedParam& InMessageParam)
{
#if WITH_EDITOR
	return;
#endif

	if (!_iapPurchaseBase.IsValid())
	{
		return;
	}

	FGsBMShopManager* shopMgr = GBMShop();
	if (false == shopMgr->IsUseIAPSystem())
		return;

	// 윈도우 인게임 접속해서 요청 할때
	// 1. 씽크페이 접중  2. 씽크페이 미접중
#if PLATFORM_WINDOWS
	FGsNetSendServiceBMShop::SendSearchUsingZpay();
#else // 폰 인게임 접속 해서 요청 할때 ( 씽크페이 모드는 보관함이 없으므로 복구 버튼을 누를수 없다 )
	_iapPurchaseBase->CheckRestore(true);
#endif

}

void FGsBMShopHandler::NotingToRestore(FGsSharedParam& InMessageParam)
{
	GSLOG(Error, TEXT("[FGsBMShopHandler::NotingToRestore]"));

	FGsUIHelper::HideBlockUI();

	if (!_iapPurchaseBase.IsValid())
	{
		return;
	}

	if (_iapPurchaseBase->_isUserCallRestoreFlag)
	{
		// 복구할 상품이 없습니다
		FText findText;
		FText::FindText(TEXT("BMShopText"), TEXT("SyncPay_NotExist_Recovery"), findText);
		FGsUIHelper::PopupSystemMsg(findText, []()
			{
			});

		_iapPurchaseBase->_isUserCallRestoreFlag = false;
	}
}

void FGsBMShopHandler::DisplayNotingToRestore(FGsSharedParam& InMessageParam)
{
	GSLOG(Error, TEXT("[FGsBMShopHandler::DisplayNotingToRestore]"));

	// 복구할 상품이 없습니다
	FText findText;
	FText::FindText(TEXT("BMShopText"), TEXT("SyncPay_NotExist_Recovery"), findText);
	FGsUIHelper::PopupSystemMsg(findText, []()
		{
		});
	//FGsUIHelper::TrayMessageTicker(findText);
}

// pc 유저만 응답 옴
void FGsBMShopHandler::AckSearchZpayUser(FGsSharedParam& InMessageParam)
{
	GSLOG(Error, TEXT("[FGsBMShopHandler::AckSearchZpayUser]"));

	const FGsBMShopZpayUserAck* param = InMessageParam.GetParam<const FGsBMShopZpayUserAck>();

	if (nullptr == param)
		return;

	if (!_iapPurchaseBase.IsValid())
	{
		return;
	}

	if (false == param->_isInZpayMode)
	{
#if PLATFORM_WINDOWS // 윈도우면 Zpay 다시 진입하라는 팝업 필요
		FText findText;
		FText::FindText(TEXT("NetText"), TEXT("ZPAY_NOTICE_PURCHASE_SUCCESS_RECOVERY"), findText);
		FGsUIHelper::PopupSystemMsg(findText, []()
			{
			});
#endif
	}
}

// Zpay 모바일 기기에서만 응답옴
void FGsBMShopHandler::NotifySearchZpayUser(FGsSharedParam& InMessageParam)
{
	GSLOG(Error, TEXT("[FGsBMShopHandler::NotifySearchZpayUser]"));

	if (!_iapPurchaseBase.IsValid())
	{
		return;
	}

#if PLATFORM_ANDROID || PLATFORM_IOS
	_iapPurchaseBase->CheckRestore(true);
#endif
}

void FGsBMShopHandler::OnReconnection()
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	uiManager->CloseByKeyName(TEXT("WindowBMShop"));
}

void FGsBMShopHandler::MarketConnection(FGsSharedParam& InMessageParam)
{
	//FGsUIHelper::HideBlockUI();

	if (_iapPurchaseBase.IsValid())
	{
		TArray<IAPV4Type> marketTypeList;
		_iapPurchaseBase->GetMarketTypeList(marketTypeList);
		if (0 >= marketTypeList.Num()) // 등록된 마켓 정보 타입이 없음
		{
			GSLOG(Error, TEXT("[FGsBMShopHandler::MarketConnection] 0 >= marketTypeList.Num()"));
			return;
		}
		FGsBMShopManager* shopMgr = GBMShop();
		if (shopMgr)
		{
			shopMgr->SetRetryHiveCallbackCount(0);
			shopMgr->OnIAPV4SheetsLog(TEXT("SuccessInitHiveIAP"));
			_iapPurchaseBase->GetHiveProductInfo();
			//_iapPurchaseBase->GetSubscriptionProductInfo();
		}

		/*
		GSLOG(Error, TEXT("[FGsBMShopHandler::MarketConnection] marketTypeList.Num() : %d"), marketTypeList.Num());
		for (int32 i = 0; i < marketTypeList.Num(); ++i)
		{
			IAPV4Type type = marketTypeList[i];
			GSLOG(Error, TEXT("[FGsBMShopHandler::MarketConnection] marketTypeList type : %d"), (uint8)type);
		}
		*/
	}
}

void FGsBMShopHandler::HiveProductInfo(FGsSharedParam& InMessageParam)
{
	FGsUIHelper::HideBlockUI();

	if (_iapPurchaseBase.IsValid())
	{
		TArray<IAPV4Product> productInfoList;
		_iapPurchaseBase->GetHiveProductInfoList(productInfoList);
		if (0 >= productInfoList.Num())
		{
			GSLOG(Warning, TEXT("[FGsBMShopHandler::HiveProductInfo] 0 >= productInfoList.Num()"));
			// 상품들 정보가 없음
			// 예외 처리 필요
			return;
		}

		GSLOG(Log, TEXT("[FGsBMShopHandler::HiveProductInfo] productInfoList.Num() : %d"), productInfoList.Num());
		/*
		for (int32 i = 0; i < productInfoList.Num(); ++i)
		{
			IAPV4Product info = productInfoList[i];
			GSLOG(Error, TEXT("[FGsBMShopHandler::HiveProductInfo] productInfoList info price : %d"), info.price);
			
			if (0 >= info.marketPid.length())
			{
				GSLOG(Error, TEXT("[FGsBMShopHandler::HiveProductInfo] 0 >= info.marketPid.length() "));
				continue;
			}
			FString temp = *FString(info.marketPid.c_str());
			GSLOG(Error, TEXT("[FGsBMShopHandler::HiveProductInfo] productInfoList info pid : %s"), *temp);
		}
		*/

		FGsBMShopManager* bmShopMgr = GBMShop();
		if (bmShopMgr)
		{
			bmShopMgr->SetRetryHiveCallbackCount(0);
			bmShopMgr->OnIAPV4SheetsLog(TEXT("SuccessGetHiveProductInfo"));
			bmShopMgr->InitHiveProductList(productInfoList, _iapPurchaseBase->GetCurrenctPlatformType());
		}
	}
}

void FGsBMShopHandler::OnHiveCallbackBlockUI(FGsSharedParam& InMessageParam)
{
	FGsUIHelper::ShowBlockUI();
	if (FGsBMShopManager * bmShopMgr = GBMShop())
	{
		bmShopMgr->OnIAPV4SheetsLog(TEXT("OnHiveCallbackBlockUI"));
	}
}

void FGsBMShopHandler::OnHiveCallbackCloseBlockUI(FGsSharedParam& InMessageParam)
{
	FGsUIHelper::HideBlockUI();
	if (FGsBMShopManager* bmShopMgr = GBMShop())
	{
		bmShopMgr->OnIAPV4SheetsLog(TEXT("OnHiveCallbackCloseBlockUI"));
	}
}

//void FGsBMShopHandler::TransactionFinish(FGsSharedParam& InMessageParam)
//{
//	GSLOG(Error, TEXT("[FGsBMShopHandler::TransactionFinish]"));
//
//	if (nullptr == InMessageParam)
//		return;
//
//	const FGsBMShopOnTransactionFinish* param = InMessageParam->Cast<const FGsBMShopOnTransactionFinish>();
//	if (nullptr == param)
//		return;
//
//	if (0 >= param->_productIDList.Num())
//		return;
//
//	if (nullptr == _iapPurchaseBase)
//		return;
//
//	FGsBMShopManager* shopMgr = GBMShop();
//	TArray<std::string> marketPIDList;
//	for (const BMProductId iter : param->_productIDList)
//	{
//		BMProductId productID = iter;
//		if(0 >= productID)
//			continue;
//
//		FGsBMShopProductData* data = shopMgr->FindProduct(productID);
//		if(nullptr == data)
//			continue;
//
//		marketPIDList.Emplace(data->GetHiveMarketPID());
//	}
//
//	_iapPurchaseBase->OnTransactionFinishMulti(marketPIDList);
//}

void FGsBMShopHandler::OpenBMShopWithTab(FGsSharedParam& InMessageParam)
{
	const FGsBMShopOpenTab* param = InMessageParam.GetParam<const FGsBMShopOpenTab>();
	if (nullptr == param)
		return;

	if (false == GBMShop()->IsExistTabID(param->_tabID))
		return;

	FGsUnlockManager* unlockMgr = GSUnlock();
	if (false == unlockMgr->IsContentsUnlock(EGsUnlockContentsType::BMSHOP))
	{
		unlockMgr->ShowLockTicker(EGsUnlockContentsType::BMSHOP);
		return;
	}

	FGsMessageHolder* msg = GMessage();
	msg->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);
	MakeShared<FGsBMShopChangeTabFromId>(param->_tabID)->Send();
}

void FGsBMShopHandler::OpenBMShopWithSpecialTab(FGsSharedParam& InMessageParam)
{
	FGsUnlockManager* unlockMgr = GSUnlock();
	if (false == unlockMgr->IsContentsUnlock(EGsUnlockContentsType::BMSHOP, true))
	{
		return;
	}

	const FGsBMShopOpenSpecialTab* param = InMessageParam.GetParam<const FGsBMShopOpenSpecialTab>();
	if (nullptr == param)
		return;

	if (EGsBMShopSpecialTab::None == param->_tabType)
		return;

	FGsMessageHolder* msg = GMessage();
	msg->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);
	MakeShared<FGsBMShopChangeTab>(param->_tabType)->Send();
}

void FGsBMShopHandler::OpenPopupShop(FGsSharedParam& InMessageParam)
{
	// 윈도우 플랫폼은 하이브 crossplay 에서 interwork 기능을 지원하지 않으므로 처리 x
#if PLATFORM_IOS || PLATFORM_ANDROID 
	const FGsBMShopOpenPopupShop* param = InMessageParam.GetParam<const FGsBMShopOpenPopupShop>();
	if (nullptr == param)
		return;

	if (0 >= param->_productID)
		return;

	FGsBMShopManager* shopMgr = GBMShop();
	FGsBMShopProductData* data = shopMgr->FindProduct(param->_productID);
	if (nullptr == data)
	{
		GSLOG(Error, TEXT("[FGsBMShopHandler::OpenPopupShop] data is nullptr !! param->_productID : %d"), param->_productID);
		return;
	}
	if (data->IsExistParentProductID())
	{
		data = shopMgr->FindProduct(data->GetParentProductID());
	}
	if (nullptr == data)
	{
		GSLOG(Error, TEXT("[FGsBMShopHandler::OpenPopupShop] GetParentProductID is nullptr !! data->GetParentProductID() : %d"), data->GetParentProductID());
		return;
	}
	if (0 >= data->GetPopupShopHiveBannerID())
	{
		GSLOG(Error, TEXT("[FGsBMShopHandler::OpenPopupShop] data->GetPopupShopHiveBannerID() is 0 !!  %d"), data->GetPopupShopHiveBannerID());
		return;
	}
	if (data->IsCashProductItem() && false == shopMgr->IsUseIAPSystem())
	{
		return;
	}
	if (_iapPurchaseBase.IsValid())
	{
		_iapPurchaseBase->ShowPopupShopCustomContents(data->GetPopupShopHiveBannerID());
	}	
#endif
}

void FGsBMShopHandler::OpenBMShopHUD(FGsSharedParam& InMessageParam)
{
	FGsBMShopManager* shopMgr = GBMShop();
	FGsUnlockManager* unlockMgr = GSUnlock();

	//주석해지필요
	if (false == unlockMgr->IsContentsUnlock(EGsUnlockContentsType::BMSHOP))
	{
		unlockMgr->ShowLockTicker(EGsUnlockContentsType::BMSHOP);
		return;
	}

	FGsMessageHolder* msg = GMessage();
	BMProductId productID = shopMgr->GetPopupShopLessRemainTimeProductID();
	if (0 < productID)
	{
		FGsBMShopProductData* data = shopMgr->FindProduct(productID);
		if (nullptr == data)
		{
			msg->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);
		}
		else
		{
			msg->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);

			MakeShared<FGsBMShopChangeFocusingProduct>(data->GetParentTabID(), data->GetSubTabID(), productID)->Send();
		}

		return;
	}

	msg->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);
}

void FGsBMShopHandler::OpenPopupShopPromotion(FGsSharedParam& InMessageParam)
{
	const FGsBMShopOpenBMShopPromotion* param = InMessageParam.GetParam<const FGsBMShopOpenBMShopPromotion>();
	if (nullptr == param)
		return;

	FGsBMShopManager* shopMgr = GBMShop();
	FGsUnlockManager* unlockMgr = GSUnlock();

	//주석해지필요
	if (false == unlockMgr->IsContentsUnlock(EGsUnlockContentsType::BMSHOP))
	{
		unlockMgr->ShowLockTicker(EGsUnlockContentsType::BMSHOP);
		return;
	}

	FGsMessageHolder* msg = GMessage();
	if (0 < param->_productID)
	{
		FGsBMShopProductData* data = shopMgr->FindProduct(param->_productID);
		if (nullptr == data)
		{
			msg->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);
		}
		else
		{
			msg->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);

			MakeShared<FGsBMShopChangeFocusingProduct>(data->GetParentTabID(), data->GetSubTabID(), param->_productID)->Send();
		}

		return;
	}

	msg->GetContents().SendMessage(MessageContents::ACTIVE_BMSHOP);
}

void FGsBMShopHandler::ServerValidCheckResult(FGsSharedParam& InMessageParam)
{
	//GSLOG(Warning, TEXT("[FGsBMShopHandler::ServerValidCheckResult]"));
	if (!_iapPurchaseBase.IsValid())
		return;

	const FGsBMShopCheckValidProductResult* param = InMessageParam.GetParam<const FGsBMShopCheckValidProductResult>();
	if (nullptr == param)
		return;

	_iapPurchaseBase->TryHivePurchase(param->_amountPairList, param->_identifier, param->_isBulkSale, param->_couponItemDbid);
}

void FGsBMShopHandler::OpenBMShopValidHiveProduct(FGsSharedParam& InMessageParam)
{
	GSLOG(Error, TEXT("FGsBMShopHandler::OpenBMShopValidHiveProduct"));

	const FGsBMShopOpenBMShopValidHive* param = InMessageParam.GetParam<const FGsBMShopOpenBMShopValidHive>();
	if (nullptr == param)
		return;

	if (_iapPurchaseBase.IsValid())
	{
		GSLOG(Error, TEXT("FGsBMShopHandler::OpenBMShopValidHiveProduct 11 "));
		TArray<IAPV4Type> marketTypeList;
		_iapPurchaseBase->GetMarketTypeList(marketTypeList);
		if (0 >= marketTypeList.Num()) // 등록된 마켓 정보 타입이 없음
		{
			if (param->_valueFlag)
			{
				// 기록 필요
				GBMShop()->OnIAPV4SheetsLog(TEXT("isErrorInitFromShopEnter"));
			}
			GSLOG(Error, TEXT("FGsBMShopHandler::OpenBMShopValidHiveProduct 22 "));

			_iapPurchaseBase->InitHiveIAP();
			return;
		}

		TArray<IAPV4Product> productInfoList;
		_iapPurchaseBase->GetHiveProductInfoList(productInfoList);
		if (0 >= productInfoList.Num()) // 상품들 정보가 없음
		{
			if (param->_valueFlag)
			{
				// 기록 필요
				GBMShop()->OnIAPV4SheetsLog(TEXT("isErrorGetProductFromShopEnter"));
			}

			GSLOG(Error, TEXT("FGsBMShopHandler::OpenBMShopValidHiveProduct 33 "));
			_iapPurchaseBase->GetHiveProductInfo();
			return;
		}

		GSLOG(Error, TEXT("FGsBMShopHandler::OpenBMShopValidHiveProduct 44 "));


		if (FGsBMShopManager* shopMgr = GBMShop())
		{
			if (0 != shopMgr->GetRetryHiveCallbackCount())
			{
				shopMgr->SetRetryHiveCallbackCount(0);
			}
		}
	}

	GSLOG(Error, TEXT("FGsBMShopHandler::OpenBMShopValidHiveProduct 55 "));
}

void FGsBMShopHandler::OnFailedHiveCallback(FGsSharedParam& InMessageParam)
{
	// Hive 팀에서 해당 Error 일때는 리트라이 시도를 해달라고 함
	static const TSet<hive::ResultAPI::Code> NEED_RETRY_CODES = {
		hive::ResultAPI::Code::IAPV4NetworkError,
		hive::ResultAPI::Code::IAPV4ResponseError,
		hive::ResultAPI::Code::IAPV4ServerResponseError,
		hive::ResultAPI::Code::IAPV4RequestMarketJsonException,
		hive::ResultAPI::Code::IAPV4AppStoreError,
		hive::ResultAPI::Code::IAPV4FailMarketConnect,
		hive::ResultAPI::Code::IAPV4AppStoreResponseEmpty,
		hive::ResultAPI::Code::IAPV4PlayStoreQueryInventoryFail
	};

	GSLOG(Log, TEXT("FGsBMShopHandler::OnFailedHiveCallback(FGsSharedParam& InMessageParam)"));

	const FGsBMShopHiveError* Param = InMessageParam.GetParam<const FGsBMShopHiveError>();
	if (nullptr == Param)
	{
		return;
	}

	GSLOG(Log, TEXT("[FGsBMShopHandler::OnFailedHiveCallback] Param: %s"), *LexToString(*Param));

	FGsUIHelper::HideBlockUI();

	if (!_iapPurchaseBase.IsValid())
	{
		return;
	}

	if (DevicePlatformType::EDITOR == Param->_devicePlatformType)
	{
		GSLOG(Log, TEXT("[FGsBMShopHandler::OnFailedHiveCallback] InCode : %d, InErrorCode : %d"), Param->_code, Param->_errorCode);
		return;
	}

	FGsBMShopManager* bmShopMgr = GBMShop();
	if (nullptr == bmShopMgr)
	{
		return;
	}

	bmShopMgr->OnIAPV4SheetsLog(TEXT("OnFailedHiveCallback"));

	if (NEED_RETRY_CODES.Contains(Param->_code))
	{
		// 마켓정보 및 상품정보 요청후에 실패 콜백이 온다면 1회 재연결 시도
		switch (Param->_hiveFailedCallbackType)
		{
		case FGsIAPPurchaseBase::HiveFailedCallbackType::MarketInit_Failed:
		{
			bmShopMgr->OnIAPV4SheetsLog(TEXT("FailedInitHiveIAP"));
			if (0 >= bmShopMgr->GetRetryHiveCallbackCount())
			{
				bmShopMgr->SetRetryHiveCallbackCount(1);
				_iapPurchaseBase->InitHiveIAP();
			}
			else
			{
				GSLOG(Log, TEXT("[FGsBMShopHandler::OnFailedHiveCallback] Retry Finished MarketInit InCode : %d, InErrorCode : %d"), Param->_code, Param->_errorCode);

				OnShowHiveErrorPopup(Param->_hiveFailedCallbackType, Param->_code, Param->_errorCode);
			}
			return;
		}
		case FGsIAPPurchaseBase::HiveFailedCallbackType::GetProductInfo_Failed:
		{
			bmShopMgr->OnIAPV4SheetsLog(TEXT("FailedGetHiveProductInfo"));
			if (0 >= bmShopMgr->GetRetryHiveCallbackCount())
			{
				bmShopMgr->SetRetryHiveCallbackCount(1);
				_iapPurchaseBase->GetHiveProductInfo();
			}
			else
			{
				GSLOG(Log, TEXT("[FGsBMShopHandler::OnFailedHiveCallback] Retry Finished GetProductInfo InCode : %d, InErrorCode : %d"), Param->_code, Param->_errorCode);

				OnShowHiveErrorPopup(Param->_hiveFailedCallbackType, Param->_code, Param->_errorCode);
			}
			return;
		}
		}
	}

	OnShowHiveErrorPopup(Param->_hiveFailedCallbackType, Param->_code, Param->_errorCode);

	GSLOG(Log, TEXT("[FGsBMShopHandler::OnFailedHiveCallback] InCode : %d, InErrorCode : %d"), Param->_code, Param->_errorCode);
}

void FGsBMShopHandler::OnFailedHiveCallbackMulti(FGsSharedParam& InMessageParam)
{
	FGsUIHelper::HideBlockUI();

	const FGsBMShopHiveErrors* Param = InMessageParam.GetParam<const FGsBMShopHiveErrors>();
	if (nullptr == Param)
	{
		return;
	}

	GSLOG(Log, TEXT("[FGsBMShopHandler::OnFailedHiveCallbackMulti] Param: %s"), *LexToString(*Param));

	if (DevicePlatformType::EDITOR == Param->_devicePlatformType)
	{
		GSLOG(Log, TEXT("[FGsBMShopHandler::OnFailedHiveCallbackMulti] InCode : %d, InErrorCode : %d"), Param->_code, Param->_errorCode);
		return;
	}

	OnShowHiveErrorPopup(Param->_hiveFailedCallbackType, Param->_code, Param->_errorCode);

	GSLOG(Log, TEXT("[FGsBMShopHandler::OnFailedHiveCallbackMulti] InCode : %d, InErrorCode : %d"), Param->_code, Param->_errorCode);
}

// Hive 쪽에서 Hive Error Callback 메세지 관련해서 유저 친화적으로 변경해달라고 하여
// Hive 도큐먼트 기준. 필수 구현 부분만 별도 처리함
void FGsBMShopHandler::OnShowHiveErrorPopup(const FGsIAPPurchaseBase::HiveFailedCallbackType InHiveFailedCallbackType, const hive::ResultAPI::Code InCode, const int32 InErrorCode)
{
	static const TSet<hive::ResultAPI::Code> MOVE_TO_TITLE_CODE_ON_ZPAY = {
		hive::ResultAPI::Code::IAPV4FailMarketConnect,
		hive::ResultAPI::Code::IAPV4NetworkError,
		hive::ResultAPI::Code::IAPV4ResponseError,
		hive::ResultAPI::Code::IAPV4ServerResponseError,
		hive::ResultAPI::Code::IAPV4RequestMarketJsonException,
		hive::ResultAPI::Code::IAPV4ProductInfoJsonException,
		hive::ResultAPI::Code::IAPV4RequestProductJsonException,
	};
	if (!_iapPurchaseBase.IsValid())
	{
		GSLOG(Error, TEXT("!_iapPurchaseBase.IsValid()"));
		return;
	}

	FString fromFailedText = "";
	switch (InHiveFailedCallbackType)
	{
	case FGsIAPPurchaseBase::HiveFailedCallbackType::MarketInit_Failed:
		fromFailedText = "MarketInit";
		break;
	case FGsIAPPurchaseBase::HiveFailedCallbackType::GetProductInfo_Failed:
		fromFailedText = "GetProductInfo";
		break;
	case FGsIAPPurchaseBase::HiveFailedCallbackType::Purchase_Failed:
		fromFailedText = "Purchase";
		break;
	case FGsIAPPurchaseBase::HiveFailedCallbackType::Restore_Failed:
		fromFailedText = "Restore";
		break;
	case FGsIAPPurchaseBase::HiveFailedCallbackType::TransactionFinishSingle_Failed:
		fromFailedText = "TF_Single";
		break;
	case FGsIAPPurchaseBase::HiveFailedCallbackType::TransactionFinishMulti_Failed:
		fromFailedText = "TF_Multi";
		break;
	}
	GSLOG(Log, TEXT("[FGsBMShopHandler::OnShowHiveErrorPopup] _failedCallbackType String : %s"), *fromFailedText);

	FTextKey findErrorTextKey;
	switch (InCode)
	{
	case hive::ResultAPI::Code::IAPV4NetworkError:
		findErrorTextKey = TEXT("APV4NetworkError");
		break;
	case hive::ResultAPI::Code::IAPV4ResponseError:
		findErrorTextKey = TEXT("IAPV4ResponseError");
		break;
	case hive::ResultAPI::Code::IAPV4ServerResponseError:
		findErrorTextKey = TEXT("IAPV4ServerResponseError");
		break;
	case hive::ResultAPI::Code::IAPV4RequestMarketJsonException:
		findErrorTextKey = TEXT("IAPV4RequestMarketJsonException");
		break;
	case hive::ResultAPI::Code::IAPV4AppStoreError:
		findErrorTextKey = TEXT("IAPV4AppStoreError");
		break;
	case hive::ResultAPI::Code::IAPV4FailMarketConnect:
		findErrorTextKey = TEXT("IAPV4FailMarketConnect");
		break;
	case hive::ResultAPI::Code::IAPV4ProductInfoJsonException:
		findErrorTextKey = TEXT("IAPV4ProductInfoJsonException");
		break;
	case hive::ResultAPI::Code::IAPV4RequestProductJsonException:
		findErrorTextKey = TEXT("IAPV4RequestProductJsonException");
		break;
	case hive::ResultAPI::Code::IAPV4EmptyMarketURL:
		findErrorTextKey = TEXT("IAPV4EmptyMarketURL");
		break;
	case hive::ResultAPI::Code::IAPV4CancelMarketSelect:
		findErrorTextKey = TEXT("IAPV4CancelMarketSelect");
		break;
	case hive::ResultAPI::Code::IAPV4RestrictPayments:
		findErrorTextKey = TEXT("IAPV4RestrictPayments");
		break;
	case hive::ResultAPI::Code::IAPV4NeedRestore:
		findErrorTextKey = TEXT("IAPV4NeedRestore");
		break;
	case hive::ResultAPI::Code::IAPV4PurchaseParamJsonException:
		findErrorTextKey = TEXT("IAPV4PurchaseParamJsonException");
		break;
	case hive::ResultAPI::Code::IAPV4CancelPayment:
		findErrorTextKey = TEXT("IAPV4CancelPayment");
		break;
	case hive::ResultAPI::Code::IAPV4FailPayment:
		findErrorTextKey = TEXT("IAPV4FailPayment");
		break;
	case hive::ResultAPI::Code::IAPPlayStoreLaunchPurchaseFlowException:
		findErrorTextKey = TEXT("IAPPlayStoreLaunchPurchaseFlowException");
		break;
	case hive::ResultAPI::Code::IAPV4LebiCancel:
		findErrorTextKey = TEXT("IAPV4LebiCancel");
		break;
	case hive::ResultAPI::Code::IAPV4LebiPurchaseNetworkError:
		findErrorTextKey = TEXT("IAPV4LebiPurchaseNetworkError");
		break;
	case hive::ResultAPI::Code::IAPV4LebiPurchaseJsonException:
		findErrorTextKey = TEXT("IAPV4LebiPurchaseJsonException");
		break;
	case hive::ResultAPI::Code::IAPV4LebiInternalRequestException:
		findErrorTextKey = TEXT("IAPV4LebiInternalRequestException");
		break;
	case hive::ResultAPI::Code::IAPV4PlayStoreFinishFail:
		findErrorTextKey = TEXT("IAPV4PlayStoreFinishFail");
		break;
	case hive::ResultAPI::Code::CommonUnknown:
		findErrorTextKey = TEXT("CommonUnknown");
		break;
	case hive::ResultAPI::Code::IAPV4FailToRestore:
		findErrorTextKey = TEXT("IAPV4FailToRestore");
		break;
	case hive::ResultAPI::Code::IAPV4InProgressMarketConnect:
		// 마켓에 연결 중입니다
		findErrorTextKey = TEXT("Notice_Loading_Market_Connect");
		break;
	case hive::ResultAPI::Code::IAPV4InProgressPurchasing:
		// 결제가 진행 중입니다.
		findErrorTextKey = TEXT("Notice_Loading_Market_Buy");
		break;
	case hive::ResultAPI::Code::IAPV4InProgressRestoring:
		// 복구가 진행 중입니다
		findErrorTextKey = TEXT("Notice_Loading_Market_Restore");
		break;
	case hive::ResultAPI::Code::IAPV4InProgressConnectAppStore:
		// 스토어에 연결 중입니다
		findErrorTextKey = TEXT("Notice_Loading_Market_Store");
		break;
	case hive::ResultAPI::Code::IAPV4NeedSignIn:
		findErrorTextKey = TEXT("IAPV4NeedSignIn");
		break;

	default:
		findErrorTextKey = TEXT("CommonUnknown");
		break;
	}
	
	FText findText;
	FText::FindText(TEXT("BMShopText"), findErrorTextKey, findText);
	
	UGsPopupSystemManager* popupMgr = GPopupSystem();
	if (nullptr != popupMgr)
	{
		const FString displayMsg = FString::Format(TEXT("{0}\nErrorCode:{1} Code:{2}\n{3}"), { findText.ToString(), InErrorCode, InCode, fromFailedText });
		popupMgr->PopupSystemMsg(FText::FromString(displayMsg), nullptr);
	}
}

void FGsBMShopHandler::OpenBulkSalePopup(FGsSharedParam& InMessageParam)
{
	const FGsBMShopOpenBulkSale* param = InMessageParam.GetParam<const FGsBMShopOpenBulkSale>();
	if (nullptr == param)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(TEXT("PopupBulkSale")))
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupBulkSale"));
	if (widget.IsValid())
	{
		UGsUIPopupBulkSale* popup = Cast<UGsUIPopupBulkSale>(widget.Get());
		if (nullptr == popup)
		{
			return;
		}
		popup->SetBulkSaleList(param->_bulkSaleProductInfoList);
	}
}

void FGsBMShopHandler::OpenCouponListPopup(FGsSharedParam& InMessageParam)
{
	const FGsBMShopOpenCouponList* param = InMessageParam.GetParam<const FGsBMShopOpenCouponList>();
	if (nullptr == param)
		return;

	if (0 >= param->_productID)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(TEXT("PopupCouponList")))
		return;

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupCouponList"));
	if (widget.IsValid())
	{
		UGsUIPopupCouponList* popup = Cast<UGsUIPopupCouponList>(widget.Get());
		if (nullptr == popup)
		{
			return;
		}
		popup->SetCouponList(param->_productID);
	}
}

void FGsBMShopHandler::OnUpdateCurrency(uint64 InData)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME);
		if (widget.IsValid())
		{
			UGsUIPopupBMShopProductBuy* popup = Cast<UGsUIPopupBMShopProductBuy>(widget.Get());
			if (nullptr == popup)
			{
				return;
			}
			popup->InvalidateCurrency();
		}
	}

	if (true == uiManager->IsActiveWidget(TEXT("PopupBulkSale")))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupBulkSale"));
		if (widget.IsValid())
		{
			UGsUIPopupBulkSale* popup = Cast<UGsUIPopupBulkSale>(widget.Get());
			if (nullptr == popup)
			{
				return;
			}
			popup->InvalidateCurrency();
		}
	}
}

void FGsBMShopHandler::OnUpdateUserLevel(const struct IGsMessageParam* inParam)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	if (true == uiManager->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME);
		if (widget.IsValid())
		{
			UGsUIPopupBMShopProductBuy* popup = Cast<UGsUIPopupBMShopProductBuy>(widget.Get());
			if (nullptr == popup)
			{
				return;
			}

			popup->InvalidateLevel();
		}
	}
	if (true == uiManager->IsActiveWidget(TEXT("PopupBulkSale")))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupBulkSale"));
		if (widget.IsValid())
		{
			UGsUIPopupBulkSale* popup = Cast<UGsUIPopupBulkSale>(widget.Get());
			if (nullptr == popup)
			{
				return;
			}
			popup->InvalidateLevel();
		}
	}
}

void FGsBMShopHandler::OnAddItemList(struct FGsItemMessageParamBase& InParam)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	bool isNeedUpdate = false;
	const FGsAddItemList* Param = StaticCast<const FGsAddItemList*>(&InParam);
	if (nullptr != Param && 0 < Param->_itemDbidList.Num())
	{
		UGsItemManager* itemMgr = GItem();
		for (const ItemDBId iter : Param->_itemDbidList)
		{
			if(0 >= iter)
				continue;

			TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter);
			if(false == findItem.IsValid())
				continue;

			if(false == findItem.Pin()->IsCurrency())
				continue;

			isNeedUpdate = true;
			break;
		}

		if (isNeedUpdate)
		{
			if (true == uiManager->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
			{
				TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME);
				if (widget.IsValid())
				{
					UGsUIPopupBMShopProductBuy* popup = Cast<UGsUIPopupBMShopProductBuy>(widget.Get());
					if (nullptr == popup)
					{
						return;
					}
					popup->InvalidateCurrency();
				}
			}
			if (true == uiManager->IsActiveWidget(TEXT("PopupBulkSale")))
			{
				TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupBulkSale"));
				if (widget.IsValid())
				{
					UGsUIPopupBulkSale* popup = Cast<UGsUIPopupBulkSale>(widget.Get());
					if (nullptr == popup)
					{
						return;
					}
					popup->InvalidateCurrency();
				}
			}
		}	
	}
}

void FGsBMShopHandler::OnUpdateItem(struct FGsItemMessageParamBase& InParam)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	if (nullptr != Param)
	{
		TWeakPtr<FGsItem> findItem = GItem()->FindItem(Param->_itemDBID);
		if (findItem.IsValid())
		{
			if (findItem.Pin()->IsCurrency())
			{
				if (true == uiManager->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
				{
					TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME);
					if (widget.IsValid())
					{
						UGsUIPopupBMShopProductBuy* popup = Cast<UGsUIPopupBMShopProductBuy>(widget.Get());
						if (nullptr == popup)
						{
							return;
						}
						popup->InvalidateCurrency();
					}
				}

				if (true == uiManager->IsActiveWidget(TEXT("PopupBulkSale")))
				{
					TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupBulkSale"));
					if (widget.IsValid())
					{
						UGsUIPopupBulkSale* popup = Cast<UGsUIPopupBulkSale>(widget.Get());
						if (nullptr == popup)
						{
							return;
						}
						popup->InvalidateCurrency();
					}
				}
			}
		}
	}
}

void FGsBMShopHandler::OnRemoveItem(struct FGsItemMessageParamBase& InParam)
{
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	FGsRemoveItem* paramData = static_cast<FGsRemoveItem*>(&InParam);
	if (nullptr == paramData)
		return;

	UGsItemManager* itemMgr = GItem();
	const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(paramData->_itemTID);
	if (nullptr == itemData)
		return;

	if (ItemType::CURRENCY == itemData->type)
	{
		if (true == uiManager->IsActiveWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME))
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(UGsUIPopupBMShopProductBuy::ASSET_NAME);
			if (widget.IsValid())
			{
				UGsUIPopupBMShopProductBuy* popup = Cast<UGsUIPopupBMShopProductBuy>(widget.Get());
				if (nullptr == popup)
				{
					return;
				}
				popup->InvalidateCurrency();
			}
		}

		if (true == uiManager->IsActiveWidget(TEXT("PopupBulkSale")))
		{
			TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupBulkSale"));
			if (widget.IsValid())
			{
				UGsUIPopupBulkSale* popup = Cast<UGsUIPopupBulkSale>(widget.Get());
				if (nullptr == popup)
				{
					return;
				}
				popup->InvalidateCurrency();
			}
		}
		return;
	}

	if (false == itemMgr->IsMatchCategorySubByItemTID(paramData->_itemTID, ItemCategorySub::DISCOUNT_COUPON))
		return;

	if (true == uiManager->IsActiveWidget(TEXT("PopupCouponList")))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("PopupCouponList"));
		if (widget.IsValid())
		{
			UGsUIPopupCouponList* popup = Cast<UGsUIPopupCouponList>(widget.Get());
			if (nullptr == popup)
			{
				return;
			}
			popup->RemoveCouponItem(paramData->_itemDBID);
		}
	}
}

// 윈도우 플랫폼은 인앱결제를 하지 않기때문에 
// 굳이 하이브 상품 정보를 받지 않은 상태여도 
// BM 상점을 오픈 시킨다
bool FGsBMShopHandler::IsValidHiveProduct() const
{
	if (_iapPurchaseBase.IsValid())
	{
#if PLATFORM_ANDROID || PLATFORM_IOS || PLATFORM_WINDOWS
		return _iapPurchaseBase->IsValidState();
#endif
	}
	return true;
}

bool FGsBMShopHandler::IsUseHiveProductPlatform() const
{
	if (_iapPurchaseBase.IsValid())
	{
		return (DevicePlatformType::EDITOR != _iapPurchaseBase->GetCurrenctPlatformType() && (GBMShop()->IsUseIAPSystem()));
	}
	return false;
}

void FGsBMShopHandler::HiveNeedExitCallback(FGsSharedParam& InParam)
{
	if (UGsHiveManager* hiveMgr = GHive())
	{
		hiveMgr->ExitGame();
	}
}

bool FGsBMShopHandler::IsReceivedHiveCallback(const FGsIAPPurchaseBase::CallbackResultType InType) const
{
	if (_iapPurchaseBase.IsValid())
	{
		return _iapPurchaseBase->GetHiveCallbackResultFalg(InType);
	}
	return false;
}

FString LexToString(const FGsBMShopSalesTimeOver& InParam)
{
	return FString::Printf(TEXT("FGsBMShopSalesTimeOver _productID: %u"), InParam._productID);
}

FString LexToString(const FGsBMShopOpenPopupBuy& InParam)
{
	return FString::Printf(TEXT("FGsBMShopOpenPopupBuy _productID: %u"), InParam._productID);
}

/*
FString LexToString(const FGsBMShopOpenPopupGroupDetail& InParam)
{
	return FString::Printf(TEXT("FGsBMShopOpenPopupGroupDetail _itemId: %u"), InParam._itemId);
}

FString LexToString(const FGsBMShopInvalidateProduct& InParam)
{
	return TEXT("FGsBMShopInvalidateProduct");
}

FString LexToString(const FGsBMShopInvalidateProductList& InParam)
{
	return TEXT("FGsBMShopInvalidateProductList");
}

FString LexToString(const FGsBMShopOpenBMShopHud& InParam)
{
	return TEXT("FGsBMShopOpenBMShopHud");
}

FString LexToString(const FGsBMShopPurchaseRestoreCheck& InParam)
{
	return TEXT("FGsBMShopPurchaseRestoreCheck");
}

FString LexToString(const FGsBMShopKickZpayUser& InParam)
{
	return TEXT("FGsBMShopKickZpayUser");
}

FString LexToString(const FGsBMShopInvalidateStorage& InParam)
{
	return TEXT("FGsBMShopInvalidateStorage");
}

FString LexToString(const FGsBMShopInvalidatePopupShopProduct& InParam)
{
	return TEXT("FGsBMShopInvalidatePopupShopProduct");
}

FString LexToString(const FGsBMShopSearchZpayUserNotify& InParam)
{
	return TEXT("FGsBMShopSearchZpayUserNotify");
}

FString LexToString(const FGsBMShopDisplayNothingToRestore& InParam)
{
	return TEXT("FGsBMShopDisplayNothingToRestore");
}

FString LexToString(const FGsBMShopHiveBlockUiClose& InParam)
{
	return TEXT("FGsBMShopHiveBlockUiClose");
}

FString LexToString(const FGsBMShopCloseBMShopByBlock& InParam)
{
	return TEXT("FGsBMShopCloseBMShopByBlock");
}

FString LexToString(const FGsBMShopUserCheckRestore& InParam)
{
	return TEXT("FGsBMShopUserCheckRestore");
}

FString LexToString(const FGsBMShopNothingToRestore& InParam)
{
	return TEXT("FGsBMShopNothingToRestore");
}

FString LexToString(const FGsBMShopHiveMarketConnection& InParam)
{
	return TEXT("FGsBMShopHiveMarketConnection");
}

FString LexToString(const FGsBMShopHiveProudctListInfo& InParam)
{
	return TEXT("FGsBMShopHiveProudctListInfo");
}

FString LexToString(const FGsBMShopHiveBlockUiOpen& InParam)
{
	return TEXT("FGsBMShopHiveBlockUiOpen");
}
*/

FString LexToString(const FGsBMShopHiveError& InParam)
{
	return FString::Printf(TEXT("FGsBMShopHiveError FGsBMShopHiveError(), _devicePlatformType: %d, _code: %d, _errorCode: %d, _hiveFailedCallbackType: %d, _desc: %s"), InParam._devicePlatformType, InParam._code, InParam._errorCode, InParam._hiveFailedCallbackType, *InParam._desc);
}

FString LexToString(const FGsBMShopHiveErrors& InParam)
{
	return FString::Printf(TEXT("FGsBMShopHiveErrors(), _devicePlatformType: %d, _code: %d, _errorCode: %d, _hiveFailedCallbackType: %d"), InParam._devicePlatformType, InParam._code, InParam._errorCode, InParam._hiveFailedCallbackType);
}

/*
FString LexToString(const FGsBMShopSendAnalyticsEvent& InParam)
{
	return TEXT("FGsBMShopSendAnalyticsEvent");
}

FString LexToString(const FGsBMShopHiveNeedExit& InParam)
{
	return TEXT("FGsBMShopHiveNeedExit");
}

FString LexToString(const FGsBMShopInvalidateServerMigrage& InParam)
{
	return TEXT("FGsBMShopInvalidateServerMigrage");
}

FString LexToString(const FGsBMShopPopupProductTimeOver& InParam)
{
	return TEXT("FGsBMShopPopupProductTimeOver");
}
*/

FString LexToString(const FGsBMShopCheckValidProduct& InParam)
{
	FString Str = FString::Printf(TEXT("FGsBMShopCheckValidProduct _isBulkSale: %d, _isUseDiscount: %d, _discountCurrencyAmount: %llu"), InParam._isBulkSale, InParam._isUseDiscount, InParam._discountCurrencyAmount);
	int32 i = 0;
	for (const BMProductIdAmountPair& ProductIdAmountPair : InParam._pairList)
	{
		Str.Appendf(TEXT("\n _pairList[%d] { mBMProductId: %u,  mAmount: %lld } "), i++, ProductIdAmountPair.mBMProductId, ProductIdAmountPair.mAmount);
	}
	return MoveTemp(Str);
}

FString LexToString(const FGsBMShopCheckValidCoupon& InParam)
{
	return FString::Printf(TEXT("FGsBMShopCheckValidCoupon _productID: %u"), InParam._productID);
}

FString LexToString(const FGsBMShopAcceptStorage& InParam)
{
	return FString::Printf(TEXT("FGsBMShopAcceptStorage _storageItemDBID: %llu"), InParam._storageItemDBID);
}

FString LexToString(const FGsBMShopChangeTab& InParam)
{
	return FString::Printf(TEXT("FGsBMShopChangeTab _tabType: %u"), InParam._tabType);
}

FString LexToString(const FGsBMShopOpenSpecialTab& InParam)
{
	return FString::Printf(TEXT("FGsBMShopOpenSpecialTab _tabType: %u"), InParam._tabType);
}

FString LexToString(const FGsBMShopOpenTab& InParam)
{
	return FString::Printf(TEXT("FGsBMShopOpenTab _tabID: %u"), InParam._tabID);
}

FString LexToString(const FGsBMShopChangeTabFromId& InParam)
{
	return FString::Printf(TEXT("FGsBMShopChangeTabFromId _tabID: %u"), InParam._tabID);
}

FString LexToString(const FGsBMShopChangeFocusingProduct& InParam)
{
	return FString::Printf(TEXT("FGsBMShopChangeFocusingProduct _mainTabID: %d, _subTabID: %d, _productID: %u"), InParam._mainTabID, InParam._subTabID, InParam._productID);
}

FString LexToString(const FGsBMShopPurchaseSuccess& InParam)
{
	FString Str = FString::Printf(TEXT("FGsBMShopPurchaseSuccess _isBulkSale: %d"), InParam._isBulkSale);
	int32 i = 0;
	for (const TPair<BMProductId, ItemAmount>& ProductIdAmountPair : InParam._productIdList)
	{
		Str.Appendf(TEXT("\n _productIdList[%d] { mBMProductId: %u,  mAmount: %lld } "), i++, ProductIdAmountPair.Key, ProductIdAmountPair.Value);
	}
	return MoveTemp(Str);
}

FString LexToString(const FGsBMShopSendTransactionFinishSingle& InParam)
{
	return FString::Printf(TEXT("FGsBMShopSendTransactionFinishSingle : %s"), *InParam._pid);
}

FString LexToString(const FGsBMShopOpenBMShopValidHive& InParam)
{
	return FString::Printf(TEXT("FGsBMShopOpenBMShopValidHive _valueFlag: %d"), InParam._valueFlag);
}

FString LexToString(const FGsBMShopCheckValidProductResult& InParam)
{
	FString Str = FString::Printf(TEXT("FGsBMShopCheckValidProductResult _identifier: %llu, _isBulkSale: %d, _couponItemDbid: %llu"), InParam._identifier, InParam._isBulkSale, InParam._couponItemDbid);
	int32 i = 0;
	for (const BMProductIdAmountPair& ProductIdAmountPair : InParam._amountPairList)
	{
		Str.Appendf(TEXT("\n _amountPairList[%d] { mBMProductId: %u,  mAmount: %lld } "), i++, ProductIdAmountPair.mBMProductId, ProductIdAmountPair.mBMProductId);
	}
	return MoveTemp(Str);
}

FString LexToString(const FGsBMShopOpenBulkSale& InParam)
{
	FString Str = TEXT("FGsBMShopOpenBulkSale");
	int32 i = 0;
	for (const BMProductIdAmountPair& ProductIdAmountPair : InParam._bulkSaleProductInfoList)
	{
		Str.Appendf(TEXT("\n _amountPairList[%d] { mBMProductId: %u,  mAmount: %lld } "), i++, ProductIdAmountPair.mBMProductId, ProductIdAmountPair.mBMProductId);
	}
	return MoveTemp(Str);
}

FString LexToString(const FGsBMShopOpenPopupShop& InParam)
{
	return FString::Printf(TEXT("FGsBMShopOpenPopupShop _productID: %llu"), InParam._productID);
}

FString LexToString(const FGsBMShopOpenBMShopPromotion& InParam)
{
	return FString::Printf(TEXT("FGsBMShopOpenBMShopPromotion _productID: %llu"), InParam._productID);
}

FString LexToString(const FGsBMShopOpenCouponList& InParam)
{
	return FString::Printf(TEXT("FGsBMShopOpenCouponList _productID: %llu"), InParam._productID);
}

FString LexToString(const FGsBMShopZpayUserAck& InParam)
{
	return FString::Printf(TEXT("FGsBMShopZpayUserAck _isInZpayMode: %d"), InParam._isInZpayMode);
}

FString LexToString(const FGsBMShopHiveMarketConnect& InParam)
{
	FString Str = FString::Printf(TEXT("FGsBMShopHiveMarketConnect _isSuccess: %d, _code: %d, _needExit: %d, _errorCode: %d"), InParam._isSuccess, InParam._code, InParam._needExit, InParam._errorCode);
	int32 i = 0;
	for (const hive::IAPV4Type& MarketId : InParam._marketIds)
	{
		Str.Appendf(TEXT("\n _marketIds[%d] = %d "), i++, MarketId);
	}
	return MoveTemp(Str);
}

FString LexToString(const FGsBMShopHiveGetProductInfo& InParam)
{
	FString Str = FString::Printf(TEXT("FGsBMShopHiveGetProductInfo _isSuccess: %d, _code: %d, _needExit: %d, _errorCode: %d"), InParam._isSuccess, InParam._code, InParam._needExit, InParam._errorCode);
	int32 i = 0;
	for (const hive::IAPV4Product& Ipa4Product : InParam._ipa4Products)
	{
		Str.Appendf(TEXT("\n _ipa4Products[%d] { productType: %s, marketPid: %s, currency: %s, displayPrice: %s, title: %s} "), i++, UTF8_TO_TCHAR(Ipa4Product.productType.c_str()), UTF8_TO_TCHAR(Ipa4Product.marketPid.c_str()), UTF8_TO_TCHAR(Ipa4Product.currency.c_str()), UTF8_TO_TCHAR(Ipa4Product.displayPrice.c_str()), UTF8_TO_TCHAR(Ipa4Product.title.c_str()));
	}
	return MoveTemp(Str);
}

FString LexToString(const FGsBMShopHiveGetSubscriptionProductInfo& InParam)
{
	FString Str = FString::Printf(TEXT("FGsBMShopHiveGetSubscriptionProductInfo _isSuccess: %d, _code: %d, _errorCode: %d"), InParam._isSuccess, InParam._code, InParam._errorCode);
	int32 i = 0;
	for (const hive::IAPV4Product& Ipa4Product : InParam._ipa4Products)
	{
		Str.Appendf(TEXT("\n _ipa4Products[%d] { productType: %s, marketPid: %s, currency: %s, displayPrice: %s, title: %s} "), i++, UTF8_TO_TCHAR(Ipa4Product.productType.c_str()), UTF8_TO_TCHAR(Ipa4Product.marketPid.c_str()), UTF8_TO_TCHAR(Ipa4Product.currency.c_str()), UTF8_TO_TCHAR(Ipa4Product.displayPrice.c_str()), UTF8_TO_TCHAR(Ipa4Product.title.c_str()));
	}
	return MoveTemp(Str);
}

FString LexToString(const FGsBMShopHiveRestore& InParam)
{
	FString Str = FString::Printf(TEXT("FGsBMShopHiveRestore _isSuccess: %d, _code: %d, _needExit: %d, _errorCode: %d"), InParam._isSuccess, InParam._code, InParam._needExit, InParam._errorCode);
	int32 i = 0;
	for (const TPair<FString, std::string>& MarketIdAndBypassInfo : InParam._marketIdAndBypassInfos)
	{
		Str.Appendf(TEXT("\n _marketIdAndBypassInfos[%d] { marketId: %s, BypassInfo: %s } "), i++, *MarketIdAndBypassInfo.Key, UTF8_TO_TCHAR(MarketIdAndBypassInfo.Value.c_str()));
	}
	return MoveTemp(Str);
}

FString LexToString(const FGsBMShopHiveRestoreSubscription& InParam)
{
	FString Str = FString::Printf(TEXT("FGsBMShopHiveRestoreSubscription _isSuccess: %d, _code: %d, _needExit: %d, _errorCode: %d"), InParam._isSuccess, InParam._code, InParam._needExit, InParam._errorCode);
	int32 i = 0;
	for (const TPair<FString, std::string>& MarketIdAndBypassInfo : InParam._marketIdAndBypassInfos)
	{
		Str.Appendf(TEXT("\n _marketIdAndBypassInfos[%d] { marketId: %s, BypassInfo: %s } "), i++, *MarketIdAndBypassInfo.Key, UTF8_TO_TCHAR(MarketIdAndBypassInfo.Value.c_str()));
	}
	return MoveTemp(Str);
}

FString LexToString(const FGsBMShopHivePurchase& InParam)
{
	return FString::Printf(TEXT("FGsBMShopHivePurchase _isSuccess: %d, _code: %d, _needExit: %d, _errorCode: %d, _currentPid: %s, _identifier: %llu, _bypassInfo: %s"), InParam._isSuccess, InParam._code, InParam._needExit, InParam._errorCode, *InParam._currentPid, InParam._identifier, UTF8_TO_TCHAR(InParam._bypassInfo.c_str()));
}

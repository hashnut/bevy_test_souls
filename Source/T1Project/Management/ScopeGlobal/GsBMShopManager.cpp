// Fill out your copyright notice in the Description page of Project Settings.
#include "Management/ScopeGlobal/GsBMShopManager.h"

#include "Net/GsNetSendServiceBMShop.h"
#include "Net/GsNetSendService.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsTutorialManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsDivergenceManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "Summon/GsSummonFunc.h"
#include "Guild/GsGuildHelper.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemIngredient.h"
#include "Sound/GsSoundPlayer.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Message/MessageParam/GsSystemMessageParam.h"

#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopProductData.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/GsBMShopStorageItemData.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"

#include "DataSchema/BMShop/GsSchemaBMShopProductData.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Hive/GsHiveEventMsg.h"

#include "T1Project.h"
#include "GsHiveManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"
#include "UI/UIContent/Window/BMShop/GsUIPopupBMShopProductBuy.h"
#include "Management/ScopeGlobal/GsUIManager.h"


void FGsBMShopManager::Initialize()
{
	//_isPopupZpay = false;
	//_isUseZpayFlag = false;
	//_isCanUseZpay = false;
	// _isStartInAppPurchase = false;
	_isSuccessIAPV4CheckOnce = false;
	_editorZpay = false;
	_usecheatFlag = false;

	_storageItemDataMap.Empty();

	_bmShopHandler = TUniquePtr<FGsBMShopHandler>(new FGsBMShopHandler());
	if (_bmShopHandler.IsValid())
	{
		_bmShopHandler->Init();
	}

	_ServerMigrateHandler = TUniquePtr<FGsServerMigrateHandler>(new FGsServerMigrateHandler());
	if (_ServerMigrateHandler.IsValid())
	{
		_ServerMigrateHandler->Init();
	}

	FGsMessageHolder* msgHandler = GMessage();
	if (msgHandler)
	{
		_gameobjectMsgHandler = msgHandler->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE,
			this, &FGsBMShopManager::OnLocalPlayerSpawnComplete);

		_tutorialMsgHandler = msgHandler->GetTutorial().AddRaw(MessageContentTutorial::COMPLETE_TUTORIAL_FOR_BM,
			this, &FGsBMShopManager::OnEmptyReservedTutorialQueueUI);

		_uiMsgHandlerList.Emplace(msgHandler->GetUI().AddRaw(MessageUI::UI_RESERVED_QUEUE_EMPTY_FOR_BM,
			this, &FGsBMShopManager::OnEmptyReservedQueueUI));

		//_uiMsgHandlerList.Emplace(msgHandler->GetUI().AddRaw(MessageUI::TITLE_CLICK_START,
			//this, &FGsBMShopManager::OnClickTitleStart));
	}

	_retryCount = 0;
	_reserveCouponDBID = 0;
	//_analyticsProductId = 0;
	_zPayUserDbid = 0;
	_zPayUserLevel = INVALID_LEVEL;
	_reserveHivePromoProductID = 0;

	_zPayModeUserServerName = "";
	_zPayModeUserName = "";
}

void FGsBMShopManager::Finalize()
{
	if (_bmShopHandler.IsValid())
	{
		_bmShopHandler->Close();
		_bmShopHandler.Reset();
	}

	if (_ServerMigrateHandler.IsValid())
	{
		_ServerMigrateHandler->Close();
		_ServerMigrateHandler.Reset();
	}

	FGsMessageHolder* msgHandler = GMessage();
	msgHandler->GetGameObject().Remove(_gameobjectMsgHandler);
	msgHandler->GetTutorial().Remove(_tutorialMsgHandler);

	for (TPair<MessageUI, FDelegateHandle>& elem : _uiMsgHandlerList)
	{
		msgHandler->GetUI().Remove(elem);
	}
	_uiMsgHandlerList.Empty();

	for (TPair<BMProductId, FGsBMShopProductData*>& iter : _bmShopProductDataMap)
	{
		delete iter.Value;
		iter.Value = nullptr;
	}

	for (auto& iter : _storageItemDataMap)
	{
		delete iter.Value;
		iter.Value = nullptr;
	}

	_stepUpProductDataMap.Empty();
	_bmShopProductDataMap.Empty();
	_bmShopChildProductIdMap.Empty();
	_storageItemDataMap.Empty();
	_mainTabDataList.Empty();
	_subTabDataList.Empty();
	_saleRemainTimeManageMap.Empty();
	_removeTimeManageList.Empty();
	_saleRemainTimeSpanMap.Empty();
	_popupShopProductDataList.Empty();
	_couponDataMap.Empty();
}

void FGsBMShopManager::ResetShopData()
{
	for (TPair<BMProductId, FGsBMShopProductData*>& iter : _bmShopProductDataMap)
	{
		FGsBMShopProductData* productData = iter.Value;
		if(nullptr == productData)
			continue;

		if (0 < productData->GetPurchaseQuantiy())
		{
			productData->SetPurchaseQuantiy(0);
			UpdateChildProductData(productData);
		}
	}

	for (TPair<uint64, FGsBMShopStorageItemData*>& iter : _storageItemDataMap)
	{
		delete iter.Value;
		iter.Value = nullptr;
	}

	_storageItemDataMap.Empty();
	_popupShopProductDataList.Empty();
	_bmShopSoundIdMap.Empty();

	_isSuccessIAPV4CheckOnce = false;

	_reserveCouponDBID = 0;
	_reserveBannerProductID = 0;
	_reserveHivePromoProductID = 0;
}

void FGsBMShopManager::InitializeAfterResourceDownload()
{
	GSLOG(Error , TEXT("FGsBMShopManager::InitializeAfterResourceDownload"));

	if (UGsDivergenceManager* mgr = GDivergence())
	{
		_isUseIAP = mgr->IsUseIAP();
	}

	if (const UGsGlobalConstant* globalData = GData()->GetGlobalData())
	{
		SetIsUseIAPV4SheetsLogFlag(globalData->_isActiveIAPV4SheetsLog);
		//_analyticsProductId = globalData->_bmShopAnalyticsProductId;
	}

	InitTableData();
	_reserveHivePromoProductID = 0;

	// 인앱결제 상품 관련 정보 요청
	CheckHiveProductData();
}

void FGsBMShopManager::OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam)
{
	_zPayUserDbid = 0;
	_zPayUserLevel = INVALID_LEVEL;
	_reserveCouponDBID = 0;

	if (false == _isSuccessIAPV4CheckOnce)
	{
		// 혹시나 인앱결제 관련 정보를 제대로 받지 못하였을 수도 있으므로 캐릭터가 
		// 정상 스폰 되었을때, 다시한번 체크하여 하이브한테 요청함
		CheckHiveProductData();
		if (IsValidHiveProduct())
		{
			// restore 체크
			MakeShared<FGsBMShopPurchaseRestoreCheck>()->Send();
		}
		_isSuccessIAPV4CheckOnce = true;
	}

	// 상품 정보 미리 받아놓음
	FGsNetSendServiceBMShop::SendBMShopProductList();
	// 보관함 정보 미리 받아놓음
	FGsNetSendServiceBMShop::SendBMShopStorageList();
}

void FGsBMShopManager::OnClickTitleStart(const struct IGsMessageParam* InParam)
{
}

void FGsBMShopManager::CheckHiveProductData(const bool InIsFromBMShopEnter) const
{
	GSLOG(Error, TEXT("FGsBMShopManager::CheckHiveProductData 11"));

	if (false == _isUseIAP)
		return;

	GSLOG(Error, TEXT("FGsBMShopManager::CheckHiveProductData 22"));

#if !WITH_EDITOR
	GSLOG(Error, TEXT("FGsBMShopManager::CheckHiveProductData 33"));

	MakeShared<FGsBMShopOpenBMShopValidHive>(InIsFromBMShopEnter)->Send();
#endif
}

// 하이브 쓰레드에서 테이블 로드하려다가 crash 발생하여, 해당 테이블 데이터 미리 맴버에 담아둠.
// 게임쓰레드가 아닌, 다른 쓰레드에서 테이블 로드하려고 하면 crash 발생
void FGsBMShopManager::LoadProductTableData()
{
	GSLOG(Warning, TEXT("[FGsBMShopManager::LoadProductTableData] in "));
	_bmShopProductTableData.Empty();

	const UGsTableBMShopProductData* table = Cast<UGsTableBMShopProductData>(FGsSchemaBMShopProductData::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Warning, TEXT("[FGsBMShopManager::LoadProductTableData] InitProductData is null "));
		return;
	}

	TArray<const FGsSchemaBMShopProductData*> allrow;
	if (false == table->GetAllRows(allrow))
	{
		GSLOG(Warning, TEXT("[FGsBMShopManager::LoadProductTableData] allrow is empty "));
		return;
	}

	for (const auto& iter : allrow)
	{
		if(nullptr == iter)
			continue;

		_bmShopProductTableData.Emplace(iter);
	}

	GSLOG(Warning, TEXT("[FGsBMShopManager::LoadProductTableData] _bmShopProductTableData.Num() : %d ") , _bmShopProductTableData.Num());
}

void FGsBMShopManager::Update(float inTick)
{
	for (TPair<BMProductId, FDateTime>& iter : _saleRemainTimeManageMap)
	{
		// TODO: 차후 국가별로 utc 시간 계산해서 출력해야함
		FDateTime serverUTC = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
		FDateTime dataTime = iter.Value;
		if (serverUTC > dataTime)
		{
			// finished
			// send message
			_removeTimeManageList.Emplace(iter.Key);

			MakeShared<FGsBMShopSalesTimeOver>(iter.Key)->Send();
			continue;
		}

		FTimespan spanTime = dataTime - serverUTC;
		if (true == _saleRemainTimeSpanMap.Contains(iter.Key))
		{
			_saleRemainTimeSpanMap[iter.Key] = spanTime;
		}
		else
		{
			_saleRemainTimeSpanMap.Emplace(iter.Key, spanTime);
		}

		//GSLOG(Warning, TEXT("[FGsBMShopManager::Update] - spanTime.GetDays : %d"), spanTime.GetDays());
		//GSLOG(Warning, TEXT("[FGsBMShopManager::Update] - spanTime.GetHours : %d"), spanTime.GetHours());
		//GSLOG(Warning, TEXT("[FGsBMShopManager::Update] - spanTime.GetMinutes : %d"), spanTime.GetMinutes());
		//GSLOG(Warning, TEXT("[FGsBMShopManager::Update] - spanTime.GetSeconds : %d"), spanTime.GetSeconds());
	}

	if (0 < _removeTimeManageList.Num())
	{
		for (int32 i = 0; i < _removeTimeManageList.Num(); ++i)
		{
			if (true == _saleRemainTimeManageMap.Contains(_removeTimeManageList[i]))
			{
				_saleRemainTimeManageMap.Remove(_removeTimeManageList[i]);
			}
			if (true == _saleRemainTimeSpanMap.Contains(_removeTimeManageList[i]))
			{
				_saleRemainTimeSpanMap.Remove(_removeTimeManageList[i]);
			}
		}
		_removeTimeManageList.Empty();
	}
	
	UpdatePopupShopInfo();

	BeginPromotionProc();
}

void FGsBMShopManager::InitTableData()
{
	// 상품 테이블 데이터 미리 캐싱처리
	LoadProductTableData();

	// 사용되는 상품 데이터 캐싱처리
	InitProductTableData();

	// 스탭업 상품 데이터
	InitStepUpProductData();

	// uitab 구성 테이블 세팅
	InitTabData();

	// child Product Data 세팅
	InitChildProductData();

	// CouponData 캐싱 처리
	InitCouponData();
}

void FGsBMShopManager::InitProductTableData()
{
	if (0 < _bmShopProductDataMap.Num())
	{
		for (TPair<BMProductId, FGsBMShopProductData*>& iter : _bmShopProductDataMap)
		{
			delete iter.Value;
			iter.Value = nullptr;
		}
		_bmShopProductDataMap.Empty();
	}

	for (const FGsSchemaBMShopProductData* iter : _bmShopProductTableData)
	{
		if (nullptr == iter)
			continue;

		if (false == iter->visibilityFlag)
			continue;

#if WITH_EDITOR
#else
		//if (CurrencyType::CASH == iter->purchaseCurrencyType)
			//continue;
#endif
		if (iter->isOOAP)
			continue;

		if (false == _bmShopProductDataMap.Contains(iter->id))
		{
			_bmShopProductDataMap.Emplace(iter->id, new FGsBMShopProductData(iter->id, nullptr));
			//GSLOG(Warning, TEXT("[FGsBMShopManager::InitProductData] display Order Default : %d"), _bmShopProductTableData[i]->displayOrder);
		}
		else
		{
			GSLOG(Warning, TEXT("[FGsBMShopManager::InitProductTableData] _bmShopProductDataMap already Exist productID"));
		}
	}

	// 정렬
	_bmShopProductDataMap.ValueSort([](FGsBMShopProductData& dataA, FGsBMShopProductData& dataB)
		{
			return dataA.GetDisplayOrder() > dataB.GetDisplayOrder();
		}
	);
}

void FGsBMShopManager::InitChildProductData()
{
	//GSLOG(Warning, TEXT("[FGsBMShopManager::InitHiveProductList] InitChildProductData "));

	TArray<BMProductId> removeChildProductList;
	for (TPair<BMProductId, FGsBMShopProductData*>& iter : _bmShopProductDataMap)
	{
		FGsBMShopProductData* data = iter.Value;
		if(nullptr == data)
			continue;

		if (data->IsExistParentProductID())
		{
			if (_bmShopProductDataMap.Contains(data->GetParentProductID()))
			{
				FGsBMShopProductData* parentData = _bmShopProductDataMap[data->GetParentProductID()];
				if (parentData)
				{
					data->UpdateChildProductData(parentData);
				}
				
				if (false == _bmShopChildProductIdMap.Contains(data->GetParentProductID()))
				{
					TArray<BMProductId> childList;
					childList.Emplace(data->GetProductID());
					_bmShopChildProductIdMap.Emplace(data->GetParentProductID(), childList);
				}
				else
				{
					TArray<BMProductId>& childList = _bmShopChildProductIdMap[data->GetParentProductID()];
					childList.Emplace(data->GetProductID());
				}
			}
			else
			{
				// parent id 는 물려있는데 해당 id 가 없으면 표시안하므로 제거처리
				removeChildProductList.Emplace(iter.Key);
			}
		}
	}

	if (0 < removeChildProductList.Num())
	{
		for (int32 iter : removeChildProductList)
		{
			if (_bmShopProductDataMap.Contains(iter))
			{
				_bmShopProductDataMap.Remove(iter);
			}
		}
	}
}

void FGsBMShopManager::InitStepUpProductData()
{
	_stepUpProductDataMap.Empty();
	for (TPair<BMProductId, FGsBMShopProductData*> iter : _bmShopProductDataMap)
	{
		if(nullptr == iter.Value)
			continue;

		if (true == iter.Value->IsStepUpProduct())
		{
			if (_stepUpProductDataMap.Contains(iter.Value->GetStepUpGroupID()))
			{
				_stepUpProductDataMap[iter.Value->GetStepUpGroupID()].Emplace(iter.Value);
			}
			else
			{
				TArray<FGsBMShopProductData*> list;
				list.Emplace(iter.Value);
				_stepUpProductDataMap.Emplace(iter.Value->GetStepUpGroupID(), list);
			}
		}
	}

	for (TPair<BMProductId, TArray<FGsBMShopProductData*>>& iter : _stepUpProductDataMap)
	{
		iter.Value.Sort([](FGsBMShopProductData& dataA, FGsBMShopProductData& dataB)
			{
				return dataA.GetStepUpLevel() < dataB.GetStepUpLevel();
			}
		);
	}
}

void FGsBMShopManager::InitHiveProductList(TArray<IAPV4Product>& InHiveInfoList, const DevicePlatformType InCurPlatformType)
{
	for (IAPV4Product& product : InHiveInfoList)
	{
		if (true == product.marketPid.empty())
		{
			GSLOG(Log, TEXT("[FGsBMShopManager::InitHiveProductList] true == product.marketPid.empty()"));
			continue;
		}

		FString convertStr = FString(UTF8_TO_TCHAR(product.marketPid.c_str()));
		GSLOG(Log, TEXT("[FGsBMShopManager::InitHiveProductList] Hive convertStr : %s ") ,*convertStr);

		for (int32 i = 0; i < _bmShopProductTableData.Num(); ++i)
		{
			if (nullptr == _bmShopProductTableData[i])
			{
				GSLOG(Log, TEXT("[FGsBMShopManager::InitHiveProductList] nullptr == _bmShopProductTableData[i]"));
				continue;
			}

			if (_bmShopProductTableData[i]->isOOAP)
				continue;

			FString tablePID = "";
			switch (InCurPlatformType)
			{
			case DevicePlatformType::GOOLE_ANDROID:
				tablePID = _bmShopProductTableData[i]->googleMarketPID;
				break;
			case DevicePlatformType::APPLE_IOS:
				tablePID = _bmShopProductTableData[i]->appStoreMarketPID;
				break;
			case DevicePlatformType::WINDOWS:
				tablePID = _bmShopProductTableData[i]->pcPlatformPID;
				break;
			}
			if (tablePID.IsEmpty())
			{
				continue;
			}

			/*
			if (false == tablePID.IsEmpty())
			{
				GSLOG(Warning, TEXT("[FGsBMShopManager::InitHiveProductList] tablePID : %s "), *tablePID);
			}
			*/

			if (convertStr == tablePID)
			{
				if (false == _bmShopProductDataMap.Contains(_bmShopProductTableData[i]->id))
				{
					_bmShopProductDataMap.Emplace(_bmShopProductTableData[i]->id, new FGsBMShopProductData(_bmShopProductTableData[i]->id, &product));
					//GSLOG(Warning, TEXT("[FGsBMShopManager::InitProductData] display Order Default : %d"), _bmShopProductTableData[i]->displayOrder);
				}
				else
				{
					FGsBMShopProductData* productData = _bmShopProductDataMap[_bmShopProductTableData[i]->id];
					if (nullptr != productData)
					{
						productData->UpdateHiveProductInfo(&product);
						productData->PrintHiveProductInfo();
					}
				}
			}
		}

		// 정렬
		_bmShopProductDataMap.ValueSort([](FGsBMShopProductData& dataA, FGsBMShopProductData& dataB)
			{
				return dataA.GetDisplayOrder() > dataB.GetDisplayOrder();
			}
		);
	}

	InitChildProductData();

	/*
	for (const auto& iter : _bmShopProductDataMap)
	{
		FGsBMShopProductData* data = iter.Value;
		if (nullptr == data)
			continue;		

		//GSLOG(Warning, TEXT("[FGsBMShopManager::InitHiveProductList] _bmShopProductDataMap data->GetHiveMarketPID() : %s "), *data->GetHiveMarketPID());
	}
	*/
}

void FGsBMShopManager::InitTabData()
{
	_mainTabDataList.Empty();
	_subTabDataList.Empty();

	const UGsTableBMShopTabInfo* table = Cast<UGsTableBMShopTabInfo>(FGsSchemaBMShopTabInfo::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Warning, TEXT("[FGsBMShopManager::InitTabData] InitTabData is null "));
		return;
	}

	TArray<const FGsSchemaBMShopTabInfo*> allrow;
	if (false == table->GetAllRows(allrow))
	{
		GSLOG(Warning, TEXT("[FGsBMShopManager::InitTabData] InitTabData is empty "));
		return;
	}

	for (int32 i = 0; i < allrow.Num(); ++i)
	{
		if (nullptr == allrow[i])
			continue;

		if (false == allrow[i]->visibilityFlag)
		{
			continue;
		}

		if (0 >= allrow[i]->parentTabid) // mainTabID
		{
			_mainTabDataList.Emplace(allrow[i]);
		}
		else
		{
			_subTabDataList.Emplace(allrow[i]);
		}
	}

	SortTab(_mainTabDataList);
	SortTab(_subTabDataList);
}

void FGsBMShopManager::InitCouponData()
{
	_couponDataMap.Empty();
	const UGsTableItemEffectDiscountCouponData* table = Cast<UGsTableItemEffectDiscountCouponData>(FGsSchemaItemEffectDiscountCouponData::GetStaticTable());
	if (nullptr == table)
	{
		GSLOG(Warning, TEXT("[FGsBMShopManager::InitCouponData] table is null "));
		return;
	}

	TArray<const FGsSchemaItemEffectDiscountCouponData*> allrow;
	if (false == table->GetAllRows(allrow))
	{
		GSLOG(Warning, TEXT("[FGsBMShopManager::InitCouponData] allrow is empty "));
		return;
	}

	for (const FGsSchemaItemEffectDiscountCouponData* iter : allrow)
	{
		if (nullptr == iter)
			continue;

		if (_couponDataMap.Contains(iter->conditionGroupId))
		{
			TMap<int32, const FGsSchemaItemEffectDiscountCouponData*>& mapData = _couponDataMap[iter->conditionGroupId];
			if (!mapData.Contains(iter->id))
			{
				mapData.Emplace(iter->id, iter);
			}
		}
		else
		{
			TMap<int32, const FGsSchemaItemEffectDiscountCouponData*> dataMap;
			dataMap.Emplace(iter->id, iter);
			_couponDataMap.Emplace(iter->conditionGroupId, dataMap);
		}
	}
}

const FGsSchemaBMShopTabInfo* FGsBMShopManager::FindSpecialTabInfo(const EGsBMShopSpecialTab InType) const
{
	if (EGsBMShopSpecialTab::None == InType)
		return nullptr;

	for (const FGsSchemaBMShopTabInfo* tabInfo : _subTabDataList)
	{
		if (nullptr == tabInfo)
			continue;

		if (tabInfo->specialTabType == InType)
		{
			return tabInfo;
		}
	}

	for (const FGsSchemaBMShopTabInfo* tabInfo : _mainTabDataList)
	{
		if (nullptr == tabInfo)
			continue;

		if (tabInfo->specialTabType == InType)
		{
			return tabInfo;
		}
	}

	return nullptr;
}

const FGsSchemaBMShopTabInfo* FGsBMShopManager::FindMainTabInfoByID(const int32 InMainTabID) const
{
	for (const FGsSchemaBMShopTabInfo* iter : _mainTabDataList)
	{
		if (nullptr == iter)
			continue;

		if (InMainTabID == iter->id)
			return iter;
	}

	return nullptr;
}

const FGsSchemaBMShopTabInfo* FGsBMShopManager::FindSubTabInfoByID(const int32 InSubTabID) const
{
	for (const FGsSchemaBMShopTabInfo* iter : _subTabDataList)
	{
		if (nullptr == iter)
			continue;

		if (InSubTabID == iter->id)
			return iter;
	}

	return nullptr;
}

void FGsBMShopManager::SetProductRemainTime(const BMProductId InProductId, FDateTime InTargetTime)
{
	TArray<BMProductId> childList;
	if (_bmShopChildProductIdMap.Contains(InProductId))
	{
		childList = _bmShopChildProductIdMap[InProductId];
		childList.Emplace(InProductId);
		for (BMProductId iter : childList)
		{
			if (false == _saleRemainTimeManageMap.Contains(iter))
			{
				_saleRemainTimeManageMap.Emplace(iter, InTargetTime);
			}
			else
			{
				_saleRemainTimeManageMap[iter] = InTargetTime;
				_removeTimeManageList.Remove(iter);
			}
		}
	}
	else
	{
		if (false == _saleRemainTimeManageMap.Contains(InProductId))
		{

			_saleRemainTimeManageMap.Emplace(InProductId, InTargetTime);
		}
		else
		{
			_saleRemainTimeManageMap[InProductId] = InTargetTime;
			_removeTimeManageList.Remove(InProductId);
		}
	}	
}

void FGsBMShopManager::FindProductListByTabInfo(const int32 InMainTabID, OUT TArray<FGsBMShopProductData*>& outDataList)
{
	for (const auto& iter : _bmShopProductDataMap)
	{
		if (nullptr == iter.Value)
			continue;

		if (InMainTabID == iter.Value->GetParentTabID())
		{
			outDataList.Emplace(iter.Value);
		}
	}
}

void FGsBMShopManager::FindMainTabData(bool IsUseZpayMode, OUT TArray<const FGsSchemaBMShopTabInfo*>& outDataList)
{
	outDataList.Empty();
	for (int32 i = 0; i < _mainTabDataList.Num(); ++i)
	{
		if (nullptr == _mainTabDataList[i])
			continue;

		if (_mainTabDataList[i]->displayTabType == EGsBMShopDisPlayType::ServerMigrate)
		{
			if (IsUseZpayMode || GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::BMSHOP_SERVER_MIGRATE, false) == false)
				continue;
		}

		if (IsUseZpayMode && _mainTabDataList[i]->isStorageTab)
		{
			continue;
		}

		outDataList.Emplace(_mainTabDataList[i]);
	}
}

void FGsBMShopManager::FindSubTabDataByMainID(const int32 InMainTabID, OUT TArray<const FGsSchemaBMShopTabInfo*>& outDataList)
{
	outDataList.Empty();
	for (int32 i = 0; i < _subTabDataList.Num(); ++i)
	{
		if (nullptr == _subTabDataList[i])
			continue;

		if (InMainTabID == _subTabDataList[i]->parentTabid)
		{
			outDataList.Emplace(_subTabDataList[i]);
		}
	}
}

void FGsBMShopManager::FindProductListByTabInfo(const int32 InMainTabID, const int32 InSubTabID, OUT TArray<FGsBMShopProductData*>& outDataList)
{
	for (const auto& iter : _bmShopProductDataMap)
	{
		if (nullptr == iter.Value)
			continue;

		if (InMainTabID == iter.Value->GetParentTabID() && InSubTabID == iter.Value->GetSubTabID())
		{
			outDataList.Emplace(iter.Value);
		}
	}
}

void FGsBMShopManager::FindStepUpProduct(TArray<BMProductId> InProductIdList, OUT TArray<FGsBMShopProductData*>& outDataList)
{
	TArray<FGsBMShopProductData*> dataList;
	for (const BMProductId iter : InProductIdList)
	{
		FGsBMShopProductData* data = FindProduct(iter);
		if(nullptr == data)
			continue;

		if(false == data->IsStepUpProduct())
			continue;

		dataList.Emplace(data);
	}

	dataList.Sort([](FGsBMShopProductData& dataA, FGsBMShopProductData& dataB)
		{
			return dataA.GetStepUpGroupID() < dataB.GetStepUpGroupID();
		}
	);

	int32 minGroupID = 0;
	for (FGsBMShopProductData* iter : dataList)
	{
		if (0 == minGroupID)
		{
			minGroupID = iter->GetStepUpGroupID();
		}

		if(minGroupID < iter->GetStepUpGroupID())
			continue;

		outDataList.Emplace(iter);
	}

	outDataList.Sort([](FGsBMShopProductData& dataA, FGsBMShopProductData& dataB)
		{
			return dataA.GetStepUpLevel() < dataB.GetStepUpLevel();
		}
	);
}

int32 FGsBMShopManager::GetMaxStepByGruopID(const int32 InStepUpGroupID) const
{	
	if (_stepUpProductDataMap.Contains(InStepUpGroupID))
	{
		return _stepUpProductDataMap[InStepUpGroupID].Num();
	}
	return 0;
}

FGsBMShopManager::StepUpState FGsBMShopManager::GetStepUpProductState(const BMProductId InProductID)
{
	FGsBMShopProductData* data = FindProduct(InProductID);
	if (nullptr == data)
		return StepUpState::State_IsLock;

	if (0 >= data->GetStepUpLevel())
		return StepUpState::State_None;

	if (_stepUpProductDataMap.Contains(data->GetStepUpGroupID()))
	{
		TArray<FGsBMShopProductData*> dataList = _stepUpProductDataMap[data->GetStepUpGroupID()];
		if (0 >= dataList.Num())
			return StepUpState::State_IsLock;

		if (data->IsCompleteSteupCondition() && dataList.IsValidIndex(data->GetStepUpLevel() + 1))
		{
			return StepUpState::State_Complete;
		}
		else
		{
			if (1 == data->GetStepUpLevel())
			{
				return StepUpState::State_IsOpen;
			}
			else
			{
				if (dataList.IsValidIndex(data->GetStepUpLevel() - 2))
				{
					FGsBMShopProductData* beforeData = dataList[data->GetStepUpLevel() - 2];
					if (nullptr == beforeData)
					{
						return StepUpState::State_IsLock;
					}

					return beforeData->IsCompleteSteupCondition() ? StepUpState::State_IsOpen : StepUpState::State_IsLock;
				}
			}
		}
	}

	return StepUpState::State_IsLock;
}

const BMProductId FGsBMShopManager::FindCurOpenStepUpProductId(const BMProductId InCurProductID)
{
	if (0 >= InCurProductID)
		return 0;

	FGsBMShopProductData* data = FindProduct(InCurProductID);
	if (nullptr == data)
		return 0;

	if (false == _stepUpProductDataMap.Contains(data->GetStepUpGroupID()))
		return 0;

	TArray<FGsBMShopProductData*> dataList = _stepUpProductDataMap[data->GetStepUpGroupID()];
	if (0 >= dataList.Num())
		return 0;

	BMProductId resultId = 0;
	for (const FGsBMShopProductData* iter : dataList)
	{
		if(nullptr == iter)
			continue;

		FGsBMShopManager::StepUpState state = GetStepUpProductState(iter->GetProductID());
		if (StepUpState::State_IsOpen == state)
		{
			resultId = iter->GetProductID();
		}
	}
	return resultId;
}

const FGsBMShopProductData* FGsBMShopManager::GetBeforeStepUpProductData(const BMProductId InCurProductID)
{
	FGsBMShopProductData* data = FindProduct(InCurProductID);
	if (nullptr == data)
		return nullptr;

	if (false == _stepUpProductDataMap.Contains(data->GetStepUpGroupID()))
		return nullptr;

	TArray<FGsBMShopProductData*> dataList = _stepUpProductDataMap[data->GetStepUpGroupID()];
	if (0 >= dataList.Num())
		return nullptr;

	if (dataList.IsValidIndex(data->GetStepUpLevel() - 2))
	{
		FGsBMShopProductData* beforeData = dataList[data->GetStepUpLevel() - 2];
		if (nullptr == beforeData)
		{
			return nullptr;
		}

		return beforeData;
	}
	
	return nullptr;
}

void FGsBMShopManager::FindProductIDListByTabInfo(const int32 InMainTabID, const int32 InSubTabID, OUT TArray<BMProductId>& outDataList)
{
	outDataList.Empty();
	for (const auto& iter : _bmShopProductDataMap)
	{
		if (nullptr == iter.Value)
			continue;

		if (iter.Value->IsPopupShopProduct())
		{
			FGsBMShopProductData* productData = FindProduct(iter.Value->GetProductID());
			if (nullptr != productData)
			{
				//if (!productData->IsSlodOut() && false == IsOpenedPopupShopProduct(iter.Value->GetProductID()))
				if (false == IsOpenedPopupShopProduct(iter.Value->GetProductID()))
					continue;
			}			
		}

		if (InMainTabID == iter.Value->GetParentTabID() && InSubTabID == iter.Value->GetSubTabID())
		{
			outDataList.Emplace(iter.Value->GetProductID());
		}
	}
}

void FGsBMShopManager::FindProductIDListByTabInfo(const int32 InMainTabID, OUT TArray<BMProductId>& outDataList)
{
	outDataList.Empty();
	for (const auto& iter : _bmShopProductDataMap)
	{
		if (nullptr == iter.Value)
			continue;

		if (iter.Value->IsPopupShopProduct())
		{
			FGsBMShopProductData* productData = FindProduct(iter.Value->GetProductID());
			if (nullptr != productData)
			{
				//if (!productData->IsSlodOut() && false == IsOpenedPopupShopProduct(iter.Value->GetProductID()))
				if (false == IsOpenedPopupShopProduct(iter.Value->GetProductID()))
					continue;
			}
		}

		if (InMainTabID == iter.Value->GetParentTabID())
		{
			outDataList.Emplace(iter.Value->GetProductID());
		}
	}
}

void FGsBMShopManager::UpdatePurchaseQuantity(const BMProductId InProductID, const uint16 InAmount)
{
	if (false == _bmShopProductDataMap.Contains(InProductID))
	{
		GSLOG(Warning, TEXT("[FGsBMShopManager::UpdatePurchaseQuantity] InProductID is Wrong : %d"), InProductID);
		return;
	}

	FGsBMShopProductData* productData = _bmShopProductDataMap[InProductID];
	productData->SetPurchaseQuantiy(InAmount);
	UpdateChildProductData(productData);
}

void FGsBMShopManager::FinishedUpdateList()
{
	MakeShared<FGsBMShopInvalidateProductList>()->Send();
	MakeShared<FGsBMShopInvalidatePopupShopProduct>()->Send();
}

void FGsBMShopManager::FinishedPurchaseItem(TArray<TPair<BMProductId, ItemAmount>> InProductIdList, bool InIsBulkSale, const bool InNotify)
{
	for (TPair<BMProductId, ItemAmount> iter : InProductIdList)
	{
		CheckPopupShopSoldOut(iter.Key);
	}

	// Send Message
	MakeShared<FGsBMShopPurchaseSuccess>(InProductIdList, InIsBulkSale)->Send();

	// Invalidate ProductList
	//FGsBMShopInvalidateList param(InProductIdList);
	MakeShared<FGsBMShopInvalidateProductList>()->Send();

	// Invalidate Hud BMShop Icon
	MakeShared<FGsBMShopInvalidatePopupShopProduct>()->Send();

	// sound
	if (InNotify)
	{
		PlayBMShopSound(BMShopSoundType::Sound_Purchase);
	}
}

void FGsBMShopManager::CheckPopupShopSoldOut(const BMProductId InProductID)
{
	FGsBMShopProductData* data = FindProduct(InProductID);
	if (nullptr == data)
		return;

	TArray<TPair<BMProductId, FDateTime>> removeList;
	if (data->IsSlodOut())
	{
		for (TPair<BMProductId, FDateTime> iter : _popupShopProductDataList)
		{
			if (iter.Key == InProductID)
			{
				removeList.Emplace(iter);
			}
		}
	}
	if (0 < removeList.Num())
	{
		for (TPair<BMProductId, FDateTime> iter : removeList)
		{
			_popupShopProductDataList.Remove(iter);
		}
	}
}

bool FGsBMShopManager::IsSameTypeProductBySubCategory(const BMProductId InProductID, const ItemCategorySub InSubCateogry)
{
	FGsBMShopProductData* data = FindProduct(InProductID);
	if (nullptr == data)
	{
		return false;
	}

	const FGsSchemaItemCommon* tableData = FGsItem::GetTableDataByID(data->GetItemTID());
	if (nullptr != tableData)
	{
		if (InSubCateogry == tableData->categorySub)
		{
			return true;
		}
	}

	return false;
}

bool FGsBMShopManager::IsSummonTypeProduct(const BMProductId InProductID)
{
	if (IsSameTypeProductBySubCategory(InProductID, ItemCategorySub::SUMMON_COSTUME) || IsSameTypeProductBySubCategory(InProductID, ItemCategorySub::SUMMON_FAIRY))
	{
		return true;
	}

	return false;
}

void FGsBMShopManager::UpdateProduct(TArray<struct BMShopProductInfo>& InServerDataList)
{
	//GSLOG(Warning, TEXT("FGsBMShopManager::UpdateProduct Call"));
	for (int32 i = 0; i < InServerDataList.Num(); ++i)
	{
		if (false == InServerDataList[i].mVisibilityFlag)
		{
			continue;
		}

		if (_bmShopProductDataMap.Contains(InServerDataList[i].mBMProductId))
		{
			FGsBMShopProductData* productData = _bmShopProductDataMap[InServerDataList[i].mBMProductId];
			if (nullptr == productData)
				continue;

			productData->SetPurchaseQuantiy(InServerDataList[i].mPurchasedQuantity);
			UpdateChildProductData(productData);
		}
		/*
		else
		{
			FGsBMShopProductData* newData = new FGsBMShopProductData(InServerDataList[i].mBMProductId);
			_bmShopProductDataMap.Emplace(InServerDataList[i].mBMProductId, newData);
		}
		*/
	}
}

void FGsBMShopManager::UpdateProduct(TArray<struct BMProductIdAmountPair>& InServerDataList)
{
	for (int32 i = 0; i < InServerDataList.Num(); ++i)
	{
		if (_bmShopProductDataMap.Contains(InServerDataList[i].mBMProductId))
		{
			FGsBMShopProductData* productData = _bmShopProductDataMap[InServerDataList[i].mBMProductId];
			if (nullptr == productData)
				continue;

			productData->SetPurchaseQuantiy(InServerDataList[i].mAmount);
			UpdateChildProductData(productData);
			
			//GSLOG(Error, TEXT("FGsBMShopManager::UpdateProduct - productData->GetName() : %s"), *productData->GetName().ToString());
			//GSLOG(Error, TEXT("FGsBMShopManager::UpdateProduct - InServerDataList[i].mAmount : %d"), InServerDataList[i].mAmount);
		}
		/*
		else
		{
			FGsBMShopProductData* newData = new FGsBMShopProductData(InServerDataList[i].mBMProductId);
			_bmShopProductDataMap.Emplace(InServerDataList[i].mBMProductId, newData);
		}
		*/
	}
}

// 상품 리스트 요청 할때 ack 으로 받음
void FGsBMShopManager::UpdateBlockList(TArray<FString>& InServerBlockList)
{
	GSLOG(Error, TEXT("[UpdateBlockList] - InServerBlockList.Num() : %d"), InServerBlockList.Num());

	if (0 >= InServerBlockList.Num())
		return;

	TArray<BMProductId> removeProductIdList;
	for (FString& blockPid : InServerBlockList)
	{
		GSLOG(Error, TEXT("[UpdateBlockList] - blockPid : %s"), *blockPid);

		for (TPair<BMProductId, FGsBMShopProductData*>& iter : _bmShopProductDataMap)
		{
			FGsBMShopProductData* productData = iter.Value;
			if (nullptr == productData)
				continue;

			if (false == productData->IsHiveProduct())
				continue;

			if (productData->GetConvertHiveMarketPID() == blockPid)
			{
				removeProductIdList.Emplace(productData->GetProductID());
			}
		}
	}

	if (0 >= removeProductIdList.Num())
		return;

	for (const BMProductId removeId : removeProductIdList)
	{
		if (_bmShopProductDataMap.Contains(removeId))
		{
			FGsBMShopProductData* productData = _bmShopProductDataMap[removeId];
			if (nullptr != productData)
			{
				productData->SetIsRemoveDisplay(true);
				UpdateChildProductData(productData);
			}
		}
		if (false == _removeTimeManageList.Contains(removeId))
		{
			_removeTimeManageList.Emplace(removeId);
		}
	}

	/*
	if (_stepUpProductDataMap.Contains(removeId))
	{
		_stepUpProductDataMap.Remove(removeId);
	}
	*/
}

// notify 식으로 실시간으로 받음
void FGsBMShopManager::UpdateBlockProductNotify(FString InPid, bool InIsRemove)
{
	GSLOG(Error, TEXT("[UpdateBlockProductNotify] - InPid : %s"), *InPid);
	GSLOG(Error, TEXT("[UpdateBlockProductNotify] - InIsRemove : %d"), (uint8)InIsRemove);

	TArray<FGsBMShopProductData*> UpdateProductList;
	for (TPair<BMProductId, FGsBMShopProductData*>& iter : _bmShopProductDataMap)
	{
		FGsBMShopProductData* productData = iter.Value;
		if (nullptr == productData)
			continue;

		if (false == productData->IsHiveProduct())
			continue;

		if (productData->GetConvertHiveMarketPID() == InPid)
		{
			UpdateProductList.Emplace(productData);
		}
	}

	if (0 >= UpdateProductList.Num())
		return;

	for (FGsBMShopProductData* data : UpdateProductList)
	{
		if (nullptr == data)
			continue;

		data->SetIsRemoveDisplay(InIsRemove);
		UpdateChildProductData(data);
	}

	MakeShared<FGsBMShopCloseBMShopByBlock>()->Send();
}

void FGsBMShopManager::UpdatePopupShopPacketInfo(TArray<TPair<BMProductId, time_t>> InServerDataList)
{
	_popupShopProductDataList.Empty();
	for (TPair<BMProductId, time_t>& iter : InServerDataList)
	{
		FDateTime time = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(iter.Value);
		_popupShopProductDataList.Emplace(TPair<BMProductId,FDateTime>(iter.Key,time));

		//GSLOG(Error, TEXT("[UpdatePopupShopPacketInfo] - time : %s"), *time.ToString());
	}

	// Test Code
	//_popupShopProductDataList.Emplace(TPair<int32, FDateTime>(106, FGsTimeSyncUtil::GetClientNowDateTime() + FTimespan(50 * ETimespan::TicksPerSecond)));

	// 정렬
	_popupShopProductDataList.Sort([](TPair<BMProductId, FDateTime> dataA, TPair<BMProductId, FDateTime> dataB)
		{
			return dataA.Value.GetTicks() < dataB.Value.GetTicks();
		}
	);
}

void FGsBMShopManager::UpdatePopupShopNotifyPacketInfo(TArray<TPair<BMProductId, time_t>> InServerDataList)
{
	_reserveBannerProductID = 0;
	for (TPair<BMProductId, time_t>& iter : InServerDataList)
	{
		if(IsOpenedPopupShopProduct(iter.Key))
			continue;

		FDateTime time = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(iter.Value);
		_popupShopProductDataList.Emplace(TPair<BMProductId, FDateTime>(iter.Key, time));

		// n개가 들어와도 제일 최종 배너만 출력함
		_reserveBannerProductID = iter.Key;
	}

	// 정렬
	_popupShopProductDataList.Sort([](TPair<BMProductId, FDateTime> dataA, TPair<BMProductId, FDateTime> dataB)
		{
			return dataA.Value.GetTicks() < dataB.Value.GetTicks();
		}
	);
}

void FGsBMShopManager::UpdatePopupShopInfo()
{
	for (int32 i = _popupShopProductDataList.Num(); i >= 0; --i)
	{
		if (_popupShopProductDataList.IsValidIndex(i))
		{
			FDateTime time = _popupShopProductDataList[i].Value;
			FTimespan span = time - FGsTimeSyncUtil::GetServerNowDateTimeUTC();
			if (0 > span.GetTicks()) // 시간 종료
			{
				BMProductId id = _popupShopProductDataList[i].Key;
				_popupShopProductDataList.RemoveAt(i);

				//FGsBMShopPopupProductTimeOver param(id);
				//GMessage()->GetBMShop().SendMessage(MessageContentStoreBM::POPUP_PRODUCT_TIME_OVER, &param);
				MakeShared<FGsBMShopInvalidateProductList>()->Send();
				MakeShared<FGsBMShopInvalidatePopupShopProduct>()->Send();
			}
			else
			{
				//GSLOG(Warning, TEXT("[UpdatePopupShopInfo] - productID : %d"), iter.Key);
				//GSLOG(Warning, TEXT("[UpdatePopupShopInfo] - GetMinutes : %d"), span.GetMinutes());
				//GSLOG(Warning, TEXT("[UpdatePopupShopInfo] - span.ToString : %s"), *span.ToString());

				// FText::FindText(TEXT("BMShopText"), TEXT("timeDay"), findText);
				//leftSaleTime
			}
		}
	}
}

bool FGsBMShopManager::IsOpenedPopupShopProduct(const BMProductId InProductID) const
{
	if (0 >= _popupShopProductDataList.Num())
		return false;

	for (TPair<BMProductId, FDateTime> iter : _popupShopProductDataList)
	{
		if (InProductID == iter.Key)
		{
			return true;
		}
	}

	return false;
}

bool FGsBMShopManager::GetPopupShopRemainTime(const BMProductId InProductID, OUT FDateTime& outDateTime) const
{
	if (0 >= _popupShopProductDataList.Num())
		return false;

	//TArray<TPair<int32, FDateTime>>
	for (const TPair<BMProductId, FDateTime>& iter : _popupShopProductDataList)
	{
		if (InProductID == iter.Key)
		{
			outDateTime = iter.Value;
			return true;
		}
	}

	return false;
}

bool FGsBMShopManager::GetPopupShopLessRemainTimeProduct(OUT FDateTime& outDateTime) const
{
	if (0 < _popupShopProductDataList.Num() && _popupShopProductDataList.IsValidIndex(0))
	{
		const TPair<BMProductId, FDateTime>& pairData = _popupShopProductDataList[0];
		outDateTime = pairData.Value;

		return true;
	}

	return false;
}

BMProductId FGsBMShopManager::GetPopupShopLessRemainTimeProductID() const
{
	if (0 < _popupShopProductDataList.Num() && _popupShopProductDataList.IsValidIndex(0))
	{
		const TPair<BMProductId, FDateTime>& pairData = _popupShopProductDataList[0];
		return pairData.Key;
	}
	return 0;
}

void FGsBMShopManager::ResetBulkSaleSettingData()
{
	_bulkSaleProductDataMap.Empty();
}

void FGsBMShopManager::UpdateBulkSaleProductList(TArray<BMProductBulkSale> InPairList)
{
	if (0 >= InPairList.Num())
		return;

	for (const BMProductBulkSale iter : InPairList)
	{
		if (_bulkSaleProductDataMap.Contains(iter.mBMProductIdAmountPair.mBMProductId))
		{
			TPair<bool, uint16>& pairData = _bulkSaleProductDataMap[iter.mBMProductIdAmountPair.mBMProductId];
			pairData.Key = iter.mIncluding;
			pairData.Value = iter.mBMProductIdAmountPair.mAmount;
		}
		else
		{
			_bulkSaleProductDataMap.Emplace(iter.mBMProductIdAmountPair.mBMProductId, TPair<bool, BMProductId>(iter.mIncluding, iter.mBMProductIdAmountPair.mAmount));
		}
	}
}

void FGsBMShopManager::UpdateStorageItems(TArray<struct BMDepotDetail>& InServerDataList)
{
	TArray<BMProductId> productIDList;
	for (int32 i = 0; i < InServerDataList.Num(); ++i)
	{
		if (false == _storageItemDataMap.Contains(InServerDataList[i].mBMItemDBId))
		{
			FGsBMShopStorageItemData* storageItemData = new FGsBMShopStorageItemData(
				InServerDataList[i].mBMItemDBId, InServerDataList[i].mBmProdutId, InServerDataList[i].mItemId, InServerDataList[i].mPurchasedDateTime);

			_storageItemDataMap.Emplace(InServerDataList[i].mBMItemDBId, storageItemData);
		}
		else
		{
			FGsBMShopStorageItemData* storageItemData = _storageItemDataMap[InServerDataList[i].mBMItemDBId];
			storageItemData->UpdatePacketData(InServerDataList[i]);
		}
		
		//productIDList.Emplace(InServerDataList[i].productID);
		
		//FDateTime temp = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(InServerDataList[i].mPurchasedDateTime);
		//GSLOG(Warning, TEXT("[FGsBMShopManager::UpdateStorageItems] Convert FDateTime Result : %s"), *temp.ToString());
	}

	MakeShared<FGsBMShopInvalidateStorage>()->Send();
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::BMSHOP, 0 < _storageItemDataMap.Num());

	//FGsBMShopOnTransactionFinish param(productIDList);
	//GMessage()->GetBMShop().SendMessage(MessageContentStoreBM::OnTransactionFinish, &param);
}

void FGsBMShopManager::RemoveStorageItems(TArray<BMItemDBId>& InServerDataList)
{
	for (int32 i = 0; i < InServerDataList.Num(); ++i)
	{
		if (true == _storageItemDataMap.Contains(InServerDataList[i]))
		{
			_storageItemDataMap.Remove(InServerDataList[i]);
		}
	}

	// ticker 출력
	// 보관함 아이템을 획득 하였습니다.
	MakeShared<FGsBMShopInvalidateStorage>()->Send();
	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::BMSHOP, 0 < _storageItemDataMap.Num());
}

FGsBMShopProductData* FGsBMShopManager::FindProduct(const BMProductId InProductID)
{
	if (_bmShopProductDataMap.Contains(InProductID))
	{
		return _bmShopProductDataMap[InProductID];
	}

	return nullptr;
}

void FGsBMShopManager::UpdateChildProductData(FGsBMShopProductData* InParentData)
{
	if (nullptr == InParentData)
		return;

	if (_bmShopChildProductIdMap.Contains(InParentData->GetProductID()))
	{
		TArray<BMProductId> childList = _bmShopChildProductIdMap[InParentData->GetProductID()];
		for (BMProductId iter : childList)
		{
			FGsBMShopProductData* data = FindProduct(iter);
			if (data)
			{
				data->UpdateChildProductData(InParentData);
			}
		}
	}
}

FGsBMShopProductData* FGsBMShopManager::FindProduct(FString InMarketPID)
{
	if (InMarketPID.IsEmpty())
		return nullptr;

	for (const TPair<BMProductId, FGsBMShopProductData*>& iter : _bmShopProductDataMap)
	{
		FGsBMShopProductData* data = iter.Value;
		if(nullptr == data)
			continue;

		if (InMarketPID == data->GetConvertHiveMarketPID())
		{
			if (data->IsExistParentProductID())
			{
				return FindProduct(data->GetParentProductID());
			}
			return data;
		}
	}
	return nullptr;
}

bool FGsBMShopManager::FindCouponDataByProductID(const BMProductId InProductID, OUT TMap<TWeakPtr<FGsItem>, const FGsSchemaItemEffectDiscountCouponData*>& outDataMap)
{
	//TMap<int32, TMap<int32, const FGsSchemaItemEffectDiscountCouponData*>> _couponDataMap;
	FGsBMShopProductData* data = FindProduct(InProductID);
	if (nullptr == data)
		return false;

	TArray<TWeakPtr<FGsItem>> findItems = GItem()->FindItem(ItemType::INGREDIENT, ItemCategoryMain::ETC, ItemCategorySub::DISCOUNT_COUPON);
	if (0 >= findItems.Num())
		return false;

	TArray<int32> groupIdList;
	data->GetCouponGroupID(groupIdList);
	if (0 >= groupIdList.Num())
		return false;

	// couponId , tableData
	TMap<int32, const FGsSchemaItemEffectDiscountCouponData*> findMapData;
	for (const int32 iter : groupIdList)
	{
		if (_couponDataMap.Contains(iter))
		{
			TMap<int32, const FGsSchemaItemEffectDiscountCouponData*> mapData = _couponDataMap[iter];
			for (TPair<int32, const FGsSchemaItemEffectDiscountCouponData*> mapIter : mapData)
			{
				if (false == findMapData.Contains(mapIter.Key))
				{
					findMapData.Emplace(mapIter.Key, mapIter.Value);
				}
			}
		}
	}

	if (0 >= findMapData.Num())
		return false;

	TArray<FGsItem> resultFindItemList;
	for (TWeakPtr<FGsItem> iter : findItems)
	{
		FGsItemIngredient* ingredientItem = iter.Pin()->GetCastItem<FGsItemIngredient>();
		if(nullptr == ingredientItem)
			continue;

		// effectData array 는 무조건 0번째만 참조해달라고 기획팀에서 요청
		TArray<int32> effectIdList;
		ingredientItem->FindEffectIdList(effectIdList);
		if(0 >= effectIdList.Num() || false == effectIdList.IsValidIndex(0))
			continue;

		int32 effectId = effectIdList[0];
		if (findMapData.Contains(effectId))
		{
			outDataMap.Emplace(iter, findMapData[effectId]);
		}
	}

	if (0 >= outDataMap.Num())
		return false;

	return true;
}

FGsBMShopStorageItemData* FGsBMShopManager::FindStorageItems(const uint64 InStorageItemDBID)
{
	if (_storageItemDataMap.Contains(InStorageItemDBID))
	{
		return _storageItemDataMap[InStorageItemDBID];
	}

	return nullptr;
}

bool FGsBMShopManager::FindProductGroupInfoByProductID(const BMProductId InProductID, OUT TArray<FGsRewardUnboxingData>& outRewardDataList)
{
	outRewardDataList.Empty();
	FGsBMShopProductData* findData = FindProduct(InProductID);
	if (nullptr == findData)
		return false;

	if (0 >= findData->GetRewardIdList().Num())
		return false;

	for (int32 id : findData->GetRewardIdList())
	{
		if (0 >= id)
			continue;

		FGsRewardUnboxingData bagItemData;
		if (FGsRewardHelper::UnboxingRewardData(id, bagItemData))
		{
			outRewardDataList.Emplace(bagItemData);
		}
	}

	return true;
}

void FGsBMShopManager::SortTab(OUT TArray<const FGsSchemaBMShopTabInfo*>& outTabInfo)
{
	if (outTabInfo.Num() > 1)
	{
		outTabInfo.Sort([](const FGsSchemaBMShopTabInfo& dataA, const FGsSchemaBMShopTabInfo& dataB)
			{
				// 정렬
				return dataA.displayOrder < dataB.displayOrder;
			}
		);
	}
}

const FGsSchemaBMShopTabInfo* FGsBMShopManager::GetTabTableDataByTabID(const int32 InTabID) const
{
	if (0 >= InTabID)
		return nullptr;

	const FGsSchemaBMShopTabInfo* findTabInfo = FGsBMShopManager::GetTabInfo(_mainTabDataList, InTabID);
	if (nullptr != findTabInfo)
	{
		return findTabInfo;
	}

	return FGsBMShopManager::GetTabInfo(_subTabDataList, InTabID);
}

const FGsSchemaBMShopTabInfo* FGsBMShopManager::GetTabInfo(const TArray<const FGsSchemaBMShopTabInfo*>& InTabInfoList, const int32 InTabID)
{
	for (int32 i = 0; i < InTabInfoList.Num(); ++i)
	{
		if (nullptr == InTabInfoList[i])
			continue;

		if (InTabID == InTabInfoList[i]->id)
		{
			return InTabInfoList[i];
		}
	}

	return nullptr;
}

FString FGsBMShopManager::GetCalcRemainTime(FTimespan InTimeSpan)
{
	FString result;
	result.Empty();
	for (uint8 i = 0; i < (uint8)RemainTimeUnitType::Type_Max; ++i)
	{
		FGsBMShopManager::GetRemainTimeUnit((RemainTimeUnitType)i, InTimeSpan, result);
	}

	return result;
}

bool FGsBMShopManager::GetDisplayRemainTime(BMProductId InProductID, FString& outString) const
{
	if (true == _saleRemainTimeSpanMap.Contains(InProductID))
	{
		const FTimespan& span = _saleRemainTimeSpanMap[InProductID];
		outString = FGsBMShopManager::GetCalcRemainTime(span);
		return true;
	}

	return false;
}

bool FGsBMShopManager::GetRemainTimeUnit(RemainTimeUnitType _type, FTimespan InTimeSpan, FString& outText)
{
	FText findText;
	switch (_type)
	{
	case RemainTimeUnitType::Day_Type:
	{
		if (0 >= InTimeSpan.GetDays())
		{
			return false;
		}

		FText::FindText(TEXT("BMShopText"), TEXT("timeDay"), findText);
		outText += (FString::FromInt(InTimeSpan.GetDays()) + findText.ToString());
	}
	return true;
	case RemainTimeUnitType::Hour_Type:
	{
		if (0 >= InTimeSpan.GetHours())
		{
			return false;
		}

		FText::FindText(TEXT("BMShopText"), TEXT("timeHour"), findText);
		outText += (FString::FromInt(InTimeSpan.GetHours()) + findText.ToString());
	}
	return true;
	case RemainTimeUnitType::Min_Type:
	{
		if (0 >= InTimeSpan.GetMinutes())
		{
			return false;
		}

		FText::FindText(TEXT("BMShopText"), TEXT("timeMinute"), findText);
		outText += (FString::FromInt(InTimeSpan.GetMinutes()) + findText.ToString());
	}
	return true;
	case RemainTimeUnitType::Sec_Type:
	{
		if (0 >= InTimeSpan.GetSeconds())
		{
			return false;
		}

		FText::FindText(TEXT("BMShopText"), TEXT("timeSecond"), findText);
		outText += (FString::FromInt(InTimeSpan.GetSeconds()) + findText.ToString());
	}
	return true;
	}

	return false;
}

void FGsBMShopManager::DisplayBuyPopup(const BMProductId InProductID)
{
	MakeShared<FGsBMShopOpenPopupBuy>(InProductID)->Send();
}

void FGsBMShopManager::GetAllStorageItemsDBID(OUT TArray<uint64>& outDBIDList) const
{
	outDBIDList.Empty();
	for (const auto& iter : _storageItemDataMap)
	{
		outDBIDList.Emplace(iter.Key);
	}
}

// targetDataTime 으로 부터 InAddedHour 시간 만큼 뺀 결과를 반환함
bool FGsBMShopManager::GetMinHourDateTime(FDateTime& InTargetDataTime, const uint8 InAddedHour, FDateTime& outDateTime)
{
	if (0 >= InAddedHour)
		return false;

	int32 year = 0;
	int32 month = 0;
	int32 day = 0;
	int32 hour = InTargetDataTime.GetHour() - InAddedHour;
	if (0 > hour)
	{
		int32 calcDay = InAddedHour / 24;
		int32 calc = InTargetDataTime.GetHour() - (InAddedHour - (calcDay * 24));
		if (0 > calc) // 24시간 기준 전날임
		{
			++calcDay;
			hour = 24 - abs(calc);
		}
		else
		{
			hour = calc;
		}

		int32 calcMonth = 0;
		day = InTargetDataTime.GetDay() - calcDay;
		if (0 >= day)
		{
			int32 curMonthDays = FDateTime::DaysInMonth(InTargetDataTime.GetYear(), InTargetDataTime.GetMonth());
			int32 lastMonthDays = 0 >= InTargetDataTime.GetMonth() - 1 ? FDateTime::DaysInMonth(InTargetDataTime.GetYear() - 1, 12) :
				FDateTime::DaysInMonth(InTargetDataTime.GetYear(), InTargetDataTime.GetMonth() - 1);

			day = lastMonthDays - abs(day);
			if (0 >= day)
			{
				return false;
			}

			++calcMonth;
		}

		month = InTargetDataTime.GetMonth() - calcMonth;
		if (0 >= month)
		{
			year = InTargetDataTime.GetYear() - 1;
			month = 12;
		}
	}

	if (false == FDateTime::Validate(0 == year ? InTargetDataTime.GetYear() : year, 0 == month ? InTargetDataTime.GetMonth() : month,
		0 == day ? InTargetDataTime.GetDay() : day, hour, InTargetDataTime.GetMinute(), InTargetDataTime.GetSecond(), InTargetDataTime.GetMillisecond()))
	{
		return false;
	}

	outDateTime = FDateTime(0 == year ? InTargetDataTime.GetYear() : year, 0 == month ? InTargetDataTime.GetMonth() : month,
		0 == day ? InTargetDataTime.GetDay() : day, hour, InTargetDataTime.GetMinute(), InTargetDataTime.GetSecond());

	return true;
}

bool FGsBMShopManager::CheckRedDot(bool bInUpdate /*= true*/)
{
	const bool IsReddot = 0 < _storageItemDataMap.Num();
	SetRedDot(IsReddot);

	return IsRedDot();
}

bool FGsBMShopManager::IsExistTabID(int32 InTabID) const
{
	if (0 >= InTabID)
		return false;

	for (const FGsSchemaBMShopTabInfo* iter : _mainTabDataList)
	{
		if (nullptr == iter)
			continue;

		if (InTabID == iter->id)
			return true;
	}
	for (const FGsSchemaBMShopTabInfo* iter : _subTabDataList)
	{
		if (nullptr == iter)
			continue;

		if (InTabID == iter->id)
			return true;
	}

	return false;
}

bool FGsBMShopManager::HasBulkSaleProduct(TArray<BMProductId> InProductIdList)
{
	for (BMProductId iter : InProductIdList)
	{
		FGsBMShopProductData* data = FindProduct(iter);
		if (data)
		{
			if (data->IsBulkSale())
			{
				return true;
			}
		}
	}

	return false;
}

EGsBMShopDisPlayType FGsBMShopManager::GetProductDisplayType(const int32 InMainTabID , const int32 InSubTabID)
{
	const FGsSchemaBMShopTabInfo* tabData = 0 >= InSubTabID ? FindMainTabInfoByID(InMainTabID) : FindSubTabInfoByID(InSubTabID);
	if (nullptr == tabData)
	{
		return EGsBMShopDisPlayType::None;
	}
	return tabData->displayTabType;
}

bool FGsBMShopManager::IsMainTabID(int32 InTabID) const
{
	if (0 >= InTabID)
		return false;

	for (const FGsSchemaBMShopTabInfo* iter : _mainTabDataList)
	{
		if (nullptr == iter)
			continue;

		if (InTabID == iter->id)
			return true;
	}

	return false;
}

bool FGsBMShopManager::hasChildTab(int32 InTabID) const
{
	if (0 >= InTabID)
		return false;

	if (false == IsMainTabID(InTabID))
		return false;

	for (const FGsSchemaBMShopTabInfo* iter : _subTabDataList)
	{
		if (nullptr == iter)
			continue;

		if (InTabID == iter->parentTabid)
			return true;
	}

	return false;
}

void FGsBMShopManager::GuildGiftNotice(const FString InUerName, const int32 InItemTID , const uint64 InBuyerDBID)
{
	const FGsSchemaItemCommon* tableData = FGsItem::GetTableDataByID(InItemTID);
	if (nullptr == tableData)
		return;	

	// TEXT: {0}님이 기사단선물 {1}|hpp(을,를) 구매하셨습니다
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("GuildGift_Ticker_Purchase"), textFormat))
	{
		FText textMsg = FText::Format(textFormat, FText::FromString(InUerName), tableData->name);		
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
	}

	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == local)
		return;	

	FGsGameObjectDataLocalPlayer* playerData = local->GetCastData<FGsGameObjectDataLocalPlayer>();
	if (nullptr == playerData)
		return;

	if (playerData->GetUserDBID() == InBuyerDBID)
	{
		// TEXT: {0}님이 기사단선물 {1}|hpp(을,를) 구매하셨습니다
		if (FText::FindText(TEXT("GuildText"), TEXT("GuildGift_GuildChat_Purchase"), textFormat))
		{
			FText textMsg = FText::Format(textFormat, FText::FromString(InUerName), tableData->name);
			FGsNetSendService::SendReqChat(ChatType::GUILD, textMsg.ToString());
		}
	}
}

uint16 FGsBMShopManager::GetBulkSaleAmount(const BMProductId InProductID) const
{
	if (_bulkSaleProductDataMap.Contains(InProductID))
	{
		const TPair<bool, BMProductId>& pair = _bulkSaleProductDataMap[InProductID];
		return pair.Value;
	}
	return 0;
}

bool FGsBMShopManager::GetBulkSaleState(const BMProductId InProductID) const
{
	if (_bulkSaleProductDataMap.Contains(InProductID))
	{
		const TPair<bool, BMProductId>& pair = _bulkSaleProductDataMap[InProductID];
		return pair.Key;
	}
	return false;
}

void FGsBMShopManager::CheckFairySummonWarnPopup()
{
	FGsSummonFunc::CheckFairySummonWaitCount(true);

	//https://jira.com2us.com/jira/browse/C2URWQ-5176
	// stack 갯수 최대 인지 체크
	FGsSummonFunc::CheckFairySummonStackCount(true);
}

void FGsBMShopManager::CheckCostumeSummonWarnPopup()
{
	FGsSummonFunc::CheckCostumeSummonWaitCount(true);

	//https://jira.com2us.com/jira/browse/C2URWQ-5176
	// stack 갯수 최대 인지 체크
	FGsSummonFunc::CheckCostumeSummonStackCount(true);
}

void FGsBMShopManager::PrintHiveProductInfo() const
{
	for (const TPair<BMProductId, FGsBMShopProductData*>& iter : _bmShopProductDataMap)
	{
		const FGsBMShopProductData* data = iter.Value;
		if(nullptr == data)
			continue;

		if(false == data->IsHiveProduct())
			continue;

		data->PrintHiveProductInfo();
	}
}

int32 FGsBMShopManager::GetDiscountPrice(FGsBMShopProductData* InProductData, FGsItem* InCouponItemData)
{
	if (nullptr == InProductData || nullptr == InCouponItemData)
		return -1;

	if (false == InCouponItemData->IsIngredient() || ItemCategorySub::DISCOUNT_COUPON != InCouponItemData->GetSubCategory())
		return -1;

	FGsItemIngredient* ingredientItem = InCouponItemData->GetCastItem<FGsItemIngredient>();
	if (nullptr != ingredientItem)
	{
		const FGsSchemaItemEffectDiscountCouponData* tableData = ingredientItem->GetFirstEffectDetailData<FGsSchemaItemEffectDiscountCouponData>();
		if (nullptr == tableData)
			return -1;

		int32 price = InProductData->GetPurchasePrice();
		if (ItemCouponDiscountType::COEFFICIENT == tableData->type)
		{
			int32 per = tableData->coefficientValue / 100;
			if (100 <= per)
			{
				return 0;
			}

			/*
			let input = 10.123456789
			let output = floor(input * 1000) / 1000
			// output == 10.123
			*/

			int32 calcPrice = price - ((price * per) / 100);
			return 0 < calcPrice ? calcPrice : 0;
		}
		else if(ItemCouponDiscountType::CONSTANT == tableData->type)
		{
			int32 calcPrice = price - tableData->constantValue;
			return 0 < calcPrice ? calcPrice : 0;
		}
	}

	return -1;
}

void FGsBMShopManager::SendPopupShopProduct()
{
	FGsNetSendServiceBMShop::SendPopupShopOpen();
}

void FGsBMShopManager::SetHivePromotionCallbackData(const BMProductId InProductID)
{
	_reserveHivePromoProductID = InProductID;
}

void FGsBMShopManager::BeginPromotionProc()
{	
	if (0 >= _reserveHivePromoProductID)
		return;

	MakeShared<FGsBMShopOpenBMShopPromotion>(_reserveHivePromoProductID)->Send();

	_reserveHivePromoProductID = 0;
}

// 출력한 팝업상점이 있으면 연출 및 튜토리얼 상황 체크필요
void FGsBMShopManager::OnEmptyReservedQueueUI(const struct IGsMessageParam* InParam)
{
	if (0 < _reserveBannerProductID)
	{
		if (GSTutorial() && GSTutorial()->IsActiveTutorial())
		{
			// 튜토리얼 끝날때까지 대기
		}
		else
		{
#if PLATFORM_IOS || PLATFORM_ANDROID 
			MakeShared<FGsBMShopOpenPopupShop>(_reserveBannerProductID)->Send();
#endif
			_reserveBannerProductID = 0;
		}
	}
}

void FGsBMShopManager::OnEmptyReservedTutorialQueueUI(const struct IGsMessageParam* InParam)
{
	if (0 < _reserveBannerProductID)
	{
#if PLATFORM_IOS || PLATFORM_ANDROID 
		MakeShared<FGsBMShopOpenPopupShop>(_reserveBannerProductID)->Send();
#endif
		_reserveBannerProductID = 0;
	}
}

bool FGsBMShopManager::IsExpiredCouponItem()
{
	if (0 >= _reserveCouponDBID)
		return false;

	TWeakPtr<FGsItem> findItem = GItem()->FindItem(_reserveCouponDBID);
	if (findItem.IsValid())
	{
		return findItem.Pin()->IsExpiredTime();
	}
	return false;
}

void FGsBMShopManager::OnIAPV4SheetsLog(FString InLogDescText) const
{
	if (_isActiveIAPV4Log && _isUseIAP)
	{
		FGsSmokeSignal(InLogDescText).Send();
	}
}

void FGsBMShopManager::OnReconectionEndBmShop()
{
	if (_bmShopHandler.IsValid())
	{
		_bmShopHandler->OnReconnection();
	}

	if (_ServerMigrateHandler.IsValid())
	{
		_ServerMigrateHandler->OnReconnection();
	}
}

bool FGsBMShopManager::IsValidHiveProduct() const
{
	if (_bmShopHandler.IsValid())
	{
		return _bmShopHandler->IsValidHiveProduct();
	}
	return false;
}

bool FGsBMShopManager::IsUseHiveProductPlatform() const
{
	if (_bmShopHandler.IsValid())
	{
		return _bmShopHandler->IsUseHiveProductPlatform();
	}
	return false;
}

void FGsBMShopManager::SendPurchaseEventToHive(const TArray<BMProductId>& InProductIdList)
{
	TArray<TPair<BMProductId, CurrencyType>> cashProductIdList;
	for (const BMProductId& productId : InProductIdList)
	{
		if (FGsBMShopProductData* productData = FindProduct(productId))
		{
			// 예외처리 필요 : CurrencyType 이 Cash 인데 HiveProductInfo 가 nullptr 이면 안된다.
			// 기획 쪽에서 현금결제 상품이 아닌데, 타입을 Cash 로 잘못 넣을 수 있기 때문.

			// 현금 결제 상품의 경우, CurrencyType 을 Cash 로 넣어 준다
			if (productData->IsHiveProduct())
			{
				cashProductIdList.Emplace(TPair<BMProductId, CurrencyType>(productId, CurrencyType::CASH));
			}
			// 인게임 재화의 경우, 각자 맞는 타입을 넣어 준다
			else
			{
				cashProductIdList.Emplace(TPair<BMProductId, CurrencyType>(productId, productData->GetPurchaseCurrencyType()));
			}
		}
	}

	FGsHiveEventPurchaseMsg purchaseMsg(EGsHiveActionType::PURCHASE, MoveTemp(cashProductIdList));
	GMessage()->GetHiveEvent().SendMessage(MessageContentHiveEvent::SEND_PURCHASE_EVENT, &purchaseMsg);
}

void FGsBMShopManager::SendRestoreEventToHive(const FString&& InMarketPID)
{
	TArray<TPair<BMProductId, CurrencyType>> cashProductIdList;

	if (FGsBMShopProductData* productData = FindProduct(InMarketPID))
	{
		// 예외처리 필요 : CurrencyType 이 Cash 인데 HiveProductInfo 가 nullptr 이면 안된다.
		// 기획 쪽에서 현금결제 상품이 아닌데, 타입을 Cash 로 잘못 넣을 수 있기 때문.

		// 현금 결제 상품의 경우, CurrencyType 을 Cash 로 넣어 준다
		if (productData->IsHiveProduct())
		{
			cashProductIdList.Emplace(TPair<BMProductId, CurrencyType>(productData->GetProductID(), CurrencyType::CASH));
		}
		// 인게임 재화의 경우, 각자 맞는 타입을 넣어 준다
		else
		{
			cashProductIdList.Emplace(TPair<BMProductId, CurrencyType>(productData->GetProductID(), productData->GetPurchaseCurrencyType()));
		}
	}

	FGsHiveEventPurchaseMsg purchaseMsg(EGsHiveActionType::PURCHASE, MoveTemp(cashProductIdList));
	GMessage()->GetHiveEvent().SendMessage(MessageContentHiveEvent::SEND_PURCHASE_EVENT, &purchaseMsg);
}

bool FGsBMShopManager::IsAllowBuyCashItem() const
{
#if PLATFORM_ANDROID || PLATFORM_IOS || PLATFORM_WINDOWS
	return _isUseIAP;
#elif WITH_EDITOR
	return true;
#endif
	return false;
}

void FGsBMShopManager::AddBMShopHandlerItemDetailMsg()
{
	if (_bmShopHandler.IsValid())
	{
		_bmShopHandler->AddItemDetailMsg();
	}
}

void FGsBMShopManager::RemoveBMShopHandlerItemDetailMsg()
{
	if (_bmShopHandler.IsValid())
	{
		_bmShopHandler->RemoveItemDetailMsg();
	}
}

void FGsBMShopManager::PlayBMShopSound(const BMShopSoundType InType)
{
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	const uint64 id = soundPlayer->PlaySound2D(FGsBMShopManager::GetSoundResName(InType));
	if(_bmShopSoundIdMap.Contains(InType))
	{
		_bmShopSoundIdMap[InType] = id;
	}
	else
	{
		_bmShopSoundIdMap.Emplace(InType, id);
	}
}

FString FGsBMShopManager::GetSoundResName(const BMShopSoundType InType)
{
	FString resultStr = "";
	switch (InType)
	{
	case BMShopSoundType::Sound_Purchase:
		resultStr = "UI_RewardOne";
		break;
	case BMShopSoundType::Sound_ReceiveStorageItem:
		break;
	}

	return resultStr;
}

void FGsBMShopManager::DeletePurchaseSequentially()
{
	GSLOG(Log, TEXT("FGsBMShopManager::DeletePurchaseSequentially()"));

	if (_purchaseSequentially.IsValid())
	{
		_purchaseSequentially.Reset();
	}
}

void FGsBMShopManager::StartPurchase(TSharedRef<FGsBMShopCheckValidProduct> InBMShopCheckValidProduct)
{
	GSLOG(Log, TEXT("FGsBMShopManager::StartPurchase()"));

	DeletePurchaseSequentially();

	if (0 == InBMShopCheckValidProduct->_pairList.Num())
	{
		GSLOG(Error, TEXT("0 == InBMShopCheckValidProduct->_pairList.Num()"));
		return;
	}

	// 결제 횟수는 기본값 1이고, 특수한 경우에 한하여 자동순차결제 기능을 사용함.
	ItemAmount PurchaseSequencialCount = 1;

	// 현금 결제 상품이고, 구매 갯수가 1을 초과하는 경우, 자동순차결제봇 사용하는 것으로 간주함.
	const ItemAmount PurchaseCount = InBMShopCheckValidProduct->_pairList[0].mAmount;
	if (1 == InBMShopCheckValidProduct->_pairList.Num() && 1 < PurchaseCount)
	{
		const BMProductId CurrentBMProductId = InBMShopCheckValidProduct->_pairList[0].mBMProductId;
		const FGsBMShopProductData* BMShopProductData = FindProduct(CurrentBMProductId);
		if (nullptr == BMShopProductData)
		{
			GSLOG(Error, TEXT("nullptr == BMShopProductData"));
			return;
		}

		// 에디터에 한하여 현금결제 상품 구매 테스트 위한 치트 코드 추가
//#if WITH_EDITOR
//		if (_usecheatFlag && BMShopProductData->IsCashProductItem())
//		{
//			if (nullptr != BMShopProductData->GetTableData())
//			{
//			}
//			return;
//		}
//#endif

		if (BMShopProductData->IsCashProductItem()) // 현금성 아이템
		{
			PurchaseSequencialCount = PurchaseCount;
		}
	}

	_purchaseSequentially = MakeUnique<FGsPurchaseSequentially>(InBMShopCheckValidProduct, PurchaseSequencialCount);
	
	RunNextPurchaseSequentially();
}

void FGsBMShopManager::CountUpPurchaseSequentially()
{
	GSLOG(Log, TEXT("FGsBMShopManager::CountUpPurchaseSequentially()"));

	if (_purchaseSequentially.IsValid())
	{
		_purchaseSequentially->CountUp();
	}
}

TOptional<ItemAmount> FGsBMShopManager::GetRemainCountPurchaseSequentially() const
{
	GSLOG(Log, TEXT("FGsBMShopManager::GetRemainCountPurchaseSequentially()"));

	TOptional<ItemAmount> RemainCount;

	if (_purchaseSequentially.IsValid())
	{
		RemainCount.Emplace(_purchaseSequentially->RemainCount());
	}

	return MoveTemp(RemainCount);
}

bool FGsBMShopManager::RunNextPurchaseSequentially()
{
	bool MessageSent = false;

	GSLOG(Log, TEXT("FGsBMShopManager::RunNextPurchaseSequentially() 0"));
	if (_purchaseSequentially.IsValid())
	{
		GSLOG(Log, TEXT("FGsBMShopManager::RunNextPurchaseSequentially() 1"));
		if (!_purchaseSequentially->IsCompleted())
		{
			// 자동순차구매가 진행중이므로, 다음 자동순차구매를 실행
			if (_purchaseSequentially->MakeMessage()->Send())
			{
				GSLOG(Log, TEXT("FGsBMShopManager::RunNextPurchaseSequentially() 2"));
				MessageSent = true;
			}
		}
	}

	GSLOG(Log, TEXT("FGsBMShopManager::RunNextPurchaseSequentially() 3"));
	if (!MessageSent)
	{
		GSLOG(Log, TEXT("FGsBMShopManager::RunNextPurchaseSequentially() 4"));

		// 메세지가 발송되지 않은 경우는, 정상 완료되었거나 에러가 발생한 경우로서, 이 후 불필요한 작업이 진행되지 않도록 삭제함
		DeletePurchaseSequentially();
	}

	return MessageSent;
}

// cash 상품인데 hive 상품 정보를 못받은 상품은 상품 출력 리스트에서 제외시킴
void FGsBMShopManager::ValidCheckCashProduct(OUT TArray<BMProductId>& InProductIdList)
{
	if (0 >= InProductIdList.Num())
		return;

	TArray<BMProductId> removeList;
	for (const BMProductId iter : InProductIdList)
	{
		FGsBMShopProductData* findData = FindProduct(iter);
		if (nullptr == findData)
			continue;

		if (findData->IsCashProductItem())
		{
			if (false == findData->IsHiveProduct())
			{
				removeList.Emplace(findData->GetProductID());
			}
			else if (findData->IsRemoveDisplay())
			{
				removeList.Emplace(findData->GetProductID());
			}
			else if (findData->IsOOAP())
			{
				removeList.Emplace(findData->GetProductID());
			}
		}
	}

	if (0 < removeList.Num())
	{
		for (BMProductId& iter : removeList)
		{
			InProductIdList.Remove(iter);
		}	
	}
}

bool FGsBMShopManager::IsReceivedHiveCallback(const FGsIAPPurchaseBase::CallbackResultType InType)
{
	if (_bmShopHandler.IsValid())
	{
		return _bmShopHandler->IsReceivedHiveCallback(InType);
	}
	return false;
}

bool FGsBMShopManager::OnClickUserPgComplete(const bool InForce /*= false*/)
{
	GSLOG(Log, TEXT("FGsBMShopManager::OnClickUserPgComplete()"));

	if (!_waitForReqPgUnconsumedPurchase && !InForce)
	{
		return false;
	}

	UGsHiveManager* HiveManager = GHive();
	if (nullptr == HiveManager)
	{
		return false;
	}

	if (FGsREQ_HIVE_PG_UNCONSUMED_PURCHASE(HiveManager->GetConfigurationGetAppId(), InForce).Send())
	{
		_waitForReqPgUnconsumedPurchase = false;
		return true;
	}

	return false;
}

void FGsBMShopManager::SetWaitForReqPgUnconsumedPurchase(const bool InWait)
{
	_waitForReqPgUnconsumedPurchase = InWait;
}

#if WITH_EDITOR
void FGsBMShopManager::EditorClearHiveProductInfos()
{
	if (!_bmShopHandler.IsValid())
	{
		GSLOG(Error, TEXT("!_bmShopHandler.IsValid()"));
		return;
	}

	FGsIAPPurchaseBase* IAPPurchase = _bmShopHandler->_iapPurchaseBase.Get();
	if (nullptr == IAPPurchase)
	{
		GSLOG(Error, TEXT("nullptr == IAPPurchase"));
		return;
	}

	IAPPurchase->EditorClearHiveProductInfos();
}
#endif

FGsServerMigrateHandler* FGsBMShopManager::GetServerMigrateHandler() const
{
	return (_ServerMigrateHandler.IsValid()) ? _ServerMigrateHandler.Get() : nullptr;
}
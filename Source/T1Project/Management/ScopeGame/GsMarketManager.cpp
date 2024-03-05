// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Market/GsSchemaMarketCategoryData.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Net/GsNetSendServiceMarket.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Market/GsMarketHelper.h"
#include "Market/GsMarketControllerBase.h"
#include "Market/GsMarketControllerNormal.h"
#include "Market/GsMarketControllerWorld.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedInclude/SharedDefine.h"


void FGsMarketManager::Initialize()
{
	_controllerNormal = new FGsMarketControllerNormal();
	_controllerWorld = new FGsMarketControllerWorld();
	_controllerList.Emplace(_controllerNormal);
	_controllerList.Emplace(_controllerWorld);

	_controllerNormal->Initialize();
	_controllerWorld->Initialize();

	TArray<const FGsSchemaItemCommon*> _rowList;
	if (const UGsTable* table = FGsSchemaItemCommon::GetStaticTable())
	{
		table->GetAllRows(_rowList);

		for (const FGsSchemaItemCommon* row : _rowList)
		{
			_controllerNormal->InitializeItemGroup(row);
			_controllerWorld->InitializeItemGroup(row);
		}
	}

	_serchItemInfo.Key = INVALID_ITEM_ID;
	_serchItemInfo.Value = false;
}

void FGsMarketManager::Finalize()
{
	_controllerList.Empty();

	if (_controllerNormal)
	{
		_controllerNormal->Finalize();
		delete _controllerNormal;
	}
	_controllerNormal = nullptr;

	if (_controllerWorld)
	{
		_controllerWorld->Finalize();
		delete _controllerWorld;
	}
	_controllerWorld = nullptr;	

	_purchaseResultList.Empty();
	 
	_collectionDataMap.Empty();
	_poolCollectionData.ReleaseAll();

	_serchItemInfo.Key = INVALID_ITEM_ID;
	_serchItemInfo.Value = false;
	_strSearch.Empty();
	_filterData.Clear();
}

bool FGsMarketManager::CheckRedDot(bool bInUpdate)
{
	if (bInUpdate)
	{
		if (FGsGameDataManager* gameDataMgr = GGameData())
		{
			int32 settlementCount = gameDataMgr->GetMarketSettlementCount(false);
			int32 wdSettlementCount = gameDataMgr->GetMarketSettlementCount(true);

			SetRedDot((0 < (settlementCount + wdSettlementCount))? true : false);
		}
	}

	return IsRedDot();
}

bool FGsMarketManager::CheckRedDoyByServer(bool bIsWorldMarket)
{
	if (FGsGameDataManager* gameDataMgr = GGameData())
	{
		int32 count = gameDataMgr->GetMarketSettlementCount(bIsWorldMarket);
		return (0 < count) ? true : false;
	}

	return false;
}

void FGsMarketManager::SetPurchaseResult(bool bIsWorldMarket, MarketTransactionId InTransactionId, int32 InResult)
{
	for (FGsMarketPurchaseResult& iter : _purchaseResultList)
	{
		if (bIsWorldMarket == iter._bIsWorldMarket &&
			InTransactionId == iter._transactionId)
		{
			iter._result = InResult;
			return;
		}
	}

	_purchaseResultList.Emplace(FGsMarketPurchaseResult(bIsWorldMarket, InTransactionId, InResult));
}

void FGsMarketManager::ClearPurchaseResult()
{
	_purchaseResultList.Empty();
}

void FGsMarketManager::RemoveItem(bool bIsWorldMarket, MarketTransactionId InTransitionId)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->RemoveItem(InTransitionId);
	}
}

void FGsMarketManager::RemoveSettlementItem(bool bIsWorldMarket, MarketTransactionId InTransitionId)
{
	int32 settlementCount = 0;
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->RemoveSettlementItem(InTransitionId);
		settlementCount = controller->GetSettlementCount();
	}

	GGameData()->SetMarketSettlementCount(bIsWorldMarket, settlementCount);

	CheckRedDot(true);
}

void FGsMarketManager::RemoveAllSettlementItems(bool bIsWorldMarket)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->RemoveAllSettlementItems();
	}

	GGameData()->SetMarketSettlementCount(bIsWorldMarket, 0);

	CheckRedDot(true);
}

void FGsMarketManager::ClearSearchData()
{
	_serchItemInfo.Key = INVALID_ITEM_ID;
	_serchItemInfo.Value = false;
	_strSearch.Empty();

	for (FGsMarketControllerBase* iter : _controllerList)
	{
		iter->ClearSearchData();
	}
}

void FGsMarketManager::ClearFilterData()
{
	_filterData.Clear();

	for (FGsMarketControllerBase* iter : _controllerList)
	{
		iter->ClearFilterData();
	}
}

bool FGsMarketManager::AddMultiplePurchaseList(bool bIsWorldMarket, MarketTransactionId InTransactionId)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		return controller->AddMultiplePurchaseList(InTransactionId);
	}

	return false;
}

bool FGsMarketManager::RemoveMultiplePurchaseList(bool bIsWorldMarket, MarketTransactionId InTransactionId)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		return controller->RemoveMultiplePurchaseList(InTransactionId);
	}

	return false;
}

bool FGsMarketManager::IsMultiplePurchaseTarget(bool bIsWorldMarket, MarketTransactionId InTransactionId) const
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		return controller->IsMultiplePurchaseTarget(InTransactionId);
	}

	return false;
}

void FGsMarketManager::ClearMultiplePurchaseList()
{
	for (FGsMarketControllerBase* iter : _controllerList)
	{
		iter->ClearMultiplePurchaseList();
	}
}

int32 FGsMarketManager::GetMultiplePurchaseCount(bool bIsWorldMarket) const
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		return controller->GetMultiplePurchaseCount();
	}

	return 0;
}

TArray<MarketTransactionId> FGsMarketManager::GetMultiplePurchaseList(bool bIsWorldMarket) const
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		return controller->GetMultiplePurchaseList();
	}

	return TArray<MarketTransactionId>();
}

void FGsMarketManager::ClearCollectionData()
{
	_collectionDataMap.Empty();
	_poolCollectionData.ReleaseAll();
}

void FGsMarketManager::SetCollectionData(PD::GC::PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ* InPacket)
{
	if (FGsMarketCollectionData** collectionDataPtr = _collectionDataMap.Find(InPacket->CollectionId()))
	{
		(*collectionDataPtr)->SetData(InPacket);
	}
	else
	{
		FGsMarketCollectionData* collectionData = CreateOrGetCollectionData(InPacket->CollectionId());
		collectionData->SetData(InPacket);
	}
}

FGsMarketCollectionData* FGsMarketManager::CreateOrGetCollectionData(CollectionId InCollectionId)
{
	FGsMarketCollectionData* collectionData = nullptr;

	if (FGsMarketCollectionData* const* dataPtr = _collectionDataMap.Find(InCollectionId))
	{
		collectionData = *dataPtr;
	}
	else
	{
		collectionData = _poolCollectionData.Claim();
		collectionData->Init(InCollectionId);

		_collectionDataMap.Emplace(InCollectionId, collectionData);
	}

	return collectionData;
}

FGsMarketCollectionData* FGsMarketManager::GetCollectionData(CollectionId InCollectionId) const
{
	if (FGsMarketCollectionData*const* dataPtr = _collectionDataMap.Find(InCollectionId))
	{
		return *dataPtr;
	}

	return nullptr;
}

void FGsMarketManager::SendReqBookmarkList(bool bIsWorldMarket)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->SendReqBookmarkList();
	}
}

void FGsMarketManager::SendReqItemGroupList(bool bIsWorldMarket, MarketCategory InCategory, bool bInResetScroll, bool bInIsScrolled)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->SendReqItemGroupList(InCategory, bInResetScroll, bInIsScrolled);
	}
}

void FGsMarketManager::SendReqItemList(bool bIsWorldMarket, ItemId InItemId, bool bResetTime)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->SendReqItemList(InItemId, bResetTime);
	}
}

void FGsMarketManager::SendReqSellItemList(bool bIsWorldMarket, bool bResetCoolTime)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->SendReqSellItemList(bResetCoolTime);
	}
}

void FGsMarketManager::SendReqSettlementList(bool bIsWorldMarket)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->SendReqSettlementList();
	}
}

void FGsMarketManager::SendReqLogList(bool bIsWorldMarket, bool bResetScroll, bool bInIsScrolled)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->SendReqLogList(bResetScroll, bInIsScrolled);
	}
}

void FGsMarketManager::SendSearchItem(bool bIsWorldMarket, ItemId InItemId)
{
	_serchItemInfo.Key = InItemId;
	_serchItemInfo.Value = bIsWorldMarket;

	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		_strSearch = controller->SendSearchItem(_serchItemInfo.Key);
	}
}

void FGsMarketManager::SendSearchedItemList(bool bIsWorldMarket, const FString& InStr, EGsMarketUICategory InCategory, int32 InIndex)
{
	_strSearch = InStr;

	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->SendSearchedItemList(InStr, InCategory, InIndex);
	}
}

void FGsMarketManager::SendSearchedItemList(bool bIsWorldMarket)
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		controller->SendSearchedItemList();
	}
}

void FGsMarketManager::SetFilterData(FGsMarketFilterData& InData)
{
	_filterData.Set(InData);

	// 둘다 동일하게 적용
	for (FGsMarketControllerBase* iter : _controllerList)
	{
		iter->SetFilterData(_filterData);
	}
}

bool FGsMarketManager::IsExistData(bool bIsWorldMarket, MarketCategory InCategory) const
{
	if (FGsMarketControllerBase* controller = GetController(bIsWorldMarket))
	{
		return controller->IsExistData(InCategory);
	}

	return false;
}

int32 FGsMarketManager::GetPurchaseSuccessCount() const
{
	int32 successCount = 0;
	for (const FGsMarketPurchaseResult& iter : _purchaseResultList)
	{
		if (PACKET_RESULT_SUCCESS == iter._result)
		{
			++successCount;
		}
	}

	return successCount;
}

int32 FGsMarketManager::GetPurchaseResult(bool bIsWorldMarket, MarketTransactionId InTransactionId)
{
	for (const FGsMarketPurchaseResult& iter : _purchaseResultList)
	{
		if (bIsWorldMarket == iter._bIsWorldMarket &&
			InTransactionId == iter._transactionId)
		{
			return iter._result;
		}
	}

	return -1;
}

FGsMarketControllerBase* FGsMarketManager::GetController(bool bIsWorldMarket) const
{
	if (bIsWorldMarket)
	{
		return _controllerWorld;
	}

	return _controllerNormal;
}

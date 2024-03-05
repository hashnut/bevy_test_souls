// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketCollectionData.h"
#include "UTIL/GsTableUtil.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Item/Collection/GsItemCollectionField.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"

//----------------------------------------------------------------------------------------------------------------------
void FGsMarketCollectionItemData::SetData(void* InData)
{
	PD::GC::PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ::TransactionList* data =
		reinterpret_cast<PD::GC::PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ::TransactionList*>(InData);

	_transactionId = data->MarketTransactionId();
	_itemId = data->ItemId();
	_enchantLevel = data->Level();
	_amount = data->Amount();
	_price = data->Price();
	_bIsWorldMarket = data->IsWdMarket();
}

const FGsSchemaItemCommon* FGsMarketCollectionItemData::GetTable() const
{
	if (INVALID_ITEM_ID != _itemId)
	{
		if (const FGsSchemaItemCommon* row = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(_itemId))
		{
			return row;
		}
	}

	return nullptr;
}

bool FGsMarketCollectionItemData::IsSameItem(const TPair<ItemId, Level>& InKeyPair) const
{
	if (InKeyPair.Key == _itemId &&
		InKeyPair.Value == _enchantLevel)
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------------------------------------------
void FGsMarketCollectionData::Init(CollectionId InCollectionId)
{
	Clear();

	_collectionId = InCollectionId;

	_checkerPrice.SetIntervalSecond(5.f);
	_checkerPricePerUnit.SetIntervalSecond(5.f);
}

void FGsMarketCollectionData::Clear()
{
	_collectionId = INVALID_COLLECTION_ID;
	_pricePerUnitDataList.Empty();
	_priceDataList.Empty();
	_otherLevelMap.Empty();
	_checkerPrice.Reset();
	_checkerPricePerUnit.Reset();
}

void FGsMarketCollectionData::GetLowestPriceRequestInfo(const FGsItemCollectionField* InData,
	OUT TArray<FGsMarketLowestPriceRequestInfo>& OutList)
{
	const TArray<FGsItemCollectionCondition>& conditionList = InData->Conditions();

	TMap<PriceKeyPair, int32> reqMap;

	for (const FGsItemCollectionCondition& condition : conditionList)
	{
		// 완료된 컬렉션 항목은 요청 보내지 않음
		if (ECollectionMaterialActionType::Completed == condition.ActionType())
		{
			continue;
		}

		// 쌓을 수 있는 아이템은 1개로 요청(상품 자체가 여러개 스택될 수 있으므로 상품 개수는 1개만 보냄)
		// 쌓을 수 없는 아이템은 갯수대로 합쳐서 요청 

		// 필요 개수가 있을 때만 진행
		Currency needCount = condition.ConditionCount() - condition.RegisterCount();
		if (0 < needCount)
		{
			const PriceKeyPair& keyPair = condition.EntryMaterialInfo();

			if (IsStackableItem(keyPair.Key))
			{
				if (int32* reqCount = reqMap.Find(keyPair))
				{
					*reqCount = 1;
				}
				else
				{
					reqMap.Emplace(keyPair, 1);
				}
			}
			else
			{
				if (int32* reqCount = reqMap.Find(keyPair))
				{
					*reqCount += needCount;
				}
				else
				{
					reqMap.Emplace(keyPair, needCount);
				}
			}
		}
	}

	for (TPair<PriceKeyPair, int32> itPair : reqMap)
	{
		OutList.Emplace(FGsMarketLowestPriceRequestInfo(itPair.Key.Key, itPair.Key.Value, itPair.Value));
	}
}

bool FGsMarketCollectionData::IsStackableItem(ItemId InItemId)
{
	if (const FGsSchemaItemCommon* row = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(InItemId))
	{
		if (ItemType::EQUIP != row->type)
		{
			return true;
		}
	}

	return false;
}

void FGsMarketCollectionData::SetData(PD::GC::PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ* InData)
{
	bool bUsePricePerUnit = InData->IsSearchPricePerUnit();

	TArray<FGsMarketCollectionItemData>& dataList = GetDataList(bUsePricePerUnit);
	dataList.Empty();
	
	TArray<TPair<MarketTransactionId, bool>> idList;

	for (PD::GC::PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ::TransactionListIterator iter = 
		InData->GetFirstTransactionListIterator(); iter != InData->GetLastTransactionListIterator(); ++iter)
	{
		// 월드에 따라 TransactionId가 고유하지 않다고 해서 페어로 키를 잡음
		TPair<MarketTransactionId, bool> key(iter->MarketTransactionId(), iter->IsWdMarket());

		// 같은 상품을 중복하여 저장하지 않기 위한 처리
		if (idList.Contains(TPair<MarketTransactionId, bool>(key)))
		{
			continue;
		}
		idList.Emplace(key);

		FGsMarketCollectionItemData data;
		data.SetData(&(*iter));
		dataList.Emplace(data);
	}

	// 다른 레벨 아이템이 존재할 경우, 표시 및 바로가기 하기 위한 정보 
	_otherLevelMap.Empty();
	for (PD::GC::PKT_GC_ACK_COLLECTION_ITEM_MARKET_PRICE_LOWEST_READ::SimilarItemListIterator iter =
		InData->GetFirstSimilarItemListIterator(); iter != InData->GetLastSimilarItemListIterator(); ++iter)
	{
		// 원래는 중복되면 안되지만, 안전을 위해 체크함. 중복정보 무시
		if (_otherLevelMap.Contains(iter->ItemId()))
		{
			continue;
		}

		_otherLevelMap.Emplace(iter->ItemId(), iter->IsWdMarket());
	}
}

FGsIntervalChecker& FGsMarketCollectionData::GetChecker(bool bInUsePricePerUnit)
{
	return bInUsePricePerUnit ? _checkerPricePerUnit : _checkerPrice;
}

TArray<FGsMarketCollectionItemData>& FGsMarketCollectionData::GetDataList(bool bInUsePricePerUnit)
{
	return bInUsePricePerUnit ? _pricePerUnitDataList : _priceDataList;
}

const FGsMarketCollectionItemData* FGsMarketCollectionData::GetLowestPricePerUnitItem(const PriceKeyPair& InKey)
{
	for (FGsMarketCollectionItemData& iter : _pricePerUnitDataList)
	{
		if (InKey.Key == iter._itemId &&
			InKey.Value == iter._enchantLevel)
		{
			return &iter;
		}
	}

	return nullptr;
}

void FGsMarketCollectionData::GetMarketItemList(TArray<PriceKeyPair>& InKeyList, bool bInUsePricePerUnit,
	OUT TArray<const FGsMarketCollectionItemData*>& OutList)
{
	TArray<FGsMarketCollectionItemData>& dataList = GetDataList(bInUsePricePerUnit);

	for (FGsMarketCollectionItemData& marketItem : dataList)
	{
		for (const PriceKeyPair& keyPair : InKeyList)
		{
			if (marketItem.IsSameItem(keyPair))
			{
				OutList.Emplace(&marketItem);
			}
		}
	}
}

bool FGsMarketCollectionData::IsOtherLevelExist(ItemId InItem) const
{
	return _otherLevelMap.Contains(InItem);
}

bool FGsMarketCollectionData::GetOtherLevelMarket(ItemId InItem, OUT bool& OutIsWorldMarket)
{
	if (_otherLevelMap.Contains(InItem))
	{
		OutIsWorldMarket = _otherLevelMap[InItem];
		return true;
	}

	return false;
}

void FGsMarketCollectionData::UpdateCoolTime(bool bInUsePricePerUnit)
{
	FGsIntervalChecker& checker = GetChecker(bInUsePricePerUnit);
	checker.UpdateTime();
}

void FGsMarketCollectionData::ResetCoolTime(bool bInUsePricePerUnit)
{
	FGsIntervalChecker& checker = GetChecker(bInUsePricePerUnit);
	checker.Reset();
}

bool FGsMarketCollectionData::IsCoolTime(bool bInUsePricePerUnit)
{
	FGsIntervalChecker& checker = GetChecker(bInUsePricePerUnit);
	return !checker.IsTimeOver();
}

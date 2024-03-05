// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketItemGroupData.h"
#include "Market/GsMarketItemData.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"


void FGsMarketItemGroupData::Set(const FGsSchemaItemCommon* InTable)
{
	_itemId = InTable->id;
	_table = InTable;
	_marketCategory = SharedFunc::GetMarketCategory(InTable->categorySub);
}

void FGsMarketItemGroupData::SetData(void* InData)
{
	PD::GC::PKT_GC_ACK_MARKET_SALES_ITEM_LIST_READ::SalesItemList* data = 
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_SALES_ITEM_LIST_READ::SalesItemList*>(InData);

	_lowestPrice = data->LowestPrice();
	_salesCount = data->SalesCount();
}

void FGsMarketItemGroupData::SetDataBookmark(void* InData)
{
	PD::GC::PKT_GC_ACK_MARKET_BOOKMARK_LIST_READ::MarketBookmarkList* data =
		reinterpret_cast<PD::GC::PKT_GC_ACK_MARKET_BOOKMARK_LIST_READ::MarketBookmarkList*>(InData);

	_lowestPrice = data->LowestPrice();
	_salesCount = data->SalesCount();
}

void FGsMarketItemGroupData::SetDataWd(void* InData)
{
	PD::GC::PKT_GC_ACK_WDMARKET_SALES_ITEM_LIST_READ::SalesItemList* data =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_SALES_ITEM_LIST_READ::SalesItemList*>(InData);

	_lowestPrice = data->LowestPrice();
	_salesCount = data->SalesCount();
}

void FGsMarketItemGroupData::SetDataBookmarkWd(void* InData)
{
	PD::GC::PKT_GC_ACK_WDMARKET_BOOKMARK_LIST_READ::MarketBookmarkList* data =
		reinterpret_cast<PD::GC::PKT_GC_ACK_WDMARKET_BOOKMARK_LIST_READ::MarketBookmarkList*>(InData);

	_lowestPrice = data->LowestPrice();
	_salesCount = data->SalesCount();
}

void FGsMarketItemGroupData::ClearItemGroupData()
{
	_lowestPrice = 0;
	_salesCount = 0;
}

void FGsMarketItemGroupData::ClearItemData()
{
	_itemLevelMap.Empty();
	_itemList.Empty();
}

void FGsMarketItemGroupData::SetItemData(FGsMarketItemData* InData)
{
	_itemList.Emplace(InData);

	int32 enchantLevel = InData->GetEnchantLevel();
	if (TArray<FGsMarketItemData*>* list = _itemLevelMap.Find(enchantLevel))
	{
		list->Emplace(InData);
	}
	else
	{
		TArray<FGsMarketItemData*> tempList;
		tempList.Emplace(InData);

		_itemLevelMap.Emplace(enchantLevel, tempList);
	}
}

void FGsMarketItemGroupData::RemoveItemData(MarketTransactionId InTransitionId)
{
	bool bIsExist = false;

	for (FGsMarketItemData* item : _itemList)
	{
		if (InTransitionId == item->_transactionId)
		{
			bIsExist = true;
			_itemList.Remove(item);
			break;
		}
	}

	for (TPair<int32, TArray<FGsMarketItemData*>>& pair : _itemLevelMap)
	{
		for (FGsMarketItemData* item : pair.Value)
		{
			if (InTransitionId == item->_transactionId)
			{
				bIsExist = true;
				pair.Value.Remove(item);
				break;
			}
		}
	}

	if (bIsExist)
	{
		if (0 < _salesCount)
		{
			--_salesCount;
		}
	}
}

bool FGsMarketItemGroupData::IsSaleItemExist() const
{
	return (0 < _salesCount) ? true : false;
}

FString FGsMarketItemGroupData::GetItemNameString() const
{
	return _table ? _table->name.ToString() : TEXT("");
}

ItemGrade FGsMarketItemGroupData::GetItemGrade() const
{
	return _table ? _table->grade : ItemGrade::NONE;
}

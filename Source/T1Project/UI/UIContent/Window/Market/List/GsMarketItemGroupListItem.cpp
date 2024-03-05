// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketItemGroupListItem.h"
#include "Market/GsMarketItemGroupData.h"
#include "Market/GsMarketItemData.h"

void UGsMarketItemGroupListItem::Clear()
{
	_bIsWorldMarket = false;
	_data = nullptr;
	_bIsSecondDepth = false;
	_enchantLevel = 0;
}

void UGsMarketItemGroupListItem::SetData(bool bIsWorldMarket, const FGsMarketItemGroupData* InData, bool bIsSecondDepth, int32 InEnchantLevel)
{
	_bIsWorldMarket = bIsWorldMarket;
	_data = InData;
	_bIsSecondDepth = bIsSecondDepth;
	_enchantLevel = InEnchantLevel;
}

MarketPrice UGsMarketItemGroupListItem::GetLowestPrice() const
{
	if (_data)
	{
		// 1뎁스
		if (false == _bIsSecondDepth)
		{
			return _data->_lowestPrice;
		}

		// 2뎁스
		if (const TArray<FGsMarketItemData*>* tempList = _data->_itemLevelMap.Find(_enchantLevel))
		{
			MarketPrice lowestPrice = 0;
			const TArray<FGsMarketItemData*> list = *tempList;
			for (int32 i = 0, maxCount = list.Num(); i < maxCount; ++i)
			{
				FGsMarketItemData* itemData = list[i];

				if (0 == i)
				{
					lowestPrice = itemData->_price;
					continue;
				}

				if (lowestPrice > itemData->_price)
				{
					lowestPrice = itemData->_price;
				}
			}

			return lowestPrice;
		}
	}

	return 0;
}

int32 UGsMarketItemGroupListItem::GetSalesCount() const
{
	if (_data)
	{
		// 1뎁스
		if (false == _bIsSecondDepth)
		{
			return _data->_salesCount;
		}

		// 2뎁스
		if (const TArray<FGsMarketItemData*>* list = _data->_itemLevelMap.Find(_enchantLevel))
		{
			return list->Num();
		}
	}

	return 0;
}

bool UGsMarketItemGroupListItem::IsSalesItemExist() const
{
	if (_data)
	{
		// 1뎁스
		if (false == _bIsSecondDepth)
		{
			return (0 < _data->_salesCount) ? true : false;
		}

		// 2뎁스
		if (const TArray<FGsMarketItemData*>* list = _data->_itemLevelMap.Find(_enchantLevel))
		{
			return (0 < list->Num()) ? true : false;
		}
	}

	return false;
}

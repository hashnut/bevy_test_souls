// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketListViewControlItem.h"
#include "UI/UIContent/Window/Market/List/GsMarketItemListItem.h"


FGsMarketListViewControlItem::~FGsMarketListViewControlItem()
{
	ClearData();
}

void FGsMarketListViewControlItem::ClearData()
{
	if (_listItemPool.IsValid())
	{
		for (UGsMarketItemListItem* listItem : _dataList)
		{
			_listItemPool->ReleaseOne(listItem);
		}
	}
	_dataList.Empty();

	_listView->ClearListItems();
}

void FGsMarketListViewControlItem::SetData(UGsMarketItemListItem* InData)
{
	_dataList.Emplace(InData);
}

bool FGsMarketListViewControlItem::UpdateList(bool bInResetScroll, int32 InLastIndex)
{
	_listView->SetListItems(_dataList);
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	return Super::UpdateList(bInResetScroll, InLastIndex);
}

void FGsMarketListViewControlItem::SortByPrice(bool isDescending)
{
	if (isDescending)
	{
		_dataList.Sort([](const UGsMarketItemListItem& A, const UGsMarketItemListItem& B)
			{
				return (A._data->_price > B._data->_price) ? true : false;
			});
	}
	else
	{
		_dataList.Sort([](const UGsMarketItemListItem& A, const UGsMarketItemListItem& B)
			{
				return (A._data->_price < B._data->_price) ? true : false;
			});
	}

	UpdateList(true);
}

void FGsMarketListViewControlItem::SortByPricePerUnit(bool isDescending)
{
	if (isDescending)
	{
		_dataList.Sort([](const UGsMarketItemListItem& A, const UGsMarketItemListItem& B)
			{
				return (A._data->GetPricePerUnit() > B._data->GetPricePerUnit()) ? true : false;
			});
	}
	else
	{
		_dataList.Sort([](const UGsMarketItemListItem& A, const UGsMarketItemListItem& B)
			{
				return (A._data->GetPricePerUnit() < B._data->GetPricePerUnit()) ? true : false;
			});
	}

	UpdateList(true);
}

void FGsMarketListViewControlItem::GetTopNTransactionIdList(int32 InCount, OUT TArray<MarketTransactionId>& OutList)
{
	OutList.Empty();

	// 데이터 수가 더 적을경우 있는만큼만 넣음
	int32 maxCount = (_dataList.Num() < InCount) ? _dataList.Num() : InCount;

	for (int i = 0; i < maxCount; ++i)
	{
		if (_dataList[i] &&
			_dataList[i]->_data)
		{
			if (INVALID_MARKET_TRANSACTION_ID != _dataList[i]->_data->_transactionId)
			{
				OutList.Emplace(_dataList[i]->_data->_transactionId);
			}
		}
	}
}

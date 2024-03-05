// Fill out your copyright notice in the Description page of Project Settings.


#include "GsMarketListViewControlItemGroup.h"
#include "UI/UIContent/Window/Market/List/GsMarketItemGroupListItem.h"
#include "Market/GsMarketItemGroupData.h"


FGsMarketListViewControlItemGroup::~FGsMarketListViewControlItemGroup()
{
	ClearData();
}

void FGsMarketListViewControlItemGroup::ClearData()
{
	if (_listItemPool.IsValid())
	{
		for (UGsMarketItemGroupListItem* listItem : _dataList)
		{
			_listItemPool->ReleaseOne(listItem);
		}
	}	
	_dataList.Empty();
	
	_listView->ClearListItems();
}

void FGsMarketListViewControlItemGroup::SetData(UGsMarketItemGroupListItem* InData)
{
	_dataList.Emplace(InData);
}

void FGsMarketListViewControlItemGroup::SortByEnchantLevel()
{
	_dataList.Sort([](UGsMarketItemGroupListItem& InA, UGsMarketItemGroupListItem& InB)
		{
			return (InA._enchantLevel >= InB._enchantLevel) ? true : false;
		});
}

bool FGsMarketListViewControlItemGroup::UpdateList(bool bInResetScroll, int32 InLastIndex)
{
	_listView->SetListItems(_dataList);
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	return Super::UpdateList(bInResetScroll, InLastIndex);
}

int32 FGsMarketListViewControlItemGroup::FindDataIndex(ItemId InItemId)
{
	for (int32 i = 0, maxCount = _dataList.Num(); i < maxCount; ++i)
	{
		if (UGsMarketItemGroupListItem* listItem = _dataList[i])
		{
			if (listItem->_data->GetItemId() == InItemId)
			{
				return i;
			}
		}
	}

	return -1;
}

void FGsMarketListViewControlItemGroup::AddData(class UGsMarketItemGroupListItem* InData)
{
	_dataList.Emplace(InData);
	_listView->AddItem(InData);
}

void FGsMarketListViewControlItemGroup::RemoveData(ItemId InItemId)
{
	for (int32 i = 0, maxCount = _dataList.Num(); i < maxCount; ++i)
	{
		if (UGsMarketItemGroupListItem* listItem = _dataList[i])
		{
			if (listItem->_data->GetItemId() == InItemId)
			{				
				_listView->RemoveItem(_dataList[i]);
				_dataList.RemoveAt(i);
				
				break;
			}
		}
	}
}

void FGsMarketListViewControlItemGroup::ChangeData(int32 InIndex, class UGsMarketItemGroupListItem* InData)
{
	if (_dataList.IsValidIndex(InIndex))
	{
		_dataList[InIndex] = InData;
	}

	UpdateList(false);
}

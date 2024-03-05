// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSaveBatteryManager.h"
#include "Item/GsItemManager.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"


Exp FGsSaveBatteryManager::IncreaseExp(Exp InExp)
{
	_increasedExp += InExp;
	return _increasedExp;
}

Currency FGsSaveBatteryManager::IncreaseGold(Currency InAmount)
{
	_increasedGold += InAmount;
	return _increasedGold;
}

void FGsSaveBatteryManager::ResetData()
{
	_increasedExp = 0;
	_increasedGold = 0;

	_startTime = FGsTimeSyncUtil::GetServerNowDateTime();

	_displayItemList.Empty();
	_displayItemAmountMap.Empty();
	_autoDecomposeItemAmountMap.Empty();
	LoadInventoryState();
}

void FGsSaveBatteryManager::LoadInventoryState()
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	_itemAmountStateMap.Empty();
	_nonStackableItemList.Empty();

	int32 itemTID = 0;
	ItemDBId itemDBId = 0;
	ItemAmount itemAmount = 0;
	const TArray<TWeakPtr<FGsItem>> items = itemManager->FindAllItems();
	for (int32 i = 0; i < items.Num(); ++i)
	{
		if (false == items[i].IsValid())
			continue;

		if (items[i].Pin()->IsStackable())
		{
			itemTID = items[i].Pin()->GetTID();
			if (_itemAmountStateMap.Contains(itemTID))
				continue;

			itemAmount = itemManager->GetAllItemCountByTID(itemTID);
			if (0 >= itemAmount)
				continue;

			_itemAmountStateMap.Emplace(itemTID, itemAmount);
		}
		else
		{
			itemDBId = items[i].Pin()->GetDBID();
			if (_nonStackableItemList.Contains(itemDBId))
				continue;

			_nonStackableItemList.Emplace(itemDBId);
		}
	}
}

ItemAmount FGsSaveBatteryManager::GetItemAmountByTID(int32 InItemTID)
{
	return _itemAmountStateMap.Contains(InItemTID) ? _itemAmountStateMap[InItemTID] : 0;
}

void FGsSaveBatteryManager::SetItemAmount(int32 InItemTID, ItemAmount InItemAmount)
{
	if (_itemAmountStateMap.Contains(InItemTID))
	{
		_itemAmountStateMap[InItemTID] = InItemAmount;
	}
	else
	{
		_itemAmountStateMap.Emplace(InItemTID, InItemAmount);
	}
}

bool FGsSaveBatteryManager::AddItem(const uint64 InItemDBID, OUT TArray<int32>& OutIndexList)
{
	OutIndexList.Empty();

	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return false;

	TWeakPtr<FGsItem> itemData = itemManager->FindItem(InItemDBID);
	if (false == itemData.IsValid())
		return false;

	const int32 itemTID = itemData.Pin()->GetTID();

	if (itemData.Pin()->IsStackable())
	{
		ItemAmount prevAmount = 0;
		ItemAmount currAmount = 0;

		// 캐릭터가 가진 총 개수 갱신 및 획득 수량 판단
		prevAmount = GetItemAmountByTID(itemTID);
		currAmount = itemManager->GetAllItemCountByTID(itemTID);
		SetItemAmount(itemTID, currAmount);

		ItemAmount deltaAmount = currAmount - prevAmount;
		if (0 >= deltaAmount)
		{
			return false;
		}

		ItemAmount gainAmount = currAmount - prevAmount;

		// 절전 모드 후 얻은 개수 갱신
		if (_displayItemAmountMap.Contains(itemTID))
		{
			prevAmount = _displayItemAmountMap[itemTID];
			currAmount = prevAmount + gainAmount;
			_displayItemAmountMap[itemTID] = currAmount;
		}
		else
		{
			prevAmount = 0;
			currAmount = gainAmount;
			_displayItemAmountMap.Emplace(itemTID, gainAmount);
		}

		// 갱신할 위치 계산
		ItemAmount maxStackCount = itemData.Pin()->GetMaxStackCount();
		ItemAmount prevDivision = prevAmount / maxStackCount;
		ItemAmount prevRemainder = prevAmount - prevDivision * maxStackCount;
		ItemAmount currDivision = currAmount / maxStackCount;
		ItemAmount currRemainder = currAmount - currDivision * maxStackCount;

		if (prevDivision < currDivision)
		{
			for (ItemAmount i = prevDivision; i < currDivision; ++i)
			{
				uint16 index = 0;
				if (i == prevDivision && 0 < prevRemainder && true == FindLastItem(itemTID, index))
				{
					_displayItemList[index]._itemAmount = maxStackCount;
					OutIndexList.Emplace(index);
				}
				else
				{
					_displayItemList.Emplace(itemTID, maxStackCount);
					OutIndexList.Emplace(_displayItemList.Num() - 1);
				}
			}

			if (0 < currRemainder)
			{
				_displayItemList.Emplace(itemTID, currRemainder);
				OutIndexList.Emplace(_displayItemList.Num() - 1);
			}
		}
		else if (prevDivision == currDivision)
		{
			uint16 index = 0;
			if (0 < prevRemainder && true == FindLastItem(itemTID, index))
			{
				_displayItemList[index]._itemAmount = currRemainder;
				OutIndexList.Emplace(index);
			}
			else
			{
				_displayItemList.Emplace(itemTID, currRemainder);
				OutIndexList.Emplace(_displayItemList.Num() - 1);
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (_nonStackableItemList.Contains(InItemDBID))
			return false;

		_nonStackableItemList.Emplace(InItemDBID);
		_displayItemList.Emplace(itemTID, 1);
		OutIndexList.Emplace(_displayItemList.Num() - 1);
	}

	return true;
}

bool FGsSaveBatteryManager::FindLastItem(int32 InItemTID, OUT uint16& OutIndex)
{
	for (uint16 i = _displayItemList.Num(); i > 0; --i)
	{
		const DisplayItem& displayItem = _displayItemList[i - 1];
		if (InItemTID == displayItem._itemTID)
		{
			OutIndex = i - 1;
			return true;
		}
	}

	return false;
}

bool FGsSaveBatteryManager::GetDisplayItemByIndex(int32 InIndex, OUT int32& OutTID, OUT ItemAmount& OutAmount)
{
	if (_displayItemList.IsValidIndex(InIndex))
	{
		OutTID = _displayItemList[InIndex]._itemTID;
		OutAmount = _displayItemList[InIndex]._itemAmount;
		return true;
	}

	return false;
}

int32 FGsSaveBatteryManager::GetDisplayItemCount()
{
	return _displayItemList.Num();
}

void FGsSaveBatteryManager::AddAutoDecomposeItemAmount(ItemId InItemTID, ItemAmount InItemAmount)
{
	if (_autoDecomposeItemAmountMap.Contains(InItemTID))
	{
		_autoDecomposeItemAmountMap[InItemTID] += InItemAmount;
	}
	else
	{
		_autoDecomposeItemAmountMap.Emplace(InItemTID, InItemAmount);
	}
}

void FGsSaveBatteryManager::ShowAndClearAutoDecomposeItemAmount()
{
	for (const TPair<int32, ItemAmount>& itemDataCreated : _autoDecomposeItemAmountMap)
	{
		const FGsSchemaItemCommon* itemCommon = UGsItemManager::GetItemTableDataByTID(itemDataCreated.Key);
		if (nullptr == itemCommon)
			continue;

		FText findText;
		if (true == FText::FindText(TEXT("OptionText"), TEXT("AutoDecompose_SleepMode_Message"), findText))
		{
			FText resultText = FText::Format(findText, itemCommon->name, itemDataCreated.Value);
			FGsUIHelper::AddChatMessageSystem(resultText);
		}
	}

	_autoDecomposeItemAmountMap.Empty();
}

bool FGsSaveBatteryManager::IsSaveBatteryTurnedOn()
{
	return GUI()->IsActiveWidget(TEXT("TraySaveBattery"));
}

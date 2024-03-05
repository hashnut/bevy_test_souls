// Fill out your copyright notice in the Description page of Project Settings.


#include "GsRewardIconDataWrapper.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "Reward/GsRewardHelper.h"


void FGsRewardIconDataWrapper::SetUIIconItem(FGsRewardIconDataWrapper* InData, OUT UGsUIIconItem* OutIcon)
{
	if (InData)
	{
		InData->SetDataToUIIconItem(OutIcon);
	}
}

bool FGsRewardIconDataWrapper::IsValid() const
{
	switch (_dataType)
	{
	case EGsRewardIconDataType::ITEM_BAG:
	{
		if (nullptr == _itemBag)
		{
			return false;
		}

		if (0 >= _itemBag->itemId &&
			CurrencyType::NONE == _itemBag->currencyType)
		{
			return false;
		}
	}
	break;
	case EGsRewardIconDataType::CURRENCY:
	{
		if (CurrencyType::NONE == _currencyType)
		{
			return false;
		}
	}
	break;
	case EGsRewardIconDataType::EXP:
	{
		// 딱히 없음 0값이면 그냥 0으로 표시하면 됨
	}
	break;
	}

	return true;
}

void FGsRewardIconDataWrapper::Clear()
{
	_dataType = EGsRewardIconDataType::NONE;
	_itemBag = nullptr;
	_currencyType = CurrencyType::NONE;
	_currencyAmountPair.Key = 0;
	_currencyAmountPair.Value = 0;
	_expPercent = 0;
}

void FGsRewardIconDataWrapper::CopyData(FGsRewardIconDataWrapper* InData)
{
	if (nullptr == InData)
	{
		Clear();
		return;
	}

	_dataType = InData->_dataType;
	_itemBag = InData->_itemBag;
	_currencyType = InData->_currencyType;
	_currencyAmountPair = InData->_currencyAmountPair;
	_expPercent = InData->_expPercent;
}

void FGsRewardIconDataWrapper::SetDataToUIIconItem(OUT UGsUIIconItem* OutIcon)
{
	if (false == IsValid())
	{
		OutIcon->SetNotOwnItem(0, 0);
		return;
	}

	switch (_dataType)
	{
	case EGsRewardIconDataType::ITEM_BAG:
	{
		if (_itemBag)
		{
			if (0 < _itemBag->itemId)
			{
				OutIcon->SetNotOwnItem(_itemBag->itemId, _itemBag->itemCountMin,
					_itemBag->itemLevel, _itemBag->enchantShield, _itemBag->enchantBonusIndex);
			}
			else if (CurrencyType::NONE != _itemBag->currencyType)
			{
				OutIcon->SetCurrency(_itemBag->currencyType, _itemBag->currencyAmountMin);
			}
		}
	}
	break;
	case EGsRewardIconDataType::CURRENCY:
	{
		OutIcon->SetCurrency(_currencyType, _currencyAmountPair.Key);
	}
	break;
	case EGsRewardIconDataType::EXP:
	{
		OutIcon->SetExpPercent(_expPercent);
	}
	break;
	}
}

#include "GsCraftTargetObject.h"

#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

FGsCraftTargetObject::FGsCraftTargetObject()
	: _costType(CostType::MAX)
{
}

FGsCraftTargetObject& FGsCraftTargetObject::operator=(const FGsCraftTargetObject& InRhs)
{
	_costType = InRhs._costType;
	_itemId = InRhs._itemId;
	_itemCount = InRhs._itemCount;
	_itemLevel = InRhs._itemLevel;
	return *this;
}

FGsCraftTargetObject::FGsCraftTargetObject(const ItemId InItemId, const ItemAmount InItemCount, const int32 InItemLevel)
	: _costType(CostType::ITEM)
	, _itemId(InItemId)
	, _itemCount(InItemCount)
	, _itemLevel(InItemLevel)
{
	if (INVALID_ITEM_ID == _itemId)
	{
		GSLOG(Error, TEXT("제작 결과 아이템이 INVALID_ITEM_ID 이므로 테이블 데이터 확인 필요"));
	}

	if (0 == _itemCount)
	{
		GSLOG(Error, TEXT("제작 결과 아이템의 갯수가 0 이므로 테이블 데이터 확인 필요"));
	}
}

FGsCraftTargetObject::FGsCraftTargetObject(const CurrencyType InCurrencyType)
	: _costType(CostType::CURRENCY)
	, _currencyType(InCurrencyType)
{
	if (CurrencyType::NONE == _currencyType)
	{
		GSLOG(Error, TEXT("제작 결과 재화가 INVALID_ITEM_ID 이므로 테이블 데이터 확인 필요"));
	}
}

FGsCraftTargetObject FGsCraftTargetObject::MakeCraftTargetObject(const FGsSchemaRewardItemBagData& InRewardItemBagData)
{
	// 다음 코드 특성 상, 리워드에 아이템과 재화가 동시 존재할 경우, 아이템 정보만 취하여 동작합니다.
	if (INVALID_ITEM_ID != InRewardItemBagData.itemId)
	{
		return FGsCraftTargetObject(InRewardItemBagData.itemId, InRewardItemBagData.itemCountMax, InRewardItemBagData.itemLevel);
	}
	else if (CurrencyType::NONE != InRewardItemBagData.currencyType)
	{
		return FGsCraftTargetObject(InRewardItemBagData.currencyType);
	}

	GSLOG(Error, TEXT("제작 결과가 아이템도 아니고 재화도 아님. itemBagPairId:%lld"), InRewardItemBagData.itemBagPairId);
	return FGsCraftTargetObject();
}

const FGsSchemaItemCommon* FGsCraftTargetObject::GetItemCommon() const
{
	check(CostType::ITEM == _costType);

	const UGsTableItemCommon* TableItemCommon = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
	if (nullptr == TableItemCommon)
	{
		GSLOG(Error, TEXT("nullptr == TableItemCommon"));
		return nullptr;
	}

	const FGsSchemaItemCommon* ItemCommon = nullptr;
	if (!TableItemCommon->FindRowById(_itemId, ItemCommon))
	{
		GSLOG(Error, TEXT("!TableItemCommon->FindRowById(_itemId, ItemCommon), _itemId:%d"), _itemId);
		return nullptr;
	}

	return ItemCommon;
}

CostType FGsCraftTargetObject::GetCostType() const
{
	return _costType;
}

ItemId FGsCraftTargetObject::GetItemId() const
{
	check(CostType::ITEM == _costType);
	return _itemId;
}

CurrencyType FGsCraftTargetObject::GetCurrencyType() const
{
	check(CostType::CURRENCY == _costType);
	return _currencyType;
}

FText FGsCraftTargetObject::GetCommonName() const
{
	if (CostType::ITEM == _costType)
	{
		const FGsSchemaItemCommon* ItemCommon = GetItemCommon();
		if (nullptr == ItemCommon)
		{
			GSLOG(Error, TEXT("nullptr == ItemCommon, _itemId:%d"), _itemId);
			return FText::GetEmpty();
		}
		else
		{
			return ItemCommon->name;
		}
	}
	else if (CostType::CURRENCY == _costType)
	{
		return FGsCurrencyHelper::GetCurrencyLocalizedText(_currencyType);
	}

	GSLOG(Error, TEXT("제작 결과가 아이템도 아니고 재화도 아님."));
	return FText::GetEmpty();
}

const FLinearColor& FGsCraftTargetObject::GetCommonColor() const
{
	if (CostType::ITEM == _costType)
	{
		return FGsUIColorHelper::GetColorItemGradeByItemId(_itemId);
	}

	return FGsUIColorHelper::GetColorItemGrade(ItemGrade::NONE);
}

FSoftObjectPath FGsCraftTargetObject::GetCommonIconPath() const
{
	if (CostType::ITEM == _costType)
	{
		const FGsSchemaItemCommon* ItemCommon = GetItemCommon();
		if (nullptr == ItemCommon)
		{
			GSLOG(Error, TEXT("nullptr == ItemCommon, _itemId:%d"), _itemId);
			return FSoftObjectPath();
		}
		else
		{
			return ItemCommon->icon;
		}
	}
	else if (CostType::CURRENCY == _costType)
	{
		return FGsCurrencyHelper::GetCurrencyIconPath(_currencyType);
	}

	GSLOG(Error, TEXT("Unknown _costType"), _itemId);
	return FSoftObjectPath();
}

const ItemAmount FGsCraftTargetObject::GetItemCount() const
{
	return _itemCount;
}

const int32 FGsCraftTargetObject::GetItemLevel() const
{
	return _itemLevel;
}

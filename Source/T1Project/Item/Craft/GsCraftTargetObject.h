#pragma once

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaCurrencyData;
struct FGsSchemaItemCommon;
struct FGsSchemaRewardItemBagData;
struct FLinearColor;

/**
 * 실제 제작 대상에 대한 정보로, 재화 혹은 아이템이 될 수 있어서 struct 로 캡슐화
 */
struct FGsCraftTargetObject final
{

private:
	CostType _costType;

	union
	{
		ItemId _itemId;
		CurrencyType _currencyType;
	};

	ItemAmount _itemCount;
	int32 _itemLevel;

public:
	FGsCraftTargetObject();

	FGsCraftTargetObject& operator=(const FGsCraftTargetObject& InRhs);

private:
	explicit FGsCraftTargetObject(const ItemId InItemId, const ItemAmount InItemCount, const int32 InItemLevel);
	
	explicit FGsCraftTargetObject(const CurrencyType InCurrencyType);

public:
	static FGsCraftTargetObject MakeCraftTargetObject(const FGsSchemaRewardItemBagData& InRewardItemBagData);

private:
	const FGsSchemaItemCommon* GetItemCommon() const;

public:
	CostType GetCostType() const;

	ItemId GetItemId() const;

	CurrencyType GetCurrencyType() const;

	FText GetCommonName() const;

	const FLinearColor& GetCommonColor() const;

	FSoftObjectPath GetCommonIconPath() const;

	const ItemAmount GetItemCount() const;

	const int32 GetItemLevel() const;
};

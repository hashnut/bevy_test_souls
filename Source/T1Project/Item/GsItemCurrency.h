#pragma once

#include "GsItem.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsSchemaItemCommon;
struct FGsSchemaItemCurrencyDetail;

/**
 * 재화형 아이템
 */
class FGsItemCurrency final : public FGsItem
{

private:
	using Super = FGsItem;

private:
	const FGsSchemaItemCurrencyDetail* _itemCurrencyDetail = nullptr;
	
public:
	explicit FGsItemCurrency(const FGsSchemaItemCommon* InItemCommon);

public:
	void UpdateBy(const FGsItem* InItem) final;

	int32 GetTooltipDurationTime() const final;

private:
	static const FGsSchemaItemCurrencyDetail* GetItemCurrencyDetailRow(const ItemId InItemDetailId);

};

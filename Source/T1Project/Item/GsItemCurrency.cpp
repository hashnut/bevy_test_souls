#include "GsItemCurrency.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "DataSchema/Item/GsSchemaItemCurrencyDetail.h"
#include "GsItem.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"

FGsItemCurrency::FGsItemCurrency(const FGsSchemaItemCommon* InItemCommonData)
	: _itemCurrencyDetail((nullptr == InItemCommonData) ? nullptr : FGsItemCurrency::GetItemCurrencyDetailRow(InItemCommonData->detailId))
{
}

void FGsItemCurrency::UpdateBy(const FGsItem* InItem)
{
	Super::UpdateBy(InItem);

	if (nullptr != InItem)
	{
		_itemCurrencyDetail = FGsItemCurrency::GetItemCurrencyDetailRow(InItem->GetDetailID());
	}
}

int32 FGsItemCurrency::GetTooltipDurationTime() const
{
	if (nullptr == _itemCurrencyDetail)
	{
		return 0;
	}

	return (_itemCurrencyDetail->tooltipDurationSec / 10000) * 100;
}

const FGsSchemaItemCurrencyDetail* FGsItemCurrency::GetItemCurrencyDetailRow(const ItemId InItemDetailId)
{
	const UGsTableItemCurrencyDetail* TableItemCurrencyDetail = Cast<UGsTableItemCurrencyDetail>(FGsSchemaItemCurrencyDetail::GetStaticTable());
	if (nullptr == TableItemCurrencyDetail)
	{
		GSLOG(Error, TEXT("nullptr == TableItemCurrencyDetail, InItemDetailId: %d"), InItemDetailId);
		return nullptr;
	}

	const FGsSchemaItemCurrencyDetail* Row = nullptr;
	if (!TableItemCurrencyDetail->FindRowById(InItemDetailId, Row))
	{
		GSLOG(Error, TEXT("!TableItemCurrencyDetail->FindRowById(%d, Row)"), InItemDetailId);
	}

	return Row;
}

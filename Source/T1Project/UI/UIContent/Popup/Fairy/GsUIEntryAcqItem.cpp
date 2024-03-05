// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIEntryAcqItem.h"
#include "Item/GsItemManager.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

void UGsUIEntryAcqItem::SetData(int index, FGsSchemaCeilingItemData& data, float rateTotal)
{
	if (data.ceilingItemId.GetRow() == nullptr) return;

	if (const FGsSchemaItemCommon* itemData = GItem()->GetItemTableDataByTID(data.ceilingItemId.GetRow()->id))
	{
		_textItemName = itemData->name;
		_colorItemName = FGsUIColorHelper::GetColorItemGrade(itemData->grade);
		_textItemAmount = FText::FromString(FString::Format(TEXT("x {0}"), { data.ceilingItemCount }));
		_colorItemAmount = FGsUIColorHelper::GetColorItemGrade(itemData->grade);
		_textAcqRate = FText::FromString(FString::Printf(TEXT("%.02f%%"), data.ceilingItemRate * 100.f / rateTotal ));
	}
}

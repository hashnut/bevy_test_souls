// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICustomizeBase.h"
#include "DataSchema/Customize/GsSchemaCustomizeColorPalette.h"


FReply UGsUICustomizeBase::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUICustomizeBase::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUICustomizeBase::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

int32 UGsUICustomizeBase::ConvertFloatToDataValue(float InValue)
{
	return FMath::FloorToInt(InValue * 100);
}

float UGsUICustomizeBase::ConvertIntToControlValue(int32 InValue)
{
	return InValue * 0.01f;
}

void UGsUICustomizeBase::GetBaseColorList(const TArray<int32>& InColorIndexList, TArray<FColor>& OutColorList)
{
	const UGsTable* table = FGsSchemaCustomizeColorPalette::GetStaticTable();
	if (nullptr == table)
	{
		return;
	}

	const FGsSchemaCustomizeColorPalette* row = nullptr;
	if (false == table->FindRow<FGsSchemaCustomizeColorPalette>(TEXT("Base"), row))
	{
		return;
	}

	OutColorList.Empty();

	int32 colorCount = row->colorList.Num();

	for (int32 i = 0; i < InColorIndexList.Num(); ++i)
	{
		int32 index = InColorIndexList[i];

		if (index < colorCount)
		{
			OutColorList.Add(row->colorList[index]);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGameStatisticsListEntry.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "GameStatistics/GsGameStatisticsData.h"


void UGsUIGameStatisticsListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnSlot->OnClicked.AddDynamic(this, &UGsUIGameStatisticsListEntry::OnClickSlot);
}

void UGsUIGameStatisticsListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_imgSelected->SetVisibility((_bIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

bool UGsUIGameStatisticsListEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIGameStatisticsListEntry::SetData(const FGsGameStatisticsData* InData)
{
	if (nullptr == InData ||
		nullptr == InData->_table)
	{
		_textBlockSubCategory->SetText(FText::GetEmpty());
		_textBlockName->SetText(FText::GetEmpty());
		_textBlockThisWeek->SetText(FText::GetEmpty());
		_textBlockLastWeek->SetText(FText::GetEmpty());
		_switcherValue->SetActiveWidgetIndex(1);
		_switcherUseDaily->SetActiveWidgetIndex(0);

		return;
	}

	_textBlockSubCategory->SetText(InData->_table->subCategoryNameText);
	_textBlockName->SetText(InData->_table->nameText);

	FText textValue;
	GetValueText(*InData, textValue);
	_textBlockThisWeek->SetText(textValue);

	// 지난주 대비
	if (InData->IsSameLastWeek())
	{
		_switcherValue->SetActiveWidgetIndex(1);
	}
	else
	{
		_switcherValue->SetActiveWidgetIndex(0);

		FText textCompare;
		FLinearColor colorUpDown;
		bool bIsPositive = GetCompareText(*InData, textCompare);		
		
		if (bIsPositive)
		{
			_switcherArrow->SetActiveWidgetIndex(1);
			colorUpDown = FGsUIColorHelper::GetColor(EGsUIColorType::UP_COMPARE_STAT_COLOR);
		}
		else
		{
			_switcherArrow->SetActiveWidgetIndex(0);
			colorUpDown = FGsUIColorHelper::GetColor(EGsUIColorType::DOWN_COMPARE_STAT_COLOR);
		}
		_textBlockLastWeek->SetText(textCompare);
		_textBlockLastWeek->SetColorAndOpacity(colorUpDown);
	}

	_switcherUseDaily->SetActiveWidgetIndex(InData->_table->useDailyGraph ? 1 : 0);
}

void UGsUIGameStatisticsListEntry::GetValueText(int64 InValue, EGsStatisticsValueType InType, OUT FText& OutText)
{
	switch (InType)
	{
	case EGsStatisticsValueType::RATE:
	{
		float valuePercent = 0.01f * InValue;
		FString strPercent = FString::Printf(TEXT("%.2f%%"), valuePercent);

		OutText = FText::FromString(strPercent);
	}
	break;
	case EGsStatisticsValueType::TIME:
	{
		int64 tempValue = InValue;

		bool bIsNagative = false;
		if (0 > InValue)
		{
			bIsNagative = true;
			tempValue *= -1;
		}		

		FTimespan time = FTimespan::FromSeconds(tempValue);
		FString strTime;
		FGsTimeStringHelper::GetTimeString(time, strTime);

		if (bIsNagative)
		{
			FString strOut = TEXT("-");
			strOut.Append(strTime);
			OutText = FText::FromString(strOut);
		}
		else
		{
			OutText = FText::FromString(strTime);
		}
	}
	break;
	default:
	{
		OutText = FText::AsNumber(InValue);
	}
	break;
	}
}

void UGsUIGameStatisticsListEntry::GetValueText(const FGsGameStatisticsData& InData, OUT FText& OutText)
{
	if (EGsStatisticsValueType::VALUE == InData._table->valueType)
	{
		OutText = FText::AsNumber(InData._value);
		return;
	}

	GetValueText(static_cast<int64>(InData._value), InData._table->valueType, OutText);
}

bool UGsUIGameStatisticsListEntry::GetCompareText(const FGsGameStatisticsData& InData, OUT FText& OutText)
{
	int64 compareValue = InData._value - InData._lastWeekValue;
	GetValueText(compareValue, InData._table->valueType, OutText);

	return (0 < compareValue) ? true : false;
}

void UGsUIGameStatisticsListEntry::OnClickSlot()
{
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}

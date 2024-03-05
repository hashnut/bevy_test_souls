// Fill out your copyright notice in the Description page of Project Settings.


#include "GsLevelUpStatListViewEntry.h"
#include "T1Project/GameObject/Stat/GsStatHelper.h"
#include "Internationalization/Text.h"
#include "UMG/Public/Components/Widget.h"
#include "Components/TextBlock.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsText.h"

void UGsLevelUpStatListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGsLevelUpStatListViewEntry::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsLevelUpStatListViewEntry::NativeDestruct()
{
	Super::NativeConstruct();
}

void UGsLevelUpStatListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsLevelUpStatListViewEntryData* data = Cast<UGsLevelUpStatListViewEntryData>(inListItemObject);

	if (data->_showStat1)
	{
		_statNameTextBlock1->SetVisibility(ESlateVisibility::Visible);
		_statValueTextBlock1->SetVisibility(ESlateVisibility::Visible);

		FText statName;
		if (FGsStatHelper::GetDisPlayStatName(data->_statType1, statName))
		{
			_statNameTextBlock1->SetText(statName);
		}

		int32 value = data->_statDiff1._currentStatValue - data->_statDiff1._preStatValue;
		FText statValueText = GetStatValue(data->_statType1, value);
		_statValueTextBlock1->SetText(statValueText);
	}
	else
	{
		_statNameTextBlock1->SetVisibility(ESlateVisibility::Hidden);
		_statValueTextBlock1->SetVisibility(ESlateVisibility::Hidden);
	}
	
	if (data->_showStat2)
	{
		_statNameTextBlock2->SetVisibility(ESlateVisibility::Visible);
		_statValueTextBlock2->SetVisibility(ESlateVisibility::Visible);

		FText statName;
		if (FGsStatHelper::GetDisPlayStatName(data->_statType2, statName))
		{
			_statNameTextBlock2->SetText(statName);
		}
		
		int32 value = data->_statDiff2._currentStatValue - data->_statDiff2._preStatValue;
		FText statValueText = GetStatValue(data->_statType2, value);
		_statValueTextBlock2->SetText(statValueText);
	}
	else
	{
		_statNameTextBlock2->SetVisibility(ESlateVisibility::Hidden);
		_statValueTextBlock2->SetVisibility(ESlateVisibility::Hidden);
	}
}

FText UGsLevelUpStatListViewEntry::GetStatValue(StatType inStatType, int32 inValue)
{
	FText statValueText;
	if (false == FGsStatHelper::GetDisPlayStatValueWithMarkFormat(inStatType, inValue, statValueText))
	{
		FString statName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), inStatType);
		GSLOG(Error, TEXT("Stat format not exist - stat name : %s"), *statName);
	}

	return FText::FromString(FString::Printf(TEXT("+%s"), *statValueText.ToString()));
}

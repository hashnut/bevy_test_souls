// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIItemSetStatListEntry.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "UI/UIContent/Common/Item/GsUIItemStatListEntry.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Item/Data/GsItemTableStruct.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "Item/Data/GsItemStatData.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

void UGsUIItemSetStatListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	int32 count = _panelStatListRoot->GetChildrenCount();
	for (int32 i = 0; i < count; ++i)
	{
		UWidget* widget = _panelStatListRoot->GetChildAt(i);
		if (UGsUIItemStatListEntry* listEntry = Cast<UGsUIItemStatListEntry>(widget))
		{
			_slotList.Emplace(listEntry);
		}
	}
}

void UGsUIItemSetStatListEntry::SetStatData(const TArray<FGsItemStatData>* InDataArray)
{
	const bool IsValidDataArray = (nullptr != InDataArray);

	for (int32 i = 0; i < _slotList.Num(); ++i)
	{
		UGsUIItemStatListEntry* slot = _slotList[i];
		if (nullptr == slot)
		{
			continue;
		}

		if (IsValidDataArray && InDataArray->IsValidIndex(i))
		{
			const FGsItemStatData& statData = (*InDataArray)[i];

			TPair<FText, FText> pairText;
			FGsStatHelper::GetDesplayStatNameAndValue(statData.type, statData.value, pairText, true);

			slot->SetVisibility(ESlateVisibility::HitTestInvisible);
			slot->SetTextName(pairText.Key);
			slot->SetTextValue(pairText.Value);

			continue;
		}

		slot->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIItemSetStatListEntry::SetTextTitle(const FText& InText)
{
	_textBlockTitle->SetText(InText);
}

void UGsUIItemSetStatListEntry::SetOnOff(bool bIsOn)
{
	SetColorAllText(FGsUIColorHelper::GetColor(bIsOn ?
		EGsUIColorType::DEFAULT_STAT_COLOR : EGsUIColorType::DISABLE_STAT_COLOR));
}

void UGsUIItemSetStatListEntry::SetColorAllText(const FLinearColor& InColor)
{
	_textBlockTitle->SetColorAndOpacity(InColor);

	for (UGsUIItemStatListEntry* slot : _slotList)
	{
		slot->SetColorAllText(InColor);
	}
}

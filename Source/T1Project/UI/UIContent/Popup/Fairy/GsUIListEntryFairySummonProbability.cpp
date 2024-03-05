// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/Fairy/GsUIListEntryFairySummonProbability.h"
#include "GsUIListDataFairySummonProbability.h"
#include "TextBlock.h"

#include "Fairy/GsFairyData.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

void UGsUIListEntryFairySummonProbability::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UGsUIListDataFairySummonProbability* item = Cast< UGsUIListDataFairySummonProbability>(ListItemObject))
	{
		if (auto fairyData = item->GetSummonChangeData()._fairyData.Pin())
		{
			_fairyNameText = fairyData->GetFairyName();
			_fairyNameColor = FGsUIColorHelper::GetColorFairyGrade(fairyData->GetFairyGrade());
		}

		_probabilityText = FText::AsNumber(item->GetSummonChangeData()._perProbability);
	}

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

}

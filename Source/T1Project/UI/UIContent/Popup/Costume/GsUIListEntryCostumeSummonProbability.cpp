// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/Costume/GsUIListEntryCostumeSummonProbability.h"
#include "GsUIListDataCostumeSummonProbability.h"
#include "TextBlock.h"

#include "Costume/GsCostumeData.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

void UGsUIListEntryCostumeSummonProbability::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UGsUIListDataCostumeSummonProbability* item = Cast< UGsUIListDataCostumeSummonProbability>(ListItemObject))
	{
		if (auto costumeData = item->GetSummonChangeData()._costumeData.Pin())
		{
			_costumeNameText = costumeData->GetCostumeName();
			_costumeNameColor = FGsUIColorHelper::GetColorGradeWidthItemGrade(costumeData->GetCostumeGrade());
		}

		_probabilityText = FText::AsNumber(item->GetSummonChangeData()._perProbability);
	}

	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

}

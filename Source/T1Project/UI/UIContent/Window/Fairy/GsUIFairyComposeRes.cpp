#include "GsUIFairyComposeRes.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"

void UGsUIFairyComposeRes::SetResult(FairyGrade preGrade, FairyGrade postGrade)
{
	SetItemResult(static_cast<ItemGrade>(preGrade), static_cast<ItemGrade>(postGrade));

	FLinearColor color = FGsUIColorHelper::GetColorFairyGrade(postGrade);
	_textPostGrade->SetColorAndOpacity(color);
}

void UGsUIFairyComposeRes::SetItemResult(ItemGrade preGrade, ItemGrade postGrade)
{
	FText findText[2];
	int index = 0;
	for (auto grade : { preGrade, postGrade })
	{
		FString gradeKey;
		switch (grade)
		{
		case ItemGrade::NORMAL:	gradeKey = TEXT("Grade_Normal");	break;
		case ItemGrade::MAGIC:	gradeKey = TEXT("Grade_Magic");		break;
		case ItemGrade::RARE:	gradeKey = TEXT("Grade_Rare");		break;
		case ItemGrade::UNIQUE:	gradeKey = TEXT("Grade_Unique");	break;
		case ItemGrade::EPIC:	gradeKey = TEXT("Grade_Epic");		break;
		case ItemGrade::LEGEND:	gradeKey = TEXT("Grade_Legend");	break;
		default: break;
		}

		if(!gradeKey.IsEmpty())
		FText::FindText(TEXT("ItemUIText"), gradeKey, findText[index]);

		index++;
	}

	_textPreGrade->SetText(findText[0]);
	_textPostGrade->SetText(findText[1]);
	_selectGrade->SetActiveWidgetIndex(static_cast<int>(postGrade));

	FLinearColor color = FGsUIColorHelper::GetColorItemGrade(postGrade);
	_textPostGrade->SetColorAndOpacity(color);
	
}


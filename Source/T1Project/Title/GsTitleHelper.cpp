

#include "GsTitleHelper.h"



bool FGsTitleHelper::FindText(const FTextKey& InKey, OUT FText& OutText)
{
	if (false == FText::FindText(TEXT("TitleText"), InKey, OutText))
	{
		OutText = FText::FromString(FString::Format(TEXT("UnKnow Message : {0}"), { InKey.GetChars() }));

		return false;
	}

	return true;
}


ETitleMainCategory FGsTitleHelper::ConvertEquipTypeToCategory(TitleEquipSlot InSlotType)
{
	ETitleMainCategory mainCategory = ETitleMainCategory::FORNT;
	if (TitleEquipSlot::BACK == InSlotType)
	{
		mainCategory = ETitleMainCategory::BACK;
	}
	return mainCategory;
}

TitleEquipSlot FGsTitleHelper::ConvertCategoryToEquipType(ETitleMainCategory InCategory)
{
	TitleEquipSlot equipSlot = TitleEquipSlot::MAX;
	
	if (ETitleMainCategory::FORNT == InCategory)
	{
		equipSlot = TitleEquipSlot::FRONT;
	}
	else if (ETitleMainCategory::BACK == InCategory)
	{
		equipSlot = TitleEquipSlot::BACK;
	}
	
	return equipSlot;
}
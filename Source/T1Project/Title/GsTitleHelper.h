#pragma once

/**
* file		GsTitleHelper.h
* @brief	Title Text Helper Class
* @author	PKT
* @date		2022/01/21
**/

#include "CoreMinimal.h"
#include "ETitleEnum.h"
#include "Shared/Client/SharedEnums/SharedTitleEnum.h"


struct T1PROJECT_API FGsTitleHelper
{
	static bool FindText(const FTextKey& InKey, OUT FText& OutText);

	// 2022/08/24 PKT - TitleEquipSlot -> ETitleMainCategory
	static ETitleMainCategory ConvertEquipTypeToCategory(TitleEquipSlot InSlotType);

	// 2022/08/24 PKT - ETitleMainCategory -> TitleEquipSlot
	static TitleEquipSlot ConvertCategoryToEquipType(ETitleMainCategory InCategory);
};
#pragma once

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Option/GsOptionValueDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
/*
* 옵션 관련 스태틱 함수
*/

class FGsOptionFunc
{
public:
	// 아이템 자동 획득 설정
	// ui 순서: 끄기, 매직이상, 레어이상, 모두획득
	// 서버 순서: 모두, 매직, 레어, off
	// 옵션->서버
	static AutoDropItemGrade ConvertAutoItemGainOptionEnumToServerEnum(int In_optionEnum);
	// 서버->옵션
	static int ConvertAutoItemGainServerEnumToOptionEnum(AutoDropItemGrade In_serverEnum);
	// 무게 옵션값에 현재 괜찮은가
	static bool IsPossibleLootByWeightOption(
		EGsOptionValueAutoItemLootWeightLimit In_weightOption, int32 In_lootWeight);

	static bool IsPossibleLootByWeightOption(int32 In_lootWeight);

	static bool IsPossibleLootByGradeOption(ItemType In_itemType, ItemGrade In_itemGrade);

	static bool IsPossibleNoneEquipItemLootByGradeOption(ItemId InItemId);

	static bool IsPossibleNoneEquipItemLootByGradeOption(const struct FGsSchemaItemCommon* InItemData);

	static bool IsPossibleNoneEquipItemLootByGradeOption(ItemCategorySub InItemCategorySub, ItemGrade InItemGrade);
};
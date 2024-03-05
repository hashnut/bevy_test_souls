#pragma once

#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"

// 별의 유산 슬롯 획득 가능한 아이템 정보
// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=396019674
class FGsStarLegacySlotItemInfo
{
public:
	// 획득 확률(%)
	float _acquisitionProbabilityPercentage;
	// 텍스트 정보(ex: 노멀 유산)
	FText _nameText;
	// 아이템 등급(텍스트 색상 표시용)
	ItemGrade _itemGrade;
	// 보상 data id
	int _rewardDataId;
};
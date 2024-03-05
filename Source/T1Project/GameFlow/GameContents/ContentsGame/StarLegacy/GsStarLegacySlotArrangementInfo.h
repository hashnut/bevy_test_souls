#pragma once

#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"

// 별의 유산 슬롯 배치 정보(ui상 노출된 아이콘 슬롯)
// https://jira.com2us.com/wiki/pages/viewpage.action?pageId=396019674

class FGsStarLegacySlotArrangementInfo
{
public:
	// 등급(ex:5등성)
	int _magnitude;
	// 아이템 id(slot 이미지 표시)
	int _itemId;
	// 아이템 수량
	int _itemCount;
	// 강화 수치
	int _enchantCount;
	// 슬롯 순서
	int _slotOrderIndex;
	// 아이템 타입
	ItemType _itemType;
};
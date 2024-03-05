#pragma once

#include "CoreMinimal.h"


/**
 * 2022/06/02 PKT - Window Dungeon 갱신때 본래 상태를 복원 하기 위해 현재 값을 캡쳐
 */
struct FGsDungeonUICaptureData
{
	int32 _toggleTabIndex = 0;				// 2022/06/14 PKT - window Tab
	int32 _toggleSlotIndex = 0;				// 2022/06/14 PKT - window Slot
	float _scrollBoxSlotOffset = 0.f;		// 2022/06/02 PKT - window Slot Scroll Offset

	int32 _lastDungeonIndex = 0;

	float _scrollBoxListOffset = 0.f;		// 2022/06/14 PKT - Detail UI List Scroll

	int32 _value0 = -1;						// 2022/08/18 PKT - 파티 던전 난이도
	int32 _mapId = 0;
};
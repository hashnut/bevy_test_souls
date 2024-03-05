#pragma once

#include "EMonsterStoneSlateTreeNodeCoordinate.generated.h"

// 2021/11/10 PKT - 몬스터 지식의 석판 트리에서 Node 위치의 좌표를 네이밍
UENUM(BlueprintType)
enum class EMonsterStoneSlateTreeNodeCoordinate : uint8
{
	Left		UMETA(DisplayName = "Left"),
	Center		UMETA(DisplayName = "Center"),
	Right		UMETA(DisplayName = "Right"),

	Max			UMETA(DisplayName = "Max"),
};

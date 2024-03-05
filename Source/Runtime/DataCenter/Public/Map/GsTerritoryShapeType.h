#pragma once

#include "GsTerritoryShapeType.generated.h"

/*
	맵 아이콘 타입 이넘
*/
UENUM(BlueprintType)
enum class EGsTerritoryShapeType : uint8
{
	Line,
	Circle,
	Polygon,
	Sqaure,
	Max,
};
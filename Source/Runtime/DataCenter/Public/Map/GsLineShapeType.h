#pragma once

#include "GsLineShapeType.generated.h"

/*
	맵 아이콘 타입 이넘
*/
UENUM(BlueprintType)
enum class EGsLineShapeType : uint8
{
	Straight,
	Circle,
	Line,
	Max,
};
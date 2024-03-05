#pragma once

#include "EItemCollectionSubCategoryStatUnit.generated.h"

/**
 * 서브 카테고리 정의 - 분류별
 */
UENUM(BlueprintType)
enum class EItemCollectionSubCategoryStatUnit : uint8
{
	None = 0,
	ALL,
	Default,
	ShotDistance,
	LongDistance,
	Magic,
	Attack,
	Defence,
	Etc,

	Max,
};

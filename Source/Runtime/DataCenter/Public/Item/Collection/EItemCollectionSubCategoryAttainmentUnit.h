#pragma once

#include "EItemCollectionSubCategoryAttainmentUnit.generated.h"

/**
 * 서브 카테고리 정의 - 난이도별
 */
UENUM(BlueprintType)
enum class EItemCollectionSubCategoryAttainmentUnit : uint8
{
	None = 0,
	ALL,
	Step1,
	Step2,
	Step3,
	Step4,
	Step5,

	Max,
};
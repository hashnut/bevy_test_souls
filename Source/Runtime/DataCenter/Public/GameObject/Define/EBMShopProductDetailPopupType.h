#pragma once

#include "EBMShopProductDetailPopupType.generated.h"

UENUM(BlueprintType)
enum class EBMShopProductDetailPopupType : uint8
{
	ContractGuideType = 0, // 삭제 예정
	OnlyPriceType,
	WithCalculatorType,
};
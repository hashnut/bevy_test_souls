#pragma once

#include "EGsDisPlayFormatType.generated.h"

UENUM(BlueprintType)
enum class EGsDisPlayFormatType : uint8
{
	Format_Normal = 0, 
	Format_RateInHundred ,
	Format_RateInTenThousand ,
	//소수점 두자리 실수(MAX_WEIGHT 같은 스탯을 표현하기 위한 포맷)
	Format_RealNumberWithTwoDecimalPointDigit	
};
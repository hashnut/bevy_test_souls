#pragma once

#include "EGsProjectileApplyTargetType.generated.h"

UENUM(BlueprintType)
enum class EGsProjectileApplyTargetType : uint8
{
	Enemy = 0,		// 공격 적중 활성 옵션에 따른 공격가능 대상 적용
	Ally,			// Party, Guild 대상 적용
	All,			// Enemy + Ally 대상 적용
};
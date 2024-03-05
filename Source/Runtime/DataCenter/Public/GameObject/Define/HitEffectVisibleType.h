#pragma once

#include "HitEffectVisibleType.generated.h"

/*
  출력기준 : 한번에 전부, 랜덤픽 1개
*/
UENUM()
enum class HitEffectVisibleType : uint8
{
	ALL = 0,
	RANDOM,
	MAX,
};

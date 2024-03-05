#pragma once

#include "HitEffectAttachType.generated.h"

/*
  부착위치 : 소켓, 위치, 소켓기준 바운드박스 + 시전자 방향 고려
*/
UENUM(BlueprintType)
enum class HitEffectAttachType : uint8
{
	SOCKET = 0,
	LOCATION,
	TARGET_ROTATION,
	SOCKET_BOUNDING_DIRECTION,
	SOCKET_RANDOM_BOUNDING,
	MAX,
};
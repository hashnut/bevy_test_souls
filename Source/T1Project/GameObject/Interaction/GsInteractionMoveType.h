#pragma once

#include "GsInteractionMoveType.generated.h"

/*
	인터랙션 이동 타입
*/

UENUM(BlueprintType)
enum class EGsInteractionMoveType : uint8
{
	PosMove,				// 위치 이동
	TargetMove,				// 타겟 이동
	None,					// 없음
};

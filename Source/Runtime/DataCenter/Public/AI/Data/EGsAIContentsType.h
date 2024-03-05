#pragma once

#include "EGsAIContentsType.generated.h"

// ai 컨텐츠별 타입
UENUM(BlueprintType)
enum class EGsAIContentsType : uint8
{
	Field,							// 일반 필드
	Auto_off,						// 오토 꺼진 상태
};

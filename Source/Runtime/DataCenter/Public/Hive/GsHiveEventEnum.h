#pragma once

UENUM(BlueprintType)
enum class EGsHiveActionType : uint8
{
	PURCHASE,		// 구매 관련 이벤트
	ATTENDANCE,		// 출석 관련 이벤트
	SPECIAL,		// 단발성 이벤트

	MAX,
};
ENUM_RANGE_BY_COUNT(EGsHiveActionType, EGsHiveActionType::MAX);
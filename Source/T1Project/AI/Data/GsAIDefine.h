#pragma once

#include "CoreMinimal.h"

// 제자리 돌아오는 타입
enum class EGsAIAutoReturnStartPosType : uint8
{
	Dist20Meter,		// 20 미터
	Dist30Meter,		// 30 미터
	Dist50Meter,		// 50 미터
	NoLimit,			// 제한 없음
};

// 아이템 줍기 최소 등급 타입
enum class EGsAIAutoLootDropItemGradeType :uint8
{
	All,				// 다주움
	OverMagic,			// 매직등급 이상
	OverRare,			// 레어등급 이상
	NotLoot,			// 줍지않음
};
// ai 전투 옵션: 매너 모드
enum class EGsAIBattleOptionMannerMode :uint8
{
	ON				=	0,		// ON
	ALLY_EXCEPTION	=	1,		// 파티, 기사단 예외
	OFF				=	2,		// OFF
};

enum class EGsAIDelayType : uint8
{
	NONE				= 0,
	MAKE_JOB_DELAY		= 1,
	RETRY_DELAY			= 2,
};
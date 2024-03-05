#pragma once

#include "HitEffectIgnoreType.generated.h"

/*
 히트 이펙트 추가 사용 타입 (이 타입은 Skillnotify 전용 타입이라고 보면 됩니다.)
*/
UENUM(BlueprintType)
enum class HitEffectIgnoreType : uint8
{
	NONE = 0,			// 조건 없음 (있는정보 모두 출력)
	IGNORE_PARENT,		// 상위에 존재하는 HitEffect출력 무시
	IGNORE_MYSELP,		// SkillNotifySet에 존재하는 HitEffect 출력 무시 (필요한지는 모르겠지만)
};
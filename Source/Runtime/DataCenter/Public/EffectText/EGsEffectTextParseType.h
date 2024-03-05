#pragma once

#include "EGsEffectTextParseType.generated.h"

/**
 * 효과 텍스트 타입
 */
UENUM(BlueprintType)
enum class EGsEffectTextParseType : uint8
{
	NONE,
	ABNORMALITY_DEFAULT, // 이름만 출력
	ABNORMALITY_STAT, // Abnormality 스탯 출력
	ABNORMALITY_DURATION_ONLY, // 이름과 지속 시간 출력
	ABNORMALITY_OVERTIME, // 오버타임 타입. 필요 시 세부 분기

	PASSIVITY_DEFAULT, // 패시비티 기본/예외사항(effectText)
	PASSIVITY_STAT, // Passivity 스탯 출력
	PASSIVITY_INVOKE_ABNORMALITY, // AbnormalityId를 통해 Abnormality 텍스트 출력
	PASSIVITY_INVOKE_SKILL_NOTIFY,	// SkillNotifyId를 통해 SkillNotify 텍스트 출력
	PASSIVITY_SKILL_UP, // 특정 스킬 레벨을 올리는 패시비티 타입

	SKILL_NOTIFY_DEFAULT, // 스킬 노티파이 기본/예외사항(effectText)
	SKILL_NOTIFY_DAMAGE, // 노티파이 데미지 출력
	SKILL_NOTIFY_ABNORMALITY, // AbnormalityId를 통해 Abnormality 텍스트 출력
	SKILL_NOTIFY_HEAL, // 노티파이 힐정보 출력
	SKILL_NOTIFY_ABSORPTION, // 피해량의 n% HP로 흡수 텍스트 출력
	SKILL_NOTIFY_PROJECTILE, // 발사체

	SKILL_LEVEL_DEFAULT, // 스킬 기본/예외사항(effectText)
	SKILL_LEVEL_DAMAGE, // 스킬 데미지 출력(여러 노티 종합)
	SKILL_LEVEL_NOTIFY_LIST,

	SKILL_PASSIVITY_LIST, // 패시브 스킬(레벨 정보 없음)
	SKILL_EXE_CONDITION_ABNORMALITY, // 스킬 발동 조건 Abnormality
	SKILL_EXE_CONDITION_STAT, // 스킬 발동 조건 Stat
	SKILL_EXE_CONDITION_PASSIVITY, // 스킬 발동 조건 Passivity

	MAX,
};

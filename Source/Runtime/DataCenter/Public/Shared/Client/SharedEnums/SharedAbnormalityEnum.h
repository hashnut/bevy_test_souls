#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedAbnormalityEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
// 이상상태 적용 타입
enum class AbnormalityStackType : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// 갱신(중첩X)
	RENEWAL = 1,

	// 갱신되면서 중첩(GroupId가 기준)
	RENEWAL_STACK = 2,

	// 병렬(중첩X)
	PARALLEL = 3,

	// 병렬되면서 중첩(GroupID 기준)
	PARALLEL_STACK = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 이상상태 효과 서브 타입
enum class AbnormalityEffectMainType : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// DOT(damage over time), HOT(Heal over time)
	OVERTIME = 1,

	// CC : Crowd Control
	CC = 2,

	// STAT 가감
	STAT = 3,

	// UTIL
	UTIL = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 이상상태 효과 타입
enum class AbnormalityEffectType : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// 출혈, 중독, 화상 (OT:Over Time, DOT:damage over time)
	OVERTIME_DAMAGE = 1,

	// 지속 회복 (OT:Over Time, HOT: Heal Over Time)
	OVERTIME_HOT = 2,

	// 살기 획득 (OT:Over Time)
	OVERTIME_SENSITIVE = 3,

	// 기절, 고치, 빙결, 뿌리묶기, 넉다운 (CC:Crowd Control)
	CC_STUN = 4,

	// 침묵 (CC:Crowd Control)
	CC_SILENCE = 5,

	// 속박 (CC:Crowd Control)
	CC_BIND = 6,

	// 워프할 수 없음 (CC:Crowd Control)
	CC_CANNOT_WARP = 7,

	// 매몰 (CC:Crowd Control)
	CC_BURY = 8,

	// 넉백 (CC:Crowd Control, Reaction:Root Motion)
	CC_KNOCK_BACK = 9,

	// 공포 (CC:Crowd Control)
	CC_FEAR = 10,

	// 도발 (CC:Crowd Control)
	CC_TAUNT = 11,

	// 위치이동 - 타겟의 위치이동을 위함 (CC:Crowd Control) : 참고:SkillNotifyMoveLocation(캐스터 한정)과 동일한 기능
	CC_MOVE_LOCATION = 12,

	// 변이 (CC: Crowd Control)
	CC_POLYMORP = 13,

	// 봉쇄 (CC: Crowd Control)
	CC_BLOCKADE = 14,

	// 스탯 가감 : + SLOW
	STAT = 15,

	// STAT과 연출 분리 : 대쉬 (스탯 가감)
	STAT_DASH = 16,

	// STAT과 연출 분리 : IMMUNE, 이뮨, 면역 (스탯 가감)
	STAT_IMMUNE = 17,

	// STAT과 연출 분리 : 살기 폭발 (공격력 증가 - 스탯 가감)
	STAT_VENT = 18,

	// Abnormality가 제거 될 때, SkillNotify 실행 (Util)
	UTIL_ON_REMOVE_EXE_SKILL_NOTIFY = 19,

	// AI의 Phase 전환 하지 않는 효과(ex.리자드맨 킹 페이즈 전환 연출, 점프 공격 간 무적 처리)
	UTIL_NOT_AI_CHANGE_PHASE = 20,

	// 불사 처리. 현재 HP가 특정 수치이하로 떨어지지 않음
	UTIL_IMMORTAL = 21,

	// 은신 : class AbnormalityEffectHiding.h 의 설명
	UTIL_HIDING = 22,

	// 폭주
	UTIL_BERSERKER_RAGE = 23,

	// 뾰족 화살 : 해당 이상상태 적용 중일때 타격 시 타겟에게 이상상태 부여
	UTIL_POINTED_ARROW = 24,

	// 쐐기 화살 : 해당 이상상태 적용 중일때 타격 시 타겟에게 이상상태 부여
	UTIL_WEDGE_ARROW = 25,

	// 톱니 화살 : 해당 이상상태 적용 중일때 타격 시 타겟에게 이상상태 부여
	UTIL_JAGGED_ARROW = 26,

	// 파이널 하울(불사 + 받는 대미지 비율로 감소)
	UTIL_FINAL_HOWL = 27,

	// 주는 대미지 비율로 조작
	UTIL_BONUS_DAMAGE = 28,

	// 반격
	UTIL_BUFF_COUNTER = 29,

	// 매몰과 같음 - 타겟 가능 하게 하기 위하여 분리 (행동제약을 사용하지 않을거라서, CC가 아님, 추후 CC_BURY도 UTIL로 변경 가능성 있음)
	UTIL_BURY_KEEP_TARGET = 30,

	// 컨퓨즈 : HP/MP HUD 불가시화
	UTIL_CONFUSE = 31,

	// 대미지 공유 : 상태이상을 지닌 오너가 받은 대미지를 캐스터에게 분배
	UTIL_DAMAGE_SHARE = 32,

	// 추가될 때 SpaceCrackConfig 의 deponMonsterList 에 지정된 NpcId 및 스탯 타입과 전달 비율에 따라 NPC 생성
	UTIL_CREATE_DEPON_NPC = 33,

	// 해당 효과를 보유한 시전자가 대상에게 대미지를 입힐 시 HP를 흡혈한다
	UTIL_HP_ABSORPTION = 34,

	// 탈 것 상태(탈 것 적용 여부를 판단하기 위한 것임으로 아무런 기능을 하지 않음)
	UTIL_MOUNT_VEHICLE = 35,

	// 피격될 때, SkillNotify 실행 (Util)
	UTIL_ON_HIT_SKILL_NOTIFY = 36,

	MAX = 37,
};

UENUM(BlueprintType)
// 이상상태 효과 적용시 수치 가감 방식
enum class AbnormalityEffectAddSubMethodType : uint8
{

	// 단순 증감 (더하기 빼기)
	ADD_SUB = 0,

	// 자원 최대치로 증감 (Max HP/MP)
	RESOURCE_RATE = 1,

	// 캐스터 StatType::ATTACK_??? 의 비율만큼을 증감 - 삭제 예정
	CASTER_ATTACK_RATE = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 이상상태 효과-쉴드의 쉴드량 계산 타입
enum class AbnormalityShieldCalcType : uint8
{

	// caster의 공격력을 참조
	POWER_RATE = 0,

	// 최대 체력을 참조
	MAX_HP = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 이상상태 제거 트리거 방식
enum class AbnormalityRemoveTriggerCondition : uint8
{

	// 시간 만료
	EXPIRED = 0,

	// 자원 만료
	RESOURCE_EXHAUSTION = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 넉백 방향 타입
enum class AbnormalityKnockBackDirectionType : uint8
{

	// 시전자 중점 부터 타겟방향
	Caster = 0,

	// 판정 중점 부터 타겟 방향으로 방사형 
	Radial = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 이상상태 제거 조건 타입
enum class AbnormalityRemoveConditionType : uint8
{

	// 로그아웃 시 제거 (false이면, DB에 저장해서, 제접해도 버프유지)
	LOGOUT = 0,

	// 죽으면 제거
	DEATH = 1,

	// 지속시간 초과면 제거
	DURATION_TIME = 2,

	// 설정된 무기가 장착되지 않으면 제거
	REQUIRE_WEAPON = 3,

	// 전투 시작 시 제거
	START_COMBAT = 4,

	// 데미지 받으면 제거
	TAKE_DAMAGE = 5,

	// 힐 받으면 제거
	TAKE_HEAL = 6,

	// 다운 리액션 당하면 제거
	TAKE_REACTION_DOWN = 7,

	// 넉백 리액션 당하면 제거
	TAKE_REACTION_KNOCKBACK = 8,

	// 에어본 리액션 당하면 제거
	TAKE_REACTION_AIRBORNE = 9,

	// 스킬 사용하면 제거
	USE_SKILL = 10,

	// 액티브 스킬 사용하면 제거
	USE_ACTIVE_SKILL = 11,

	// 액티브 스킬 사용이 끝나면 제거(단, 자신을 생성한 스킬은 제외)
	USE_ACTIVE_SKILL_END = 12,

	// 아이템 사용하면 제거
	USE_ITEM = 13,

	// 인터랙션시 제거
	INTERACTION = 14,

	// 디스폰 시 제거 (환경 속성, 개인 채널 스폰 금지 해제 용도)
	DESPAWN = 15,

	// 시스템에서 강제로 삭제 가능한가? : ex)해로운 모든이상상태 제거 에서 사용 : true이면 제거 안됨
	SYSTEM_FORCE_NOT_REMOVEABLE = 16,

	// 보호막이 0이되면 제거
	CONSUME_HP_SHIELD = 17,

	// 착용 페어리 교체 시 제거
	CHANGE_FAIRY = 18,

	// 회귀 상태가 되었을 때 제거
	AI_RETURN_STATE = 19,

	// 안전 지대에 위치할 경우 해당 상태이상 제거(TOWN CHAOS + 침공전 끝났을 때)
	IN_TOWN = 20,

	// 프리셋 교체 시 제거
	CHANGE_PRESET = 21,

	MAX = 22,
};

UENUM(BlueprintType)
// 
 /*
 스탯형 이상상태 발동 타입 
 AbnormalityEffectType::STAT - param1:AbnormalityEffectStatMethodType 
*/
enum class AbnormalityEffectStatMethodType : uint8
{

	// 기본값 - 지정된 스텟에 지정된 수치 반영
	NORMAL = 0,

	// 비례 - 특정 스탯에 비례하여 지정된 스탯의 유동적인 수치 반영
	PROPORTION = 1,

	// 잔여 체력 비율에 비례하여 지정된 스탯의 유동적 수치 반영
	LESS_HP_RATE = 2,

	// 상승되는 스탯 고정 수치만큼 현재 HP 회복
	NORMAL_WITH_HP_ON_ACTIVE = 3,

	// 상승되는 스탯 비례 수치만큼 현재 HP 회복
	PROPORTION_WITH_HP_ON_ACTIVE = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 
 /*
 반격 - Param 1 
 AbnormalityEffectType::UTIL_BUFF_COUNTER - param1:AbnormalityEffectBuffCountHitType 
*/
enum class AbnormalityEffectBuffCountHitType : uint8
{

	// 평타 피격시
	HIT_SELF_NORMAL = 0,

	// 모든 스킬 피격시
	HIT_SELF_SKILL = 1,

	// 모든 공격 피격시
	HIT_SELF = 2,

	// 모든 공격 피격 또는 회피 시
	HIT_OR_DODGE_SELF = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 
 /*
 반격 - Param 1 
 AbnormalityEffectType::UTIL_BUFF_COUNTER - param1:AbnormalityEffectBuffCountHitType 
*/
enum class AbnormalityEffectBuffCountSkillGrade : uint8
{

	// 에러값
	NONE = 0,

	// 등급 - NORMAL
	NORMAL = 1,

	// 등급 - MAGIC 보다 아래
	BLOW_MAGIC = 2,

	// 등급 - RARE 보다 아래
	BLOW_RARE = 3,

	// 등급 - EPIC 보다 아래
	BLOW_EPIC = 4,

	// 등급 - UNIQUE 보다 아래
	BLOW_UNIQUE = 5,

	// 등급 - LEGEND 보다 아래
	BLOW_LEGEND = 6,

	MAX = 7,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline AbnormalityStackType AbnormalityStackTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityStackType::NONE;
	if (str.Compare(TEXT("RENEWAL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityStackType::RENEWAL;
	if (str.Compare(TEXT("RENEWAL_STACK"), ESearchCase::IgnoreCase) == 0)	return AbnormalityStackType::RENEWAL_STACK;
	if (str.Compare(TEXT("PARALLEL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityStackType::PARALLEL;
	if (str.Compare(TEXT("PARALLEL_STACK"), ESearchCase::IgnoreCase) == 0)	return AbnormalityStackType::PARALLEL_STACK;
	return AbnormalityStackType::MAX;
}
inline AbnormalityEffectMainType AbnormalityEffectMainTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectMainType::NONE;
	if (str.Compare(TEXT("OVERTIME"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectMainType::OVERTIME;
	if (str.Compare(TEXT("CC"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectMainType::CC;
	if (str.Compare(TEXT("STAT"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectMainType::STAT;
	if (str.Compare(TEXT("UTIL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectMainType::UTIL;
	return AbnormalityEffectMainType::MAX;
}
inline AbnormalityEffectType AbnormalityEffectTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::NONE;
	if (str.Compare(TEXT("OVERTIME_DAMAGE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::OVERTIME_DAMAGE;
	if (str.Compare(TEXT("OVERTIME_HOT"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::OVERTIME_HOT;
	if (str.Compare(TEXT("OVERTIME_SENSITIVE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::OVERTIME_SENSITIVE;
	if (str.Compare(TEXT("CC_STUN"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_STUN;
	if (str.Compare(TEXT("CC_SILENCE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_SILENCE;
	if (str.Compare(TEXT("CC_BIND"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_BIND;
	if (str.Compare(TEXT("CC_CANNOT_WARP"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_CANNOT_WARP;
	if (str.Compare(TEXT("CC_BURY"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_BURY;
	if (str.Compare(TEXT("CC_KNOCK_BACK"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_KNOCK_BACK;
	if (str.Compare(TEXT("CC_FEAR"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_FEAR;
	if (str.Compare(TEXT("CC_TAUNT"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_TAUNT;
	if (str.Compare(TEXT("CC_MOVE_LOCATION"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_MOVE_LOCATION;
	if (str.Compare(TEXT("CC_POLYMORP"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_POLYMORP;
	if (str.Compare(TEXT("CC_BLOCKADE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::CC_BLOCKADE;
	if (str.Compare(TEXT("STAT"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::STAT;
	if (str.Compare(TEXT("STAT_DASH"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::STAT_DASH;
	if (str.Compare(TEXT("STAT_IMMUNE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::STAT_IMMUNE;
	if (str.Compare(TEXT("STAT_VENT"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::STAT_VENT;
	if (str.Compare(TEXT("UTIL_ON_REMOVE_EXE_SKILL_NOTIFY"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_ON_REMOVE_EXE_SKILL_NOTIFY;
	if (str.Compare(TEXT("UTIL_NOT_AI_CHANGE_PHASE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_NOT_AI_CHANGE_PHASE;
	if (str.Compare(TEXT("UTIL_IMMORTAL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_IMMORTAL;
	if (str.Compare(TEXT("UTIL_HIDING"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_HIDING;
	if (str.Compare(TEXT("UTIL_BERSERKER_RAGE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_BERSERKER_RAGE;
	if (str.Compare(TEXT("UTIL_POINTED_ARROW"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_POINTED_ARROW;
	if (str.Compare(TEXT("UTIL_WEDGE_ARROW"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_WEDGE_ARROW;
	if (str.Compare(TEXT("UTIL_JAGGED_ARROW"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_JAGGED_ARROW;
	if (str.Compare(TEXT("UTIL_FINAL_HOWL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_FINAL_HOWL;
	if (str.Compare(TEXT("UTIL_BONUS_DAMAGE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_BONUS_DAMAGE;
	if (str.Compare(TEXT("UTIL_BUFF_COUNTER"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_BUFF_COUNTER;
	if (str.Compare(TEXT("UTIL_BURY_KEEP_TARGET"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_BURY_KEEP_TARGET;
	if (str.Compare(TEXT("UTIL_CONFUSE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_CONFUSE;
	if (str.Compare(TEXT("UTIL_DAMAGE_SHARE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_DAMAGE_SHARE;
	if (str.Compare(TEXT("UTIL_CREATE_DEPON_NPC"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_CREATE_DEPON_NPC;
	if (str.Compare(TEXT("UTIL_HP_ABSORPTION"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_HP_ABSORPTION;
	if (str.Compare(TEXT("UTIL_MOUNT_VEHICLE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_MOUNT_VEHICLE;
	if (str.Compare(TEXT("UTIL_ON_HIT_SKILL_NOTIFY"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectType::UTIL_ON_HIT_SKILL_NOTIFY;
	return AbnormalityEffectType::MAX;
}
inline AbnormalityEffectAddSubMethodType AbnormalityEffectAddSubMethodTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ADD_SUB"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectAddSubMethodType::ADD_SUB;
	if (str.Compare(TEXT("RESOURCE_RATE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectAddSubMethodType::RESOURCE_RATE;
	if (str.Compare(TEXT("CASTER_ATTACK_RATE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectAddSubMethodType::CASTER_ATTACK_RATE;
	return AbnormalityEffectAddSubMethodType::MAX;
}
inline AbnormalityShieldCalcType AbnormalityShieldCalcTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("POWER_RATE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityShieldCalcType::POWER_RATE;
	if (str.Compare(TEXT("MAX_HP"), ESearchCase::IgnoreCase) == 0)	return AbnormalityShieldCalcType::MAX_HP;
	return AbnormalityShieldCalcType::MAX;
}
inline AbnormalityRemoveTriggerCondition AbnormalityRemoveTriggerConditionStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("EXPIRED"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveTriggerCondition::EXPIRED;
	if (str.Compare(TEXT("RESOURCE_EXHAUSTION"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveTriggerCondition::RESOURCE_EXHAUSTION;
	return AbnormalityRemoveTriggerCondition::MAX;
}
inline AbnormalityKnockBackDirectionType AbnormalityKnockBackDirectionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("Caster"), ESearchCase::IgnoreCase) == 0)	return AbnormalityKnockBackDirectionType::Caster;
	if (str.Compare(TEXT("Radial"), ESearchCase::IgnoreCase) == 0)	return AbnormalityKnockBackDirectionType::Radial;
	return AbnormalityKnockBackDirectionType::MAX;
}
inline AbnormalityRemoveConditionType AbnormalityRemoveConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("LOGOUT"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::LOGOUT;
	if (str.Compare(TEXT("DEATH"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::DEATH;
	if (str.Compare(TEXT("DURATION_TIME"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::DURATION_TIME;
	if (str.Compare(TEXT("REQUIRE_WEAPON"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::REQUIRE_WEAPON;
	if (str.Compare(TEXT("START_COMBAT"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::START_COMBAT;
	if (str.Compare(TEXT("TAKE_DAMAGE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::TAKE_DAMAGE;
	if (str.Compare(TEXT("TAKE_HEAL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::TAKE_HEAL;
	if (str.Compare(TEXT("TAKE_REACTION_DOWN"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::TAKE_REACTION_DOWN;
	if (str.Compare(TEXT("TAKE_REACTION_KNOCKBACK"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::TAKE_REACTION_KNOCKBACK;
	if (str.Compare(TEXT("TAKE_REACTION_AIRBORNE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::TAKE_REACTION_AIRBORNE;
	if (str.Compare(TEXT("USE_SKILL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::USE_SKILL;
	if (str.Compare(TEXT("USE_ACTIVE_SKILL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::USE_ACTIVE_SKILL;
	if (str.Compare(TEXT("USE_ACTIVE_SKILL_END"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::USE_ACTIVE_SKILL_END;
	if (str.Compare(TEXT("USE_ITEM"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::USE_ITEM;
	if (str.Compare(TEXT("INTERACTION"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::INTERACTION;
	if (str.Compare(TEXT("DESPAWN"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::DESPAWN;
	if (str.Compare(TEXT("SYSTEM_FORCE_NOT_REMOVEABLE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::SYSTEM_FORCE_NOT_REMOVEABLE;
	if (str.Compare(TEXT("CONSUME_HP_SHIELD"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::CONSUME_HP_SHIELD;
	if (str.Compare(TEXT("CHANGE_FAIRY"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::CHANGE_FAIRY;
	if (str.Compare(TEXT("AI_RETURN_STATE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::AI_RETURN_STATE;
	if (str.Compare(TEXT("IN_TOWN"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::IN_TOWN;
	if (str.Compare(TEXT("CHANGE_PRESET"), ESearchCase::IgnoreCase) == 0)	return AbnormalityRemoveConditionType::CHANGE_PRESET;
	return AbnormalityRemoveConditionType::MAX;
}
inline AbnormalityEffectStatMethodType AbnormalityEffectStatMethodTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectStatMethodType::NORMAL;
	if (str.Compare(TEXT("PROPORTION"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectStatMethodType::PROPORTION;
	if (str.Compare(TEXT("LESS_HP_RATE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectStatMethodType::LESS_HP_RATE;
	if (str.Compare(TEXT("NORMAL_WITH_HP_ON_ACTIVE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectStatMethodType::NORMAL_WITH_HP_ON_ACTIVE;
	if (str.Compare(TEXT("PROPORTION_WITH_HP_ON_ACTIVE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectStatMethodType::PROPORTION_WITH_HP_ON_ACTIVE;
	return AbnormalityEffectStatMethodType::MAX;
}
inline AbnormalityEffectBuffCountHitType AbnormalityEffectBuffCountHitTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("HIT_SELF_NORMAL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountHitType::HIT_SELF_NORMAL;
	if (str.Compare(TEXT("HIT_SELF_SKILL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountHitType::HIT_SELF_SKILL;
	if (str.Compare(TEXT("HIT_SELF"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountHitType::HIT_SELF;
	if (str.Compare(TEXT("HIT_OR_DODGE_SELF"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountHitType::HIT_OR_DODGE_SELF;
	return AbnormalityEffectBuffCountHitType::MAX;
}
inline AbnormalityEffectBuffCountSkillGrade AbnormalityEffectBuffCountSkillGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountSkillGrade::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountSkillGrade::NORMAL;
	if (str.Compare(TEXT("BLOW_MAGIC"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountSkillGrade::BLOW_MAGIC;
	if (str.Compare(TEXT("BLOW_RARE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountSkillGrade::BLOW_RARE;
	if (str.Compare(TEXT("BLOW_EPIC"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountSkillGrade::BLOW_EPIC;
	if (str.Compare(TEXT("BLOW_UNIQUE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountSkillGrade::BLOW_UNIQUE;
	if (str.Compare(TEXT("BLOW_LEGEND"), ESearchCase::IgnoreCase) == 0)	return AbnormalityEffectBuffCountSkillGrade::BLOW_LEGEND;
	return AbnormalityEffectBuffCountSkillGrade::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString AbnormalityStackTypeEnumToString(AbnormalityStackType value)
{
	switch (value)
	{
		case AbnormalityStackType::NONE: return TEXT("NONE");
		case AbnormalityStackType::RENEWAL: return TEXT("RENEWAL");
		case AbnormalityStackType::RENEWAL_STACK: return TEXT("RENEWAL_STACK");
		case AbnormalityStackType::PARALLEL: return TEXT("PARALLEL");
		case AbnormalityStackType::PARALLEL_STACK: return TEXT("PARALLEL_STACK");
	}
	return FString();
}
inline FString AbnormalityEffectMainTypeEnumToString(AbnormalityEffectMainType value)
{
	switch (value)
	{
		case AbnormalityEffectMainType::NONE: return TEXT("NONE");
		case AbnormalityEffectMainType::OVERTIME: return TEXT("OVERTIME");
		case AbnormalityEffectMainType::CC: return TEXT("CC");
		case AbnormalityEffectMainType::STAT: return TEXT("STAT");
		case AbnormalityEffectMainType::UTIL: return TEXT("UTIL");
	}
	return FString();
}
inline FString AbnormalityEffectTypeEnumToString(AbnormalityEffectType value)
{
	switch (value)
	{
		case AbnormalityEffectType::NONE: return TEXT("NONE");
		case AbnormalityEffectType::OVERTIME_DAMAGE: return TEXT("OVERTIME_DAMAGE");
		case AbnormalityEffectType::OVERTIME_HOT: return TEXT("OVERTIME_HOT");
		case AbnormalityEffectType::OVERTIME_SENSITIVE: return TEXT("OVERTIME_SENSITIVE");
		case AbnormalityEffectType::CC_STUN: return TEXT("CC_STUN");
		case AbnormalityEffectType::CC_SILENCE: return TEXT("CC_SILENCE");
		case AbnormalityEffectType::CC_BIND: return TEXT("CC_BIND");
		case AbnormalityEffectType::CC_CANNOT_WARP: return TEXT("CC_CANNOT_WARP");
		case AbnormalityEffectType::CC_BURY: return TEXT("CC_BURY");
		case AbnormalityEffectType::CC_KNOCK_BACK: return TEXT("CC_KNOCK_BACK");
		case AbnormalityEffectType::CC_FEAR: return TEXT("CC_FEAR");
		case AbnormalityEffectType::CC_TAUNT: return TEXT("CC_TAUNT");
		case AbnormalityEffectType::CC_MOVE_LOCATION: return TEXT("CC_MOVE_LOCATION");
		case AbnormalityEffectType::CC_POLYMORP: return TEXT("CC_POLYMORP");
		case AbnormalityEffectType::CC_BLOCKADE: return TEXT("CC_BLOCKADE");
		case AbnormalityEffectType::STAT: return TEXT("STAT");
		case AbnormalityEffectType::STAT_DASH: return TEXT("STAT_DASH");
		case AbnormalityEffectType::STAT_IMMUNE: return TEXT("STAT_IMMUNE");
		case AbnormalityEffectType::STAT_VENT: return TEXT("STAT_VENT");
		case AbnormalityEffectType::UTIL_ON_REMOVE_EXE_SKILL_NOTIFY: return TEXT("UTIL_ON_REMOVE_EXE_SKILL_NOTIFY");
		case AbnormalityEffectType::UTIL_NOT_AI_CHANGE_PHASE: return TEXT("UTIL_NOT_AI_CHANGE_PHASE");
		case AbnormalityEffectType::UTIL_IMMORTAL: return TEXT("UTIL_IMMORTAL");
		case AbnormalityEffectType::UTIL_HIDING: return TEXT("UTIL_HIDING");
		case AbnormalityEffectType::UTIL_BERSERKER_RAGE: return TEXT("UTIL_BERSERKER_RAGE");
		case AbnormalityEffectType::UTIL_POINTED_ARROW: return TEXT("UTIL_POINTED_ARROW");
		case AbnormalityEffectType::UTIL_WEDGE_ARROW: return TEXT("UTIL_WEDGE_ARROW");
		case AbnormalityEffectType::UTIL_JAGGED_ARROW: return TEXT("UTIL_JAGGED_ARROW");
		case AbnormalityEffectType::UTIL_FINAL_HOWL: return TEXT("UTIL_FINAL_HOWL");
		case AbnormalityEffectType::UTIL_BONUS_DAMAGE: return TEXT("UTIL_BONUS_DAMAGE");
		case AbnormalityEffectType::UTIL_BUFF_COUNTER: return TEXT("UTIL_BUFF_COUNTER");
		case AbnormalityEffectType::UTIL_BURY_KEEP_TARGET: return TEXT("UTIL_BURY_KEEP_TARGET");
		case AbnormalityEffectType::UTIL_CONFUSE: return TEXT("UTIL_CONFUSE");
		case AbnormalityEffectType::UTIL_DAMAGE_SHARE: return TEXT("UTIL_DAMAGE_SHARE");
		case AbnormalityEffectType::UTIL_CREATE_DEPON_NPC: return TEXT("UTIL_CREATE_DEPON_NPC");
		case AbnormalityEffectType::UTIL_HP_ABSORPTION: return TEXT("UTIL_HP_ABSORPTION");
		case AbnormalityEffectType::UTIL_MOUNT_VEHICLE: return TEXT("UTIL_MOUNT_VEHICLE");
		case AbnormalityEffectType::UTIL_ON_HIT_SKILL_NOTIFY: return TEXT("UTIL_ON_HIT_SKILL_NOTIFY");
	}
	return FString();
}
inline FString AbnormalityEffectAddSubMethodTypeEnumToString(AbnormalityEffectAddSubMethodType value)
{
	switch (value)
	{
		case AbnormalityEffectAddSubMethodType::ADD_SUB: return TEXT("ADD_SUB");
		case AbnormalityEffectAddSubMethodType::RESOURCE_RATE: return TEXT("RESOURCE_RATE");
		case AbnormalityEffectAddSubMethodType::CASTER_ATTACK_RATE: return TEXT("CASTER_ATTACK_RATE");
	}
	return FString();
}
inline FString AbnormalityShieldCalcTypeEnumToString(AbnormalityShieldCalcType value)
{
	switch (value)
	{
		case AbnormalityShieldCalcType::POWER_RATE: return TEXT("POWER_RATE");
		case AbnormalityShieldCalcType::MAX_HP: return TEXT("MAX_HP");
	}
	return FString();
}
inline FString AbnormalityRemoveTriggerConditionEnumToString(AbnormalityRemoveTriggerCondition value)
{
	switch (value)
	{
		case AbnormalityRemoveTriggerCondition::EXPIRED: return TEXT("EXPIRED");
		case AbnormalityRemoveTriggerCondition::RESOURCE_EXHAUSTION: return TEXT("RESOURCE_EXHAUSTION");
	}
	return FString();
}
inline FString AbnormalityKnockBackDirectionTypeEnumToString(AbnormalityKnockBackDirectionType value)
{
	switch (value)
	{
		case AbnormalityKnockBackDirectionType::Caster: return TEXT("Caster");
		case AbnormalityKnockBackDirectionType::Radial: return TEXT("Radial");
	}
	return FString();
}
inline FString AbnormalityRemoveConditionTypeEnumToString(AbnormalityRemoveConditionType value)
{
	switch (value)
	{
		case AbnormalityRemoveConditionType::LOGOUT: return TEXT("LOGOUT");
		case AbnormalityRemoveConditionType::DEATH: return TEXT("DEATH");
		case AbnormalityRemoveConditionType::DURATION_TIME: return TEXT("DURATION_TIME");
		case AbnormalityRemoveConditionType::REQUIRE_WEAPON: return TEXT("REQUIRE_WEAPON");
		case AbnormalityRemoveConditionType::START_COMBAT: return TEXT("START_COMBAT");
		case AbnormalityRemoveConditionType::TAKE_DAMAGE: return TEXT("TAKE_DAMAGE");
		case AbnormalityRemoveConditionType::TAKE_HEAL: return TEXT("TAKE_HEAL");
		case AbnormalityRemoveConditionType::TAKE_REACTION_DOWN: return TEXT("TAKE_REACTION_DOWN");
		case AbnormalityRemoveConditionType::TAKE_REACTION_KNOCKBACK: return TEXT("TAKE_REACTION_KNOCKBACK");
		case AbnormalityRemoveConditionType::TAKE_REACTION_AIRBORNE: return TEXT("TAKE_REACTION_AIRBORNE");
		case AbnormalityRemoveConditionType::USE_SKILL: return TEXT("USE_SKILL");
		case AbnormalityRemoveConditionType::USE_ACTIVE_SKILL: return TEXT("USE_ACTIVE_SKILL");
		case AbnormalityRemoveConditionType::USE_ACTIVE_SKILL_END: return TEXT("USE_ACTIVE_SKILL_END");
		case AbnormalityRemoveConditionType::USE_ITEM: return TEXT("USE_ITEM");
		case AbnormalityRemoveConditionType::INTERACTION: return TEXT("INTERACTION");
		case AbnormalityRemoveConditionType::DESPAWN: return TEXT("DESPAWN");
		case AbnormalityRemoveConditionType::SYSTEM_FORCE_NOT_REMOVEABLE: return TEXT("SYSTEM_FORCE_NOT_REMOVEABLE");
		case AbnormalityRemoveConditionType::CONSUME_HP_SHIELD: return TEXT("CONSUME_HP_SHIELD");
		case AbnormalityRemoveConditionType::CHANGE_FAIRY: return TEXT("CHANGE_FAIRY");
		case AbnormalityRemoveConditionType::AI_RETURN_STATE: return TEXT("AI_RETURN_STATE");
		case AbnormalityRemoveConditionType::IN_TOWN: return TEXT("IN_TOWN");
		case AbnormalityRemoveConditionType::CHANGE_PRESET: return TEXT("CHANGE_PRESET");
	}
	return FString();
}
inline FString AbnormalityEffectStatMethodTypeEnumToString(AbnormalityEffectStatMethodType value)
{
	switch (value)
	{
		case AbnormalityEffectStatMethodType::NORMAL: return TEXT("NORMAL");
		case AbnormalityEffectStatMethodType::PROPORTION: return TEXT("PROPORTION");
		case AbnormalityEffectStatMethodType::LESS_HP_RATE: return TEXT("LESS_HP_RATE");
		case AbnormalityEffectStatMethodType::NORMAL_WITH_HP_ON_ACTIVE: return TEXT("NORMAL_WITH_HP_ON_ACTIVE");
		case AbnormalityEffectStatMethodType::PROPORTION_WITH_HP_ON_ACTIVE: return TEXT("PROPORTION_WITH_HP_ON_ACTIVE");
	}
	return FString();
}
inline FString AbnormalityEffectBuffCountHitTypeEnumToString(AbnormalityEffectBuffCountHitType value)
{
	switch (value)
	{
		case AbnormalityEffectBuffCountHitType::HIT_SELF_NORMAL: return TEXT("HIT_SELF_NORMAL");
		case AbnormalityEffectBuffCountHitType::HIT_SELF_SKILL: return TEXT("HIT_SELF_SKILL");
		case AbnormalityEffectBuffCountHitType::HIT_SELF: return TEXT("HIT_SELF");
		case AbnormalityEffectBuffCountHitType::HIT_OR_DODGE_SELF: return TEXT("HIT_OR_DODGE_SELF");
	}
	return FString();
}
inline FString AbnormalityEffectBuffCountSkillGradeEnumToString(AbnormalityEffectBuffCountSkillGrade value)
{
	switch (value)
	{
		case AbnormalityEffectBuffCountSkillGrade::NONE: return TEXT("NONE");
		case AbnormalityEffectBuffCountSkillGrade::NORMAL: return TEXT("NORMAL");
		case AbnormalityEffectBuffCountSkillGrade::BLOW_MAGIC: return TEXT("BLOW_MAGIC");
		case AbnormalityEffectBuffCountSkillGrade::BLOW_RARE: return TEXT("BLOW_RARE");
		case AbnormalityEffectBuffCountSkillGrade::BLOW_EPIC: return TEXT("BLOW_EPIC");
		case AbnormalityEffectBuffCountSkillGrade::BLOW_UNIQUE: return TEXT("BLOW_UNIQUE");
		case AbnormalityEffectBuffCountSkillGrade::BLOW_LEGEND: return TEXT("BLOW_LEGEND");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString AbnormalityStackTypeEnumToDisplayKey(AbnormalityStackType value)
{
	switch (value)
	{
		case AbnormalityStackType::NONE: return TEXT("");
		case AbnormalityStackType::RENEWAL: return TEXT("");
		case AbnormalityStackType::RENEWAL_STACK: return TEXT("");
		case AbnormalityStackType::PARALLEL: return TEXT("");
		case AbnormalityStackType::PARALLEL_STACK: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityEffectMainTypeEnumToDisplayKey(AbnormalityEffectMainType value)
{
	switch (value)
	{
		case AbnormalityEffectMainType::NONE: return TEXT("");
		case AbnormalityEffectMainType::OVERTIME: return TEXT("");
		case AbnormalityEffectMainType::CC: return TEXT("");
		case AbnormalityEffectMainType::STAT: return TEXT("");
		case AbnormalityEffectMainType::UTIL: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityEffectTypeEnumToDisplayKey(AbnormalityEffectType value)
{
	switch (value)
	{
		case AbnormalityEffectType::NONE: return TEXT("");
		case AbnormalityEffectType::OVERTIME_DAMAGE: return TEXT("");
		case AbnormalityEffectType::OVERTIME_HOT: return TEXT("");
		case AbnormalityEffectType::OVERTIME_SENSITIVE: return TEXT("");
		case AbnormalityEffectType::CC_STUN: return TEXT("");
		case AbnormalityEffectType::CC_SILENCE: return TEXT("");
		case AbnormalityEffectType::CC_BIND: return TEXT("");
		case AbnormalityEffectType::CC_CANNOT_WARP: return TEXT("");
		case AbnormalityEffectType::CC_BURY: return TEXT("");
		case AbnormalityEffectType::CC_KNOCK_BACK: return TEXT("");
		case AbnormalityEffectType::CC_FEAR: return TEXT("");
		case AbnormalityEffectType::CC_TAUNT: return TEXT("");
		case AbnormalityEffectType::CC_MOVE_LOCATION: return TEXT("");
		case AbnormalityEffectType::CC_POLYMORP: return TEXT("");
		case AbnormalityEffectType::CC_BLOCKADE: return TEXT("");
		case AbnormalityEffectType::STAT: return TEXT("");
		case AbnormalityEffectType::STAT_DASH: return TEXT("");
		case AbnormalityEffectType::STAT_IMMUNE: return TEXT("");
		case AbnormalityEffectType::STAT_VENT: return TEXT("");
		case AbnormalityEffectType::UTIL_ON_REMOVE_EXE_SKILL_NOTIFY: return TEXT("");
		case AbnormalityEffectType::UTIL_NOT_AI_CHANGE_PHASE: return TEXT("");
		case AbnormalityEffectType::UTIL_IMMORTAL: return TEXT("");
		case AbnormalityEffectType::UTIL_HIDING: return TEXT("");
		case AbnormalityEffectType::UTIL_BERSERKER_RAGE: return TEXT("");
		case AbnormalityEffectType::UTIL_POINTED_ARROW: return TEXT("");
		case AbnormalityEffectType::UTIL_WEDGE_ARROW: return TEXT("");
		case AbnormalityEffectType::UTIL_JAGGED_ARROW: return TEXT("");
		case AbnormalityEffectType::UTIL_FINAL_HOWL: return TEXT("");
		case AbnormalityEffectType::UTIL_BONUS_DAMAGE: return TEXT("");
		case AbnormalityEffectType::UTIL_BUFF_COUNTER: return TEXT("");
		case AbnormalityEffectType::UTIL_BURY_KEEP_TARGET: return TEXT("");
		case AbnormalityEffectType::UTIL_CONFUSE: return TEXT("");
		case AbnormalityEffectType::UTIL_DAMAGE_SHARE: return TEXT("");
		case AbnormalityEffectType::UTIL_CREATE_DEPON_NPC: return TEXT("");
		case AbnormalityEffectType::UTIL_HP_ABSORPTION: return TEXT("");
		case AbnormalityEffectType::UTIL_MOUNT_VEHICLE: return TEXT("");
		case AbnormalityEffectType::UTIL_ON_HIT_SKILL_NOTIFY: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityEffectAddSubMethodTypeEnumToDisplayKey(AbnormalityEffectAddSubMethodType value)
{
	switch (value)
	{
		case AbnormalityEffectAddSubMethodType::ADD_SUB: return TEXT("");
		case AbnormalityEffectAddSubMethodType::RESOURCE_RATE: return TEXT("");
		case AbnormalityEffectAddSubMethodType::CASTER_ATTACK_RATE: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityShieldCalcTypeEnumToDisplayKey(AbnormalityShieldCalcType value)
{
	switch (value)
	{
		case AbnormalityShieldCalcType::POWER_RATE: return TEXT("");
		case AbnormalityShieldCalcType::MAX_HP: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityRemoveTriggerConditionEnumToDisplayKey(AbnormalityRemoveTriggerCondition value)
{
	switch (value)
	{
		case AbnormalityRemoveTriggerCondition::EXPIRED: return TEXT("");
		case AbnormalityRemoveTriggerCondition::RESOURCE_EXHAUSTION: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityKnockBackDirectionTypeEnumToDisplayKey(AbnormalityKnockBackDirectionType value)
{
	switch (value)
	{
		case AbnormalityKnockBackDirectionType::Caster: return TEXT("");
		case AbnormalityKnockBackDirectionType::Radial: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityRemoveConditionTypeEnumToDisplayKey(AbnormalityRemoveConditionType value)
{
	switch (value)
	{
		case AbnormalityRemoveConditionType::LOGOUT: return TEXT("");
		case AbnormalityRemoveConditionType::DEATH: return TEXT("");
		case AbnormalityRemoveConditionType::DURATION_TIME: return TEXT("");
		case AbnormalityRemoveConditionType::REQUIRE_WEAPON: return TEXT("");
		case AbnormalityRemoveConditionType::START_COMBAT: return TEXT("");
		case AbnormalityRemoveConditionType::TAKE_DAMAGE: return TEXT("");
		case AbnormalityRemoveConditionType::TAKE_HEAL: return TEXT("");
		case AbnormalityRemoveConditionType::TAKE_REACTION_DOWN: return TEXT("");
		case AbnormalityRemoveConditionType::TAKE_REACTION_KNOCKBACK: return TEXT("");
		case AbnormalityRemoveConditionType::TAKE_REACTION_AIRBORNE: return TEXT("");
		case AbnormalityRemoveConditionType::USE_SKILL: return TEXT("");
		case AbnormalityRemoveConditionType::USE_ACTIVE_SKILL: return TEXT("");
		case AbnormalityRemoveConditionType::USE_ACTIVE_SKILL_END: return TEXT("");
		case AbnormalityRemoveConditionType::USE_ITEM: return TEXT("");
		case AbnormalityRemoveConditionType::INTERACTION: return TEXT("");
		case AbnormalityRemoveConditionType::DESPAWN: return TEXT("");
		case AbnormalityRemoveConditionType::SYSTEM_FORCE_NOT_REMOVEABLE: return TEXT("");
		case AbnormalityRemoveConditionType::CONSUME_HP_SHIELD: return TEXT("");
		case AbnormalityRemoveConditionType::CHANGE_FAIRY: return TEXT("");
		case AbnormalityRemoveConditionType::AI_RETURN_STATE: return TEXT("");
		case AbnormalityRemoveConditionType::IN_TOWN: return TEXT("");
		case AbnormalityRemoveConditionType::CHANGE_PRESET: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityEffectStatMethodTypeEnumToDisplayKey(AbnormalityEffectStatMethodType value)
{
	switch (value)
	{
		case AbnormalityEffectStatMethodType::NORMAL: return TEXT("");
		case AbnormalityEffectStatMethodType::PROPORTION: return TEXT("");
		case AbnormalityEffectStatMethodType::LESS_HP_RATE: return TEXT("");
		case AbnormalityEffectStatMethodType::NORMAL_WITH_HP_ON_ACTIVE: return TEXT("");
		case AbnormalityEffectStatMethodType::PROPORTION_WITH_HP_ON_ACTIVE: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityEffectBuffCountHitTypeEnumToDisplayKey(AbnormalityEffectBuffCountHitType value)
{
	switch (value)
	{
		case AbnormalityEffectBuffCountHitType::HIT_SELF_NORMAL: return TEXT("");
		case AbnormalityEffectBuffCountHitType::HIT_SELF_SKILL: return TEXT("");
		case AbnormalityEffectBuffCountHitType::HIT_SELF: return TEXT("");
		case AbnormalityEffectBuffCountHitType::HIT_OR_DODGE_SELF: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityEffectBuffCountSkillGradeEnumToDisplayKey(AbnormalityEffectBuffCountSkillGrade value)
{
	switch (value)
	{
		case AbnormalityEffectBuffCountSkillGrade::NONE: return TEXT("");
		case AbnormalityEffectBuffCountSkillGrade::NORMAL: return TEXT("");
		case AbnormalityEffectBuffCountSkillGrade::BLOW_MAGIC: return TEXT("");
		case AbnormalityEffectBuffCountSkillGrade::BLOW_RARE: return TEXT("");
		case AbnormalityEffectBuffCountSkillGrade::BLOW_EPIC: return TEXT("");
		case AbnormalityEffectBuffCountSkillGrade::BLOW_UNIQUE: return TEXT("");
		case AbnormalityEffectBuffCountSkillGrade::BLOW_LEGEND: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidAbnormalityStackType(AbnormalityStackType value) noexcept
{
	return (value >= AbnormalityStackType::NONE && value < AbnormalityStackType::MAX);
}

inline bool IsValidAbnormalityEffectMainType(AbnormalityEffectMainType value) noexcept
{
	return (value >= AbnormalityEffectMainType::NONE && value < AbnormalityEffectMainType::MAX);
}

inline bool IsValidAbnormalityEffectType(AbnormalityEffectType value) noexcept
{
	return (value >= AbnormalityEffectType::NONE && value < AbnormalityEffectType::MAX);
}

inline bool IsValidAbnormalityEffectAddSubMethodType(AbnormalityEffectAddSubMethodType value) noexcept
{
	return (value >= AbnormalityEffectAddSubMethodType::ADD_SUB && value < AbnormalityEffectAddSubMethodType::MAX);
}

inline bool IsValidAbnormalityShieldCalcType(AbnormalityShieldCalcType value) noexcept
{
	return (value >= AbnormalityShieldCalcType::POWER_RATE && value < AbnormalityShieldCalcType::MAX);
}

inline bool IsValidAbnormalityRemoveTriggerCondition(AbnormalityRemoveTriggerCondition value) noexcept
{
	return (value >= AbnormalityRemoveTriggerCondition::EXPIRED && value < AbnormalityRemoveTriggerCondition::MAX);
}

inline bool IsValidAbnormalityKnockBackDirectionType(AbnormalityKnockBackDirectionType value) noexcept
{
	return (value >= AbnormalityKnockBackDirectionType::Caster && value < AbnormalityKnockBackDirectionType::MAX);
}

inline bool IsValidAbnormalityRemoveConditionType(AbnormalityRemoveConditionType value) noexcept
{
	return (value >= AbnormalityRemoveConditionType::LOGOUT && value < AbnormalityRemoveConditionType::MAX);
}

inline bool IsValidAbnormalityEffectStatMethodType(AbnormalityEffectStatMethodType value) noexcept
{
	return (value >= AbnormalityEffectStatMethodType::NORMAL && value < AbnormalityEffectStatMethodType::MAX);
}

inline bool IsValidAbnormalityEffectBuffCountHitType(AbnormalityEffectBuffCountHitType value) noexcept
{
	return (value >= AbnormalityEffectBuffCountHitType::HIT_SELF_NORMAL && value < AbnormalityEffectBuffCountHitType::MAX);
}

inline bool IsValidAbnormalityEffectBuffCountSkillGrade(AbnormalityEffectBuffCountSkillGrade value) noexcept
{
	return (value >= AbnormalityEffectBuffCountSkillGrade::NONE && value < AbnormalityEffectBuffCountSkillGrade::MAX);
}


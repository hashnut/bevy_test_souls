#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedSkillEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 스킬의 카테고리 집합 - 이는 DT의 칼럼으로 쓰이지 않는, 내부 분류
enum class SkillCategorySet : uint8
{

	// 액티브 - 즉발형
	ACTIVE = 0,

	// 버프 - 지속형
	BUFF = 1,

	// 패시브 - 패시브
	PASSIVE = 2,

	// 버프 - 대시
	DASH = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 스킬의 세부 카테고리
enum class SkillCategory : uint8
{
	// SkillCategorySet::ACTIVE

	// 일반 - 일반 스킬
	NORMAL = 0,

	// 콤보 - 콤보 스킬
	COMBO = 1,

	// 대시 - 대시 스킬
	DASH = 2,

	// 캐스팅 스킬 - 캐스팅의 경우 타임라인 완료 후 스킬의 타임라인 실행
	CAST = 3,

	// SkillCategorySet::BUFF

	// 버프스킬 - 모션없음, 즉시발동, 이동가능
	BUFF = 4,

	// SkillCategorySet::PASSIVE

	// 패시브 - 패시브 스킬
	PASSIVE = 5,

	MAX = 6,
};

UENUM()
// 스킬 근/원거리 타입
enum class SkillRangeType : uint8
{

	// 근거리
	MELEE = 0,

	// 원거리
	RANGE = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 스킬 이뮨 타입
enum class SkillImmuneType : uint8
{

	// 해당 사항 없음
	NONE = 0,

	MAX = 1,
};

UENUM(BlueprintType)
// 스킬 타겟팅 방식
enum class SkillTargetMethod : uint8
{

	// 타겟팅 방식
	TARGET = 0,

	// 논타겟팅 방식
	NON_TARGET = 1,

	MAX = 2,
};

UENUM()
// 스킬 제약 체크 대상 타입
enum class SkillRestrictionTarget : uint8
{

	//  내 상태를 가지고 제약체크 하면 됨
	SELF = 0,

	// 대상의 상태를 가지고 제약체크 하면 됨 (타겟팅 스킬일 때만 의미가 있음)
	TARGET = 1,

	MAX = 2,
};

UENUM()
// 스킬 제약 타입
enum class SkillRestrictionType : uint8
{

	// 해당 사항 없음
	NONE = 0,

	// Hp 요구비율 이상을 가지고 있는지 체크 (캐스터 or 타겟)
	HP_RATE_GREATER = 1,

	// Hp 요구비율 이하를 가지고 있는지 체크 (캐스터 or 타겟)
	HP_RATE_LESS = 2,

	// Mp 요구비율 이상을 가지고 있는지 체크 (캐스터 or 타겟)
	MP_RATE_GREATER = 3,

	// Mp 요구비율 이하를 가지고 있는지 체크 (캐스터 or 타겟)
	MP_RATE_LESS = 4,

	// 특정 상태효과에 걸려 있어야 함 (캐스터 or 타겟)
	ABNORMALITY_ON = 5,

	// 특정 상태효과에 걸려 있지 않아야 함 (캐스터 or 타겟)
	ABNORMALITY_OFF = 6,

	MAX = 7,
};

UENUM(BlueprintType)
// 스킬 충돌 판정 타입
enum class SkillCollisionType : uint8
{

	// 해당 사항 없음 - 예외 처리용
	NONE = 0,

	// 원형 체크
	CIRCLE = 1,

	// 부채꼴 체크
	FAN = 2,

	// 사각형(concave-4points) 체크
	RECT = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 스킬 판정 영역 결정시 원점 타입
enum class SkillAreaCenter : uint8
{

	// 캐스터(시전자)
	CASTER = 0,

	// 대상 위치
	TARGET = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 
 /* 스킬 효과 대상 
 	1. AbnormalityEffectSubType::STAT - param1:AbnormalityEffectStatMethodType::PROPORTION에서 param2의 값 
 	2. 추후 다른 DataSheet 위치 추가 
*/
enum class SkillTargetType : uint8
{

	// 캐스터(시전자)
	CASTER = 0,

	// 대상 위치
	TARGET = 1,

	MAX = 2,
};

UENUM()
// 실행 조건 대상 (가안 테스트)
enum class ExecuteConditionTargetType : uint8
{

	// 캐스터(시전자)
	CASTER = 0,

	// 대상
	TARGET = 1,

	// 노티파이에서 취한 대상
	NOTIFY_TARGET = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 스킬 타격 결과 유형
enum class HitResultType : uint8
{

	// 일반
	NORMAL = 0,

	// 회피
	DODGE = 1,

	// 크리티컬
	CRITICAL = 2,

	// 일정 이상의 대미지 비율이 발생할 시
	GREAT = 3,

	// 이뮨 - 피해면역(데미지)
	IMMUNE_DAMAGE = 4,

	// 이뮨 - 상태이상면역
	IMMUNE_ABNORMALITY = 5,

	// 회복
	HEAL = 6,

	// 반격 : AbnormalityEffectType::UTIL_BUFF_COUNTER
	BUFF_COUNTER = 7,

	// 상태이상 미적중
	MISS_ABNORMALITY = 8,

	// ABSORBED(보호막 등에 흡수), KILL(사망) 안 써서 삭제

	MAX = 9,
};

UENUM()
// 스킬 완료 사유
enum class SkillFinishReason : uint8
{

	// 스킬 정상 종료
	NORMAL = 0,

	// 유저 요청
	USER_CANCEL = 1,

	// 사망
	DEATH = 2,

	// 리액션
	REACTION = 3,

	// 일반 피격 (반격기?)
	HIT_BY_NORMAL = 4,

	// 크리티컬 피격
	HIT_BY_CRITICAL = 5,

	// 비용 부족
	NO_COST = 6,

	// 카운터스펠 (마법 차단)
	COUNTER_SPELL = 7,

	// AI 변경
	CHANGE_AI = 8,

	// 행위 불가로 정지됨
	BEHAVIOR_DISABLED = 9,

	// 스킬 효과 실행 실패
	SKILL_NOTIFY_EXEC_FAILED = 10,

	// noTimeLine 일괄 실행으로 스킬 종료
	NO_TIME_LINE = 11,

	MAX = 12,
};

UENUM(BlueprintType)
// 스킬 강화 결과
enum class SkillEnchantResult : uint8
{

	// 에러 값
	NONE = 0,

	// 성공
	SUCCESS = 1,

	// 실패
	FAILED = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 스킬 등급
enum class SkillGrade : uint8
{

	// 에러값
	NONE = 0,

	// 등급 - NORMAL
	NORMAL = 1,

	// 등급 - MAGIC
	MAGIC = 2,

	// 등급 - RARE
	RARE = 3,

	// 등급 - EPIC
	EPIC = 4,

	// 등급 - UNIQUE
	UNIQUE = 5,

	// 등급 - LEGEND
	LEGEND = 6,

	MAX = 7,
};

UENUM()
// 스킬 시작 알림 구분자
enum class SkillStartType : uint8
{

	// 초기값
	NONE = 0,

	// 캐스팅 시작 알림
	CAST = 1,

	// 스킬 효과 시작 알림
	SKILL = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 스킬진행 횟수 - 단일, 콤보(평타), 체인(연결된 스킬)
enum class SkillSequenceType : uint8
{

	// 액티브 - 즉발형
	NONE = 0,

	// 콤보 - 기존의 평타
	COMBO = 1,

	// 체인 - 앞 스킬을 사용 후, 뒤스킬 사용
	CHAIN = 2,
};

UENUM(BlueprintType)
// 스킬 업그레이드 비용 종류
enum class SkillUpConsumeCostType : uint8
{

	// 초기값 - 소모 없음
	NONE = 0,

	// 소모 - 아이템
	ONLY_ITEM = 1,

	// 소모 - CURRENCY(재화)
	ONLY_CURRENCY = 2,

	// 소모 - 아이템, CURRENCY
	BOTH = 3,
};

UENUM(BlueprintType)
// 체인스킬 발동 조건
enum class SkillChainCondition : uint8
{

	// 초기값
	NONE = 0,

	// 이전 스킬 사용 시, 체인스킬 사용
	PREV_SKILL = 1,

	// 이전 스킬 적중 시, 체인스킬 사용
	HIT_PREV_SKILL = 2,

	// 이전 스킬 적중 시, 체인스킬 사용, 시간이 지나도 체인 스킬 유지
	PREV_SKILL_REMAIN = 3,
};

UENUM(BlueprintType)
// 스킬 슬롯 인덱스
enum class SkillSlotIndex : uint8
{

	// 초기값 - 클라이언트에선 평타 스킬 슬롯으로 사용, 서버에서는 무효한 스킬 슬롯으로 사용
	INVALID_SKILL_SLOT_ID = 0,

	// 즉발형 스킬 슬롯 (1~5)

	// 즉발형 스킬의 등록 가능한 슬롯 Index
	ACTIVE_SKILL_SLOT_INDEX = 5,

	// 지속형 스킬 슬롯 (6~10)

	// 지속형 스킬의 등록 가능한 슬롯 Index
	BUFF_SKILL_SLOT_INDEX = 10,

	// 코스튬 스킬 슬롯 (11)

	// 코스튬 스킬의 등록 가능한 슬롯 Index
	CUSTUME_SKILL_SLOT_INDEX = 11,
};

UENUM(BlueprintType)
// 스킬/스킬 효과 발동 조건 타입
enum class SkillExecuteConditionMethod : uint8
{

	// None - 조건 검증하지 않음, 무조건 발동
	NONE = 0,

	// 이상상태 검증
	ABNORMALITY = 1,

	// 스탯 검증
	STAT = 2,

	// 패시비티 검증
	PASSIVITY = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 스탯 조건
enum class SkillExecuteConditionStatConditionMethod : uint8
{

	// 미만일 떄(이하가 아닌 미만으로 처리하기로 기획팀과 약속함)
	LESS = 0,

	// 이상일 때
	MORE = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 스탯 조건 분류
enum class SkillExecuteConditionStatConditionType : uint8
{

	// 특정 스탯 비율 검증
	RATIO = 0,

	// 특정 스탯 값 검증
	VALUE = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 상태이상 조건 분류
enum class SkillExecuteConditionAbnormalityConditionType : uint8
{

	// 조건 없음, 충족으로 판단
	NONE = 0,

	// 특정 스탯 비율 검증
	ABNORMALITY_ID = 1,

	// 특정 업노멀 EffectType 검증
	ABNORMALITY_EFFECT_TYPE = 2,

	// 특정 스탯 값 검증 ---- 삭제 필요
	ABNORMALITY_EFFECT_SUB_TYPE = 3,

	// 특정 효과 그룹
	ABNORMALITY_EFFECT_GROUP_ID = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 대상 필터 방식
enum class SkillTargetFilterType : uint8
{

	// 필터 안함
	NONE = 0,

	// 특정 이상상태 ID 가 걸려 있는 대상만 추출
	ABNORMALITY_ID = 1,

	// 특정 이상상태 효과 서브 타입이 걸려 있는 대상만 추출
	ABNORMALITY_EFFECT_TYPE = 2,

	// 특정 이상상태 효과 서브 타입이 걸려 있는 대상만 추출 -- 삭제 핊요
	ABNORMALITY_EFFECT_SUB_TYPE = 3,

	// 특정 Npc Faction Id 인 대상만 추출(NpcData - npcFactionId)
	NPC_FACTION_ID = 4,

	// 특정 효과 그룹 추출
	ABNORMALITY_EFFECT_GROUP_ID = 5,

	// 유저 대상 한정
	PC_ONLY = 6,

	// NPC 대상 한정
	NPC_ONLY = 7,

	MAX = 8,
};

UENUM(BlueprintType)
// 특정 이상상태의 적용/미적용 여부 구분 타입
enum class SetAbnormalityAllow : uint8
{

	// 하위 업노멀 보유 시 조건을 충족 하는 것으로 간주 ex) 은신 버프 보유 시 사용 가능한 스킬
	ALLOW = 0,

	// 하위 업노멀 미 보유 시 조건을 충족 하는 것으로 간주 ex) 룰 브레이커 디버프 보유 시 사용 불가능한 스킬
	DISALLOW = 1,

	MAX = 2,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline SkillCategorySet SkillCategorySetStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ACTIVE"), ESearchCase::IgnoreCase) == 0)	return SkillCategorySet::ACTIVE;
	if (str.Compare(TEXT("BUFF"), ESearchCase::IgnoreCase) == 0)	return SkillCategorySet::BUFF;
	if (str.Compare(TEXT("PASSIVE"), ESearchCase::IgnoreCase) == 0)	return SkillCategorySet::PASSIVE;
	if (str.Compare(TEXT("DASH"), ESearchCase::IgnoreCase) == 0)	return SkillCategorySet::DASH;
	return SkillCategorySet::MAX;
}
inline SkillCategory SkillCategoryStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return SkillCategory::NORMAL;
	if (str.Compare(TEXT("COMBO"), ESearchCase::IgnoreCase) == 0)	return SkillCategory::COMBO;
	if (str.Compare(TEXT("DASH"), ESearchCase::IgnoreCase) == 0)	return SkillCategory::DASH;
	if (str.Compare(TEXT("CAST"), ESearchCase::IgnoreCase) == 0)	return SkillCategory::CAST;
	if (str.Compare(TEXT("BUFF"), ESearchCase::IgnoreCase) == 0)	return SkillCategory::BUFF;
	if (str.Compare(TEXT("PASSIVE"), ESearchCase::IgnoreCase) == 0)	return SkillCategory::PASSIVE;
	return SkillCategory::MAX;
}
inline SkillRangeType SkillRangeTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("MELEE"), ESearchCase::IgnoreCase) == 0)	return SkillRangeType::MELEE;
	if (str.Compare(TEXT("RANGE"), ESearchCase::IgnoreCase) == 0)	return SkillRangeType::RANGE;
	return SkillRangeType::MAX;
}
inline SkillImmuneType SkillImmuneTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillImmuneType::NONE;
	return SkillImmuneType::MAX;
}
inline SkillTargetMethod SkillTargetMethodStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return SkillTargetMethod::TARGET;
	if (str.Compare(TEXT("NON_TARGET"), ESearchCase::IgnoreCase) == 0)	return SkillTargetMethod::NON_TARGET;
	return SkillTargetMethod::MAX;
}
inline SkillRestrictionTarget SkillRestrictionTargetStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("SELF"), ESearchCase::IgnoreCase) == 0)	return SkillRestrictionTarget::SELF;
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return SkillRestrictionTarget::TARGET;
	return SkillRestrictionTarget::MAX;
}
inline SkillRestrictionType SkillRestrictionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillRestrictionType::NONE;
	if (str.Compare(TEXT("HP_RATE_GREATER"), ESearchCase::IgnoreCase) == 0)	return SkillRestrictionType::HP_RATE_GREATER;
	if (str.Compare(TEXT("HP_RATE_LESS"), ESearchCase::IgnoreCase) == 0)	return SkillRestrictionType::HP_RATE_LESS;
	if (str.Compare(TEXT("MP_RATE_GREATER"), ESearchCase::IgnoreCase) == 0)	return SkillRestrictionType::MP_RATE_GREATER;
	if (str.Compare(TEXT("MP_RATE_LESS"), ESearchCase::IgnoreCase) == 0)	return SkillRestrictionType::MP_RATE_LESS;
	if (str.Compare(TEXT("ABNORMALITY_ON"), ESearchCase::IgnoreCase) == 0)	return SkillRestrictionType::ABNORMALITY_ON;
	if (str.Compare(TEXT("ABNORMALITY_OFF"), ESearchCase::IgnoreCase) == 0)	return SkillRestrictionType::ABNORMALITY_OFF;
	return SkillRestrictionType::MAX;
}
inline SkillCollisionType SkillCollisionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillCollisionType::NONE;
	if (str.Compare(TEXT("CIRCLE"), ESearchCase::IgnoreCase) == 0)	return SkillCollisionType::CIRCLE;
	if (str.Compare(TEXT("FAN"), ESearchCase::IgnoreCase) == 0)	return SkillCollisionType::FAN;
	if (str.Compare(TEXT("RECT"), ESearchCase::IgnoreCase) == 0)	return SkillCollisionType::RECT;
	return SkillCollisionType::MAX;
}
inline SkillAreaCenter SkillAreaCenterStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CASTER"), ESearchCase::IgnoreCase) == 0)	return SkillAreaCenter::CASTER;
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return SkillAreaCenter::TARGET;
	return SkillAreaCenter::MAX;
}
inline SkillTargetType SkillTargetTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CASTER"), ESearchCase::IgnoreCase) == 0)	return SkillTargetType::CASTER;
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return SkillTargetType::TARGET;
	return SkillTargetType::MAX;
}
inline ExecuteConditionTargetType ExecuteConditionTargetTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CASTER"), ESearchCase::IgnoreCase) == 0)	return ExecuteConditionTargetType::CASTER;
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return ExecuteConditionTargetType::TARGET;
	if (str.Compare(TEXT("NOTIFY_TARGET"), ESearchCase::IgnoreCase) == 0)	return ExecuteConditionTargetType::NOTIFY_TARGET;
	return ExecuteConditionTargetType::MAX;
}
inline HitResultType HitResultTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return HitResultType::NORMAL;
	if (str.Compare(TEXT("DODGE"), ESearchCase::IgnoreCase) == 0)	return HitResultType::DODGE;
	if (str.Compare(TEXT("CRITICAL"), ESearchCase::IgnoreCase) == 0)	return HitResultType::CRITICAL;
	if (str.Compare(TEXT("GREAT"), ESearchCase::IgnoreCase) == 0)	return HitResultType::GREAT;
	if (str.Compare(TEXT("IMMUNE_DAMAGE"), ESearchCase::IgnoreCase) == 0)	return HitResultType::IMMUNE_DAMAGE;
	if (str.Compare(TEXT("IMMUNE_ABNORMALITY"), ESearchCase::IgnoreCase) == 0)	return HitResultType::IMMUNE_ABNORMALITY;
	if (str.Compare(TEXT("HEAL"), ESearchCase::IgnoreCase) == 0)	return HitResultType::HEAL;
	if (str.Compare(TEXT("BUFF_COUNTER"), ESearchCase::IgnoreCase) == 0)	return HitResultType::BUFF_COUNTER;
	if (str.Compare(TEXT("MISS_ABNORMALITY"), ESearchCase::IgnoreCase) == 0)	return HitResultType::MISS_ABNORMALITY;
	return HitResultType::MAX;
}
inline SkillFinishReason SkillFinishReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::NORMAL;
	if (str.Compare(TEXT("USER_CANCEL"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::USER_CANCEL;
	if (str.Compare(TEXT("DEATH"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::DEATH;
	if (str.Compare(TEXT("REACTION"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::REACTION;
	if (str.Compare(TEXT("HIT_BY_NORMAL"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::HIT_BY_NORMAL;
	if (str.Compare(TEXT("HIT_BY_CRITICAL"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::HIT_BY_CRITICAL;
	if (str.Compare(TEXT("NO_COST"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::NO_COST;
	if (str.Compare(TEXT("COUNTER_SPELL"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::COUNTER_SPELL;
	if (str.Compare(TEXT("CHANGE_AI"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::CHANGE_AI;
	if (str.Compare(TEXT("BEHAVIOR_DISABLED"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::BEHAVIOR_DISABLED;
	if (str.Compare(TEXT("SKILL_NOTIFY_EXEC_FAILED"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::SKILL_NOTIFY_EXEC_FAILED;
	if (str.Compare(TEXT("NO_TIME_LINE"), ESearchCase::IgnoreCase) == 0)	return SkillFinishReason::NO_TIME_LINE;
	return SkillFinishReason::MAX;
}
inline SkillEnchantResult SkillEnchantResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillEnchantResult::NONE;
	if (str.Compare(TEXT("SUCCESS"), ESearchCase::IgnoreCase) == 0)	return SkillEnchantResult::SUCCESS;
	if (str.Compare(TEXT("FAILED"), ESearchCase::IgnoreCase) == 0)	return SkillEnchantResult::FAILED;
	return SkillEnchantResult::MAX;
}
inline SkillGrade SkillGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillGrade::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return SkillGrade::NORMAL;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return SkillGrade::MAGIC;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return SkillGrade::RARE;
	if (str.Compare(TEXT("EPIC"), ESearchCase::IgnoreCase) == 0)	return SkillGrade::EPIC;
	if (str.Compare(TEXT("UNIQUE"), ESearchCase::IgnoreCase) == 0)	return SkillGrade::UNIQUE;
	if (str.Compare(TEXT("LEGEND"), ESearchCase::IgnoreCase) == 0)	return SkillGrade::LEGEND;
	return SkillGrade::MAX;
}
inline SkillStartType SkillStartTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillStartType::NONE;
	if (str.Compare(TEXT("CAST"), ESearchCase::IgnoreCase) == 0)	return SkillStartType::CAST;
	if (str.Compare(TEXT("SKILL"), ESearchCase::IgnoreCase) == 0)	return SkillStartType::SKILL;
	return SkillStartType::MAX;
}
inline SkillSequenceType SkillSequenceTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillSequenceType::NONE;
	if (str.Compare(TEXT("COMBO"), ESearchCase::IgnoreCase) == 0)	return SkillSequenceType::COMBO;
	if (str.Compare(TEXT("CHAIN"), ESearchCase::IgnoreCase) == 0)	return SkillSequenceType::CHAIN;
	return SkillSequenceType::CHAIN;
}
inline SkillUpConsumeCostType SkillUpConsumeCostTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillUpConsumeCostType::NONE;
	if (str.Compare(TEXT("ONLY_ITEM"), ESearchCase::IgnoreCase) == 0)	return SkillUpConsumeCostType::ONLY_ITEM;
	if (str.Compare(TEXT("ONLY_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return SkillUpConsumeCostType::ONLY_CURRENCY;
	if (str.Compare(TEXT("BOTH"), ESearchCase::IgnoreCase) == 0)	return SkillUpConsumeCostType::BOTH;
	return SkillUpConsumeCostType::BOTH;
}
inline SkillChainCondition SkillChainConditionStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillChainCondition::NONE;
	if (str.Compare(TEXT("PREV_SKILL"), ESearchCase::IgnoreCase) == 0)	return SkillChainCondition::PREV_SKILL;
	if (str.Compare(TEXT("HIT_PREV_SKILL"), ESearchCase::IgnoreCase) == 0)	return SkillChainCondition::HIT_PREV_SKILL;
	if (str.Compare(TEXT("PREV_SKILL_REMAIN"), ESearchCase::IgnoreCase) == 0)	return SkillChainCondition::PREV_SKILL_REMAIN;
	return SkillChainCondition::PREV_SKILL_REMAIN;
}
inline SkillSlotIndex SkillSlotIndexStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("INVALID_SKILL_SLOT_ID"), ESearchCase::IgnoreCase) == 0)	return SkillSlotIndex::INVALID_SKILL_SLOT_ID;
	if (str.Compare(TEXT("ACTIVE_SKILL_SLOT_INDEX"), ESearchCase::IgnoreCase) == 0)	return SkillSlotIndex::ACTIVE_SKILL_SLOT_INDEX;
	if (str.Compare(TEXT("BUFF_SKILL_SLOT_INDEX"), ESearchCase::IgnoreCase) == 0)	return SkillSlotIndex::BUFF_SKILL_SLOT_INDEX;
	if (str.Compare(TEXT("CUSTUME_SKILL_SLOT_INDEX"), ESearchCase::IgnoreCase) == 0)	return SkillSlotIndex::CUSTUME_SKILL_SLOT_INDEX;
	return SkillSlotIndex::CUSTUME_SKILL_SLOT_INDEX;
}
inline SkillExecuteConditionMethod SkillExecuteConditionMethodStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionMethod::NONE;
	if (str.Compare(TEXT("ABNORMALITY"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionMethod::ABNORMALITY;
	if (str.Compare(TEXT("STAT"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionMethod::STAT;
	if (str.Compare(TEXT("PASSIVITY"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionMethod::PASSIVITY;
	return SkillExecuteConditionMethod::MAX;
}
inline SkillExecuteConditionStatConditionMethod SkillExecuteConditionStatConditionMethodStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("LESS"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionStatConditionMethod::LESS;
	if (str.Compare(TEXT("MORE"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionStatConditionMethod::MORE;
	return SkillExecuteConditionStatConditionMethod::MAX;
}
inline SkillExecuteConditionStatConditionType SkillExecuteConditionStatConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("RATIO"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionStatConditionType::RATIO;
	if (str.Compare(TEXT("VALUE"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionStatConditionType::VALUE;
	return SkillExecuteConditionStatConditionType::MAX;
}
inline SkillExecuteConditionAbnormalityConditionType SkillExecuteConditionAbnormalityConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionAbnormalityConditionType::NONE;
	if (str.Compare(TEXT("ABNORMALITY_ID"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_ID;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_TYPE"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_TYPE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_SUB_TYPE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_GROUP_ID"), ESearchCase::IgnoreCase) == 0)	return SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_GROUP_ID;
	return SkillExecuteConditionAbnormalityConditionType::MAX;
}
inline SkillTargetFilterType SkillTargetFilterTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SkillTargetFilterType::NONE;
	if (str.Compare(TEXT("ABNORMALITY_ID"), ESearchCase::IgnoreCase) == 0)	return SkillTargetFilterType::ABNORMALITY_ID;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_TYPE"), ESearchCase::IgnoreCase) == 0)	return SkillTargetFilterType::ABNORMALITY_EFFECT_TYPE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE"), ESearchCase::IgnoreCase) == 0)	return SkillTargetFilterType::ABNORMALITY_EFFECT_SUB_TYPE;
	if (str.Compare(TEXT("NPC_FACTION_ID"), ESearchCase::IgnoreCase) == 0)	return SkillTargetFilterType::NPC_FACTION_ID;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_GROUP_ID"), ESearchCase::IgnoreCase) == 0)	return SkillTargetFilterType::ABNORMALITY_EFFECT_GROUP_ID;
	if (str.Compare(TEXT("PC_ONLY"), ESearchCase::IgnoreCase) == 0)	return SkillTargetFilterType::PC_ONLY;
	if (str.Compare(TEXT("NPC_ONLY"), ESearchCase::IgnoreCase) == 0)	return SkillTargetFilterType::NPC_ONLY;
	return SkillTargetFilterType::MAX;
}
inline SetAbnormalityAllow SetAbnormalityAllowStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ALLOW"), ESearchCase::IgnoreCase) == 0)	return SetAbnormalityAllow::ALLOW;
	if (str.Compare(TEXT("DISALLOW"), ESearchCase::IgnoreCase) == 0)	return SetAbnormalityAllow::DISALLOW;
	return SetAbnormalityAllow::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString SkillCategorySetEnumToString(SkillCategorySet value)
{
	switch (value)
	{
		case SkillCategorySet::ACTIVE: return TEXT("ACTIVE");
		case SkillCategorySet::BUFF: return TEXT("BUFF");
		case SkillCategorySet::PASSIVE: return TEXT("PASSIVE");
		case SkillCategorySet::DASH: return TEXT("DASH");
	}
	return FString();
}
inline FString SkillCategoryEnumToString(SkillCategory value)
{
	switch (value)
	{
		case SkillCategory::NORMAL: return TEXT("NORMAL");
		case SkillCategory::COMBO: return TEXT("COMBO");
		case SkillCategory::DASH: return TEXT("DASH");
		case SkillCategory::CAST: return TEXT("CAST");
		case SkillCategory::BUFF: return TEXT("BUFF");
		case SkillCategory::PASSIVE: return TEXT("PASSIVE");
	}
	return FString();
}
inline FString SkillRangeTypeEnumToString(SkillRangeType value)
{
	switch (value)
	{
		case SkillRangeType::MELEE: return TEXT("MELEE");
		case SkillRangeType::RANGE: return TEXT("RANGE");
	}
	return FString();
}
inline FString SkillImmuneTypeEnumToString(SkillImmuneType value)
{
	switch (value)
	{
		case SkillImmuneType::NONE: return TEXT("NONE");
	}
	return FString();
}
inline FString SkillTargetMethodEnumToString(SkillTargetMethod value)
{
	switch (value)
	{
		case SkillTargetMethod::TARGET: return TEXT("TARGET");
		case SkillTargetMethod::NON_TARGET: return TEXT("NON_TARGET");
	}
	return FString();
}
inline FString SkillRestrictionTargetEnumToString(SkillRestrictionTarget value)
{
	switch (value)
	{
		case SkillRestrictionTarget::SELF: return TEXT("SELF");
		case SkillRestrictionTarget::TARGET: return TEXT("TARGET");
	}
	return FString();
}
inline FString SkillRestrictionTypeEnumToString(SkillRestrictionType value)
{
	switch (value)
	{
		case SkillRestrictionType::NONE: return TEXT("NONE");
		case SkillRestrictionType::HP_RATE_GREATER: return TEXT("HP_RATE_GREATER");
		case SkillRestrictionType::HP_RATE_LESS: return TEXT("HP_RATE_LESS");
		case SkillRestrictionType::MP_RATE_GREATER: return TEXT("MP_RATE_GREATER");
		case SkillRestrictionType::MP_RATE_LESS: return TEXT("MP_RATE_LESS");
		case SkillRestrictionType::ABNORMALITY_ON: return TEXT("ABNORMALITY_ON");
		case SkillRestrictionType::ABNORMALITY_OFF: return TEXT("ABNORMALITY_OFF");
	}
	return FString();
}
inline FString SkillCollisionTypeEnumToString(SkillCollisionType value)
{
	switch (value)
	{
		case SkillCollisionType::NONE: return TEXT("NONE");
		case SkillCollisionType::CIRCLE: return TEXT("CIRCLE");
		case SkillCollisionType::FAN: return TEXT("FAN");
		case SkillCollisionType::RECT: return TEXT("RECT");
	}
	return FString();
}
inline FString SkillAreaCenterEnumToString(SkillAreaCenter value)
{
	switch (value)
	{
		case SkillAreaCenter::CASTER: return TEXT("CASTER");
		case SkillAreaCenter::TARGET: return TEXT("TARGET");
	}
	return FString();
}
inline FString SkillTargetTypeEnumToString(SkillTargetType value)
{
	switch (value)
	{
		case SkillTargetType::CASTER: return TEXT("CASTER");
		case SkillTargetType::TARGET: return TEXT("TARGET");
	}
	return FString();
}
inline FString ExecuteConditionTargetTypeEnumToString(ExecuteConditionTargetType value)
{
	switch (value)
	{
		case ExecuteConditionTargetType::CASTER: return TEXT("CASTER");
		case ExecuteConditionTargetType::TARGET: return TEXT("TARGET");
		case ExecuteConditionTargetType::NOTIFY_TARGET: return TEXT("NOTIFY_TARGET");
	}
	return FString();
}
inline FString HitResultTypeEnumToString(HitResultType value)
{
	switch (value)
	{
		case HitResultType::NORMAL: return TEXT("NORMAL");
		case HitResultType::DODGE: return TEXT("DODGE");
		case HitResultType::CRITICAL: return TEXT("CRITICAL");
		case HitResultType::GREAT: return TEXT("GREAT");
		case HitResultType::IMMUNE_DAMAGE: return TEXT("IMMUNE_DAMAGE");
		case HitResultType::IMMUNE_ABNORMALITY: return TEXT("IMMUNE_ABNORMALITY");
		case HitResultType::HEAL: return TEXT("HEAL");
		case HitResultType::BUFF_COUNTER: return TEXT("BUFF_COUNTER");
		case HitResultType::MISS_ABNORMALITY: return TEXT("MISS_ABNORMALITY");
	}
	return FString();
}
inline FString SkillFinishReasonEnumToString(SkillFinishReason value)
{
	switch (value)
	{
		case SkillFinishReason::NORMAL: return TEXT("NORMAL");
		case SkillFinishReason::USER_CANCEL: return TEXT("USER_CANCEL");
		case SkillFinishReason::DEATH: return TEXT("DEATH");
		case SkillFinishReason::REACTION: return TEXT("REACTION");
		case SkillFinishReason::HIT_BY_NORMAL: return TEXT("HIT_BY_NORMAL");
		case SkillFinishReason::HIT_BY_CRITICAL: return TEXT("HIT_BY_CRITICAL");
		case SkillFinishReason::NO_COST: return TEXT("NO_COST");
		case SkillFinishReason::COUNTER_SPELL: return TEXT("COUNTER_SPELL");
		case SkillFinishReason::CHANGE_AI: return TEXT("CHANGE_AI");
		case SkillFinishReason::BEHAVIOR_DISABLED: return TEXT("BEHAVIOR_DISABLED");
		case SkillFinishReason::SKILL_NOTIFY_EXEC_FAILED: return TEXT("SKILL_NOTIFY_EXEC_FAILED");
		case SkillFinishReason::NO_TIME_LINE: return TEXT("NO_TIME_LINE");
	}
	return FString();
}
inline FString SkillEnchantResultEnumToString(SkillEnchantResult value)
{
	switch (value)
	{
		case SkillEnchantResult::NONE: return TEXT("NONE");
		case SkillEnchantResult::SUCCESS: return TEXT("SUCCESS");
		case SkillEnchantResult::FAILED: return TEXT("FAILED");
	}
	return FString();
}
inline FString SkillGradeEnumToString(SkillGrade value)
{
	switch (value)
	{
		case SkillGrade::NONE: return TEXT("NONE");
		case SkillGrade::NORMAL: return TEXT("NORMAL");
		case SkillGrade::MAGIC: return TEXT("MAGIC");
		case SkillGrade::RARE: return TEXT("RARE");
		case SkillGrade::EPIC: return TEXT("EPIC");
		case SkillGrade::UNIQUE: return TEXT("UNIQUE");
		case SkillGrade::LEGEND: return TEXT("LEGEND");
	}
	return FString();
}
inline FString SkillStartTypeEnumToString(SkillStartType value)
{
	switch (value)
	{
		case SkillStartType::NONE: return TEXT("NONE");
		case SkillStartType::CAST: return TEXT("CAST");
		case SkillStartType::SKILL: return TEXT("SKILL");
	}
	return FString();
}
inline FString SkillSequenceTypeEnumToString(SkillSequenceType value)
{
	switch (value)
	{
		case SkillSequenceType::NONE: return TEXT("NONE");
		case SkillSequenceType::COMBO: return TEXT("COMBO");
		case SkillSequenceType::CHAIN: return TEXT("CHAIN");
	}
	return FString();
}
inline FString SkillUpConsumeCostTypeEnumToString(SkillUpConsumeCostType value)
{
	switch (value)
	{
		case SkillUpConsumeCostType::NONE: return TEXT("NONE");
		case SkillUpConsumeCostType::ONLY_ITEM: return TEXT("ONLY_ITEM");
		case SkillUpConsumeCostType::ONLY_CURRENCY: return TEXT("ONLY_CURRENCY");
		case SkillUpConsumeCostType::BOTH: return TEXT("BOTH");
	}
	return FString();
}
inline FString SkillChainConditionEnumToString(SkillChainCondition value)
{
	switch (value)
	{
		case SkillChainCondition::NONE: return TEXT("NONE");
		case SkillChainCondition::PREV_SKILL: return TEXT("PREV_SKILL");
		case SkillChainCondition::HIT_PREV_SKILL: return TEXT("HIT_PREV_SKILL");
		case SkillChainCondition::PREV_SKILL_REMAIN: return TEXT("PREV_SKILL_REMAIN");
	}
	return FString();
}
inline FString SkillSlotIndexEnumToString(SkillSlotIndex value)
{
	switch (value)
	{
		case SkillSlotIndex::INVALID_SKILL_SLOT_ID: return TEXT("INVALID_SKILL_SLOT_ID");
		case SkillSlotIndex::ACTIVE_SKILL_SLOT_INDEX: return TEXT("ACTIVE_SKILL_SLOT_INDEX");
		case SkillSlotIndex::BUFF_SKILL_SLOT_INDEX: return TEXT("BUFF_SKILL_SLOT_INDEX");
		case SkillSlotIndex::CUSTUME_SKILL_SLOT_INDEX: return TEXT("CUSTUME_SKILL_SLOT_INDEX");
	}
	return FString();
}
inline FString SkillExecuteConditionMethodEnumToString(SkillExecuteConditionMethod value)
{
	switch (value)
	{
		case SkillExecuteConditionMethod::NONE: return TEXT("NONE");
		case SkillExecuteConditionMethod::ABNORMALITY: return TEXT("ABNORMALITY");
		case SkillExecuteConditionMethod::STAT: return TEXT("STAT");
		case SkillExecuteConditionMethod::PASSIVITY: return TEXT("PASSIVITY");
	}
	return FString();
}
inline FString SkillExecuteConditionStatConditionMethodEnumToString(SkillExecuteConditionStatConditionMethod value)
{
	switch (value)
	{
		case SkillExecuteConditionStatConditionMethod::LESS: return TEXT("LESS");
		case SkillExecuteConditionStatConditionMethod::MORE: return TEXT("MORE");
	}
	return FString();
}
inline FString SkillExecuteConditionStatConditionTypeEnumToString(SkillExecuteConditionStatConditionType value)
{
	switch (value)
	{
		case SkillExecuteConditionStatConditionType::RATIO: return TEXT("RATIO");
		case SkillExecuteConditionStatConditionType::VALUE: return TEXT("VALUE");
	}
	return FString();
}
inline FString SkillExecuteConditionAbnormalityConditionTypeEnumToString(SkillExecuteConditionAbnormalityConditionType value)
{
	switch (value)
	{
		case SkillExecuteConditionAbnormalityConditionType::NONE: return TEXT("NONE");
		case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_ID: return TEXT("ABNORMALITY_ID");
		case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_TYPE: return TEXT("ABNORMALITY_EFFECT_TYPE");
		case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_SUB_TYPE: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE");
		case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_GROUP_ID: return TEXT("ABNORMALITY_EFFECT_GROUP_ID");
	}
	return FString();
}
inline FString SkillTargetFilterTypeEnumToString(SkillTargetFilterType value)
{
	switch (value)
	{
		case SkillTargetFilterType::NONE: return TEXT("NONE");
		case SkillTargetFilterType::ABNORMALITY_ID: return TEXT("ABNORMALITY_ID");
		case SkillTargetFilterType::ABNORMALITY_EFFECT_TYPE: return TEXT("ABNORMALITY_EFFECT_TYPE");
		case SkillTargetFilterType::ABNORMALITY_EFFECT_SUB_TYPE: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE");
		case SkillTargetFilterType::NPC_FACTION_ID: return TEXT("NPC_FACTION_ID");
		case SkillTargetFilterType::ABNORMALITY_EFFECT_GROUP_ID: return TEXT("ABNORMALITY_EFFECT_GROUP_ID");
		case SkillTargetFilterType::PC_ONLY: return TEXT("PC_ONLY");
		case SkillTargetFilterType::NPC_ONLY: return TEXT("NPC_ONLY");
	}
	return FString();
}
inline FString SetAbnormalityAllowEnumToString(SetAbnormalityAllow value)
{
	switch (value)
	{
		case SetAbnormalityAllow::ALLOW: return TEXT("ALLOW");
		case SetAbnormalityAllow::DISALLOW: return TEXT("DISALLOW");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString SkillCategorySetEnumToDisplayKey(SkillCategorySet value)
{
	switch (value)
	{
		case SkillCategorySet::ACTIVE: return TEXT("");
		case SkillCategorySet::BUFF: return TEXT("");
		case SkillCategorySet::PASSIVE: return TEXT("");
		case SkillCategorySet::DASH: return TEXT("");
	}
	return FString();
}
inline FString SkillCategoryEnumToDisplayKey(SkillCategory value)
{
	switch (value)
	{
		case SkillCategory::NORMAL: return TEXT("");
		case SkillCategory::COMBO: return TEXT("");
		case SkillCategory::DASH: return TEXT("");
		case SkillCategory::CAST: return TEXT("");
		case SkillCategory::BUFF: return TEXT("");
		case SkillCategory::PASSIVE: return TEXT("");
	}
	return FString();
}
inline FString SkillRangeTypeEnumToDisplayKey(SkillRangeType value)
{
	switch (value)
	{
		case SkillRangeType::MELEE: return TEXT("");
		case SkillRangeType::RANGE: return TEXT("");
	}
	return FString();
}
inline FString SkillImmuneTypeEnumToDisplayKey(SkillImmuneType value)
{
	switch (value)
	{
		case SkillImmuneType::NONE: return TEXT("");
	}
	return FString();
}
inline FString SkillTargetMethodEnumToDisplayKey(SkillTargetMethod value)
{
	switch (value)
	{
		case SkillTargetMethod::TARGET: return TEXT("");
		case SkillTargetMethod::NON_TARGET: return TEXT("");
	}
	return FString();
}
inline FString SkillRestrictionTargetEnumToDisplayKey(SkillRestrictionTarget value)
{
	switch (value)
	{
		case SkillRestrictionTarget::SELF: return TEXT("");
		case SkillRestrictionTarget::TARGET: return TEXT("");
	}
	return FString();
}
inline FString SkillRestrictionTypeEnumToDisplayKey(SkillRestrictionType value)
{
	switch (value)
	{
		case SkillRestrictionType::NONE: return TEXT("");
		case SkillRestrictionType::HP_RATE_GREATER: return TEXT("");
		case SkillRestrictionType::HP_RATE_LESS: return TEXT("");
		case SkillRestrictionType::MP_RATE_GREATER: return TEXT("");
		case SkillRestrictionType::MP_RATE_LESS: return TEXT("");
		case SkillRestrictionType::ABNORMALITY_ON: return TEXT("");
		case SkillRestrictionType::ABNORMALITY_OFF: return TEXT("");
	}
	return FString();
}
inline FString SkillCollisionTypeEnumToDisplayKey(SkillCollisionType value)
{
	switch (value)
	{
		case SkillCollisionType::NONE: return TEXT("");
		case SkillCollisionType::CIRCLE: return TEXT("");
		case SkillCollisionType::FAN: return TEXT("");
		case SkillCollisionType::RECT: return TEXT("");
	}
	return FString();
}
inline FString SkillAreaCenterEnumToDisplayKey(SkillAreaCenter value)
{
	switch (value)
	{
		case SkillAreaCenter::CASTER: return TEXT("");
		case SkillAreaCenter::TARGET: return TEXT("");
	}
	return FString();
}
inline FString SkillTargetTypeEnumToDisplayKey(SkillTargetType value)
{
	switch (value)
	{
		case SkillTargetType::CASTER: return TEXT("");
		case SkillTargetType::TARGET: return TEXT("");
	}
	return FString();
}
inline FString ExecuteConditionTargetTypeEnumToDisplayKey(ExecuteConditionTargetType value)
{
	switch (value)
	{
		case ExecuteConditionTargetType::CASTER: return TEXT("");
		case ExecuteConditionTargetType::TARGET: return TEXT("");
		case ExecuteConditionTargetType::NOTIFY_TARGET: return TEXT("");
	}
	return FString();
}
inline FString HitResultTypeEnumToDisplayKey(HitResultType value)
{
	switch (value)
	{
		case HitResultType::NORMAL: return TEXT("");
		case HitResultType::DODGE: return TEXT("");
		case HitResultType::CRITICAL: return TEXT("");
		case HitResultType::GREAT: return TEXT("");
		case HitResultType::IMMUNE_DAMAGE: return TEXT("");
		case HitResultType::IMMUNE_ABNORMALITY: return TEXT("");
		case HitResultType::HEAL: return TEXT("");
		case HitResultType::BUFF_COUNTER: return TEXT("");
		case HitResultType::MISS_ABNORMALITY: return TEXT("");
	}
	return FString();
}
inline FString SkillFinishReasonEnumToDisplayKey(SkillFinishReason value)
{
	switch (value)
	{
		case SkillFinishReason::NORMAL: return TEXT("");
		case SkillFinishReason::USER_CANCEL: return TEXT("");
		case SkillFinishReason::DEATH: return TEXT("");
		case SkillFinishReason::REACTION: return TEXT("");
		case SkillFinishReason::HIT_BY_NORMAL: return TEXT("");
		case SkillFinishReason::HIT_BY_CRITICAL: return TEXT("");
		case SkillFinishReason::NO_COST: return TEXT("");
		case SkillFinishReason::COUNTER_SPELL: return TEXT("");
		case SkillFinishReason::CHANGE_AI: return TEXT("");
		case SkillFinishReason::BEHAVIOR_DISABLED: return TEXT("");
		case SkillFinishReason::SKILL_NOTIFY_EXEC_FAILED: return TEXT("");
		case SkillFinishReason::NO_TIME_LINE: return TEXT("");
	}
	return FString();
}
inline FString SkillEnchantResultEnumToDisplayKey(SkillEnchantResult value)
{
	switch (value)
	{
		case SkillEnchantResult::NONE: return TEXT("");
		case SkillEnchantResult::SUCCESS: return TEXT("");
		case SkillEnchantResult::FAILED: return TEXT("");
	}
	return FString();
}
inline FString SkillGradeEnumToDisplayKey(SkillGrade value)
{
	switch (value)
	{
		case SkillGrade::NONE: return TEXT("");
		case SkillGrade::NORMAL: return TEXT("");
		case SkillGrade::MAGIC: return TEXT("");
		case SkillGrade::RARE: return TEXT("");
		case SkillGrade::EPIC: return TEXT("");
		case SkillGrade::UNIQUE: return TEXT("");
		case SkillGrade::LEGEND: return TEXT("");
	}
	return FString();
}
inline FString SkillStartTypeEnumToDisplayKey(SkillStartType value)
{
	switch (value)
	{
		case SkillStartType::NONE: return TEXT("");
		case SkillStartType::CAST: return TEXT("");
		case SkillStartType::SKILL: return TEXT("");
	}
	return FString();
}
inline FString SkillSequenceTypeEnumToDisplayKey(SkillSequenceType value)
{
	switch (value)
	{
		case SkillSequenceType::NONE: return TEXT("");
		case SkillSequenceType::COMBO: return TEXT("");
		case SkillSequenceType::CHAIN: return TEXT("");
	}
	return FString();
}
inline FString SkillUpConsumeCostTypeEnumToDisplayKey(SkillUpConsumeCostType value)
{
	switch (value)
	{
		case SkillUpConsumeCostType::NONE: return TEXT("");
		case SkillUpConsumeCostType::ONLY_ITEM: return TEXT("");
		case SkillUpConsumeCostType::ONLY_CURRENCY: return TEXT("");
		case SkillUpConsumeCostType::BOTH: return TEXT("");
	}
	return FString();
}
inline FString SkillChainConditionEnumToDisplayKey(SkillChainCondition value)
{
	switch (value)
	{
		case SkillChainCondition::NONE: return TEXT("");
		case SkillChainCondition::PREV_SKILL: return TEXT("");
		case SkillChainCondition::HIT_PREV_SKILL: return TEXT("");
		case SkillChainCondition::PREV_SKILL_REMAIN: return TEXT("");
	}
	return FString();
}
inline FString SkillSlotIndexEnumToDisplayKey(SkillSlotIndex value)
{
	switch (value)
	{
		case SkillSlotIndex::INVALID_SKILL_SLOT_ID: return TEXT("");
		case SkillSlotIndex::ACTIVE_SKILL_SLOT_INDEX: return TEXT("");
		case SkillSlotIndex::BUFF_SKILL_SLOT_INDEX: return TEXT("");
		case SkillSlotIndex::CUSTUME_SKILL_SLOT_INDEX: return TEXT("");
	}
	return FString();
}
inline FString SkillExecuteConditionMethodEnumToDisplayKey(SkillExecuteConditionMethod value)
{
	switch (value)
	{
		case SkillExecuteConditionMethod::NONE: return TEXT("");
		case SkillExecuteConditionMethod::ABNORMALITY: return TEXT("");
		case SkillExecuteConditionMethod::STAT: return TEXT("");
		case SkillExecuteConditionMethod::PASSIVITY: return TEXT("");
	}
	return FString();
}
inline FString SkillExecuteConditionStatConditionMethodEnumToDisplayKey(SkillExecuteConditionStatConditionMethod value)
{
	switch (value)
	{
		case SkillExecuteConditionStatConditionMethod::LESS: return TEXT("");
		case SkillExecuteConditionStatConditionMethod::MORE: return TEXT("");
	}
	return FString();
}
inline FString SkillExecuteConditionStatConditionTypeEnumToDisplayKey(SkillExecuteConditionStatConditionType value)
{
	switch (value)
	{
		case SkillExecuteConditionStatConditionType::RATIO: return TEXT("");
		case SkillExecuteConditionStatConditionType::VALUE: return TEXT("");
	}
	return FString();
}
inline FString SkillExecuteConditionAbnormalityConditionTypeEnumToDisplayKey(SkillExecuteConditionAbnormalityConditionType value)
{
	switch (value)
	{
		case SkillExecuteConditionAbnormalityConditionType::NONE: return TEXT("");
		case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_ID: return TEXT("");
		case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_TYPE: return TEXT("");
		case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_SUB_TYPE: return TEXT("");
		case SkillExecuteConditionAbnormalityConditionType::ABNORMALITY_EFFECT_GROUP_ID: return TEXT("");
	}
	return FString();
}
inline FString SkillTargetFilterTypeEnumToDisplayKey(SkillTargetFilterType value)
{
	switch (value)
	{
		case SkillTargetFilterType::NONE: return TEXT("");
		case SkillTargetFilterType::ABNORMALITY_ID: return TEXT("");
		case SkillTargetFilterType::ABNORMALITY_EFFECT_TYPE: return TEXT("");
		case SkillTargetFilterType::ABNORMALITY_EFFECT_SUB_TYPE: return TEXT("");
		case SkillTargetFilterType::NPC_FACTION_ID: return TEXT("");
		case SkillTargetFilterType::ABNORMALITY_EFFECT_GROUP_ID: return TEXT("");
		case SkillTargetFilterType::PC_ONLY: return TEXT("");
		case SkillTargetFilterType::NPC_ONLY: return TEXT("");
	}
	return FString();
}
inline FString SetAbnormalityAllowEnumToDisplayKey(SetAbnormalityAllow value)
{
	switch (value)
	{
		case SetAbnormalityAllow::ALLOW: return TEXT("");
		case SetAbnormalityAllow::DISALLOW: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidSkillCategorySet(SkillCategorySet value) noexcept
{
	return (value >= SkillCategorySet::ACTIVE && value < SkillCategorySet::MAX);
}

inline bool IsValidSkillCategory(SkillCategory value) noexcept
{
	return (value >= SkillCategory::NORMAL && value < SkillCategory::MAX);
}

inline bool IsValidSkillRangeType(SkillRangeType value) noexcept
{
	return (value >= SkillRangeType::MELEE && value < SkillRangeType::MAX);
}

inline bool IsValidSkillImmuneType(SkillImmuneType value) noexcept
{
	return (value >= SkillImmuneType::NONE && value < SkillImmuneType::MAX);
}

inline bool IsValidSkillTargetMethod(SkillTargetMethod value) noexcept
{
	return (value >= SkillTargetMethod::TARGET && value < SkillTargetMethod::MAX);
}

inline bool IsValidSkillRestrictionTarget(SkillRestrictionTarget value) noexcept
{
	return (value >= SkillRestrictionTarget::SELF && value < SkillRestrictionTarget::MAX);
}

inline bool IsValidSkillRestrictionType(SkillRestrictionType value) noexcept
{
	return (value >= SkillRestrictionType::NONE && value < SkillRestrictionType::MAX);
}

inline bool IsValidSkillCollisionType(SkillCollisionType value) noexcept
{
	return (value >= SkillCollisionType::NONE && value < SkillCollisionType::MAX);
}

inline bool IsValidSkillAreaCenter(SkillAreaCenter value) noexcept
{
	return (value >= SkillAreaCenter::CASTER && value < SkillAreaCenter::MAX);
}

inline bool IsValidSkillTargetType(SkillTargetType value) noexcept
{
	return (value >= SkillTargetType::CASTER && value < SkillTargetType::MAX);
}

inline bool IsValidExecuteConditionTargetType(ExecuteConditionTargetType value) noexcept
{
	return (value >= ExecuteConditionTargetType::CASTER && value < ExecuteConditionTargetType::MAX);
}

inline bool IsValidHitResultType(HitResultType value) noexcept
{
	return (value >= HitResultType::NORMAL && value < HitResultType::MAX);
}

inline bool IsValidSkillFinishReason(SkillFinishReason value) noexcept
{
	return (value >= SkillFinishReason::NORMAL && value < SkillFinishReason::MAX);
}

inline bool IsValidSkillEnchantResult(SkillEnchantResult value) noexcept
{
	return (value >= SkillEnchantResult::NONE && value < SkillEnchantResult::MAX);
}

inline bool IsValidSkillGrade(SkillGrade value) noexcept
{
	return (value >= SkillGrade::NONE && value < SkillGrade::MAX);
}

inline bool IsValidSkillStartType(SkillStartType value) noexcept
{
	return (value >= SkillStartType::NONE && value < SkillStartType::MAX);
}

inline bool IsValidSkillSequenceType(SkillSequenceType value) noexcept
{
	return (value >= SkillSequenceType::NONE && value <= SkillSequenceType::CHAIN);
}

inline bool IsValidSkillUpConsumeCostType(SkillUpConsumeCostType value) noexcept
{
	return (value >= SkillUpConsumeCostType::NONE && value <= SkillUpConsumeCostType::BOTH);
}

inline bool IsValidSkillChainCondition(SkillChainCondition value) noexcept
{
	return (value >= SkillChainCondition::NONE && value <= SkillChainCondition::PREV_SKILL_REMAIN);
}

inline bool IsValidSkillSlotIndex(SkillSlotIndex value) noexcept
{
	return (value >= SkillSlotIndex::INVALID_SKILL_SLOT_ID && value <= SkillSlotIndex::CUSTUME_SKILL_SLOT_INDEX);
}

inline bool IsValidSkillExecuteConditionMethod(SkillExecuteConditionMethod value) noexcept
{
	return (value >= SkillExecuteConditionMethod::NONE && value < SkillExecuteConditionMethod::MAX);
}

inline bool IsValidSkillExecuteConditionStatConditionMethod(SkillExecuteConditionStatConditionMethod value) noexcept
{
	return (value >= SkillExecuteConditionStatConditionMethod::LESS && value < SkillExecuteConditionStatConditionMethod::MAX);
}

inline bool IsValidSkillExecuteConditionStatConditionType(SkillExecuteConditionStatConditionType value) noexcept
{
	return (value >= SkillExecuteConditionStatConditionType::RATIO && value < SkillExecuteConditionStatConditionType::MAX);
}

inline bool IsValidSkillExecuteConditionAbnormalityConditionType(SkillExecuteConditionAbnormalityConditionType value) noexcept
{
	return (value >= SkillExecuteConditionAbnormalityConditionType::NONE && value < SkillExecuteConditionAbnormalityConditionType::MAX);
}

inline bool IsValidSkillTargetFilterType(SkillTargetFilterType value) noexcept
{
	return (value >= SkillTargetFilterType::NONE && value < SkillTargetFilterType::MAX);
}

inline bool IsValidSetAbnormalityAllow(SetAbnormalityAllow value) noexcept
{
	return (value >= SetAbnormalityAllow::ALLOW && value < SetAbnormalityAllow::MAX);
}


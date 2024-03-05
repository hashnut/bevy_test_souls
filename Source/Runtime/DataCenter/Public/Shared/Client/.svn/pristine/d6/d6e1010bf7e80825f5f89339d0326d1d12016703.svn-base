#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedSkillNotifyEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 스킬 효과 타입
enum class SkillNotifyType : uint8
{

	// 데미지 타입 - 데미지
	DAMAGE = 0,

	// 프로젝타일 발사 - 프로젝타일 발사
	FIRE_PROJECTILE = 1,

	// 소환
	SUMMON_NPC = 2,

	// HP, MP 변화 - 힐, 마나번 등
	VARY_HP_MP = 3,

	// 스킬예시선(클라)
	SKILLGUIDE_EFFECT = 4,

	// 스킬 발동 이펙트(클라)
	SKILL_EFFECT = 5,

	// 위치 이동
	MOVE_LOCATION = 6,

	// 클라이언트, 서버간의 스킬 연출 및 애니메이션 시점을 맞추기 위한 동기화 처리 효과
	END_SECTION = 7,

	// 빔 파티클 이펙트 사용(클라)
	BEAM_PARTICLE_EFFECT = 8,

	// 명령 효과 - 특정 스킬 사용 명령 및 자신의 어그로 리스트 전달
	COMMAND = 9,

	// 가변 스탯을 타겟 대상과 공유
	STAT_DISTRIBUTION = 10,

	// 마을로 가는 기능
	WARP_TO_TOWN = 11,

	// 스킬 쿨다운 기능
	COOLDOWN = 12,

	MAX = 13,
};

UENUM(BlueprintType)
// 스킬 효과 타입
enum class AbnormalityApplyType : uint8
{

	// 적용 없음
	NONE = 0,

	// Abnormal 을 하나 추가. 이에 관련되어서 어떤 Abnormal인지는 Abnormality ID 컬럼에 기입된 값을 참조
	ADD = 1,

	// 삭제 예정
	ADD_GROUP = 2,

	// Abnormal 을 하나 제거. 이에 관련되어서 어떤 Abnormal인지는 Abnormality ID 컬럼에 기입된 값을 참조
	REMOVE = 3,

	// Abnormal 을 전부 제거. 다른 컬럼 참조하지 않고 기존 기능 기준으로 Abnormality Set 테이블의  					 //Remove ConditionSystem Forece Remove 가 체크되어 있는 모든 Abnormal 제거. Is Buff 항목 확인하지 않음.  					 //변경된 기능 기준으로는 Remove ConditionSystem Force Remove가 True인 Abnormal은 제외.
	REMOVE_ALL = 4,

	// is Buff가 체크되어 있는 Abnormal이면 다 지움.  					 //단, 변경된 기능 기준으로 Remove ConditionSystem Force Remove가 True인 Abnormal은 제외.
	REMOVE_BUFF = 5,

	// is Buff가 체크되어 있지 않는 Abnormal이면 다 지움.  					 // 단, 변경된 기능 기준으로 Remove ConditionSystem Force Remove가 True인 Abnormal은 제외.
	REMOVE_DEBUFF = 6,

	// 제거 그룹 단위로 제거할 때 사용하는 것
	REMOVE_GROUP = 7,

	// 특정 AbnormalityEffectType 모두 해제 - 삭제 예정
	REMOVE_ALL_ABNORMALITY_EFFECT_TYPE = 8,

	// 특정 AbnormalityEffectSubType 모두 해제 -- 삭제 필요
	REMOVE_ALL_ABNORMALITY_EFFECT_SUB_TYPE = 9,

	MAX = 10,
};

UENUM(BlueprintType)
// 이동 스킬 효과의 이동 구분
enum class SkillNotifyMoveType : uint8
{

	// 타겟과의 최단 위치 (distance 데이터 참조하지 않음, 캐스터를 타겟으로 할 경우 사용 불가)
	NEAR_TARGET = 0,

	// 캐스터의 위치에서 타겟 위치로의 방향으로 distance 값만큼 이동한 위치 (캐스터를 타겟으로 할 경우 사용 불가)
	TO_TARGET = 1,

	// 타겟의 위치에서 캐스터의 위치로의 방향으로 distance 값만큼 이동한 위치 (캐스터를 타겟으로 할 경우 사용 불가)
	FROM_TARGET = 2,

	// centerType(SkillAreaCenter)의 대상이 바라보는 방향의 앞 방향으로 distance 값만큼 이동한 위치
	FRONT = 3,

	// centerType(SkillAreaCenter)의 대상이 바라보는 방향의 뒷 방향으로 distance 값만큼 이동한 위치
	BACK = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 스킬 효과 타입
enum class SkillNotifyPostDirectionType : uint8
{

	// 이동 후 위치에서 타겟을 향한 방향
	TO_TARGET = 0,

	// 이동 전 캐스터가 바라보는 방향
	CASTER_ORIGIN = 1,

	// 이동 전 캐스터의 위치를 향한 방향
	BEFORE_CASTER_POS = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 소환수 스폰 타입
enum class SummoneeSpawnType : uint8
{

	// 캐스터(소환사)를 중심으로 위치 산정 (캐스터의 현재 방향, 위치 데이터 참조 + 소환 스킬 효과 정보의 방향, 거리 값 참조)
	CASTER = 0,

	// 타겟을 중심으로 위치 산정 (타겟의 현재 방향, 위치 데이터 참조 + 소환 스킬 효과 정보의 방향, 거리 값 참조)
	TARGET = 1,

	// 캐스터를 중심으로 고정 위치(캐스터의 현재 방향은 고려하지 않으며, 위치만 참조 + 소환 스킬 효과 정보의 방향, 거리 값 참조)
	FIXED = 2,

	// 영역 안의 랜덤 위치 스폰 (스킬 혹은 스킬 효과의 영역 안의 랜덤한 위치 스폰, 소환 스킬 효과 정보의 방향, 거리 값 참조 안함)
	RANDOM = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 힐 스킬 효과 비율 관련 타입
enum class HealRatioValueType : uint8
{

	// 비율 사용하지 않음
	NONE = 0,

	// 전투 공식 상 최초 대미지 계산 참고
	ATTACK = 1,

	// 최대 체력
	MAX_HP = 2,

	// 현재 체력
	CURRENT_HP = 3,

	// 최대 마력
	MAX_MP = 4,

	// 현재 마력
	CURRENT_MP = 5,

	MAX = 6,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline SkillNotifyType SkillNotifyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("DAMAGE"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::DAMAGE;
	if (str.Compare(TEXT("FIRE_PROJECTILE"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::FIRE_PROJECTILE;
	if (str.Compare(TEXT("SUMMON_NPC"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::SUMMON_NPC;
	if (str.Compare(TEXT("VARY_HP_MP"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::VARY_HP_MP;
	if (str.Compare(TEXT("SKILLGUIDE_EFFECT"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::SKILLGUIDE_EFFECT;
	if (str.Compare(TEXT("SKILL_EFFECT"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::SKILL_EFFECT;
	if (str.Compare(TEXT("MOVE_LOCATION"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::MOVE_LOCATION;
	if (str.Compare(TEXT("END_SECTION"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::END_SECTION;
	if (str.Compare(TEXT("BEAM_PARTICLE_EFFECT"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::BEAM_PARTICLE_EFFECT;
	if (str.Compare(TEXT("COMMAND"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::COMMAND;
	if (str.Compare(TEXT("STAT_DISTRIBUTION"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::STAT_DISTRIBUTION;
	if (str.Compare(TEXT("WARP_TO_TOWN"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::WARP_TO_TOWN;
	if (str.Compare(TEXT("COOLDOWN"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyType::COOLDOWN;
	return SkillNotifyType::MAX;
}
inline AbnormalityApplyType AbnormalityApplyTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityApplyType::NONE;
	if (str.Compare(TEXT("ADD"), ESearchCase::IgnoreCase) == 0)	return AbnormalityApplyType::ADD;
	if (str.Compare(TEXT("ADD_GROUP"), ESearchCase::IgnoreCase) == 0)	return AbnormalityApplyType::ADD_GROUP;
	if (str.Compare(TEXT("REMOVE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityApplyType::REMOVE;
	if (str.Compare(TEXT("REMOVE_ALL"), ESearchCase::IgnoreCase) == 0)	return AbnormalityApplyType::REMOVE_ALL;
	if (str.Compare(TEXT("REMOVE_BUFF"), ESearchCase::IgnoreCase) == 0)	return AbnormalityApplyType::REMOVE_BUFF;
	if (str.Compare(TEXT("REMOVE_DEBUFF"), ESearchCase::IgnoreCase) == 0)	return AbnormalityApplyType::REMOVE_DEBUFF;
	if (str.Compare(TEXT("REMOVE_GROUP"), ESearchCase::IgnoreCase) == 0)	return AbnormalityApplyType::REMOVE_GROUP;
	if (str.Compare(TEXT("REMOVE_ALL_ABNORMALITY_EFFECT_TYPE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityApplyType::REMOVE_ALL_ABNORMALITY_EFFECT_TYPE;
	if (str.Compare(TEXT("REMOVE_ALL_ABNORMALITY_EFFECT_SUB_TYPE"), ESearchCase::IgnoreCase) == 0)	return AbnormalityApplyType::REMOVE_ALL_ABNORMALITY_EFFECT_SUB_TYPE;
	return AbnormalityApplyType::MAX;
}
inline SkillNotifyMoveType SkillNotifyMoveTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NEAR_TARGET"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyMoveType::NEAR_TARGET;
	if (str.Compare(TEXT("TO_TARGET"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyMoveType::TO_TARGET;
	if (str.Compare(TEXT("FROM_TARGET"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyMoveType::FROM_TARGET;
	if (str.Compare(TEXT("FRONT"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyMoveType::FRONT;
	if (str.Compare(TEXT("BACK"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyMoveType::BACK;
	return SkillNotifyMoveType::MAX;
}
inline SkillNotifyPostDirectionType SkillNotifyPostDirectionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("TO_TARGET"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyPostDirectionType::TO_TARGET;
	if (str.Compare(TEXT("CASTER_ORIGIN"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyPostDirectionType::CASTER_ORIGIN;
	if (str.Compare(TEXT("BEFORE_CASTER_POS"), ESearchCase::IgnoreCase) == 0)	return SkillNotifyPostDirectionType::BEFORE_CASTER_POS;
	return SkillNotifyPostDirectionType::MAX;
}
inline SummoneeSpawnType SummoneeSpawnTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CASTER"), ESearchCase::IgnoreCase) == 0)	return SummoneeSpawnType::CASTER;
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return SummoneeSpawnType::TARGET;
	if (str.Compare(TEXT("FIXED"), ESearchCase::IgnoreCase) == 0)	return SummoneeSpawnType::FIXED;
	if (str.Compare(TEXT("RANDOM"), ESearchCase::IgnoreCase) == 0)	return SummoneeSpawnType::RANDOM;
	return SummoneeSpawnType::MAX;
}
inline HealRatioValueType HealRatioValueTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return HealRatioValueType::NONE;
	if (str.Compare(TEXT("ATTACK"), ESearchCase::IgnoreCase) == 0)	return HealRatioValueType::ATTACK;
	if (str.Compare(TEXT("MAX_HP"), ESearchCase::IgnoreCase) == 0)	return HealRatioValueType::MAX_HP;
	if (str.Compare(TEXT("CURRENT_HP"), ESearchCase::IgnoreCase) == 0)	return HealRatioValueType::CURRENT_HP;
	if (str.Compare(TEXT("MAX_MP"), ESearchCase::IgnoreCase) == 0)	return HealRatioValueType::MAX_MP;
	if (str.Compare(TEXT("CURRENT_MP"), ESearchCase::IgnoreCase) == 0)	return HealRatioValueType::CURRENT_MP;
	return HealRatioValueType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString SkillNotifyTypeEnumToString(SkillNotifyType value)
{
	switch (value)
	{
		case SkillNotifyType::DAMAGE: return TEXT("DAMAGE");
		case SkillNotifyType::FIRE_PROJECTILE: return TEXT("FIRE_PROJECTILE");
		case SkillNotifyType::SUMMON_NPC: return TEXT("SUMMON_NPC");
		case SkillNotifyType::VARY_HP_MP: return TEXT("VARY_HP_MP");
		case SkillNotifyType::SKILLGUIDE_EFFECT: return TEXT("SKILLGUIDE_EFFECT");
		case SkillNotifyType::SKILL_EFFECT: return TEXT("SKILL_EFFECT");
		case SkillNotifyType::MOVE_LOCATION: return TEXT("MOVE_LOCATION");
		case SkillNotifyType::END_SECTION: return TEXT("END_SECTION");
		case SkillNotifyType::BEAM_PARTICLE_EFFECT: return TEXT("BEAM_PARTICLE_EFFECT");
		case SkillNotifyType::COMMAND: return TEXT("COMMAND");
		case SkillNotifyType::STAT_DISTRIBUTION: return TEXT("STAT_DISTRIBUTION");
		case SkillNotifyType::WARP_TO_TOWN: return TEXT("WARP_TO_TOWN");
		case SkillNotifyType::COOLDOWN: return TEXT("COOLDOWN");
	}
	return FString();
}
inline FString AbnormalityApplyTypeEnumToString(AbnormalityApplyType value)
{
	switch (value)
	{
		case AbnormalityApplyType::NONE: return TEXT("NONE");
		case AbnormalityApplyType::ADD: return TEXT("ADD");
		case AbnormalityApplyType::ADD_GROUP: return TEXT("ADD_GROUP");
		case AbnormalityApplyType::REMOVE: return TEXT("REMOVE");
		case AbnormalityApplyType::REMOVE_ALL: return TEXT("REMOVE_ALL");
		case AbnormalityApplyType::REMOVE_BUFF: return TEXT("REMOVE_BUFF");
		case AbnormalityApplyType::REMOVE_DEBUFF: return TEXT("REMOVE_DEBUFF");
		case AbnormalityApplyType::REMOVE_GROUP: return TEXT("REMOVE_GROUP");
		case AbnormalityApplyType::REMOVE_ALL_ABNORMALITY_EFFECT_TYPE: return TEXT("REMOVE_ALL_ABNORMALITY_EFFECT_TYPE");
		case AbnormalityApplyType::REMOVE_ALL_ABNORMALITY_EFFECT_SUB_TYPE: return TEXT("REMOVE_ALL_ABNORMALITY_EFFECT_SUB_TYPE");
	}
	return FString();
}
inline FString SkillNotifyMoveTypeEnumToString(SkillNotifyMoveType value)
{
	switch (value)
	{
		case SkillNotifyMoveType::NEAR_TARGET: return TEXT("NEAR_TARGET");
		case SkillNotifyMoveType::TO_TARGET: return TEXT("TO_TARGET");
		case SkillNotifyMoveType::FROM_TARGET: return TEXT("FROM_TARGET");
		case SkillNotifyMoveType::FRONT: return TEXT("FRONT");
		case SkillNotifyMoveType::BACK: return TEXT("BACK");
	}
	return FString();
}
inline FString SkillNotifyPostDirectionTypeEnumToString(SkillNotifyPostDirectionType value)
{
	switch (value)
	{
		case SkillNotifyPostDirectionType::TO_TARGET: return TEXT("TO_TARGET");
		case SkillNotifyPostDirectionType::CASTER_ORIGIN: return TEXT("CASTER_ORIGIN");
		case SkillNotifyPostDirectionType::BEFORE_CASTER_POS: return TEXT("BEFORE_CASTER_POS");
	}
	return FString();
}
inline FString SummoneeSpawnTypeEnumToString(SummoneeSpawnType value)
{
	switch (value)
	{
		case SummoneeSpawnType::CASTER: return TEXT("CASTER");
		case SummoneeSpawnType::TARGET: return TEXT("TARGET");
		case SummoneeSpawnType::FIXED: return TEXT("FIXED");
		case SummoneeSpawnType::RANDOM: return TEXT("RANDOM");
	}
	return FString();
}
inline FString HealRatioValueTypeEnumToString(HealRatioValueType value)
{
	switch (value)
	{
		case HealRatioValueType::NONE: return TEXT("NONE");
		case HealRatioValueType::ATTACK: return TEXT("ATTACK");
		case HealRatioValueType::MAX_HP: return TEXT("MAX_HP");
		case HealRatioValueType::CURRENT_HP: return TEXT("CURRENT_HP");
		case HealRatioValueType::MAX_MP: return TEXT("MAX_MP");
		case HealRatioValueType::CURRENT_MP: return TEXT("CURRENT_MP");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString SkillNotifyTypeEnumToDisplayKey(SkillNotifyType value)
{
	switch (value)
	{
		case SkillNotifyType::DAMAGE: return TEXT("");
		case SkillNotifyType::FIRE_PROJECTILE: return TEXT("");
		case SkillNotifyType::SUMMON_NPC: return TEXT("");
		case SkillNotifyType::VARY_HP_MP: return TEXT("");
		case SkillNotifyType::SKILLGUIDE_EFFECT: return TEXT("");
		case SkillNotifyType::SKILL_EFFECT: return TEXT("");
		case SkillNotifyType::MOVE_LOCATION: return TEXT("");
		case SkillNotifyType::END_SECTION: return TEXT("");
		case SkillNotifyType::BEAM_PARTICLE_EFFECT: return TEXT("");
		case SkillNotifyType::COMMAND: return TEXT("");
		case SkillNotifyType::STAT_DISTRIBUTION: return TEXT("");
		case SkillNotifyType::WARP_TO_TOWN: return TEXT("");
		case SkillNotifyType::COOLDOWN: return TEXT("");
	}
	return FString();
}
inline FString AbnormalityApplyTypeEnumToDisplayKey(AbnormalityApplyType value)
{
	switch (value)
	{
		case AbnormalityApplyType::NONE: return TEXT("");
		case AbnormalityApplyType::ADD: return TEXT("");
		case AbnormalityApplyType::ADD_GROUP: return TEXT("");
		case AbnormalityApplyType::REMOVE: return TEXT("");
		case AbnormalityApplyType::REMOVE_ALL: return TEXT("");
		case AbnormalityApplyType::REMOVE_BUFF: return TEXT("");
		case AbnormalityApplyType::REMOVE_DEBUFF: return TEXT("");
		case AbnormalityApplyType::REMOVE_GROUP: return TEXT("");
		case AbnormalityApplyType::REMOVE_ALL_ABNORMALITY_EFFECT_TYPE: return TEXT("");
		case AbnormalityApplyType::REMOVE_ALL_ABNORMALITY_EFFECT_SUB_TYPE: return TEXT("");
	}
	return FString();
}
inline FString SkillNotifyMoveTypeEnumToDisplayKey(SkillNotifyMoveType value)
{
	switch (value)
	{
		case SkillNotifyMoveType::NEAR_TARGET: return TEXT("");
		case SkillNotifyMoveType::TO_TARGET: return TEXT("");
		case SkillNotifyMoveType::FROM_TARGET: return TEXT("");
		case SkillNotifyMoveType::FRONT: return TEXT("");
		case SkillNotifyMoveType::BACK: return TEXT("");
	}
	return FString();
}
inline FString SkillNotifyPostDirectionTypeEnumToDisplayKey(SkillNotifyPostDirectionType value)
{
	switch (value)
	{
		case SkillNotifyPostDirectionType::TO_TARGET: return TEXT("");
		case SkillNotifyPostDirectionType::CASTER_ORIGIN: return TEXT("");
		case SkillNotifyPostDirectionType::BEFORE_CASTER_POS: return TEXT("");
	}
	return FString();
}
inline FString SummoneeSpawnTypeEnumToDisplayKey(SummoneeSpawnType value)
{
	switch (value)
	{
		case SummoneeSpawnType::CASTER: return TEXT("");
		case SummoneeSpawnType::TARGET: return TEXT("");
		case SummoneeSpawnType::FIXED: return TEXT("");
		case SummoneeSpawnType::RANDOM: return TEXT("");
	}
	return FString();
}
inline FString HealRatioValueTypeEnumToDisplayKey(HealRatioValueType value)
{
	switch (value)
	{
		case HealRatioValueType::NONE: return TEXT("");
		case HealRatioValueType::ATTACK: return TEXT("");
		case HealRatioValueType::MAX_HP: return TEXT("");
		case HealRatioValueType::CURRENT_HP: return TEXT("");
		case HealRatioValueType::MAX_MP: return TEXT("");
		case HealRatioValueType::CURRENT_MP: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidSkillNotifyType(SkillNotifyType value) noexcept
{
	return (value >= SkillNotifyType::DAMAGE && value < SkillNotifyType::MAX);
}

inline bool IsValidAbnormalityApplyType(AbnormalityApplyType value) noexcept
{
	return (value >= AbnormalityApplyType::NONE && value < AbnormalityApplyType::MAX);
}

inline bool IsValidSkillNotifyMoveType(SkillNotifyMoveType value) noexcept
{
	return (value >= SkillNotifyMoveType::NEAR_TARGET && value < SkillNotifyMoveType::MAX);
}

inline bool IsValidSkillNotifyPostDirectionType(SkillNotifyPostDirectionType value) noexcept
{
	return (value >= SkillNotifyPostDirectionType::TO_TARGET && value < SkillNotifyPostDirectionType::MAX);
}

inline bool IsValidSummoneeSpawnType(SummoneeSpawnType value) noexcept
{
	return (value >= SummoneeSpawnType::CASTER && value < SummoneeSpawnType::MAX);
}

inline bool IsValidHealRatioValueType(HealRatioValueType value) noexcept
{
	return (value >= HealRatioValueType::NONE && value < HealRatioValueType::MAX);
}


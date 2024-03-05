#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedCombatEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 스킬 카테고리 분류
enum class DamageCategory : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// 물리
	PHYSIC = 1,

	// 마법
	MAGIC = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 스킬 데미지 속성 타입
enum class DamageElementalType : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// 암흑
	DARK = 1,

	// 신성
	HOLY = 2,

	// 화염
	FLAME = 3,

	// 냉기
	FROST = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 공격 타입
enum class AttackType : uint8
{

	// 근거리
	MELEE = 0,

	// 원거리
	RANGE = 1,

	// 마법
	MAGIC = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 데미지 받을 스탯 타입
enum class DamageStatType : uint8
{

	// 아무의미없음 - (history:AbnormalityOverTime.SENSITIVE 에서 안쓰기에 추가)
	NONE = 0,

	// 체력 - StatType::HP
	HP = 1,

	// 체력 보호막 - StatType::HP_SHIELD
	HP_SHIELD = 2,

	// 마나 포인트 - StatType::MP
	MP = 3,

	// 살기 포인트 - StatType::SENSITIVE
	SENSITIVE = 4,

	MAX = 5,
};

UENUM()
// 데미지를 가한 타입
enum class DamageByType : uint8
{

	// 스킬로 인한 데미지
	SKILL = 0,

	// Overtime류로 인한 데미지 (Ex. DOT)
	OVERTIME = 1,

	// 반격 : AbnormalityEffectType::UTIL_BUFF_COUNTER
	BUFF_COUNTER = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 데미지를 가한 타입
enum class HealByType : uint8
{

	// Item에 의한 힐
	ITEM = 0,

	// 스킬에 의한 힐
	SKILL = 1,

	// Abnormality에 의한 힐
	ABNORMALITY_HP = 2,

	// Abnormality에 의한 힐
	ABNORMALITY_HP_SHIELD = 3,

	// Abnormality에 의한 힐
	ABNORMALITY_MP = 4,

	// Abnormality에 의한 힐
	ABNORMALITY_SENSITIVE = 5,

	// 흡혈에 의한 힐
	ABSORPTION = 6,

	MAX = 7,
};

UENUM()
// 전투 텍스트 위치 기준
enum class CombatTextPivot : uint8
{

	// 상단
	TOP = 0,

	// 하단
	BOTTOM = 1,

	// 중앙
	CENTER = 2,

	MAX = 3,
};

UENUM()
// 전투 옵션 타입
enum class CombatOptionType : uint8
{

	// 내가 타겟으로 지정한 대상
	TARGET = 0,

	// 적대 기사단
	ENEMY_GUILD = 1,

	// 원수
	ENEMY = 2,

	// 카오
	ENEMY_USER = 3,

	// 모든유저
	ALL_USER = 4,

	// 나를 공격주인 몬스터
	ATTACK_ME_NPC = 5,

	// 퀘스트 목표
	QUEST_TARGET = 6,

	// 선공 몬스터
	FIRST_NPC = 7,

	// 나와 적대인 캐릭터
	ENEMY_ME = 8,

	// ATTACKED_ME_NPC
	ATTACKED_ME_NPC = 9,

	MAX = 10,
};

UENUM()
// 전투 텍스트 유형
enum class CombatTextType : uint8
{

	// 일반 데미지
	NORMAL = 0,

	// 크리티컬 데미지
	CRITICAL = 1,

	// DOT 데미지
	DOT = 2,

	// 힐
	HEAL = 3,

	// 공유
	SHARED = 4,

	// 플레이어용 일반데미지
	NORMAL_FOR_PLAYER = 5,

	// 플레이어용 크리티컬
	CRITICAL_FOR_PLAYER = 6,

	// 플레이어용 도트데미지
	DOT_FOR_PLAYER = 7,

	// 플레이어용 힐
	HEAL_FOR_PLAYER = 8,

	// 플레이어용 공유
	SHARED_FOR_PLAYER = 9,

	// MP
	MP = 10,

	// 경험치
	EXP = 11,

	MAX = 12,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline DamageCategory DamageCategoryStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return DamageCategory::NONE;
	if (str.Compare(TEXT("PHYSIC"), ESearchCase::IgnoreCase) == 0)	return DamageCategory::PHYSIC;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return DamageCategory::MAGIC;
	return DamageCategory::MAX;
}
inline DamageElementalType DamageElementalTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return DamageElementalType::NONE;
	if (str.Compare(TEXT("DARK"), ESearchCase::IgnoreCase) == 0)	return DamageElementalType::DARK;
	if (str.Compare(TEXT("HOLY"), ESearchCase::IgnoreCase) == 0)	return DamageElementalType::HOLY;
	if (str.Compare(TEXT("FLAME"), ESearchCase::IgnoreCase) == 0)	return DamageElementalType::FLAME;
	if (str.Compare(TEXT("FROST"), ESearchCase::IgnoreCase) == 0)	return DamageElementalType::FROST;
	return DamageElementalType::MAX;
}
inline AttackType AttackTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("MELEE"), ESearchCase::IgnoreCase) == 0)	return AttackType::MELEE;
	if (str.Compare(TEXT("RANGE"), ESearchCase::IgnoreCase) == 0)	return AttackType::RANGE;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return AttackType::MAGIC;
	return AttackType::MAX;
}
inline DamageStatType DamageStatTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return DamageStatType::NONE;
	if (str.Compare(TEXT("HP"), ESearchCase::IgnoreCase) == 0)	return DamageStatType::HP;
	if (str.Compare(TEXT("HP_SHIELD"), ESearchCase::IgnoreCase) == 0)	return DamageStatType::HP_SHIELD;
	if (str.Compare(TEXT("MP"), ESearchCase::IgnoreCase) == 0)	return DamageStatType::MP;
	if (str.Compare(TEXT("SENSITIVE"), ESearchCase::IgnoreCase) == 0)	return DamageStatType::SENSITIVE;
	return DamageStatType::MAX;
}
inline DamageByType DamageByTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("SKILL"), ESearchCase::IgnoreCase) == 0)	return DamageByType::SKILL;
	if (str.Compare(TEXT("OVERTIME"), ESearchCase::IgnoreCase) == 0)	return DamageByType::OVERTIME;
	if (str.Compare(TEXT("BUFF_COUNTER"), ESearchCase::IgnoreCase) == 0)	return DamageByType::BUFF_COUNTER;
	return DamageByType::MAX;
}
inline HealByType HealByTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ITEM"), ESearchCase::IgnoreCase) == 0)	return HealByType::ITEM;
	if (str.Compare(TEXT("SKILL"), ESearchCase::IgnoreCase) == 0)	return HealByType::SKILL;
	if (str.Compare(TEXT("ABNORMALITY_HP"), ESearchCase::IgnoreCase) == 0)	return HealByType::ABNORMALITY_HP;
	if (str.Compare(TEXT("ABNORMALITY_HP_SHIELD"), ESearchCase::IgnoreCase) == 0)	return HealByType::ABNORMALITY_HP_SHIELD;
	if (str.Compare(TEXT("ABNORMALITY_MP"), ESearchCase::IgnoreCase) == 0)	return HealByType::ABNORMALITY_MP;
	if (str.Compare(TEXT("ABNORMALITY_SENSITIVE"), ESearchCase::IgnoreCase) == 0)	return HealByType::ABNORMALITY_SENSITIVE;
	if (str.Compare(TEXT("ABSORPTION"), ESearchCase::IgnoreCase) == 0)	return HealByType::ABSORPTION;
	return HealByType::MAX;
}
inline CombatTextPivot CombatTextPivotStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("TOP"), ESearchCase::IgnoreCase) == 0)	return CombatTextPivot::TOP;
	if (str.Compare(TEXT("BOTTOM"), ESearchCase::IgnoreCase) == 0)	return CombatTextPivot::BOTTOM;
	if (str.Compare(TEXT("CENTER"), ESearchCase::IgnoreCase) == 0)	return CombatTextPivot::CENTER;
	return CombatTextPivot::MAX;
}
inline CombatOptionType CombatOptionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return CombatOptionType::TARGET;
	if (str.Compare(TEXT("ENEMY_GUILD"), ESearchCase::IgnoreCase) == 0)	return CombatOptionType::ENEMY_GUILD;
	if (str.Compare(TEXT("ENEMY"), ESearchCase::IgnoreCase) == 0)	return CombatOptionType::ENEMY;
	if (str.Compare(TEXT("ENEMY_USER"), ESearchCase::IgnoreCase) == 0)	return CombatOptionType::ENEMY_USER;
	if (str.Compare(TEXT("ALL_USER"), ESearchCase::IgnoreCase) == 0)	return CombatOptionType::ALL_USER;
	if (str.Compare(TEXT("ATTACK_ME_NPC"), ESearchCase::IgnoreCase) == 0)	return CombatOptionType::ATTACK_ME_NPC;
	if (str.Compare(TEXT("QUEST_TARGET"), ESearchCase::IgnoreCase) == 0)	return CombatOptionType::QUEST_TARGET;
	if (str.Compare(TEXT("FIRST_NPC"), ESearchCase::IgnoreCase) == 0)	return CombatOptionType::FIRST_NPC;
	if (str.Compare(TEXT("ENEMY_ME"), ESearchCase::IgnoreCase) == 0)	return CombatOptionType::ENEMY_ME;
	if (str.Compare(TEXT("ATTACKED_ME_NPC"), ESearchCase::IgnoreCase) == 0)	return CombatOptionType::ATTACKED_ME_NPC;
	return CombatOptionType::MAX;
}
inline CombatTextType CombatTextTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::NORMAL;
	if (str.Compare(TEXT("CRITICAL"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::CRITICAL;
	if (str.Compare(TEXT("DOT"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::DOT;
	if (str.Compare(TEXT("HEAL"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::HEAL;
	if (str.Compare(TEXT("SHARED"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::SHARED;
	if (str.Compare(TEXT("NORMAL_FOR_PLAYER"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::NORMAL_FOR_PLAYER;
	if (str.Compare(TEXT("CRITICAL_FOR_PLAYER"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::CRITICAL_FOR_PLAYER;
	if (str.Compare(TEXT("DOT_FOR_PLAYER"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::DOT_FOR_PLAYER;
	if (str.Compare(TEXT("HEAL_FOR_PLAYER"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::HEAL_FOR_PLAYER;
	if (str.Compare(TEXT("SHARED_FOR_PLAYER"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::SHARED_FOR_PLAYER;
	if (str.Compare(TEXT("MP"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::MP;
	if (str.Compare(TEXT("EXP"), ESearchCase::IgnoreCase) == 0)	return CombatTextType::EXP;
	return CombatTextType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString DamageCategoryEnumToString(DamageCategory value)
{
	switch (value)
	{
		case DamageCategory::NONE: return TEXT("NONE");
		case DamageCategory::PHYSIC: return TEXT("PHYSIC");
		case DamageCategory::MAGIC: return TEXT("MAGIC");
	}
	return FString();
}
inline FString DamageElementalTypeEnumToString(DamageElementalType value)
{
	switch (value)
	{
		case DamageElementalType::NONE: return TEXT("NONE");
		case DamageElementalType::DARK: return TEXT("DARK");
		case DamageElementalType::HOLY: return TEXT("HOLY");
		case DamageElementalType::FLAME: return TEXT("FLAME");
		case DamageElementalType::FROST: return TEXT("FROST");
	}
	return FString();
}
inline FString AttackTypeEnumToString(AttackType value)
{
	switch (value)
	{
		case AttackType::MELEE: return TEXT("MELEE");
		case AttackType::RANGE: return TEXT("RANGE");
		case AttackType::MAGIC: return TEXT("MAGIC");
	}
	return FString();
}
inline FString DamageStatTypeEnumToString(DamageStatType value)
{
	switch (value)
	{
		case DamageStatType::NONE: return TEXT("NONE");
		case DamageStatType::HP: return TEXT("HP");
		case DamageStatType::HP_SHIELD: return TEXT("HP_SHIELD");
		case DamageStatType::MP: return TEXT("MP");
		case DamageStatType::SENSITIVE: return TEXT("SENSITIVE");
	}
	return FString();
}
inline FString DamageByTypeEnumToString(DamageByType value)
{
	switch (value)
	{
		case DamageByType::SKILL: return TEXT("SKILL");
		case DamageByType::OVERTIME: return TEXT("OVERTIME");
		case DamageByType::BUFF_COUNTER: return TEXT("BUFF_COUNTER");
	}
	return FString();
}
inline FString HealByTypeEnumToString(HealByType value)
{
	switch (value)
	{
		case HealByType::ITEM: return TEXT("ITEM");
		case HealByType::SKILL: return TEXT("SKILL");
		case HealByType::ABNORMALITY_HP: return TEXT("ABNORMALITY_HP");
		case HealByType::ABNORMALITY_HP_SHIELD: return TEXT("ABNORMALITY_HP_SHIELD");
		case HealByType::ABNORMALITY_MP: return TEXT("ABNORMALITY_MP");
		case HealByType::ABNORMALITY_SENSITIVE: return TEXT("ABNORMALITY_SENSITIVE");
		case HealByType::ABSORPTION: return TEXT("ABSORPTION");
	}
	return FString();
}
inline FString CombatTextPivotEnumToString(CombatTextPivot value)
{
	switch (value)
	{
		case CombatTextPivot::TOP: return TEXT("TOP");
		case CombatTextPivot::BOTTOM: return TEXT("BOTTOM");
		case CombatTextPivot::CENTER: return TEXT("CENTER");
	}
	return FString();
}
inline FString CombatOptionTypeEnumToString(CombatOptionType value)
{
	switch (value)
	{
		case CombatOptionType::TARGET: return TEXT("TARGET");
		case CombatOptionType::ENEMY_GUILD: return TEXT("ENEMY_GUILD");
		case CombatOptionType::ENEMY: return TEXT("ENEMY");
		case CombatOptionType::ENEMY_USER: return TEXT("ENEMY_USER");
		case CombatOptionType::ALL_USER: return TEXT("ALL_USER");
		case CombatOptionType::ATTACK_ME_NPC: return TEXT("ATTACK_ME_NPC");
		case CombatOptionType::QUEST_TARGET: return TEXT("QUEST_TARGET");
		case CombatOptionType::FIRST_NPC: return TEXT("FIRST_NPC");
		case CombatOptionType::ENEMY_ME: return TEXT("ENEMY_ME");
		case CombatOptionType::ATTACKED_ME_NPC: return TEXT("ATTACKED_ME_NPC");
	}
	return FString();
}
inline FString CombatTextTypeEnumToString(CombatTextType value)
{
	switch (value)
	{
		case CombatTextType::NORMAL: return TEXT("NORMAL");
		case CombatTextType::CRITICAL: return TEXT("CRITICAL");
		case CombatTextType::DOT: return TEXT("DOT");
		case CombatTextType::HEAL: return TEXT("HEAL");
		case CombatTextType::SHARED: return TEXT("SHARED");
		case CombatTextType::NORMAL_FOR_PLAYER: return TEXT("NORMAL_FOR_PLAYER");
		case CombatTextType::CRITICAL_FOR_PLAYER: return TEXT("CRITICAL_FOR_PLAYER");
		case CombatTextType::DOT_FOR_PLAYER: return TEXT("DOT_FOR_PLAYER");
		case CombatTextType::HEAL_FOR_PLAYER: return TEXT("HEAL_FOR_PLAYER");
		case CombatTextType::SHARED_FOR_PLAYER: return TEXT("SHARED_FOR_PLAYER");
		case CombatTextType::MP: return TEXT("MP");
		case CombatTextType::EXP: return TEXT("EXP");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString DamageCategoryEnumToDisplayKey(DamageCategory value)
{
	switch (value)
	{
		case DamageCategory::NONE: return TEXT("");
		case DamageCategory::PHYSIC: return TEXT("");
		case DamageCategory::MAGIC: return TEXT("");
	}
	return FString();
}
inline FString DamageElementalTypeEnumToDisplayKey(DamageElementalType value)
{
	switch (value)
	{
		case DamageElementalType::NONE: return TEXT("");
		case DamageElementalType::DARK: return TEXT("");
		case DamageElementalType::HOLY: return TEXT("");
		case DamageElementalType::FLAME: return TEXT("");
		case DamageElementalType::FROST: return TEXT("");
	}
	return FString();
}
inline FString AttackTypeEnumToDisplayKey(AttackType value)
{
	switch (value)
	{
		case AttackType::MELEE: return TEXT("");
		case AttackType::RANGE: return TEXT("");
		case AttackType::MAGIC: return TEXT("");
	}
	return FString();
}
inline FString DamageStatTypeEnumToDisplayKey(DamageStatType value)
{
	switch (value)
	{
		case DamageStatType::NONE: return TEXT("");
		case DamageStatType::HP: return TEXT("");
		case DamageStatType::HP_SHIELD: return TEXT("");
		case DamageStatType::MP: return TEXT("");
		case DamageStatType::SENSITIVE: return TEXT("");
	}
	return FString();
}
inline FString DamageByTypeEnumToDisplayKey(DamageByType value)
{
	switch (value)
	{
		case DamageByType::SKILL: return TEXT("");
		case DamageByType::OVERTIME: return TEXT("");
		case DamageByType::BUFF_COUNTER: return TEXT("");
	}
	return FString();
}
inline FString HealByTypeEnumToDisplayKey(HealByType value)
{
	switch (value)
	{
		case HealByType::ITEM: return TEXT("");
		case HealByType::SKILL: return TEXT("");
		case HealByType::ABNORMALITY_HP: return TEXT("");
		case HealByType::ABNORMALITY_HP_SHIELD: return TEXT("");
		case HealByType::ABNORMALITY_MP: return TEXT("");
		case HealByType::ABNORMALITY_SENSITIVE: return TEXT("");
		case HealByType::ABSORPTION: return TEXT("");
	}
	return FString();
}
inline FString CombatTextPivotEnumToDisplayKey(CombatTextPivot value)
{
	switch (value)
	{
		case CombatTextPivot::TOP: return TEXT("");
		case CombatTextPivot::BOTTOM: return TEXT("");
		case CombatTextPivot::CENTER: return TEXT("");
	}
	return FString();
}
inline FString CombatOptionTypeEnumToDisplayKey(CombatOptionType value)
{
	switch (value)
	{
		case CombatOptionType::TARGET: return TEXT("");
		case CombatOptionType::ENEMY_GUILD: return TEXT("");
		case CombatOptionType::ENEMY: return TEXT("");
		case CombatOptionType::ENEMY_USER: return TEXT("");
		case CombatOptionType::ALL_USER: return TEXT("");
		case CombatOptionType::ATTACK_ME_NPC: return TEXT("");
		case CombatOptionType::QUEST_TARGET: return TEXT("");
		case CombatOptionType::FIRST_NPC: return TEXT("");
		case CombatOptionType::ENEMY_ME: return TEXT("");
		case CombatOptionType::ATTACKED_ME_NPC: return TEXT("");
	}
	return FString();
}
inline FString CombatTextTypeEnumToDisplayKey(CombatTextType value)
{
	switch (value)
	{
		case CombatTextType::NORMAL: return TEXT("");
		case CombatTextType::CRITICAL: return TEXT("");
		case CombatTextType::DOT: return TEXT("");
		case CombatTextType::HEAL: return TEXT("");
		case CombatTextType::SHARED: return TEXT("");
		case CombatTextType::NORMAL_FOR_PLAYER: return TEXT("");
		case CombatTextType::CRITICAL_FOR_PLAYER: return TEXT("");
		case CombatTextType::DOT_FOR_PLAYER: return TEXT("");
		case CombatTextType::HEAL_FOR_PLAYER: return TEXT("");
		case CombatTextType::SHARED_FOR_PLAYER: return TEXT("");
		case CombatTextType::MP: return TEXT("");
		case CombatTextType::EXP: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidDamageCategory(DamageCategory value) noexcept
{
	return (value >= DamageCategory::NONE && value < DamageCategory::MAX);
}

inline bool IsValidDamageElementalType(DamageElementalType value) noexcept
{
	return (value >= DamageElementalType::NONE && value < DamageElementalType::MAX);
}

inline bool IsValidAttackType(AttackType value) noexcept
{
	return (value >= AttackType::MELEE && value < AttackType::MAX);
}

inline bool IsValidDamageStatType(DamageStatType value) noexcept
{
	return (value >= DamageStatType::NONE && value < DamageStatType::MAX);
}

inline bool IsValidDamageByType(DamageByType value) noexcept
{
	return (value >= DamageByType::SKILL && value < DamageByType::MAX);
}

inline bool IsValidHealByType(HealByType value) noexcept
{
	return (value >= HealByType::ITEM && value < HealByType::MAX);
}

inline bool IsValidCombatTextPivot(CombatTextPivot value) noexcept
{
	return (value >= CombatTextPivot::TOP && value < CombatTextPivot::MAX);
}

inline bool IsValidCombatOptionType(CombatOptionType value) noexcept
{
	return (value >= CombatOptionType::TARGET && value < CombatOptionType::MAX);
}

inline bool IsValidCombatTextType(CombatTextType value) noexcept
{
	return (value >= CombatTextType::NORMAL && value < CombatTextType::MAX);
}


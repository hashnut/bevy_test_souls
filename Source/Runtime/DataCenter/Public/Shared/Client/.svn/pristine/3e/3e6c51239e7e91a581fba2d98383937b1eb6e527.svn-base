#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedPassivityEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 패시비티 타입
enum class PassivityType : uint8
{

	// 스탯 부여
	STAT = 0,

	// 발동형 - 이상상태
	INVOKE_ABNORMALITY = 1,

	// 발동형 - 스킬효과
	INVOKE_SKILL_NOTIFY = 2,

	// 셋팅형 - 이상상태
	SETUP_ABNORMALITY = 3,

	// 스킬 레벨 효과
	SKILL_LEVEL = 4,

	// 스킬 업그레이드 효과
	SKILL_UPGRADE = 5,

	// 발동형 - 쿨다운 효과
	INVOKE_COOLDOWN = 6,

	// 효과가 없는 패시비티 타입
	INCOMPETENCE = 7,

	MAX = 8,
};

UENUM(BlueprintType)
// 패시비티 부여 주체 타입
enum class PassivityOwnerType : uint8
{

	// 치트 명령어
	CHEAT = 0,

	// 아이템
	ITEM = 1,

	// 스킬
	SKILL = 2,

	// 페어리
	FAIRY = 3,

	// 맵(환경속성)
	MAP = 4,

	// 페어리 컬렉션
	FAIRY_COLLECTION = 5,

	// 아르카스 버프
	ARCAS = 6,

	// 기사단 건물
	GUILD_BUILDING = 7,

	// 코스튬 장착
	COSTUME_EQUIP = 8,

	// 코스튬 컬렉션
	COSTUME_COLLECTION = 9,

	// 아이템 컬렉션
	ITEM_COLLECTION = 10,

	// PVP성향(카오틱)
	PVP_ALIGNMENT = 11,

	// 다단계 랭크
	MULTI_LEVEL_RANK = 12,

	// 기사단 아이템 컬렉션
	GUILD_ITEM_COLLECTION = 13,

	// 랭킹 보상
	RANKING_REWARD = 14,

	// 기사단 랭킹 보상
	GUILD_RANKING_REWARD = 15,

	// 도감
	MONSTER_BOOK = 16,

	// 칭호 장착
	TITLE_EQUIP = 17,

	// 칭호 컬렉션
	TITLE_COLLECTION = 18,

	// 기사단 던전
	GUILD_DUNGEON = 19,

	// 기사단 캠프
	GUILD_CAMP = 20,

	// 이벤트 관리 시스템: MAP_PASSIVITY
	EVENT_MAP = 21,

	// 침공
	INVADE = 22,

	// 침공방어
	DEFENSE = 23,

	// 침공 수호자
	INVADE_GUARDIAN = 24,

	// 기기등록 보상
	DEVICE_REGISTER = 25,

	// 천마석 강화
	SPIRIT_SHOT_ENCHANT = 26,

	// 천마석 강화 총합(토탈) 레벨
	SPIRIT_SHOT_ENCHANT_SUM_LEVEL = 27,

	// 기사단 성소
	SANCTUM_NEXUS = 28,

	// 아이템 장비 마법부여
	ITEM_MAGICAL_FORGE = 29,

	// 도감 석판(특수 노드)
	SLATE = 30,

	// 도감 몬스터 일람
	CODEX = 31,

	MAX = 32,
};

UENUM(BlueprintType)
// 패시비티 활성 조건 검증 대상
enum class PassivityConditionType : uint8
{

	// 조건 없음
	NONE = 0,

	// 잔여 Hp 백분율
	HP_RATE = 1,

	// 잔여 Mp 백분율
	MP_RATE = 2,

	// 특정 AbnormalityId 가 걸려있는지 조사
	ABNORMALITY_ID = 3,

	// 특정 AbnormalityEffectType걸려있는지 조사
	ABNORMALITY_EFFECT_TYPE = 4,

	// 특정 AbnormalityEffectSubType걸려있는지 조사 ---- 삭제 필요
	ABNORMALITY_EFFECT_SUB_TYPE = 5,

	// 효과 그룹
	ABNORMALITY_EFFECT_GROUP_ID = 6,

	// SENSITIVE
	SENSITIVE = 7,

	// STR
	STR = 8,

	// DEX
	DEX = 9,

	// INT
	INT = 10,

	// AGI
	AGI = 11,

	// WIS
	WIS = 12,

	// CON
	CON = 13,

	// 안전 지대(마을 포함)에 체류하고 있을 경우
	ENTER_SAFE_ZONE = 14,

	MAX = 15,
};

UENUM(BlueprintType)
// 패시비티 활성 조건 검증 방안
enum class PassivityConditionMethod : uint8
{

	// None
	NONE = 0,

	// 이하
	BELOW = 1,

	// 이상
	ABOVE = 2,

	// 동일
	EQUAL = 3,

	// 범위
	RANGE = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 패시비티 발동 조건 타입
enum class PassivityCauseType : uint8
{

	// 없음
	NONE = 0,

	// 타격 시
	HIT_TARGET = 1,

	// 평타 타격 시
	HIT_TARGET_NORMAL = 2,

	// 스킬 타격 시
	HIT_TARGET_SKILL = 3,

	// 치명타 타격 시
	HIT_TARGET_CRITICAL = 4,

	// 나의 스킬 공격이 치명타로 적중 시
	HIT_TARGET_SKILL_CRITICAL = 5,

	// 피격 시
	HIT_SELF = 6,

	// 평타 피격 시
	HIT_SELF_NORMAL = 7,

	// 스킬 피격 시
	HIT_SELF_SKILL = 8,

	// 치명타 피격 시
	HIT_SELF_CRITICAL = 9,

	// 자신이 사망 시
	DIE_SELF = 10,

	// 나의 공격으로 대상 사망 시
	DIE_TARGET = 11,

	// 나의 특정 스킬 공격으로 인한 대상 사망 시
	DIE_TARGET_BY_MY_SKILL = 12,

	// 자신이 특정 스킬 Id 사용 시 (조건 검증 데이터 구조 필요)
	START_SKILL_SELF = 13,

	// 자신이 특정 아이템 착용 시 (조건 검증 데이터 구조 필요)
	EQUIP_ITEM_SELF = 14,

	// 나의 공격으로 대상이 이상상태 적용 시 (조건 검증 데이터 구조 필요)
	UPSERT_ABNORMALITY_TARGET = 15,

	MAX = 16,
};

UENUM(BlueprintType)
// 패시비티 발동 대상 타입
enum class PassivityInvokeTargetType : uint8
{

	// 나 자신
	SELF = 0,

	// 대상
	TARGET = 1,

	MAX = 2,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline PassivityType PassivityTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("STAT"), ESearchCase::IgnoreCase) == 0)	return PassivityType::STAT;
	if (str.Compare(TEXT("INVOKE_ABNORMALITY"), ESearchCase::IgnoreCase) == 0)	return PassivityType::INVOKE_ABNORMALITY;
	if (str.Compare(TEXT("INVOKE_SKILL_NOTIFY"), ESearchCase::IgnoreCase) == 0)	return PassivityType::INVOKE_SKILL_NOTIFY;
	if (str.Compare(TEXT("SETUP_ABNORMALITY"), ESearchCase::IgnoreCase) == 0)	return PassivityType::SETUP_ABNORMALITY;
	if (str.Compare(TEXT("SKILL_LEVEL"), ESearchCase::IgnoreCase) == 0)	return PassivityType::SKILL_LEVEL;
	if (str.Compare(TEXT("SKILL_UPGRADE"), ESearchCase::IgnoreCase) == 0)	return PassivityType::SKILL_UPGRADE;
	if (str.Compare(TEXT("INVOKE_COOLDOWN"), ESearchCase::IgnoreCase) == 0)	return PassivityType::INVOKE_COOLDOWN;
	if (str.Compare(TEXT("INCOMPETENCE"), ESearchCase::IgnoreCase) == 0)	return PassivityType::INCOMPETENCE;
	return PassivityType::MAX;
}
inline PassivityOwnerType PassivityOwnerTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CHEAT"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::CHEAT;
	if (str.Compare(TEXT("ITEM"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::ITEM;
	if (str.Compare(TEXT("SKILL"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::SKILL;
	if (str.Compare(TEXT("FAIRY"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::FAIRY;
	if (str.Compare(TEXT("MAP"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::MAP;
	if (str.Compare(TEXT("FAIRY_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::FAIRY_COLLECTION;
	if (str.Compare(TEXT("ARCAS"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::ARCAS;
	if (str.Compare(TEXT("GUILD_BUILDING"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::GUILD_BUILDING;
	if (str.Compare(TEXT("COSTUME_EQUIP"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::COSTUME_EQUIP;
	if (str.Compare(TEXT("COSTUME_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::COSTUME_COLLECTION;
	if (str.Compare(TEXT("ITEM_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::ITEM_COLLECTION;
	if (str.Compare(TEXT("PVP_ALIGNMENT"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::PVP_ALIGNMENT;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::MULTI_LEVEL_RANK;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::GUILD_ITEM_COLLECTION;
	if (str.Compare(TEXT("RANKING_REWARD"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::RANKING_REWARD;
	if (str.Compare(TEXT("GUILD_RANKING_REWARD"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::GUILD_RANKING_REWARD;
	if (str.Compare(TEXT("MONSTER_BOOK"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::MONSTER_BOOK;
	if (str.Compare(TEXT("TITLE_EQUIP"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::TITLE_EQUIP;
	if (str.Compare(TEXT("TITLE_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::TITLE_COLLECTION;
	if (str.Compare(TEXT("GUILD_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::GUILD_DUNGEON;
	if (str.Compare(TEXT("GUILD_CAMP"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::GUILD_CAMP;
	if (str.Compare(TEXT("EVENT_MAP"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::EVENT_MAP;
	if (str.Compare(TEXT("INVADE"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::INVADE;
	if (str.Compare(TEXT("DEFENSE"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::DEFENSE;
	if (str.Compare(TEXT("INVADE_GUARDIAN"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::INVADE_GUARDIAN;
	if (str.Compare(TEXT("DEVICE_REGISTER"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::DEVICE_REGISTER;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::SPIRIT_SHOT_ENCHANT;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT_SUM_LEVEL"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::SPIRIT_SHOT_ENCHANT_SUM_LEVEL;
	if (str.Compare(TEXT("SANCTUM_NEXUS"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::SANCTUM_NEXUS;
	if (str.Compare(TEXT("ITEM_MAGICAL_FORGE"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::ITEM_MAGICAL_FORGE;
	if (str.Compare(TEXT("SLATE"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::SLATE;
	if (str.Compare(TEXT("CODEX"), ESearchCase::IgnoreCase) == 0)	return PassivityOwnerType::CODEX;
	return PassivityOwnerType::MAX;
}
inline PassivityConditionType PassivityConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::NONE;
	if (str.Compare(TEXT("HP_RATE"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::HP_RATE;
	if (str.Compare(TEXT("MP_RATE"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::MP_RATE;
	if (str.Compare(TEXT("ABNORMALITY_ID"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::ABNORMALITY_ID;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_TYPE"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::ABNORMALITY_EFFECT_TYPE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::ABNORMALITY_EFFECT_SUB_TYPE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_GROUP_ID"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::ABNORMALITY_EFFECT_GROUP_ID;
	if (str.Compare(TEXT("SENSITIVE"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::SENSITIVE;
	if (str.Compare(TEXT("STR"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::STR;
	if (str.Compare(TEXT("DEX"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::DEX;
	if (str.Compare(TEXT("INT"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::INT;
	if (str.Compare(TEXT("AGI"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::AGI;
	if (str.Compare(TEXT("WIS"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::WIS;
	if (str.Compare(TEXT("CON"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::CON;
	if (str.Compare(TEXT("ENTER_SAFE_ZONE"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionType::ENTER_SAFE_ZONE;
	return PassivityConditionType::MAX;
}
inline PassivityConditionMethod PassivityConditionMethodStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionMethod::NONE;
	if (str.Compare(TEXT("BELOW"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionMethod::BELOW;
	if (str.Compare(TEXT("ABOVE"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionMethod::ABOVE;
	if (str.Compare(TEXT("EQUAL"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionMethod::EQUAL;
	if (str.Compare(TEXT("RANGE"), ESearchCase::IgnoreCase) == 0)	return PassivityConditionMethod::RANGE;
	return PassivityConditionMethod::MAX;
}
inline PassivityCauseType PassivityCauseTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::NONE;
	if (str.Compare(TEXT("HIT_TARGET"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::HIT_TARGET;
	if (str.Compare(TEXT("HIT_TARGET_NORMAL"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::HIT_TARGET_NORMAL;
	if (str.Compare(TEXT("HIT_TARGET_SKILL"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::HIT_TARGET_SKILL;
	if (str.Compare(TEXT("HIT_TARGET_CRITICAL"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::HIT_TARGET_CRITICAL;
	if (str.Compare(TEXT("HIT_TARGET_SKILL_CRITICAL"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::HIT_TARGET_SKILL_CRITICAL;
	if (str.Compare(TEXT("HIT_SELF"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::HIT_SELF;
	if (str.Compare(TEXT("HIT_SELF_NORMAL"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::HIT_SELF_NORMAL;
	if (str.Compare(TEXT("HIT_SELF_SKILL"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::HIT_SELF_SKILL;
	if (str.Compare(TEXT("HIT_SELF_CRITICAL"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::HIT_SELF_CRITICAL;
	if (str.Compare(TEXT("DIE_SELF"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::DIE_SELF;
	if (str.Compare(TEXT("DIE_TARGET"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::DIE_TARGET;
	if (str.Compare(TEXT("DIE_TARGET_BY_MY_SKILL"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::DIE_TARGET_BY_MY_SKILL;
	if (str.Compare(TEXT("START_SKILL_SELF"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::START_SKILL_SELF;
	if (str.Compare(TEXT("EQUIP_ITEM_SELF"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::EQUIP_ITEM_SELF;
	if (str.Compare(TEXT("UPSERT_ABNORMALITY_TARGET"), ESearchCase::IgnoreCase) == 0)	return PassivityCauseType::UPSERT_ABNORMALITY_TARGET;
	return PassivityCauseType::MAX;
}
inline PassivityInvokeTargetType PassivityInvokeTargetTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("SELF"), ESearchCase::IgnoreCase) == 0)	return PassivityInvokeTargetType::SELF;
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return PassivityInvokeTargetType::TARGET;
	return PassivityInvokeTargetType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString PassivityTypeEnumToString(PassivityType value)
{
	switch (value)
	{
		case PassivityType::STAT: return TEXT("STAT");
		case PassivityType::INVOKE_ABNORMALITY: return TEXT("INVOKE_ABNORMALITY");
		case PassivityType::INVOKE_SKILL_NOTIFY: return TEXT("INVOKE_SKILL_NOTIFY");
		case PassivityType::SETUP_ABNORMALITY: return TEXT("SETUP_ABNORMALITY");
		case PassivityType::SKILL_LEVEL: return TEXT("SKILL_LEVEL");
		case PassivityType::SKILL_UPGRADE: return TEXT("SKILL_UPGRADE");
		case PassivityType::INVOKE_COOLDOWN: return TEXT("INVOKE_COOLDOWN");
		case PassivityType::INCOMPETENCE: return TEXT("INCOMPETENCE");
	}
	return FString();
}
inline FString PassivityOwnerTypeEnumToString(PassivityOwnerType value)
{
	switch (value)
	{
		case PassivityOwnerType::CHEAT: return TEXT("CHEAT");
		case PassivityOwnerType::ITEM: return TEXT("ITEM");
		case PassivityOwnerType::SKILL: return TEXT("SKILL");
		case PassivityOwnerType::FAIRY: return TEXT("FAIRY");
		case PassivityOwnerType::MAP: return TEXT("MAP");
		case PassivityOwnerType::FAIRY_COLLECTION: return TEXT("FAIRY_COLLECTION");
		case PassivityOwnerType::ARCAS: return TEXT("ARCAS");
		case PassivityOwnerType::GUILD_BUILDING: return TEXT("GUILD_BUILDING");
		case PassivityOwnerType::COSTUME_EQUIP: return TEXT("COSTUME_EQUIP");
		case PassivityOwnerType::COSTUME_COLLECTION: return TEXT("COSTUME_COLLECTION");
		case PassivityOwnerType::ITEM_COLLECTION: return TEXT("ITEM_COLLECTION");
		case PassivityOwnerType::PVP_ALIGNMENT: return TEXT("PVP_ALIGNMENT");
		case PassivityOwnerType::MULTI_LEVEL_RANK: return TEXT("MULTI_LEVEL_RANK");
		case PassivityOwnerType::GUILD_ITEM_COLLECTION: return TEXT("GUILD_ITEM_COLLECTION");
		case PassivityOwnerType::RANKING_REWARD: return TEXT("RANKING_REWARD");
		case PassivityOwnerType::GUILD_RANKING_REWARD: return TEXT("GUILD_RANKING_REWARD");
		case PassivityOwnerType::MONSTER_BOOK: return TEXT("MONSTER_BOOK");
		case PassivityOwnerType::TITLE_EQUIP: return TEXT("TITLE_EQUIP");
		case PassivityOwnerType::TITLE_COLLECTION: return TEXT("TITLE_COLLECTION");
		case PassivityOwnerType::GUILD_DUNGEON: return TEXT("GUILD_DUNGEON");
		case PassivityOwnerType::GUILD_CAMP: return TEXT("GUILD_CAMP");
		case PassivityOwnerType::EVENT_MAP: return TEXT("EVENT_MAP");
		case PassivityOwnerType::INVADE: return TEXT("INVADE");
		case PassivityOwnerType::DEFENSE: return TEXT("DEFENSE");
		case PassivityOwnerType::INVADE_GUARDIAN: return TEXT("INVADE_GUARDIAN");
		case PassivityOwnerType::DEVICE_REGISTER: return TEXT("DEVICE_REGISTER");
		case PassivityOwnerType::SPIRIT_SHOT_ENCHANT: return TEXT("SPIRIT_SHOT_ENCHANT");
		case PassivityOwnerType::SPIRIT_SHOT_ENCHANT_SUM_LEVEL: return TEXT("SPIRIT_SHOT_ENCHANT_SUM_LEVEL");
		case PassivityOwnerType::SANCTUM_NEXUS: return TEXT("SANCTUM_NEXUS");
		case PassivityOwnerType::ITEM_MAGICAL_FORGE: return TEXT("ITEM_MAGICAL_FORGE");
		case PassivityOwnerType::SLATE: return TEXT("SLATE");
		case PassivityOwnerType::CODEX: return TEXT("CODEX");
	}
	return FString();
}
inline FString PassivityConditionTypeEnumToString(PassivityConditionType value)
{
	switch (value)
	{
		case PassivityConditionType::NONE: return TEXT("NONE");
		case PassivityConditionType::HP_RATE: return TEXT("HP_RATE");
		case PassivityConditionType::MP_RATE: return TEXT("MP_RATE");
		case PassivityConditionType::ABNORMALITY_ID: return TEXT("ABNORMALITY_ID");
		case PassivityConditionType::ABNORMALITY_EFFECT_TYPE: return TEXT("ABNORMALITY_EFFECT_TYPE");
		case PassivityConditionType::ABNORMALITY_EFFECT_SUB_TYPE: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE");
		case PassivityConditionType::ABNORMALITY_EFFECT_GROUP_ID: return TEXT("ABNORMALITY_EFFECT_GROUP_ID");
		case PassivityConditionType::SENSITIVE: return TEXT("SENSITIVE");
		case PassivityConditionType::STR: return TEXT("STR");
		case PassivityConditionType::DEX: return TEXT("DEX");
		case PassivityConditionType::INT: return TEXT("INT");
		case PassivityConditionType::AGI: return TEXT("AGI");
		case PassivityConditionType::WIS: return TEXT("WIS");
		case PassivityConditionType::CON: return TEXT("CON");
		case PassivityConditionType::ENTER_SAFE_ZONE: return TEXT("ENTER_SAFE_ZONE");
	}
	return FString();
}
inline FString PassivityConditionMethodEnumToString(PassivityConditionMethod value)
{
	switch (value)
	{
		case PassivityConditionMethod::NONE: return TEXT("NONE");
		case PassivityConditionMethod::BELOW: return TEXT("BELOW");
		case PassivityConditionMethod::ABOVE: return TEXT("ABOVE");
		case PassivityConditionMethod::EQUAL: return TEXT("EQUAL");
		case PassivityConditionMethod::RANGE: return TEXT("RANGE");
	}
	return FString();
}
inline FString PassivityCauseTypeEnumToString(PassivityCauseType value)
{
	switch (value)
	{
		case PassivityCauseType::NONE: return TEXT("NONE");
		case PassivityCauseType::HIT_TARGET: return TEXT("HIT_TARGET");
		case PassivityCauseType::HIT_TARGET_NORMAL: return TEXT("HIT_TARGET_NORMAL");
		case PassivityCauseType::HIT_TARGET_SKILL: return TEXT("HIT_TARGET_SKILL");
		case PassivityCauseType::HIT_TARGET_CRITICAL: return TEXT("HIT_TARGET_CRITICAL");
		case PassivityCauseType::HIT_TARGET_SKILL_CRITICAL: return TEXT("HIT_TARGET_SKILL_CRITICAL");
		case PassivityCauseType::HIT_SELF: return TEXT("HIT_SELF");
		case PassivityCauseType::HIT_SELF_NORMAL: return TEXT("HIT_SELF_NORMAL");
		case PassivityCauseType::HIT_SELF_SKILL: return TEXT("HIT_SELF_SKILL");
		case PassivityCauseType::HIT_SELF_CRITICAL: return TEXT("HIT_SELF_CRITICAL");
		case PassivityCauseType::DIE_SELF: return TEXT("DIE_SELF");
		case PassivityCauseType::DIE_TARGET: return TEXT("DIE_TARGET");
		case PassivityCauseType::DIE_TARGET_BY_MY_SKILL: return TEXT("DIE_TARGET_BY_MY_SKILL");
		case PassivityCauseType::START_SKILL_SELF: return TEXT("START_SKILL_SELF");
		case PassivityCauseType::EQUIP_ITEM_SELF: return TEXT("EQUIP_ITEM_SELF");
		case PassivityCauseType::UPSERT_ABNORMALITY_TARGET: return TEXT("UPSERT_ABNORMALITY_TARGET");
	}
	return FString();
}
inline FString PassivityInvokeTargetTypeEnumToString(PassivityInvokeTargetType value)
{
	switch (value)
	{
		case PassivityInvokeTargetType::SELF: return TEXT("SELF");
		case PassivityInvokeTargetType::TARGET: return TEXT("TARGET");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString PassivityTypeEnumToDisplayKey(PassivityType value)
{
	switch (value)
	{
		case PassivityType::STAT: return TEXT("");
		case PassivityType::INVOKE_ABNORMALITY: return TEXT("");
		case PassivityType::INVOKE_SKILL_NOTIFY: return TEXT("");
		case PassivityType::SETUP_ABNORMALITY: return TEXT("");
		case PassivityType::SKILL_LEVEL: return TEXT("");
		case PassivityType::SKILL_UPGRADE: return TEXT("");
		case PassivityType::INVOKE_COOLDOWN: return TEXT("");
		case PassivityType::INCOMPETENCE: return TEXT("");
	}
	return FString();
}
inline FString PassivityOwnerTypeEnumToDisplayKey(PassivityOwnerType value)
{
	switch (value)
	{
		case PassivityOwnerType::CHEAT: return TEXT("");
		case PassivityOwnerType::ITEM: return TEXT("");
		case PassivityOwnerType::SKILL: return TEXT("");
		case PassivityOwnerType::FAIRY: return TEXT("");
		case PassivityOwnerType::MAP: return TEXT("");
		case PassivityOwnerType::FAIRY_COLLECTION: return TEXT("");
		case PassivityOwnerType::ARCAS: return TEXT("");
		case PassivityOwnerType::GUILD_BUILDING: return TEXT("");
		case PassivityOwnerType::COSTUME_EQUIP: return TEXT("");
		case PassivityOwnerType::COSTUME_COLLECTION: return TEXT("");
		case PassivityOwnerType::ITEM_COLLECTION: return TEXT("");
		case PassivityOwnerType::PVP_ALIGNMENT: return TEXT("");
		case PassivityOwnerType::MULTI_LEVEL_RANK: return TEXT("");
		case PassivityOwnerType::GUILD_ITEM_COLLECTION: return TEXT("");
		case PassivityOwnerType::RANKING_REWARD: return TEXT("");
		case PassivityOwnerType::GUILD_RANKING_REWARD: return TEXT("");
		case PassivityOwnerType::MONSTER_BOOK: return TEXT("");
		case PassivityOwnerType::TITLE_EQUIP: return TEXT("");
		case PassivityOwnerType::TITLE_COLLECTION: return TEXT("");
		case PassivityOwnerType::GUILD_DUNGEON: return TEXT("");
		case PassivityOwnerType::GUILD_CAMP: return TEXT("");
		case PassivityOwnerType::EVENT_MAP: return TEXT("");
		case PassivityOwnerType::INVADE: return TEXT("");
		case PassivityOwnerType::DEFENSE: return TEXT("");
		case PassivityOwnerType::INVADE_GUARDIAN: return TEXT("");
		case PassivityOwnerType::DEVICE_REGISTER: return TEXT("");
		case PassivityOwnerType::SPIRIT_SHOT_ENCHANT: return TEXT("");
		case PassivityOwnerType::SPIRIT_SHOT_ENCHANT_SUM_LEVEL: return TEXT("");
		case PassivityOwnerType::SANCTUM_NEXUS: return TEXT("");
		case PassivityOwnerType::ITEM_MAGICAL_FORGE: return TEXT("");
		case PassivityOwnerType::SLATE: return TEXT("");
		case PassivityOwnerType::CODEX: return TEXT("");
	}
	return FString();
}
inline FString PassivityConditionTypeEnumToDisplayKey(PassivityConditionType value)
{
	switch (value)
	{
		case PassivityConditionType::NONE: return TEXT("");
		case PassivityConditionType::HP_RATE: return TEXT("");
		case PassivityConditionType::MP_RATE: return TEXT("");
		case PassivityConditionType::ABNORMALITY_ID: return TEXT("");
		case PassivityConditionType::ABNORMALITY_EFFECT_TYPE: return TEXT("");
		case PassivityConditionType::ABNORMALITY_EFFECT_SUB_TYPE: return TEXT("");
		case PassivityConditionType::ABNORMALITY_EFFECT_GROUP_ID: return TEXT("");
		case PassivityConditionType::SENSITIVE: return TEXT("");
		case PassivityConditionType::STR: return TEXT("");
		case PassivityConditionType::DEX: return TEXT("");
		case PassivityConditionType::INT: return TEXT("");
		case PassivityConditionType::AGI: return TEXT("");
		case PassivityConditionType::WIS: return TEXT("");
		case PassivityConditionType::CON: return TEXT("");
		case PassivityConditionType::ENTER_SAFE_ZONE: return TEXT("");
	}
	return FString();
}
inline FString PassivityConditionMethodEnumToDisplayKey(PassivityConditionMethod value)
{
	switch (value)
	{
		case PassivityConditionMethod::NONE: return TEXT("");
		case PassivityConditionMethod::BELOW: return TEXT("");
		case PassivityConditionMethod::ABOVE: return TEXT("");
		case PassivityConditionMethod::EQUAL: return TEXT("");
		case PassivityConditionMethod::RANGE: return TEXT("");
	}
	return FString();
}
inline FString PassivityCauseTypeEnumToDisplayKey(PassivityCauseType value)
{
	switch (value)
	{
		case PassivityCauseType::NONE: return TEXT("");
		case PassivityCauseType::HIT_TARGET: return TEXT("");
		case PassivityCauseType::HIT_TARGET_NORMAL: return TEXT("");
		case PassivityCauseType::HIT_TARGET_SKILL: return TEXT("");
		case PassivityCauseType::HIT_TARGET_CRITICAL: return TEXT("");
		case PassivityCauseType::HIT_TARGET_SKILL_CRITICAL: return TEXT("");
		case PassivityCauseType::HIT_SELF: return TEXT("");
		case PassivityCauseType::HIT_SELF_NORMAL: return TEXT("");
		case PassivityCauseType::HIT_SELF_SKILL: return TEXT("");
		case PassivityCauseType::HIT_SELF_CRITICAL: return TEXT("");
		case PassivityCauseType::DIE_SELF: return TEXT("");
		case PassivityCauseType::DIE_TARGET: return TEXT("");
		case PassivityCauseType::DIE_TARGET_BY_MY_SKILL: return TEXT("");
		case PassivityCauseType::START_SKILL_SELF: return TEXT("");
		case PassivityCauseType::EQUIP_ITEM_SELF: return TEXT("");
		case PassivityCauseType::UPSERT_ABNORMALITY_TARGET: return TEXT("");
	}
	return FString();
}
inline FString PassivityInvokeTargetTypeEnumToDisplayKey(PassivityInvokeTargetType value)
{
	switch (value)
	{
		case PassivityInvokeTargetType::SELF: return TEXT("");
		case PassivityInvokeTargetType::TARGET: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidPassivityType(PassivityType value) noexcept
{
	return (value >= PassivityType::STAT && value < PassivityType::MAX);
}

inline bool IsValidPassivityOwnerType(PassivityOwnerType value) noexcept
{
	return (value >= PassivityOwnerType::CHEAT && value < PassivityOwnerType::MAX);
}

inline bool IsValidPassivityConditionType(PassivityConditionType value) noexcept
{
	return (value >= PassivityConditionType::NONE && value < PassivityConditionType::MAX);
}

inline bool IsValidPassivityConditionMethod(PassivityConditionMethod value) noexcept
{
	return (value >= PassivityConditionMethod::NONE && value < PassivityConditionMethod::MAX);
}

inline bool IsValidPassivityCauseType(PassivityCauseType value) noexcept
{
	return (value >= PassivityCauseType::NONE && value < PassivityCauseType::MAX);
}

inline bool IsValidPassivityInvokeTargetType(PassivityInvokeTargetType value) noexcept
{
	return (value >= PassivityInvokeTargetType::SELF && value < PassivityInvokeTargetType::MAX);
}


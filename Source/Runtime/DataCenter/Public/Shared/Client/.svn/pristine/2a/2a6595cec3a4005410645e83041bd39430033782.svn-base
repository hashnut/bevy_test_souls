#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedUnlockEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// UnlockContents.json의 id와 매칭
enum class UnlockCondition : uint8
{

	// 없음
	NONE = 0,

	// 퀘스트 진행에 따라 잠금 해제
	QUEST = 1,

	// 레벨로 인한 잠금 해제
	LEVEL = 2,

	// 업데이트로 인한 잠금 해제(운영툴에서도 설정 가능)
	UPDATE = 3,

	// 버그로 인한 잠금 해제(운영툴에서도 설정 가능)
	BUG_ERROR = 4,

	// 맵 이동 금지
	MAP_MOVE_PROHIBITED = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// Category별 언락 컨텐츠
enum class UnlockCategory : uint8
{

	// 없음
	NONE = 0,

	// 침공 상태
	INVADE = 1,

	// 기사단 던전 관람
	GUILD_DUNGEON_WATCHING = 2,

	// 대결 상태
	DUEL = 3,

	// 일반 던전
	NORMAL_DUNGEON = 4,

	// 특수 던전
	SPECIAL_DUNGEON = 5,

	// 파티 던전
	PARTY_DUNGEON = 6,

	// 길드 던전
	GUILD_DUNGEON = 7,

	// 엘리트 던전
	ELITE_DUNGEON = 8,

	// 기기 등록
	REGISTER_DEVICE = 9,

	// 인터 필드
	INTER_FIELD = 10,

	// 길드 아레나
	GUILD_WAR = 11,

	// 기사단 영지
	AGIT = 12,

	// 기사단 영지 던전
	AGIT_DUNGEON_RAID = 13,

	MAX = 14,
};

UENUM(BlueprintType)
// 잠금 해제
enum class UnlockType : uint8
{

	// 컨텐츠별 조건
	CONTENTS_CONDITION = 0,

	// 카테고리별 조건
	CATEGORY = 1,

	MAX = 2,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline UnlockCondition UnlockConditionStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return UnlockCondition::NONE;
	if (str.Compare(TEXT("QUEST"), ESearchCase::IgnoreCase) == 0)	return UnlockCondition::QUEST;
	if (str.Compare(TEXT("LEVEL"), ESearchCase::IgnoreCase) == 0)	return UnlockCondition::LEVEL;
	if (str.Compare(TEXT("UPDATE"), ESearchCase::IgnoreCase) == 0)	return UnlockCondition::UPDATE;
	if (str.Compare(TEXT("BUG_ERROR"), ESearchCase::IgnoreCase) == 0)	return UnlockCondition::BUG_ERROR;
	if (str.Compare(TEXT("MAP_MOVE_PROHIBITED"), ESearchCase::IgnoreCase) == 0)	return UnlockCondition::MAP_MOVE_PROHIBITED;
	return UnlockCondition::MAX;
}
inline UnlockCategory UnlockCategoryStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::NONE;
	if (str.Compare(TEXT("INVADE"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::INVADE;
	if (str.Compare(TEXT("GUILD_DUNGEON_WATCHING"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::GUILD_DUNGEON_WATCHING;
	if (str.Compare(TEXT("DUEL"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::DUEL;
	if (str.Compare(TEXT("NORMAL_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::NORMAL_DUNGEON;
	if (str.Compare(TEXT("SPECIAL_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::SPECIAL_DUNGEON;
	if (str.Compare(TEXT("PARTY_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::PARTY_DUNGEON;
	if (str.Compare(TEXT("GUILD_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::GUILD_DUNGEON;
	if (str.Compare(TEXT("ELITE_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::ELITE_DUNGEON;
	if (str.Compare(TEXT("REGISTER_DEVICE"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::REGISTER_DEVICE;
	if (str.Compare(TEXT("INTER_FIELD"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::INTER_FIELD;
	if (str.Compare(TEXT("GUILD_WAR"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::GUILD_WAR;
	if (str.Compare(TEXT("AGIT"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::AGIT;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID"), ESearchCase::IgnoreCase) == 0)	return UnlockCategory::AGIT_DUNGEON_RAID;
	return UnlockCategory::MAX;
}
inline UnlockType UnlockTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CONTENTS_CONDITION"), ESearchCase::IgnoreCase) == 0)	return UnlockType::CONTENTS_CONDITION;
	if (str.Compare(TEXT("CATEGORY"), ESearchCase::IgnoreCase) == 0)	return UnlockType::CATEGORY;
	return UnlockType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString UnlockConditionEnumToString(UnlockCondition value)
{
	switch (value)
	{
		case UnlockCondition::NONE: return TEXT("NONE");
		case UnlockCondition::QUEST: return TEXT("QUEST");
		case UnlockCondition::LEVEL: return TEXT("LEVEL");
		case UnlockCondition::UPDATE: return TEXT("UPDATE");
		case UnlockCondition::BUG_ERROR: return TEXT("BUG_ERROR");
		case UnlockCondition::MAP_MOVE_PROHIBITED: return TEXT("MAP_MOVE_PROHIBITED");
	}
	return FString();
}
inline FString UnlockCategoryEnumToString(UnlockCategory value)
{
	switch (value)
	{
		case UnlockCategory::NONE: return TEXT("NONE");
		case UnlockCategory::INVADE: return TEXT("INVADE");
		case UnlockCategory::GUILD_DUNGEON_WATCHING: return TEXT("GUILD_DUNGEON_WATCHING");
		case UnlockCategory::DUEL: return TEXT("DUEL");
		case UnlockCategory::NORMAL_DUNGEON: return TEXT("NORMAL_DUNGEON");
		case UnlockCategory::SPECIAL_DUNGEON: return TEXT("SPECIAL_DUNGEON");
		case UnlockCategory::PARTY_DUNGEON: return TEXT("PARTY_DUNGEON");
		case UnlockCategory::GUILD_DUNGEON: return TEXT("GUILD_DUNGEON");
		case UnlockCategory::ELITE_DUNGEON: return TEXT("ELITE_DUNGEON");
		case UnlockCategory::REGISTER_DEVICE: return TEXT("REGISTER_DEVICE");
		case UnlockCategory::INTER_FIELD: return TEXT("INTER_FIELD");
		case UnlockCategory::GUILD_WAR: return TEXT("GUILD_WAR");
		case UnlockCategory::AGIT: return TEXT("AGIT");
		case UnlockCategory::AGIT_DUNGEON_RAID: return TEXT("AGIT_DUNGEON_RAID");
	}
	return FString();
}
inline FString UnlockTypeEnumToString(UnlockType value)
{
	switch (value)
	{
		case UnlockType::CONTENTS_CONDITION: return TEXT("CONTENTS_CONDITION");
		case UnlockType::CATEGORY: return TEXT("CATEGORY");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString UnlockConditionEnumToDisplayKey(UnlockCondition value)
{
	switch (value)
	{
		case UnlockCondition::NONE: return TEXT("");
		case UnlockCondition::QUEST: return TEXT("");
		case UnlockCondition::LEVEL: return TEXT("");
		case UnlockCondition::UPDATE: return TEXT("");
		case UnlockCondition::BUG_ERROR: return TEXT("");
		case UnlockCondition::MAP_MOVE_PROHIBITED: return TEXT("");
	}
	return FString();
}
inline FString UnlockCategoryEnumToDisplayKey(UnlockCategory value)
{
	switch (value)
	{
		case UnlockCategory::NONE: return TEXT("");
		case UnlockCategory::INVADE: return TEXT("");
		case UnlockCategory::GUILD_DUNGEON_WATCHING: return TEXT("");
		case UnlockCategory::DUEL: return TEXT("");
		case UnlockCategory::NORMAL_DUNGEON: return TEXT("");
		case UnlockCategory::SPECIAL_DUNGEON: return TEXT("");
		case UnlockCategory::PARTY_DUNGEON: return TEXT("");
		case UnlockCategory::GUILD_DUNGEON: return TEXT("");
		case UnlockCategory::ELITE_DUNGEON: return TEXT("");
		case UnlockCategory::REGISTER_DEVICE: return TEXT("");
		case UnlockCategory::INTER_FIELD: return TEXT("");
		case UnlockCategory::GUILD_WAR: return TEXT("");
		case UnlockCategory::AGIT: return TEXT("");
		case UnlockCategory::AGIT_DUNGEON_RAID: return TEXT("");
	}
	return FString();
}
inline FString UnlockTypeEnumToDisplayKey(UnlockType value)
{
	switch (value)
	{
		case UnlockType::CONTENTS_CONDITION: return TEXT("");
		case UnlockType::CATEGORY: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidUnlockCondition(UnlockCondition value) noexcept
{
	return (value >= UnlockCondition::NONE && value < UnlockCondition::MAX);
}

inline bool IsValidUnlockCategory(UnlockCategory value) noexcept
{
	return (value >= UnlockCategory::NONE && value < UnlockCategory::MAX);
}

inline bool IsValidUnlockType(UnlockType value) noexcept
{
	return (value >= UnlockType::CONTENTS_CONDITION && value < UnlockType::MAX);
}


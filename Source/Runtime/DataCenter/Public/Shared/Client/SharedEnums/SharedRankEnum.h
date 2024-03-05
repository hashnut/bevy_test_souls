#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedRankEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 랭킹 타입
enum class RankType : uint8
{

	// 없음 - 에러값
	NONE = 0,

	// 단일 서버 레벨 순위
	SERVER_LEVEL_RANK = 1,

	// 전체 서버 레벨 순위
	ALL_SERVER_LEVEL_RANK = 2,

	// 스킬 강화 누적 횟수 순위
	SKILL_ENCHANT_COUNT_RANK = 3,

	// 장비 강화 시도 누적 횟수
	EQUIPMENTS_ENCHANT_RANK = 4,

	// 장비 강화 실패 횟수
	EQUIPMENTS_ENCHANT_FAIL_RANK = 5,

	// 제작 누적 횟수
	CRAFTING_COUNT_RANK = 6,

	// 거래소 거래 횟수 (판매+구매)
	MARKET_USE_COUNT_RANK = 7,

	// 거래소 최고가 구매 이력
	MARKET_HIGHEST_BUY_RANK = 8,

	// 기사단 기부금 누적 총합
	KNIGHTS_TOTAL_CONTRIBUTION_RANK = 9,

	// PK 누적 횟수 (막타만 인정)
	TOTAL_PK_COUNT_RANK = 10,

	// PK 누적 사망 횟수
	TOTAL_PK_DIE_COUNT_RANK = 11,

	// 시공의 틈새 클리어 누적 횟수
	SPACE_CRACK_CLEAR_COUNT_RANK = 12,

	// 필드 보스 처치 누적 횟수(참여)
	FIELD_BOSS_CLEAR_COUNT_RANK = 13,

	// 반복 퀘스트 클리어 누적 횟수
	REPEAT_QUEST_CLEAR_RANK = 14,

	// 장비 컬렉션 달성 개수
	EQUIPMEMTS_COLLECTION_RANK = 15,

	// 페어리 컬렉션 달성 개수
	FAIRIES_COLLECTION_RANK = 16,

	// 코스튬 컬렉션 달성 개수
	COSTUMES_COLLECTION_RANK = 17,

	// 누적 접속일
	ACCUMULATE_ACCESS_DAY_RANK = 18,

	// 업적 달성 누적 개수
	ACHIEVEMENTS_RANK = 19,

	// 기사단 던전 클리어 레벨
	KNIGHTS_DUNGEON_CLEAR_RANK = 20,

	// 다이아 누적 소모량
	DIAMOND_CONSUMPTION_RANK = 21,

	// 그림자 전장 랭킹
	GUILD_ARENA_RANK = 22,

	MAX = 23,
};

UENUM(BlueprintType)
// 랭킹 검색 타입
enum class RankSearchType : uint8
{

	// 개인
	PERSONAL = 0,

	// 기사단
	KNIGHTS = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 서버 검색 범위
enum class RankServerRangeType : uint8
{

	// 현재 속한 서버
	LOCAL = 0,

	// 전체 서버
	WORLD = 1,

	MAX = 2,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline RankType RankTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return RankType::NONE;
	if (str.Compare(TEXT("SERVER_LEVEL_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::SERVER_LEVEL_RANK;
	if (str.Compare(TEXT("ALL_SERVER_LEVEL_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::ALL_SERVER_LEVEL_RANK;
	if (str.Compare(TEXT("SKILL_ENCHANT_COUNT_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::SKILL_ENCHANT_COUNT_RANK;
	if (str.Compare(TEXT("EQUIPMENTS_ENCHANT_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::EQUIPMENTS_ENCHANT_RANK;
	if (str.Compare(TEXT("EQUIPMENTS_ENCHANT_FAIL_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::EQUIPMENTS_ENCHANT_FAIL_RANK;
	if (str.Compare(TEXT("CRAFTING_COUNT_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::CRAFTING_COUNT_RANK;
	if (str.Compare(TEXT("MARKET_USE_COUNT_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::MARKET_USE_COUNT_RANK;
	if (str.Compare(TEXT("MARKET_HIGHEST_BUY_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::MARKET_HIGHEST_BUY_RANK;
	if (str.Compare(TEXT("KNIGHTS_TOTAL_CONTRIBUTION_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::KNIGHTS_TOTAL_CONTRIBUTION_RANK;
	if (str.Compare(TEXT("TOTAL_PK_COUNT_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::TOTAL_PK_COUNT_RANK;
	if (str.Compare(TEXT("TOTAL_PK_DIE_COUNT_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::TOTAL_PK_DIE_COUNT_RANK;
	if (str.Compare(TEXT("SPACE_CRACK_CLEAR_COUNT_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::SPACE_CRACK_CLEAR_COUNT_RANK;
	if (str.Compare(TEXT("FIELD_BOSS_CLEAR_COUNT_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::FIELD_BOSS_CLEAR_COUNT_RANK;
	if (str.Compare(TEXT("REPEAT_QUEST_CLEAR_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::REPEAT_QUEST_CLEAR_RANK;
	if (str.Compare(TEXT("EQUIPMEMTS_COLLECTION_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::EQUIPMEMTS_COLLECTION_RANK;
	if (str.Compare(TEXT("FAIRIES_COLLECTION_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::FAIRIES_COLLECTION_RANK;
	if (str.Compare(TEXT("COSTUMES_COLLECTION_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::COSTUMES_COLLECTION_RANK;
	if (str.Compare(TEXT("ACCUMULATE_ACCESS_DAY_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::ACCUMULATE_ACCESS_DAY_RANK;
	if (str.Compare(TEXT("ACHIEVEMENTS_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::ACHIEVEMENTS_RANK;
	if (str.Compare(TEXT("KNIGHTS_DUNGEON_CLEAR_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::KNIGHTS_DUNGEON_CLEAR_RANK;
	if (str.Compare(TEXT("DIAMOND_CONSUMPTION_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::DIAMOND_CONSUMPTION_RANK;
	if (str.Compare(TEXT("GUILD_ARENA_RANK"), ESearchCase::IgnoreCase) == 0)	return RankType::GUILD_ARENA_RANK;
	return RankType::MAX;
}
inline RankSearchType RankSearchTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("PERSONAL"), ESearchCase::IgnoreCase) == 0)	return RankSearchType::PERSONAL;
	if (str.Compare(TEXT("KNIGHTS"), ESearchCase::IgnoreCase) == 0)	return RankSearchType::KNIGHTS;
	return RankSearchType::MAX;
}
inline RankServerRangeType RankServerRangeTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("LOCAL"), ESearchCase::IgnoreCase) == 0)	return RankServerRangeType::LOCAL;
	if (str.Compare(TEXT("WORLD"), ESearchCase::IgnoreCase) == 0)	return RankServerRangeType::WORLD;
	return RankServerRangeType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString RankTypeEnumToString(RankType value)
{
	switch (value)
	{
		case RankType::NONE: return TEXT("NONE");
		case RankType::SERVER_LEVEL_RANK: return TEXT("SERVER_LEVEL_RANK");
		case RankType::ALL_SERVER_LEVEL_RANK: return TEXT("ALL_SERVER_LEVEL_RANK");
		case RankType::SKILL_ENCHANT_COUNT_RANK: return TEXT("SKILL_ENCHANT_COUNT_RANK");
		case RankType::EQUIPMENTS_ENCHANT_RANK: return TEXT("EQUIPMENTS_ENCHANT_RANK");
		case RankType::EQUIPMENTS_ENCHANT_FAIL_RANK: return TEXT("EQUIPMENTS_ENCHANT_FAIL_RANK");
		case RankType::CRAFTING_COUNT_RANK: return TEXT("CRAFTING_COUNT_RANK");
		case RankType::MARKET_USE_COUNT_RANK: return TEXT("MARKET_USE_COUNT_RANK");
		case RankType::MARKET_HIGHEST_BUY_RANK: return TEXT("MARKET_HIGHEST_BUY_RANK");
		case RankType::KNIGHTS_TOTAL_CONTRIBUTION_RANK: return TEXT("KNIGHTS_TOTAL_CONTRIBUTION_RANK");
		case RankType::TOTAL_PK_COUNT_RANK: return TEXT("TOTAL_PK_COUNT_RANK");
		case RankType::TOTAL_PK_DIE_COUNT_RANK: return TEXT("TOTAL_PK_DIE_COUNT_RANK");
		case RankType::SPACE_CRACK_CLEAR_COUNT_RANK: return TEXT("SPACE_CRACK_CLEAR_COUNT_RANK");
		case RankType::FIELD_BOSS_CLEAR_COUNT_RANK: return TEXT("FIELD_BOSS_CLEAR_COUNT_RANK");
		case RankType::REPEAT_QUEST_CLEAR_RANK: return TEXT("REPEAT_QUEST_CLEAR_RANK");
		case RankType::EQUIPMEMTS_COLLECTION_RANK: return TEXT("EQUIPMEMTS_COLLECTION_RANK");
		case RankType::FAIRIES_COLLECTION_RANK: return TEXT("FAIRIES_COLLECTION_RANK");
		case RankType::COSTUMES_COLLECTION_RANK: return TEXT("COSTUMES_COLLECTION_RANK");
		case RankType::ACCUMULATE_ACCESS_DAY_RANK: return TEXT("ACCUMULATE_ACCESS_DAY_RANK");
		case RankType::ACHIEVEMENTS_RANK: return TEXT("ACHIEVEMENTS_RANK");
		case RankType::KNIGHTS_DUNGEON_CLEAR_RANK: return TEXT("KNIGHTS_DUNGEON_CLEAR_RANK");
		case RankType::DIAMOND_CONSUMPTION_RANK: return TEXT("DIAMOND_CONSUMPTION_RANK");
		case RankType::GUILD_ARENA_RANK: return TEXT("GUILD_ARENA_RANK");
	}
	return FString();
}
inline FString RankSearchTypeEnumToString(RankSearchType value)
{
	switch (value)
	{
		case RankSearchType::PERSONAL: return TEXT("PERSONAL");
		case RankSearchType::KNIGHTS: return TEXT("KNIGHTS");
	}
	return FString();
}
inline FString RankServerRangeTypeEnumToString(RankServerRangeType value)
{
	switch (value)
	{
		case RankServerRangeType::LOCAL: return TEXT("LOCAL");
		case RankServerRangeType::WORLD: return TEXT("WORLD");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString RankTypeEnumToDisplayKey(RankType value)
{
	switch (value)
	{
		case RankType::NONE: return TEXT("");
		case RankType::SERVER_LEVEL_RANK: return TEXT("");
		case RankType::ALL_SERVER_LEVEL_RANK: return TEXT("");
		case RankType::SKILL_ENCHANT_COUNT_RANK: return TEXT("");
		case RankType::EQUIPMENTS_ENCHANT_RANK: return TEXT("");
		case RankType::EQUIPMENTS_ENCHANT_FAIL_RANK: return TEXT("");
		case RankType::CRAFTING_COUNT_RANK: return TEXT("");
		case RankType::MARKET_USE_COUNT_RANK: return TEXT("");
		case RankType::MARKET_HIGHEST_BUY_RANK: return TEXT("");
		case RankType::KNIGHTS_TOTAL_CONTRIBUTION_RANK: return TEXT("");
		case RankType::TOTAL_PK_COUNT_RANK: return TEXT("");
		case RankType::TOTAL_PK_DIE_COUNT_RANK: return TEXT("");
		case RankType::SPACE_CRACK_CLEAR_COUNT_RANK: return TEXT("");
		case RankType::FIELD_BOSS_CLEAR_COUNT_RANK: return TEXT("");
		case RankType::REPEAT_QUEST_CLEAR_RANK: return TEXT("");
		case RankType::EQUIPMEMTS_COLLECTION_RANK: return TEXT("");
		case RankType::FAIRIES_COLLECTION_RANK: return TEXT("");
		case RankType::COSTUMES_COLLECTION_RANK: return TEXT("");
		case RankType::ACCUMULATE_ACCESS_DAY_RANK: return TEXT("");
		case RankType::ACHIEVEMENTS_RANK: return TEXT("");
		case RankType::KNIGHTS_DUNGEON_CLEAR_RANK: return TEXT("");
		case RankType::DIAMOND_CONSUMPTION_RANK: return TEXT("");
		case RankType::GUILD_ARENA_RANK: return TEXT("");
	}
	return FString();
}
inline FString RankSearchTypeEnumToDisplayKey(RankSearchType value)
{
	switch (value)
	{
		case RankSearchType::PERSONAL: return TEXT("");
		case RankSearchType::KNIGHTS: return TEXT("");
	}
	return FString();
}
inline FString RankServerRangeTypeEnumToDisplayKey(RankServerRangeType value)
{
	switch (value)
	{
		case RankServerRangeType::LOCAL: return TEXT("");
		case RankServerRangeType::WORLD: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidRankType(RankType value) noexcept
{
	return (value >= RankType::NONE && value < RankType::MAX);
}

inline bool IsValidRankSearchType(RankSearchType value) noexcept
{
	return (value >= RankSearchType::PERSONAL && value < RankSearchType::MAX);
}

inline bool IsValidRankServerRangeType(RankServerRangeType value) noexcept
{
	return (value >= RankServerRangeType::LOCAL && value < RankServerRangeType::MAX);
}


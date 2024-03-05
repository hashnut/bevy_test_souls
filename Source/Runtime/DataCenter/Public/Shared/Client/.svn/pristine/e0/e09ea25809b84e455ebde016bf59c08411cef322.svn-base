#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedStatisticsEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 통계 타입
enum class StatisticsType : uint8
{

	// 분류 없음
	NONE = 0,

	// 가한 데미지 - 총 데미지
	DAMAGE_INFLICT_TOTAL = 1,

	// 가한 데미지 - 최대 데미지
	DAMAGE_INFLICT_MAX = 2,

	// 가한 데미지 - 치명타 비율
	CRITICAL_HIT_INFLICT_RATE = 3,

	// 받은 데미지 - 총 데미지
	DAMAGE_RECEIVE_TOTAL = 4,

	// 받은 데미지 - 최대 데미지
	DAMAGE_RECEIVE_MAX = 5,

	// 받은 데미지 - 치명타 비율
	CRITICAL_HIT_RECEIVE_RATE = 6,

	// 처치한 몬스터
	KILL_MONSTER = 7,

	// 처치한 유저
	KILL_PLAYER = 8,

	// 사망 횟수
	DEATH_COUNT = 9,

	// HP 회복 - 자신
	HP_RECOVERY_MYSELF = 10,

	// HP 회복 - 파티원
	HP_RECOVERY_PARTY = 11,

	// 획득한 경험치
	EXP_GAIN = 12,

	// 페어리 소환 횟수
	FAIRY_SUMMON = 13,

	// 페어리 강화 횟수
	FAIRY_ENCHANT = 14,

	// 장비 강화 횟수
	EQUIPMENT_ENCHANT = 15,

	// 코스튬 소환 횟수
	COSTUME_SUMMON = 16,

	// 아이템 제작 갯수
	ITEM_CRAFT = 17,

	// 장비 획득 - 노말
	ITEM_GET_NORMAL_EQUIPMENT = 18,

	// 장비 획득 - 매직
	ITEM_GET_MAGIC_EQUIPMENT = 19,

	// 장비 획득 - 레어
	ITEM_GET_RARE_EQUIPMENT = 20,

	// 장비 획득 - 에픽
	ITEM_GET_EPIC_EQUIPMENT = 21,

	// 장비 획득 - 유니크
	ITEM_GET_UNIQUE_EQUIPMENT = 22,

	// 회복 물약 사용 갯수
	ITEM_USE_HP_POTION = 23,

	// 음식 사용 갯수
	ITEM_USE_FOOD_BUFF = 24,

	// 버프 주문서 사용 갯수
	ITEM_USE_SCROLL_BUFF = 25,

	// 재화 획득 - 골드(누적)
	CURRENCY_GOLD_ACCUMULATE_GET = 26,

	// 재화 사용 - 골드(누적)
	CURRENCY_GOLD_ACCUMULATE_USE = 27,

	// 접속 시간(누적)
	LOGIN_TIME_ACCUMULATE = 28,

	// 가한 데미지 - 총 데미지(일반)
	DAMAGE_INFLICT_TOTAL_NORMAL = 29,

	// 가한 데미지 - 총 데미지(스킬)
	DAMAGE_INFLICT_TOTAL_SKILL = 30,

	// 접속 일자(누적)
	LOGIN_DATE_ACCUMULATE = 31,

	MAX = 32,
};

UENUM(BlueprintType)
// 통계 계산 타입
enum class StatisticsCalculateType : uint8
{

	// 없음
	NONE = 0,

	// 누적 계산
	CALCULATE_ACCUMULATE = 1,

	// 최대치 계산
	CALCULATE_MAX = 2,

	// 비율 계산
	CALCULATE_RATIO = 3,

	MAX = 4,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline StatisticsType StatisticsTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::NONE;
	if (str.Compare(TEXT("DAMAGE_INFLICT_TOTAL"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::DAMAGE_INFLICT_TOTAL;
	if (str.Compare(TEXT("DAMAGE_INFLICT_MAX"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::DAMAGE_INFLICT_MAX;
	if (str.Compare(TEXT("CRITICAL_HIT_INFLICT_RATE"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::CRITICAL_HIT_INFLICT_RATE;
	if (str.Compare(TEXT("DAMAGE_RECEIVE_TOTAL"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::DAMAGE_RECEIVE_TOTAL;
	if (str.Compare(TEXT("DAMAGE_RECEIVE_MAX"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::DAMAGE_RECEIVE_MAX;
	if (str.Compare(TEXT("CRITICAL_HIT_RECEIVE_RATE"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::CRITICAL_HIT_RECEIVE_RATE;
	if (str.Compare(TEXT("KILL_MONSTER"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::KILL_MONSTER;
	if (str.Compare(TEXT("KILL_PLAYER"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::KILL_PLAYER;
	if (str.Compare(TEXT("DEATH_COUNT"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::DEATH_COUNT;
	if (str.Compare(TEXT("HP_RECOVERY_MYSELF"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::HP_RECOVERY_MYSELF;
	if (str.Compare(TEXT("HP_RECOVERY_PARTY"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::HP_RECOVERY_PARTY;
	if (str.Compare(TEXT("EXP_GAIN"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::EXP_GAIN;
	if (str.Compare(TEXT("FAIRY_SUMMON"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::FAIRY_SUMMON;
	if (str.Compare(TEXT("FAIRY_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::FAIRY_ENCHANT;
	if (str.Compare(TEXT("EQUIPMENT_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::EQUIPMENT_ENCHANT;
	if (str.Compare(TEXT("COSTUME_SUMMON"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::COSTUME_SUMMON;
	if (str.Compare(TEXT("ITEM_CRAFT"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::ITEM_CRAFT;
	if (str.Compare(TEXT("ITEM_GET_NORMAL_EQUIPMENT"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::ITEM_GET_NORMAL_EQUIPMENT;
	if (str.Compare(TEXT("ITEM_GET_MAGIC_EQUIPMENT"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::ITEM_GET_MAGIC_EQUIPMENT;
	if (str.Compare(TEXT("ITEM_GET_RARE_EQUIPMENT"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::ITEM_GET_RARE_EQUIPMENT;
	if (str.Compare(TEXT("ITEM_GET_EPIC_EQUIPMENT"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::ITEM_GET_EPIC_EQUIPMENT;
	if (str.Compare(TEXT("ITEM_GET_UNIQUE_EQUIPMENT"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::ITEM_GET_UNIQUE_EQUIPMENT;
	if (str.Compare(TEXT("ITEM_USE_HP_POTION"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::ITEM_USE_HP_POTION;
	if (str.Compare(TEXT("ITEM_USE_FOOD_BUFF"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::ITEM_USE_FOOD_BUFF;
	if (str.Compare(TEXT("ITEM_USE_SCROLL_BUFF"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::ITEM_USE_SCROLL_BUFF;
	if (str.Compare(TEXT("CURRENCY_GOLD_ACCUMULATE_GET"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::CURRENCY_GOLD_ACCUMULATE_GET;
	if (str.Compare(TEXT("CURRENCY_GOLD_ACCUMULATE_USE"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::CURRENCY_GOLD_ACCUMULATE_USE;
	if (str.Compare(TEXT("LOGIN_TIME_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::LOGIN_TIME_ACCUMULATE;
	if (str.Compare(TEXT("DAMAGE_INFLICT_TOTAL_NORMAL"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::DAMAGE_INFLICT_TOTAL_NORMAL;
	if (str.Compare(TEXT("DAMAGE_INFLICT_TOTAL_SKILL"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::DAMAGE_INFLICT_TOTAL_SKILL;
	if (str.Compare(TEXT("LOGIN_DATE_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return StatisticsType::LOGIN_DATE_ACCUMULATE;
	return StatisticsType::MAX;
}
inline StatisticsCalculateType StatisticsCalculateTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return StatisticsCalculateType::NONE;
	if (str.Compare(TEXT("CALCULATE_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return StatisticsCalculateType::CALCULATE_ACCUMULATE;
	if (str.Compare(TEXT("CALCULATE_MAX"), ESearchCase::IgnoreCase) == 0)	return StatisticsCalculateType::CALCULATE_MAX;
	if (str.Compare(TEXT("CALCULATE_RATIO"), ESearchCase::IgnoreCase) == 0)	return StatisticsCalculateType::CALCULATE_RATIO;
	return StatisticsCalculateType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString StatisticsTypeEnumToString(StatisticsType value)
{
	switch (value)
	{
		case StatisticsType::NONE: return TEXT("NONE");
		case StatisticsType::DAMAGE_INFLICT_TOTAL: return TEXT("DAMAGE_INFLICT_TOTAL");
		case StatisticsType::DAMAGE_INFLICT_MAX: return TEXT("DAMAGE_INFLICT_MAX");
		case StatisticsType::CRITICAL_HIT_INFLICT_RATE: return TEXT("CRITICAL_HIT_INFLICT_RATE");
		case StatisticsType::DAMAGE_RECEIVE_TOTAL: return TEXT("DAMAGE_RECEIVE_TOTAL");
		case StatisticsType::DAMAGE_RECEIVE_MAX: return TEXT("DAMAGE_RECEIVE_MAX");
		case StatisticsType::CRITICAL_HIT_RECEIVE_RATE: return TEXT("CRITICAL_HIT_RECEIVE_RATE");
		case StatisticsType::KILL_MONSTER: return TEXT("KILL_MONSTER");
		case StatisticsType::KILL_PLAYER: return TEXT("KILL_PLAYER");
		case StatisticsType::DEATH_COUNT: return TEXT("DEATH_COUNT");
		case StatisticsType::HP_RECOVERY_MYSELF: return TEXT("HP_RECOVERY_MYSELF");
		case StatisticsType::HP_RECOVERY_PARTY: return TEXT("HP_RECOVERY_PARTY");
		case StatisticsType::EXP_GAIN: return TEXT("EXP_GAIN");
		case StatisticsType::FAIRY_SUMMON: return TEXT("FAIRY_SUMMON");
		case StatisticsType::FAIRY_ENCHANT: return TEXT("FAIRY_ENCHANT");
		case StatisticsType::EQUIPMENT_ENCHANT: return TEXT("EQUIPMENT_ENCHANT");
		case StatisticsType::COSTUME_SUMMON: return TEXT("COSTUME_SUMMON");
		case StatisticsType::ITEM_CRAFT: return TEXT("ITEM_CRAFT");
		case StatisticsType::ITEM_GET_NORMAL_EQUIPMENT: return TEXT("ITEM_GET_NORMAL_EQUIPMENT");
		case StatisticsType::ITEM_GET_MAGIC_EQUIPMENT: return TEXT("ITEM_GET_MAGIC_EQUIPMENT");
		case StatisticsType::ITEM_GET_RARE_EQUIPMENT: return TEXT("ITEM_GET_RARE_EQUIPMENT");
		case StatisticsType::ITEM_GET_EPIC_EQUIPMENT: return TEXT("ITEM_GET_EPIC_EQUIPMENT");
		case StatisticsType::ITEM_GET_UNIQUE_EQUIPMENT: return TEXT("ITEM_GET_UNIQUE_EQUIPMENT");
		case StatisticsType::ITEM_USE_HP_POTION: return TEXT("ITEM_USE_HP_POTION");
		case StatisticsType::ITEM_USE_FOOD_BUFF: return TEXT("ITEM_USE_FOOD_BUFF");
		case StatisticsType::ITEM_USE_SCROLL_BUFF: return TEXT("ITEM_USE_SCROLL_BUFF");
		case StatisticsType::CURRENCY_GOLD_ACCUMULATE_GET: return TEXT("CURRENCY_GOLD_ACCUMULATE_GET");
		case StatisticsType::CURRENCY_GOLD_ACCUMULATE_USE: return TEXT("CURRENCY_GOLD_ACCUMULATE_USE");
		case StatisticsType::LOGIN_TIME_ACCUMULATE: return TEXT("LOGIN_TIME_ACCUMULATE");
		case StatisticsType::DAMAGE_INFLICT_TOTAL_NORMAL: return TEXT("DAMAGE_INFLICT_TOTAL_NORMAL");
		case StatisticsType::DAMAGE_INFLICT_TOTAL_SKILL: return TEXT("DAMAGE_INFLICT_TOTAL_SKILL");
		case StatisticsType::LOGIN_DATE_ACCUMULATE: return TEXT("LOGIN_DATE_ACCUMULATE");
	}
	return FString();
}
inline FString StatisticsCalculateTypeEnumToString(StatisticsCalculateType value)
{
	switch (value)
	{
		case StatisticsCalculateType::NONE: return TEXT("NONE");
		case StatisticsCalculateType::CALCULATE_ACCUMULATE: return TEXT("CALCULATE_ACCUMULATE");
		case StatisticsCalculateType::CALCULATE_MAX: return TEXT("CALCULATE_MAX");
		case StatisticsCalculateType::CALCULATE_RATIO: return TEXT("CALCULATE_RATIO");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString StatisticsTypeEnumToDisplayKey(StatisticsType value)
{
	switch (value)
	{
		case StatisticsType::NONE: return TEXT("");
		case StatisticsType::DAMAGE_INFLICT_TOTAL: return TEXT("");
		case StatisticsType::DAMAGE_INFLICT_MAX: return TEXT("");
		case StatisticsType::CRITICAL_HIT_INFLICT_RATE: return TEXT("");
		case StatisticsType::DAMAGE_RECEIVE_TOTAL: return TEXT("");
		case StatisticsType::DAMAGE_RECEIVE_MAX: return TEXT("");
		case StatisticsType::CRITICAL_HIT_RECEIVE_RATE: return TEXT("");
		case StatisticsType::KILL_MONSTER: return TEXT("");
		case StatisticsType::KILL_PLAYER: return TEXT("");
		case StatisticsType::DEATH_COUNT: return TEXT("");
		case StatisticsType::HP_RECOVERY_MYSELF: return TEXT("");
		case StatisticsType::HP_RECOVERY_PARTY: return TEXT("");
		case StatisticsType::EXP_GAIN: return TEXT("");
		case StatisticsType::FAIRY_SUMMON: return TEXT("");
		case StatisticsType::FAIRY_ENCHANT: return TEXT("");
		case StatisticsType::EQUIPMENT_ENCHANT: return TEXT("");
		case StatisticsType::COSTUME_SUMMON: return TEXT("");
		case StatisticsType::ITEM_CRAFT: return TEXT("");
		case StatisticsType::ITEM_GET_NORMAL_EQUIPMENT: return TEXT("");
		case StatisticsType::ITEM_GET_MAGIC_EQUIPMENT: return TEXT("");
		case StatisticsType::ITEM_GET_RARE_EQUIPMENT: return TEXT("");
		case StatisticsType::ITEM_GET_EPIC_EQUIPMENT: return TEXT("");
		case StatisticsType::ITEM_GET_UNIQUE_EQUIPMENT: return TEXT("");
		case StatisticsType::ITEM_USE_HP_POTION: return TEXT("");
		case StatisticsType::ITEM_USE_FOOD_BUFF: return TEXT("");
		case StatisticsType::ITEM_USE_SCROLL_BUFF: return TEXT("");
		case StatisticsType::CURRENCY_GOLD_ACCUMULATE_GET: return TEXT("");
		case StatisticsType::CURRENCY_GOLD_ACCUMULATE_USE: return TEXT("");
		case StatisticsType::LOGIN_TIME_ACCUMULATE: return TEXT("");
		case StatisticsType::DAMAGE_INFLICT_TOTAL_NORMAL: return TEXT("");
		case StatisticsType::DAMAGE_INFLICT_TOTAL_SKILL: return TEXT("");
		case StatisticsType::LOGIN_DATE_ACCUMULATE: return TEXT("");
	}
	return FString();
}
inline FString StatisticsCalculateTypeEnumToDisplayKey(StatisticsCalculateType value)
{
	switch (value)
	{
		case StatisticsCalculateType::NONE: return TEXT("");
		case StatisticsCalculateType::CALCULATE_ACCUMULATE: return TEXT("");
		case StatisticsCalculateType::CALCULATE_MAX: return TEXT("");
		case StatisticsCalculateType::CALCULATE_RATIO: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidStatisticsType(StatisticsType value) noexcept
{
	return (value >= StatisticsType::NONE && value < StatisticsType::MAX);
}

inline bool IsValidStatisticsCalculateType(StatisticsCalculateType value) noexcept
{
	return (value >= StatisticsCalculateType::NONE && value < StatisticsCalculateType::MAX);
}


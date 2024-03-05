#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedCommunityEventEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 커뮤니티 이벤트
enum class CommunityEventType : uint8
{

	// 배너로만 사용
	BANNER = 0,

	// 출석부
	ATTENDANCE = 1,

	// 미션
	MISSION = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 커뮤니티 이벤트 출석부
enum class AttendanceType : uint8
{

	// 기본 출석부
	DEFAULT = 0,

	// 신규 유저 출석부
	NEW = 1,

	// 복귀 유저 출석부
	RETURNING = 2,

	// 이벤트
	EVENT = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 유저의 접속 상태를 체크하기 위함
enum class CommunityEventConditionType : uint8
{

	// 기본 유저
	DEFAULT_USER = 0,

	// 신규 유저
	NEW_USER = 1,

	// 복귀 유저
	RETURNING_USER = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 커뮤니티 이벤트 미션 타입
enum class CommunityEventMissionType : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// 캐릭터 레벨
	CHARACTER_LEVEL = 1,

	// 기사단 기부
	GUILD_CONTRIBUTE = 2,

	// 몬스터 사냥
	KILL_MONSTER = 3,

	// 아이템 강화
	ITEM_ENCHANT = 4,

	// 아이템 재련
	ITEM_REFINE = 5,

	// 아이템 제작
	ITEM_CRAFT = 6,

	// 아이템 루팅
	ITEM_LOOT = 7,

	// 아이템 분해
	ITEM_DECOMPOSE = 8,

	// 장착 아이템 등급 체크
	ITEM_EQUIP = 9,

	// 아이템 습득
	ITEM_GET = 10,

	// 아이템 제거(재료형 아이템)
	ITEM_DELETE = 11,

	// 아이템 누적 소모량
	ITEM_ACCUMULATE_USE = 12,

	// 기여도 체크
	RANK_CONTRIBUTE = 13,

	// 커뮤니티 이벤트 미션 달성 정도 체크
	COMMUNITY_EVENT_MISSION_CLEAR = 14,

	// 서버당 누적 출석 여부 체크
	LOGIN_ACCUMULATE = 15,

	// 서버당 출석 연속 최대 횟수 체크
	LOGIN_CONTINUOUS_MAX = 16,

	// 재화 타입 획득량 체크
	CURRENCY_ACCUMULATE_GET = 17,

	// 재화 타입 소비량 체크
	CURRENCY_ACCUMULATE_CONSUME = 18,

	// 코스튬 소환 체크
	COSTUME_SUMMON = 19,

	// 코스튬 합성 체크
	COSTUME_COMPOSE = 20,

	// 페어리 소환 체크
	FAIRY_SUMMON = 21,

	// 페어리_합성 체크
	FAIRY_COMPOSE = 22,

	// 코스튬 소환 마일리지 획득량 체크
	COSTUME_SUMMON_MILEAGE = 23,

	// 페어리 소환 마일리지 획득량 체크
	FAIRY_SUMMON_MILEAGE = 24,

	// BM 상점 내 상품 구매 체크
	BM_SHOP_BUY_ITEM = 25,

	// 결제량 체크
	BM_SHOP_PURCHACE_CASH = 26,

	// 기사단 출석 횟수 체크
	GUILD_ATTENDANCE = 27,

	// 퀘스트 수행 체크
	QUEST_COMPLETE = 28,

	// 기사단 던전 입장
	GUILD_DUNGEON_ENTER = 29,

	// 기사단 던전 클리어 여부 체크
	GUILD_DUNGEON_CLEAR = 30,

	// 천마석 강화(잠식)
	SPIRIT_SHOT_ENCHANT = 31,

	// 그림자 전장 참여. 전투 시간이 끝나고, 승패가 결정될 때, 소속 여부에 관계없이 카운트
	GUILD_ARENA_PLAY = 32,

	// 그림자 전장 승리. 전투 시간이 끝나고, 승패가 결정될 때, 승리한 쪽에 포함되어 있을시 카운트
	GUILD_ARENA_WIN = 33,

	MAX = 34,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline CommunityEventType CommunityEventTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("BANNER"), ESearchCase::IgnoreCase) == 0)	return CommunityEventType::BANNER;
	if (str.Compare(TEXT("ATTENDANCE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventType::ATTENDANCE;
	if (str.Compare(TEXT("MISSION"), ESearchCase::IgnoreCase) == 0)	return CommunityEventType::MISSION;
	return CommunityEventType::MAX;
}
inline AttendanceType AttendanceTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("DEFAULT"), ESearchCase::IgnoreCase) == 0)	return AttendanceType::DEFAULT;
	if (str.Compare(TEXT("NEW"), ESearchCase::IgnoreCase) == 0)	return AttendanceType::NEW;
	if (str.Compare(TEXT("RETURNING"), ESearchCase::IgnoreCase) == 0)	return AttendanceType::RETURNING;
	if (str.Compare(TEXT("EVENT"), ESearchCase::IgnoreCase) == 0)	return AttendanceType::EVENT;
	return AttendanceType::MAX;
}
inline CommunityEventConditionType CommunityEventConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("DEFAULT_USER"), ESearchCase::IgnoreCase) == 0)	return CommunityEventConditionType::DEFAULT_USER;
	if (str.Compare(TEXT("NEW_USER"), ESearchCase::IgnoreCase) == 0)	return CommunityEventConditionType::NEW_USER;
	if (str.Compare(TEXT("RETURNING_USER"), ESearchCase::IgnoreCase) == 0)	return CommunityEventConditionType::RETURNING_USER;
	return CommunityEventConditionType::MAX;
}
inline CommunityEventMissionType CommunityEventMissionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::NONE;
	if (str.Compare(TEXT("CHARACTER_LEVEL"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::CHARACTER_LEVEL;
	if (str.Compare(TEXT("GUILD_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::GUILD_CONTRIBUTE;
	if (str.Compare(TEXT("KILL_MONSTER"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::KILL_MONSTER;
	if (str.Compare(TEXT("ITEM_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::ITEM_ENCHANT;
	if (str.Compare(TEXT("ITEM_REFINE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::ITEM_REFINE;
	if (str.Compare(TEXT("ITEM_CRAFT"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::ITEM_CRAFT;
	if (str.Compare(TEXT("ITEM_LOOT"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::ITEM_LOOT;
	if (str.Compare(TEXT("ITEM_DECOMPOSE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::ITEM_DECOMPOSE;
	if (str.Compare(TEXT("ITEM_EQUIP"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::ITEM_EQUIP;
	if (str.Compare(TEXT("ITEM_GET"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::ITEM_GET;
	if (str.Compare(TEXT("ITEM_DELETE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::ITEM_DELETE;
	if (str.Compare(TEXT("ITEM_ACCUMULATE_USE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::ITEM_ACCUMULATE_USE;
	if (str.Compare(TEXT("RANK_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::RANK_CONTRIBUTE;
	if (str.Compare(TEXT("COMMUNITY_EVENT_MISSION_CLEAR"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::COMMUNITY_EVENT_MISSION_CLEAR;
	if (str.Compare(TEXT("LOGIN_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::LOGIN_ACCUMULATE;
	if (str.Compare(TEXT("LOGIN_CONTINUOUS_MAX"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::LOGIN_CONTINUOUS_MAX;
	if (str.Compare(TEXT("CURRENCY_ACCUMULATE_GET"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::CURRENCY_ACCUMULATE_GET;
	if (str.Compare(TEXT("CURRENCY_ACCUMULATE_CONSUME"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::CURRENCY_ACCUMULATE_CONSUME;
	if (str.Compare(TEXT("COSTUME_SUMMON"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::COSTUME_SUMMON;
	if (str.Compare(TEXT("COSTUME_COMPOSE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::COSTUME_COMPOSE;
	if (str.Compare(TEXT("FAIRY_SUMMON"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::FAIRY_SUMMON;
	if (str.Compare(TEXT("FAIRY_COMPOSE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::FAIRY_COMPOSE;
	if (str.Compare(TEXT("COSTUME_SUMMON_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::COSTUME_SUMMON_MILEAGE;
	if (str.Compare(TEXT("FAIRY_SUMMON_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::FAIRY_SUMMON_MILEAGE;
	if (str.Compare(TEXT("BM_SHOP_BUY_ITEM"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::BM_SHOP_BUY_ITEM;
	if (str.Compare(TEXT("BM_SHOP_PURCHACE_CASH"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::BM_SHOP_PURCHACE_CASH;
	if (str.Compare(TEXT("GUILD_ATTENDANCE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::GUILD_ATTENDANCE;
	if (str.Compare(TEXT("QUEST_COMPLETE"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::QUEST_COMPLETE;
	if (str.Compare(TEXT("GUILD_DUNGEON_ENTER"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::GUILD_DUNGEON_ENTER;
	if (str.Compare(TEXT("GUILD_DUNGEON_CLEAR"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::GUILD_DUNGEON_CLEAR;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::SPIRIT_SHOT_ENCHANT;
	if (str.Compare(TEXT("GUILD_ARENA_PLAY"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::GUILD_ARENA_PLAY;
	if (str.Compare(TEXT("GUILD_ARENA_WIN"), ESearchCase::IgnoreCase) == 0)	return CommunityEventMissionType::GUILD_ARENA_WIN;
	return CommunityEventMissionType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString CommunityEventTypeEnumToString(CommunityEventType value)
{
	switch (value)
	{
		case CommunityEventType::BANNER: return TEXT("BANNER");
		case CommunityEventType::ATTENDANCE: return TEXT("ATTENDANCE");
		case CommunityEventType::MISSION: return TEXT("MISSION");
	}
	return FString();
}
inline FString AttendanceTypeEnumToString(AttendanceType value)
{
	switch (value)
	{
		case AttendanceType::DEFAULT: return TEXT("DEFAULT");
		case AttendanceType::NEW: return TEXT("NEW");
		case AttendanceType::RETURNING: return TEXT("RETURNING");
		case AttendanceType::EVENT: return TEXT("EVENT");
	}
	return FString();
}
inline FString CommunityEventConditionTypeEnumToString(CommunityEventConditionType value)
{
	switch (value)
	{
		case CommunityEventConditionType::DEFAULT_USER: return TEXT("DEFAULT_USER");
		case CommunityEventConditionType::NEW_USER: return TEXT("NEW_USER");
		case CommunityEventConditionType::RETURNING_USER: return TEXT("RETURNING_USER");
	}
	return FString();
}
inline FString CommunityEventMissionTypeEnumToString(CommunityEventMissionType value)
{
	switch (value)
	{
		case CommunityEventMissionType::NONE: return TEXT("NONE");
		case CommunityEventMissionType::CHARACTER_LEVEL: return TEXT("CHARACTER_LEVEL");
		case CommunityEventMissionType::GUILD_CONTRIBUTE: return TEXT("GUILD_CONTRIBUTE");
		case CommunityEventMissionType::KILL_MONSTER: return TEXT("KILL_MONSTER");
		case CommunityEventMissionType::ITEM_ENCHANT: return TEXT("ITEM_ENCHANT");
		case CommunityEventMissionType::ITEM_REFINE: return TEXT("ITEM_REFINE");
		case CommunityEventMissionType::ITEM_CRAFT: return TEXT("ITEM_CRAFT");
		case CommunityEventMissionType::ITEM_LOOT: return TEXT("ITEM_LOOT");
		case CommunityEventMissionType::ITEM_DECOMPOSE: return TEXT("ITEM_DECOMPOSE");
		case CommunityEventMissionType::ITEM_EQUIP: return TEXT("ITEM_EQUIP");
		case CommunityEventMissionType::ITEM_GET: return TEXT("ITEM_GET");
		case CommunityEventMissionType::ITEM_DELETE: return TEXT("ITEM_DELETE");
		case CommunityEventMissionType::ITEM_ACCUMULATE_USE: return TEXT("ITEM_ACCUMULATE_USE");
		case CommunityEventMissionType::RANK_CONTRIBUTE: return TEXT("RANK_CONTRIBUTE");
		case CommunityEventMissionType::COMMUNITY_EVENT_MISSION_CLEAR: return TEXT("COMMUNITY_EVENT_MISSION_CLEAR");
		case CommunityEventMissionType::LOGIN_ACCUMULATE: return TEXT("LOGIN_ACCUMULATE");
		case CommunityEventMissionType::LOGIN_CONTINUOUS_MAX: return TEXT("LOGIN_CONTINUOUS_MAX");
		case CommunityEventMissionType::CURRENCY_ACCUMULATE_GET: return TEXT("CURRENCY_ACCUMULATE_GET");
		case CommunityEventMissionType::CURRENCY_ACCUMULATE_CONSUME: return TEXT("CURRENCY_ACCUMULATE_CONSUME");
		case CommunityEventMissionType::COSTUME_SUMMON: return TEXT("COSTUME_SUMMON");
		case CommunityEventMissionType::COSTUME_COMPOSE: return TEXT("COSTUME_COMPOSE");
		case CommunityEventMissionType::FAIRY_SUMMON: return TEXT("FAIRY_SUMMON");
		case CommunityEventMissionType::FAIRY_COMPOSE: return TEXT("FAIRY_COMPOSE");
		case CommunityEventMissionType::COSTUME_SUMMON_MILEAGE: return TEXT("COSTUME_SUMMON_MILEAGE");
		case CommunityEventMissionType::FAIRY_SUMMON_MILEAGE: return TEXT("FAIRY_SUMMON_MILEAGE");
		case CommunityEventMissionType::BM_SHOP_BUY_ITEM: return TEXT("BM_SHOP_BUY_ITEM");
		case CommunityEventMissionType::BM_SHOP_PURCHACE_CASH: return TEXT("BM_SHOP_PURCHACE_CASH");
		case CommunityEventMissionType::GUILD_ATTENDANCE: return TEXT("GUILD_ATTENDANCE");
		case CommunityEventMissionType::QUEST_COMPLETE: return TEXT("QUEST_COMPLETE");
		case CommunityEventMissionType::GUILD_DUNGEON_ENTER: return TEXT("GUILD_DUNGEON_ENTER");
		case CommunityEventMissionType::GUILD_DUNGEON_CLEAR: return TEXT("GUILD_DUNGEON_CLEAR");
		case CommunityEventMissionType::SPIRIT_SHOT_ENCHANT: return TEXT("SPIRIT_SHOT_ENCHANT");
		case CommunityEventMissionType::GUILD_ARENA_PLAY: return TEXT("GUILD_ARENA_PLAY");
		case CommunityEventMissionType::GUILD_ARENA_WIN: return TEXT("GUILD_ARENA_WIN");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString CommunityEventTypeEnumToDisplayKey(CommunityEventType value)
{
	switch (value)
	{
		case CommunityEventType::BANNER: return TEXT("");
		case CommunityEventType::ATTENDANCE: return TEXT("");
		case CommunityEventType::MISSION: return TEXT("");
	}
	return FString();
}
inline FString AttendanceTypeEnumToDisplayKey(AttendanceType value)
{
	switch (value)
	{
		case AttendanceType::DEFAULT: return TEXT("");
		case AttendanceType::NEW: return TEXT("");
		case AttendanceType::RETURNING: return TEXT("");
		case AttendanceType::EVENT: return TEXT("");
	}
	return FString();
}
inline FString CommunityEventConditionTypeEnumToDisplayKey(CommunityEventConditionType value)
{
	switch (value)
	{
		case CommunityEventConditionType::DEFAULT_USER: return TEXT("");
		case CommunityEventConditionType::NEW_USER: return TEXT("");
		case CommunityEventConditionType::RETURNING_USER: return TEXT("");
	}
	return FString();
}
inline FString CommunityEventMissionTypeEnumToDisplayKey(CommunityEventMissionType value)
{
	switch (value)
	{
		case CommunityEventMissionType::NONE: return TEXT("");
		case CommunityEventMissionType::CHARACTER_LEVEL: return TEXT("");
		case CommunityEventMissionType::GUILD_CONTRIBUTE: return TEXT("");
		case CommunityEventMissionType::KILL_MONSTER: return TEXT("");
		case CommunityEventMissionType::ITEM_ENCHANT: return TEXT("");
		case CommunityEventMissionType::ITEM_REFINE: return TEXT("");
		case CommunityEventMissionType::ITEM_CRAFT: return TEXT("");
		case CommunityEventMissionType::ITEM_LOOT: return TEXT("");
		case CommunityEventMissionType::ITEM_DECOMPOSE: return TEXT("");
		case CommunityEventMissionType::ITEM_EQUIP: return TEXT("");
		case CommunityEventMissionType::ITEM_GET: return TEXT("");
		case CommunityEventMissionType::ITEM_DELETE: return TEXT("");
		case CommunityEventMissionType::ITEM_ACCUMULATE_USE: return TEXT("");
		case CommunityEventMissionType::RANK_CONTRIBUTE: return TEXT("");
		case CommunityEventMissionType::COMMUNITY_EVENT_MISSION_CLEAR: return TEXT("");
		case CommunityEventMissionType::LOGIN_ACCUMULATE: return TEXT("");
		case CommunityEventMissionType::LOGIN_CONTINUOUS_MAX: return TEXT("");
		case CommunityEventMissionType::CURRENCY_ACCUMULATE_GET: return TEXT("");
		case CommunityEventMissionType::CURRENCY_ACCUMULATE_CONSUME: return TEXT("");
		case CommunityEventMissionType::COSTUME_SUMMON: return TEXT("");
		case CommunityEventMissionType::COSTUME_COMPOSE: return TEXT("");
		case CommunityEventMissionType::FAIRY_SUMMON: return TEXT("");
		case CommunityEventMissionType::FAIRY_COMPOSE: return TEXT("");
		case CommunityEventMissionType::COSTUME_SUMMON_MILEAGE: return TEXT("");
		case CommunityEventMissionType::FAIRY_SUMMON_MILEAGE: return TEXT("");
		case CommunityEventMissionType::BM_SHOP_BUY_ITEM: return TEXT("");
		case CommunityEventMissionType::BM_SHOP_PURCHACE_CASH: return TEXT("");
		case CommunityEventMissionType::GUILD_ATTENDANCE: return TEXT("");
		case CommunityEventMissionType::QUEST_COMPLETE: return TEXT("");
		case CommunityEventMissionType::GUILD_DUNGEON_ENTER: return TEXT("");
		case CommunityEventMissionType::GUILD_DUNGEON_CLEAR: return TEXT("");
		case CommunityEventMissionType::SPIRIT_SHOT_ENCHANT: return TEXT("");
		case CommunityEventMissionType::GUILD_ARENA_PLAY: return TEXT("");
		case CommunityEventMissionType::GUILD_ARENA_WIN: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidCommunityEventType(CommunityEventType value) noexcept
{
	return (value >= CommunityEventType::BANNER && value < CommunityEventType::MAX);
}

inline bool IsValidAttendanceType(AttendanceType value) noexcept
{
	return (value >= AttendanceType::DEFAULT && value < AttendanceType::MAX);
}

inline bool IsValidCommunityEventConditionType(CommunityEventConditionType value) noexcept
{
	return (value >= CommunityEventConditionType::DEFAULT_USER && value < CommunityEventConditionType::MAX);
}

inline bool IsValidCommunityEventMissionType(CommunityEventMissionType value) noexcept
{
	return (value >= CommunityEventMissionType::NONE && value < CommunityEventMissionType::MAX);
}


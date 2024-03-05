#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedBattlePassEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 배틀 패스 노출 조건 타입
enum class BattlePassExposureConditionType : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// 레벨 N 이상 / 이하
	LEVEL = 1,

	// N일 이상 미접 이후 복귀
	RETURNING = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 배틀 패스 미션 타입
enum class BattlePassMissionType : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// 캐릭터 레벨
	CHARACTER_LEVEL = 1,

	// 1회 공격 시 데미지 n
	DAMAGE_ONE_TIME = 2,

	// 친구 등록
	FRIEND_MAKE = 3,

	// 타 플레이어 처치 횟수
	KILL_PLAYER = 4,

	// 누적 로그인
	LOGIN_ACCUMULATE = 5,

	// 거래소에서 구매
	MARKET_BUY = 6,

	// 거래소에서 판매 성공
	MARKET_SELL = 7,

	// 기사단 기부
	GUILD_CONTRIBUTE = 8,

	// 몬스터 사냥
	KILL_MONSTER = 9,

	// 몬스터에게 사망한 횟수
	DEATH_BY_MONSTER = 10,

	// 플레이어에게 사망한 횟수
	DEATH_BY_PLAYER = 11,

	// 시공의 틈새에서 몬스터 처치
	SPACE_CRACK_KILL_MONSTER = 12,

	// 시공의 틈새 클리어
	SPACE_CRACK_CLEAR = 13,

	// 아이템 누적 사용 횟수 n
	ITEM_ACCUMULATE_USE = 14,

	// NPC 상점 아이템 구매
	NPC_SHOP_BUY_ITEM = 15,

	// 아이템 강화
	ITEM_ENCHANT = 16,

	// 아이템 재련
	ITEM_REFINE = 17,

	// 아이템 제작
	ITEM_CRAFT = 18,

	// 아이템 루팅
	ITEM_LOOT = 19,

	// 재화 누적 소비량
	CURRENCY_ACCUMULATE_CONSUME = 20,

	// 재화 누적 획득량
	CURRENCY_ACCUMULATE_GET = 21,

	// 코스튬 획득
	COSTUME_GET = 22,

	// 코스튬 소환
	COSTUME_SUMMON = 23,

	// 코스튬 합성
	COSTUME_COMPOSE = 24,

	// 퀘스트 클리어
	QUEST_COMPLETE = 25,

	// 페어리 획득
	FAIRY_GET = 26,

	// 페어리 소환
	FAIRY_SUMMON = 27,

	// 페어리 합성
	FAIRY_COMPOSE = 28,

	// 페어리 레벨 달성
	FAIRY_GET_LEVEL = 29,

	// 오프라인 플레이 누적 시간(분)
	OFFLINE_PLAY_TIME_ACCUMULATE = 30,

	// PVP 단계 달성
	PVP_ALIGNMENT_ACHIEVEMENT = 31,

	// 사망 페널티 복구
	DEATH_PENALTY_RESTORE = 32,

	// 장소 이동
	WARP = 33,

	// NPC와 대화
	INTERACT_NPC = 34,

	// 거래소 구매 금액 누적
	MARKET_BUY_PRICE_ACCUMULATE = 35,

	// 거래서 판매 금액 누적
	MARKET_SELL_PRICE_ACCUMULATE = 36,

	// 기사단 출석
	GUILD_ATTENDANCE = 37,

	// 기사단 던전 클리어
	GUILD_DUNGEON_CLEAR = 38,

	// 보물 몬스터 수호
	SAVE_TREASURE_MONSTER = 39,

	// 스킬 습득
	SKILL_LEARN = 40,

	// BM SHOP 아이템 구매
	BM_SHOP_BUY_ITEM = 41,

	// 아이템 제거
	ITEM_DELETE = 42,

	// 장착 아이템 등급 체크
	ITEM_EQUIP = 43,

	// 침공전 n회 참여
	INVADE_COUNT = 44,

	// 침공전 플레이어 킬
	INVADE_KILL_PLAYER = 45,

	// 침공전 플레이어에 의해 죽음
	INVADE_DEATH_BY_PLAYER = 46,

	// 아이템 습득
	ITEM_GET = 47,

	// 시네마틱 보상 획득
	CINEMATIC_REWARD = 48,

	// 아이템 분해 횟수 n회
	ITEM_DECOMPOSE_TRY = 49,

	// 몬스터 처치시 기여도 달성 x위 안에 들기
	RANK_CONTRIBUTE = 50,

	MAX = 51,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline BattlePassExposureConditionType BattlePassExposureConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return BattlePassExposureConditionType::NONE;
	if (str.Compare(TEXT("LEVEL"), ESearchCase::IgnoreCase) == 0)	return BattlePassExposureConditionType::LEVEL;
	if (str.Compare(TEXT("RETURNING"), ESearchCase::IgnoreCase) == 0)	return BattlePassExposureConditionType::RETURNING;
	return BattlePassExposureConditionType::MAX;
}
inline BattlePassMissionType BattlePassMissionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::NONE;
	if (str.Compare(TEXT("CHARACTER_LEVEL"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::CHARACTER_LEVEL;
	if (str.Compare(TEXT("DAMAGE_ONE_TIME"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::DAMAGE_ONE_TIME;
	if (str.Compare(TEXT("FRIEND_MAKE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::FRIEND_MAKE;
	if (str.Compare(TEXT("KILL_PLAYER"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::KILL_PLAYER;
	if (str.Compare(TEXT("LOGIN_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::LOGIN_ACCUMULATE;
	if (str.Compare(TEXT("MARKET_BUY"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::MARKET_BUY;
	if (str.Compare(TEXT("MARKET_SELL"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::MARKET_SELL;
	if (str.Compare(TEXT("GUILD_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::GUILD_CONTRIBUTE;
	if (str.Compare(TEXT("KILL_MONSTER"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::KILL_MONSTER;
	if (str.Compare(TEXT("DEATH_BY_MONSTER"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::DEATH_BY_MONSTER;
	if (str.Compare(TEXT("DEATH_BY_PLAYER"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::DEATH_BY_PLAYER;
	if (str.Compare(TEXT("SPACE_CRACK_KILL_MONSTER"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::SPACE_CRACK_KILL_MONSTER;
	if (str.Compare(TEXT("SPACE_CRACK_CLEAR"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::SPACE_CRACK_CLEAR;
	if (str.Compare(TEXT("ITEM_ACCUMULATE_USE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::ITEM_ACCUMULATE_USE;
	if (str.Compare(TEXT("NPC_SHOP_BUY_ITEM"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::NPC_SHOP_BUY_ITEM;
	if (str.Compare(TEXT("ITEM_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::ITEM_ENCHANT;
	if (str.Compare(TEXT("ITEM_REFINE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::ITEM_REFINE;
	if (str.Compare(TEXT("ITEM_CRAFT"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::ITEM_CRAFT;
	if (str.Compare(TEXT("ITEM_LOOT"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::ITEM_LOOT;
	if (str.Compare(TEXT("CURRENCY_ACCUMULATE_CONSUME"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::CURRENCY_ACCUMULATE_CONSUME;
	if (str.Compare(TEXT("CURRENCY_ACCUMULATE_GET"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::CURRENCY_ACCUMULATE_GET;
	if (str.Compare(TEXT("COSTUME_GET"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::COSTUME_GET;
	if (str.Compare(TEXT("COSTUME_SUMMON"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::COSTUME_SUMMON;
	if (str.Compare(TEXT("COSTUME_COMPOSE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::COSTUME_COMPOSE;
	if (str.Compare(TEXT("QUEST_COMPLETE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::QUEST_COMPLETE;
	if (str.Compare(TEXT("FAIRY_GET"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::FAIRY_GET;
	if (str.Compare(TEXT("FAIRY_SUMMON"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::FAIRY_SUMMON;
	if (str.Compare(TEXT("FAIRY_COMPOSE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::FAIRY_COMPOSE;
	if (str.Compare(TEXT("FAIRY_GET_LEVEL"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::FAIRY_GET_LEVEL;
	if (str.Compare(TEXT("OFFLINE_PLAY_TIME_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::OFFLINE_PLAY_TIME_ACCUMULATE;
	if (str.Compare(TEXT("PVP_ALIGNMENT_ACHIEVEMENT"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::PVP_ALIGNMENT_ACHIEVEMENT;
	if (str.Compare(TEXT("DEATH_PENALTY_RESTORE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::DEATH_PENALTY_RESTORE;
	if (str.Compare(TEXT("WARP"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::WARP;
	if (str.Compare(TEXT("INTERACT_NPC"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::INTERACT_NPC;
	if (str.Compare(TEXT("MARKET_BUY_PRICE_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::MARKET_BUY_PRICE_ACCUMULATE;
	if (str.Compare(TEXT("MARKET_SELL_PRICE_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::MARKET_SELL_PRICE_ACCUMULATE;
	if (str.Compare(TEXT("GUILD_ATTENDANCE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::GUILD_ATTENDANCE;
	if (str.Compare(TEXT("GUILD_DUNGEON_CLEAR"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::GUILD_DUNGEON_CLEAR;
	if (str.Compare(TEXT("SAVE_TREASURE_MONSTER"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::SAVE_TREASURE_MONSTER;
	if (str.Compare(TEXT("SKILL_LEARN"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::SKILL_LEARN;
	if (str.Compare(TEXT("BM_SHOP_BUY_ITEM"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::BM_SHOP_BUY_ITEM;
	if (str.Compare(TEXT("ITEM_DELETE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::ITEM_DELETE;
	if (str.Compare(TEXT("ITEM_EQUIP"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::ITEM_EQUIP;
	if (str.Compare(TEXT("INVADE_COUNT"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::INVADE_COUNT;
	if (str.Compare(TEXT("INVADE_KILL_PLAYER"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::INVADE_KILL_PLAYER;
	if (str.Compare(TEXT("INVADE_DEATH_BY_PLAYER"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::INVADE_DEATH_BY_PLAYER;
	if (str.Compare(TEXT("ITEM_GET"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::ITEM_GET;
	if (str.Compare(TEXT("CINEMATIC_REWARD"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::CINEMATIC_REWARD;
	if (str.Compare(TEXT("ITEM_DECOMPOSE_TRY"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::ITEM_DECOMPOSE_TRY;
	if (str.Compare(TEXT("RANK_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return BattlePassMissionType::RANK_CONTRIBUTE;
	return BattlePassMissionType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString BattlePassExposureConditionTypeEnumToString(BattlePassExposureConditionType value)
{
	switch (value)
	{
		case BattlePassExposureConditionType::NONE: return TEXT("NONE");
		case BattlePassExposureConditionType::LEVEL: return TEXT("LEVEL");
		case BattlePassExposureConditionType::RETURNING: return TEXT("RETURNING");
	}
	return FString();
}
inline FString BattlePassMissionTypeEnumToString(BattlePassMissionType value)
{
	switch (value)
	{
		case BattlePassMissionType::NONE: return TEXT("NONE");
		case BattlePassMissionType::CHARACTER_LEVEL: return TEXT("CHARACTER_LEVEL");
		case BattlePassMissionType::DAMAGE_ONE_TIME: return TEXT("DAMAGE_ONE_TIME");
		case BattlePassMissionType::FRIEND_MAKE: return TEXT("FRIEND_MAKE");
		case BattlePassMissionType::KILL_PLAYER: return TEXT("KILL_PLAYER");
		case BattlePassMissionType::LOGIN_ACCUMULATE: return TEXT("LOGIN_ACCUMULATE");
		case BattlePassMissionType::MARKET_BUY: return TEXT("MARKET_BUY");
		case BattlePassMissionType::MARKET_SELL: return TEXT("MARKET_SELL");
		case BattlePassMissionType::GUILD_CONTRIBUTE: return TEXT("GUILD_CONTRIBUTE");
		case BattlePassMissionType::KILL_MONSTER: return TEXT("KILL_MONSTER");
		case BattlePassMissionType::DEATH_BY_MONSTER: return TEXT("DEATH_BY_MONSTER");
		case BattlePassMissionType::DEATH_BY_PLAYER: return TEXT("DEATH_BY_PLAYER");
		case BattlePassMissionType::SPACE_CRACK_KILL_MONSTER: return TEXT("SPACE_CRACK_KILL_MONSTER");
		case BattlePassMissionType::SPACE_CRACK_CLEAR: return TEXT("SPACE_CRACK_CLEAR");
		case BattlePassMissionType::ITEM_ACCUMULATE_USE: return TEXT("ITEM_ACCUMULATE_USE");
		case BattlePassMissionType::NPC_SHOP_BUY_ITEM: return TEXT("NPC_SHOP_BUY_ITEM");
		case BattlePassMissionType::ITEM_ENCHANT: return TEXT("ITEM_ENCHANT");
		case BattlePassMissionType::ITEM_REFINE: return TEXT("ITEM_REFINE");
		case BattlePassMissionType::ITEM_CRAFT: return TEXT("ITEM_CRAFT");
		case BattlePassMissionType::ITEM_LOOT: return TEXT("ITEM_LOOT");
		case BattlePassMissionType::CURRENCY_ACCUMULATE_CONSUME: return TEXT("CURRENCY_ACCUMULATE_CONSUME");
		case BattlePassMissionType::CURRENCY_ACCUMULATE_GET: return TEXT("CURRENCY_ACCUMULATE_GET");
		case BattlePassMissionType::COSTUME_GET: return TEXT("COSTUME_GET");
		case BattlePassMissionType::COSTUME_SUMMON: return TEXT("COSTUME_SUMMON");
		case BattlePassMissionType::COSTUME_COMPOSE: return TEXT("COSTUME_COMPOSE");
		case BattlePassMissionType::QUEST_COMPLETE: return TEXT("QUEST_COMPLETE");
		case BattlePassMissionType::FAIRY_GET: return TEXT("FAIRY_GET");
		case BattlePassMissionType::FAIRY_SUMMON: return TEXT("FAIRY_SUMMON");
		case BattlePassMissionType::FAIRY_COMPOSE: return TEXT("FAIRY_COMPOSE");
		case BattlePassMissionType::FAIRY_GET_LEVEL: return TEXT("FAIRY_GET_LEVEL");
		case BattlePassMissionType::OFFLINE_PLAY_TIME_ACCUMULATE: return TEXT("OFFLINE_PLAY_TIME_ACCUMULATE");
		case BattlePassMissionType::PVP_ALIGNMENT_ACHIEVEMENT: return TEXT("PVP_ALIGNMENT_ACHIEVEMENT");
		case BattlePassMissionType::DEATH_PENALTY_RESTORE: return TEXT("DEATH_PENALTY_RESTORE");
		case BattlePassMissionType::WARP: return TEXT("WARP");
		case BattlePassMissionType::INTERACT_NPC: return TEXT("INTERACT_NPC");
		case BattlePassMissionType::MARKET_BUY_PRICE_ACCUMULATE: return TEXT("MARKET_BUY_PRICE_ACCUMULATE");
		case BattlePassMissionType::MARKET_SELL_PRICE_ACCUMULATE: return TEXT("MARKET_SELL_PRICE_ACCUMULATE");
		case BattlePassMissionType::GUILD_ATTENDANCE: return TEXT("GUILD_ATTENDANCE");
		case BattlePassMissionType::GUILD_DUNGEON_CLEAR: return TEXT("GUILD_DUNGEON_CLEAR");
		case BattlePassMissionType::SAVE_TREASURE_MONSTER: return TEXT("SAVE_TREASURE_MONSTER");
		case BattlePassMissionType::SKILL_LEARN: return TEXT("SKILL_LEARN");
		case BattlePassMissionType::BM_SHOP_BUY_ITEM: return TEXT("BM_SHOP_BUY_ITEM");
		case BattlePassMissionType::ITEM_DELETE: return TEXT("ITEM_DELETE");
		case BattlePassMissionType::ITEM_EQUIP: return TEXT("ITEM_EQUIP");
		case BattlePassMissionType::INVADE_COUNT: return TEXT("INVADE_COUNT");
		case BattlePassMissionType::INVADE_KILL_PLAYER: return TEXT("INVADE_KILL_PLAYER");
		case BattlePassMissionType::INVADE_DEATH_BY_PLAYER: return TEXT("INVADE_DEATH_BY_PLAYER");
		case BattlePassMissionType::ITEM_GET: return TEXT("ITEM_GET");
		case BattlePassMissionType::CINEMATIC_REWARD: return TEXT("CINEMATIC_REWARD");
		case BattlePassMissionType::ITEM_DECOMPOSE_TRY: return TEXT("ITEM_DECOMPOSE_TRY");
		case BattlePassMissionType::RANK_CONTRIBUTE: return TEXT("RANK_CONTRIBUTE");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString BattlePassExposureConditionTypeEnumToDisplayKey(BattlePassExposureConditionType value)
{
	switch (value)
	{
		case BattlePassExposureConditionType::NONE: return TEXT("");
		case BattlePassExposureConditionType::LEVEL: return TEXT("");
		case BattlePassExposureConditionType::RETURNING: return TEXT("");
	}
	return FString();
}
inline FString BattlePassMissionTypeEnumToDisplayKey(BattlePassMissionType value)
{
	switch (value)
	{
		case BattlePassMissionType::NONE: return TEXT("");
		case BattlePassMissionType::CHARACTER_LEVEL: return TEXT("");
		case BattlePassMissionType::DAMAGE_ONE_TIME: return TEXT("");
		case BattlePassMissionType::FRIEND_MAKE: return TEXT("");
		case BattlePassMissionType::KILL_PLAYER: return TEXT("");
		case BattlePassMissionType::LOGIN_ACCUMULATE: return TEXT("");
		case BattlePassMissionType::MARKET_BUY: return TEXT("");
		case BattlePassMissionType::MARKET_SELL: return TEXT("");
		case BattlePassMissionType::GUILD_CONTRIBUTE: return TEXT("");
		case BattlePassMissionType::KILL_MONSTER: return TEXT("");
		case BattlePassMissionType::DEATH_BY_MONSTER: return TEXT("");
		case BattlePassMissionType::DEATH_BY_PLAYER: return TEXT("");
		case BattlePassMissionType::SPACE_CRACK_KILL_MONSTER: return TEXT("");
		case BattlePassMissionType::SPACE_CRACK_CLEAR: return TEXT("");
		case BattlePassMissionType::ITEM_ACCUMULATE_USE: return TEXT("");
		case BattlePassMissionType::NPC_SHOP_BUY_ITEM: return TEXT("");
		case BattlePassMissionType::ITEM_ENCHANT: return TEXT("");
		case BattlePassMissionType::ITEM_REFINE: return TEXT("");
		case BattlePassMissionType::ITEM_CRAFT: return TEXT("");
		case BattlePassMissionType::ITEM_LOOT: return TEXT("");
		case BattlePassMissionType::CURRENCY_ACCUMULATE_CONSUME: return TEXT("");
		case BattlePassMissionType::CURRENCY_ACCUMULATE_GET: return TEXT("");
		case BattlePassMissionType::COSTUME_GET: return TEXT("");
		case BattlePassMissionType::COSTUME_SUMMON: return TEXT("");
		case BattlePassMissionType::COSTUME_COMPOSE: return TEXT("");
		case BattlePassMissionType::QUEST_COMPLETE: return TEXT("");
		case BattlePassMissionType::FAIRY_GET: return TEXT("");
		case BattlePassMissionType::FAIRY_SUMMON: return TEXT("");
		case BattlePassMissionType::FAIRY_COMPOSE: return TEXT("");
		case BattlePassMissionType::FAIRY_GET_LEVEL: return TEXT("");
		case BattlePassMissionType::OFFLINE_PLAY_TIME_ACCUMULATE: return TEXT("");
		case BattlePassMissionType::PVP_ALIGNMENT_ACHIEVEMENT: return TEXT("");
		case BattlePassMissionType::DEATH_PENALTY_RESTORE: return TEXT("");
		case BattlePassMissionType::WARP: return TEXT("");
		case BattlePassMissionType::INTERACT_NPC: return TEXT("");
		case BattlePassMissionType::MARKET_BUY_PRICE_ACCUMULATE: return TEXT("");
		case BattlePassMissionType::MARKET_SELL_PRICE_ACCUMULATE: return TEXT("");
		case BattlePassMissionType::GUILD_ATTENDANCE: return TEXT("");
		case BattlePassMissionType::GUILD_DUNGEON_CLEAR: return TEXT("");
		case BattlePassMissionType::SAVE_TREASURE_MONSTER: return TEXT("");
		case BattlePassMissionType::SKILL_LEARN: return TEXT("");
		case BattlePassMissionType::BM_SHOP_BUY_ITEM: return TEXT("");
		case BattlePassMissionType::ITEM_DELETE: return TEXT("");
		case BattlePassMissionType::ITEM_EQUIP: return TEXT("");
		case BattlePassMissionType::INVADE_COUNT: return TEXT("");
		case BattlePassMissionType::INVADE_KILL_PLAYER: return TEXT("");
		case BattlePassMissionType::INVADE_DEATH_BY_PLAYER: return TEXT("");
		case BattlePassMissionType::ITEM_GET: return TEXT("");
		case BattlePassMissionType::CINEMATIC_REWARD: return TEXT("");
		case BattlePassMissionType::ITEM_DECOMPOSE_TRY: return TEXT("");
		case BattlePassMissionType::RANK_CONTRIBUTE: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidBattlePassExposureConditionType(BattlePassExposureConditionType value) noexcept
{
	return (value >= BattlePassExposureConditionType::NONE && value < BattlePassExposureConditionType::MAX);
}

inline bool IsValidBattlePassMissionType(BattlePassMissionType value) noexcept
{
	return (value >= BattlePassMissionType::NONE && value < BattlePassMissionType::MAX);
}


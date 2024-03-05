#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedAchievementEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 업적 타입
enum class AchievementType : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// 캐릭터 레벨
	CHARACTER_LEVEL = 1,

	// 캐릭터 경험치
	CHARACTER_EXP = 2,

	// 1회 공격 시 데미지 n 달성하기
	DAMAGE_ONE_TIME = 3,

	// 코스튬 소환 n회
	COSTUME_SUMMON = 4,

	// 코스튬 합성 n회
	COSTUME_COMPOSE = 5,

	// 코스튬 합성으로 특정 등급 이상 코스튬 획득
	COSTUME_COMPOSE_GET_GREATER_GRADE = 6,

	// 페어리 소환 n회
	FAIRY_SUMMON = 7,

	// 페어리 합성 n회
	FAIRY_COMPOSE = 8,

	// 페어리 합성으로 특정 등급 이상 페어리 획득
	FAIRY_COMPOSE_GET_GREATER_GRADE = 9,

	// 특정 레벨 페어리 획득
	FAIRY_GET_LEVEL = 10,

	// 아이템 강화 시도 n회
	ITEM_ENCHANT_TRY = 11,

	// 노멀 등급 이상 아이템 강화 n강화 성공
	ITEM_ENCHANT_SUCCESS_GREATER_NORMAL = 12,

	// 특정 카테고리 아이템 제련 횟수 n회
	ITEM_REFINE_TRY = 13,

	// 아이템 분해 횟수 n회
	ITEM_DECOMPOSE_TRY = 14,

	// 특정 등급 이상 아이템 제작 성공/대성공
	ITEM_CRAFT_RESULT_GREATER_GRADE = 15,

	// 등급에 따른 반복 퀘스트 완료 n회
	QUEST_COMPLETE_REPEAT_GRADE = 16,

	// 몬스터에게 사망한 횟수 n회
	DEATH_BY_MONSTER = 17,

	// 플레이어에게 사망한 횟수 n회
	DEATH_BY_PLAYER = 18,

	// 즉시 부활 사용 횟수 n회
	RESURRECT_INSTANT_USE = 19,

	// 타 플레이어 처치 횟수 n회
	KILL_PLAYER = 20,

	// 타 플레이어 처치 횟수 n회 - 인터서버
	KILL_PLAYER_INTER_SERVER = 21,

	// 특정 등급 몬스터 처치
	KILL_MONSTER_BY_GRADE = 22,

	// 특정 몬스터 n마리 처치
	KILL_MONSTER_BY_ID = 23,

	// 필드 보스 기여도 달성 x위 안에 들기 n회
	RANK_FIELD_BOSS_CONTRIBUTE = 24,

	// 레이드 보스 기여도 달성 x위 안에 들기 n회
	RANK_RAID_BOSS_CONTRIBUTE = 25,

	// 시공의 틈새 클리어 n회
	SPACE_CRACK_CLEAR = 26,

	// 시공의 틈새에서 특정 등급 몬스터 n마리 처치
	SPACE_CRACK_KILL_MONSTER = 27,

	// 친구 n명 만들기
	FRIEND_MAKE = 28,

	// 거래소에서 판매 성공 n회
	MARKET_SELL = 29,

	// 거래소에서 구매 n회
	MARKET_BUY = 30,

	// 기사단 기부하기 n회
	GUILD_CONTRIBUTE = 31,

	// 기사단 아이템 컬렉션 등록 n회
	GUILD_ITEM_COLLECTION_REGISTER = 32,

	// 누적 로그인 n회
	LOGIN_ACCUMULATE = 33,

	// 특정 재화 누적 소비량 n
	CURRENCY_ACCUMULATE_CONSUME = 34,

	// 특정 재화 누적 획득량 n
	CURRENCY_ACCUMULATE_GET = 35,

	// 특정 아이템 누적 사용 횟수 n
	ITEM_ACCUMULATE_USE = 36,

	// 특정 등급의 특정 카테고리 아이템 루팅 횟수 n
	ITEM_LOOT_GRADE_CATEGORY = 37,

	// 퀘스트 타입별 완료 횟수
	QUEST_COMPLETE_TYPE = 38,

	// 스킬 강화 시도 횟수 n
	SKILL_ENCHANT_TRY = 39,

	// 스킬 업그레이드 시도 횟수 n (Deprecated, 사용하지 않습니다)
	SKILL_UPGRADE_TRY = 40,

	// 특정 등급의 특정 타입 아이템 루팅 횟수 n
	ITEM_LOOT_GRADE_TYPE = 41,

	// npc 상점에서 특정 카테고리 아이템 구매 개수 n
	NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB = 42,

	// npc 상점에서 특정 id 아이템 구매 개수 n
	NPC_SHOP_BUY_ITEM_ITEM_ID = 43,

	// 특정 카테고리 아이템 사용 횟수 n
	ITEM_ACCUMULATE_USE_CATEGORY_MAIN_SUB = 44,

	// 특정 등급 코스튬을 목표로 합성 성공/실패 누적 횟수
	COSTUME_COMPOSE_RESULT_SPECIFIC_GRADE = 45,

	// 특정 등급 페어리 목표로 합성 성공/실패 누적 횟수
	FAIRY_COMPOSE_RESULT_SPECIFIC_GRADE = 46,

	// 아이템 컬렉션 완료 누적 횟수 체크
	ITEM_COLLECTION_COMPLETE = 47,

	// 아이템 제작 성공/실패 누적 횟수 체크 - 아이템 타입
	ITEM_CRAFT_RESULT = 48,

	// 스킬 강화 성공/실패 누적 횟수 체크
	SKILL_ENCHANT_RESULT = 49,

	// 적대 기사단 수호탑 파괴 누적 횟수 체크
	GUILD_CAMP_DESTROY = 50,

	// 특정 등급 코스튬을 목표로 합성 시도 횟수
	COSTUME_COMPOSE_TRY_SPECIFIC_GRADE = 51,

	// 특정 등급 페어리 목표로 합성 시도 횟수
	FAIRY_COMPOSE_TRY_SPECIFIC_GRADE = 52,

	// 아이템 제작 시도 - 아이템 타입
	ITEM_CRAFT_TRY_ITEM_TYPE = 53,

	// 기여도 - 성소 파괴
	RANK_SANCTUM_CONTRIBUTE = 54,

	// 봉인 - 각인 시도
	SEAL_NODE_REROLL = 55,

	// 페어리 - 레벨 성장 시도
	FAIRY_ENCHANT_TRY = 56,

	// 천마석 - 잠식 시도
	SPIRITSHOT_ENCHANT_TRY = 57,

	// 파티 던전 - 클리어 보상 획득
	PARTY_DUNGEON_CLEAR = 58,

	// 기사단 던전 - 단계 클리어(기여도 있어야 함)
	GUILD_DUNGEON_CLEAR = 59,

	// NpcFunctionType = INVADE_WAGON 처치 기여
	RANK_INVADE_WAGON_CONTRIBUTE = 60,

	// NpcFunctionType = INVADE_GUARDIAN 처치 기여
	RANK_INVADE_GUARDIAN_CONTRIBUTE = 61,

	// NpcFunctionType = INVADE_BOSS 처치 기여
	RANK_INVADE_BOSS_CONTRIBUTE = 62,

	// 전투력 n 도달
	CHARACTER_COMBATPOWER = 63,

	// 개화(마법 부여) 시도
	ITEM_MAGICALFORGE_TRY = 64,

	// 그림자 전장 - 승리 또는 패배
	GUILD_ARENA_ENTRY = 65,

	// NpcFunctionType = GUILD_BOSS 처치 기여
	RANK_GUILD_BOSS_CONTRIBUTE = 66,

	MAX = 67,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline AchievementType AchievementTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::NONE;
	if (str.Compare(TEXT("CHARACTER_LEVEL"), ESearchCase::IgnoreCase) == 0)	return AchievementType::CHARACTER_LEVEL;
	if (str.Compare(TEXT("CHARACTER_EXP"), ESearchCase::IgnoreCase) == 0)	return AchievementType::CHARACTER_EXP;
	if (str.Compare(TEXT("DAMAGE_ONE_TIME"), ESearchCase::IgnoreCase) == 0)	return AchievementType::DAMAGE_ONE_TIME;
	if (str.Compare(TEXT("COSTUME_SUMMON"), ESearchCase::IgnoreCase) == 0)	return AchievementType::COSTUME_SUMMON;
	if (str.Compare(TEXT("COSTUME_COMPOSE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::COSTUME_COMPOSE;
	if (str.Compare(TEXT("COSTUME_COMPOSE_GET_GREATER_GRADE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::COSTUME_COMPOSE_GET_GREATER_GRADE;
	if (str.Compare(TEXT("FAIRY_SUMMON"), ESearchCase::IgnoreCase) == 0)	return AchievementType::FAIRY_SUMMON;
	if (str.Compare(TEXT("FAIRY_COMPOSE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::FAIRY_COMPOSE;
	if (str.Compare(TEXT("FAIRY_COMPOSE_GET_GREATER_GRADE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::FAIRY_COMPOSE_GET_GREATER_GRADE;
	if (str.Compare(TEXT("FAIRY_GET_LEVEL"), ESearchCase::IgnoreCase) == 0)	return AchievementType::FAIRY_GET_LEVEL;
	if (str.Compare(TEXT("ITEM_ENCHANT_TRY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_ENCHANT_TRY;
	if (str.Compare(TEXT("ITEM_ENCHANT_SUCCESS_GREATER_NORMAL"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_ENCHANT_SUCCESS_GREATER_NORMAL;
	if (str.Compare(TEXT("ITEM_REFINE_TRY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_REFINE_TRY;
	if (str.Compare(TEXT("ITEM_DECOMPOSE_TRY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_DECOMPOSE_TRY;
	if (str.Compare(TEXT("ITEM_CRAFT_RESULT_GREATER_GRADE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_CRAFT_RESULT_GREATER_GRADE;
	if (str.Compare(TEXT("QUEST_COMPLETE_REPEAT_GRADE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::QUEST_COMPLETE_REPEAT_GRADE;
	if (str.Compare(TEXT("DEATH_BY_MONSTER"), ESearchCase::IgnoreCase) == 0)	return AchievementType::DEATH_BY_MONSTER;
	if (str.Compare(TEXT("DEATH_BY_PLAYER"), ESearchCase::IgnoreCase) == 0)	return AchievementType::DEATH_BY_PLAYER;
	if (str.Compare(TEXT("RESURRECT_INSTANT_USE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::RESURRECT_INSTANT_USE;
	if (str.Compare(TEXT("KILL_PLAYER"), ESearchCase::IgnoreCase) == 0)	return AchievementType::KILL_PLAYER;
	if (str.Compare(TEXT("KILL_PLAYER_INTER_SERVER"), ESearchCase::IgnoreCase) == 0)	return AchievementType::KILL_PLAYER_INTER_SERVER;
	if (str.Compare(TEXT("KILL_MONSTER_BY_GRADE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::KILL_MONSTER_BY_GRADE;
	if (str.Compare(TEXT("KILL_MONSTER_BY_ID"), ESearchCase::IgnoreCase) == 0)	return AchievementType::KILL_MONSTER_BY_ID;
	if (str.Compare(TEXT("RANK_FIELD_BOSS_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::RANK_FIELD_BOSS_CONTRIBUTE;
	if (str.Compare(TEXT("RANK_RAID_BOSS_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::RANK_RAID_BOSS_CONTRIBUTE;
	if (str.Compare(TEXT("SPACE_CRACK_CLEAR"), ESearchCase::IgnoreCase) == 0)	return AchievementType::SPACE_CRACK_CLEAR;
	if (str.Compare(TEXT("SPACE_CRACK_KILL_MONSTER"), ESearchCase::IgnoreCase) == 0)	return AchievementType::SPACE_CRACK_KILL_MONSTER;
	if (str.Compare(TEXT("FRIEND_MAKE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::FRIEND_MAKE;
	if (str.Compare(TEXT("MARKET_SELL"), ESearchCase::IgnoreCase) == 0)	return AchievementType::MARKET_SELL;
	if (str.Compare(TEXT("MARKET_BUY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::MARKET_BUY;
	if (str.Compare(TEXT("GUILD_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::GUILD_CONTRIBUTE;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION_REGISTER"), ESearchCase::IgnoreCase) == 0)	return AchievementType::GUILD_ITEM_COLLECTION_REGISTER;
	if (str.Compare(TEXT("LOGIN_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::LOGIN_ACCUMULATE;
	if (str.Compare(TEXT("CURRENCY_ACCUMULATE_CONSUME"), ESearchCase::IgnoreCase) == 0)	return AchievementType::CURRENCY_ACCUMULATE_CONSUME;
	if (str.Compare(TEXT("CURRENCY_ACCUMULATE_GET"), ESearchCase::IgnoreCase) == 0)	return AchievementType::CURRENCY_ACCUMULATE_GET;
	if (str.Compare(TEXT("ITEM_ACCUMULATE_USE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_ACCUMULATE_USE;
	if (str.Compare(TEXT("ITEM_LOOT_GRADE_CATEGORY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_LOOT_GRADE_CATEGORY;
	if (str.Compare(TEXT("QUEST_COMPLETE_TYPE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::QUEST_COMPLETE_TYPE;
	if (str.Compare(TEXT("SKILL_ENCHANT_TRY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::SKILL_ENCHANT_TRY;
	if (str.Compare(TEXT("SKILL_UPGRADE_TRY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::SKILL_UPGRADE_TRY;
	if (str.Compare(TEXT("ITEM_LOOT_GRADE_TYPE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_LOOT_GRADE_TYPE;
	if (str.Compare(TEXT("NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB"), ESearchCase::IgnoreCase) == 0)	return AchievementType::NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB;
	if (str.Compare(TEXT("NPC_SHOP_BUY_ITEM_ITEM_ID"), ESearchCase::IgnoreCase) == 0)	return AchievementType::NPC_SHOP_BUY_ITEM_ITEM_ID;
	if (str.Compare(TEXT("ITEM_ACCUMULATE_USE_CATEGORY_MAIN_SUB"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_ACCUMULATE_USE_CATEGORY_MAIN_SUB;
	if (str.Compare(TEXT("COSTUME_COMPOSE_RESULT_SPECIFIC_GRADE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::COSTUME_COMPOSE_RESULT_SPECIFIC_GRADE;
	if (str.Compare(TEXT("FAIRY_COMPOSE_RESULT_SPECIFIC_GRADE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::FAIRY_COMPOSE_RESULT_SPECIFIC_GRADE;
	if (str.Compare(TEXT("ITEM_COLLECTION_COMPLETE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_COLLECTION_COMPLETE;
	if (str.Compare(TEXT("ITEM_CRAFT_RESULT"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_CRAFT_RESULT;
	if (str.Compare(TEXT("SKILL_ENCHANT_RESULT"), ESearchCase::IgnoreCase) == 0)	return AchievementType::SKILL_ENCHANT_RESULT;
	if (str.Compare(TEXT("GUILD_CAMP_DESTROY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::GUILD_CAMP_DESTROY;
	if (str.Compare(TEXT("COSTUME_COMPOSE_TRY_SPECIFIC_GRADE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::COSTUME_COMPOSE_TRY_SPECIFIC_GRADE;
	if (str.Compare(TEXT("FAIRY_COMPOSE_TRY_SPECIFIC_GRADE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::FAIRY_COMPOSE_TRY_SPECIFIC_GRADE;
	if (str.Compare(TEXT("ITEM_CRAFT_TRY_ITEM_TYPE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_CRAFT_TRY_ITEM_TYPE;
	if (str.Compare(TEXT("RANK_SANCTUM_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::RANK_SANCTUM_CONTRIBUTE;
	if (str.Compare(TEXT("SEAL_NODE_REROLL"), ESearchCase::IgnoreCase) == 0)	return AchievementType::SEAL_NODE_REROLL;
	if (str.Compare(TEXT("FAIRY_ENCHANT_TRY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::FAIRY_ENCHANT_TRY;
	if (str.Compare(TEXT("SPIRITSHOT_ENCHANT_TRY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::SPIRITSHOT_ENCHANT_TRY;
	if (str.Compare(TEXT("PARTY_DUNGEON_CLEAR"), ESearchCase::IgnoreCase) == 0)	return AchievementType::PARTY_DUNGEON_CLEAR;
	if (str.Compare(TEXT("GUILD_DUNGEON_CLEAR"), ESearchCase::IgnoreCase) == 0)	return AchievementType::GUILD_DUNGEON_CLEAR;
	if (str.Compare(TEXT("RANK_INVADE_WAGON_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::RANK_INVADE_WAGON_CONTRIBUTE;
	if (str.Compare(TEXT("RANK_INVADE_GUARDIAN_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::RANK_INVADE_GUARDIAN_CONTRIBUTE;
	if (str.Compare(TEXT("RANK_INVADE_BOSS_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::RANK_INVADE_BOSS_CONTRIBUTE;
	if (str.Compare(TEXT("CHARACTER_COMBATPOWER"), ESearchCase::IgnoreCase) == 0)	return AchievementType::CHARACTER_COMBATPOWER;
	if (str.Compare(TEXT("ITEM_MAGICALFORGE_TRY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::ITEM_MAGICALFORGE_TRY;
	if (str.Compare(TEXT("GUILD_ARENA_ENTRY"), ESearchCase::IgnoreCase) == 0)	return AchievementType::GUILD_ARENA_ENTRY;
	if (str.Compare(TEXT("RANK_GUILD_BOSS_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return AchievementType::RANK_GUILD_BOSS_CONTRIBUTE;
	return AchievementType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString AchievementTypeEnumToString(AchievementType value)
{
	switch (value)
	{
		case AchievementType::NONE: return TEXT("NONE");
		case AchievementType::CHARACTER_LEVEL: return TEXT("CHARACTER_LEVEL");
		case AchievementType::CHARACTER_EXP: return TEXT("CHARACTER_EXP");
		case AchievementType::DAMAGE_ONE_TIME: return TEXT("DAMAGE_ONE_TIME");
		case AchievementType::COSTUME_SUMMON: return TEXT("COSTUME_SUMMON");
		case AchievementType::COSTUME_COMPOSE: return TEXT("COSTUME_COMPOSE");
		case AchievementType::COSTUME_COMPOSE_GET_GREATER_GRADE: return TEXT("COSTUME_COMPOSE_GET_GREATER_GRADE");
		case AchievementType::FAIRY_SUMMON: return TEXT("FAIRY_SUMMON");
		case AchievementType::FAIRY_COMPOSE: return TEXT("FAIRY_COMPOSE");
		case AchievementType::FAIRY_COMPOSE_GET_GREATER_GRADE: return TEXT("FAIRY_COMPOSE_GET_GREATER_GRADE");
		case AchievementType::FAIRY_GET_LEVEL: return TEXT("FAIRY_GET_LEVEL");
		case AchievementType::ITEM_ENCHANT_TRY: return TEXT("ITEM_ENCHANT_TRY");
		case AchievementType::ITEM_ENCHANT_SUCCESS_GREATER_NORMAL: return TEXT("ITEM_ENCHANT_SUCCESS_GREATER_NORMAL");
		case AchievementType::ITEM_REFINE_TRY: return TEXT("ITEM_REFINE_TRY");
		case AchievementType::ITEM_DECOMPOSE_TRY: return TEXT("ITEM_DECOMPOSE_TRY");
		case AchievementType::ITEM_CRAFT_RESULT_GREATER_GRADE: return TEXT("ITEM_CRAFT_RESULT_GREATER_GRADE");
		case AchievementType::QUEST_COMPLETE_REPEAT_GRADE: return TEXT("QUEST_COMPLETE_REPEAT_GRADE");
		case AchievementType::DEATH_BY_MONSTER: return TEXT("DEATH_BY_MONSTER");
		case AchievementType::DEATH_BY_PLAYER: return TEXT("DEATH_BY_PLAYER");
		case AchievementType::RESURRECT_INSTANT_USE: return TEXT("RESURRECT_INSTANT_USE");
		case AchievementType::KILL_PLAYER: return TEXT("KILL_PLAYER");
		case AchievementType::KILL_PLAYER_INTER_SERVER: return TEXT("KILL_PLAYER_INTER_SERVER");
		case AchievementType::KILL_MONSTER_BY_GRADE: return TEXT("KILL_MONSTER_BY_GRADE");
		case AchievementType::KILL_MONSTER_BY_ID: return TEXT("KILL_MONSTER_BY_ID");
		case AchievementType::RANK_FIELD_BOSS_CONTRIBUTE: return TEXT("RANK_FIELD_BOSS_CONTRIBUTE");
		case AchievementType::RANK_RAID_BOSS_CONTRIBUTE: return TEXT("RANK_RAID_BOSS_CONTRIBUTE");
		case AchievementType::SPACE_CRACK_CLEAR: return TEXT("SPACE_CRACK_CLEAR");
		case AchievementType::SPACE_CRACK_KILL_MONSTER: return TEXT("SPACE_CRACK_KILL_MONSTER");
		case AchievementType::FRIEND_MAKE: return TEXT("FRIEND_MAKE");
		case AchievementType::MARKET_SELL: return TEXT("MARKET_SELL");
		case AchievementType::MARKET_BUY: return TEXT("MARKET_BUY");
		case AchievementType::GUILD_CONTRIBUTE: return TEXT("GUILD_CONTRIBUTE");
		case AchievementType::GUILD_ITEM_COLLECTION_REGISTER: return TEXT("GUILD_ITEM_COLLECTION_REGISTER");
		case AchievementType::LOGIN_ACCUMULATE: return TEXT("LOGIN_ACCUMULATE");
		case AchievementType::CURRENCY_ACCUMULATE_CONSUME: return TEXT("CURRENCY_ACCUMULATE_CONSUME");
		case AchievementType::CURRENCY_ACCUMULATE_GET: return TEXT("CURRENCY_ACCUMULATE_GET");
		case AchievementType::ITEM_ACCUMULATE_USE: return TEXT("ITEM_ACCUMULATE_USE");
		case AchievementType::ITEM_LOOT_GRADE_CATEGORY: return TEXT("ITEM_LOOT_GRADE_CATEGORY");
		case AchievementType::QUEST_COMPLETE_TYPE: return TEXT("QUEST_COMPLETE_TYPE");
		case AchievementType::SKILL_ENCHANT_TRY: return TEXT("SKILL_ENCHANT_TRY");
		case AchievementType::SKILL_UPGRADE_TRY: return TEXT("SKILL_UPGRADE_TRY");
		case AchievementType::ITEM_LOOT_GRADE_TYPE: return TEXT("ITEM_LOOT_GRADE_TYPE");
		case AchievementType::NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB: return TEXT("NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB");
		case AchievementType::NPC_SHOP_BUY_ITEM_ITEM_ID: return TEXT("NPC_SHOP_BUY_ITEM_ITEM_ID");
		case AchievementType::ITEM_ACCUMULATE_USE_CATEGORY_MAIN_SUB: return TEXT("ITEM_ACCUMULATE_USE_CATEGORY_MAIN_SUB");
		case AchievementType::COSTUME_COMPOSE_RESULT_SPECIFIC_GRADE: return TEXT("COSTUME_COMPOSE_RESULT_SPECIFIC_GRADE");
		case AchievementType::FAIRY_COMPOSE_RESULT_SPECIFIC_GRADE: return TEXT("FAIRY_COMPOSE_RESULT_SPECIFIC_GRADE");
		case AchievementType::ITEM_COLLECTION_COMPLETE: return TEXT("ITEM_COLLECTION_COMPLETE");
		case AchievementType::ITEM_CRAFT_RESULT: return TEXT("ITEM_CRAFT_RESULT");
		case AchievementType::SKILL_ENCHANT_RESULT: return TEXT("SKILL_ENCHANT_RESULT");
		case AchievementType::GUILD_CAMP_DESTROY: return TEXT("GUILD_CAMP_DESTROY");
		case AchievementType::COSTUME_COMPOSE_TRY_SPECIFIC_GRADE: return TEXT("COSTUME_COMPOSE_TRY_SPECIFIC_GRADE");
		case AchievementType::FAIRY_COMPOSE_TRY_SPECIFIC_GRADE: return TEXT("FAIRY_COMPOSE_TRY_SPECIFIC_GRADE");
		case AchievementType::ITEM_CRAFT_TRY_ITEM_TYPE: return TEXT("ITEM_CRAFT_TRY_ITEM_TYPE");
		case AchievementType::RANK_SANCTUM_CONTRIBUTE: return TEXT("RANK_SANCTUM_CONTRIBUTE");
		case AchievementType::SEAL_NODE_REROLL: return TEXT("SEAL_NODE_REROLL");
		case AchievementType::FAIRY_ENCHANT_TRY: return TEXT("FAIRY_ENCHANT_TRY");
		case AchievementType::SPIRITSHOT_ENCHANT_TRY: return TEXT("SPIRITSHOT_ENCHANT_TRY");
		case AchievementType::PARTY_DUNGEON_CLEAR: return TEXT("PARTY_DUNGEON_CLEAR");
		case AchievementType::GUILD_DUNGEON_CLEAR: return TEXT("GUILD_DUNGEON_CLEAR");
		case AchievementType::RANK_INVADE_WAGON_CONTRIBUTE: return TEXT("RANK_INVADE_WAGON_CONTRIBUTE");
		case AchievementType::RANK_INVADE_GUARDIAN_CONTRIBUTE: return TEXT("RANK_INVADE_GUARDIAN_CONTRIBUTE");
		case AchievementType::RANK_INVADE_BOSS_CONTRIBUTE: return TEXT("RANK_INVADE_BOSS_CONTRIBUTE");
		case AchievementType::CHARACTER_COMBATPOWER: return TEXT("CHARACTER_COMBATPOWER");
		case AchievementType::ITEM_MAGICALFORGE_TRY: return TEXT("ITEM_MAGICALFORGE_TRY");
		case AchievementType::GUILD_ARENA_ENTRY: return TEXT("GUILD_ARENA_ENTRY");
		case AchievementType::RANK_GUILD_BOSS_CONTRIBUTE: return TEXT("RANK_GUILD_BOSS_CONTRIBUTE");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString AchievementTypeEnumToDisplayKey(AchievementType value)
{
	switch (value)
	{
		case AchievementType::NONE: return TEXT("");
		case AchievementType::CHARACTER_LEVEL: return TEXT("");
		case AchievementType::CHARACTER_EXP: return TEXT("");
		case AchievementType::DAMAGE_ONE_TIME: return TEXT("");
		case AchievementType::COSTUME_SUMMON: return TEXT("");
		case AchievementType::COSTUME_COMPOSE: return TEXT("");
		case AchievementType::COSTUME_COMPOSE_GET_GREATER_GRADE: return TEXT("");
		case AchievementType::FAIRY_SUMMON: return TEXT("");
		case AchievementType::FAIRY_COMPOSE: return TEXT("");
		case AchievementType::FAIRY_COMPOSE_GET_GREATER_GRADE: return TEXT("");
		case AchievementType::FAIRY_GET_LEVEL: return TEXT("");
		case AchievementType::ITEM_ENCHANT_TRY: return TEXT("");
		case AchievementType::ITEM_ENCHANT_SUCCESS_GREATER_NORMAL: return TEXT("");
		case AchievementType::ITEM_REFINE_TRY: return TEXT("");
		case AchievementType::ITEM_DECOMPOSE_TRY: return TEXT("");
		case AchievementType::ITEM_CRAFT_RESULT_GREATER_GRADE: return TEXT("");
		case AchievementType::QUEST_COMPLETE_REPEAT_GRADE: return TEXT("");
		case AchievementType::DEATH_BY_MONSTER: return TEXT("");
		case AchievementType::DEATH_BY_PLAYER: return TEXT("");
		case AchievementType::RESURRECT_INSTANT_USE: return TEXT("");
		case AchievementType::KILL_PLAYER: return TEXT("");
		case AchievementType::KILL_PLAYER_INTER_SERVER: return TEXT("");
		case AchievementType::KILL_MONSTER_BY_GRADE: return TEXT("");
		case AchievementType::KILL_MONSTER_BY_ID: return TEXT("");
		case AchievementType::RANK_FIELD_BOSS_CONTRIBUTE: return TEXT("");
		case AchievementType::RANK_RAID_BOSS_CONTRIBUTE: return TEXT("");
		case AchievementType::SPACE_CRACK_CLEAR: return TEXT("");
		case AchievementType::SPACE_CRACK_KILL_MONSTER: return TEXT("");
		case AchievementType::FRIEND_MAKE: return TEXT("");
		case AchievementType::MARKET_SELL: return TEXT("");
		case AchievementType::MARKET_BUY: return TEXT("");
		case AchievementType::GUILD_CONTRIBUTE: return TEXT("");
		case AchievementType::GUILD_ITEM_COLLECTION_REGISTER: return TEXT("");
		case AchievementType::LOGIN_ACCUMULATE: return TEXT("");
		case AchievementType::CURRENCY_ACCUMULATE_CONSUME: return TEXT("");
		case AchievementType::CURRENCY_ACCUMULATE_GET: return TEXT("");
		case AchievementType::ITEM_ACCUMULATE_USE: return TEXT("");
		case AchievementType::ITEM_LOOT_GRADE_CATEGORY: return TEXT("");
		case AchievementType::QUEST_COMPLETE_TYPE: return TEXT("");
		case AchievementType::SKILL_ENCHANT_TRY: return TEXT("");
		case AchievementType::SKILL_UPGRADE_TRY: return TEXT("");
		case AchievementType::ITEM_LOOT_GRADE_TYPE: return TEXT("");
		case AchievementType::NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB: return TEXT("");
		case AchievementType::NPC_SHOP_BUY_ITEM_ITEM_ID: return TEXT("");
		case AchievementType::ITEM_ACCUMULATE_USE_CATEGORY_MAIN_SUB: return TEXT("");
		case AchievementType::COSTUME_COMPOSE_RESULT_SPECIFIC_GRADE: return TEXT("");
		case AchievementType::FAIRY_COMPOSE_RESULT_SPECIFIC_GRADE: return TEXT("");
		case AchievementType::ITEM_COLLECTION_COMPLETE: return TEXT("");
		case AchievementType::ITEM_CRAFT_RESULT: return TEXT("");
		case AchievementType::SKILL_ENCHANT_RESULT: return TEXT("");
		case AchievementType::GUILD_CAMP_DESTROY: return TEXT("");
		case AchievementType::COSTUME_COMPOSE_TRY_SPECIFIC_GRADE: return TEXT("");
		case AchievementType::FAIRY_COMPOSE_TRY_SPECIFIC_GRADE: return TEXT("");
		case AchievementType::ITEM_CRAFT_TRY_ITEM_TYPE: return TEXT("");
		case AchievementType::RANK_SANCTUM_CONTRIBUTE: return TEXT("");
		case AchievementType::SEAL_NODE_REROLL: return TEXT("");
		case AchievementType::FAIRY_ENCHANT_TRY: return TEXT("");
		case AchievementType::SPIRITSHOT_ENCHANT_TRY: return TEXT("");
		case AchievementType::PARTY_DUNGEON_CLEAR: return TEXT("");
		case AchievementType::GUILD_DUNGEON_CLEAR: return TEXT("");
		case AchievementType::RANK_INVADE_WAGON_CONTRIBUTE: return TEXT("");
		case AchievementType::RANK_INVADE_GUARDIAN_CONTRIBUTE: return TEXT("");
		case AchievementType::RANK_INVADE_BOSS_CONTRIBUTE: return TEXT("");
		case AchievementType::CHARACTER_COMBATPOWER: return TEXT("");
		case AchievementType::ITEM_MAGICALFORGE_TRY: return TEXT("");
		case AchievementType::GUILD_ARENA_ENTRY: return TEXT("");
		case AchievementType::RANK_GUILD_BOSS_CONTRIBUTE: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidAchievementType(AchievementType value) noexcept
{
	return (value >= AchievementType::NONE && value < AchievementType::MAX);
}


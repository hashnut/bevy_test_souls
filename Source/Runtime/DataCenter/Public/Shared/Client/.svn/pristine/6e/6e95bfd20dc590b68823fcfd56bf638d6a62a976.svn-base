#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedQuestEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 반복 퀘스트 스토리 타입
enum class RepeatStoryType : uint8
{

	// 의뢰게시판 타입
	BOARD = 0,

	// 스크롤 타입
	SCROLL = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 반복 퀘스트스토리 등급
enum class RepeatStoryGrade : uint8
{

	// 노멀
	NORMAL = 0,

	// 매직
	MAGIC = 1,

	// 레어
	RARE = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 퀘스트 타입(대분류)
enum class QuestType : uint8
{

	// 메인 퀘스트
	MAIN = 0,

	// 서브 퀘스트
	SUB = 1,

	// 반복 퀘스트
	REPEAT = 2,

	// 다단계 랭크 퀘스트
	MULTI_LEVEL_RANK = 3,

	// 가이드 퀘스트
	GUIDE = 4,

	// 기사단 퀘스트
	GUILD = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// 퀘스트 ObjectTive - 유저가 실제로 수행하는 Type : ***Quest::Mask 에 추가 필요 //sectionDesc을 뒤늦게 나눠서, 보일 때 마다 Section과 순서 맞추기
enum class QuestObjectiveType : uint8
{
	// 별도 Objective Type - 해당 Section에 타입 추가시 클라이언트 확인이 필요하다.

	// 사냥
	KILL = 0,

	// 이동
	GOTO_POS = 1,

	// Npc 대화
	INTERACT_NPC = 2,

	// PROP TOUCH
	INTERACT_PROP_TOUCH = 3,

	// 인스턴스던전으로 들어가기
	WARP_TO_INSTANCE_FIELD = 4,

	// 인스턴스던전에서 나오기(들어가기전 공용맵으로 이동)
	WARP_TO_LAST_PUBLIC_FIELD = 5,

	// 특정 타입 몬스터 처치
	KILL_CREATURE_TYPE = 6,

	// 아이템 제작
	ITEM_CRAFT = 7,

	// 특정 타입 퀘스트 완료
	COMPLETE_QUEST_TYPE = 8,

	// 특정 타입 스토리 완료
	COMPLETE_STORY_TYPE = 9,

	// 시공의 틈새 몬스터 처치
	KILL_SPACE_CRACK = 10,

	// 위치이동 - 동일한 맵에서 위치(Spot)만 이동
	TELEPORT = 11,

	// Common Objective Type - 해당 Section이후에 추가시 공통 타입 Objective이므로 클라이언트 작업이 별도로 필요없다.

	// SHOP

	// NPC 상점에서 아이템 구매 : ItemId(0=anyItem), count
	BUY_SHOP_ITEM = 12,

	// NPC 상점에서 아이템 구매 : CategoryMain, count
	BUY_SHOP_ITEM_BY_CATEGORY_MAIN = 13,

	// BM_SHOP

	// BM 상점 상품 구매 : ProductId, count
	BM_SHOP_BUY_PRODUCT = 14,

	// MARKET

	// 거래소 아이템 등록 : -, count
	MARKET_REGISTER = 15,

	// 거래소 아이템 구매 : -, count
	MARKET_PURCHASE = 16,

	// ITEM

	// 장비 아이템 장착 : ItemId(0=anyItem), count
	EQUIP_ITEM = 17,

	// 장비 아이템 강화 : ItemId(0=anyItem), count
	ENCHANT_ITEM_BY_ITEM_ID = 18,

	// 장비 아이템 강화 : ItemType(NONE-anyItem), count
	ENCHANT_ITEM_BY_ITEM_TYPE = 19,

	// 모든 장비 강화 성공 회수 : - count
	ENCHANT_ITEM_SUCCESS = 20,

	// 특정 아이템 사용 : ItemId(0=anyItem), count
	USE_CONSUME_ITEM_ID = 21,

	// 특정 아이템Type 사용 : ItemType(NONE-anyItem), count
	USE_CONSUME_ITEM_TYPE = 22,

	// 특정 아이템ItemCategoryMain 사용 : ItemCategoryMain(NONE-anyItem), count
	USE_CONSUME_ITEM_CATEGORY_MAIN = 23,

	// 장비 아이템 제련 : ItemCategoryMain(NONE-anyItem), count
	ITEM_REFINE_BY_ITEM_CATEGORY_MAIN = 24,

	// ITEM STORAGE

	// 창고에 아이템 보관 : ItemId(0=anyItem), count
	ITEM_INVENTORY_TO_DEPOT = 25,

	// FAIRY

	// 페어리 소환 : - , count
	SUMMON_FAIRY = 26,

	// 페어리 합성 : - , count
	COMPOSE_FAIRY = 27,

	// 페어리 강화 : Level , count
	FAIRY_LEVEL_COUNT = 28,

	// COSTUME

	// 코스튬 소환 : - , count
	SUMMON_COSTUME = 29,

	// 코스튬 합성 : - , count
	COMPOSE_COSTUME = 30,

	// MAP

	// 특정 맵 진입 : MapId, count
	ENTER_MAP_BY_MAP_ID = 31,

	// 특정 맵 진입 : MapType, count
	ENTER_MAP_BY_MAP_TYPE = 32,

	// 특정 맵 진입 : MapContentsType, count
	ENTER_MAP_BY_MAP_CONTENTS_TYPE = 33,

	// SKILL

	// 스킬 강화 : SkillId(0=anyItem), count
	ENCHANT_SKILL_BY_SKILL_ID = 34,

	// -

	// 기억 장소 등록 : - , count
	ADD_MAP_BOOKMARK = 35,

	// 레이더 스캔 시스템 사용(npc탐색) : - , count
	USE_RADAR_SCAN_SYSTEM = 36,

	// 사망 패널티 복구 : - , count
	RECOVERY_DEATH_PANALTY = 37,

	// 워프탈 때 이유에 의한 달성 : WarpReason, count
	TELEPORT_BY_WARP_REASON = 38,

	// 보물 몬스터 보호 성공 횟수 : -, count
	PROTECT_TREASURE_MONSTER = 39,

	// MONSTER_KNOWLEDGE

	// 도감 노드 개수 체크 : -, count
	MONSTER_KNOWLEDGE_NODE_COUNT = 40,

	// TITLE

	// 보유한 칭호 개수 체크 : -, count
	TITLE_HAVE_COUNT = 41,

	// -

	// 아이템 컬렉션 달성 개수 체크 : -, count
	ITEM_COLLECTION_COMPLETE_COUNT = 42,

	// 코스튬 컬렉션 달성 개수 체크 : -, count
	COSTUME_COLLECTION_COMPLETE_COUNT = 43,

	// 페어리 컬렉션 달성 개수 체크 : -, count
	FAIRY_COLLECTION_COMPLETE_COUNT = 44,

	// -

	// 타 유저 n회 공격하기 : -, count
	DAMAGE_OTHER_PLAYERS_COUNT = 45,

	// INVADE

	// 침공전 n회 참여하기 : -, count
	INVADE_COUNT = 46,

	// 침공전 보물상자 N회 참여하기 : -, count
	INVADE_TRREASUREBOX_PROP = 47,

	// -

	// PK n회 성공하기 : -, count
	KILL_PLAYER = 48,

	// 침공전에서 PK n회 성공하기: -, count
	KILL_PLAYER_INVADE = 49,

	// 본인 레벨보다 낮은 레벨의 유저 PK N회 성공하기: -, count
	KILL_PLAYER_WEAK = 50,

	// 본인 레벨보다 높거나 같은 레벨의 유저 PK N회 성공하기: -, count
	KILL_PLAYER_SAME_OR_STRONG = 51,

	// PK 랭킹 N등 안에 들어있는 유저 PK M회 성공하기: Rank, count
	KILL_PLAYER_RANKING_PK = 52,

	// 동일 서버 랭킹 N등 안에 들어있는 유저 PK M회 성공하기: Rank, count
	KILL_PLAYER_RANKING_LEVEL = 53,

	// 전체 서버 랭킹 N등 안에 들어있는 유저 PK M회 성공하기: Rank, count
	KILL_PLAYER_RANKING_ALLSERVERLEVEL = 54,

	// 적대 상태의 기사단에 소속한 유저 PK N회 성공하기: -, count
	KILL_PLAYER_ENEMYGUILD = 55,

	// AUTO_USE

	// 자동사용 설정 : SkillCategorySet, count
	AUTO_USE_SKILL_BY_CATEGORY_SET = 56,

	// 자동사용 설정 : 아이템인데 해당 ItemCategorySub : Category_Sub, count
	AUTO_USE_ITEM_BY_CATEGORY_SUB = 57,

	// MULTI_LEVEL_RANK

	// 달성한 승급의 '단계'에 대해 체크하는 조건(MultiLevelRankCommon.json의 rank를 가지고 비교): MultiLevelRank, count
	MULTI_LEVEL_RANK_RANK = 58,

	// GUILD

	// 길드 가입 : - , count
	JOIN_GUILD = 59,

	// Guild Dungeon Clear : -, count
	GUILD_DUNGEON_CLEAR = 60,

	// GUILD_CAMP

	// 기사단 수호탑 건설(소속 기사단에서 건설) : - , -
	GUILD_CAMP_BUILD = 61,

	// 기사단 수호탑 N회 파괴하기 (기여도만 있으면 체크) : - , -
	DESTORY_CAMP = 62,

	// 적대 상태의 기사단 수호탑 N회 파괴하기 (기여도만 있으면 체크) : - , -
	DESTORY_CAMP_ENEMYGUILD = 63,

	// PARTY

	// 파티멤버 수 : -, count
	PARTY_JOIN = 64,

	// FRIEND

	// 친구 신청 횟수 : -, count
	FRIEND_REQUEST = 65,

	// PROFILE

	// 프로필 - 다른 유저 프로필에 한마디 남기기 입력 : -, count
	PROFILE_OTHER_USER_COMMENT_INPUT = 66,

	// OFFLINE

	// 오프라인 플레이 누적시간 : -, hour
	OFFLINE_PLAY_TIME_ACCUMULATE = 67,

	// MapEvent

	// MapEventId 에서 호출 : MapEventId, -
	COMPLETE_MAP_EVENT = 68,

	// Dungeon

	// 특정 파티 던전 N회 클리어하기 : DungeonGroupId, -
	PARTY_DUNGEON_CLEAR_GROUPID = 69,

	// 특정 던전 그룹 Id에 설정된 dungeonList에 포함된 맵에서 몬스터 처치하기 : DungeonGroupId, -
	KILL_CREATURE_DUNGEONGROUPID = 70,

	MAX = 71,
};

UENUM()
// 퀘스트 상태
enum class QuestState : uint8
{
	// DB에서 사용 중 :	// t_quest_main.c_quest_state
	// t_quest_sub.c_quest_state
	// t_quest_sub.c_quest_state
	// t_quest_multi_level_rank.c_quest_state	// t_quest_guild_knight.c_quest_state	// t_quest_guide.c_quest_state 


	// 수락 전 상태, 클라이언트에서는 Story 시작 연출용도로 사용됨
	NONE = 0,

	// 수락 전 상태, preEventAction 진행을 위해 서버에서 다음퀘스트로 보낸 후 사용 됨
	PRE_ACCEPT = 1,

	// 진행중 (수락완료)
	ACCEPTED = 2,

	// 완료 (보상 지급 전)
	COMPLETED = 3,

	// 완료 (보상 지급 완료)
	REWARDED = 4,

	// 삭제됨, 기사단 퀘스트 기간 만료 시 현재 진행중 퀘스트 삭제 처리
	DELETED = 5,

	// 퀘스트 포기 (HIVE 로그에서만 사용)
	GIVEUP = 6,

	MAX = 7,
};

UENUM()
// 퀘스트 ObjectTive 상태
enum class QuestObjectiveState : uint8
{

	// 진행 중
	IN_PROGRESS = 0,

	// 완료
	COMPLETE = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 퀘스트 ObjectTive 처리 순서
enum class QuestObjectiveProcessType : uint8
{

	// 병렬 - objective 3개 동시 처리
	PARALLER = 0,

	// 순차 - obejctive 앞부터 처리
	ORDER = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 기사단 개별 스토리(퀘스트) 등급
enum class GuildQuestGrade : uint8
{

	// 노멀
	NORMAL = 0,

	// 고급
	MAGIC = 1,

	// 희귀
	RARE = 2,

	// 에픽
	EPIC = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 퀘스트 게시판 타입
enum class QuestBoardType : uint8
{

	// 없음
	NONE = 0,

	// 마을 퀘스트 게시판
	VILLAGE = 1,

	// 침공 퀘스트 게시판
	INVADE = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 퀘스트 컨텐츠 타입
enum class QuestContentsType : uint8
{

	// 퀘스트 컨텐츠 타입 - 일반,침공 모두
	ALL = 0,

	// 퀘스트 컨텐츠 타입 - 일반
	NORMAL = 1,

	// 퀘스트 컨텐츠 타입 - 침공
	INVADE = 2,

	MAX = 3,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline RepeatStoryType RepeatStoryTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("BOARD"), ESearchCase::IgnoreCase) == 0)	return RepeatStoryType::BOARD;
	if (str.Compare(TEXT("SCROLL"), ESearchCase::IgnoreCase) == 0)	return RepeatStoryType::SCROLL;
	return RepeatStoryType::MAX;
}
inline RepeatStoryGrade RepeatStoryGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return RepeatStoryGrade::NORMAL;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return RepeatStoryGrade::MAGIC;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return RepeatStoryGrade::RARE;
	return RepeatStoryGrade::MAX;
}
inline QuestType QuestTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("MAIN"), ESearchCase::IgnoreCase) == 0)	return QuestType::MAIN;
	if (str.Compare(TEXT("SUB"), ESearchCase::IgnoreCase) == 0)	return QuestType::SUB;
	if (str.Compare(TEXT("REPEAT"), ESearchCase::IgnoreCase) == 0)	return QuestType::REPEAT;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK"), ESearchCase::IgnoreCase) == 0)	return QuestType::MULTI_LEVEL_RANK;
	if (str.Compare(TEXT("GUIDE"), ESearchCase::IgnoreCase) == 0)	return QuestType::GUIDE;
	if (str.Compare(TEXT("GUILD"), ESearchCase::IgnoreCase) == 0)	return QuestType::GUILD;
	return QuestType::MAX;
}
inline QuestObjectiveType QuestObjectiveTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("KILL"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL;
	if (str.Compare(TEXT("GOTO_POS"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::GOTO_POS;
	if (str.Compare(TEXT("INTERACT_NPC"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::INTERACT_NPC;
	if (str.Compare(TEXT("INTERACT_PROP_TOUCH"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::INTERACT_PROP_TOUCH;
	if (str.Compare(TEXT("WARP_TO_INSTANCE_FIELD"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::WARP_TO_INSTANCE_FIELD;
	if (str.Compare(TEXT("WARP_TO_LAST_PUBLIC_FIELD"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::WARP_TO_LAST_PUBLIC_FIELD;
	if (str.Compare(TEXT("KILL_CREATURE_TYPE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_CREATURE_TYPE;
	if (str.Compare(TEXT("ITEM_CRAFT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ITEM_CRAFT;
	if (str.Compare(TEXT("COMPLETE_QUEST_TYPE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::COMPLETE_QUEST_TYPE;
	if (str.Compare(TEXT("COMPLETE_STORY_TYPE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::COMPLETE_STORY_TYPE;
	if (str.Compare(TEXT("KILL_SPACE_CRACK"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_SPACE_CRACK;
	if (str.Compare(TEXT("TELEPORT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::TELEPORT;
	if (str.Compare(TEXT("BUY_SHOP_ITEM"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::BUY_SHOP_ITEM;
	if (str.Compare(TEXT("BUY_SHOP_ITEM_BY_CATEGORY_MAIN"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::BUY_SHOP_ITEM_BY_CATEGORY_MAIN;
	if (str.Compare(TEXT("BM_SHOP_BUY_PRODUCT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::BM_SHOP_BUY_PRODUCT;
	if (str.Compare(TEXT("MARKET_REGISTER"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::MARKET_REGISTER;
	if (str.Compare(TEXT("MARKET_PURCHASE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::MARKET_PURCHASE;
	if (str.Compare(TEXT("EQUIP_ITEM"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::EQUIP_ITEM;
	if (str.Compare(TEXT("ENCHANT_ITEM_BY_ITEM_ID"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ENCHANT_ITEM_BY_ITEM_ID;
	if (str.Compare(TEXT("ENCHANT_ITEM_BY_ITEM_TYPE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ENCHANT_ITEM_BY_ITEM_TYPE;
	if (str.Compare(TEXT("ENCHANT_ITEM_SUCCESS"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ENCHANT_ITEM_SUCCESS;
	if (str.Compare(TEXT("USE_CONSUME_ITEM_ID"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::USE_CONSUME_ITEM_ID;
	if (str.Compare(TEXT("USE_CONSUME_ITEM_TYPE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::USE_CONSUME_ITEM_TYPE;
	if (str.Compare(TEXT("USE_CONSUME_ITEM_CATEGORY_MAIN"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::USE_CONSUME_ITEM_CATEGORY_MAIN;
	if (str.Compare(TEXT("ITEM_REFINE_BY_ITEM_CATEGORY_MAIN"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ITEM_REFINE_BY_ITEM_CATEGORY_MAIN;
	if (str.Compare(TEXT("ITEM_INVENTORY_TO_DEPOT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ITEM_INVENTORY_TO_DEPOT;
	if (str.Compare(TEXT("SUMMON_FAIRY"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::SUMMON_FAIRY;
	if (str.Compare(TEXT("COMPOSE_FAIRY"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::COMPOSE_FAIRY;
	if (str.Compare(TEXT("FAIRY_LEVEL_COUNT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::FAIRY_LEVEL_COUNT;
	if (str.Compare(TEXT("SUMMON_COSTUME"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::SUMMON_COSTUME;
	if (str.Compare(TEXT("COMPOSE_COSTUME"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::COMPOSE_COSTUME;
	if (str.Compare(TEXT("ENTER_MAP_BY_MAP_ID"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ENTER_MAP_BY_MAP_ID;
	if (str.Compare(TEXT("ENTER_MAP_BY_MAP_TYPE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ENTER_MAP_BY_MAP_TYPE;
	if (str.Compare(TEXT("ENTER_MAP_BY_MAP_CONTENTS_TYPE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ENTER_MAP_BY_MAP_CONTENTS_TYPE;
	if (str.Compare(TEXT("ENCHANT_SKILL_BY_SKILL_ID"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ENCHANT_SKILL_BY_SKILL_ID;
	if (str.Compare(TEXT("ADD_MAP_BOOKMARK"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ADD_MAP_BOOKMARK;
	if (str.Compare(TEXT("USE_RADAR_SCAN_SYSTEM"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::USE_RADAR_SCAN_SYSTEM;
	if (str.Compare(TEXT("RECOVERY_DEATH_PANALTY"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::RECOVERY_DEATH_PANALTY;
	if (str.Compare(TEXT("TELEPORT_BY_WARP_REASON"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::TELEPORT_BY_WARP_REASON;
	if (str.Compare(TEXT("PROTECT_TREASURE_MONSTER"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::PROTECT_TREASURE_MONSTER;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_NODE_COUNT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::MONSTER_KNOWLEDGE_NODE_COUNT;
	if (str.Compare(TEXT("TITLE_HAVE_COUNT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::TITLE_HAVE_COUNT;
	if (str.Compare(TEXT("ITEM_COLLECTION_COMPLETE_COUNT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::ITEM_COLLECTION_COMPLETE_COUNT;
	if (str.Compare(TEXT("COSTUME_COLLECTION_COMPLETE_COUNT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::COSTUME_COLLECTION_COMPLETE_COUNT;
	if (str.Compare(TEXT("FAIRY_COLLECTION_COMPLETE_COUNT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::FAIRY_COLLECTION_COMPLETE_COUNT;
	if (str.Compare(TEXT("DAMAGE_OTHER_PLAYERS_COUNT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::DAMAGE_OTHER_PLAYERS_COUNT;
	if (str.Compare(TEXT("INVADE_COUNT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::INVADE_COUNT;
	if (str.Compare(TEXT("INVADE_TRREASUREBOX_PROP"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::INVADE_TRREASUREBOX_PROP;
	if (str.Compare(TEXT("KILL_PLAYER"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_PLAYER;
	if (str.Compare(TEXT("KILL_PLAYER_INVADE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_PLAYER_INVADE;
	if (str.Compare(TEXT("KILL_PLAYER_WEAK"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_PLAYER_WEAK;
	if (str.Compare(TEXT("KILL_PLAYER_SAME_OR_STRONG"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_PLAYER_SAME_OR_STRONG;
	if (str.Compare(TEXT("KILL_PLAYER_RANKING_PK"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_PLAYER_RANKING_PK;
	if (str.Compare(TEXT("KILL_PLAYER_RANKING_LEVEL"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_PLAYER_RANKING_LEVEL;
	if (str.Compare(TEXT("KILL_PLAYER_RANKING_ALLSERVERLEVEL"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_PLAYER_RANKING_ALLSERVERLEVEL;
	if (str.Compare(TEXT("KILL_PLAYER_ENEMYGUILD"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_PLAYER_ENEMYGUILD;
	if (str.Compare(TEXT("AUTO_USE_SKILL_BY_CATEGORY_SET"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::AUTO_USE_SKILL_BY_CATEGORY_SET;
	if (str.Compare(TEXT("AUTO_USE_ITEM_BY_CATEGORY_SUB"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::AUTO_USE_ITEM_BY_CATEGORY_SUB;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_RANK"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::MULTI_LEVEL_RANK_RANK;
	if (str.Compare(TEXT("JOIN_GUILD"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::JOIN_GUILD;
	if (str.Compare(TEXT("GUILD_DUNGEON_CLEAR"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::GUILD_DUNGEON_CLEAR;
	if (str.Compare(TEXT("GUILD_CAMP_BUILD"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::GUILD_CAMP_BUILD;
	if (str.Compare(TEXT("DESTORY_CAMP"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::DESTORY_CAMP;
	if (str.Compare(TEXT("DESTORY_CAMP_ENEMYGUILD"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::DESTORY_CAMP_ENEMYGUILD;
	if (str.Compare(TEXT("PARTY_JOIN"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::PARTY_JOIN;
	if (str.Compare(TEXT("FRIEND_REQUEST"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::FRIEND_REQUEST;
	if (str.Compare(TEXT("PROFILE_OTHER_USER_COMMENT_INPUT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::PROFILE_OTHER_USER_COMMENT_INPUT;
	if (str.Compare(TEXT("OFFLINE_PLAY_TIME_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::OFFLINE_PLAY_TIME_ACCUMULATE;
	if (str.Compare(TEXT("COMPLETE_MAP_EVENT"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::COMPLETE_MAP_EVENT;
	if (str.Compare(TEXT("PARTY_DUNGEON_CLEAR_GROUPID"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::PARTY_DUNGEON_CLEAR_GROUPID;
	if (str.Compare(TEXT("KILL_CREATURE_DUNGEONGROUPID"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveType::KILL_CREATURE_DUNGEONGROUPID;
	return QuestObjectiveType::MAX;
}
inline QuestState QuestStateStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return QuestState::NONE;
	if (str.Compare(TEXT("PRE_ACCEPT"), ESearchCase::IgnoreCase) == 0)	return QuestState::PRE_ACCEPT;
	if (str.Compare(TEXT("ACCEPTED"), ESearchCase::IgnoreCase) == 0)	return QuestState::ACCEPTED;
	if (str.Compare(TEXT("COMPLETED"), ESearchCase::IgnoreCase) == 0)	return QuestState::COMPLETED;
	if (str.Compare(TEXT("REWARDED"), ESearchCase::IgnoreCase) == 0)	return QuestState::REWARDED;
	if (str.Compare(TEXT("DELETED"), ESearchCase::IgnoreCase) == 0)	return QuestState::DELETED;
	if (str.Compare(TEXT("GIVEUP"), ESearchCase::IgnoreCase) == 0)	return QuestState::GIVEUP;
	return QuestState::MAX;
}
inline QuestObjectiveState QuestObjectiveStateStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("IN_PROGRESS"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveState::IN_PROGRESS;
	if (str.Compare(TEXT("COMPLETE"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveState::COMPLETE;
	return QuestObjectiveState::MAX;
}
inline QuestObjectiveProcessType QuestObjectiveProcessTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("PARALLER"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveProcessType::PARALLER;
	if (str.Compare(TEXT("ORDER"), ESearchCase::IgnoreCase) == 0)	return QuestObjectiveProcessType::ORDER;
	return QuestObjectiveProcessType::MAX;
}
inline GuildQuestGrade GuildQuestGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return GuildQuestGrade::NORMAL;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return GuildQuestGrade::MAGIC;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return GuildQuestGrade::RARE;
	if (str.Compare(TEXT("EPIC"), ESearchCase::IgnoreCase) == 0)	return GuildQuestGrade::EPIC;
	return GuildQuestGrade::MAX;
}
inline QuestBoardType QuestBoardTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return QuestBoardType::NONE;
	if (str.Compare(TEXT("VILLAGE"), ESearchCase::IgnoreCase) == 0)	return QuestBoardType::VILLAGE;
	if (str.Compare(TEXT("INVADE"), ESearchCase::IgnoreCase) == 0)	return QuestBoardType::INVADE;
	return QuestBoardType::MAX;
}
inline QuestContentsType QuestContentsTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ALL"), ESearchCase::IgnoreCase) == 0)	return QuestContentsType::ALL;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return QuestContentsType::NORMAL;
	if (str.Compare(TEXT("INVADE"), ESearchCase::IgnoreCase) == 0)	return QuestContentsType::INVADE;
	return QuestContentsType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString RepeatStoryTypeEnumToString(RepeatStoryType value)
{
	switch (value)
	{
		case RepeatStoryType::BOARD: return TEXT("BOARD");
		case RepeatStoryType::SCROLL: return TEXT("SCROLL");
	}
	return FString();
}
inline FString RepeatStoryGradeEnumToString(RepeatStoryGrade value)
{
	switch (value)
	{
		case RepeatStoryGrade::NORMAL: return TEXT("NORMAL");
		case RepeatStoryGrade::MAGIC: return TEXT("MAGIC");
		case RepeatStoryGrade::RARE: return TEXT("RARE");
	}
	return FString();
}
inline FString QuestTypeEnumToString(QuestType value)
{
	switch (value)
	{
		case QuestType::MAIN: return TEXT("MAIN");
		case QuestType::SUB: return TEXT("SUB");
		case QuestType::REPEAT: return TEXT("REPEAT");
		case QuestType::MULTI_LEVEL_RANK: return TEXT("MULTI_LEVEL_RANK");
		case QuestType::GUIDE: return TEXT("GUIDE");
		case QuestType::GUILD: return TEXT("GUILD");
	}
	return FString();
}
inline FString QuestObjectiveTypeEnumToString(QuestObjectiveType value)
{
	switch (value)
	{
		case QuestObjectiveType::KILL: return TEXT("KILL");
		case QuestObjectiveType::GOTO_POS: return TEXT("GOTO_POS");
		case QuestObjectiveType::INTERACT_NPC: return TEXT("INTERACT_NPC");
		case QuestObjectiveType::INTERACT_PROP_TOUCH: return TEXT("INTERACT_PROP_TOUCH");
		case QuestObjectiveType::WARP_TO_INSTANCE_FIELD: return TEXT("WARP_TO_INSTANCE_FIELD");
		case QuestObjectiveType::WARP_TO_LAST_PUBLIC_FIELD: return TEXT("WARP_TO_LAST_PUBLIC_FIELD");
		case QuestObjectiveType::KILL_CREATURE_TYPE: return TEXT("KILL_CREATURE_TYPE");
		case QuestObjectiveType::ITEM_CRAFT: return TEXT("ITEM_CRAFT");
		case QuestObjectiveType::COMPLETE_QUEST_TYPE: return TEXT("COMPLETE_QUEST_TYPE");
		case QuestObjectiveType::COMPLETE_STORY_TYPE: return TEXT("COMPLETE_STORY_TYPE");
		case QuestObjectiveType::KILL_SPACE_CRACK: return TEXT("KILL_SPACE_CRACK");
		case QuestObjectiveType::TELEPORT: return TEXT("TELEPORT");
		case QuestObjectiveType::BUY_SHOP_ITEM: return TEXT("BUY_SHOP_ITEM");
		case QuestObjectiveType::BUY_SHOP_ITEM_BY_CATEGORY_MAIN: return TEXT("BUY_SHOP_ITEM_BY_CATEGORY_MAIN");
		case QuestObjectiveType::BM_SHOP_BUY_PRODUCT: return TEXT("BM_SHOP_BUY_PRODUCT");
		case QuestObjectiveType::MARKET_REGISTER: return TEXT("MARKET_REGISTER");
		case QuestObjectiveType::MARKET_PURCHASE: return TEXT("MARKET_PURCHASE");
		case QuestObjectiveType::EQUIP_ITEM: return TEXT("EQUIP_ITEM");
		case QuestObjectiveType::ENCHANT_ITEM_BY_ITEM_ID: return TEXT("ENCHANT_ITEM_BY_ITEM_ID");
		case QuestObjectiveType::ENCHANT_ITEM_BY_ITEM_TYPE: return TEXT("ENCHANT_ITEM_BY_ITEM_TYPE");
		case QuestObjectiveType::ENCHANT_ITEM_SUCCESS: return TEXT("ENCHANT_ITEM_SUCCESS");
		case QuestObjectiveType::USE_CONSUME_ITEM_ID: return TEXT("USE_CONSUME_ITEM_ID");
		case QuestObjectiveType::USE_CONSUME_ITEM_TYPE: return TEXT("USE_CONSUME_ITEM_TYPE");
		case QuestObjectiveType::USE_CONSUME_ITEM_CATEGORY_MAIN: return TEXT("USE_CONSUME_ITEM_CATEGORY_MAIN");
		case QuestObjectiveType::ITEM_REFINE_BY_ITEM_CATEGORY_MAIN: return TEXT("ITEM_REFINE_BY_ITEM_CATEGORY_MAIN");
		case QuestObjectiveType::ITEM_INVENTORY_TO_DEPOT: return TEXT("ITEM_INVENTORY_TO_DEPOT");
		case QuestObjectiveType::SUMMON_FAIRY: return TEXT("SUMMON_FAIRY");
		case QuestObjectiveType::COMPOSE_FAIRY: return TEXT("COMPOSE_FAIRY");
		case QuestObjectiveType::FAIRY_LEVEL_COUNT: return TEXT("FAIRY_LEVEL_COUNT");
		case QuestObjectiveType::SUMMON_COSTUME: return TEXT("SUMMON_COSTUME");
		case QuestObjectiveType::COMPOSE_COSTUME: return TEXT("COMPOSE_COSTUME");
		case QuestObjectiveType::ENTER_MAP_BY_MAP_ID: return TEXT("ENTER_MAP_BY_MAP_ID");
		case QuestObjectiveType::ENTER_MAP_BY_MAP_TYPE: return TEXT("ENTER_MAP_BY_MAP_TYPE");
		case QuestObjectiveType::ENTER_MAP_BY_MAP_CONTENTS_TYPE: return TEXT("ENTER_MAP_BY_MAP_CONTENTS_TYPE");
		case QuestObjectiveType::ENCHANT_SKILL_BY_SKILL_ID: return TEXT("ENCHANT_SKILL_BY_SKILL_ID");
		case QuestObjectiveType::ADD_MAP_BOOKMARK: return TEXT("ADD_MAP_BOOKMARK");
		case QuestObjectiveType::USE_RADAR_SCAN_SYSTEM: return TEXT("USE_RADAR_SCAN_SYSTEM");
		case QuestObjectiveType::RECOVERY_DEATH_PANALTY: return TEXT("RECOVERY_DEATH_PANALTY");
		case QuestObjectiveType::TELEPORT_BY_WARP_REASON: return TEXT("TELEPORT_BY_WARP_REASON");
		case QuestObjectiveType::PROTECT_TREASURE_MONSTER: return TEXT("PROTECT_TREASURE_MONSTER");
		case QuestObjectiveType::MONSTER_KNOWLEDGE_NODE_COUNT: return TEXT("MONSTER_KNOWLEDGE_NODE_COUNT");
		case QuestObjectiveType::TITLE_HAVE_COUNT: return TEXT("TITLE_HAVE_COUNT");
		case QuestObjectiveType::ITEM_COLLECTION_COMPLETE_COUNT: return TEXT("ITEM_COLLECTION_COMPLETE_COUNT");
		case QuestObjectiveType::COSTUME_COLLECTION_COMPLETE_COUNT: return TEXT("COSTUME_COLLECTION_COMPLETE_COUNT");
		case QuestObjectiveType::FAIRY_COLLECTION_COMPLETE_COUNT: return TEXT("FAIRY_COLLECTION_COMPLETE_COUNT");
		case QuestObjectiveType::DAMAGE_OTHER_PLAYERS_COUNT: return TEXT("DAMAGE_OTHER_PLAYERS_COUNT");
		case QuestObjectiveType::INVADE_COUNT: return TEXT("INVADE_COUNT");
		case QuestObjectiveType::INVADE_TRREASUREBOX_PROP: return TEXT("INVADE_TRREASUREBOX_PROP");
		case QuestObjectiveType::KILL_PLAYER: return TEXT("KILL_PLAYER");
		case QuestObjectiveType::KILL_PLAYER_INVADE: return TEXT("KILL_PLAYER_INVADE");
		case QuestObjectiveType::KILL_PLAYER_WEAK: return TEXT("KILL_PLAYER_WEAK");
		case QuestObjectiveType::KILL_PLAYER_SAME_OR_STRONG: return TEXT("KILL_PLAYER_SAME_OR_STRONG");
		case QuestObjectiveType::KILL_PLAYER_RANKING_PK: return TEXT("KILL_PLAYER_RANKING_PK");
		case QuestObjectiveType::KILL_PLAYER_RANKING_LEVEL: return TEXT("KILL_PLAYER_RANKING_LEVEL");
		case QuestObjectiveType::KILL_PLAYER_RANKING_ALLSERVERLEVEL: return TEXT("KILL_PLAYER_RANKING_ALLSERVERLEVEL");
		case QuestObjectiveType::KILL_PLAYER_ENEMYGUILD: return TEXT("KILL_PLAYER_ENEMYGUILD");
		case QuestObjectiveType::AUTO_USE_SKILL_BY_CATEGORY_SET: return TEXT("AUTO_USE_SKILL_BY_CATEGORY_SET");
		case QuestObjectiveType::AUTO_USE_ITEM_BY_CATEGORY_SUB: return TEXT("AUTO_USE_ITEM_BY_CATEGORY_SUB");
		case QuestObjectiveType::MULTI_LEVEL_RANK_RANK: return TEXT("MULTI_LEVEL_RANK_RANK");
		case QuestObjectiveType::JOIN_GUILD: return TEXT("JOIN_GUILD");
		case QuestObjectiveType::GUILD_DUNGEON_CLEAR: return TEXT("GUILD_DUNGEON_CLEAR");
		case QuestObjectiveType::GUILD_CAMP_BUILD: return TEXT("GUILD_CAMP_BUILD");
		case QuestObjectiveType::DESTORY_CAMP: return TEXT("DESTORY_CAMP");
		case QuestObjectiveType::DESTORY_CAMP_ENEMYGUILD: return TEXT("DESTORY_CAMP_ENEMYGUILD");
		case QuestObjectiveType::PARTY_JOIN: return TEXT("PARTY_JOIN");
		case QuestObjectiveType::FRIEND_REQUEST: return TEXT("FRIEND_REQUEST");
		case QuestObjectiveType::PROFILE_OTHER_USER_COMMENT_INPUT: return TEXT("PROFILE_OTHER_USER_COMMENT_INPUT");
		case QuestObjectiveType::OFFLINE_PLAY_TIME_ACCUMULATE: return TEXT("OFFLINE_PLAY_TIME_ACCUMULATE");
		case QuestObjectiveType::COMPLETE_MAP_EVENT: return TEXT("COMPLETE_MAP_EVENT");
		case QuestObjectiveType::PARTY_DUNGEON_CLEAR_GROUPID: return TEXT("PARTY_DUNGEON_CLEAR_GROUPID");
		case QuestObjectiveType::KILL_CREATURE_DUNGEONGROUPID: return TEXT("KILL_CREATURE_DUNGEONGROUPID");
	}
	return FString();
}
inline FString QuestStateEnumToString(QuestState value)
{
	switch (value)
	{
		case QuestState::NONE: return TEXT("NONE");
		case QuestState::PRE_ACCEPT: return TEXT("PRE_ACCEPT");
		case QuestState::ACCEPTED: return TEXT("ACCEPTED");
		case QuestState::COMPLETED: return TEXT("COMPLETED");
		case QuestState::REWARDED: return TEXT("REWARDED");
		case QuestState::DELETED: return TEXT("DELETED");
		case QuestState::GIVEUP: return TEXT("GIVEUP");
	}
	return FString();
}
inline FString QuestObjectiveStateEnumToString(QuestObjectiveState value)
{
	switch (value)
	{
		case QuestObjectiveState::IN_PROGRESS: return TEXT("IN_PROGRESS");
		case QuestObjectiveState::COMPLETE: return TEXT("COMPLETE");
	}
	return FString();
}
inline FString QuestObjectiveProcessTypeEnumToString(QuestObjectiveProcessType value)
{
	switch (value)
	{
		case QuestObjectiveProcessType::PARALLER: return TEXT("PARALLER");
		case QuestObjectiveProcessType::ORDER: return TEXT("ORDER");
	}
	return FString();
}
inline FString GuildQuestGradeEnumToString(GuildQuestGrade value)
{
	switch (value)
	{
		case GuildQuestGrade::NORMAL: return TEXT("NORMAL");
		case GuildQuestGrade::MAGIC: return TEXT("MAGIC");
		case GuildQuestGrade::RARE: return TEXT("RARE");
		case GuildQuestGrade::EPIC: return TEXT("EPIC");
	}
	return FString();
}
inline FString QuestBoardTypeEnumToString(QuestBoardType value)
{
	switch (value)
	{
		case QuestBoardType::NONE: return TEXT("NONE");
		case QuestBoardType::VILLAGE: return TEXT("VILLAGE");
		case QuestBoardType::INVADE: return TEXT("INVADE");
	}
	return FString();
}
inline FString QuestContentsTypeEnumToString(QuestContentsType value)
{
	switch (value)
	{
		case QuestContentsType::ALL: return TEXT("ALL");
		case QuestContentsType::NORMAL: return TEXT("NORMAL");
		case QuestContentsType::INVADE: return TEXT("INVADE");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString RepeatStoryTypeEnumToDisplayKey(RepeatStoryType value)
{
	switch (value)
	{
		case RepeatStoryType::BOARD: return TEXT("");
		case RepeatStoryType::SCROLL: return TEXT("");
	}
	return FString();
}
inline FString RepeatStoryGradeEnumToDisplayKey(RepeatStoryGrade value)
{
	switch (value)
	{
		case RepeatStoryGrade::NORMAL: return TEXT("");
		case RepeatStoryGrade::MAGIC: return TEXT("");
		case RepeatStoryGrade::RARE: return TEXT("");
	}
	return FString();
}
inline FString QuestTypeEnumToDisplayKey(QuestType value)
{
	switch (value)
	{
		case QuestType::MAIN: return TEXT("");
		case QuestType::SUB: return TEXT("");
		case QuestType::REPEAT: return TEXT("");
		case QuestType::MULTI_LEVEL_RANK: return TEXT("");
		case QuestType::GUIDE: return TEXT("");
		case QuestType::GUILD: return TEXT("");
	}
	return FString();
}
inline FString QuestObjectiveTypeEnumToDisplayKey(QuestObjectiveType value)
{
	switch (value)
	{
		case QuestObjectiveType::KILL: return TEXT("");
		case QuestObjectiveType::GOTO_POS: return TEXT("");
		case QuestObjectiveType::INTERACT_NPC: return TEXT("");
		case QuestObjectiveType::INTERACT_PROP_TOUCH: return TEXT("");
		case QuestObjectiveType::WARP_TO_INSTANCE_FIELD: return TEXT("");
		case QuestObjectiveType::WARP_TO_LAST_PUBLIC_FIELD: return TEXT("");
		case QuestObjectiveType::KILL_CREATURE_TYPE: return TEXT("");
		case QuestObjectiveType::ITEM_CRAFT: return TEXT("");
		case QuestObjectiveType::COMPLETE_QUEST_TYPE: return TEXT("");
		case QuestObjectiveType::COMPLETE_STORY_TYPE: return TEXT("");
		case QuestObjectiveType::KILL_SPACE_CRACK: return TEXT("");
		case QuestObjectiveType::TELEPORT: return TEXT("");
		case QuestObjectiveType::BUY_SHOP_ITEM: return TEXT("");
		case QuestObjectiveType::BUY_SHOP_ITEM_BY_CATEGORY_MAIN: return TEXT("");
		case QuestObjectiveType::BM_SHOP_BUY_PRODUCT: return TEXT("");
		case QuestObjectiveType::MARKET_REGISTER: return TEXT("");
		case QuestObjectiveType::MARKET_PURCHASE: return TEXT("");
		case QuestObjectiveType::EQUIP_ITEM: return TEXT("");
		case QuestObjectiveType::ENCHANT_ITEM_BY_ITEM_ID: return TEXT("");
		case QuestObjectiveType::ENCHANT_ITEM_BY_ITEM_TYPE: return TEXT("");
		case QuestObjectiveType::ENCHANT_ITEM_SUCCESS: return TEXT("");
		case QuestObjectiveType::USE_CONSUME_ITEM_ID: return TEXT("");
		case QuestObjectiveType::USE_CONSUME_ITEM_TYPE: return TEXT("");
		case QuestObjectiveType::USE_CONSUME_ITEM_CATEGORY_MAIN: return TEXT("");
		case QuestObjectiveType::ITEM_REFINE_BY_ITEM_CATEGORY_MAIN: return TEXT("");
		case QuestObjectiveType::ITEM_INVENTORY_TO_DEPOT: return TEXT("");
		case QuestObjectiveType::SUMMON_FAIRY: return TEXT("");
		case QuestObjectiveType::COMPOSE_FAIRY: return TEXT("");
		case QuestObjectiveType::FAIRY_LEVEL_COUNT: return TEXT("");
		case QuestObjectiveType::SUMMON_COSTUME: return TEXT("");
		case QuestObjectiveType::COMPOSE_COSTUME: return TEXT("");
		case QuestObjectiveType::ENTER_MAP_BY_MAP_ID: return TEXT("");
		case QuestObjectiveType::ENTER_MAP_BY_MAP_TYPE: return TEXT("");
		case QuestObjectiveType::ENTER_MAP_BY_MAP_CONTENTS_TYPE: return TEXT("");
		case QuestObjectiveType::ENCHANT_SKILL_BY_SKILL_ID: return TEXT("");
		case QuestObjectiveType::ADD_MAP_BOOKMARK: return TEXT("");
		case QuestObjectiveType::USE_RADAR_SCAN_SYSTEM: return TEXT("");
		case QuestObjectiveType::RECOVERY_DEATH_PANALTY: return TEXT("");
		case QuestObjectiveType::TELEPORT_BY_WARP_REASON: return TEXT("");
		case QuestObjectiveType::PROTECT_TREASURE_MONSTER: return TEXT("");
		case QuestObjectiveType::MONSTER_KNOWLEDGE_NODE_COUNT: return TEXT("");
		case QuestObjectiveType::TITLE_HAVE_COUNT: return TEXT("");
		case QuestObjectiveType::ITEM_COLLECTION_COMPLETE_COUNT: return TEXT("");
		case QuestObjectiveType::COSTUME_COLLECTION_COMPLETE_COUNT: return TEXT("");
		case QuestObjectiveType::FAIRY_COLLECTION_COMPLETE_COUNT: return TEXT("");
		case QuestObjectiveType::DAMAGE_OTHER_PLAYERS_COUNT: return TEXT("");
		case QuestObjectiveType::INVADE_COUNT: return TEXT("");
		case QuestObjectiveType::INVADE_TRREASUREBOX_PROP: return TEXT("");
		case QuestObjectiveType::KILL_PLAYER: return TEXT("");
		case QuestObjectiveType::KILL_PLAYER_INVADE: return TEXT("");
		case QuestObjectiveType::KILL_PLAYER_WEAK: return TEXT("");
		case QuestObjectiveType::KILL_PLAYER_SAME_OR_STRONG: return TEXT("");
		case QuestObjectiveType::KILL_PLAYER_RANKING_PK: return TEXT("");
		case QuestObjectiveType::KILL_PLAYER_RANKING_LEVEL: return TEXT("");
		case QuestObjectiveType::KILL_PLAYER_RANKING_ALLSERVERLEVEL: return TEXT("");
		case QuestObjectiveType::KILL_PLAYER_ENEMYGUILD: return TEXT("");
		case QuestObjectiveType::AUTO_USE_SKILL_BY_CATEGORY_SET: return TEXT("");
		case QuestObjectiveType::AUTO_USE_ITEM_BY_CATEGORY_SUB: return TEXT("");
		case QuestObjectiveType::MULTI_LEVEL_RANK_RANK: return TEXT("");
		case QuestObjectiveType::JOIN_GUILD: return TEXT("");
		case QuestObjectiveType::GUILD_DUNGEON_CLEAR: return TEXT("");
		case QuestObjectiveType::GUILD_CAMP_BUILD: return TEXT("");
		case QuestObjectiveType::DESTORY_CAMP: return TEXT("");
		case QuestObjectiveType::DESTORY_CAMP_ENEMYGUILD: return TEXT("");
		case QuestObjectiveType::PARTY_JOIN: return TEXT("");
		case QuestObjectiveType::FRIEND_REQUEST: return TEXT("");
		case QuestObjectiveType::PROFILE_OTHER_USER_COMMENT_INPUT: return TEXT("");
		case QuestObjectiveType::OFFLINE_PLAY_TIME_ACCUMULATE: return TEXT("");
		case QuestObjectiveType::COMPLETE_MAP_EVENT: return TEXT("");
		case QuestObjectiveType::PARTY_DUNGEON_CLEAR_GROUPID: return TEXT("");
		case QuestObjectiveType::KILL_CREATURE_DUNGEONGROUPID: return TEXT("");
	}
	return FString();
}
inline FString QuestStateEnumToDisplayKey(QuestState value)
{
	switch (value)
	{
		case QuestState::NONE: return TEXT("");
		case QuestState::PRE_ACCEPT: return TEXT("");
		case QuestState::ACCEPTED: return TEXT("");
		case QuestState::COMPLETED: return TEXT("");
		case QuestState::REWARDED: return TEXT("");
		case QuestState::DELETED: return TEXT("");
		case QuestState::GIVEUP: return TEXT("");
	}
	return FString();
}
inline FString QuestObjectiveStateEnumToDisplayKey(QuestObjectiveState value)
{
	switch (value)
	{
		case QuestObjectiveState::IN_PROGRESS: return TEXT("");
		case QuestObjectiveState::COMPLETE: return TEXT("");
	}
	return FString();
}
inline FString QuestObjectiveProcessTypeEnumToDisplayKey(QuestObjectiveProcessType value)
{
	switch (value)
	{
		case QuestObjectiveProcessType::PARALLER: return TEXT("");
		case QuestObjectiveProcessType::ORDER: return TEXT("");
	}
	return FString();
}
inline FString GuildQuestGradeEnumToDisplayKey(GuildQuestGrade value)
{
	switch (value)
	{
		case GuildQuestGrade::NORMAL: return TEXT("");
		case GuildQuestGrade::MAGIC: return TEXT("");
		case GuildQuestGrade::RARE: return TEXT("");
		case GuildQuestGrade::EPIC: return TEXT("");
	}
	return FString();
}
inline FString QuestBoardTypeEnumToDisplayKey(QuestBoardType value)
{
	switch (value)
	{
		case QuestBoardType::NONE: return TEXT("");
		case QuestBoardType::VILLAGE: return TEXT("");
		case QuestBoardType::INVADE: return TEXT("");
	}
	return FString();
}
inline FString QuestContentsTypeEnumToDisplayKey(QuestContentsType value)
{
	switch (value)
	{
		case QuestContentsType::ALL: return TEXT("");
		case QuestContentsType::NORMAL: return TEXT("");
		case QuestContentsType::INVADE: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidRepeatStoryType(RepeatStoryType value) noexcept
{
	return (value >= RepeatStoryType::BOARD && value < RepeatStoryType::MAX);
}

inline bool IsValidRepeatStoryGrade(RepeatStoryGrade value) noexcept
{
	return (value >= RepeatStoryGrade::NORMAL && value < RepeatStoryGrade::MAX);
}

inline bool IsValidQuestType(QuestType value) noexcept
{
	return (value >= QuestType::MAIN && value < QuestType::MAX);
}

inline bool IsValidQuestObjectiveType(QuestObjectiveType value) noexcept
{
	return (value >= QuestObjectiveType::KILL && value < QuestObjectiveType::MAX);
}

inline bool IsValidQuestState(QuestState value) noexcept
{
	return (value >= QuestState::NONE && value < QuestState::MAX);
}

inline bool IsValidQuestObjectiveState(QuestObjectiveState value) noexcept
{
	return (value >= QuestObjectiveState::IN_PROGRESS && value < QuestObjectiveState::MAX);
}

inline bool IsValidQuestObjectiveProcessType(QuestObjectiveProcessType value) noexcept
{
	return (value >= QuestObjectiveProcessType::PARALLER && value < QuestObjectiveProcessType::MAX);
}

inline bool IsValidGuildQuestGrade(GuildQuestGrade value) noexcept
{
	return (value >= GuildQuestGrade::NORMAL && value < GuildQuestGrade::MAX);
}

inline bool IsValidQuestBoardType(QuestBoardType value) noexcept
{
	return (value >= QuestBoardType::NONE && value < QuestBoardType::MAX);
}

inline bool IsValidQuestContentsType(QuestContentsType value) noexcept
{
	return (value >= QuestContentsType::ALL && value < QuestContentsType::MAX);
}


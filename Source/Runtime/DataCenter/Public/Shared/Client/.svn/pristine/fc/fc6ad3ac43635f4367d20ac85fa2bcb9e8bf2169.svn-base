#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedTitleEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 타이틀 조건 타입(t_user_title에 사용)
enum class TitleConditionType : uint8
{

	// 조건 없음 - 에러
	NONE = 0,

	// 케릭터 래벨 달성
	CHARACTER_LEVEL = 1,

	// 특정 코스튬 획득
	COSTUME_GET_ID = 2,

	// 코스튬 등급 소환
	COSTUME_SUMMON_GRADE = 3,

	// 코스튬 합성
	COSTUME_COMPOSE_GRADE = 4,

	// 특정 페어리 획득
	FAIRY_GET_ID = 5,

	// 페어리 소환
	FAIRY_SUMMON_GRADE = 6,

	// 페어리 합성
	FAIRY_COMPOSE_GRADE = 7,

	// 페어리 성장 시도
	FAIRY_ENCHANT = 8,

	// 페어리 성장 실패
	FAIRY_ENCHANT_FAIL = 9,

	// 페어리 성장 성공
	FAIRY_ENCHANT_SUCCESS = 10,

	// BM샵 아이템 구매
	BM_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB = 11,

	// 재화 소비
	CURRENCY_ACCUMULATE_CONSUME = 12,

	// 재화 획득
	CURRENCY_ACCUMULATE_GET = 13,

	// 몬스터에게 사망
	DEATH_BY_MONSTER = 14,

	// 플레이어에게 사망
	DEATH_BY_PLAYER = 15,

	// 경계대상에게 사망
	DEATH_BY_PLAYER_MONITORING_USER = 16,

	// 친구 추가
	FRIEND_MAKE = 17,

	// 기사단 출석
	GUILD_ATTENDANCE = 18,

	// 기사단 기여
	GUILD_CONTRIBUTE = 19,

	// 기사단 기부 재화소모
	GUILD_DONATE_CURRENCY_ACCUMULATE = 20,

	// 기사단 던전 클리어
	GUILD_DUNGEON_CLEAR = 21,

	// 기사단 컬렉션 등록
	GUILD_ITEM_COLLECTION_REGISTER = 22,

	// 특정 레벨 기사단의 특정 직위 달성
	GUILD_MEMBER_GRADE_LEVEL = 23,

	// 기사단 퀘스트 클리어
	GUILD_QUEST_COMPLETE = 24,

	// 아이템 사용
	ITEM_ACCUMULATE_USE = 25,

	// 특정 카테고리/등급 아이템 사용
	ITEM_ACCUMULATE_USE_CATEGORY_GRADE = 26,

	// 특정 서브 카테고리 아이템 사용
	ITEM_ACCUMULATE_USE_CATEGORY_SUB = 27,

	// 특정 카테고리 아이템 제작 실패
	ITEM_CRAFT_FAIL_CATEGORY_MAIN_SUB = 28,

	// 특정 카테고리 아이템 대성공
	ITEM_CRAFT_GREATE_SUCCESS_CATEGORY = 29,

	// 특정 타입 아이템 제작 대성공
	ITEM_CRAFT_GREATE_SUCCESS_TYPE_GRADE = 30,

	// 특정 카테고리 아이템/등급 제작 성공
	ITEM_CRAFT_SUCCESS_CATEGORY_GRADE = 31,

	// 특정 서브카테고리 제작 성공
	ITEM_CRAFT_SUCCESS_SUB_CATEGORY = 32,

	// 특정 카테고리 아이템 제작 시도
	ITEM_CRAFT_TRY_CATEGORY = 33,

	// 특정 타입/등급 제작시도
	ITEM_CRAFT_TRY_TYPE_GRADE = 34,

	// 아이템 분해 시도
	ITEM_DECOMPOSE_TRY = 35,

	// 아이템 강화 실패
	ITEM_ENCHANT_FAIL_CATEGORY = 36,

	// 특정 카테고리/등급 강화 성공
	ITEM_ENCHANT_SUCCESS_CATEGORY_GRADE = 37,

	// 아이템 강화 성공(강화레벨/서브카테고리)
	ITEM_ENCHANT_SUCCESS_LEVEL_SUB = 38,

	// 특정 등급 아이템 루팅
	ITEM_LOOT_GRADE_CATEGORY = 39,

	// 아이템 재련
	ITEM_REFINE = 40,

	// 특정 몬스터 사냥
	KILL_MONSTER_GRADE_ID = 41,

	// 특정 지역 몬스터 사냥
	KILL_MONSTER_MAP_GRADE = 42,

	// 특정 종족 몬스터 사냥
	KILL_MONSTER_RACE = 43,

	// 플레이어 사냥
	KILL_PLAYER = 44,

	// 인터서버 플레이어 사냥
	KILL_PLAYER_INTER_SERVER = 45,

	// 보물몬스터 사냥
	KILL_TREASURE_MONSTER = 46,

	// 보물몬스터 수호
	SAVE_TREASURE_MONSTER = 47,

	// 누적 로그인
	LOGIN_ACCUMULATE = 48,

	// 거래소 구매
	MARKET_BUY = 49,

	// 거래소 구매 누적금액
	MARKET_BUY_PRICE_ACCUMULATE = 50,

	// 거래소 판매
	MARKET_SELL = 51,

	// 거래소 판매 누적금액
	MARKET_SELL_PRICE_ACCUMULATE = 52,

	// 특정 종족 몬스터 도감 완성
	MONSTER_KNOWLEDGE_COMPLETE_RACE = 53,

	// 특정 카테고리/등급 아이템 구매
	NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_GRADE = 54,

	// 메인/서브 카테고리 아이템 구매
	NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB = 55,

	// 특정 ID아이템 구매
	NPC_SHOP_BUY_ITEM_ITEM_ID = 56,

	// PVP단계 달성
	PVP_ALIGNMENT = 57,

	// 특정 단계 반복퀘스트 완료
	QUEST_COMPLETE_REPEAT_GRADE = 58,

	// 특정 지역 반복퀘스트 완료
	QUEST_COMPLETE_REPEAT_MAP = 59,

	// 랭킹 달성
	RANK_ACHIEVEMENT = 60,

	// 필드보스 기여
	RANK_FIELD_BOSS_CONTRIBUTE = 61,

	// 레이드 보스 기여
	RANK_RAID_BOSS_CONTRIBUTE = 62,

	// 특정 타입 몬스터 처치 기여 : NpcFunctionType
	RANK_CONTRIBUTE_NPCFUNCTIONTYPE = 63,

	// 랭킹 범위 달성
	RANK_RANGE_ACHIEVEMENT = 64,

	// 즉시 부활 사용
	RESURRECT_INSTANT_USE = 65,

	// 스킬강화 시도
	SKILL_ENCHANT_TRY = 66,

	// 스킬 업그레이드 시도
	SKILL_UPGRADE_TRY = 67,

	// 시공의 틈새 클리어
	SPACE_CRACK_CLEAR = 68,

	// 시공의 틈새 특정 몬스터 사냥
	SPACE_CRACK_KILL_MONSTER_GRADE = 69,

	// 특정 등급 기사단 퀘스트 클리어
	GUILD_QUEST_COMPLETE_GRADE = 70,

	// 침공 플레이어 처치
	KILL_PLAYER_INVASION = 71,

	// 특정 퀘스트 클리어(퀘스트 스토리 완료, 보상 받을때 체크)
	QUEST_CLEAR = 72,

	// 프로필 키워드 획득
	PROFILE_PROFILE_KEYWORD_GET = 73,

	// 전체 프로필 키워드 획득
	ALL_PROFILE_KEYWORD_GET = 74,

	// 특정 등급 장비 아이템 제거
	EQUIP_ITEM_DELETE_GRADE = 75,

	// 수동 공격 사용
	USE_MANUAL_ATTACK = 76,

	// 오프라인 플레이 사용 누적시간
	USE_OFFLINE_PLAY = 77,

	// 프로필 자기소개 입력
	PROFILE_INTRODUCTION_INPUT = 78,

	// 인벤토리 확장
	INVENTORY_EXTEND = 79,

	// 창고 확장
	DEPOT_EXTEND = 80,

	// 장착된 모든 아이템이 특정 등급 이상인지 체크
	EQUIPED_ALL_ITEM_MIN_GRADE = 81,

	// BM샵 상품 구매(BMProductID)
	BM_SHOP_BUY_PRODUCT = 82,

	// 승급 랭킹 달성(MultiLevelRankId)
	MULTI_LEVEL_RANK_ACTIVE = 83,

	// 한번의 데미지로 유저를 죽임
	KILL_USER_BY_ONE_DAMAGE = 84,

	// 적대길드 유저를 죽임
	KILL_HOSTILE_GUILD_USER = 85,

	// 침공의 증표 목표 달성
	INVADE_CURRENCY_ITEM = 86,

	// 아이템 컬렉션 완성
	ITEM_COLLECTION_COMPLETE = 87,

	// 이벤트 기도사 버프 획득
	GET_EVENT_PRAYER_EFFECTIVE = 88,

	// 출석 체크 보상 : AttendanceId, AttendanceCheckIdx
	CHECK_ATTENDANCE = 89,

	// 타이틀 보유 개수 : TitleCategoryId
	TITLE_HAVE_COUNT = 90,

	// 카오 상태에서 플레이어 처치
	KILL_PLAYER_IN_CHAOTIC = 91,

	// PK북 기여도 - 특정 기여도 이상
	PK_BOOK_CONTRIBUTE_UP = 92,

	// PK북 기여도 - 특정 기여도 이하
	PK_BOOK_CONTRIBUTE_DOWN = 93,

	// PK북 처치 기록
	PK_BOOK_KILL = 94,

	// PK북 도움 기록
	PK_BOOK_ASSIST = 95,

	// PK북 사망 기록
	PK_BOOK_DIE = 96,

	// PK북 조롱하기
	PK_BOOK_TEASE = 97,

	// 적대길드 캠프 파괴
	DESTROY_HOSTILE_GUILD_CAMP = 98,

	// 배틀패스 일반 보상 획득 : BattlePassId, Level
	BATTLE_PASS_REWARD_NORMAL = 99,

	// 배틀패스 추가 보상 획득 : BattlePassId, Level
	BATTLE_PASS_REWARD_ADDITIONAL = 100,

	// 봉인석 각인 시도
	SEAL_NODE_REROLL = 101,

	// 천마석 잠식 시도
	SPIRITSHOT_ENCHANT_TRY = 102,

	// 천마석 강화 합산레벨 달성 : Level
	SPIRITSHOT_ENCHANT_SUMLEVEL = 103,

	// 도감 시스템 노드 해금
	SLATE_CODEX_COUNT = 104,

	// 성소 점령전 승리
	SANCTUM_VICTORY = 105,

	// 성소 보유
	SANCTUM_POSSESS = 106,

	// 그림자전장 승리
	GUILD_ARENA_VICTORY = 107,

	// 그림자전장 패배
	GUILD_ARENA_DEFEAT = 108,

	// 아이템 개화 시도 : ItemCategorySub
	ITEM_MAGICALFORGE_TRY = 109,

	// 아이템 개화 등급,레벨 달성 : MagicalForgeGrade, MagicalForgeLevel
	ITEM_MAGICALFORGE_GRADE_LEVEL = 110,

	// 누적 지식 획득량 달성
	CODEX_TOTAL_KNOWLEDGE = 111,

	MAX = 112,
};

UENUM(BlueprintType)
// 타이틀 등급
enum class TitleGrade : uint8
{

	// 노멀
	NORMAL = 0,

	// 매직
	MAGIC = 1,

	// 레어
	RARE = 2,

	// 에픽
	EPIC = 3,

	// 유니크
	UNIQUE = 4,

	// 레전드
	LEGEND = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// 칭호 슬롯 타입(t_user_title_equip에 사용)
enum class TitleEquipSlot : uint8
{

	// 앞
	FRONT = 0,

	// 뒤
	BACK = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 칭호 보유 타입
enum class TitleRetainType : uint8
{

	// 영구 보유
	PERMANENT = 0,

	// 조건부 보유
	CONDITIONAL = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 칭호 조건 체크 타입
enum class TitleConditionCheckType : uint8
{

	// 캐릭터 생성 시점
	CREATE_CHARACTER = 0,

	// 컨텐츠 오픈 시점
	UNLOCK_CONTENTS = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 칭호 상태
enum class TitleStatus : uint8
{

	// 비활성화
	DISABLED = 0,

	// 활성화 상태
	ACTIVED = 1,

	// 활성화, 알림확인 완료
	COMPLETED = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 타이틀 조건데이터 타입
enum class TitleConditionDataType : uint8
{

	// 없음
	NONE = 0,

	// 코스튬
	COSTUME = 1,

	// 아이템
	ITEM = 2,

	// 퀘스트
	QUEST = 3,

	// 랭크
	RANK = 4,

	// 크리쳐
	CREATURE = 5,

	// 재화
	CURRENCY = 6,

	// 길드
	GUILD = 7,

	// 페어리
	FAIRY = 8,

	// 스킬
	SKILL = 9,

	// 기타
	ETC = 10,

	MAX = 11,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline TitleConditionType TitleConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::NONE;
	if (str.Compare(TEXT("CHARACTER_LEVEL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::CHARACTER_LEVEL;
	if (str.Compare(TEXT("COSTUME_GET_ID"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::COSTUME_GET_ID;
	if (str.Compare(TEXT("COSTUME_SUMMON_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::COSTUME_SUMMON_GRADE;
	if (str.Compare(TEXT("COSTUME_COMPOSE_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::COSTUME_COMPOSE_GRADE;
	if (str.Compare(TEXT("FAIRY_GET_ID"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::FAIRY_GET_ID;
	if (str.Compare(TEXT("FAIRY_SUMMON_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::FAIRY_SUMMON_GRADE;
	if (str.Compare(TEXT("FAIRY_COMPOSE_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::FAIRY_COMPOSE_GRADE;
	if (str.Compare(TEXT("FAIRY_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::FAIRY_ENCHANT;
	if (str.Compare(TEXT("FAIRY_ENCHANT_FAIL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::FAIRY_ENCHANT_FAIL;
	if (str.Compare(TEXT("FAIRY_ENCHANT_SUCCESS"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::FAIRY_ENCHANT_SUCCESS;
	if (str.Compare(TEXT("BM_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::BM_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB;
	if (str.Compare(TEXT("CURRENCY_ACCUMULATE_CONSUME"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::CURRENCY_ACCUMULATE_CONSUME;
	if (str.Compare(TEXT("CURRENCY_ACCUMULATE_GET"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::CURRENCY_ACCUMULATE_GET;
	if (str.Compare(TEXT("DEATH_BY_MONSTER"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::DEATH_BY_MONSTER;
	if (str.Compare(TEXT("DEATH_BY_PLAYER"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::DEATH_BY_PLAYER;
	if (str.Compare(TEXT("DEATH_BY_PLAYER_MONITORING_USER"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::DEATH_BY_PLAYER_MONITORING_USER;
	if (str.Compare(TEXT("FRIEND_MAKE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::FRIEND_MAKE;
	if (str.Compare(TEXT("GUILD_ATTENDANCE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GUILD_ATTENDANCE;
	if (str.Compare(TEXT("GUILD_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GUILD_CONTRIBUTE;
	if (str.Compare(TEXT("GUILD_DONATE_CURRENCY_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GUILD_DONATE_CURRENCY_ACCUMULATE;
	if (str.Compare(TEXT("GUILD_DUNGEON_CLEAR"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GUILD_DUNGEON_CLEAR;
	if (str.Compare(TEXT("GUILD_ITEM_COLLECTION_REGISTER"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GUILD_ITEM_COLLECTION_REGISTER;
	if (str.Compare(TEXT("GUILD_MEMBER_GRADE_LEVEL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GUILD_MEMBER_GRADE_LEVEL;
	if (str.Compare(TEXT("GUILD_QUEST_COMPLETE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GUILD_QUEST_COMPLETE;
	if (str.Compare(TEXT("ITEM_ACCUMULATE_USE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_ACCUMULATE_USE;
	if (str.Compare(TEXT("ITEM_ACCUMULATE_USE_CATEGORY_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_ACCUMULATE_USE_CATEGORY_GRADE;
	if (str.Compare(TEXT("ITEM_ACCUMULATE_USE_CATEGORY_SUB"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_ACCUMULATE_USE_CATEGORY_SUB;
	if (str.Compare(TEXT("ITEM_CRAFT_FAIL_CATEGORY_MAIN_SUB"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_CRAFT_FAIL_CATEGORY_MAIN_SUB;
	if (str.Compare(TEXT("ITEM_CRAFT_GREATE_SUCCESS_CATEGORY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_CRAFT_GREATE_SUCCESS_CATEGORY;
	if (str.Compare(TEXT("ITEM_CRAFT_GREATE_SUCCESS_TYPE_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_CRAFT_GREATE_SUCCESS_TYPE_GRADE;
	if (str.Compare(TEXT("ITEM_CRAFT_SUCCESS_CATEGORY_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_CRAFT_SUCCESS_CATEGORY_GRADE;
	if (str.Compare(TEXT("ITEM_CRAFT_SUCCESS_SUB_CATEGORY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_CRAFT_SUCCESS_SUB_CATEGORY;
	if (str.Compare(TEXT("ITEM_CRAFT_TRY_CATEGORY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_CRAFT_TRY_CATEGORY;
	if (str.Compare(TEXT("ITEM_CRAFT_TRY_TYPE_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_CRAFT_TRY_TYPE_GRADE;
	if (str.Compare(TEXT("ITEM_DECOMPOSE_TRY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_DECOMPOSE_TRY;
	if (str.Compare(TEXT("ITEM_ENCHANT_FAIL_CATEGORY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_ENCHANT_FAIL_CATEGORY;
	if (str.Compare(TEXT("ITEM_ENCHANT_SUCCESS_CATEGORY_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_ENCHANT_SUCCESS_CATEGORY_GRADE;
	if (str.Compare(TEXT("ITEM_ENCHANT_SUCCESS_LEVEL_SUB"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_ENCHANT_SUCCESS_LEVEL_SUB;
	if (str.Compare(TEXT("ITEM_LOOT_GRADE_CATEGORY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_LOOT_GRADE_CATEGORY;
	if (str.Compare(TEXT("ITEM_REFINE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_REFINE;
	if (str.Compare(TEXT("KILL_MONSTER_GRADE_ID"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::KILL_MONSTER_GRADE_ID;
	if (str.Compare(TEXT("KILL_MONSTER_MAP_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::KILL_MONSTER_MAP_GRADE;
	if (str.Compare(TEXT("KILL_MONSTER_RACE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::KILL_MONSTER_RACE;
	if (str.Compare(TEXT("KILL_PLAYER"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::KILL_PLAYER;
	if (str.Compare(TEXT("KILL_PLAYER_INTER_SERVER"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::KILL_PLAYER_INTER_SERVER;
	if (str.Compare(TEXT("KILL_TREASURE_MONSTER"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::KILL_TREASURE_MONSTER;
	if (str.Compare(TEXT("SAVE_TREASURE_MONSTER"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SAVE_TREASURE_MONSTER;
	if (str.Compare(TEXT("LOGIN_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::LOGIN_ACCUMULATE;
	if (str.Compare(TEXT("MARKET_BUY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::MARKET_BUY;
	if (str.Compare(TEXT("MARKET_BUY_PRICE_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::MARKET_BUY_PRICE_ACCUMULATE;
	if (str.Compare(TEXT("MARKET_SELL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::MARKET_SELL;
	if (str.Compare(TEXT("MARKET_SELL_PRICE_ACCUMULATE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::MARKET_SELL_PRICE_ACCUMULATE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_COMPLETE_RACE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::MONSTER_KNOWLEDGE_COMPLETE_RACE;
	if (str.Compare(TEXT("NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_GRADE;
	if (str.Compare(TEXT("NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB;
	if (str.Compare(TEXT("NPC_SHOP_BUY_ITEM_ITEM_ID"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::NPC_SHOP_BUY_ITEM_ITEM_ID;
	if (str.Compare(TEXT("PVP_ALIGNMENT"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::PVP_ALIGNMENT;
	if (str.Compare(TEXT("QUEST_COMPLETE_REPEAT_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::QUEST_COMPLETE_REPEAT_GRADE;
	if (str.Compare(TEXT("QUEST_COMPLETE_REPEAT_MAP"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::QUEST_COMPLETE_REPEAT_MAP;
	if (str.Compare(TEXT("RANK_ACHIEVEMENT"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::RANK_ACHIEVEMENT;
	if (str.Compare(TEXT("RANK_FIELD_BOSS_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::RANK_FIELD_BOSS_CONTRIBUTE;
	if (str.Compare(TEXT("RANK_RAID_BOSS_CONTRIBUTE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::RANK_RAID_BOSS_CONTRIBUTE;
	if (str.Compare(TEXT("RANK_CONTRIBUTE_NPCFUNCTIONTYPE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::RANK_CONTRIBUTE_NPCFUNCTIONTYPE;
	if (str.Compare(TEXT("RANK_RANGE_ACHIEVEMENT"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::RANK_RANGE_ACHIEVEMENT;
	if (str.Compare(TEXT("RESURRECT_INSTANT_USE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::RESURRECT_INSTANT_USE;
	if (str.Compare(TEXT("SKILL_ENCHANT_TRY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SKILL_ENCHANT_TRY;
	if (str.Compare(TEXT("SKILL_UPGRADE_TRY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SKILL_UPGRADE_TRY;
	if (str.Compare(TEXT("SPACE_CRACK_CLEAR"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SPACE_CRACK_CLEAR;
	if (str.Compare(TEXT("SPACE_CRACK_KILL_MONSTER_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SPACE_CRACK_KILL_MONSTER_GRADE;
	if (str.Compare(TEXT("GUILD_QUEST_COMPLETE_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GUILD_QUEST_COMPLETE_GRADE;
	if (str.Compare(TEXT("KILL_PLAYER_INVASION"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::KILL_PLAYER_INVASION;
	if (str.Compare(TEXT("QUEST_CLEAR"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::QUEST_CLEAR;
	if (str.Compare(TEXT("PROFILE_PROFILE_KEYWORD_GET"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::PROFILE_PROFILE_KEYWORD_GET;
	if (str.Compare(TEXT("ALL_PROFILE_KEYWORD_GET"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ALL_PROFILE_KEYWORD_GET;
	if (str.Compare(TEXT("EQUIP_ITEM_DELETE_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::EQUIP_ITEM_DELETE_GRADE;
	if (str.Compare(TEXT("USE_MANUAL_ATTACK"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::USE_MANUAL_ATTACK;
	if (str.Compare(TEXT("USE_OFFLINE_PLAY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::USE_OFFLINE_PLAY;
	if (str.Compare(TEXT("PROFILE_INTRODUCTION_INPUT"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::PROFILE_INTRODUCTION_INPUT;
	if (str.Compare(TEXT("INVENTORY_EXTEND"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::INVENTORY_EXTEND;
	if (str.Compare(TEXT("DEPOT_EXTEND"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::DEPOT_EXTEND;
	if (str.Compare(TEXT("EQUIPED_ALL_ITEM_MIN_GRADE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::EQUIPED_ALL_ITEM_MIN_GRADE;
	if (str.Compare(TEXT("BM_SHOP_BUY_PRODUCT"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::BM_SHOP_BUY_PRODUCT;
	if (str.Compare(TEXT("MULTI_LEVEL_RANK_ACTIVE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::MULTI_LEVEL_RANK_ACTIVE;
	if (str.Compare(TEXT("KILL_USER_BY_ONE_DAMAGE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::KILL_USER_BY_ONE_DAMAGE;
	if (str.Compare(TEXT("KILL_HOSTILE_GUILD_USER"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::KILL_HOSTILE_GUILD_USER;
	if (str.Compare(TEXT("INVADE_CURRENCY_ITEM"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::INVADE_CURRENCY_ITEM;
	if (str.Compare(TEXT("ITEM_COLLECTION_COMPLETE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_COLLECTION_COMPLETE;
	if (str.Compare(TEXT("GET_EVENT_PRAYER_EFFECTIVE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GET_EVENT_PRAYER_EFFECTIVE;
	if (str.Compare(TEXT("CHECK_ATTENDANCE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::CHECK_ATTENDANCE;
	if (str.Compare(TEXT("TITLE_HAVE_COUNT"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::TITLE_HAVE_COUNT;
	if (str.Compare(TEXT("KILL_PLAYER_IN_CHAOTIC"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::KILL_PLAYER_IN_CHAOTIC;
	if (str.Compare(TEXT("PK_BOOK_CONTRIBUTE_UP"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::PK_BOOK_CONTRIBUTE_UP;
	if (str.Compare(TEXT("PK_BOOK_CONTRIBUTE_DOWN"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::PK_BOOK_CONTRIBUTE_DOWN;
	if (str.Compare(TEXT("PK_BOOK_KILL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::PK_BOOK_KILL;
	if (str.Compare(TEXT("PK_BOOK_ASSIST"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::PK_BOOK_ASSIST;
	if (str.Compare(TEXT("PK_BOOK_DIE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::PK_BOOK_DIE;
	if (str.Compare(TEXT("PK_BOOK_TEASE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::PK_BOOK_TEASE;
	if (str.Compare(TEXT("DESTROY_HOSTILE_GUILD_CAMP"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::DESTROY_HOSTILE_GUILD_CAMP;
	if (str.Compare(TEXT("BATTLE_PASS_REWARD_NORMAL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::BATTLE_PASS_REWARD_NORMAL;
	if (str.Compare(TEXT("BATTLE_PASS_REWARD_ADDITIONAL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::BATTLE_PASS_REWARD_ADDITIONAL;
	if (str.Compare(TEXT("SEAL_NODE_REROLL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SEAL_NODE_REROLL;
	if (str.Compare(TEXT("SPIRITSHOT_ENCHANT_TRY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SPIRITSHOT_ENCHANT_TRY;
	if (str.Compare(TEXT("SPIRITSHOT_ENCHANT_SUMLEVEL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SPIRITSHOT_ENCHANT_SUMLEVEL;
	if (str.Compare(TEXT("SLATE_CODEX_COUNT"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SLATE_CODEX_COUNT;
	if (str.Compare(TEXT("SANCTUM_VICTORY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SANCTUM_VICTORY;
	if (str.Compare(TEXT("SANCTUM_POSSESS"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::SANCTUM_POSSESS;
	if (str.Compare(TEXT("GUILD_ARENA_VICTORY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GUILD_ARENA_VICTORY;
	if (str.Compare(TEXT("GUILD_ARENA_DEFEAT"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::GUILD_ARENA_DEFEAT;
	if (str.Compare(TEXT("ITEM_MAGICALFORGE_TRY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_MAGICALFORGE_TRY;
	if (str.Compare(TEXT("ITEM_MAGICALFORGE_GRADE_LEVEL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::ITEM_MAGICALFORGE_GRADE_LEVEL;
	if (str.Compare(TEXT("CODEX_TOTAL_KNOWLEDGE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionType::CODEX_TOTAL_KNOWLEDGE;
	return TitleConditionType::MAX;
}
inline TitleGrade TitleGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return TitleGrade::NORMAL;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return TitleGrade::MAGIC;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return TitleGrade::RARE;
	if (str.Compare(TEXT("EPIC"), ESearchCase::IgnoreCase) == 0)	return TitleGrade::EPIC;
	if (str.Compare(TEXT("UNIQUE"), ESearchCase::IgnoreCase) == 0)	return TitleGrade::UNIQUE;
	if (str.Compare(TEXT("LEGEND"), ESearchCase::IgnoreCase) == 0)	return TitleGrade::LEGEND;
	return TitleGrade::MAX;
}
inline TitleEquipSlot TitleEquipSlotStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("FRONT"), ESearchCase::IgnoreCase) == 0)	return TitleEquipSlot::FRONT;
	if (str.Compare(TEXT("BACK"), ESearchCase::IgnoreCase) == 0)	return TitleEquipSlot::BACK;
	return TitleEquipSlot::MAX;
}
inline TitleRetainType TitleRetainTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("PERMANENT"), ESearchCase::IgnoreCase) == 0)	return TitleRetainType::PERMANENT;
	if (str.Compare(TEXT("CONDITIONAL"), ESearchCase::IgnoreCase) == 0)	return TitleRetainType::CONDITIONAL;
	return TitleRetainType::MAX;
}
inline TitleConditionCheckType TitleConditionCheckTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CREATE_CHARACTER"), ESearchCase::IgnoreCase) == 0)	return TitleConditionCheckType::CREATE_CHARACTER;
	if (str.Compare(TEXT("UNLOCK_CONTENTS"), ESearchCase::IgnoreCase) == 0)	return TitleConditionCheckType::UNLOCK_CONTENTS;
	return TitleConditionCheckType::MAX;
}
inline TitleStatus TitleStatusStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("DISABLED"), ESearchCase::IgnoreCase) == 0)	return TitleStatus::DISABLED;
	if (str.Compare(TEXT("ACTIVED"), ESearchCase::IgnoreCase) == 0)	return TitleStatus::ACTIVED;
	if (str.Compare(TEXT("COMPLETED"), ESearchCase::IgnoreCase) == 0)	return TitleStatus::COMPLETED;
	return TitleStatus::MAX;
}
inline TitleConditionDataType TitleConditionDataTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::NONE;
	if (str.Compare(TEXT("COSTUME"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::COSTUME;
	if (str.Compare(TEXT("ITEM"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::ITEM;
	if (str.Compare(TEXT("QUEST"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::QUEST;
	if (str.Compare(TEXT("RANK"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::RANK;
	if (str.Compare(TEXT("CREATURE"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::CREATURE;
	if (str.Compare(TEXT("CURRENCY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::CURRENCY;
	if (str.Compare(TEXT("GUILD"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::GUILD;
	if (str.Compare(TEXT("FAIRY"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::FAIRY;
	if (str.Compare(TEXT("SKILL"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::SKILL;
	if (str.Compare(TEXT("ETC"), ESearchCase::IgnoreCase) == 0)	return TitleConditionDataType::ETC;
	return TitleConditionDataType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString TitleConditionTypeEnumToString(TitleConditionType value)
{
	switch (value)
	{
		case TitleConditionType::NONE: return TEXT("NONE");
		case TitleConditionType::CHARACTER_LEVEL: return TEXT("CHARACTER_LEVEL");
		case TitleConditionType::COSTUME_GET_ID: return TEXT("COSTUME_GET_ID");
		case TitleConditionType::COSTUME_SUMMON_GRADE: return TEXT("COSTUME_SUMMON_GRADE");
		case TitleConditionType::COSTUME_COMPOSE_GRADE: return TEXT("COSTUME_COMPOSE_GRADE");
		case TitleConditionType::FAIRY_GET_ID: return TEXT("FAIRY_GET_ID");
		case TitleConditionType::FAIRY_SUMMON_GRADE: return TEXT("FAIRY_SUMMON_GRADE");
		case TitleConditionType::FAIRY_COMPOSE_GRADE: return TEXT("FAIRY_COMPOSE_GRADE");
		case TitleConditionType::FAIRY_ENCHANT: return TEXT("FAIRY_ENCHANT");
		case TitleConditionType::FAIRY_ENCHANT_FAIL: return TEXT("FAIRY_ENCHANT_FAIL");
		case TitleConditionType::FAIRY_ENCHANT_SUCCESS: return TEXT("FAIRY_ENCHANT_SUCCESS");
		case TitleConditionType::BM_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB: return TEXT("BM_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB");
		case TitleConditionType::CURRENCY_ACCUMULATE_CONSUME: return TEXT("CURRENCY_ACCUMULATE_CONSUME");
		case TitleConditionType::CURRENCY_ACCUMULATE_GET: return TEXT("CURRENCY_ACCUMULATE_GET");
		case TitleConditionType::DEATH_BY_MONSTER: return TEXT("DEATH_BY_MONSTER");
		case TitleConditionType::DEATH_BY_PLAYER: return TEXT("DEATH_BY_PLAYER");
		case TitleConditionType::DEATH_BY_PLAYER_MONITORING_USER: return TEXT("DEATH_BY_PLAYER_MONITORING_USER");
		case TitleConditionType::FRIEND_MAKE: return TEXT("FRIEND_MAKE");
		case TitleConditionType::GUILD_ATTENDANCE: return TEXT("GUILD_ATTENDANCE");
		case TitleConditionType::GUILD_CONTRIBUTE: return TEXT("GUILD_CONTRIBUTE");
		case TitleConditionType::GUILD_DONATE_CURRENCY_ACCUMULATE: return TEXT("GUILD_DONATE_CURRENCY_ACCUMULATE");
		case TitleConditionType::GUILD_DUNGEON_CLEAR: return TEXT("GUILD_DUNGEON_CLEAR");
		case TitleConditionType::GUILD_ITEM_COLLECTION_REGISTER: return TEXT("GUILD_ITEM_COLLECTION_REGISTER");
		case TitleConditionType::GUILD_MEMBER_GRADE_LEVEL: return TEXT("GUILD_MEMBER_GRADE_LEVEL");
		case TitleConditionType::GUILD_QUEST_COMPLETE: return TEXT("GUILD_QUEST_COMPLETE");
		case TitleConditionType::ITEM_ACCUMULATE_USE: return TEXT("ITEM_ACCUMULATE_USE");
		case TitleConditionType::ITEM_ACCUMULATE_USE_CATEGORY_GRADE: return TEXT("ITEM_ACCUMULATE_USE_CATEGORY_GRADE");
		case TitleConditionType::ITEM_ACCUMULATE_USE_CATEGORY_SUB: return TEXT("ITEM_ACCUMULATE_USE_CATEGORY_SUB");
		case TitleConditionType::ITEM_CRAFT_FAIL_CATEGORY_MAIN_SUB: return TEXT("ITEM_CRAFT_FAIL_CATEGORY_MAIN_SUB");
		case TitleConditionType::ITEM_CRAFT_GREATE_SUCCESS_CATEGORY: return TEXT("ITEM_CRAFT_GREATE_SUCCESS_CATEGORY");
		case TitleConditionType::ITEM_CRAFT_GREATE_SUCCESS_TYPE_GRADE: return TEXT("ITEM_CRAFT_GREATE_SUCCESS_TYPE_GRADE");
		case TitleConditionType::ITEM_CRAFT_SUCCESS_CATEGORY_GRADE: return TEXT("ITEM_CRAFT_SUCCESS_CATEGORY_GRADE");
		case TitleConditionType::ITEM_CRAFT_SUCCESS_SUB_CATEGORY: return TEXT("ITEM_CRAFT_SUCCESS_SUB_CATEGORY");
		case TitleConditionType::ITEM_CRAFT_TRY_CATEGORY: return TEXT("ITEM_CRAFT_TRY_CATEGORY");
		case TitleConditionType::ITEM_CRAFT_TRY_TYPE_GRADE: return TEXT("ITEM_CRAFT_TRY_TYPE_GRADE");
		case TitleConditionType::ITEM_DECOMPOSE_TRY: return TEXT("ITEM_DECOMPOSE_TRY");
		case TitleConditionType::ITEM_ENCHANT_FAIL_CATEGORY: return TEXT("ITEM_ENCHANT_FAIL_CATEGORY");
		case TitleConditionType::ITEM_ENCHANT_SUCCESS_CATEGORY_GRADE: return TEXT("ITEM_ENCHANT_SUCCESS_CATEGORY_GRADE");
		case TitleConditionType::ITEM_ENCHANT_SUCCESS_LEVEL_SUB: return TEXT("ITEM_ENCHANT_SUCCESS_LEVEL_SUB");
		case TitleConditionType::ITEM_LOOT_GRADE_CATEGORY: return TEXT("ITEM_LOOT_GRADE_CATEGORY");
		case TitleConditionType::ITEM_REFINE: return TEXT("ITEM_REFINE");
		case TitleConditionType::KILL_MONSTER_GRADE_ID: return TEXT("KILL_MONSTER_GRADE_ID");
		case TitleConditionType::KILL_MONSTER_MAP_GRADE: return TEXT("KILL_MONSTER_MAP_GRADE");
		case TitleConditionType::KILL_MONSTER_RACE: return TEXT("KILL_MONSTER_RACE");
		case TitleConditionType::KILL_PLAYER: return TEXT("KILL_PLAYER");
		case TitleConditionType::KILL_PLAYER_INTER_SERVER: return TEXT("KILL_PLAYER_INTER_SERVER");
		case TitleConditionType::KILL_TREASURE_MONSTER: return TEXT("KILL_TREASURE_MONSTER");
		case TitleConditionType::SAVE_TREASURE_MONSTER: return TEXT("SAVE_TREASURE_MONSTER");
		case TitleConditionType::LOGIN_ACCUMULATE: return TEXT("LOGIN_ACCUMULATE");
		case TitleConditionType::MARKET_BUY: return TEXT("MARKET_BUY");
		case TitleConditionType::MARKET_BUY_PRICE_ACCUMULATE: return TEXT("MARKET_BUY_PRICE_ACCUMULATE");
		case TitleConditionType::MARKET_SELL: return TEXT("MARKET_SELL");
		case TitleConditionType::MARKET_SELL_PRICE_ACCUMULATE: return TEXT("MARKET_SELL_PRICE_ACCUMULATE");
		case TitleConditionType::MONSTER_KNOWLEDGE_COMPLETE_RACE: return TEXT("MONSTER_KNOWLEDGE_COMPLETE_RACE");
		case TitleConditionType::NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_GRADE: return TEXT("NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_GRADE");
		case TitleConditionType::NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB: return TEXT("NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB");
		case TitleConditionType::NPC_SHOP_BUY_ITEM_ITEM_ID: return TEXT("NPC_SHOP_BUY_ITEM_ITEM_ID");
		case TitleConditionType::PVP_ALIGNMENT: return TEXT("PVP_ALIGNMENT");
		case TitleConditionType::QUEST_COMPLETE_REPEAT_GRADE: return TEXT("QUEST_COMPLETE_REPEAT_GRADE");
		case TitleConditionType::QUEST_COMPLETE_REPEAT_MAP: return TEXT("QUEST_COMPLETE_REPEAT_MAP");
		case TitleConditionType::RANK_ACHIEVEMENT: return TEXT("RANK_ACHIEVEMENT");
		case TitleConditionType::RANK_FIELD_BOSS_CONTRIBUTE: return TEXT("RANK_FIELD_BOSS_CONTRIBUTE");
		case TitleConditionType::RANK_RAID_BOSS_CONTRIBUTE: return TEXT("RANK_RAID_BOSS_CONTRIBUTE");
		case TitleConditionType::RANK_CONTRIBUTE_NPCFUNCTIONTYPE: return TEXT("RANK_CONTRIBUTE_NPCFUNCTIONTYPE");
		case TitleConditionType::RANK_RANGE_ACHIEVEMENT: return TEXT("RANK_RANGE_ACHIEVEMENT");
		case TitleConditionType::RESURRECT_INSTANT_USE: return TEXT("RESURRECT_INSTANT_USE");
		case TitleConditionType::SKILL_ENCHANT_TRY: return TEXT("SKILL_ENCHANT_TRY");
		case TitleConditionType::SKILL_UPGRADE_TRY: return TEXT("SKILL_UPGRADE_TRY");
		case TitleConditionType::SPACE_CRACK_CLEAR: return TEXT("SPACE_CRACK_CLEAR");
		case TitleConditionType::SPACE_CRACK_KILL_MONSTER_GRADE: return TEXT("SPACE_CRACK_KILL_MONSTER_GRADE");
		case TitleConditionType::GUILD_QUEST_COMPLETE_GRADE: return TEXT("GUILD_QUEST_COMPLETE_GRADE");
		case TitleConditionType::KILL_PLAYER_INVASION: return TEXT("KILL_PLAYER_INVASION");
		case TitleConditionType::QUEST_CLEAR: return TEXT("QUEST_CLEAR");
		case TitleConditionType::PROFILE_PROFILE_KEYWORD_GET: return TEXT("PROFILE_PROFILE_KEYWORD_GET");
		case TitleConditionType::ALL_PROFILE_KEYWORD_GET: return TEXT("ALL_PROFILE_KEYWORD_GET");
		case TitleConditionType::EQUIP_ITEM_DELETE_GRADE: return TEXT("EQUIP_ITEM_DELETE_GRADE");
		case TitleConditionType::USE_MANUAL_ATTACK: return TEXT("USE_MANUAL_ATTACK");
		case TitleConditionType::USE_OFFLINE_PLAY: return TEXT("USE_OFFLINE_PLAY");
		case TitleConditionType::PROFILE_INTRODUCTION_INPUT: return TEXT("PROFILE_INTRODUCTION_INPUT");
		case TitleConditionType::INVENTORY_EXTEND: return TEXT("INVENTORY_EXTEND");
		case TitleConditionType::DEPOT_EXTEND: return TEXT("DEPOT_EXTEND");
		case TitleConditionType::EQUIPED_ALL_ITEM_MIN_GRADE: return TEXT("EQUIPED_ALL_ITEM_MIN_GRADE");
		case TitleConditionType::BM_SHOP_BUY_PRODUCT: return TEXT("BM_SHOP_BUY_PRODUCT");
		case TitleConditionType::MULTI_LEVEL_RANK_ACTIVE: return TEXT("MULTI_LEVEL_RANK_ACTIVE");
		case TitleConditionType::KILL_USER_BY_ONE_DAMAGE: return TEXT("KILL_USER_BY_ONE_DAMAGE");
		case TitleConditionType::KILL_HOSTILE_GUILD_USER: return TEXT("KILL_HOSTILE_GUILD_USER");
		case TitleConditionType::INVADE_CURRENCY_ITEM: return TEXT("INVADE_CURRENCY_ITEM");
		case TitleConditionType::ITEM_COLLECTION_COMPLETE: return TEXT("ITEM_COLLECTION_COMPLETE");
		case TitleConditionType::GET_EVENT_PRAYER_EFFECTIVE: return TEXT("GET_EVENT_PRAYER_EFFECTIVE");
		case TitleConditionType::CHECK_ATTENDANCE: return TEXT("CHECK_ATTENDANCE");
		case TitleConditionType::TITLE_HAVE_COUNT: return TEXT("TITLE_HAVE_COUNT");
		case TitleConditionType::KILL_PLAYER_IN_CHAOTIC: return TEXT("KILL_PLAYER_IN_CHAOTIC");
		case TitleConditionType::PK_BOOK_CONTRIBUTE_UP: return TEXT("PK_BOOK_CONTRIBUTE_UP");
		case TitleConditionType::PK_BOOK_CONTRIBUTE_DOWN: return TEXT("PK_BOOK_CONTRIBUTE_DOWN");
		case TitleConditionType::PK_BOOK_KILL: return TEXT("PK_BOOK_KILL");
		case TitleConditionType::PK_BOOK_ASSIST: return TEXT("PK_BOOK_ASSIST");
		case TitleConditionType::PK_BOOK_DIE: return TEXT("PK_BOOK_DIE");
		case TitleConditionType::PK_BOOK_TEASE: return TEXT("PK_BOOK_TEASE");
		case TitleConditionType::DESTROY_HOSTILE_GUILD_CAMP: return TEXT("DESTROY_HOSTILE_GUILD_CAMP");
		case TitleConditionType::BATTLE_PASS_REWARD_NORMAL: return TEXT("BATTLE_PASS_REWARD_NORMAL");
		case TitleConditionType::BATTLE_PASS_REWARD_ADDITIONAL: return TEXT("BATTLE_PASS_REWARD_ADDITIONAL");
		case TitleConditionType::SEAL_NODE_REROLL: return TEXT("SEAL_NODE_REROLL");
		case TitleConditionType::SPIRITSHOT_ENCHANT_TRY: return TEXT("SPIRITSHOT_ENCHANT_TRY");
		case TitleConditionType::SPIRITSHOT_ENCHANT_SUMLEVEL: return TEXT("SPIRITSHOT_ENCHANT_SUMLEVEL");
		case TitleConditionType::SLATE_CODEX_COUNT: return TEXT("SLATE_CODEX_COUNT");
		case TitleConditionType::SANCTUM_VICTORY: return TEXT("SANCTUM_VICTORY");
		case TitleConditionType::SANCTUM_POSSESS: return TEXT("SANCTUM_POSSESS");
		case TitleConditionType::GUILD_ARENA_VICTORY: return TEXT("GUILD_ARENA_VICTORY");
		case TitleConditionType::GUILD_ARENA_DEFEAT: return TEXT("GUILD_ARENA_DEFEAT");
		case TitleConditionType::ITEM_MAGICALFORGE_TRY: return TEXT("ITEM_MAGICALFORGE_TRY");
		case TitleConditionType::ITEM_MAGICALFORGE_GRADE_LEVEL: return TEXT("ITEM_MAGICALFORGE_GRADE_LEVEL");
		case TitleConditionType::CODEX_TOTAL_KNOWLEDGE: return TEXT("CODEX_TOTAL_KNOWLEDGE");
	}
	return FString();
}
inline FString TitleGradeEnumToString(TitleGrade value)
{
	switch (value)
	{
		case TitleGrade::NORMAL: return TEXT("NORMAL");
		case TitleGrade::MAGIC: return TEXT("MAGIC");
		case TitleGrade::RARE: return TEXT("RARE");
		case TitleGrade::EPIC: return TEXT("EPIC");
		case TitleGrade::UNIQUE: return TEXT("UNIQUE");
		case TitleGrade::LEGEND: return TEXT("LEGEND");
	}
	return FString();
}
inline FString TitleEquipSlotEnumToString(TitleEquipSlot value)
{
	switch (value)
	{
		case TitleEquipSlot::FRONT: return TEXT("FRONT");
		case TitleEquipSlot::BACK: return TEXT("BACK");
	}
	return FString();
}
inline FString TitleRetainTypeEnumToString(TitleRetainType value)
{
	switch (value)
	{
		case TitleRetainType::PERMANENT: return TEXT("PERMANENT");
		case TitleRetainType::CONDITIONAL: return TEXT("CONDITIONAL");
	}
	return FString();
}
inline FString TitleConditionCheckTypeEnumToString(TitleConditionCheckType value)
{
	switch (value)
	{
		case TitleConditionCheckType::CREATE_CHARACTER: return TEXT("CREATE_CHARACTER");
		case TitleConditionCheckType::UNLOCK_CONTENTS: return TEXT("UNLOCK_CONTENTS");
	}
	return FString();
}
inline FString TitleStatusEnumToString(TitleStatus value)
{
	switch (value)
	{
		case TitleStatus::DISABLED: return TEXT("DISABLED");
		case TitleStatus::ACTIVED: return TEXT("ACTIVED");
		case TitleStatus::COMPLETED: return TEXT("COMPLETED");
	}
	return FString();
}
inline FString TitleConditionDataTypeEnumToString(TitleConditionDataType value)
{
	switch (value)
	{
		case TitleConditionDataType::NONE: return TEXT("NONE");
		case TitleConditionDataType::COSTUME: return TEXT("COSTUME");
		case TitleConditionDataType::ITEM: return TEXT("ITEM");
		case TitleConditionDataType::QUEST: return TEXT("QUEST");
		case TitleConditionDataType::RANK: return TEXT("RANK");
		case TitleConditionDataType::CREATURE: return TEXT("CREATURE");
		case TitleConditionDataType::CURRENCY: return TEXT("CURRENCY");
		case TitleConditionDataType::GUILD: return TEXT("GUILD");
		case TitleConditionDataType::FAIRY: return TEXT("FAIRY");
		case TitleConditionDataType::SKILL: return TEXT("SKILL");
		case TitleConditionDataType::ETC: return TEXT("ETC");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString TitleConditionTypeEnumToDisplayKey(TitleConditionType value)
{
	switch (value)
	{
		case TitleConditionType::NONE: return TEXT("");
		case TitleConditionType::CHARACTER_LEVEL: return TEXT("");
		case TitleConditionType::COSTUME_GET_ID: return TEXT("");
		case TitleConditionType::COSTUME_SUMMON_GRADE: return TEXT("");
		case TitleConditionType::COSTUME_COMPOSE_GRADE: return TEXT("");
		case TitleConditionType::FAIRY_GET_ID: return TEXT("");
		case TitleConditionType::FAIRY_SUMMON_GRADE: return TEXT("");
		case TitleConditionType::FAIRY_COMPOSE_GRADE: return TEXT("");
		case TitleConditionType::FAIRY_ENCHANT: return TEXT("");
		case TitleConditionType::FAIRY_ENCHANT_FAIL: return TEXT("");
		case TitleConditionType::FAIRY_ENCHANT_SUCCESS: return TEXT("");
		case TitleConditionType::BM_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB: return TEXT("");
		case TitleConditionType::CURRENCY_ACCUMULATE_CONSUME: return TEXT("");
		case TitleConditionType::CURRENCY_ACCUMULATE_GET: return TEXT("");
		case TitleConditionType::DEATH_BY_MONSTER: return TEXT("");
		case TitleConditionType::DEATH_BY_PLAYER: return TEXT("");
		case TitleConditionType::DEATH_BY_PLAYER_MONITORING_USER: return TEXT("");
		case TitleConditionType::FRIEND_MAKE: return TEXT("");
		case TitleConditionType::GUILD_ATTENDANCE: return TEXT("");
		case TitleConditionType::GUILD_CONTRIBUTE: return TEXT("");
		case TitleConditionType::GUILD_DONATE_CURRENCY_ACCUMULATE: return TEXT("");
		case TitleConditionType::GUILD_DUNGEON_CLEAR: return TEXT("");
		case TitleConditionType::GUILD_ITEM_COLLECTION_REGISTER: return TEXT("");
		case TitleConditionType::GUILD_MEMBER_GRADE_LEVEL: return TEXT("");
		case TitleConditionType::GUILD_QUEST_COMPLETE: return TEXT("");
		case TitleConditionType::ITEM_ACCUMULATE_USE: return TEXT("");
		case TitleConditionType::ITEM_ACCUMULATE_USE_CATEGORY_GRADE: return TEXT("");
		case TitleConditionType::ITEM_ACCUMULATE_USE_CATEGORY_SUB: return TEXT("");
		case TitleConditionType::ITEM_CRAFT_FAIL_CATEGORY_MAIN_SUB: return TEXT("");
		case TitleConditionType::ITEM_CRAFT_GREATE_SUCCESS_CATEGORY: return TEXT("");
		case TitleConditionType::ITEM_CRAFT_GREATE_SUCCESS_TYPE_GRADE: return TEXT("");
		case TitleConditionType::ITEM_CRAFT_SUCCESS_CATEGORY_GRADE: return TEXT("");
		case TitleConditionType::ITEM_CRAFT_SUCCESS_SUB_CATEGORY: return TEXT("");
		case TitleConditionType::ITEM_CRAFT_TRY_CATEGORY: return TEXT("");
		case TitleConditionType::ITEM_CRAFT_TRY_TYPE_GRADE: return TEXT("");
		case TitleConditionType::ITEM_DECOMPOSE_TRY: return TEXT("");
		case TitleConditionType::ITEM_ENCHANT_FAIL_CATEGORY: return TEXT("");
		case TitleConditionType::ITEM_ENCHANT_SUCCESS_CATEGORY_GRADE: return TEXT("");
		case TitleConditionType::ITEM_ENCHANT_SUCCESS_LEVEL_SUB: return TEXT("");
		case TitleConditionType::ITEM_LOOT_GRADE_CATEGORY: return TEXT("");
		case TitleConditionType::ITEM_REFINE: return TEXT("");
		case TitleConditionType::KILL_MONSTER_GRADE_ID: return TEXT("");
		case TitleConditionType::KILL_MONSTER_MAP_GRADE: return TEXT("");
		case TitleConditionType::KILL_MONSTER_RACE: return TEXT("");
		case TitleConditionType::KILL_PLAYER: return TEXT("");
		case TitleConditionType::KILL_PLAYER_INTER_SERVER: return TEXT("");
		case TitleConditionType::KILL_TREASURE_MONSTER: return TEXT("");
		case TitleConditionType::SAVE_TREASURE_MONSTER: return TEXT("");
		case TitleConditionType::LOGIN_ACCUMULATE: return TEXT("");
		case TitleConditionType::MARKET_BUY: return TEXT("");
		case TitleConditionType::MARKET_BUY_PRICE_ACCUMULATE: return TEXT("");
		case TitleConditionType::MARKET_SELL: return TEXT("");
		case TitleConditionType::MARKET_SELL_PRICE_ACCUMULATE: return TEXT("");
		case TitleConditionType::MONSTER_KNOWLEDGE_COMPLETE_RACE: return TEXT("");
		case TitleConditionType::NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_GRADE: return TEXT("");
		case TitleConditionType::NPC_SHOP_BUY_ITEM_CATEGORY_MAIN_SUB: return TEXT("");
		case TitleConditionType::NPC_SHOP_BUY_ITEM_ITEM_ID: return TEXT("");
		case TitleConditionType::PVP_ALIGNMENT: return TEXT("");
		case TitleConditionType::QUEST_COMPLETE_REPEAT_GRADE: return TEXT("");
		case TitleConditionType::QUEST_COMPLETE_REPEAT_MAP: return TEXT("");
		case TitleConditionType::RANK_ACHIEVEMENT: return TEXT("");
		case TitleConditionType::RANK_FIELD_BOSS_CONTRIBUTE: return TEXT("");
		case TitleConditionType::RANK_RAID_BOSS_CONTRIBUTE: return TEXT("");
		case TitleConditionType::RANK_CONTRIBUTE_NPCFUNCTIONTYPE: return TEXT("");
		case TitleConditionType::RANK_RANGE_ACHIEVEMENT: return TEXT("");
		case TitleConditionType::RESURRECT_INSTANT_USE: return TEXT("");
		case TitleConditionType::SKILL_ENCHANT_TRY: return TEXT("");
		case TitleConditionType::SKILL_UPGRADE_TRY: return TEXT("");
		case TitleConditionType::SPACE_CRACK_CLEAR: return TEXT("");
		case TitleConditionType::SPACE_CRACK_KILL_MONSTER_GRADE: return TEXT("");
		case TitleConditionType::GUILD_QUEST_COMPLETE_GRADE: return TEXT("");
		case TitleConditionType::KILL_PLAYER_INVASION: return TEXT("");
		case TitleConditionType::QUEST_CLEAR: return TEXT("");
		case TitleConditionType::PROFILE_PROFILE_KEYWORD_GET: return TEXT("");
		case TitleConditionType::ALL_PROFILE_KEYWORD_GET: return TEXT("");
		case TitleConditionType::EQUIP_ITEM_DELETE_GRADE: return TEXT("");
		case TitleConditionType::USE_MANUAL_ATTACK: return TEXT("");
		case TitleConditionType::USE_OFFLINE_PLAY: return TEXT("");
		case TitleConditionType::PROFILE_INTRODUCTION_INPUT: return TEXT("");
		case TitleConditionType::INVENTORY_EXTEND: return TEXT("");
		case TitleConditionType::DEPOT_EXTEND: return TEXT("");
		case TitleConditionType::EQUIPED_ALL_ITEM_MIN_GRADE: return TEXT("");
		case TitleConditionType::BM_SHOP_BUY_PRODUCT: return TEXT("");
		case TitleConditionType::MULTI_LEVEL_RANK_ACTIVE: return TEXT("");
		case TitleConditionType::KILL_USER_BY_ONE_DAMAGE: return TEXT("");
		case TitleConditionType::KILL_HOSTILE_GUILD_USER: return TEXT("");
		case TitleConditionType::INVADE_CURRENCY_ITEM: return TEXT("");
		case TitleConditionType::ITEM_COLLECTION_COMPLETE: return TEXT("");
		case TitleConditionType::GET_EVENT_PRAYER_EFFECTIVE: return TEXT("");
		case TitleConditionType::CHECK_ATTENDANCE: return TEXT("");
		case TitleConditionType::TITLE_HAVE_COUNT: return TEXT("");
		case TitleConditionType::KILL_PLAYER_IN_CHAOTIC: return TEXT("");
		case TitleConditionType::PK_BOOK_CONTRIBUTE_UP: return TEXT("");
		case TitleConditionType::PK_BOOK_CONTRIBUTE_DOWN: return TEXT("");
		case TitleConditionType::PK_BOOK_KILL: return TEXT("");
		case TitleConditionType::PK_BOOK_ASSIST: return TEXT("");
		case TitleConditionType::PK_BOOK_DIE: return TEXT("");
		case TitleConditionType::PK_BOOK_TEASE: return TEXT("");
		case TitleConditionType::DESTROY_HOSTILE_GUILD_CAMP: return TEXT("");
		case TitleConditionType::BATTLE_PASS_REWARD_NORMAL: return TEXT("");
		case TitleConditionType::BATTLE_PASS_REWARD_ADDITIONAL: return TEXT("");
		case TitleConditionType::SEAL_NODE_REROLL: return TEXT("");
		case TitleConditionType::SPIRITSHOT_ENCHANT_TRY: return TEXT("");
		case TitleConditionType::SPIRITSHOT_ENCHANT_SUMLEVEL: return TEXT("");
		case TitleConditionType::SLATE_CODEX_COUNT: return TEXT("");
		case TitleConditionType::SANCTUM_VICTORY: return TEXT("");
		case TitleConditionType::SANCTUM_POSSESS: return TEXT("");
		case TitleConditionType::GUILD_ARENA_VICTORY: return TEXT("");
		case TitleConditionType::GUILD_ARENA_DEFEAT: return TEXT("");
		case TitleConditionType::ITEM_MAGICALFORGE_TRY: return TEXT("");
		case TitleConditionType::ITEM_MAGICALFORGE_GRADE_LEVEL: return TEXT("");
		case TitleConditionType::CODEX_TOTAL_KNOWLEDGE: return TEXT("");
	}
	return FString();
}
inline FString TitleGradeEnumToDisplayKey(TitleGrade value)
{
	switch (value)
	{
		case TitleGrade::NORMAL: return TEXT("");
		case TitleGrade::MAGIC: return TEXT("");
		case TitleGrade::RARE: return TEXT("");
		case TitleGrade::EPIC: return TEXT("");
		case TitleGrade::UNIQUE: return TEXT("");
		case TitleGrade::LEGEND: return TEXT("");
	}
	return FString();
}
inline FString TitleEquipSlotEnumToDisplayKey(TitleEquipSlot value)
{
	switch (value)
	{
		case TitleEquipSlot::FRONT: return TEXT("");
		case TitleEquipSlot::BACK: return TEXT("");
	}
	return FString();
}
inline FString TitleRetainTypeEnumToDisplayKey(TitleRetainType value)
{
	switch (value)
	{
		case TitleRetainType::PERMANENT: return TEXT("");
		case TitleRetainType::CONDITIONAL: return TEXT("");
	}
	return FString();
}
inline FString TitleConditionCheckTypeEnumToDisplayKey(TitleConditionCheckType value)
{
	switch (value)
	{
		case TitleConditionCheckType::CREATE_CHARACTER: return TEXT("");
		case TitleConditionCheckType::UNLOCK_CONTENTS: return TEXT("");
	}
	return FString();
}
inline FString TitleStatusEnumToDisplayKey(TitleStatus value)
{
	switch (value)
	{
		case TitleStatus::DISABLED: return TEXT("");
		case TitleStatus::ACTIVED: return TEXT("");
		case TitleStatus::COMPLETED: return TEXT("");
	}
	return FString();
}
inline FString TitleConditionDataTypeEnumToDisplayKey(TitleConditionDataType value)
{
	switch (value)
	{
		case TitleConditionDataType::NONE: return TEXT("");
		case TitleConditionDataType::COSTUME: return TEXT("");
		case TitleConditionDataType::ITEM: return TEXT("");
		case TitleConditionDataType::QUEST: return TEXT("");
		case TitleConditionDataType::RANK: return TEXT("");
		case TitleConditionDataType::CREATURE: return TEXT("");
		case TitleConditionDataType::CURRENCY: return TEXT("");
		case TitleConditionDataType::GUILD: return TEXT("");
		case TitleConditionDataType::FAIRY: return TEXT("");
		case TitleConditionDataType::SKILL: return TEXT("");
		case TitleConditionDataType::ETC: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidTitleConditionType(TitleConditionType value) noexcept
{
	return (value >= TitleConditionType::NONE && value < TitleConditionType::MAX);
}

inline bool IsValidTitleGrade(TitleGrade value) noexcept
{
	return (value >= TitleGrade::NORMAL && value < TitleGrade::MAX);
}

inline bool IsValidTitleEquipSlot(TitleEquipSlot value) noexcept
{
	return (value >= TitleEquipSlot::FRONT && value < TitleEquipSlot::MAX);
}

inline bool IsValidTitleRetainType(TitleRetainType value) noexcept
{
	return (value >= TitleRetainType::PERMANENT && value < TitleRetainType::MAX);
}

inline bool IsValidTitleConditionCheckType(TitleConditionCheckType value) noexcept
{
	return (value >= TitleConditionCheckType::CREATE_CHARACTER && value < TitleConditionCheckType::MAX);
}

inline bool IsValidTitleStatus(TitleStatus value) noexcept
{
	return (value >= TitleStatus::DISABLED && value < TitleStatus::MAX);
}

inline bool IsValidTitleConditionDataType(TitleConditionDataType value) noexcept
{
	return (value >= TitleConditionDataType::NONE && value < TitleConditionDataType::MAX);
}


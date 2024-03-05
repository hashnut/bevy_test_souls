#pragma once
/*
	언락 define
*/
UENUM(BlueprintType)
enum class EGsUnlockContentsType : uint8
{
	NONE = 0,

	ACHIEVEMENT UMETA(Tooltip = "업적"),
	AUTO_ATTACK UMETA(Tooltip = "자동 사냥"),
	ARENA UMETA(Tooltip = "대결"),

	BMSHOP UMETA(Tooltip = "BM 상점"),
	BMSHOP_STORAGE UMETA(Tooltip = "BM 상점 보관함"),
	BOSS UMETA(Tooltip = "보스"),
	BUDDY UMETA(Tooltip = "친구"),

	COMMUNITY UMETA(Tooltip = "커뮤니티 소녀"),
	COSTUME UMETA(Tooltip = "코스튬"),
	CRAFT UMETA(Tooltip = "제작"),
	CAMP UMETA(Tooltip = "캠프 설치"),

	DEPOT UMETA(Tooltip = "창고"),
	DUNGEON UMETA(Tooltip = "던전"),
	DEATH_PANELTY UMETA(Tooltip = "사망 페널티"),
	DELIVERY_BOX UMETA(Tooltip = "재합성 및 복구 기능"),
	DECOMPOSE UMETA(Tooltip = "아이템 분해"),

	FAIRY UMETA(Tooltip = "페어리"),

	GAME_STATISTICS UMETA(Tooltip = "통계"),
	GUILD UMETA(Tooltip = "기사단"),
	GUILD_DUNGEON_WATCH UMETA(Tooltip = "기사단 던전 관전 기능"),
	GUILD_MAIN_TAB UMETA(Tooltip = "기사단 메인"),
	GUILD_MEMBER_TAB UMETA(Tooltip = "기사단 단원"),
	GUILD_DUNGEON_TAB UMETA(Tooltip = "기사단 던전"),
	GUILD_QUEST_TAB UMETA(Tooltip = "기사단 퀘스트"),
	GUILD_AGIT_TAB UMETA(Tooltip = "기사단 영지"),
	GUILD_BUILDING_TAB UMETA(Tooltip = "기사단 운영"),
	GUILD_MANAGEMENT_TAB UMETA(Tooltip = "기사단 관리"),
	GUILD_DIVISION_TAB UMETA(Tooltip = "기사단 분배"),
	GUILD_AUCTION_TAB UMETA(Tooltip = "기사단 경매"),
	GUILD_MEMBER_BAN UMETA(Tooltip = "기사단 단원 추방"),

	INVENTORY UMETA(Tooltip = "인벤토리"),
	ITEM_COLLECTION UMETA(Tooltip = "아이템 컬렉션"),
	ITEM_ENCHANT UMETA(Tooltip = "아이템 강화"),
	ITEM_REFINE UMETA(Tooltip = "아이템 재련"),
	ITEM_MAGICAL_FORGE UMETA(Tooltip = "아이템 마법 부여"),
	INTERSERVER UMETA(Tooltip = "인터서버"),
	INVADE_ENTER UMETA(Tooltip = "침공전 포탈 입장"),
	INVADE_INFO UMETA(Tooltip = "침공전 정보"),

	MAIL UMETA(Tooltip = "우편"),

	MARKET UMETA(Tooltip = "거래소"),
	MARKET_SELL UMETA(Tooltip = "거래소 판매"),
	MARKET_WORLD UMETA(Tooltip = "월드 거래소"),
	MARKET_WORLD_SELL UMETA(Tooltip = "월드 거래소 판매"),

	MONSTER_KNOWLEDGE_COLLECTION UMETA(Tooltip = "도감"),
	MULTI_LEVEL_RANK UMETA(Tooltip = "승급"),
	MOVE_TO_OTHER_REGION UMETA(Tooltip = "타 지역 이동"),

	NPC_SHOP UMETA(Tooltip = "NPC상점"),

	OFFLINE_PLAY UMETA(Tooltip = "오프라인 플레이"),
	OPTION UMETA(Tooltip = "옵션"),

	PK_BOOK UMETA(Tooltip = "PK북"),
	PARTY_CREATE_OR_JOIN UMETA(Tooltip = "파티 생성 및 참가"),
	PARTY_WARP_TO_LEADER UMETA(Tooltip = "파티장에게 이동"),
	PARTY_NO_PARTY_INVITES UMETA(Tooltip = "파티 초대 금지"),
	PERSONAL_TRADING UMETA(Tooltip = "개인 거래"),
	PRAYER UMETA(Tooltip = "기도사"),
	
	PRESET UMETA(Tooltip = "프리셋"),
	PRESET_FIRST UMETA(Tooltip = "프리셋 슬롯 1"),
	PRESET_SECOND UMETA(Tooltip = "프리셋 슬롯 2"),
	PRESET_THIRD UMETA(Tooltip = "프리셋 슬롯 3"),

	QUEST UMETA(Tooltip = "퀘스트"),
	QUEST_SUB UMETA(Tooltip = "서브 퀘스트"),
	QUEST_GUIDE UMETA(Tooltip = "가이드 퀘스트"),
	QUEST_REPEAT UMETA(Tooltip = "반복 퀘스트"),
	QUEST_MULTI_LEVEL_RANK UMETA(Tooltip = "승급 퀘스트"),
	QUEST_BOARD UMETA(Tooltip = "퀘스트 게시판"),
	VILLAGE_QUEST_BOARD UMETA(Tooltip = "퀘스트 게시판 마을"),
	INVADE_QUEST_BOARD UMETA(Tooltip = "퀘스트 게시판 침공전"),

	RANKING UMETA(Tooltip = "랭킹"),
	REVIVE UMETA(Tooltip = "경험치복구"),
	RETURN_SCROLL UMETA(Tooltip = "귀환 주문서"),
	
	SKILL UMETA(Tooltip = "스킬"),
	SAVE_BATTERY UMETA(Tooltip = "절전 모드"),
	STAT_DISTRIBUTION UMETA(Tooltip = "스텟 분배"),
	SPACE_CRACK_REWARD UMETA(Tooltip = "시공의 틈새 보상 요청"),
	
	TITLE UMETA(Tooltip = "칭호"),
	TELEPORT UMETA(Tooltip = "위치 즐겨찾기"),

	USER_PROFILE UMETA(Tooltip = "프로필"),
	
	WORLDMAP UMETA(Tooltip = "월드맵"),
	WORLD_BOSS UMETA(Tooltip = "월드 보스"),
	CONTENTS_WORLD_BOSS UMETA(Tooltip = "월드 보스"),

	SEAL UMETA(Tooltip = "봉인"),

	NORMAL_DUNGEON UMETA(Tooltip = "일반 던전"),
	SPECIAL_DUNGEON UMETA(Tooltip = "특수 던전"),
	PARTY_DUNGEON UMETA(Tooltip = "파티 던전"),
	GUILD_DUNGEON UMETA(Tooltip = "길드 던전"),
	ELITE_DUNGEON UMETA(Tooltip = "정예 던전"),
	DUNGEON_TIME_CHARGE UMETA(Tooltip = "던전 시간 충전"),
	
	PARTY_DUNGEON_01,
	PARTY_DUNGEON_02,
	PARTY_DUNGEON_03,
	CONTENTS_DUNGEON_GROUP,

	BATTLEPASS UMETA(Tooltip = "배틀패스"),
	SIEGE_WARFARE UMETA(Tooltip = "공성전"),

	HALL_OF_TRIALS_CANNOT_ENTER UMETA("시련의 전당 입장 불가"),
	HALL_OF_TRIALS_1F_CANNOT_USE UMETA("시련의 전당 1층"),
	HALL_OF_TRIALS_2F_CANNOT_USE UMETA("시련의 전당 2층"),
	HALL_OF_TRIALS_3F_CANNOT_USE UMETA("시련의 전당 3층"),
	HALL_OF_TRIALS_4F_CANNOT_USE UMETA("시련의 전당 4층"),
	HALL_OF_TRIALS_5F_CANNOT_USE UMETA("시련의 전당 5층"),
	HALL_OF_TRIALS_6F_CANNOT_USE UMETA("시련의 전당 6층"),
	HALL_OF_TRIALS_7F_CANNOT_USE UMETA("시련의 전당 7층"),

	PARTY_DUNGEON_HIDDEN UMETA("파티 던전 Hidden"),

	SPIRIT_SHOT UMETA(Tooltip = "천마석"),

	ITEM_FUSITION UMETA(Tooltip = "아이템 합성"),

	ARENA_DUNGEON_HIDDEN UMETA("Arena 던전 Hidden"),

	AGIT_DUNGEON_RAID UMETA("길드 영지 공략 던전"),

	BMSHOP_SERVER_MIGRATE UMETA("상점 서버 이전"),

	BATTLE_ARENA_GUILD_WAR_REGISTER UMETA("그림자 전장 신청"),
	
	BATTLE_ARENA_GUILD_WAR_ENTER UMETA("그림자 전장 신청"),
};
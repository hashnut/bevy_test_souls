#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedItemEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 아이템 타입(대분류) - 참고로 ItemCategory가 소분류에 해당
enum class ItemType : uint8
{

	// 에러값
	NONE = 0,

	// 장비
	EQUIP = 1,

	// 재료 - 사용성이 있을수도 / 없을수도 있음
	INGREDIENT = 2,

	// 소모품 - 사용성 있음
	CONSUME = 3,

	// 재화
	CURRENCY = 4,

	// 기타 - 뭘까? 일단 만들어두긴 했는데...
	ETC = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// 아이템 대분류, (수정 시 CommonConfig의 itemCategoryValidationList 도 수정 필요)
enum class ItemCategoryMain : uint8
{

	// 에러값
	NONE = 0,

	// 무기
	WEAPON = 1,

	// 방어구
	ARMOR = 2,

	// 장신구
	ACCESSORY = 3,

	// 물약
	POTION = 4,

	// 스킬북 - 스킬 배울 때 사용
	SKILLBOOK = 5,

	// 주문서
	SCROLL = 6,

	// 음식
	FOOD = 7,

	// 페어리
	FAIRY = 8,

	// 코스튬
	COSTUME = 9,

	// 제작
	CRAFT = 10,

	// 퀘스트
	QUEST = 11,

	// 티켓
	TICKET = 12,

	// 몬스터 도감
	MONSTER_KNOWLEDGE = 13,

	// 지식의 석판
	MONSTER_KNOWLEDGE_SLATE = 14,

	// 수집 가능 아이템
	COLLECTABLE = 15,

	// 충전
	CHARGE = 16,

	// 재화
	CURRENCY = 17,

	// 기타
	ETC = 18,

	// 복구 이벤트 상자
	DELIVERY_BOX = 19,

	// 봉인(문양)
	SEAL = 20,

	// 유료 장비
	SPECIAL = 21,

	MAX = 22,
};

UENUM(BlueprintType)
// 아이템 소분류, (수정 시 CommonConfig의 itemCategoryValidationList 도 수정 필요)
enum class ItemCategorySub : uint8
{

	// 에러값
	NONE = 0,

	// 착용성 아이템 (1~100)

	// 대검
	SWORD = 1,

	// 단검
	DAGGER = 2,

	// 활
	BOW = 3,

	// 지팡이
	WAND = 4,

	// 무기_오브
	WAEPON_ORB = 5,

	// 샷건
	SHOTGUN = 6,

	// 투구 - 머리
	HELMET = 7,

	// 갑옷 - 상의
	BODY_ARMOR = 8,

	// 장갑 - 손
	GLOVE = 9,

	// 신발 - 발
	SHOES = 10,

	// 벨트 - 허리
	BELT = 11,

	// 스카프 - 목
	SCARF = 12,

	// 목걸이
	NECKLACE = 13,

	// 귀걸이
	EAR_RING = 14,

	// 반지
	RING = 15,

	// 참
	CHARM = 16,

	// 오브 - ItemCategoryEquipSlot:ORB_1 대응
	ORB = 17,

	// 오브 2 - ItemCategoryEquipSlot:ORB_2 대응
	ORB_2 = 18,

	// 팔찌
	BRACELET = 19,

	// 듀랄
	DURAL = 20,

	// 마블
	MARBLE = 21,

	// 착용성 아이템 최대 값
	MAX_EQUIP = 22,

	// 소모성 아이템 (101~200)

	// 각종 물약

	// 체력 회복 포션
	HP_POTION = 101,

	// 수동 사용 체력 회복 포션 (예시: 상급 물약)
	HP_POTION_MANUAL = 102,

	// MP 회복 틱 증가
	MP_TICK_RECOVERY_POTION = 103,

	// MP 회복 틱 증가
	MP_POTION = 104,

	// 버프 물약(예시: 가속 물약)
	POTION_BUFF = 105,

	// 보너스 스탯 포인트 물약
	BONUS_STAT_POINT = 106,

	// 각종 스킬 북

	// 축복 스킬북
	COMMON_SKILL_BOOK = 107,

	// 대검 스킬북
	SWORD_SKILL_BOOK = 108,

	// 단검 스킬북
	DAGGER_SKILL_BOOK = 109,

	// 활 스킬북
	BOW_SKILL_BOOK = 110,

	// 지팡이 스킬북
	WAND_SKILL_BOOK = 111,

	// 오브 스킬북
	ORB_SKILL_BOOK = 112,

	// 샷건 스킬북
	SHOTGUN_SKILL_BOOK = 113,

	// 각종 주문서

	// 스킬 강화 주문서
	ENCHANT_SKILL_SCROLL = 114,

	// 귀환 주문서
	WARP_TO_TOWN_SCROLL = 115,

	// 축복받은 순간이동 주문서
	WARP_TO_BOOKMARK_SCROLL = 116,

	// 랜덤 순간이동 주문서
	WARP_TO_RANDOM_SCROLL = 117,

	// 영지 이동 주문서
	WARP_TO_AGIT = 118,

	// 강화 주문서

	// 무기 강화 주문서
	ENCHANT_WEAPON_SCROLL = 119,

	// 축복받은 무기 강화 주문서
	BLESSED_ENCHANT_WEAPON = 120,

	// 저주받은 무기 강화 주문서
	CURSED_ENCHANT_WEAPON = 121,

	// 방어구 강화 주문서
	ENCHANT_ARMOR = 122,

	// 축복받은 방어구 강화 주문서
	BLESSED_ENCHANT_ARMOR = 123,

	// 저주받은 방어구 강화 주문서
	CURSED_ENCHANT_ARMOR = 124,

	// 장신구 강화 주문서
	ENCHANT_ACCESSORY = 125,

	// 축복받은 장신구 강화 주문서
	BLESSED_ENCHANT_ACCESSORY = 126,

	// 저주받은 장신구 강화 주문서
	CURSED_ENCHANT_ACCESSORY = 127,

	// 기타 주문서

	// 버프 스크롤
	SCROLL_BUFF = 128,

	// 버프 조각상
	STATUE_BUFF = 129,

	// 제련석
	OPTION_STONE = 130,

	// 빛나는 제련석
	BLESSED_OPTION_STONE = 131,

	// 분배 스탯 초기화 주문서
	RESET_BONUS_STAT = 132,

	// 마법부여 주문서
	MAGICAL_FORGE = 133,

	// 스킬 발동 주문서
	SKILL_SCROLL = 134,

	// 음식

	// 음식 - 1차 스탯(STAT_ORDER_1 중 기초스탯)만 상승
	FOOD_BUFF = 135,

	// 페어리 관련

	// 페어리 소환석
	SUMMON_FAIRY = 136,

	// 페어리 성장 재료
	ENCHANT_FAIRY = 137,

	// 페어리 천장 재료
	FAIRY_CEILING = 138,

	// 코스튬 관련

	// 코스튬 소환
	SUMMON_COSTUME = 139,

	// 코스튬 천장 재료
	COSTUME_CEILING = 140,

	// 코스튬 교환권(랜덤)
	CHANGE_COSTUME_RANDOM = 141,

	// 코스튬 교환권(선택)
	CHANGE_COSTUME_SELECT = 142,

	// 제작 관련

	// 제작 - 장비아이템 분해 결과 재료
	INGREDIENT_CRAFT = 143,

	// 제작 - 기초 재료 (가죽, 철, 보석, 천, 나무)
	MATERIALS = 144,

	// 제작 - 정수 (보스 정수, 종족 정수)
	ESSENCE = 145,

	// 제작 - 장비 제작 재료 (레시피, 레시피 조각)
	RECIPE = 146,

	// 제작 - 스킬 제작 재료 (잉크, 안료)
	INK = 147,

	// 제작 - 신비가루, 빛나는 신비가루
	MYSTERIOUS_POWDER = 148,

	// 퀘스트 관련

	// 반복 퀘스트 획득 스크롤
	QUEST_REPEAT = 149,

	// 티켓 관련

	// 기사단 퀘스트 즉시 완료 티켓
	TICKET_QUEST_GUILD_KNIGHT_COMPLETE = 150,

	// 캐릭터 슬롯 언락 티켓
	TICKET_CHARACTER_SLOT_UNLOCK = 151,

	// 캐릭터 커스터마이징 변경 티켓
	TICKET_CHARACTER_CUSTOMIZING = 152,

	// 몬스터 도감 관련

	// 몬스터 지식 증가
	MONSTER_KNOWLEDGE_GAIN_EXP = 153,

	// 지식의 석판 관련

	// 지식의 석판 - 공격 테마
	MONSTER_KNOWLEDGE_SLATE_UNLOCK_ATTACK = 154,

	// 지식의 석판 - 적중 테마
	MONSTER_KNOWLEDGE_SLATE_UNLOCK_HIT = 155,

	// 지식의 석판 - 증폭 테마
	MONSTER_KNOWLEDGE_SLATE_UNLOCK_AMPLIFICATION = 156,

	// 지식의 석판 - 방어 테마
	MONSTER_KNOWLEDGE_SLATE_UNLOCK_DEFENCE = 157,

	// 지식의 석판 - 저항 테마
	MONSTER_KNOWLEDGE_SLATE_UNLOCK_RESISTANCE = 158,

	// 지식의 석판 - 생존 테마
	MONSTER_KNOWLEDGE_SLATE_UNLOCK_SURVIVAL = 159,

	// 지식의 석판 초기화
	MONSTER_KNOWLEDGE_SLATE_RESET = 160,

	// 기타

	// 장비 분해 소모품 - 인게임에서 사용 안하도록 수정추후 삭제 예정).
	DECOMPOSE_EQUIP_KIT = 161,

	// 스킬북 분해 소모품 - 인게임에서 사용 안하도록 수정추후 삭제 예정) .
	DECOMPOSE_SKILLBOOK_KIT = 162,

	// 아이템 박스
	REWARD_BOX = 163,

	// 화살 (원거리 클래스 공격 시 소모 재료)
	ARROW = 164,

	// 천마석(L2M의 정령탄)
	SPIRIT_SHOT = 165,

	// 마법석 (마법 클래스 공격 시 소모 재료)
	MAGIC_STONE = 166,

	// 특정 스킬 사용 시 소모품(유사: L2M의 마정석)
	SPELL_STONE = 167,

	// 아르카스
	ARCAS = 168,

	// PVP성향(카오틱) 감소
	PVP_ALIGNMENT = 169,

	// 임시 치트 발동용 아이템
	CHEAT = 170,

	// 보물 몬스터 소환
	SPAWN_TREASURE_MONSTER = 171,

	// 기사단 선물 상자
	GUILD_GIFT_BOX = 172,

	// 캠프 설치
	SPAWN_CAMP = 173,

	// 신비가루로 제작하는 수집품
	GODS_BLESS = 174,

	// 수집품 강화 주문서
	ENCHANT_COLLECTABLE_SCROLL = 175,

	// 고레벨필드던전 입장권
	ELITE_DUNGEON_TICKET = 176,

	// 던전 시간
	DUNGEON_TIME = 177,

	// 침공 포탈 정수
	INVADE_PORTAL = 178,

	// 몬스터 소환
	SPAWN_MONSTER = 179,

	// 유저 이모션 활성화 아이템
	SOCIAL_EMOTION = 180,

	// 출석 보상 해금 열쇠
	ATTENDANCE_OPEN_KEY = 181,

	// 서브 퀘스트 보상 컬렉션 전용 아이템
	SQ_STAMP = 182,

	// 보스 컬랙션
	BOSS_COLLECTION = 183,

	// 재화형아이템

	// 칭공의 증표
	INVADE_CURRENCY = 184,

	// 신규 이벤트용 재화
	EVENT_CURRENCY = 185,

	// 칭공신규, 젠, 기사단 주화, 시련의 결정, 바실의 은총, 격전의 증표 등 기본적으로 사용하는 재화
	NORMAL_CURRENCY = 186,

	// 신규, 코스튬 인장, 세계수의 인장, 의지의 주화 같은 특별하거나 BM 이용 시 획득 가능 재화
	COIN_CURRENCY = 187,

	// BM상점쿠폰아이템
	DISCOUNT_COUPON = 188,

	// 봉인석(문양)
	SEAL_STONE = 189,

	// 파티던전 열쇠
	PARTYDUNGEON_KEY = 190,

	// 기타
	ETC = 191,

	// 이벤트 관련

	// 복구 이벤트 상자 - 장비
	DELIVERY_BOX_EQUIP = 201,

	// 복구 이벤트 상자 - 코스튬
	DELIVERY_BOX_COSTUME = 202,

	// 복구 이벤트 상자 - 페어리
	DELIVERY_BOX_FAIRY = 203,

	// 사망 패널티 복구

	// 사망 패널티 복구 아이템
	DEATH_PENALTY_RECOVERY = 220,

	// 고레벨필드던전 보주
	ELITE_DUNGEON_BALL = 221,

	// 캐릭터 이름 변경권
	NICKNAME_CHANGE = 222,

	MAX = 223,
};

UENUM(BlueprintType)
// 아이템 등급
enum class ItemGrade : uint8
{

	// 에러값
	NONE = 0,

	// 등급 - NORMAL
	NORMAL = 1,

	// 등급 - MAGIC
	MAGIC = 2,

	// 등급 - RARE
	RARE = 3,

	// 등급 - EPIC
	EPIC = 4,

	// 등급 - UNIQUE
	UNIQUE = 5,

	// 등급 - LEGEND
	LEGEND = 6,

	// 등급 - ETERNAL
	ETERNAL = 7,

	MAX = 8,
};

UENUM(BlueprintType)
// 아이템 재질
enum class ItemMaterial : uint8
{

	// 천
	FABRIC = 0,

	// 나무
	WOOD = 1,

	// 뼈
	BONE = 2,

	// 가죽
	LEATHER = 3,

	// 철
	STEEL = 4,

	// 아다만티움
	ADAMATIUM = 5,

	// 미스릴
	MITHRIL = 6,

	// 보석
	JEWEL = 7,

	// 젠나이트
	ZENITE = 8,

	// 트루실버
	TRUE_SILVER = 9,

	// 오리하르콘
	ORICALCUM = 10,

	// 갑각
	SHELL = 11,

	// 없음 - 귀신이나 영혼체 재질
	SPIRIT = 12,

	// 석재(돌)
	STONE = 13,

	// 청동
	BRONZE = 14,

	// 토륨
	THORIUM = 15,

	MAX = 16,
};

UENUM(BlueprintType)
// 아이템 사용 시 효과 타입
enum class ItemEffectType : uint8
{

	// 효과 없음 - 에러 아님
	NONE = 0,

	// 특정 스탯을 즉시 회복
	RESTORE_STAT = 1,

	// ABNORMALITY를 이용하여 특정 스탯을 상승 효과
	ADD_ABNORMALITY_STAT = 2,

	// 귀환 주문서
	WARP_TO_TOWN = 3,

	// 순간이동 주문서
	WARP_TO_BOOKMARK = 4,

	// 랜덤 순간이동 주문서
	WARP_TO_RANDOM = 5,

	// 상자 아이템
	GET_REWARD = 6,

	// 반복 퀘스트 획득
	QUEST_REPEAT = 7,

	// 스탯 초기화 아이템
	RESET_BONUS_STAT = 8,

	// 보너스 스탯 포인트 아이템
	BONUS_STAT_POINT = 9,

	// 장비아이템 분해 재료(ex.용해제)
	DECOMPOSE_EQUIP_KIT = 10,

	// 스킬북 분해 재료 - 스킬 강화 할 때 사용하는 아이템을 만들기 위함
	DECOMPOSE_SKILLBOOK_KIT = 11,

	// 스킬북 - 스킬 배울 때 사용
	SKILL_BOOK = 12,

	// 스킬 강화 주문서 - 스킬 강화 할 때 사용
	ENCHANT_SKILL = 13,

	// 페어리 소환석
	SUMMON_FAIRY = 14,

	// 아르카스 성흔 - 아르카스 포인트 획득
	ARCAS_POINT = 15,

	// 제련석
	OPTION_STONE = 16,

	// 빛나는 제련석
	BLESSED_OPTION_STONE = 17,

	// PVP성향(카오틱) 감소 - 0으로 갈 수록 NETURAL로 변경됨
	PVP_ALIGNMENT_POINT = 18,

	// 코스튬 소환
	SUMMON_COSTUME = 19,

	// 기사단 선물
	GUILD_GIFT = 20,

	// 보물 몬스터 소환
	SPAWN_TREASURE_MONSTER = 21,

	// 몬스터 지식 획득
	GAIN_MONSTER_KNOWLEDGE_EXP = 22,

	// 몬스터 지식의 석판 해금
	UNLOCK_MONSTER_KNOWLEDGE_SLATE = 23,

	// 몬스터 지식의 석판 초기화
	RESET_MONSTER_KNOWLEDGE_SLATE_SLOT = 24,

	// 캠프 설치
	SPAWN_CAMP = 25,

	// 침공 포탈 설치
	SPAWN_INVADE_PORTAL = 26,

	// 아이템 강화(기존 카테고리별 축복/저주/일반) 통합 
	ITEM_ENCHANT = 27,

	// 던전 워프
	WARP_TO_DUNGEON = 28,

	// 던전 시간 충
	DUNGEON_TIME_CHARGE = 29,

	// 캐릭터 슬롯 확장
	CHARACTER_SLOT_UNLOCK = 30,

	// 복구 이벤트 상자
	DELIVERY_BOX = 31,

	// 캐릭터 커스터마이징 변경
	CHARACTER_CUSTOMIZING = 32,

	// 몬스터 소환
	SPAWN_MONSTER = 33,

	// BM상점쿠폰아이템
	DISCOUNT_COUPON = 34,

	// 유저 이모션
	SOCIAL_EMOTION = 35,

	// 사망 복구 아이템
	DEATH_PENALTY_RECOVERY = 36,

	// 코스튬 교환권
	CHANGE_COSTUME = 37,

	// 버프 및 보상 부여
	GET_ABNORMALITY_STAT_AND_REWARD = 38,

	// 닉네임 변경
	TICKET_NICKNAME_CHANGE = 39,

	// 마법부여
	MAGICAL_FORGE = 40,

	// 스킬 효과 실행
	EXE_SKILL_NOTIFY = 41,

	// 영지 귀환 주문서
	WARP_TO_AGIT = 42,

	MAX = 43,
};

UENUM(BlueprintType)
// 무기 타입 교체 종료 이유
enum class ChangeWeaponTypeFinishReason : uint8
{

	// 정상 종료
	NORMAL = 0,

	// 유저 사망
	DIE = 1,

	// 워프
	WARP = 2,

	// 이상 상태 적용
	ABNORMALITY = 3,

	// 착용 데이터 오류
	INVALID_DATA = 4,

	MAX = 5,
};

UENUM(BlueprintType)
enum class ItemEnchantResult : uint8
{

	// 강화 발생하지 않음
	NONE = 0,

	// 강화 성공
	ENCHANT_SUCCEEDED = 1,

	// 강화 성공 - 레벨 차감
	ENCHANT_SUCCEEDED_CURSED = 2,

	// 강화 성공 - 대성공
	ENCHANT_SUCCEEDED_BLESSED = 3,

	// 강화 실패
	ENCHANT_FAILED = 4,

	// 강화 실패 - 내구도 소모
	ENCHANT_FAILED_SHIELDED = 5,

	// 강화 실패 - 파괴
	ENCHANT_FAILED_DESTROYED = 6,

	// 강화 유지
	ENCHANT_PRESERVED = 7,

	// 강화 실패 - 강화도 감소
	ENCHANT_FAILED_DOWN = 8,

	MAX = 9,
};

UENUM(BlueprintType)
// 아이템 생성의 출처
enum class ItemAcquireType : uint8
{

	// 인게임으로 인해 생성
	IN_GAME = 0,

	// 결제(캐시)로 인해 생성
	PAID = 1,

	MAX = 2,
};

UENUM(BlueprintType)
enum class ItemRefineResult : uint8
{

	// 제련 발생하지 않음
	NONE = 0,

	// 제련 성공
	REFINE_SUCCEEDED = 1,

	// 제련 성공 - 보너스
	REFINE_SUCCEEDED_BONUS = 2,

	MAX = 3,
};

UENUM(BlueprintType)
enum class ItemMagicalForgeResult : uint8
{

	// 마법부여 발생하지 않음, 사실 상의 실패
	NONE = 0,

	// 마법부여 성공(레벨상승 or MagicalForgeID 변경되서 다시 1레벨)
	MAGICAL_FORGE_SUCCEEDED = 1,

	MAX = 2,
};

UENUM(BlueprintType)
enum class ItemCollectionType : uint8
{

	// 컬렉션 없음
	NONE = 0,

	// 영구 컬렉션
	PERMANENT = 1,

	// 기간제 컬렉션
	FIXED_TERM = 2,

	MAX = 3,
};

UENUM(BlueprintType)
enum class ItemCollectionUnlockType : uint8
{

	// 언락 - 해당 없음
	NONE = 0,

	// 언락 - 레벨
	LEVEL = 1,

	// 언락 - 퀘스트
	QUEST = 2,

	MAX = 3,
};

UENUM(BlueprintType)
enum class ItemCollectionEventType : uint8
{
	NONE = 0,
	NORMAL = 1,
	EVENT = 2,

	MAX = 3,
};

UENUM(BlueprintType)
enum class CraftResult : uint8
{

	// 제작 발생하지 않음
	NONE = 0,

	// 제작 성공
	CRAFT_SUCCEEDED = 1,

	// 제작 성공 - 대성공
	CRAFT_SUCCEEDED_GREAT = 2,

	// 제작 실패
	CRAFT_FAIL = 3,

	MAX = 4,
};

UENUM()
// 자동 판매/구매/보관 설정
enum class AutoPurchaseType : uint8
{

	// 자동 구매 설정
	AUTO_SHOP_BUY = 0,

	// 자동 판매설정(소모류)
	AUTO_SHOP_SELL_CONSUME = 1,

	// 자동 판매설정(장착류)
	AUTO_SHOP_SELL_EQUIP = 2,

	// 자동 창고 보관설정
	AUTO_DEPOT_KEEP = 3,

	// 자동 창고 그룹 보관 
	AUTO_DEPOT_KEEP_GROUP = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 기사단 선물 관련 타입
enum class GuildGiftGiveType : uint8
{

	// 아무것도아니다
	NONE = 0,

	// 구매자
	GIVE_TO_BUYER = 1,

	// 기사단 맴버들
	GIVE_TO_GUILD_MEMBERS = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 보물 몬스터 티커 정보
enum class TickerRange : uint8
{

	// 없음
	NONE = 0,

	// 맵 유저
	MAP = 1,

	// 모든 유저
	ALL = 2,

	// 자신
	SELF = 3,

	MAX = 4,
};

UENUM(BlueprintType)
enum class ItemCraftUnlockType : uint8
{

	// 언락 - 해당 없음
	NONE = 0,

	// 언락 - 레벨
	LEVEL = 1,

	// 언락 - 퀘스트
	QUEST = 2,

	MAX = 3,
};

UENUM(BlueprintType)
enum class ItemCraftRestrict : uint8
{

	// 없음
	NONE = 0,

	// 캐릭터별 제작 제한
	CHARACTER = 1,

	// 계정별 제작 제한
	ACCOUNT = 2,

	// 서버별 제작 제한
	SERVER = 3,

	MAX = 4,
};

UENUM(BlueprintType)
enum class ItemEnchantType : uint8
{

	// 일반
	NORMAL_ENCHANT = 0,

	// 축복 받은
	BLESSED_ENCHANT = 1,

	// 저주 받은
	CURSED_ENCHANT = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 아이템 드롭시 effect 처리
enum class DropItemEffect : uint8
{

	// 일반
	NORAM_TYPE = 0,

	// 랜덤 타격
	RANDOM_HIT_TYPE = 1,

	// 마지막 타격
	LAST_HIT_TYPE = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 아이템 쿠폰 할인 타입
enum class ItemCouponDiscountType : uint8
{

	// 상수차감방식
	CONSTANT = 0,

	// 계수차감방식
	COEFFICIENT = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 봉인 슬롯 색상
enum class SealSlotColor : uint8
{

	// 없음
	NONE = 0,

	// 파랑
	RARE = 1,

	// 보라
	EPIC = 2,

	// 노랑
	UNIQUE = 3,

	MAX = 4,
};

UENUM()
enum class ItemUpdateReason : uint16
{

	// 아이템 생성
	CREATE_ITEM = 0,

	// 아이템 생성 (admin)
	CREATE_ITEM_ADMIN_COMMAND = 1,

	// 아이템 삭제
	ITEM_DELETE = 2,

	// 아이템 삭제(버리기)
	ITEM_DELETE_THROW_AWAY = 3,

	// 아이템 삭제(admin)
	ITEM_DELETE_ADMIN_COMMAND = 4,

	// 아이템 삭제(위치 즐겨찾기)
	ITEM_DELETE_WARP_BOOK_MARK = 5,

	// 사망패널티 아이템복구
	DEATH_PANALTY_RESTORE = 6,

	// 업적 보상
	ACHIEVEMENT_REWARD = 7,

	// 업적 보상 ALL
	ACHIEVEMENT_REWARDALL = 8,

	// 업적 보상 THEME
	ACHIEVEMENT_REWARDTHEME = 9,

	// 아르카스 포인트 업데이트
	ARCAS_UPDATEPOINT = 10,

	// PVPALIGNMENT(카오틱 수치) 포인트 업데이트
	PVPALIGNMENT_UPDATEPOINT = 11,

	// 출석부 구매
	ATTENDANCE_PURCHASE = 12,

	// 배틀 패스 활성화
	BATTLEPASS_ACTIVATE = 13,

	// BM 상점 구매
	BMSHOP_PURCHASE = 14,

	// BM 구매함
	BMSHOP_RECIVE = 15,

	// 코스튬 소환
	COSTUME_SUMMON = 16,

	// 코스튬 합성
	COSTUME_COMPOSE = 17,

	// 코스튬 천장
	COSTUME_CEILING = 18,

	// 코스튬 교환
	COSTUME_CHANGE = 19,

	// 유저 사망
	USER_DEATH = 20,

	// 페어리 강화
	FAIRY_ENCHANT = 21,

	// 페어리 소환
	FAIRY_SUMMON = 22,

	// 페어리 합성
	FAIRY_COMPOSE = 23,

	// 페어리 천장
	FAIRY_CEILING = 24,

	// 기사단 구매
	GUILD_BUYSHOP = 25,

	// 기사단 선물
	GUILD_GUILDGIFT = 26,

	// 기사단 기부
	GUILD_DAILY_ACTIVITY = 27,

	// 기사단 아이템 콜렉션
	GUILD_ITEMCOLLECTION = 28,

	// 보너스 스텟 리셋
	ITEM_BONUSSTATRESET = 29,

	// 보너스 스텟 포인트 획득
	ITEM_BONUSSTATPOINT = 30,

	// 아이템 구매
	ITEM_BUY = 31,

	// 아이템 콜렉션
	ITEM_COLLECTION = 32,

	// 아이템 제작
	ITEM_CRAFT = 33,

	// 아이템 만료 삭제
	ITEM_DELETE_EXPIRED = 34,

	// 이벤트 복구 상자 사용(코스튬 합성 재시도)
	ITEM_DELIVERY_BOX_COSTUME = 35,

	// 이벤트 복구 상자 사용(장비)
	ITEM_DELIVERY_BOX_EQUIP = 36,

	// 이벤트 복구 상자 사용(페어리)
	ITEM_DELIVERY_BOX_FAIRY = 37,

	// 아이템 강화
	ITEM_ENCHANT = 38,

	// 강화 실패로 인한 아이템 파괴
	ENCHANT_FAILED_DESTORYED = 39,

	// 보상획득
	ITEM_GETREWARD = 40,

	// 아이템 분해
	ITEM_DECOMPOSE = 41,

	// 창고->인벤이동
	ITEM_MOVE_DEPOT_TO_INVENTORY = 42,

	// 인벤->창고이동
	ITEM_MOVE_INVEN_TO_DEPOT = 43,

	// 반복퀘
	ITEM_QUEST_REPEAT = 44,

	// 아이템 재련
	ITEM_REFINE = 45,

	// 선택 보상
	ITEM_SELECT_REWARD = 46,

	// 아이템 판매
	ITEM_SELL = 47,

	// 기사단 창고
	GUILD_STORE = 48,

	// 캠프 설치
	ITEM_SPAWN_CAMP = 49,

	// 보물 몬스터 소환
	ITEM_SPAWN_TREASURE_MONSTER = 50,

	// 아이템 사용
	ITEM_USE = 51,

	// 아이템 해제 및 착용
	ITEM_EQUIP_CHANGED = 52,

	// 메일 받기
	MAIL_RECV = 53,

	// 거래소 물품 등록 취소
	MARKET_TRANSACTIONCANCEL = 54,

	// 거래소 물품 등록
	MARKET_TRANSACTIONREGISTER = 55,

	// 거래소 물품 구입
	MARKET_TRANSACTIONPURCHASE = 56,

	// 몬스터 지식 획득
	MONSTER_KNOWLEDGE_EXP_GAIN = 57,

	// 몬스터 지식 획득 ALL
	MONSTER_KNOWLEDGE_EXP_GAIN_ALL = 58,

	// 몬스터 지식의 석판 초기화
	MONSTER_KNOWLEDGE_SLATE_RESET = 59,

	// 몬스터 지식의 석판 슬롯 오픈
	MONSTER_KNOWLEDGE_SLATE_SLOT = 60,

	// 다단계 랭크 변경
	MULTILEVELRANK_CHANGE = 61,

	// 다단계 랭크 승급
	MULTILEVELRANK_PROTOMTION = 62,

	// 다단계 랭크 보상
	MULTILEVELRANK_REWARD = 63,

	// 이벤트 상점 아이템 구매
	NPC_EVENT_SHOP_PURCHASE = 64,

	// 퀘스트 보상
	QUEST_REWARD = 65,

	// 아이템 강화
	SKILL_ENCHANT = 66,

	// 스킬 습득
	SKILL_LEARN = 67,

	// 캐릭터 슬롯 확장
	CHARACTER_SLOT_UNLOCK = 68,

	// USER_INVADE_PENALTY
	USER_INVADE_PENALTY = 69,

	// PvpAlignment 카오틱 Point 갱신
	PVP_ALIGNMENT_UPDATE_POINT = 70,

	// 파티 던전 클리어
	PARTY_DUNGEON_CLEAR = 71,

	// 보스 사냥
	BOSS_KILL = 72,

	// 드랍 습득
	DROP_GET = 73,

	// 맵 이벤트 툴에서 지급
	MAP_EVENT_API = 74,

	// 몬스터 사냥 획득
	NPC_REWARD = 75,

	// 캐릭터 커스터마이징
	CHARACTER_CUSTOMIZING = 76,

	// 침공 포털 활성화
	INVADE_PORTAL_ACTIVATE = 77,

	// 캐릭터 생성
	MAKE_CHARACTER = 78,

	// 하이브 아이템 지급 회수처리
	HVE_ITEM_GVIE_RETRIVE = 79,

	// 몬스터 소환
	ITEM_SPAWN_MONSTER = 80,

	// 월드 보스 클리어 보상
	WORLD_BOSS_CLEAR_REWARD = 81,

	// 월드 보스 참여 보상
	WORLD_BOSS_BASIC_REWARD = 82,

	// 월드 보스 위로 보상
	WORLD_BOSS_CONSOLATION_REWARD = 83,

	// 봉인(문양) 슬롯 전체 변경
	SEAL_SLOT_CHANGE_ALL = 84,

	// 봉인(문양) 레벨업
	SEAL_LEVEL_UP = 85,

	// 초과된 몬스터 지식에 대한 아이템 보상
	MONSTER_KNOWLEDGE_EXP_SURPLUS_REWARD = 86,

	// 침공 보물상자 보상
	INVADE_TREASUREBOX_REWARD = 87,

	// 개인거래
	PERSONAL_TRADING = 88,

	// 침공 패널티
	INVADE_PENALTY = 89,

	// 배틀 패스 보상
	BATTLEPASS_REWARD = 90,

	// 사망 패널티 복구 아이템 사용
	ITEM_DEATH_PENALTY_RECOVERY = 91,

	// 배틀 패스 보상 모두 받기
	BATTLEPASS_REWARD_ALL = 92,

	// 출석체크 출석보상
	ATTENDANCE_UPDATE = 93,

	// 아이템 복구 (admin)
	RESTORE_ITEM_ADMIN_COMMAND = 94,

	// 페어리 소환 마일리지
	SUMMON_FAIRY_MILEAGE = 95,

	// 코스튬 소환 마일리지
	SUMMON_COSTUME_MILEAGE = 96,

	// 천마석 강화
	SPIRIT_SHOT_ENCHANT = 97,

	// 커뮤니티 소녀 미션 이벤트
	COMMUNITY_EVENT_MISSION = 98,

	// 듀랄 사용
	DURAL = 99,

	// 아이템 합성
	ITEM_FUSE = 100,

	// 닉네임 변경권
	ITEM_NICKNAME_CHANGE = 101,

	// 몬스터 지식의 석판 특수 노드 개방
	SLATE_SPECIAL_NODE_ACTIVATE = 102,

	// 드랍 습득 -침공보스 KILL
	DROP_GET_INVADE_BOSS_KILL = 103,

	// 드랍 습득 -침공마차 KILL
	DROP_GET_INVADE_WAGON_KILL = 104,

	// 드랍 습득 -침공기사단장 KILL
	DROP_GET_INVADE_GUARDIAN_KILL = 105,

	// 드랍 습득 -침공PK 패널티
	DROP_GET_INVADE_PK_DIE_PENALTY = 106,

	// 드랍 습득 -침공 나가기 패널티
	DROP_GET_INVADE_HOME_WARP_PENALTY = 107,

	// 아이템 장비 마법부여
	ITEM_MAGICAL_FORGE = 108,

	// 영지 임대
	AGIT_RENTAL = 109,

	// 영지 입장
	AGIT_ENTER = 110,

	// 영지 퇴장
	AGIT_LEAVE = 111,

	// 길드전 보상
	ITEM_REWARD_GUILD_WAR = 112,

	// 유니크 페어리 소환 마일리지
	SUMMON_UNIQUE_FAIRY_MILEAGE = 113,

	// 유니크 코스튬 소환 마일리지
	SUMMON_UNIQUE_COSTUME_MILEAGE = 114,

	// 루팅한 아이템Id, ItemDBId 존재하지 않음 - 실제로 획득하지 않고, 획득과 동시에 자동분해 후 분해결과 아이템을 획득
	AUTO_DECOMPOSE = 115,

	ITEM_UPDATE_REASON_MAX = 116,
};

UENUM(BlueprintType)
// 옵션에서 드랍 아이템 자동 획득 옵션 타입(서버에서만 구분을 위해 사용)
enum class ItemGainType : uint8
{

	// 에러값
	NONE = 0,

	// 소모품 - 일반
	CONSUME_NORMAL = 1,

	// 소모품 - 특수
	CONSUME_SPECIAL = 2,

	// 주문서 - 일반
	CONSTUME_SCROLL_NORMAL = 3,

	// 주문서 - 특수
	CONSTUME_SCROLL_SPECIAL = 4,

	// 주문서 - 장비 강화
	CONSTUME_SCROLL_ENCHANT = 5,

	// 기타 - 일반
	OTHER_NORMAL = 6,

	// 기타 - 특수
	OTHER_SPECIAL = 7,

	// 기타 - 기타
	OTHER_ETC = 8,

	MAX = 9,
};

UENUM(BlueprintType)
// 아이템 분해를 어떤기준으로 할지에 대한 Type
enum class ItemDecomposeType : uint8
{

	// 에러값
	NONE = 0,

	// itemId : ItemCommon.Id 
	ITEM_ID = 1,

	// ItemCategoryMain : ItemCommon.categoryMain
	ITEM_CATEGORY_MAIN = 2,

	MAX = 3,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ItemType ItemTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemType::NONE;
	if (str.Compare(TEXT("EQUIP"), ESearchCase::IgnoreCase) == 0)	return ItemType::EQUIP;
	if (str.Compare(TEXT("INGREDIENT"), ESearchCase::IgnoreCase) == 0)	return ItemType::INGREDIENT;
	if (str.Compare(TEXT("CONSUME"), ESearchCase::IgnoreCase) == 0)	return ItemType::CONSUME;
	if (str.Compare(TEXT("CURRENCY"), ESearchCase::IgnoreCase) == 0)	return ItemType::CURRENCY;
	if (str.Compare(TEXT("ETC"), ESearchCase::IgnoreCase) == 0)	return ItemType::ETC;
	return ItemType::MAX;
}
inline ItemCategoryMain ItemCategoryMainStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::NONE;
	if (str.Compare(TEXT("WEAPON"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::WEAPON;
	if (str.Compare(TEXT("ARMOR"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::ARMOR;
	if (str.Compare(TEXT("ACCESSORY"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::ACCESSORY;
	if (str.Compare(TEXT("POTION"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::POTION;
	if (str.Compare(TEXT("SKILLBOOK"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::SKILLBOOK;
	if (str.Compare(TEXT("SCROLL"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::SCROLL;
	if (str.Compare(TEXT("FOOD"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::FOOD;
	if (str.Compare(TEXT("FAIRY"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::FAIRY;
	if (str.Compare(TEXT("COSTUME"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::COSTUME;
	if (str.Compare(TEXT("CRAFT"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::CRAFT;
	if (str.Compare(TEXT("QUEST"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::QUEST;
	if (str.Compare(TEXT("TICKET"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::TICKET;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::MONSTER_KNOWLEDGE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_SLATE"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::MONSTER_KNOWLEDGE_SLATE;
	if (str.Compare(TEXT("COLLECTABLE"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::COLLECTABLE;
	if (str.Compare(TEXT("CHARGE"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::CHARGE;
	if (str.Compare(TEXT("CURRENCY"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::CURRENCY;
	if (str.Compare(TEXT("ETC"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::ETC;
	if (str.Compare(TEXT("DELIVERY_BOX"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::DELIVERY_BOX;
	if (str.Compare(TEXT("SEAL"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::SEAL;
	if (str.Compare(TEXT("SPECIAL"), ESearchCase::IgnoreCase) == 0)	return ItemCategoryMain::SPECIAL;
	return ItemCategoryMain::MAX;
}
inline ItemCategorySub ItemCategorySubStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::NONE;
	if (str.Compare(TEXT("SWORD"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SWORD;
	if (str.Compare(TEXT("DAGGER"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DAGGER;
	if (str.Compare(TEXT("BOW"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BOW;
	if (str.Compare(TEXT("WAND"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::WAND;
	if (str.Compare(TEXT("WAEPON_ORB"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::WAEPON_ORB;
	if (str.Compare(TEXT("SHOTGUN"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SHOTGUN;
	if (str.Compare(TEXT("HELMET"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::HELMET;
	if (str.Compare(TEXT("BODY_ARMOR"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BODY_ARMOR;
	if (str.Compare(TEXT("GLOVE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::GLOVE;
	if (str.Compare(TEXT("SHOES"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SHOES;
	if (str.Compare(TEXT("BELT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BELT;
	if (str.Compare(TEXT("SCARF"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SCARF;
	if (str.Compare(TEXT("NECKLACE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::NECKLACE;
	if (str.Compare(TEXT("EAR_RING"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::EAR_RING;
	if (str.Compare(TEXT("RING"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::RING;
	if (str.Compare(TEXT("CHARM"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::CHARM;
	if (str.Compare(TEXT("ORB"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ORB;
	if (str.Compare(TEXT("ORB_2"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ORB_2;
	if (str.Compare(TEXT("BRACELET"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BRACELET;
	if (str.Compare(TEXT("DURAL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DURAL;
	if (str.Compare(TEXT("MARBLE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MARBLE;
	if (str.Compare(TEXT("MAX_EQUIP"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MAX_EQUIP;
	if (str.Compare(TEXT("HP_POTION"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::HP_POTION;
	if (str.Compare(TEXT("HP_POTION_MANUAL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::HP_POTION_MANUAL;
	if (str.Compare(TEXT("MP_TICK_RECOVERY_POTION"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MP_TICK_RECOVERY_POTION;
	if (str.Compare(TEXT("MP_POTION"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MP_POTION;
	if (str.Compare(TEXT("POTION_BUFF"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::POTION_BUFF;
	if (str.Compare(TEXT("BONUS_STAT_POINT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BONUS_STAT_POINT;
	if (str.Compare(TEXT("COMMON_SKILL_BOOK"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::COMMON_SKILL_BOOK;
	if (str.Compare(TEXT("SWORD_SKILL_BOOK"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SWORD_SKILL_BOOK;
	if (str.Compare(TEXT("DAGGER_SKILL_BOOK"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DAGGER_SKILL_BOOK;
	if (str.Compare(TEXT("BOW_SKILL_BOOK"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BOW_SKILL_BOOK;
	if (str.Compare(TEXT("WAND_SKILL_BOOK"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::WAND_SKILL_BOOK;
	if (str.Compare(TEXT("ORB_SKILL_BOOK"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ORB_SKILL_BOOK;
	if (str.Compare(TEXT("SHOTGUN_SKILL_BOOK"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SHOTGUN_SKILL_BOOK;
	if (str.Compare(TEXT("ENCHANT_SKILL_SCROLL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ENCHANT_SKILL_SCROLL;
	if (str.Compare(TEXT("WARP_TO_TOWN_SCROLL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::WARP_TO_TOWN_SCROLL;
	if (str.Compare(TEXT("WARP_TO_BOOKMARK_SCROLL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::WARP_TO_BOOKMARK_SCROLL;
	if (str.Compare(TEXT("WARP_TO_RANDOM_SCROLL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::WARP_TO_RANDOM_SCROLL;
	if (str.Compare(TEXT("WARP_TO_AGIT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::WARP_TO_AGIT;
	if (str.Compare(TEXT("ENCHANT_WEAPON_SCROLL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ENCHANT_WEAPON_SCROLL;
	if (str.Compare(TEXT("BLESSED_ENCHANT_WEAPON"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BLESSED_ENCHANT_WEAPON;
	if (str.Compare(TEXT("CURSED_ENCHANT_WEAPON"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::CURSED_ENCHANT_WEAPON;
	if (str.Compare(TEXT("ENCHANT_ARMOR"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ENCHANT_ARMOR;
	if (str.Compare(TEXT("BLESSED_ENCHANT_ARMOR"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BLESSED_ENCHANT_ARMOR;
	if (str.Compare(TEXT("CURSED_ENCHANT_ARMOR"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::CURSED_ENCHANT_ARMOR;
	if (str.Compare(TEXT("ENCHANT_ACCESSORY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ENCHANT_ACCESSORY;
	if (str.Compare(TEXT("BLESSED_ENCHANT_ACCESSORY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BLESSED_ENCHANT_ACCESSORY;
	if (str.Compare(TEXT("CURSED_ENCHANT_ACCESSORY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::CURSED_ENCHANT_ACCESSORY;
	if (str.Compare(TEXT("SCROLL_BUFF"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SCROLL_BUFF;
	if (str.Compare(TEXT("STATUE_BUFF"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::STATUE_BUFF;
	if (str.Compare(TEXT("OPTION_STONE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::OPTION_STONE;
	if (str.Compare(TEXT("BLESSED_OPTION_STONE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BLESSED_OPTION_STONE;
	if (str.Compare(TEXT("RESET_BONUS_STAT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::RESET_BONUS_STAT;
	if (str.Compare(TEXT("MAGICAL_FORGE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MAGICAL_FORGE;
	if (str.Compare(TEXT("SKILL_SCROLL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SKILL_SCROLL;
	if (str.Compare(TEXT("FOOD_BUFF"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::FOOD_BUFF;
	if (str.Compare(TEXT("SUMMON_FAIRY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SUMMON_FAIRY;
	if (str.Compare(TEXT("ENCHANT_FAIRY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ENCHANT_FAIRY;
	if (str.Compare(TEXT("FAIRY_CEILING"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::FAIRY_CEILING;
	if (str.Compare(TEXT("SUMMON_COSTUME"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SUMMON_COSTUME;
	if (str.Compare(TEXT("COSTUME_CEILING"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::COSTUME_CEILING;
	if (str.Compare(TEXT("CHANGE_COSTUME_RANDOM"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::CHANGE_COSTUME_RANDOM;
	if (str.Compare(TEXT("CHANGE_COSTUME_SELECT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::CHANGE_COSTUME_SELECT;
	if (str.Compare(TEXT("INGREDIENT_CRAFT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::INGREDIENT_CRAFT;
	if (str.Compare(TEXT("MATERIALS"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MATERIALS;
	if (str.Compare(TEXT("ESSENCE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ESSENCE;
	if (str.Compare(TEXT("RECIPE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::RECIPE;
	if (str.Compare(TEXT("INK"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::INK;
	if (str.Compare(TEXT("MYSTERIOUS_POWDER"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MYSTERIOUS_POWDER;
	if (str.Compare(TEXT("QUEST_REPEAT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::QUEST_REPEAT;
	if (str.Compare(TEXT("TICKET_QUEST_GUILD_KNIGHT_COMPLETE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::TICKET_QUEST_GUILD_KNIGHT_COMPLETE;
	if (str.Compare(TEXT("TICKET_CHARACTER_SLOT_UNLOCK"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::TICKET_CHARACTER_SLOT_UNLOCK;
	if (str.Compare(TEXT("TICKET_CHARACTER_CUSTOMIZING"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::TICKET_CHARACTER_CUSTOMIZING;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_GAIN_EXP"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MONSTER_KNOWLEDGE_GAIN_EXP;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_ATTACK"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_ATTACK;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_HIT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_HIT;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_AMPLIFICATION"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_AMPLIFICATION;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_DEFENCE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_DEFENCE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_RESISTANCE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_RESISTANCE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_SURVIVAL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_SURVIVAL;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_SLATE_RESET"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_RESET;
	if (str.Compare(TEXT("DECOMPOSE_EQUIP_KIT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DECOMPOSE_EQUIP_KIT;
	if (str.Compare(TEXT("DECOMPOSE_SKILLBOOK_KIT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DECOMPOSE_SKILLBOOK_KIT;
	if (str.Compare(TEXT("REWARD_BOX"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::REWARD_BOX;
	if (str.Compare(TEXT("ARROW"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ARROW;
	if (str.Compare(TEXT("SPIRIT_SHOT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SPIRIT_SHOT;
	if (str.Compare(TEXT("MAGIC_STONE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::MAGIC_STONE;
	if (str.Compare(TEXT("SPELL_STONE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SPELL_STONE;
	if (str.Compare(TEXT("ARCAS"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ARCAS;
	if (str.Compare(TEXT("PVP_ALIGNMENT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::PVP_ALIGNMENT;
	if (str.Compare(TEXT("CHEAT"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::CHEAT;
	if (str.Compare(TEXT("SPAWN_TREASURE_MONSTER"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SPAWN_TREASURE_MONSTER;
	if (str.Compare(TEXT("GUILD_GIFT_BOX"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::GUILD_GIFT_BOX;
	if (str.Compare(TEXT("SPAWN_CAMP"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SPAWN_CAMP;
	if (str.Compare(TEXT("GODS_BLESS"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::GODS_BLESS;
	if (str.Compare(TEXT("ENCHANT_COLLECTABLE_SCROLL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ENCHANT_COLLECTABLE_SCROLL;
	if (str.Compare(TEXT("ELITE_DUNGEON_TICKET"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ELITE_DUNGEON_TICKET;
	if (str.Compare(TEXT("DUNGEON_TIME"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DUNGEON_TIME;
	if (str.Compare(TEXT("INVADE_PORTAL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::INVADE_PORTAL;
	if (str.Compare(TEXT("SPAWN_MONSTER"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SPAWN_MONSTER;
	if (str.Compare(TEXT("SOCIAL_EMOTION"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SOCIAL_EMOTION;
	if (str.Compare(TEXT("ATTENDANCE_OPEN_KEY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ATTENDANCE_OPEN_KEY;
	if (str.Compare(TEXT("SQ_STAMP"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SQ_STAMP;
	if (str.Compare(TEXT("BOSS_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::BOSS_COLLECTION;
	if (str.Compare(TEXT("INVADE_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::INVADE_CURRENCY;
	if (str.Compare(TEXT("EVENT_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::EVENT_CURRENCY;
	if (str.Compare(TEXT("NORMAL_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::NORMAL_CURRENCY;
	if (str.Compare(TEXT("COIN_CURRENCY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::COIN_CURRENCY;
	if (str.Compare(TEXT("DISCOUNT_COUPON"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DISCOUNT_COUPON;
	if (str.Compare(TEXT("SEAL_STONE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::SEAL_STONE;
	if (str.Compare(TEXT("PARTYDUNGEON_KEY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::PARTYDUNGEON_KEY;
	if (str.Compare(TEXT("ETC"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ETC;
	if (str.Compare(TEXT("DELIVERY_BOX_EQUIP"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DELIVERY_BOX_EQUIP;
	if (str.Compare(TEXT("DELIVERY_BOX_COSTUME"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DELIVERY_BOX_COSTUME;
	if (str.Compare(TEXT("DELIVERY_BOX_FAIRY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DELIVERY_BOX_FAIRY;
	if (str.Compare(TEXT("DEATH_PENALTY_RECOVERY"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::DEATH_PENALTY_RECOVERY;
	if (str.Compare(TEXT("ELITE_DUNGEON_BALL"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::ELITE_DUNGEON_BALL;
	if (str.Compare(TEXT("NICKNAME_CHANGE"), ESearchCase::IgnoreCase) == 0)	return ItemCategorySub::NICKNAME_CHANGE;
	return ItemCategorySub::MAX;
}
inline ItemGrade ItemGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemGrade::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return ItemGrade::NORMAL;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return ItemGrade::MAGIC;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return ItemGrade::RARE;
	if (str.Compare(TEXT("EPIC"), ESearchCase::IgnoreCase) == 0)	return ItemGrade::EPIC;
	if (str.Compare(TEXT("UNIQUE"), ESearchCase::IgnoreCase) == 0)	return ItemGrade::UNIQUE;
	if (str.Compare(TEXT("LEGEND"), ESearchCase::IgnoreCase) == 0)	return ItemGrade::LEGEND;
	if (str.Compare(TEXT("ETERNAL"), ESearchCase::IgnoreCase) == 0)	return ItemGrade::ETERNAL;
	return ItemGrade::MAX;
}
inline ItemMaterial ItemMaterialStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("FABRIC"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::FABRIC;
	if (str.Compare(TEXT("WOOD"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::WOOD;
	if (str.Compare(TEXT("BONE"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::BONE;
	if (str.Compare(TEXT("LEATHER"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::LEATHER;
	if (str.Compare(TEXT("STEEL"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::STEEL;
	if (str.Compare(TEXT("ADAMATIUM"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::ADAMATIUM;
	if (str.Compare(TEXT("MITHRIL"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::MITHRIL;
	if (str.Compare(TEXT("JEWEL"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::JEWEL;
	if (str.Compare(TEXT("ZENITE"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::ZENITE;
	if (str.Compare(TEXT("TRUE_SILVER"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::TRUE_SILVER;
	if (str.Compare(TEXT("ORICALCUM"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::ORICALCUM;
	if (str.Compare(TEXT("SHELL"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::SHELL;
	if (str.Compare(TEXT("SPIRIT"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::SPIRIT;
	if (str.Compare(TEXT("STONE"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::STONE;
	if (str.Compare(TEXT("BRONZE"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::BRONZE;
	if (str.Compare(TEXT("THORIUM"), ESearchCase::IgnoreCase) == 0)	return ItemMaterial::THORIUM;
	return ItemMaterial::MAX;
}
inline ItemEffectType ItemEffectTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::NONE;
	if (str.Compare(TEXT("RESTORE_STAT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::RESTORE_STAT;
	if (str.Compare(TEXT("ADD_ABNORMALITY_STAT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::ADD_ABNORMALITY_STAT;
	if (str.Compare(TEXT("WARP_TO_TOWN"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::WARP_TO_TOWN;
	if (str.Compare(TEXT("WARP_TO_BOOKMARK"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::WARP_TO_BOOKMARK;
	if (str.Compare(TEXT("WARP_TO_RANDOM"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::WARP_TO_RANDOM;
	if (str.Compare(TEXT("GET_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::GET_REWARD;
	if (str.Compare(TEXT("QUEST_REPEAT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::QUEST_REPEAT;
	if (str.Compare(TEXT("RESET_BONUS_STAT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::RESET_BONUS_STAT;
	if (str.Compare(TEXT("BONUS_STAT_POINT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::BONUS_STAT_POINT;
	if (str.Compare(TEXT("DECOMPOSE_EQUIP_KIT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::DECOMPOSE_EQUIP_KIT;
	if (str.Compare(TEXT("DECOMPOSE_SKILLBOOK_KIT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::DECOMPOSE_SKILLBOOK_KIT;
	if (str.Compare(TEXT("SKILL_BOOK"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::SKILL_BOOK;
	if (str.Compare(TEXT("ENCHANT_SKILL"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::ENCHANT_SKILL;
	if (str.Compare(TEXT("SUMMON_FAIRY"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::SUMMON_FAIRY;
	if (str.Compare(TEXT("ARCAS_POINT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::ARCAS_POINT;
	if (str.Compare(TEXT("OPTION_STONE"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::OPTION_STONE;
	if (str.Compare(TEXT("BLESSED_OPTION_STONE"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::BLESSED_OPTION_STONE;
	if (str.Compare(TEXT("PVP_ALIGNMENT_POINT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::PVP_ALIGNMENT_POINT;
	if (str.Compare(TEXT("SUMMON_COSTUME"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::SUMMON_COSTUME;
	if (str.Compare(TEXT("GUILD_GIFT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::GUILD_GIFT;
	if (str.Compare(TEXT("SPAWN_TREASURE_MONSTER"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::SPAWN_TREASURE_MONSTER;
	if (str.Compare(TEXT("GAIN_MONSTER_KNOWLEDGE_EXP"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::GAIN_MONSTER_KNOWLEDGE_EXP;
	if (str.Compare(TEXT("UNLOCK_MONSTER_KNOWLEDGE_SLATE"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::UNLOCK_MONSTER_KNOWLEDGE_SLATE;
	if (str.Compare(TEXT("RESET_MONSTER_KNOWLEDGE_SLATE_SLOT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::RESET_MONSTER_KNOWLEDGE_SLATE_SLOT;
	if (str.Compare(TEXT("SPAWN_CAMP"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::SPAWN_CAMP;
	if (str.Compare(TEXT("SPAWN_INVADE_PORTAL"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::SPAWN_INVADE_PORTAL;
	if (str.Compare(TEXT("ITEM_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::ITEM_ENCHANT;
	if (str.Compare(TEXT("WARP_TO_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::WARP_TO_DUNGEON;
	if (str.Compare(TEXT("DUNGEON_TIME_CHARGE"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::DUNGEON_TIME_CHARGE;
	if (str.Compare(TEXT("CHARACTER_SLOT_UNLOCK"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::CHARACTER_SLOT_UNLOCK;
	if (str.Compare(TEXT("DELIVERY_BOX"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::DELIVERY_BOX;
	if (str.Compare(TEXT("CHARACTER_CUSTOMIZING"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::CHARACTER_CUSTOMIZING;
	if (str.Compare(TEXT("SPAWN_MONSTER"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::SPAWN_MONSTER;
	if (str.Compare(TEXT("DISCOUNT_COUPON"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::DISCOUNT_COUPON;
	if (str.Compare(TEXT("SOCIAL_EMOTION"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::SOCIAL_EMOTION;
	if (str.Compare(TEXT("DEATH_PENALTY_RECOVERY"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::DEATH_PENALTY_RECOVERY;
	if (str.Compare(TEXT("CHANGE_COSTUME"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::CHANGE_COSTUME;
	if (str.Compare(TEXT("GET_ABNORMALITY_STAT_AND_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::GET_ABNORMALITY_STAT_AND_REWARD;
	if (str.Compare(TEXT("TICKET_NICKNAME_CHANGE"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::TICKET_NICKNAME_CHANGE;
	if (str.Compare(TEXT("MAGICAL_FORGE"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::MAGICAL_FORGE;
	if (str.Compare(TEXT("EXE_SKILL_NOTIFY"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::EXE_SKILL_NOTIFY;
	if (str.Compare(TEXT("WARP_TO_AGIT"), ESearchCase::IgnoreCase) == 0)	return ItemEffectType::WARP_TO_AGIT;
	return ItemEffectType::MAX;
}
inline ChangeWeaponTypeFinishReason ChangeWeaponTypeFinishReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return ChangeWeaponTypeFinishReason::NORMAL;
	if (str.Compare(TEXT("DIE"), ESearchCase::IgnoreCase) == 0)	return ChangeWeaponTypeFinishReason::DIE;
	if (str.Compare(TEXT("WARP"), ESearchCase::IgnoreCase) == 0)	return ChangeWeaponTypeFinishReason::WARP;
	if (str.Compare(TEXT("ABNORMALITY"), ESearchCase::IgnoreCase) == 0)	return ChangeWeaponTypeFinishReason::ABNORMALITY;
	if (str.Compare(TEXT("INVALID_DATA"), ESearchCase::IgnoreCase) == 0)	return ChangeWeaponTypeFinishReason::INVALID_DATA;
	return ChangeWeaponTypeFinishReason::MAX;
}
inline ItemEnchantResult ItemEnchantResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantResult::NONE;
	if (str.Compare(TEXT("ENCHANT_SUCCEEDED"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantResult::ENCHANT_SUCCEEDED;
	if (str.Compare(TEXT("ENCHANT_SUCCEEDED_CURSED"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED;
	if (str.Compare(TEXT("ENCHANT_SUCCEEDED_BLESSED"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED;
	if (str.Compare(TEXT("ENCHANT_FAILED"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantResult::ENCHANT_FAILED;
	if (str.Compare(TEXT("ENCHANT_FAILED_SHIELDED"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantResult::ENCHANT_FAILED_SHIELDED;
	if (str.Compare(TEXT("ENCHANT_FAILED_DESTROYED"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantResult::ENCHANT_FAILED_DESTROYED;
	if (str.Compare(TEXT("ENCHANT_PRESERVED"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantResult::ENCHANT_PRESERVED;
	if (str.Compare(TEXT("ENCHANT_FAILED_DOWN"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantResult::ENCHANT_FAILED_DOWN;
	return ItemEnchantResult::MAX;
}
inline ItemAcquireType ItemAcquireTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("IN_GAME"), ESearchCase::IgnoreCase) == 0)	return ItemAcquireType::IN_GAME;
	if (str.Compare(TEXT("PAID"), ESearchCase::IgnoreCase) == 0)	return ItemAcquireType::PAID;
	return ItemAcquireType::MAX;
}
inline ItemRefineResult ItemRefineResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemRefineResult::NONE;
	if (str.Compare(TEXT("REFINE_SUCCEEDED"), ESearchCase::IgnoreCase) == 0)	return ItemRefineResult::REFINE_SUCCEEDED;
	if (str.Compare(TEXT("REFINE_SUCCEEDED_BONUS"), ESearchCase::IgnoreCase) == 0)	return ItemRefineResult::REFINE_SUCCEEDED_BONUS;
	return ItemRefineResult::MAX;
}
inline ItemMagicalForgeResult ItemMagicalForgeResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemMagicalForgeResult::NONE;
	if (str.Compare(TEXT("MAGICAL_FORGE_SUCCEEDED"), ESearchCase::IgnoreCase) == 0)	return ItemMagicalForgeResult::MAGICAL_FORGE_SUCCEEDED;
	return ItemMagicalForgeResult::MAX;
}
inline ItemCollectionType ItemCollectionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemCollectionType::NONE;
	if (str.Compare(TEXT("PERMANENT"), ESearchCase::IgnoreCase) == 0)	return ItemCollectionType::PERMANENT;
	if (str.Compare(TEXT("FIXED_TERM"), ESearchCase::IgnoreCase) == 0)	return ItemCollectionType::FIXED_TERM;
	return ItemCollectionType::MAX;
}
inline ItemCollectionUnlockType ItemCollectionUnlockTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemCollectionUnlockType::NONE;
	if (str.Compare(TEXT("LEVEL"), ESearchCase::IgnoreCase) == 0)	return ItemCollectionUnlockType::LEVEL;
	if (str.Compare(TEXT("QUEST"), ESearchCase::IgnoreCase) == 0)	return ItemCollectionUnlockType::QUEST;
	return ItemCollectionUnlockType::MAX;
}
inline ItemCollectionEventType ItemCollectionEventTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemCollectionEventType::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return ItemCollectionEventType::NORMAL;
	if (str.Compare(TEXT("EVENT"), ESearchCase::IgnoreCase) == 0)	return ItemCollectionEventType::EVENT;
	return ItemCollectionEventType::MAX;
}
inline CraftResult CraftResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CraftResult::NONE;
	if (str.Compare(TEXT("CRAFT_SUCCEEDED"), ESearchCase::IgnoreCase) == 0)	return CraftResult::CRAFT_SUCCEEDED;
	if (str.Compare(TEXT("CRAFT_SUCCEEDED_GREAT"), ESearchCase::IgnoreCase) == 0)	return CraftResult::CRAFT_SUCCEEDED_GREAT;
	if (str.Compare(TEXT("CRAFT_FAIL"), ESearchCase::IgnoreCase) == 0)	return CraftResult::CRAFT_FAIL;
	return CraftResult::MAX;
}
inline AutoPurchaseType AutoPurchaseTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("AUTO_SHOP_BUY"), ESearchCase::IgnoreCase) == 0)	return AutoPurchaseType::AUTO_SHOP_BUY;
	if (str.Compare(TEXT("AUTO_SHOP_SELL_CONSUME"), ESearchCase::IgnoreCase) == 0)	return AutoPurchaseType::AUTO_SHOP_SELL_CONSUME;
	if (str.Compare(TEXT("AUTO_SHOP_SELL_EQUIP"), ESearchCase::IgnoreCase) == 0)	return AutoPurchaseType::AUTO_SHOP_SELL_EQUIP;
	if (str.Compare(TEXT("AUTO_DEPOT_KEEP"), ESearchCase::IgnoreCase) == 0)	return AutoPurchaseType::AUTO_DEPOT_KEEP;
	if (str.Compare(TEXT("AUTO_DEPOT_KEEP_GROUP"), ESearchCase::IgnoreCase) == 0)	return AutoPurchaseType::AUTO_DEPOT_KEEP_GROUP;
	return AutoPurchaseType::MAX;
}
inline GuildGiftGiveType GuildGiftGiveTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return GuildGiftGiveType::NONE;
	if (str.Compare(TEXT("GIVE_TO_BUYER"), ESearchCase::IgnoreCase) == 0)	return GuildGiftGiveType::GIVE_TO_BUYER;
	if (str.Compare(TEXT("GIVE_TO_GUILD_MEMBERS"), ESearchCase::IgnoreCase) == 0)	return GuildGiftGiveType::GIVE_TO_GUILD_MEMBERS;
	return GuildGiftGiveType::MAX;
}
inline TickerRange TickerRangeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return TickerRange::NONE;
	if (str.Compare(TEXT("MAP"), ESearchCase::IgnoreCase) == 0)	return TickerRange::MAP;
	if (str.Compare(TEXT("ALL"), ESearchCase::IgnoreCase) == 0)	return TickerRange::ALL;
	if (str.Compare(TEXT("SELF"), ESearchCase::IgnoreCase) == 0)	return TickerRange::SELF;
	return TickerRange::MAX;
}
inline ItemCraftUnlockType ItemCraftUnlockTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemCraftUnlockType::NONE;
	if (str.Compare(TEXT("LEVEL"), ESearchCase::IgnoreCase) == 0)	return ItemCraftUnlockType::LEVEL;
	if (str.Compare(TEXT("QUEST"), ESearchCase::IgnoreCase) == 0)	return ItemCraftUnlockType::QUEST;
	return ItemCraftUnlockType::MAX;
}
inline ItemCraftRestrict ItemCraftRestrictStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemCraftRestrict::NONE;
	if (str.Compare(TEXT("CHARACTER"), ESearchCase::IgnoreCase) == 0)	return ItemCraftRestrict::CHARACTER;
	if (str.Compare(TEXT("ACCOUNT"), ESearchCase::IgnoreCase) == 0)	return ItemCraftRestrict::ACCOUNT;
	if (str.Compare(TEXT("SERVER"), ESearchCase::IgnoreCase) == 0)	return ItemCraftRestrict::SERVER;
	return ItemCraftRestrict::MAX;
}
inline ItemEnchantType ItemEnchantTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantType::NORMAL_ENCHANT;
	if (str.Compare(TEXT("BLESSED_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantType::BLESSED_ENCHANT;
	if (str.Compare(TEXT("CURSED_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return ItemEnchantType::CURSED_ENCHANT;
	return ItemEnchantType::MAX;
}
inline DropItemEffect DropItemEffectStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORAM_TYPE"), ESearchCase::IgnoreCase) == 0)	return DropItemEffect::NORAM_TYPE;
	if (str.Compare(TEXT("RANDOM_HIT_TYPE"), ESearchCase::IgnoreCase) == 0)	return DropItemEffect::RANDOM_HIT_TYPE;
	if (str.Compare(TEXT("LAST_HIT_TYPE"), ESearchCase::IgnoreCase) == 0)	return DropItemEffect::LAST_HIT_TYPE;
	return DropItemEffect::MAX;
}
inline ItemCouponDiscountType ItemCouponDiscountTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CONSTANT"), ESearchCase::IgnoreCase) == 0)	return ItemCouponDiscountType::CONSTANT;
	if (str.Compare(TEXT("COEFFICIENT"), ESearchCase::IgnoreCase) == 0)	return ItemCouponDiscountType::COEFFICIENT;
	return ItemCouponDiscountType::MAX;
}
inline SealSlotColor SealSlotColorStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return SealSlotColor::NONE;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return SealSlotColor::RARE;
	if (str.Compare(TEXT("EPIC"), ESearchCase::IgnoreCase) == 0)	return SealSlotColor::EPIC;
	if (str.Compare(TEXT("UNIQUE"), ESearchCase::IgnoreCase) == 0)	return SealSlotColor::UNIQUE;
	return SealSlotColor::MAX;
}
inline ItemUpdateReason ItemUpdateReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CREATE_ITEM"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::CREATE_ITEM;
	if (str.Compare(TEXT("CREATE_ITEM_ADMIN_COMMAND"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::CREATE_ITEM_ADMIN_COMMAND;
	if (str.Compare(TEXT("ITEM_DELETE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_DELETE;
	if (str.Compare(TEXT("ITEM_DELETE_THROW_AWAY"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_DELETE_THROW_AWAY;
	if (str.Compare(TEXT("ITEM_DELETE_ADMIN_COMMAND"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_DELETE_ADMIN_COMMAND;
	if (str.Compare(TEXT("ITEM_DELETE_WARP_BOOK_MARK"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_DELETE_WARP_BOOK_MARK;
	if (str.Compare(TEXT("DEATH_PANALTY_RESTORE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::DEATH_PANALTY_RESTORE;
	if (str.Compare(TEXT("ACHIEVEMENT_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ACHIEVEMENT_REWARD;
	if (str.Compare(TEXT("ACHIEVEMENT_REWARDALL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ACHIEVEMENT_REWARDALL;
	if (str.Compare(TEXT("ACHIEVEMENT_REWARDTHEME"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ACHIEVEMENT_REWARDTHEME;
	if (str.Compare(TEXT("ARCAS_UPDATEPOINT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ARCAS_UPDATEPOINT;
	if (str.Compare(TEXT("PVPALIGNMENT_UPDATEPOINT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::PVPALIGNMENT_UPDATEPOINT;
	if (str.Compare(TEXT("ATTENDANCE_PURCHASE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ATTENDANCE_PURCHASE;
	if (str.Compare(TEXT("BATTLEPASS_ACTIVATE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::BATTLEPASS_ACTIVATE;
	if (str.Compare(TEXT("BMSHOP_PURCHASE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::BMSHOP_PURCHASE;
	if (str.Compare(TEXT("BMSHOP_RECIVE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::BMSHOP_RECIVE;
	if (str.Compare(TEXT("COSTUME_SUMMON"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::COSTUME_SUMMON;
	if (str.Compare(TEXT("COSTUME_COMPOSE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::COSTUME_COMPOSE;
	if (str.Compare(TEXT("COSTUME_CEILING"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::COSTUME_CEILING;
	if (str.Compare(TEXT("COSTUME_CHANGE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::COSTUME_CHANGE;
	if (str.Compare(TEXT("USER_DEATH"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::USER_DEATH;
	if (str.Compare(TEXT("FAIRY_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::FAIRY_ENCHANT;
	if (str.Compare(TEXT("FAIRY_SUMMON"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::FAIRY_SUMMON;
	if (str.Compare(TEXT("FAIRY_COMPOSE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::FAIRY_COMPOSE;
	if (str.Compare(TEXT("FAIRY_CEILING"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::FAIRY_CEILING;
	if (str.Compare(TEXT("GUILD_BUYSHOP"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::GUILD_BUYSHOP;
	if (str.Compare(TEXT("GUILD_GUILDGIFT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::GUILD_GUILDGIFT;
	if (str.Compare(TEXT("GUILD_DAILY_ACTIVITY"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::GUILD_DAILY_ACTIVITY;
	if (str.Compare(TEXT("GUILD_ITEMCOLLECTION"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::GUILD_ITEMCOLLECTION;
	if (str.Compare(TEXT("ITEM_BONUSSTATRESET"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_BONUSSTATRESET;
	if (str.Compare(TEXT("ITEM_BONUSSTATPOINT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_BONUSSTATPOINT;
	if (str.Compare(TEXT("ITEM_BUY"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_BUY;
	if (str.Compare(TEXT("ITEM_COLLECTION"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_COLLECTION;
	if (str.Compare(TEXT("ITEM_CRAFT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_CRAFT;
	if (str.Compare(TEXT("ITEM_DELETE_EXPIRED"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_DELETE_EXPIRED;
	if (str.Compare(TEXT("ITEM_DELIVERY_BOX_COSTUME"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_DELIVERY_BOX_COSTUME;
	if (str.Compare(TEXT("ITEM_DELIVERY_BOX_EQUIP"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_DELIVERY_BOX_EQUIP;
	if (str.Compare(TEXT("ITEM_DELIVERY_BOX_FAIRY"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_DELIVERY_BOX_FAIRY;
	if (str.Compare(TEXT("ITEM_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_ENCHANT;
	if (str.Compare(TEXT("ENCHANT_FAILED_DESTORYED"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ENCHANT_FAILED_DESTORYED;
	if (str.Compare(TEXT("ITEM_GETREWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_GETREWARD;
	if (str.Compare(TEXT("ITEM_DECOMPOSE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_DECOMPOSE;
	if (str.Compare(TEXT("ITEM_MOVE_DEPOT_TO_INVENTORY"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_MOVE_DEPOT_TO_INVENTORY;
	if (str.Compare(TEXT("ITEM_MOVE_INVEN_TO_DEPOT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_MOVE_INVEN_TO_DEPOT;
	if (str.Compare(TEXT("ITEM_QUEST_REPEAT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_QUEST_REPEAT;
	if (str.Compare(TEXT("ITEM_REFINE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_REFINE;
	if (str.Compare(TEXT("ITEM_SELECT_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_SELECT_REWARD;
	if (str.Compare(TEXT("ITEM_SELL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_SELL;
	if (str.Compare(TEXT("GUILD_STORE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::GUILD_STORE;
	if (str.Compare(TEXT("ITEM_SPAWN_CAMP"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_SPAWN_CAMP;
	if (str.Compare(TEXT("ITEM_SPAWN_TREASURE_MONSTER"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_SPAWN_TREASURE_MONSTER;
	if (str.Compare(TEXT("ITEM_USE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_USE;
	if (str.Compare(TEXT("ITEM_EQUIP_CHANGED"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_EQUIP_CHANGED;
	if (str.Compare(TEXT("MAIL_RECV"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MAIL_RECV;
	if (str.Compare(TEXT("MARKET_TRANSACTIONCANCEL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MARKET_TRANSACTIONCANCEL;
	if (str.Compare(TEXT("MARKET_TRANSACTIONREGISTER"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MARKET_TRANSACTIONREGISTER;
	if (str.Compare(TEXT("MARKET_TRANSACTIONPURCHASE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MARKET_TRANSACTIONPURCHASE;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_EXP_GAIN"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MONSTER_KNOWLEDGE_EXP_GAIN;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_EXP_GAIN_ALL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MONSTER_KNOWLEDGE_EXP_GAIN_ALL;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_SLATE_RESET"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MONSTER_KNOWLEDGE_SLATE_RESET;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_SLATE_SLOT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MONSTER_KNOWLEDGE_SLATE_SLOT;
	if (str.Compare(TEXT("MULTILEVELRANK_CHANGE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MULTILEVELRANK_CHANGE;
	if (str.Compare(TEXT("MULTILEVELRANK_PROTOMTION"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MULTILEVELRANK_PROTOMTION;
	if (str.Compare(TEXT("MULTILEVELRANK_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MULTILEVELRANK_REWARD;
	if (str.Compare(TEXT("NPC_EVENT_SHOP_PURCHASE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::NPC_EVENT_SHOP_PURCHASE;
	if (str.Compare(TEXT("QUEST_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::QUEST_REWARD;
	if (str.Compare(TEXT("SKILL_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::SKILL_ENCHANT;
	if (str.Compare(TEXT("SKILL_LEARN"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::SKILL_LEARN;
	if (str.Compare(TEXT("CHARACTER_SLOT_UNLOCK"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::CHARACTER_SLOT_UNLOCK;
	if (str.Compare(TEXT("USER_INVADE_PENALTY"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::USER_INVADE_PENALTY;
	if (str.Compare(TEXT("PVP_ALIGNMENT_UPDATE_POINT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::PVP_ALIGNMENT_UPDATE_POINT;
	if (str.Compare(TEXT("PARTY_DUNGEON_CLEAR"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::PARTY_DUNGEON_CLEAR;
	if (str.Compare(TEXT("BOSS_KILL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::BOSS_KILL;
	if (str.Compare(TEXT("DROP_GET"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::DROP_GET;
	if (str.Compare(TEXT("MAP_EVENT_API"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MAP_EVENT_API;
	if (str.Compare(TEXT("NPC_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::NPC_REWARD;
	if (str.Compare(TEXT("CHARACTER_CUSTOMIZING"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::CHARACTER_CUSTOMIZING;
	if (str.Compare(TEXT("INVADE_PORTAL_ACTIVATE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::INVADE_PORTAL_ACTIVATE;
	if (str.Compare(TEXT("MAKE_CHARACTER"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MAKE_CHARACTER;
	if (str.Compare(TEXT("HVE_ITEM_GVIE_RETRIVE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::HVE_ITEM_GVIE_RETRIVE;
	if (str.Compare(TEXT("ITEM_SPAWN_MONSTER"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_SPAWN_MONSTER;
	if (str.Compare(TEXT("WORLD_BOSS_CLEAR_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::WORLD_BOSS_CLEAR_REWARD;
	if (str.Compare(TEXT("WORLD_BOSS_BASIC_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::WORLD_BOSS_BASIC_REWARD;
	if (str.Compare(TEXT("WORLD_BOSS_CONSOLATION_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::WORLD_BOSS_CONSOLATION_REWARD;
	if (str.Compare(TEXT("SEAL_SLOT_CHANGE_ALL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::SEAL_SLOT_CHANGE_ALL;
	if (str.Compare(TEXT("SEAL_LEVEL_UP"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::SEAL_LEVEL_UP;
	if (str.Compare(TEXT("MONSTER_KNOWLEDGE_EXP_SURPLUS_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::MONSTER_KNOWLEDGE_EXP_SURPLUS_REWARD;
	if (str.Compare(TEXT("INVADE_TREASUREBOX_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::INVADE_TREASUREBOX_REWARD;
	if (str.Compare(TEXT("PERSONAL_TRADING"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::PERSONAL_TRADING;
	if (str.Compare(TEXT("INVADE_PENALTY"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::INVADE_PENALTY;
	if (str.Compare(TEXT("BATTLEPASS_REWARD"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::BATTLEPASS_REWARD;
	if (str.Compare(TEXT("ITEM_DEATH_PENALTY_RECOVERY"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_DEATH_PENALTY_RECOVERY;
	if (str.Compare(TEXT("BATTLEPASS_REWARD_ALL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::BATTLEPASS_REWARD_ALL;
	if (str.Compare(TEXT("ATTENDANCE_UPDATE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ATTENDANCE_UPDATE;
	if (str.Compare(TEXT("RESTORE_ITEM_ADMIN_COMMAND"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::RESTORE_ITEM_ADMIN_COMMAND;
	if (str.Compare(TEXT("SUMMON_FAIRY_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::SUMMON_FAIRY_MILEAGE;
	if (str.Compare(TEXT("SUMMON_COSTUME_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::SUMMON_COSTUME_MILEAGE;
	if (str.Compare(TEXT("SPIRIT_SHOT_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::SPIRIT_SHOT_ENCHANT;
	if (str.Compare(TEXT("COMMUNITY_EVENT_MISSION"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::COMMUNITY_EVENT_MISSION;
	if (str.Compare(TEXT("DURAL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::DURAL;
	if (str.Compare(TEXT("ITEM_FUSE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_FUSE;
	if (str.Compare(TEXT("ITEM_NICKNAME_CHANGE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_NICKNAME_CHANGE;
	if (str.Compare(TEXT("SLATE_SPECIAL_NODE_ACTIVATE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::SLATE_SPECIAL_NODE_ACTIVATE;
	if (str.Compare(TEXT("DROP_GET_INVADE_BOSS_KILL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::DROP_GET_INVADE_BOSS_KILL;
	if (str.Compare(TEXT("DROP_GET_INVADE_WAGON_KILL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::DROP_GET_INVADE_WAGON_KILL;
	if (str.Compare(TEXT("DROP_GET_INVADE_GUARDIAN_KILL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::DROP_GET_INVADE_GUARDIAN_KILL;
	if (str.Compare(TEXT("DROP_GET_INVADE_PK_DIE_PENALTY"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::DROP_GET_INVADE_PK_DIE_PENALTY;
	if (str.Compare(TEXT("DROP_GET_INVADE_HOME_WARP_PENALTY"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::DROP_GET_INVADE_HOME_WARP_PENALTY;
	if (str.Compare(TEXT("ITEM_MAGICAL_FORGE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_MAGICAL_FORGE;
	if (str.Compare(TEXT("AGIT_RENTAL"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::AGIT_RENTAL;
	if (str.Compare(TEXT("AGIT_ENTER"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::AGIT_ENTER;
	if (str.Compare(TEXT("AGIT_LEAVE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::AGIT_LEAVE;
	if (str.Compare(TEXT("ITEM_REWARD_GUILD_WAR"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::ITEM_REWARD_GUILD_WAR;
	if (str.Compare(TEXT("SUMMON_UNIQUE_FAIRY_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::SUMMON_UNIQUE_FAIRY_MILEAGE;
	if (str.Compare(TEXT("SUMMON_UNIQUE_COSTUME_MILEAGE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::SUMMON_UNIQUE_COSTUME_MILEAGE;
	if (str.Compare(TEXT("AUTO_DECOMPOSE"), ESearchCase::IgnoreCase) == 0)	return ItemUpdateReason::AUTO_DECOMPOSE;
	return ItemUpdateReason::ITEM_UPDATE_REASON_MAX;
}
inline ItemGainType ItemGainTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemGainType::NONE;
	if (str.Compare(TEXT("CONSUME_NORMAL"), ESearchCase::IgnoreCase) == 0)	return ItemGainType::CONSUME_NORMAL;
	if (str.Compare(TEXT("CONSUME_SPECIAL"), ESearchCase::IgnoreCase) == 0)	return ItemGainType::CONSUME_SPECIAL;
	if (str.Compare(TEXT("CONSTUME_SCROLL_NORMAL"), ESearchCase::IgnoreCase) == 0)	return ItemGainType::CONSTUME_SCROLL_NORMAL;
	if (str.Compare(TEXT("CONSTUME_SCROLL_SPECIAL"), ESearchCase::IgnoreCase) == 0)	return ItemGainType::CONSTUME_SCROLL_SPECIAL;
	if (str.Compare(TEXT("CONSTUME_SCROLL_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return ItemGainType::CONSTUME_SCROLL_ENCHANT;
	if (str.Compare(TEXT("OTHER_NORMAL"), ESearchCase::IgnoreCase) == 0)	return ItemGainType::OTHER_NORMAL;
	if (str.Compare(TEXT("OTHER_SPECIAL"), ESearchCase::IgnoreCase) == 0)	return ItemGainType::OTHER_SPECIAL;
	if (str.Compare(TEXT("OTHER_ETC"), ESearchCase::IgnoreCase) == 0)	return ItemGainType::OTHER_ETC;
	return ItemGainType::MAX;
}
inline ItemDecomposeType ItemDecomposeTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ItemDecomposeType::NONE;
	if (str.Compare(TEXT("ITEM_ID"), ESearchCase::IgnoreCase) == 0)	return ItemDecomposeType::ITEM_ID;
	if (str.Compare(TEXT("ITEM_CATEGORY_MAIN"), ESearchCase::IgnoreCase) == 0)	return ItemDecomposeType::ITEM_CATEGORY_MAIN;
	return ItemDecomposeType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString ItemTypeEnumToString(ItemType value)
{
	switch (value)
	{
		case ItemType::NONE: return TEXT("NONE");
		case ItemType::EQUIP: return TEXT("EQUIP");
		case ItemType::INGREDIENT: return TEXT("INGREDIENT");
		case ItemType::CONSUME: return TEXT("CONSUME");
		case ItemType::CURRENCY: return TEXT("CURRENCY");
		case ItemType::ETC: return TEXT("ETC");
	}
	return FString();
}
inline FString ItemCategoryMainEnumToString(ItemCategoryMain value)
{
	switch (value)
	{
		case ItemCategoryMain::NONE: return TEXT("NONE");
		case ItemCategoryMain::WEAPON: return TEXT("WEAPON");
		case ItemCategoryMain::ARMOR: return TEXT("ARMOR");
		case ItemCategoryMain::ACCESSORY: return TEXT("ACCESSORY");
		case ItemCategoryMain::POTION: return TEXT("POTION");
		case ItemCategoryMain::SKILLBOOK: return TEXT("SKILLBOOK");
		case ItemCategoryMain::SCROLL: return TEXT("SCROLL");
		case ItemCategoryMain::FOOD: return TEXT("FOOD");
		case ItemCategoryMain::FAIRY: return TEXT("FAIRY");
		case ItemCategoryMain::COSTUME: return TEXT("COSTUME");
		case ItemCategoryMain::CRAFT: return TEXT("CRAFT");
		case ItemCategoryMain::QUEST: return TEXT("QUEST");
		case ItemCategoryMain::TICKET: return TEXT("TICKET");
		case ItemCategoryMain::MONSTER_KNOWLEDGE: return TEXT("MONSTER_KNOWLEDGE");
		case ItemCategoryMain::MONSTER_KNOWLEDGE_SLATE: return TEXT("MONSTER_KNOWLEDGE_SLATE");
		case ItemCategoryMain::COLLECTABLE: return TEXT("COLLECTABLE");
		case ItemCategoryMain::CHARGE: return TEXT("CHARGE");
		case ItemCategoryMain::CURRENCY: return TEXT("CURRENCY");
		case ItemCategoryMain::ETC: return TEXT("ETC");
		case ItemCategoryMain::DELIVERY_BOX: return TEXT("DELIVERY_BOX");
		case ItemCategoryMain::SEAL: return TEXT("SEAL");
		case ItemCategoryMain::SPECIAL: return TEXT("SPECIAL");
	}
	return FString();
}
inline FString ItemCategorySubEnumToString(ItemCategorySub value)
{
	switch (value)
	{
		case ItemCategorySub::NONE: return TEXT("NONE");
		case ItemCategorySub::SWORD: return TEXT("SWORD");
		case ItemCategorySub::DAGGER: return TEXT("DAGGER");
		case ItemCategorySub::BOW: return TEXT("BOW");
		case ItemCategorySub::WAND: return TEXT("WAND");
		case ItemCategorySub::WAEPON_ORB: return TEXT("WAEPON_ORB");
		case ItemCategorySub::SHOTGUN: return TEXT("SHOTGUN");
		case ItemCategorySub::HELMET: return TEXT("HELMET");
		case ItemCategorySub::BODY_ARMOR: return TEXT("BODY_ARMOR");
		case ItemCategorySub::GLOVE: return TEXT("GLOVE");
		case ItemCategorySub::SHOES: return TEXT("SHOES");
		case ItemCategorySub::BELT: return TEXT("BELT");
		case ItemCategorySub::SCARF: return TEXT("SCARF");
		case ItemCategorySub::NECKLACE: return TEXT("NECKLACE");
		case ItemCategorySub::EAR_RING: return TEXT("EAR_RING");
		case ItemCategorySub::RING: return TEXT("RING");
		case ItemCategorySub::CHARM: return TEXT("CHARM");
		case ItemCategorySub::ORB: return TEXT("ORB");
		case ItemCategorySub::ORB_2: return TEXT("ORB_2");
		case ItemCategorySub::BRACELET: return TEXT("BRACELET");
		case ItemCategorySub::DURAL: return TEXT("DURAL");
		case ItemCategorySub::MARBLE: return TEXT("MARBLE");
		case ItemCategorySub::MAX_EQUIP: return TEXT("MAX_EQUIP");
		case ItemCategorySub::HP_POTION: return TEXT("HP_POTION");
		case ItemCategorySub::HP_POTION_MANUAL: return TEXT("HP_POTION_MANUAL");
		case ItemCategorySub::MP_TICK_RECOVERY_POTION: return TEXT("MP_TICK_RECOVERY_POTION");
		case ItemCategorySub::MP_POTION: return TEXT("MP_POTION");
		case ItemCategorySub::POTION_BUFF: return TEXT("POTION_BUFF");
		case ItemCategorySub::BONUS_STAT_POINT: return TEXT("BONUS_STAT_POINT");
		case ItemCategorySub::COMMON_SKILL_BOOK: return TEXT("COMMON_SKILL_BOOK");
		case ItemCategorySub::SWORD_SKILL_BOOK: return TEXT("SWORD_SKILL_BOOK");
		case ItemCategorySub::DAGGER_SKILL_BOOK: return TEXT("DAGGER_SKILL_BOOK");
		case ItemCategorySub::BOW_SKILL_BOOK: return TEXT("BOW_SKILL_BOOK");
		case ItemCategorySub::WAND_SKILL_BOOK: return TEXT("WAND_SKILL_BOOK");
		case ItemCategorySub::ORB_SKILL_BOOK: return TEXT("ORB_SKILL_BOOK");
		case ItemCategorySub::SHOTGUN_SKILL_BOOK: return TEXT("SHOTGUN_SKILL_BOOK");
		case ItemCategorySub::ENCHANT_SKILL_SCROLL: return TEXT("ENCHANT_SKILL_SCROLL");
		case ItemCategorySub::WARP_TO_TOWN_SCROLL: return TEXT("WARP_TO_TOWN_SCROLL");
		case ItemCategorySub::WARP_TO_BOOKMARK_SCROLL: return TEXT("WARP_TO_BOOKMARK_SCROLL");
		case ItemCategorySub::WARP_TO_RANDOM_SCROLL: return TEXT("WARP_TO_RANDOM_SCROLL");
		case ItemCategorySub::WARP_TO_AGIT: return TEXT("WARP_TO_AGIT");
		case ItemCategorySub::ENCHANT_WEAPON_SCROLL: return TEXT("ENCHANT_WEAPON_SCROLL");
		case ItemCategorySub::BLESSED_ENCHANT_WEAPON: return TEXT("BLESSED_ENCHANT_WEAPON");
		case ItemCategorySub::CURSED_ENCHANT_WEAPON: return TEXT("CURSED_ENCHANT_WEAPON");
		case ItemCategorySub::ENCHANT_ARMOR: return TEXT("ENCHANT_ARMOR");
		case ItemCategorySub::BLESSED_ENCHANT_ARMOR: return TEXT("BLESSED_ENCHANT_ARMOR");
		case ItemCategorySub::CURSED_ENCHANT_ARMOR: return TEXT("CURSED_ENCHANT_ARMOR");
		case ItemCategorySub::ENCHANT_ACCESSORY: return TEXT("ENCHANT_ACCESSORY");
		case ItemCategorySub::BLESSED_ENCHANT_ACCESSORY: return TEXT("BLESSED_ENCHANT_ACCESSORY");
		case ItemCategorySub::CURSED_ENCHANT_ACCESSORY: return TEXT("CURSED_ENCHANT_ACCESSORY");
		case ItemCategorySub::SCROLL_BUFF: return TEXT("SCROLL_BUFF");
		case ItemCategorySub::STATUE_BUFF: return TEXT("STATUE_BUFF");
		case ItemCategorySub::OPTION_STONE: return TEXT("OPTION_STONE");
		case ItemCategorySub::BLESSED_OPTION_STONE: return TEXT("BLESSED_OPTION_STONE");
		case ItemCategorySub::RESET_BONUS_STAT: return TEXT("RESET_BONUS_STAT");
		case ItemCategorySub::MAGICAL_FORGE: return TEXT("MAGICAL_FORGE");
		case ItemCategorySub::SKILL_SCROLL: return TEXT("SKILL_SCROLL");
		case ItemCategorySub::FOOD_BUFF: return TEXT("FOOD_BUFF");
		case ItemCategorySub::SUMMON_FAIRY: return TEXT("SUMMON_FAIRY");
		case ItemCategorySub::ENCHANT_FAIRY: return TEXT("ENCHANT_FAIRY");
		case ItemCategorySub::FAIRY_CEILING: return TEXT("FAIRY_CEILING");
		case ItemCategorySub::SUMMON_COSTUME: return TEXT("SUMMON_COSTUME");
		case ItemCategorySub::COSTUME_CEILING: return TEXT("COSTUME_CEILING");
		case ItemCategorySub::CHANGE_COSTUME_RANDOM: return TEXT("CHANGE_COSTUME_RANDOM");
		case ItemCategorySub::CHANGE_COSTUME_SELECT: return TEXT("CHANGE_COSTUME_SELECT");
		case ItemCategorySub::INGREDIENT_CRAFT: return TEXT("INGREDIENT_CRAFT");
		case ItemCategorySub::MATERIALS: return TEXT("MATERIALS");
		case ItemCategorySub::ESSENCE: return TEXT("ESSENCE");
		case ItemCategorySub::RECIPE: return TEXT("RECIPE");
		case ItemCategorySub::INK: return TEXT("INK");
		case ItemCategorySub::MYSTERIOUS_POWDER: return TEXT("MYSTERIOUS_POWDER");
		case ItemCategorySub::QUEST_REPEAT: return TEXT("QUEST_REPEAT");
		case ItemCategorySub::TICKET_QUEST_GUILD_KNIGHT_COMPLETE: return TEXT("TICKET_QUEST_GUILD_KNIGHT_COMPLETE");
		case ItemCategorySub::TICKET_CHARACTER_SLOT_UNLOCK: return TEXT("TICKET_CHARACTER_SLOT_UNLOCK");
		case ItemCategorySub::TICKET_CHARACTER_CUSTOMIZING: return TEXT("TICKET_CHARACTER_CUSTOMIZING");
		case ItemCategorySub::MONSTER_KNOWLEDGE_GAIN_EXP: return TEXT("MONSTER_KNOWLEDGE_GAIN_EXP");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_ATTACK: return TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_ATTACK");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_HIT: return TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_HIT");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_AMPLIFICATION: return TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_AMPLIFICATION");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_DEFENCE: return TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_DEFENCE");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_RESISTANCE: return TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_RESISTANCE");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_SURVIVAL: return TEXT("MONSTER_KNOWLEDGE_SLATE_UNLOCK_SURVIVAL");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_RESET: return TEXT("MONSTER_KNOWLEDGE_SLATE_RESET");
		case ItemCategorySub::DECOMPOSE_EQUIP_KIT: return TEXT("DECOMPOSE_EQUIP_KIT");
		case ItemCategorySub::DECOMPOSE_SKILLBOOK_KIT: return TEXT("DECOMPOSE_SKILLBOOK_KIT");
		case ItemCategorySub::REWARD_BOX: return TEXT("REWARD_BOX");
		case ItemCategorySub::ARROW: return TEXT("ARROW");
		case ItemCategorySub::SPIRIT_SHOT: return TEXT("SPIRIT_SHOT");
		case ItemCategorySub::MAGIC_STONE: return TEXT("MAGIC_STONE");
		case ItemCategorySub::SPELL_STONE: return TEXT("SPELL_STONE");
		case ItemCategorySub::ARCAS: return TEXT("ARCAS");
		case ItemCategorySub::PVP_ALIGNMENT: return TEXT("PVP_ALIGNMENT");
		case ItemCategorySub::CHEAT: return TEXT("CHEAT");
		case ItemCategorySub::SPAWN_TREASURE_MONSTER: return TEXT("SPAWN_TREASURE_MONSTER");
		case ItemCategorySub::GUILD_GIFT_BOX: return TEXT("GUILD_GIFT_BOX");
		case ItemCategorySub::SPAWN_CAMP: return TEXT("SPAWN_CAMP");
		case ItemCategorySub::GODS_BLESS: return TEXT("GODS_BLESS");
		case ItemCategorySub::ENCHANT_COLLECTABLE_SCROLL: return TEXT("ENCHANT_COLLECTABLE_SCROLL");
		case ItemCategorySub::ELITE_DUNGEON_TICKET: return TEXT("ELITE_DUNGEON_TICKET");
		case ItemCategorySub::DUNGEON_TIME: return TEXT("DUNGEON_TIME");
		case ItemCategorySub::INVADE_PORTAL: return TEXT("INVADE_PORTAL");
		case ItemCategorySub::SPAWN_MONSTER: return TEXT("SPAWN_MONSTER");
		case ItemCategorySub::SOCIAL_EMOTION: return TEXT("SOCIAL_EMOTION");
		case ItemCategorySub::ATTENDANCE_OPEN_KEY: return TEXT("ATTENDANCE_OPEN_KEY");
		case ItemCategorySub::SQ_STAMP: return TEXT("SQ_STAMP");
		case ItemCategorySub::BOSS_COLLECTION: return TEXT("BOSS_COLLECTION");
		case ItemCategorySub::INVADE_CURRENCY: return TEXT("INVADE_CURRENCY");
		case ItemCategorySub::EVENT_CURRENCY: return TEXT("EVENT_CURRENCY");
		case ItemCategorySub::NORMAL_CURRENCY: return TEXT("NORMAL_CURRENCY");
		case ItemCategorySub::COIN_CURRENCY: return TEXT("COIN_CURRENCY");
		case ItemCategorySub::DISCOUNT_COUPON: return TEXT("DISCOUNT_COUPON");
		case ItemCategorySub::SEAL_STONE: return TEXT("SEAL_STONE");
		case ItemCategorySub::PARTYDUNGEON_KEY: return TEXT("PARTYDUNGEON_KEY");
		case ItemCategorySub::ETC: return TEXT("ETC");
		case ItemCategorySub::DELIVERY_BOX_EQUIP: return TEXT("DELIVERY_BOX_EQUIP");
		case ItemCategorySub::DELIVERY_BOX_COSTUME: return TEXT("DELIVERY_BOX_COSTUME");
		case ItemCategorySub::DELIVERY_BOX_FAIRY: return TEXT("DELIVERY_BOX_FAIRY");
		case ItemCategorySub::DEATH_PENALTY_RECOVERY: return TEXT("DEATH_PENALTY_RECOVERY");
		case ItemCategorySub::ELITE_DUNGEON_BALL: return TEXT("ELITE_DUNGEON_BALL");
		case ItemCategorySub::NICKNAME_CHANGE: return TEXT("NICKNAME_CHANGE");
	}
	return FString();
}
inline FString ItemGradeEnumToString(ItemGrade value)
{
	switch (value)
	{
		case ItemGrade::NONE: return TEXT("NONE");
		case ItemGrade::NORMAL: return TEXT("NORMAL");
		case ItemGrade::MAGIC: return TEXT("MAGIC");
		case ItemGrade::RARE: return TEXT("RARE");
		case ItemGrade::EPIC: return TEXT("EPIC");
		case ItemGrade::UNIQUE: return TEXT("UNIQUE");
		case ItemGrade::LEGEND: return TEXT("LEGEND");
		case ItemGrade::ETERNAL: return TEXT("ETERNAL");
	}
	return FString();
}
inline FString ItemMaterialEnumToString(ItemMaterial value)
{
	switch (value)
	{
		case ItemMaterial::FABRIC: return TEXT("FABRIC");
		case ItemMaterial::WOOD: return TEXT("WOOD");
		case ItemMaterial::BONE: return TEXT("BONE");
		case ItemMaterial::LEATHER: return TEXT("LEATHER");
		case ItemMaterial::STEEL: return TEXT("STEEL");
		case ItemMaterial::ADAMATIUM: return TEXT("ADAMATIUM");
		case ItemMaterial::MITHRIL: return TEXT("MITHRIL");
		case ItemMaterial::JEWEL: return TEXT("JEWEL");
		case ItemMaterial::ZENITE: return TEXT("ZENITE");
		case ItemMaterial::TRUE_SILVER: return TEXT("TRUE_SILVER");
		case ItemMaterial::ORICALCUM: return TEXT("ORICALCUM");
		case ItemMaterial::SHELL: return TEXT("SHELL");
		case ItemMaterial::SPIRIT: return TEXT("SPIRIT");
		case ItemMaterial::STONE: return TEXT("STONE");
		case ItemMaterial::BRONZE: return TEXT("BRONZE");
		case ItemMaterial::THORIUM: return TEXT("THORIUM");
	}
	return FString();
}
inline FString ItemEffectTypeEnumToString(ItemEffectType value)
{
	switch (value)
	{
		case ItemEffectType::NONE: return TEXT("NONE");
		case ItemEffectType::RESTORE_STAT: return TEXT("RESTORE_STAT");
		case ItemEffectType::ADD_ABNORMALITY_STAT: return TEXT("ADD_ABNORMALITY_STAT");
		case ItemEffectType::WARP_TO_TOWN: return TEXT("WARP_TO_TOWN");
		case ItemEffectType::WARP_TO_BOOKMARK: return TEXT("WARP_TO_BOOKMARK");
		case ItemEffectType::WARP_TO_RANDOM: return TEXT("WARP_TO_RANDOM");
		case ItemEffectType::GET_REWARD: return TEXT("GET_REWARD");
		case ItemEffectType::QUEST_REPEAT: return TEXT("QUEST_REPEAT");
		case ItemEffectType::RESET_BONUS_STAT: return TEXT("RESET_BONUS_STAT");
		case ItemEffectType::BONUS_STAT_POINT: return TEXT("BONUS_STAT_POINT");
		case ItemEffectType::DECOMPOSE_EQUIP_KIT: return TEXT("DECOMPOSE_EQUIP_KIT");
		case ItemEffectType::DECOMPOSE_SKILLBOOK_KIT: return TEXT("DECOMPOSE_SKILLBOOK_KIT");
		case ItemEffectType::SKILL_BOOK: return TEXT("SKILL_BOOK");
		case ItemEffectType::ENCHANT_SKILL: return TEXT("ENCHANT_SKILL");
		case ItemEffectType::SUMMON_FAIRY: return TEXT("SUMMON_FAIRY");
		case ItemEffectType::ARCAS_POINT: return TEXT("ARCAS_POINT");
		case ItemEffectType::OPTION_STONE: return TEXT("OPTION_STONE");
		case ItemEffectType::BLESSED_OPTION_STONE: return TEXT("BLESSED_OPTION_STONE");
		case ItemEffectType::PVP_ALIGNMENT_POINT: return TEXT("PVP_ALIGNMENT_POINT");
		case ItemEffectType::SUMMON_COSTUME: return TEXT("SUMMON_COSTUME");
		case ItemEffectType::GUILD_GIFT: return TEXT("GUILD_GIFT");
		case ItemEffectType::SPAWN_TREASURE_MONSTER: return TEXT("SPAWN_TREASURE_MONSTER");
		case ItemEffectType::GAIN_MONSTER_KNOWLEDGE_EXP: return TEXT("GAIN_MONSTER_KNOWLEDGE_EXP");
		case ItemEffectType::UNLOCK_MONSTER_KNOWLEDGE_SLATE: return TEXT("UNLOCK_MONSTER_KNOWLEDGE_SLATE");
		case ItemEffectType::RESET_MONSTER_KNOWLEDGE_SLATE_SLOT: return TEXT("RESET_MONSTER_KNOWLEDGE_SLATE_SLOT");
		case ItemEffectType::SPAWN_CAMP: return TEXT("SPAWN_CAMP");
		case ItemEffectType::SPAWN_INVADE_PORTAL: return TEXT("SPAWN_INVADE_PORTAL");
		case ItemEffectType::ITEM_ENCHANT: return TEXT("ITEM_ENCHANT");
		case ItemEffectType::WARP_TO_DUNGEON: return TEXT("WARP_TO_DUNGEON");
		case ItemEffectType::DUNGEON_TIME_CHARGE: return TEXT("DUNGEON_TIME_CHARGE");
		case ItemEffectType::CHARACTER_SLOT_UNLOCK: return TEXT("CHARACTER_SLOT_UNLOCK");
		case ItemEffectType::DELIVERY_BOX: return TEXT("DELIVERY_BOX");
		case ItemEffectType::CHARACTER_CUSTOMIZING: return TEXT("CHARACTER_CUSTOMIZING");
		case ItemEffectType::SPAWN_MONSTER: return TEXT("SPAWN_MONSTER");
		case ItemEffectType::DISCOUNT_COUPON: return TEXT("DISCOUNT_COUPON");
		case ItemEffectType::SOCIAL_EMOTION: return TEXT("SOCIAL_EMOTION");
		case ItemEffectType::DEATH_PENALTY_RECOVERY: return TEXT("DEATH_PENALTY_RECOVERY");
		case ItemEffectType::CHANGE_COSTUME: return TEXT("CHANGE_COSTUME");
		case ItemEffectType::GET_ABNORMALITY_STAT_AND_REWARD: return TEXT("GET_ABNORMALITY_STAT_AND_REWARD");
		case ItemEffectType::TICKET_NICKNAME_CHANGE: return TEXT("TICKET_NICKNAME_CHANGE");
		case ItemEffectType::MAGICAL_FORGE: return TEXT("MAGICAL_FORGE");
		case ItemEffectType::EXE_SKILL_NOTIFY: return TEXT("EXE_SKILL_NOTIFY");
		case ItemEffectType::WARP_TO_AGIT: return TEXT("WARP_TO_AGIT");
	}
	return FString();
}
inline FString ChangeWeaponTypeFinishReasonEnumToString(ChangeWeaponTypeFinishReason value)
{
	switch (value)
	{
		case ChangeWeaponTypeFinishReason::NORMAL: return TEXT("NORMAL");
		case ChangeWeaponTypeFinishReason::DIE: return TEXT("DIE");
		case ChangeWeaponTypeFinishReason::WARP: return TEXT("WARP");
		case ChangeWeaponTypeFinishReason::ABNORMALITY: return TEXT("ABNORMALITY");
		case ChangeWeaponTypeFinishReason::INVALID_DATA: return TEXT("INVALID_DATA");
	}
	return FString();
}
inline FString ItemEnchantResultEnumToString(ItemEnchantResult value)
{
	switch (value)
	{
		case ItemEnchantResult::NONE: return TEXT("NONE");
		case ItemEnchantResult::ENCHANT_SUCCEEDED: return TEXT("ENCHANT_SUCCEEDED");
		case ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED: return TEXT("ENCHANT_SUCCEEDED_CURSED");
		case ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED: return TEXT("ENCHANT_SUCCEEDED_BLESSED");
		case ItemEnchantResult::ENCHANT_FAILED: return TEXT("ENCHANT_FAILED");
		case ItemEnchantResult::ENCHANT_FAILED_SHIELDED: return TEXT("ENCHANT_FAILED_SHIELDED");
		case ItemEnchantResult::ENCHANT_FAILED_DESTROYED: return TEXT("ENCHANT_FAILED_DESTROYED");
		case ItemEnchantResult::ENCHANT_PRESERVED: return TEXT("ENCHANT_PRESERVED");
		case ItemEnchantResult::ENCHANT_FAILED_DOWN: return TEXT("ENCHANT_FAILED_DOWN");
	}
	return FString();
}
inline FString ItemAcquireTypeEnumToString(ItemAcquireType value)
{
	switch (value)
	{
		case ItemAcquireType::IN_GAME: return TEXT("IN_GAME");
		case ItemAcquireType::PAID: return TEXT("PAID");
	}
	return FString();
}
inline FString ItemRefineResultEnumToString(ItemRefineResult value)
{
	switch (value)
	{
		case ItemRefineResult::NONE: return TEXT("NONE");
		case ItemRefineResult::REFINE_SUCCEEDED: return TEXT("REFINE_SUCCEEDED");
		case ItemRefineResult::REFINE_SUCCEEDED_BONUS: return TEXT("REFINE_SUCCEEDED_BONUS");
	}
	return FString();
}
inline FString ItemMagicalForgeResultEnumToString(ItemMagicalForgeResult value)
{
	switch (value)
	{
		case ItemMagicalForgeResult::NONE: return TEXT("NONE");
		case ItemMagicalForgeResult::MAGICAL_FORGE_SUCCEEDED: return TEXT("MAGICAL_FORGE_SUCCEEDED");
	}
	return FString();
}
inline FString ItemCollectionTypeEnumToString(ItemCollectionType value)
{
	switch (value)
	{
		case ItemCollectionType::NONE: return TEXT("NONE");
		case ItemCollectionType::PERMANENT: return TEXT("PERMANENT");
		case ItemCollectionType::FIXED_TERM: return TEXT("FIXED_TERM");
	}
	return FString();
}
inline FString ItemCollectionUnlockTypeEnumToString(ItemCollectionUnlockType value)
{
	switch (value)
	{
		case ItemCollectionUnlockType::NONE: return TEXT("NONE");
		case ItemCollectionUnlockType::LEVEL: return TEXT("LEVEL");
		case ItemCollectionUnlockType::QUEST: return TEXT("QUEST");
	}
	return FString();
}
inline FString ItemCollectionEventTypeEnumToString(ItemCollectionEventType value)
{
	switch (value)
	{
		case ItemCollectionEventType::NONE: return TEXT("NONE");
		case ItemCollectionEventType::NORMAL: return TEXT("NORMAL");
		case ItemCollectionEventType::EVENT: return TEXT("EVENT");
	}
	return FString();
}
inline FString CraftResultEnumToString(CraftResult value)
{
	switch (value)
	{
		case CraftResult::NONE: return TEXT("NONE");
		case CraftResult::CRAFT_SUCCEEDED: return TEXT("CRAFT_SUCCEEDED");
		case CraftResult::CRAFT_SUCCEEDED_GREAT: return TEXT("CRAFT_SUCCEEDED_GREAT");
		case CraftResult::CRAFT_FAIL: return TEXT("CRAFT_FAIL");
	}
	return FString();
}
inline FString AutoPurchaseTypeEnumToString(AutoPurchaseType value)
{
	switch (value)
	{
		case AutoPurchaseType::AUTO_SHOP_BUY: return TEXT("AUTO_SHOP_BUY");
		case AutoPurchaseType::AUTO_SHOP_SELL_CONSUME: return TEXT("AUTO_SHOP_SELL_CONSUME");
		case AutoPurchaseType::AUTO_SHOP_SELL_EQUIP: return TEXT("AUTO_SHOP_SELL_EQUIP");
		case AutoPurchaseType::AUTO_DEPOT_KEEP: return TEXT("AUTO_DEPOT_KEEP");
		case AutoPurchaseType::AUTO_DEPOT_KEEP_GROUP: return TEXT("AUTO_DEPOT_KEEP_GROUP");
	}
	return FString();
}
inline FString GuildGiftGiveTypeEnumToString(GuildGiftGiveType value)
{
	switch (value)
	{
		case GuildGiftGiveType::NONE: return TEXT("NONE");
		case GuildGiftGiveType::GIVE_TO_BUYER: return TEXT("GIVE_TO_BUYER");
		case GuildGiftGiveType::GIVE_TO_GUILD_MEMBERS: return TEXT("GIVE_TO_GUILD_MEMBERS");
	}
	return FString();
}
inline FString TickerRangeEnumToString(TickerRange value)
{
	switch (value)
	{
		case TickerRange::NONE: return TEXT("NONE");
		case TickerRange::MAP: return TEXT("MAP");
		case TickerRange::ALL: return TEXT("ALL");
		case TickerRange::SELF: return TEXT("SELF");
	}
	return FString();
}
inline FString ItemCraftUnlockTypeEnumToString(ItemCraftUnlockType value)
{
	switch (value)
	{
		case ItemCraftUnlockType::NONE: return TEXT("NONE");
		case ItemCraftUnlockType::LEVEL: return TEXT("LEVEL");
		case ItemCraftUnlockType::QUEST: return TEXT("QUEST");
	}
	return FString();
}
inline FString ItemCraftRestrictEnumToString(ItemCraftRestrict value)
{
	switch (value)
	{
		case ItemCraftRestrict::NONE: return TEXT("NONE");
		case ItemCraftRestrict::CHARACTER: return TEXT("CHARACTER");
		case ItemCraftRestrict::ACCOUNT: return TEXT("ACCOUNT");
		case ItemCraftRestrict::SERVER: return TEXT("SERVER");
	}
	return FString();
}
inline FString ItemEnchantTypeEnumToString(ItemEnchantType value)
{
	switch (value)
	{
		case ItemEnchantType::NORMAL_ENCHANT: return TEXT("NORMAL_ENCHANT");
		case ItemEnchantType::BLESSED_ENCHANT: return TEXT("BLESSED_ENCHANT");
		case ItemEnchantType::CURSED_ENCHANT: return TEXT("CURSED_ENCHANT");
	}
	return FString();
}
inline FString DropItemEffectEnumToString(DropItemEffect value)
{
	switch (value)
	{
		case DropItemEffect::NORAM_TYPE: return TEXT("NORAM_TYPE");
		case DropItemEffect::RANDOM_HIT_TYPE: return TEXT("RANDOM_HIT_TYPE");
		case DropItemEffect::LAST_HIT_TYPE: return TEXT("LAST_HIT_TYPE");
	}
	return FString();
}
inline FString ItemCouponDiscountTypeEnumToString(ItemCouponDiscountType value)
{
	switch (value)
	{
		case ItemCouponDiscountType::CONSTANT: return TEXT("CONSTANT");
		case ItemCouponDiscountType::COEFFICIENT: return TEXT("COEFFICIENT");
	}
	return FString();
}
inline FString SealSlotColorEnumToString(SealSlotColor value)
{
	switch (value)
	{
		case SealSlotColor::NONE: return TEXT("NONE");
		case SealSlotColor::RARE: return TEXT("RARE");
		case SealSlotColor::EPIC: return TEXT("EPIC");
		case SealSlotColor::UNIQUE: return TEXT("UNIQUE");
	}
	return FString();
}
inline FString ItemUpdateReasonEnumToString(ItemUpdateReason value)
{
	switch (value)
	{
		case ItemUpdateReason::CREATE_ITEM: return TEXT("CREATE_ITEM");
		case ItemUpdateReason::CREATE_ITEM_ADMIN_COMMAND: return TEXT("CREATE_ITEM_ADMIN_COMMAND");
		case ItemUpdateReason::ITEM_DELETE: return TEXT("ITEM_DELETE");
		case ItemUpdateReason::ITEM_DELETE_THROW_AWAY: return TEXT("ITEM_DELETE_THROW_AWAY");
		case ItemUpdateReason::ITEM_DELETE_ADMIN_COMMAND: return TEXT("ITEM_DELETE_ADMIN_COMMAND");
		case ItemUpdateReason::ITEM_DELETE_WARP_BOOK_MARK: return TEXT("ITEM_DELETE_WARP_BOOK_MARK");
		case ItemUpdateReason::DEATH_PANALTY_RESTORE: return TEXT("DEATH_PANALTY_RESTORE");
		case ItemUpdateReason::ACHIEVEMENT_REWARD: return TEXT("ACHIEVEMENT_REWARD");
		case ItemUpdateReason::ACHIEVEMENT_REWARDALL: return TEXT("ACHIEVEMENT_REWARDALL");
		case ItemUpdateReason::ACHIEVEMENT_REWARDTHEME: return TEXT("ACHIEVEMENT_REWARDTHEME");
		case ItemUpdateReason::ARCAS_UPDATEPOINT: return TEXT("ARCAS_UPDATEPOINT");
		case ItemUpdateReason::PVPALIGNMENT_UPDATEPOINT: return TEXT("PVPALIGNMENT_UPDATEPOINT");
		case ItemUpdateReason::ATTENDANCE_PURCHASE: return TEXT("ATTENDANCE_PURCHASE");
		case ItemUpdateReason::BATTLEPASS_ACTIVATE: return TEXT("BATTLEPASS_ACTIVATE");
		case ItemUpdateReason::BMSHOP_PURCHASE: return TEXT("BMSHOP_PURCHASE");
		case ItemUpdateReason::BMSHOP_RECIVE: return TEXT("BMSHOP_RECIVE");
		case ItemUpdateReason::COSTUME_SUMMON: return TEXT("COSTUME_SUMMON");
		case ItemUpdateReason::COSTUME_COMPOSE: return TEXT("COSTUME_COMPOSE");
		case ItemUpdateReason::COSTUME_CEILING: return TEXT("COSTUME_CEILING");
		case ItemUpdateReason::COSTUME_CHANGE: return TEXT("COSTUME_CHANGE");
		case ItemUpdateReason::USER_DEATH: return TEXT("USER_DEATH");
		case ItemUpdateReason::FAIRY_ENCHANT: return TEXT("FAIRY_ENCHANT");
		case ItemUpdateReason::FAIRY_SUMMON: return TEXT("FAIRY_SUMMON");
		case ItemUpdateReason::FAIRY_COMPOSE: return TEXT("FAIRY_COMPOSE");
		case ItemUpdateReason::FAIRY_CEILING: return TEXT("FAIRY_CEILING");
		case ItemUpdateReason::GUILD_BUYSHOP: return TEXT("GUILD_BUYSHOP");
		case ItemUpdateReason::GUILD_GUILDGIFT: return TEXT("GUILD_GUILDGIFT");
		case ItemUpdateReason::GUILD_DAILY_ACTIVITY: return TEXT("GUILD_DAILY_ACTIVITY");
		case ItemUpdateReason::GUILD_ITEMCOLLECTION: return TEXT("GUILD_ITEMCOLLECTION");
		case ItemUpdateReason::ITEM_BONUSSTATRESET: return TEXT("ITEM_BONUSSTATRESET");
		case ItemUpdateReason::ITEM_BONUSSTATPOINT: return TEXT("ITEM_BONUSSTATPOINT");
		case ItemUpdateReason::ITEM_BUY: return TEXT("ITEM_BUY");
		case ItemUpdateReason::ITEM_COLLECTION: return TEXT("ITEM_COLLECTION");
		case ItemUpdateReason::ITEM_CRAFT: return TEXT("ITEM_CRAFT");
		case ItemUpdateReason::ITEM_DELETE_EXPIRED: return TEXT("ITEM_DELETE_EXPIRED");
		case ItemUpdateReason::ITEM_DELIVERY_BOX_COSTUME: return TEXT("ITEM_DELIVERY_BOX_COSTUME");
		case ItemUpdateReason::ITEM_DELIVERY_BOX_EQUIP: return TEXT("ITEM_DELIVERY_BOX_EQUIP");
		case ItemUpdateReason::ITEM_DELIVERY_BOX_FAIRY: return TEXT("ITEM_DELIVERY_BOX_FAIRY");
		case ItemUpdateReason::ITEM_ENCHANT: return TEXT("ITEM_ENCHANT");
		case ItemUpdateReason::ENCHANT_FAILED_DESTORYED: return TEXT("ENCHANT_FAILED_DESTORYED");
		case ItemUpdateReason::ITEM_GETREWARD: return TEXT("ITEM_GETREWARD");
		case ItemUpdateReason::ITEM_DECOMPOSE: return TEXT("ITEM_DECOMPOSE");
		case ItemUpdateReason::ITEM_MOVE_DEPOT_TO_INVENTORY: return TEXT("ITEM_MOVE_DEPOT_TO_INVENTORY");
		case ItemUpdateReason::ITEM_MOVE_INVEN_TO_DEPOT: return TEXT("ITEM_MOVE_INVEN_TO_DEPOT");
		case ItemUpdateReason::ITEM_QUEST_REPEAT: return TEXT("ITEM_QUEST_REPEAT");
		case ItemUpdateReason::ITEM_REFINE: return TEXT("ITEM_REFINE");
		case ItemUpdateReason::ITEM_SELECT_REWARD: return TEXT("ITEM_SELECT_REWARD");
		case ItemUpdateReason::ITEM_SELL: return TEXT("ITEM_SELL");
		case ItemUpdateReason::GUILD_STORE: return TEXT("GUILD_STORE");
		case ItemUpdateReason::ITEM_SPAWN_CAMP: return TEXT("ITEM_SPAWN_CAMP");
		case ItemUpdateReason::ITEM_SPAWN_TREASURE_MONSTER: return TEXT("ITEM_SPAWN_TREASURE_MONSTER");
		case ItemUpdateReason::ITEM_USE: return TEXT("ITEM_USE");
		case ItemUpdateReason::ITEM_EQUIP_CHANGED: return TEXT("ITEM_EQUIP_CHANGED");
		case ItemUpdateReason::MAIL_RECV: return TEXT("MAIL_RECV");
		case ItemUpdateReason::MARKET_TRANSACTIONCANCEL: return TEXT("MARKET_TRANSACTIONCANCEL");
		case ItemUpdateReason::MARKET_TRANSACTIONREGISTER: return TEXT("MARKET_TRANSACTIONREGISTER");
		case ItemUpdateReason::MARKET_TRANSACTIONPURCHASE: return TEXT("MARKET_TRANSACTIONPURCHASE");
		case ItemUpdateReason::MONSTER_KNOWLEDGE_EXP_GAIN: return TEXT("MONSTER_KNOWLEDGE_EXP_GAIN");
		case ItemUpdateReason::MONSTER_KNOWLEDGE_EXP_GAIN_ALL: return TEXT("MONSTER_KNOWLEDGE_EXP_GAIN_ALL");
		case ItemUpdateReason::MONSTER_KNOWLEDGE_SLATE_RESET: return TEXT("MONSTER_KNOWLEDGE_SLATE_RESET");
		case ItemUpdateReason::MONSTER_KNOWLEDGE_SLATE_SLOT: return TEXT("MONSTER_KNOWLEDGE_SLATE_SLOT");
		case ItemUpdateReason::MULTILEVELRANK_CHANGE: return TEXT("MULTILEVELRANK_CHANGE");
		case ItemUpdateReason::MULTILEVELRANK_PROTOMTION: return TEXT("MULTILEVELRANK_PROTOMTION");
		case ItemUpdateReason::MULTILEVELRANK_REWARD: return TEXT("MULTILEVELRANK_REWARD");
		case ItemUpdateReason::NPC_EVENT_SHOP_PURCHASE: return TEXT("NPC_EVENT_SHOP_PURCHASE");
		case ItemUpdateReason::QUEST_REWARD: return TEXT("QUEST_REWARD");
		case ItemUpdateReason::SKILL_ENCHANT: return TEXT("SKILL_ENCHANT");
		case ItemUpdateReason::SKILL_LEARN: return TEXT("SKILL_LEARN");
		case ItemUpdateReason::CHARACTER_SLOT_UNLOCK: return TEXT("CHARACTER_SLOT_UNLOCK");
		case ItemUpdateReason::USER_INVADE_PENALTY: return TEXT("USER_INVADE_PENALTY");
		case ItemUpdateReason::PVP_ALIGNMENT_UPDATE_POINT: return TEXT("PVP_ALIGNMENT_UPDATE_POINT");
		case ItemUpdateReason::PARTY_DUNGEON_CLEAR: return TEXT("PARTY_DUNGEON_CLEAR");
		case ItemUpdateReason::BOSS_KILL: return TEXT("BOSS_KILL");
		case ItemUpdateReason::DROP_GET: return TEXT("DROP_GET");
		case ItemUpdateReason::MAP_EVENT_API: return TEXT("MAP_EVENT_API");
		case ItemUpdateReason::NPC_REWARD: return TEXT("NPC_REWARD");
		case ItemUpdateReason::CHARACTER_CUSTOMIZING: return TEXT("CHARACTER_CUSTOMIZING");
		case ItemUpdateReason::INVADE_PORTAL_ACTIVATE: return TEXT("INVADE_PORTAL_ACTIVATE");
		case ItemUpdateReason::MAKE_CHARACTER: return TEXT("MAKE_CHARACTER");
		case ItemUpdateReason::HVE_ITEM_GVIE_RETRIVE: return TEXT("HVE_ITEM_GVIE_RETRIVE");
		case ItemUpdateReason::ITEM_SPAWN_MONSTER: return TEXT("ITEM_SPAWN_MONSTER");
		case ItemUpdateReason::WORLD_BOSS_CLEAR_REWARD: return TEXT("WORLD_BOSS_CLEAR_REWARD");
		case ItemUpdateReason::WORLD_BOSS_BASIC_REWARD: return TEXT("WORLD_BOSS_BASIC_REWARD");
		case ItemUpdateReason::WORLD_BOSS_CONSOLATION_REWARD: return TEXT("WORLD_BOSS_CONSOLATION_REWARD");
		case ItemUpdateReason::SEAL_SLOT_CHANGE_ALL: return TEXT("SEAL_SLOT_CHANGE_ALL");
		case ItemUpdateReason::SEAL_LEVEL_UP: return TEXT("SEAL_LEVEL_UP");
		case ItemUpdateReason::MONSTER_KNOWLEDGE_EXP_SURPLUS_REWARD: return TEXT("MONSTER_KNOWLEDGE_EXP_SURPLUS_REWARD");
		case ItemUpdateReason::INVADE_TREASUREBOX_REWARD: return TEXT("INVADE_TREASUREBOX_REWARD");
		case ItemUpdateReason::PERSONAL_TRADING: return TEXT("PERSONAL_TRADING");
		case ItemUpdateReason::INVADE_PENALTY: return TEXT("INVADE_PENALTY");
		case ItemUpdateReason::BATTLEPASS_REWARD: return TEXT("BATTLEPASS_REWARD");
		case ItemUpdateReason::ITEM_DEATH_PENALTY_RECOVERY: return TEXT("ITEM_DEATH_PENALTY_RECOVERY");
		case ItemUpdateReason::BATTLEPASS_REWARD_ALL: return TEXT("BATTLEPASS_REWARD_ALL");
		case ItemUpdateReason::ATTENDANCE_UPDATE: return TEXT("ATTENDANCE_UPDATE");
		case ItemUpdateReason::RESTORE_ITEM_ADMIN_COMMAND: return TEXT("RESTORE_ITEM_ADMIN_COMMAND");
		case ItemUpdateReason::SUMMON_FAIRY_MILEAGE: return TEXT("SUMMON_FAIRY_MILEAGE");
		case ItemUpdateReason::SUMMON_COSTUME_MILEAGE: return TEXT("SUMMON_COSTUME_MILEAGE");
		case ItemUpdateReason::SPIRIT_SHOT_ENCHANT: return TEXT("SPIRIT_SHOT_ENCHANT");
		case ItemUpdateReason::COMMUNITY_EVENT_MISSION: return TEXT("COMMUNITY_EVENT_MISSION");
		case ItemUpdateReason::DURAL: return TEXT("DURAL");
		case ItemUpdateReason::ITEM_FUSE: return TEXT("ITEM_FUSE");
		case ItemUpdateReason::ITEM_NICKNAME_CHANGE: return TEXT("ITEM_NICKNAME_CHANGE");
		case ItemUpdateReason::SLATE_SPECIAL_NODE_ACTIVATE: return TEXT("SLATE_SPECIAL_NODE_ACTIVATE");
		case ItemUpdateReason::DROP_GET_INVADE_BOSS_KILL: return TEXT("DROP_GET_INVADE_BOSS_KILL");
		case ItemUpdateReason::DROP_GET_INVADE_WAGON_KILL: return TEXT("DROP_GET_INVADE_WAGON_KILL");
		case ItemUpdateReason::DROP_GET_INVADE_GUARDIAN_KILL: return TEXT("DROP_GET_INVADE_GUARDIAN_KILL");
		case ItemUpdateReason::DROP_GET_INVADE_PK_DIE_PENALTY: return TEXT("DROP_GET_INVADE_PK_DIE_PENALTY");
		case ItemUpdateReason::DROP_GET_INVADE_HOME_WARP_PENALTY: return TEXT("DROP_GET_INVADE_HOME_WARP_PENALTY");
		case ItemUpdateReason::ITEM_MAGICAL_FORGE: return TEXT("ITEM_MAGICAL_FORGE");
		case ItemUpdateReason::AGIT_RENTAL: return TEXT("AGIT_RENTAL");
		case ItemUpdateReason::AGIT_ENTER: return TEXT("AGIT_ENTER");
		case ItemUpdateReason::AGIT_LEAVE: return TEXT("AGIT_LEAVE");
		case ItemUpdateReason::ITEM_REWARD_GUILD_WAR: return TEXT("ITEM_REWARD_GUILD_WAR");
		case ItemUpdateReason::SUMMON_UNIQUE_FAIRY_MILEAGE: return TEXT("SUMMON_UNIQUE_FAIRY_MILEAGE");
		case ItemUpdateReason::SUMMON_UNIQUE_COSTUME_MILEAGE: return TEXT("SUMMON_UNIQUE_COSTUME_MILEAGE");
		case ItemUpdateReason::AUTO_DECOMPOSE: return TEXT("AUTO_DECOMPOSE");
	}
	return FString();
}
inline FString ItemGainTypeEnumToString(ItemGainType value)
{
	switch (value)
	{
		case ItemGainType::NONE: return TEXT("NONE");
		case ItemGainType::CONSUME_NORMAL: return TEXT("CONSUME_NORMAL");
		case ItemGainType::CONSUME_SPECIAL: return TEXT("CONSUME_SPECIAL");
		case ItemGainType::CONSTUME_SCROLL_NORMAL: return TEXT("CONSTUME_SCROLL_NORMAL");
		case ItemGainType::CONSTUME_SCROLL_SPECIAL: return TEXT("CONSTUME_SCROLL_SPECIAL");
		case ItemGainType::CONSTUME_SCROLL_ENCHANT: return TEXT("CONSTUME_SCROLL_ENCHANT");
		case ItemGainType::OTHER_NORMAL: return TEXT("OTHER_NORMAL");
		case ItemGainType::OTHER_SPECIAL: return TEXT("OTHER_SPECIAL");
		case ItemGainType::OTHER_ETC: return TEXT("OTHER_ETC");
	}
	return FString();
}
inline FString ItemDecomposeTypeEnumToString(ItemDecomposeType value)
{
	switch (value)
	{
		case ItemDecomposeType::NONE: return TEXT("NONE");
		case ItemDecomposeType::ITEM_ID: return TEXT("ITEM_ID");
		case ItemDecomposeType::ITEM_CATEGORY_MAIN: return TEXT("ITEM_CATEGORY_MAIN");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString ItemTypeEnumToDisplayKey(ItemType value)
{
	switch (value)
	{
		case ItemType::NONE: return TEXT("");
		case ItemType::EQUIP: return TEXT("");
		case ItemType::INGREDIENT: return TEXT("");
		case ItemType::CONSUME: return TEXT("");
		case ItemType::CURRENCY: return TEXT("");
		case ItemType::ETC: return TEXT("");
	}
	return FString();
}
inline FString ItemCategoryMainEnumToDisplayKey(ItemCategoryMain value)
{
	switch (value)
	{
		case ItemCategoryMain::NONE: return TEXT("");
		case ItemCategoryMain::WEAPON: return TEXT("");
		case ItemCategoryMain::ARMOR: return TEXT("");
		case ItemCategoryMain::ACCESSORY: return TEXT("");
		case ItemCategoryMain::POTION: return TEXT("");
		case ItemCategoryMain::SKILLBOOK: return TEXT("");
		case ItemCategoryMain::SCROLL: return TEXT("");
		case ItemCategoryMain::FOOD: return TEXT("");
		case ItemCategoryMain::FAIRY: return TEXT("");
		case ItemCategoryMain::COSTUME: return TEXT("");
		case ItemCategoryMain::CRAFT: return TEXT("");
		case ItemCategoryMain::QUEST: return TEXT("");
		case ItemCategoryMain::TICKET: return TEXT("");
		case ItemCategoryMain::MONSTER_KNOWLEDGE: return TEXT("");
		case ItemCategoryMain::MONSTER_KNOWLEDGE_SLATE: return TEXT("");
		case ItemCategoryMain::COLLECTABLE: return TEXT("");
		case ItemCategoryMain::CHARGE: return TEXT("");
		case ItemCategoryMain::CURRENCY: return TEXT("");
		case ItemCategoryMain::ETC: return TEXT("");
		case ItemCategoryMain::DELIVERY_BOX: return TEXT("");
		case ItemCategoryMain::SEAL: return TEXT("");
		case ItemCategoryMain::SPECIAL: return TEXT("");
	}
	return FString();
}
inline FString ItemCategorySubEnumToDisplayKey(ItemCategorySub value)
{
	switch (value)
	{
		case ItemCategorySub::NONE: return TEXT("");
		case ItemCategorySub::SWORD: return TEXT("");
		case ItemCategorySub::DAGGER: return TEXT("");
		case ItemCategorySub::BOW: return TEXT("");
		case ItemCategorySub::WAND: return TEXT("");
		case ItemCategorySub::WAEPON_ORB: return TEXT("");
		case ItemCategorySub::SHOTGUN: return TEXT("");
		case ItemCategorySub::HELMET: return TEXT("");
		case ItemCategorySub::BODY_ARMOR: return TEXT("");
		case ItemCategorySub::GLOVE: return TEXT("");
		case ItemCategorySub::SHOES: return TEXT("");
		case ItemCategorySub::BELT: return TEXT("");
		case ItemCategorySub::SCARF: return TEXT("");
		case ItemCategorySub::NECKLACE: return TEXT("");
		case ItemCategorySub::EAR_RING: return TEXT("");
		case ItemCategorySub::RING: return TEXT("");
		case ItemCategorySub::CHARM: return TEXT("");
		case ItemCategorySub::ORB: return TEXT("");
		case ItemCategorySub::ORB_2: return TEXT("");
		case ItemCategorySub::BRACELET: return TEXT("");
		case ItemCategorySub::DURAL: return TEXT("");
		case ItemCategorySub::MARBLE: return TEXT("");
		case ItemCategorySub::MAX_EQUIP: return TEXT("");
		case ItemCategorySub::HP_POTION: return TEXT("");
		case ItemCategorySub::HP_POTION_MANUAL: return TEXT("");
		case ItemCategorySub::MP_TICK_RECOVERY_POTION: return TEXT("");
		case ItemCategorySub::MP_POTION: return TEXT("");
		case ItemCategorySub::POTION_BUFF: return TEXT("");
		case ItemCategorySub::BONUS_STAT_POINT: return TEXT("");
		case ItemCategorySub::COMMON_SKILL_BOOK: return TEXT("");
		case ItemCategorySub::SWORD_SKILL_BOOK: return TEXT("");
		case ItemCategorySub::DAGGER_SKILL_BOOK: return TEXT("");
		case ItemCategorySub::BOW_SKILL_BOOK: return TEXT("");
		case ItemCategorySub::WAND_SKILL_BOOK: return TEXT("");
		case ItemCategorySub::ORB_SKILL_BOOK: return TEXT("");
		case ItemCategorySub::SHOTGUN_SKILL_BOOK: return TEXT("");
		case ItemCategorySub::ENCHANT_SKILL_SCROLL: return TEXT("");
		case ItemCategorySub::WARP_TO_TOWN_SCROLL: return TEXT("");
		case ItemCategorySub::WARP_TO_BOOKMARK_SCROLL: return TEXT("");
		case ItemCategorySub::WARP_TO_RANDOM_SCROLL: return TEXT("");
		case ItemCategorySub::WARP_TO_AGIT: return TEXT("");
		case ItemCategorySub::ENCHANT_WEAPON_SCROLL: return TEXT("");
		case ItemCategorySub::BLESSED_ENCHANT_WEAPON: return TEXT("");
		case ItemCategorySub::CURSED_ENCHANT_WEAPON: return TEXT("");
		case ItemCategorySub::ENCHANT_ARMOR: return TEXT("");
		case ItemCategorySub::BLESSED_ENCHANT_ARMOR: return TEXT("");
		case ItemCategorySub::CURSED_ENCHANT_ARMOR: return TEXT("");
		case ItemCategorySub::ENCHANT_ACCESSORY: return TEXT("");
		case ItemCategorySub::BLESSED_ENCHANT_ACCESSORY: return TEXT("");
		case ItemCategorySub::CURSED_ENCHANT_ACCESSORY: return TEXT("");
		case ItemCategorySub::SCROLL_BUFF: return TEXT("");
		case ItemCategorySub::STATUE_BUFF: return TEXT("");
		case ItemCategorySub::OPTION_STONE: return TEXT("");
		case ItemCategorySub::BLESSED_OPTION_STONE: return TEXT("");
		case ItemCategorySub::RESET_BONUS_STAT: return TEXT("");
		case ItemCategorySub::MAGICAL_FORGE: return TEXT("");
		case ItemCategorySub::SKILL_SCROLL: return TEXT("");
		case ItemCategorySub::FOOD_BUFF: return TEXT("");
		case ItemCategorySub::SUMMON_FAIRY: return TEXT("");
		case ItemCategorySub::ENCHANT_FAIRY: return TEXT("");
		case ItemCategorySub::FAIRY_CEILING: return TEXT("");
		case ItemCategorySub::SUMMON_COSTUME: return TEXT("");
		case ItemCategorySub::COSTUME_CEILING: return TEXT("");
		case ItemCategorySub::CHANGE_COSTUME_RANDOM: return TEXT("");
		case ItemCategorySub::CHANGE_COSTUME_SELECT: return TEXT("");
		case ItemCategorySub::INGREDIENT_CRAFT: return TEXT("");
		case ItemCategorySub::MATERIALS: return TEXT("");
		case ItemCategorySub::ESSENCE: return TEXT("");
		case ItemCategorySub::RECIPE: return TEXT("");
		case ItemCategorySub::INK: return TEXT("");
		case ItemCategorySub::MYSTERIOUS_POWDER: return TEXT("");
		case ItemCategorySub::QUEST_REPEAT: return TEXT("");
		case ItemCategorySub::TICKET_QUEST_GUILD_KNIGHT_COMPLETE: return TEXT("");
		case ItemCategorySub::TICKET_CHARACTER_SLOT_UNLOCK: return TEXT("");
		case ItemCategorySub::TICKET_CHARACTER_CUSTOMIZING: return TEXT("");
		case ItemCategorySub::MONSTER_KNOWLEDGE_GAIN_EXP: return TEXT("");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_ATTACK: return TEXT("");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_HIT: return TEXT("");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_AMPLIFICATION: return TEXT("");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_DEFENCE: return TEXT("");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_RESISTANCE: return TEXT("");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_UNLOCK_SURVIVAL: return TEXT("");
		case ItemCategorySub::MONSTER_KNOWLEDGE_SLATE_RESET: return TEXT("");
		case ItemCategorySub::DECOMPOSE_EQUIP_KIT: return TEXT("");
		case ItemCategorySub::DECOMPOSE_SKILLBOOK_KIT: return TEXT("");
		case ItemCategorySub::REWARD_BOX: return TEXT("");
		case ItemCategorySub::ARROW: return TEXT("");
		case ItemCategorySub::SPIRIT_SHOT: return TEXT("");
		case ItemCategorySub::MAGIC_STONE: return TEXT("");
		case ItemCategorySub::SPELL_STONE: return TEXT("");
		case ItemCategorySub::ARCAS: return TEXT("");
		case ItemCategorySub::PVP_ALIGNMENT: return TEXT("");
		case ItemCategorySub::CHEAT: return TEXT("");
		case ItemCategorySub::SPAWN_TREASURE_MONSTER: return TEXT("");
		case ItemCategorySub::GUILD_GIFT_BOX: return TEXT("");
		case ItemCategorySub::SPAWN_CAMP: return TEXT("");
		case ItemCategorySub::GODS_BLESS: return TEXT("");
		case ItemCategorySub::ENCHANT_COLLECTABLE_SCROLL: return TEXT("");
		case ItemCategorySub::ELITE_DUNGEON_TICKET: return TEXT("");
		case ItemCategorySub::DUNGEON_TIME: return TEXT("");
		case ItemCategorySub::INVADE_PORTAL: return TEXT("");
		case ItemCategorySub::SPAWN_MONSTER: return TEXT("");
		case ItemCategorySub::SOCIAL_EMOTION: return TEXT("");
		case ItemCategorySub::ATTENDANCE_OPEN_KEY: return TEXT("");
		case ItemCategorySub::SQ_STAMP: return TEXT("");
		case ItemCategorySub::BOSS_COLLECTION: return TEXT("");
		case ItemCategorySub::INVADE_CURRENCY: return TEXT("");
		case ItemCategorySub::EVENT_CURRENCY: return TEXT("");
		case ItemCategorySub::NORMAL_CURRENCY: return TEXT("");
		case ItemCategorySub::COIN_CURRENCY: return TEXT("");
		case ItemCategorySub::DISCOUNT_COUPON: return TEXT("");
		case ItemCategorySub::SEAL_STONE: return TEXT("");
		case ItemCategorySub::PARTYDUNGEON_KEY: return TEXT("");
		case ItemCategorySub::ETC: return TEXT("");
		case ItemCategorySub::DELIVERY_BOX_EQUIP: return TEXT("");
		case ItemCategorySub::DELIVERY_BOX_COSTUME: return TEXT("");
		case ItemCategorySub::DELIVERY_BOX_FAIRY: return TEXT("");
		case ItemCategorySub::DEATH_PENALTY_RECOVERY: return TEXT("");
		case ItemCategorySub::ELITE_DUNGEON_BALL: return TEXT("");
		case ItemCategorySub::NICKNAME_CHANGE: return TEXT("");
	}
	return FString();
}
inline FString ItemGradeEnumToDisplayKey(ItemGrade value)
{
	switch (value)
	{
		case ItemGrade::NONE: return TEXT("");
		case ItemGrade::NORMAL: return TEXT("");
		case ItemGrade::MAGIC: return TEXT("");
		case ItemGrade::RARE: return TEXT("");
		case ItemGrade::EPIC: return TEXT("");
		case ItemGrade::UNIQUE: return TEXT("");
		case ItemGrade::LEGEND: return TEXT("");
		case ItemGrade::ETERNAL: return TEXT("");
	}
	return FString();
}
inline FString ItemMaterialEnumToDisplayKey(ItemMaterial value)
{
	switch (value)
	{
		case ItemMaterial::FABRIC: return TEXT("");
		case ItemMaterial::WOOD: return TEXT("");
		case ItemMaterial::BONE: return TEXT("");
		case ItemMaterial::LEATHER: return TEXT("");
		case ItemMaterial::STEEL: return TEXT("");
		case ItemMaterial::ADAMATIUM: return TEXT("");
		case ItemMaterial::MITHRIL: return TEXT("");
		case ItemMaterial::JEWEL: return TEXT("");
		case ItemMaterial::ZENITE: return TEXT("");
		case ItemMaterial::TRUE_SILVER: return TEXT("");
		case ItemMaterial::ORICALCUM: return TEXT("");
		case ItemMaterial::SHELL: return TEXT("");
		case ItemMaterial::SPIRIT: return TEXT("");
		case ItemMaterial::STONE: return TEXT("");
		case ItemMaterial::BRONZE: return TEXT("");
		case ItemMaterial::THORIUM: return TEXT("");
	}
	return FString();
}
inline FString ItemEffectTypeEnumToDisplayKey(ItemEffectType value)
{
	switch (value)
	{
		case ItemEffectType::NONE: return TEXT("");
		case ItemEffectType::RESTORE_STAT: return TEXT("");
		case ItemEffectType::ADD_ABNORMALITY_STAT: return TEXT("");
		case ItemEffectType::WARP_TO_TOWN: return TEXT("");
		case ItemEffectType::WARP_TO_BOOKMARK: return TEXT("");
		case ItemEffectType::WARP_TO_RANDOM: return TEXT("");
		case ItemEffectType::GET_REWARD: return TEXT("");
		case ItemEffectType::QUEST_REPEAT: return TEXT("");
		case ItemEffectType::RESET_BONUS_STAT: return TEXT("");
		case ItemEffectType::BONUS_STAT_POINT: return TEXT("");
		case ItemEffectType::DECOMPOSE_EQUIP_KIT: return TEXT("");
		case ItemEffectType::DECOMPOSE_SKILLBOOK_KIT: return TEXT("");
		case ItemEffectType::SKILL_BOOK: return TEXT("");
		case ItemEffectType::ENCHANT_SKILL: return TEXT("");
		case ItemEffectType::SUMMON_FAIRY: return TEXT("");
		case ItemEffectType::ARCAS_POINT: return TEXT("");
		case ItemEffectType::OPTION_STONE: return TEXT("");
		case ItemEffectType::BLESSED_OPTION_STONE: return TEXT("");
		case ItemEffectType::PVP_ALIGNMENT_POINT: return TEXT("");
		case ItemEffectType::SUMMON_COSTUME: return TEXT("");
		case ItemEffectType::GUILD_GIFT: return TEXT("");
		case ItemEffectType::SPAWN_TREASURE_MONSTER: return TEXT("");
		case ItemEffectType::GAIN_MONSTER_KNOWLEDGE_EXP: return TEXT("");
		case ItemEffectType::UNLOCK_MONSTER_KNOWLEDGE_SLATE: return TEXT("");
		case ItemEffectType::RESET_MONSTER_KNOWLEDGE_SLATE_SLOT: return TEXT("");
		case ItemEffectType::SPAWN_CAMP: return TEXT("");
		case ItemEffectType::SPAWN_INVADE_PORTAL: return TEXT("");
		case ItemEffectType::ITEM_ENCHANT: return TEXT("");
		case ItemEffectType::WARP_TO_DUNGEON: return TEXT("");
		case ItemEffectType::DUNGEON_TIME_CHARGE: return TEXT("");
		case ItemEffectType::CHARACTER_SLOT_UNLOCK: return TEXT("");
		case ItemEffectType::DELIVERY_BOX: return TEXT("");
		case ItemEffectType::CHARACTER_CUSTOMIZING: return TEXT("");
		case ItemEffectType::SPAWN_MONSTER: return TEXT("");
		case ItemEffectType::DISCOUNT_COUPON: return TEXT("");
		case ItemEffectType::SOCIAL_EMOTION: return TEXT("");
		case ItemEffectType::DEATH_PENALTY_RECOVERY: return TEXT("");
		case ItemEffectType::CHANGE_COSTUME: return TEXT("");
		case ItemEffectType::GET_ABNORMALITY_STAT_AND_REWARD: return TEXT("");
		case ItemEffectType::TICKET_NICKNAME_CHANGE: return TEXT("");
		case ItemEffectType::MAGICAL_FORGE: return TEXT("");
		case ItemEffectType::EXE_SKILL_NOTIFY: return TEXT("");
		case ItemEffectType::WARP_TO_AGIT: return TEXT("");
	}
	return FString();
}
inline FString ChangeWeaponTypeFinishReasonEnumToDisplayKey(ChangeWeaponTypeFinishReason value)
{
	switch (value)
	{
		case ChangeWeaponTypeFinishReason::NORMAL: return TEXT("");
		case ChangeWeaponTypeFinishReason::DIE: return TEXT("");
		case ChangeWeaponTypeFinishReason::WARP: return TEXT("");
		case ChangeWeaponTypeFinishReason::ABNORMALITY: return TEXT("");
		case ChangeWeaponTypeFinishReason::INVALID_DATA: return TEXT("");
	}
	return FString();
}
inline FString ItemEnchantResultEnumToDisplayKey(ItemEnchantResult value)
{
	switch (value)
	{
		case ItemEnchantResult::NONE: return TEXT("");
		case ItemEnchantResult::ENCHANT_SUCCEEDED: return TEXT("");
		case ItemEnchantResult::ENCHANT_SUCCEEDED_CURSED: return TEXT("");
		case ItemEnchantResult::ENCHANT_SUCCEEDED_BLESSED: return TEXT("");
		case ItemEnchantResult::ENCHANT_FAILED: return TEXT("");
		case ItemEnchantResult::ENCHANT_FAILED_SHIELDED: return TEXT("");
		case ItemEnchantResult::ENCHANT_FAILED_DESTROYED: return TEXT("");
		case ItemEnchantResult::ENCHANT_PRESERVED: return TEXT("");
		case ItemEnchantResult::ENCHANT_FAILED_DOWN: return TEXT("");
	}
	return FString();
}
inline FString ItemAcquireTypeEnumToDisplayKey(ItemAcquireType value)
{
	switch (value)
	{
		case ItemAcquireType::IN_GAME: return TEXT("");
		case ItemAcquireType::PAID: return TEXT("");
	}
	return FString();
}
inline FString ItemRefineResultEnumToDisplayKey(ItemRefineResult value)
{
	switch (value)
	{
		case ItemRefineResult::NONE: return TEXT("");
		case ItemRefineResult::REFINE_SUCCEEDED: return TEXT("");
		case ItemRefineResult::REFINE_SUCCEEDED_BONUS: return TEXT("");
	}
	return FString();
}
inline FString ItemMagicalForgeResultEnumToDisplayKey(ItemMagicalForgeResult value)
{
	switch (value)
	{
		case ItemMagicalForgeResult::NONE: return TEXT("");
		case ItemMagicalForgeResult::MAGICAL_FORGE_SUCCEEDED: return TEXT("");
	}
	return FString();
}
inline FString ItemCollectionTypeEnumToDisplayKey(ItemCollectionType value)
{
	switch (value)
	{
		case ItemCollectionType::NONE: return TEXT("");
		case ItemCollectionType::PERMANENT: return TEXT("");
		case ItemCollectionType::FIXED_TERM: return TEXT("");
	}
	return FString();
}
inline FString ItemCollectionUnlockTypeEnumToDisplayKey(ItemCollectionUnlockType value)
{
	switch (value)
	{
		case ItemCollectionUnlockType::NONE: return TEXT("");
		case ItemCollectionUnlockType::LEVEL: return TEXT("");
		case ItemCollectionUnlockType::QUEST: return TEXT("");
	}
	return FString();
}
inline FString ItemCollectionEventTypeEnumToDisplayKey(ItemCollectionEventType value)
{
	switch (value)
	{
		case ItemCollectionEventType::NONE: return TEXT("");
		case ItemCollectionEventType::NORMAL: return TEXT("");
		case ItemCollectionEventType::EVENT: return TEXT("");
	}
	return FString();
}
inline FString CraftResultEnumToDisplayKey(CraftResult value)
{
	switch (value)
	{
		case CraftResult::NONE: return TEXT("");
		case CraftResult::CRAFT_SUCCEEDED: return TEXT("");
		case CraftResult::CRAFT_SUCCEEDED_GREAT: return TEXT("");
		case CraftResult::CRAFT_FAIL: return TEXT("");
	}
	return FString();
}
inline FString AutoPurchaseTypeEnumToDisplayKey(AutoPurchaseType value)
{
	switch (value)
	{
		case AutoPurchaseType::AUTO_SHOP_BUY: return TEXT("");
		case AutoPurchaseType::AUTO_SHOP_SELL_CONSUME: return TEXT("");
		case AutoPurchaseType::AUTO_SHOP_SELL_EQUIP: return TEXT("");
		case AutoPurchaseType::AUTO_DEPOT_KEEP: return TEXT("");
		case AutoPurchaseType::AUTO_DEPOT_KEEP_GROUP: return TEXT("");
	}
	return FString();
}
inline FString GuildGiftGiveTypeEnumToDisplayKey(GuildGiftGiveType value)
{
	switch (value)
	{
		case GuildGiftGiveType::NONE: return TEXT("");
		case GuildGiftGiveType::GIVE_TO_BUYER: return TEXT("");
		case GuildGiftGiveType::GIVE_TO_GUILD_MEMBERS: return TEXT("");
	}
	return FString();
}
inline FString TickerRangeEnumToDisplayKey(TickerRange value)
{
	switch (value)
	{
		case TickerRange::NONE: return TEXT("");
		case TickerRange::MAP: return TEXT("");
		case TickerRange::ALL: return TEXT("");
		case TickerRange::SELF: return TEXT("");
	}
	return FString();
}
inline FString ItemCraftUnlockTypeEnumToDisplayKey(ItemCraftUnlockType value)
{
	switch (value)
	{
		case ItemCraftUnlockType::NONE: return TEXT("");
		case ItemCraftUnlockType::LEVEL: return TEXT("");
		case ItemCraftUnlockType::QUEST: return TEXT("");
	}
	return FString();
}
inline FString ItemCraftRestrictEnumToDisplayKey(ItemCraftRestrict value)
{
	switch (value)
	{
		case ItemCraftRestrict::NONE: return TEXT("");
		case ItemCraftRestrict::CHARACTER: return TEXT("");
		case ItemCraftRestrict::ACCOUNT: return TEXT("");
		case ItemCraftRestrict::SERVER: return TEXT("");
	}
	return FString();
}
inline FString ItemEnchantTypeEnumToDisplayKey(ItemEnchantType value)
{
	switch (value)
	{
		case ItemEnchantType::NORMAL_ENCHANT: return TEXT("");
		case ItemEnchantType::BLESSED_ENCHANT: return TEXT("");
		case ItemEnchantType::CURSED_ENCHANT: return TEXT("");
	}
	return FString();
}
inline FString DropItemEffectEnumToDisplayKey(DropItemEffect value)
{
	switch (value)
	{
		case DropItemEffect::NORAM_TYPE: return TEXT("");
		case DropItemEffect::RANDOM_HIT_TYPE: return TEXT("");
		case DropItemEffect::LAST_HIT_TYPE: return TEXT("");
	}
	return FString();
}
inline FString ItemCouponDiscountTypeEnumToDisplayKey(ItemCouponDiscountType value)
{
	switch (value)
	{
		case ItemCouponDiscountType::CONSTANT: return TEXT("");
		case ItemCouponDiscountType::COEFFICIENT: return TEXT("");
	}
	return FString();
}
inline FString SealSlotColorEnumToDisplayKey(SealSlotColor value)
{
	switch (value)
	{
		case SealSlotColor::NONE: return TEXT("");
		case SealSlotColor::RARE: return TEXT("");
		case SealSlotColor::EPIC: return TEXT("");
		case SealSlotColor::UNIQUE: return TEXT("");
	}
	return FString();
}
inline FString ItemUpdateReasonEnumToDisplayKey(ItemUpdateReason value)
{
	switch (value)
	{
		case ItemUpdateReason::CREATE_ITEM: return TEXT("");
		case ItemUpdateReason::CREATE_ITEM_ADMIN_COMMAND: return TEXT("");
		case ItemUpdateReason::ITEM_DELETE: return TEXT("");
		case ItemUpdateReason::ITEM_DELETE_THROW_AWAY: return TEXT("");
		case ItemUpdateReason::ITEM_DELETE_ADMIN_COMMAND: return TEXT("");
		case ItemUpdateReason::ITEM_DELETE_WARP_BOOK_MARK: return TEXT("");
		case ItemUpdateReason::DEATH_PANALTY_RESTORE: return TEXT("");
		case ItemUpdateReason::ACHIEVEMENT_REWARD: return TEXT("");
		case ItemUpdateReason::ACHIEVEMENT_REWARDALL: return TEXT("");
		case ItemUpdateReason::ACHIEVEMENT_REWARDTHEME: return TEXT("");
		case ItemUpdateReason::ARCAS_UPDATEPOINT: return TEXT("");
		case ItemUpdateReason::PVPALIGNMENT_UPDATEPOINT: return TEXT("");
		case ItemUpdateReason::ATTENDANCE_PURCHASE: return TEXT("");
		case ItemUpdateReason::BATTLEPASS_ACTIVATE: return TEXT("");
		case ItemUpdateReason::BMSHOP_PURCHASE: return TEXT("");
		case ItemUpdateReason::BMSHOP_RECIVE: return TEXT("");
		case ItemUpdateReason::COSTUME_SUMMON: return TEXT("");
		case ItemUpdateReason::COSTUME_COMPOSE: return TEXT("");
		case ItemUpdateReason::COSTUME_CEILING: return TEXT("");
		case ItemUpdateReason::COSTUME_CHANGE: return TEXT("");
		case ItemUpdateReason::USER_DEATH: return TEXT("");
		case ItemUpdateReason::FAIRY_ENCHANT: return TEXT("");
		case ItemUpdateReason::FAIRY_SUMMON: return TEXT("");
		case ItemUpdateReason::FAIRY_COMPOSE: return TEXT("");
		case ItemUpdateReason::FAIRY_CEILING: return TEXT("");
		case ItemUpdateReason::GUILD_BUYSHOP: return TEXT("");
		case ItemUpdateReason::GUILD_GUILDGIFT: return TEXT("");
		case ItemUpdateReason::GUILD_DAILY_ACTIVITY: return TEXT("");
		case ItemUpdateReason::GUILD_ITEMCOLLECTION: return TEXT("");
		case ItemUpdateReason::ITEM_BONUSSTATRESET: return TEXT("");
		case ItemUpdateReason::ITEM_BONUSSTATPOINT: return TEXT("");
		case ItemUpdateReason::ITEM_BUY: return TEXT("");
		case ItemUpdateReason::ITEM_COLLECTION: return TEXT("");
		case ItemUpdateReason::ITEM_CRAFT: return TEXT("");
		case ItemUpdateReason::ITEM_DELETE_EXPIRED: return TEXT("");
		case ItemUpdateReason::ITEM_DELIVERY_BOX_COSTUME: return TEXT("");
		case ItemUpdateReason::ITEM_DELIVERY_BOX_EQUIP: return TEXT("");
		case ItemUpdateReason::ITEM_DELIVERY_BOX_FAIRY: return TEXT("");
		case ItemUpdateReason::ITEM_ENCHANT: return TEXT("");
		case ItemUpdateReason::ENCHANT_FAILED_DESTORYED: return TEXT("");
		case ItemUpdateReason::ITEM_GETREWARD: return TEXT("");
		case ItemUpdateReason::ITEM_DECOMPOSE: return TEXT("");
		case ItemUpdateReason::ITEM_MOVE_DEPOT_TO_INVENTORY: return TEXT("");
		case ItemUpdateReason::ITEM_MOVE_INVEN_TO_DEPOT: return TEXT("");
		case ItemUpdateReason::ITEM_QUEST_REPEAT: return TEXT("");
		case ItemUpdateReason::ITEM_REFINE: return TEXT("");
		case ItemUpdateReason::ITEM_SELECT_REWARD: return TEXT("");
		case ItemUpdateReason::ITEM_SELL: return TEXT("");
		case ItemUpdateReason::GUILD_STORE: return TEXT("");
		case ItemUpdateReason::ITEM_SPAWN_CAMP: return TEXT("");
		case ItemUpdateReason::ITEM_SPAWN_TREASURE_MONSTER: return TEXT("");
		case ItemUpdateReason::ITEM_USE: return TEXT("");
		case ItemUpdateReason::ITEM_EQUIP_CHANGED: return TEXT("");
		case ItemUpdateReason::MAIL_RECV: return TEXT("");
		case ItemUpdateReason::MARKET_TRANSACTIONCANCEL: return TEXT("");
		case ItemUpdateReason::MARKET_TRANSACTIONREGISTER: return TEXT("");
		case ItemUpdateReason::MARKET_TRANSACTIONPURCHASE: return TEXT("");
		case ItemUpdateReason::MONSTER_KNOWLEDGE_EXP_GAIN: return TEXT("");
		case ItemUpdateReason::MONSTER_KNOWLEDGE_EXP_GAIN_ALL: return TEXT("");
		case ItemUpdateReason::MONSTER_KNOWLEDGE_SLATE_RESET: return TEXT("");
		case ItemUpdateReason::MONSTER_KNOWLEDGE_SLATE_SLOT: return TEXT("");
		case ItemUpdateReason::MULTILEVELRANK_CHANGE: return TEXT("");
		case ItemUpdateReason::MULTILEVELRANK_PROTOMTION: return TEXT("");
		case ItemUpdateReason::MULTILEVELRANK_REWARD: return TEXT("");
		case ItemUpdateReason::NPC_EVENT_SHOP_PURCHASE: return TEXT("");
		case ItemUpdateReason::QUEST_REWARD: return TEXT("");
		case ItemUpdateReason::SKILL_ENCHANT: return TEXT("");
		case ItemUpdateReason::SKILL_LEARN: return TEXT("");
		case ItemUpdateReason::CHARACTER_SLOT_UNLOCK: return TEXT("");
		case ItemUpdateReason::USER_INVADE_PENALTY: return TEXT("");
		case ItemUpdateReason::PVP_ALIGNMENT_UPDATE_POINT: return TEXT("");
		case ItemUpdateReason::PARTY_DUNGEON_CLEAR: return TEXT("");
		case ItemUpdateReason::BOSS_KILL: return TEXT("");
		case ItemUpdateReason::DROP_GET: return TEXT("");
		case ItemUpdateReason::MAP_EVENT_API: return TEXT("");
		case ItemUpdateReason::NPC_REWARD: return TEXT("");
		case ItemUpdateReason::CHARACTER_CUSTOMIZING: return TEXT("");
		case ItemUpdateReason::INVADE_PORTAL_ACTIVATE: return TEXT("");
		case ItemUpdateReason::MAKE_CHARACTER: return TEXT("");
		case ItemUpdateReason::HVE_ITEM_GVIE_RETRIVE: return TEXT("");
		case ItemUpdateReason::ITEM_SPAWN_MONSTER: return TEXT("");
		case ItemUpdateReason::WORLD_BOSS_CLEAR_REWARD: return TEXT("");
		case ItemUpdateReason::WORLD_BOSS_BASIC_REWARD: return TEXT("");
		case ItemUpdateReason::WORLD_BOSS_CONSOLATION_REWARD: return TEXT("");
		case ItemUpdateReason::SEAL_SLOT_CHANGE_ALL: return TEXT("");
		case ItemUpdateReason::SEAL_LEVEL_UP: return TEXT("");
		case ItemUpdateReason::MONSTER_KNOWLEDGE_EXP_SURPLUS_REWARD: return TEXT("");
		case ItemUpdateReason::INVADE_TREASUREBOX_REWARD: return TEXT("");
		case ItemUpdateReason::PERSONAL_TRADING: return TEXT("");
		case ItemUpdateReason::INVADE_PENALTY: return TEXT("");
		case ItemUpdateReason::BATTLEPASS_REWARD: return TEXT("");
		case ItemUpdateReason::ITEM_DEATH_PENALTY_RECOVERY: return TEXT("");
		case ItemUpdateReason::BATTLEPASS_REWARD_ALL: return TEXT("");
		case ItemUpdateReason::ATTENDANCE_UPDATE: return TEXT("");
		case ItemUpdateReason::RESTORE_ITEM_ADMIN_COMMAND: return TEXT("");
		case ItemUpdateReason::SUMMON_FAIRY_MILEAGE: return TEXT("");
		case ItemUpdateReason::SUMMON_COSTUME_MILEAGE: return TEXT("");
		case ItemUpdateReason::SPIRIT_SHOT_ENCHANT: return TEXT("");
		case ItemUpdateReason::COMMUNITY_EVENT_MISSION: return TEXT("");
		case ItemUpdateReason::DURAL: return TEXT("");
		case ItemUpdateReason::ITEM_FUSE: return TEXT("");
		case ItemUpdateReason::ITEM_NICKNAME_CHANGE: return TEXT("");
		case ItemUpdateReason::SLATE_SPECIAL_NODE_ACTIVATE: return TEXT("");
		case ItemUpdateReason::DROP_GET_INVADE_BOSS_KILL: return TEXT("");
		case ItemUpdateReason::DROP_GET_INVADE_WAGON_KILL: return TEXT("");
		case ItemUpdateReason::DROP_GET_INVADE_GUARDIAN_KILL: return TEXT("");
		case ItemUpdateReason::DROP_GET_INVADE_PK_DIE_PENALTY: return TEXT("");
		case ItemUpdateReason::DROP_GET_INVADE_HOME_WARP_PENALTY: return TEXT("");
		case ItemUpdateReason::ITEM_MAGICAL_FORGE: return TEXT("");
		case ItemUpdateReason::AGIT_RENTAL: return TEXT("");
		case ItemUpdateReason::AGIT_ENTER: return TEXT("");
		case ItemUpdateReason::AGIT_LEAVE: return TEXT("");
		case ItemUpdateReason::ITEM_REWARD_GUILD_WAR: return TEXT("");
		case ItemUpdateReason::SUMMON_UNIQUE_FAIRY_MILEAGE: return TEXT("");
		case ItemUpdateReason::SUMMON_UNIQUE_COSTUME_MILEAGE: return TEXT("");
		case ItemUpdateReason::AUTO_DECOMPOSE: return TEXT("");
	}
	return FString();
}
inline FString ItemGainTypeEnumToDisplayKey(ItemGainType value)
{
	switch (value)
	{
		case ItemGainType::NONE: return TEXT("");
		case ItemGainType::CONSUME_NORMAL: return TEXT("");
		case ItemGainType::CONSUME_SPECIAL: return TEXT("");
		case ItemGainType::CONSTUME_SCROLL_NORMAL: return TEXT("");
		case ItemGainType::CONSTUME_SCROLL_SPECIAL: return TEXT("");
		case ItemGainType::CONSTUME_SCROLL_ENCHANT: return TEXT("");
		case ItemGainType::OTHER_NORMAL: return TEXT("");
		case ItemGainType::OTHER_SPECIAL: return TEXT("");
		case ItemGainType::OTHER_ETC: return TEXT("");
	}
	return FString();
}
inline FString ItemDecomposeTypeEnumToDisplayKey(ItemDecomposeType value)
{
	switch (value)
	{
		case ItemDecomposeType::NONE: return TEXT("");
		case ItemDecomposeType::ITEM_ID: return TEXT("");
		case ItemDecomposeType::ITEM_CATEGORY_MAIN: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidItemType(ItemType value) noexcept
{
	return (value >= ItemType::NONE && value < ItemType::MAX);
}

inline bool IsValidItemCategoryMain(ItemCategoryMain value) noexcept
{
	return (value >= ItemCategoryMain::NONE && value < ItemCategoryMain::MAX);
}

inline bool IsValidItemCategorySub(ItemCategorySub value) noexcept
{
	return (value >= ItemCategorySub::NONE && value < ItemCategorySub::MAX);
}

inline bool IsValidItemGrade(ItemGrade value) noexcept
{
	return (value >= ItemGrade::NONE && value < ItemGrade::MAX);
}

inline bool IsValidItemMaterial(ItemMaterial value) noexcept
{
	return (value >= ItemMaterial::FABRIC && value < ItemMaterial::MAX);
}

inline bool IsValidItemEffectType(ItemEffectType value) noexcept
{
	return (value >= ItemEffectType::NONE && value < ItemEffectType::MAX);
}

inline bool IsValidChangeWeaponTypeFinishReason(ChangeWeaponTypeFinishReason value) noexcept
{
	return (value >= ChangeWeaponTypeFinishReason::NORMAL && value < ChangeWeaponTypeFinishReason::MAX);
}

inline bool IsValidItemEnchantResult(ItemEnchantResult value) noexcept
{
	return (value >= ItemEnchantResult::NONE && value < ItemEnchantResult::MAX);
}

inline bool IsValidItemAcquireType(ItemAcquireType value) noexcept
{
	return (value >= ItemAcquireType::IN_GAME && value < ItemAcquireType::MAX);
}

inline bool IsValidItemRefineResult(ItemRefineResult value) noexcept
{
	return (value >= ItemRefineResult::NONE && value < ItemRefineResult::MAX);
}

inline bool IsValidItemMagicalForgeResult(ItemMagicalForgeResult value) noexcept
{
	return (value >= ItemMagicalForgeResult::NONE && value < ItemMagicalForgeResult::MAX);
}

inline bool IsValidItemCollectionType(ItemCollectionType value) noexcept
{
	return (value >= ItemCollectionType::NONE && value < ItemCollectionType::MAX);
}

inline bool IsValidItemCollectionUnlockType(ItemCollectionUnlockType value) noexcept
{
	return (value >= ItemCollectionUnlockType::NONE && value < ItemCollectionUnlockType::MAX);
}

inline bool IsValidItemCollectionEventType(ItemCollectionEventType value) noexcept
{
	return (value >= ItemCollectionEventType::NONE && value < ItemCollectionEventType::MAX);
}

inline bool IsValidCraftResult(CraftResult value) noexcept
{
	return (value >= CraftResult::NONE && value < CraftResult::MAX);
}

inline bool IsValidAutoPurchaseType(AutoPurchaseType value) noexcept
{
	return (value >= AutoPurchaseType::AUTO_SHOP_BUY && value < AutoPurchaseType::MAX);
}

inline bool IsValidGuildGiftGiveType(GuildGiftGiveType value) noexcept
{
	return (value >= GuildGiftGiveType::NONE && value < GuildGiftGiveType::MAX);
}

inline bool IsValidTickerRange(TickerRange value) noexcept
{
	return (value >= TickerRange::NONE && value < TickerRange::MAX);
}

inline bool IsValidItemCraftUnlockType(ItemCraftUnlockType value) noexcept
{
	return (value >= ItemCraftUnlockType::NONE && value < ItemCraftUnlockType::MAX);
}

inline bool IsValidItemCraftRestrict(ItemCraftRestrict value) noexcept
{
	return (value >= ItemCraftRestrict::NONE && value < ItemCraftRestrict::MAX);
}

inline bool IsValidItemEnchantType(ItemEnchantType value) noexcept
{
	return (value >= ItemEnchantType::NORMAL_ENCHANT && value < ItemEnchantType::MAX);
}

inline bool IsValidDropItemEffect(DropItemEffect value) noexcept
{
	return (value >= DropItemEffect::NORAM_TYPE && value < DropItemEffect::MAX);
}

inline bool IsValidItemCouponDiscountType(ItemCouponDiscountType value) noexcept
{
	return (value >= ItemCouponDiscountType::CONSTANT && value < ItemCouponDiscountType::MAX);
}

inline bool IsValidSealSlotColor(SealSlotColor value) noexcept
{
	return (value >= SealSlotColor::NONE && value < SealSlotColor::MAX);
}

inline bool IsValidItemUpdateReason(ItemUpdateReason value) noexcept
{
	return (value >= ItemUpdateReason::CREATE_ITEM && value < ItemUpdateReason::ITEM_UPDATE_REASON_MAX);
}

inline bool IsValidItemGainType(ItemGainType value) noexcept
{
	return (value >= ItemGainType::NONE && value < ItemGainType::MAX);
}

inline bool IsValidItemDecomposeType(ItemDecomposeType value) noexcept
{
	return (value >= ItemDecomposeType::NONE && value < ItemDecomposeType::MAX);
}


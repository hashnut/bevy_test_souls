#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedCreatureEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// Creature의 성별 타입
enum class CreatureGenderType : uint8
{

	// 모든 성별
	ALL = 0,

	// 남성
	MALE = 1,

	// 여성
	FEMALE = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// Creature의 종족 타입
enum class CreatureRaceType : uint8
{

	// 종족 없음
	NONE = 0,

	// 인간
	HUMAN = 1,

	// 생물형
	NORMAL_CREATURE = 2,

	// 언데드
	UNDEAD = 3,

	// 정령
	ELEMENTAL = 4,

	// 마족
	DEMON = 5,

	// 아인족
	DEMIHUMAN = 6,

	// 마법 생물
	MAGICAL_CREATURE = 7,

	// 시공의 틈새 구슬
	MARBLE = 8,

	// 식물형
	PLANT = 9,

	MAX = 10,
};

UENUM(BlueprintType)
// Creature의 무기 타입
enum class CreatureWeaponType : uint8
{

	// 타입 없음
	NONE = 0,

	// 대검(임시 이름)
	SWORD = 1,

	// 단검(임시 이름)
	DAGGER = 2,

	// 활(임시 이름)
	BOW = 3,

	// 지팡이
	WAND = 4,

	// 오브
	ORB = 5,

	// 샷건
	SHOTGUN = 6,

	// 맨손
	HAND = 7,

	// 모든 무기 타입
	ALL = 8,

	MAX = 9,
};

UENUM(BlueprintType)
// Creature의 랭크 타입
enum class CreatureRankType : uint8
{

	// 타입 없음
	NONE = 0,

	// 일반 (User 포함)
	NORMAL = 1,

	// 정예
	ELITE = 2,

	// 네임드
	NAMED = 3,

	// 보스
	BOSS = 4,

	// 레이드 (보스 중에서도 특별 보스급)
	RAID = 5,

	MAX = 6,
};

UENUM()
// 행동제약을 걸어야 하는 시점(이벤트,상황)
/*
	 class BehaviorResolver 
	 enum CreatureActionType 에 각각 셋팅 필요
*/
enum class CommonBehaviorResolverType : uint8
{

	// Default
	NONE = 0,

	// enum AbnormalityEffectSubType

	// AbnormalityEffectSubType::OVERTIME_BLEED
	ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BLEED = 1,

	// AbnormalityEffectSubType::OVERTIME_POISON
	ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_POISON = 2,

	// AbnormalityEffectSubType::OVERTIME_BURN
	ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BURN = 3,

	// AbnormalityEffectSubType::OVERTIME_HOT
	ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_HOT = 4,

	// AbnormalityEffectSubType::OVERTIME_SENSITIVE
	ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_SENSITIVE = 5,

	// AbnormalityEffectSubType::CC_STUN
	ABNORMALITY_EFFECT_SUB_TYPE_CC_STUN = 6,

	// AbnormalityEffectSubType::CC_SILENCE
	ABNORMALITY_EFFECT_SUB_TYPE_CC_SILENCE = 7,

	// AbnormalityEffectSubType::CC_BIND
	ABNORMALITY_EFFECT_SUB_TYPE_CC_BIND = 8,

	// AbnormalityEffectSubType::CC_CANNOT_WARP
	ABNORMALITY_EFFECT_SUB_TYPE_CC_CANNOT_WARP = 9,

	// AbnormalityEffectSubType::CC_BURY
	ABNORMALITY_EFFECT_SUB_TYPE_CC_BURY = 10,

	// AbnormalityEffectSubType::CC_KNOCK_DOWN
	ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_DOWN = 11,

	// AbnormalityEffectSubType::CC_KNOCK_BACK
	ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_BACK = 12,

	// AbnormalityEffectSubType::CC_FEAR
	ABNORMALITY_EFFECT_SUB_TYPE_CC_FEAR = 13,

	// AbnormalityEffectSubType::CC_TAUNT
	ABNORMALITY_EFFECT_SUB_TYPE_CC_TAUNT = 14,

	// AbnormalityEffectSubType::CC_MOVE_LOCATION
	ABNORMALITY_EFFECT_SUB_TYPE_CC_MOVE_LOCATION = 15,

	// AbnormalityEffectSubType::CC_POLYMORP
	ABNORMALITY_EFFECT_SUB_TYPE_CC_POLYMORP = 16,

	// AbnormalityEffectSubType::CC_COCOON
	ABNORMALITY_EFFECT_SUB_TYPE_CC_COCOON = 17,

	// AbnormalityEffectSubType::CC_FREEZE
	ABNORMALITY_EFFECT_SUB_TYPE_CC_FREEZE = 18,

	// AbnormalityEffectSubType::CC_ROOT_WINDING
	ABNORMALITY_EFFECT_SUB_TYPE_CC_ROOT_WINDING = 19,

	// AbnormalityEffectSubType::STAT
	ABNORMALITY_EFFECT_SUB_TYPE_STAT = 20,

	// AbnormalityEffectSubType::STAT_SLOW
	ABNORMALITY_EFFECT_SUB_TYPE_STAT_SLOW = 21,

	// AbnormalityEffectSubType::STAT_DASH
	ABNORMALITY_EFFECT_SUB_TYPE_STAT_DASH = 22,

	// AbnormalityEffectSubType::STAT_IMMUNE
	ABNORMALITY_EFFECT_SUB_TYPE_STAT_IMMUNE = 23,

	// AbnormalityEffectSubType::STAT_VENT
	ABNORMALITY_EFFECT_SUB_TYPE_STAT_VENT = 24,

	// AbnormalityEffectSubType::UTIL_ARCAS_FAIRY_BUFF
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ARCAS_FAIRY_BUFF = 25,

	// AbnormalityEffectSubType::UTIL_ON_REMOVE_EXE_SKILL_NOTIFY
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ON_REMOVE_EXE_SKILL_NOTIFY = 26,

	// AbnormalityEffectSubType::UTIL_NOT_AI_CHANGE_PHASE
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_NOT_AI_CHANGE_PHASE = 27,

	// AbnormalityEffectSubType::UTIL_IMMORTAL
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_IMMORTAL = 28,

	// AbnormalityEffectSubType::UTIL_HIDING
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_HIDING = 29,

	// AbnormalityEffectSubType::UTIL_BERSERKER_RAGE
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BERSERKER_RAGE = 30,

	// AbnormalityEffectSubType::UTIL_POINTED_ARROW
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_POINTED_ARROW = 31,

	// AbnormalityEffectSubType::UTIL_WEDGE_ARROW
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_WEDGE_ARROW = 32,

	// AbnormalityEffectSubType::UTIL_JAGGED_ARROW
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_JAGGED_ARROW = 33,

	// AbnormalityEffectSubType::UTIL_FINAL_HOWL
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_FINAL_HOWL = 34,

	// AbnormalityEffectSubType::UTIL_BONUS_DAMAGE
	ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BONUS_DAMAGE = 35,

	// enum MotionControllerType

	// MotionControllerType::USER
	MOTION_CONTROLLER_TYPE_USER = 36,

	// MotionControllerType::JUMP
	MOTION_CONTROLLER_TYPE_JUMP = 37,

	// MotionControllerType::FALL
	MOTION_CONTROLLER_TYPE_FALL = 38,

	// MotionControllerType::SKILL
	MOTION_CONTROLLER_TYPE_SKILL = 39,

	// MotionControllerType::AI
	MOTION_CONTROLLER_TYPE_AI = 40,

	// MotionControllerType::REACTION
	MOTION_CONTROLLER_TYPE_REACTION = 41,

	// MotionControllerType::FEAR
	MOTION_CONTROLLER_TYPE_FEAR = 42,

	// MotionControllerType::CHARM
	MOTION_CONTROLLER_TYPE_CHARM = 43,

	// enum UserBehaviorStatusType

	// UserBehaviorStatusType::CHANGE_WEAPON_TYPE
	USER_BEHAVIOR_STATUS_TYPE_CHANGE_WEAPON_TYPE = 44,

	// UserBehaviorStatusType::FIRST_WEIGHT_PENALTY
	USER_BEHAVIOR_STATUS_TYPE_FIRST_WEIGHT_PENALTY = 45,

	// UserBehaviorStatusType::SECOND_WEIGHT_PENALTY
	USER_BEHAVIOR_STATUS_TYPE_SECOND_WEIGHT_PENALTY = 46,

	// UserBehaviorStatusType::IN_SAFE_ZONE
	USER_BEHAVIOR_STATUS_TYPE_IN_SAFE_ZONE = 47,

	// UserBehaviorStatusType::SPAWN_ACTION
	USER_BEHAVIOR_STATUS_TYPE_IN_SPAWN_ACTION = 48,

	// UserBehaviorStatusType::PERSONAL_TRADING_ACTION
	USER_BEHAVIOR_STATUS_TYPE_IN_PERSONAL_TRADING = 49,

	MAX = 50,
};

UENUM()
// Creature가 할 수 있는 행동의 종류
enum class CreatureActionType : uint8
{

	// USER - Client로부터 이동 요청, NPC에 의한 이동 - AI
	CREATURE_ACTION_TYPE_MOVE_DEFAULT = 0,

	// 리소스 기반 이동(Root Motion) - AbnormalityEffectKnockBack
	CREATURE_ACTION_TYPE_MOVE_ROOT_MOTION = 1,

	// 점(좌표) 이동(MOVE_LOCATION) : SkillNotifyMoveLocation
	CREATURE_ACTION_TYPE_MOVE_LOCATON = 2,

	// 서버 주관으로 이동 : 공포(AbnormalityEffectFear)
	CREATURE_ACTION_TYPE_MOVE_FEAR = 3,

	// 평타 사용
	CREATURE_ACTION_TYPE_SKILL_BASIC = 4,

	// 스킬 사용
	CREATURE_ACTION_TYPE_SKILL = 5,

	// 대시 스킬 사용
	CREATURE_ACTION_TYPE_DASH_SKILL = 6,

	// 버프 스킬 사용
	CREATURE_ACTION_TYPE_BUFF_SKILL = 7,

	// 아이템 사용
	CREATURE_ACTION_TYPE_ITEM = 8,

	// 소셜 액션
	CREATURE_ACTION_TYPE_SOCIAL = 9,

	// 워프
	CREATURE_ACTION_TYPE_WARP = 10,

	// 피격
	CREATURE_ACTION_TYPE_TAKE_DAMAGE = 11,

	// 페이즈 변경
	CREATURE_ACTION_TYPE_CHANGE_PAHSE = 12,

	// 자연 회복
	CREATURE_ACTION_TYPE_RECOVERY = 13,

	// 타겟 변경
	CREATURE_ACTION_TYPE_CHANGE_TARGET = 14,

	// 탈 것 이용
	CREATURE_ACTION_TYPE_MOUNT_VEHICLE = 15,

	MAX = 16,
};

UENUM(BlueprintType)
// 유저 행동 상태 타입 정의. (행동 제약과 관련됨)
enum class UserBehaviorStatusType : uint8
{

	// 없음
	NONE = 0,

	// 무기 교체로 인한 클래스 변경 상태.
	CHANGE_WEAPON_TYPE = 1,

	// 1단계 무게 패널티(자연 회복 불가)
	FIRST_WEIGHT_PENALTY = 2,

	// 2단계 무게 패널티(평타,스킬 사용 불가)
	SECOND_WEIGHT_PENALTY = 3,

	// 안전 지대에 있는 상태
	IN_SAFE_ZONE = 4,

	// 결투장에서 대기중인 상태
	ARENA_WAITING = 5,

	// 스폰 연출중인 상태
	SPAWN_ACTION = 6,

	// 개인 거래중인 상태
	PERSONAL_TRADING = 7,

	MAX = 8,
};

UENUM(BlueprintType)
// Creature의 리액션 타입, 클라이언트에 먼저 잡혀있던 구조, 클라이언트의 Enum 명을 사용합니다
enum class EReactionType : uint8
{

	// 없음
	NONE = 0,

	// 스턴
	STUN = 1,

	// 히트
	HIT = 2,

	// 넉다운
	KNOCKDOWN = 3,

	// 넉백
	KNOCKBACK = 4,

	// 강제 위치 이동(서버에서는 Teleport, 클라이언트에서는 이동 연출)
	MOVELOCATION = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// NpcFunctionType
enum class NpcFunctionType : uint8
{

	// 기능 없음 (임시)
	NONE = 0,

	// 잡화상점
	SHOP = 1,

	// 회복
	REVIVAL = 2,

	// 아이템 매입 (임시)
	PURCHASE = 3,

	// 퀘스트 대화
	QUEST_DIALOG = 4,

	// 퀘스트용 움직이지 않는 NPC
	SANDBAG = 5,

	// 창고
	DEPOT = 6,

	// 커뮤니티
	COMMUNITY = 7,

	// 이벤트 상점
	EVENT_SHOP = 8,

	// 장비 상점
	SHOP_EQUIP = 9,

	// 스킬북 상점
	SHOP_SKILL_BOOK = 10,

	// 필드보스
	FIELD_BOSS = 11,

	// 보물 몬스터
	TREASURE_MONSTER = 12,

	// 캠프 관련 수호탑
	CAMP = 13,

	// 파티던전보스
	PARTY_DUNGEON_BOSS = 14,

	// 고레벨 층이동 포털 보유 몬스터
	SPACE_TRACER = 15,

	// 문지기
	GATEKEEPER = 16,

	// 문
	DOOR = 17,

	// 퀘스트 게시판
	QUEST_BOARD = 18,

	// 미션에서 사용되는 웨이브 몬스터
	MISSION_WAVE = 19,

	// 이벤트 기도사
	EVENT_PRAYER = 20,

	// 소환 몬스터
	SPAWN_MONSTER = 21,

	// 월드보스
	WORLD_BOSS = 22,

	// 침공 수호자
	INVADE_GUARDIAN = 23,

	// 침공 마차
	INVADE_WAGON = 24,

	// 침공 보스
	INVADE_BOSS = 25,

	// 마력 오브젝트 - 파티 던전
	MAGIC_OBJECT = 26,

	// 시공의 틈새 몬스터
	SPACE_CRACK_MONSTER = 27,

	// 경비병
	GUARD = 28,

	// 마차호위
	INVADE_WAGON_GUARD = 29,

	// 침공 도적단
	INVADE_BANDITS = 30,

	// 보급품
	CART = 31,

	// 시공의틈새
	SPACE_CRACK_SUPPLYMONSTER = 32,

	// 시공의틈새
	SPACE_CRACK_ELITE = 33,

	// 시공의틈새
	SPACE_CRACK_CONTAMINANT = 34,

	// 시공의틈새
	SPACE_CRACK_BOSS = 35,

	// 시공의틈새
	SPACE_CRACK_SPECIALBOSS = 36,

	// 시공의틈새
	SPACE_CRACK_HUMANDEPON = 37,

	// 일반 미믹
	MIMIC_NORMAL = 38,

	// 엘리트 미믹
	MIMIC_ELITE = 39,

	// 성소
	SANCTUM = 40,

	// 성소 기사단설치용
	SANCTUM_NEXUS = 41,

	// 그림자 전장 보스(기사단 아레나)
	BATTLE_ARENA_BOSS = 42,

	// 비석: 처치 시 다수의 코인 드랍
	BATTLE_ARENA_TOMBSTONE = 43,

	// 부하 몬스터가 보호해주는 기믹이 들어간 특수 몬스터
	BATTLE_ARENA_GIMMICKMONSTER = 44,

	// 기사단 던전 보스
	GUILD_BOSS = 45,

	// 그림자 전장 전용 상인
	GUILDARENA_SHOP = 46,

	// 영지 던전 입장npc
	AGIT_DUNGEON = 47,

	// 그림자 전장 전용 회복사 NPC
	HP_RECOVER = 48,

	// 그림자 전장 전용 밀정의 요새 관리자
	SECRECT_AREA_PORTAL = 49,

	// 그림자 전장 전용 군단장의 보루 관리자
	MIDDLE_AREA_PORTAL = 50,

	// 그림자 전장 전용 군주의 성전 관리자
	FINAL_AREA_PORTAL = 51,

	// 영지 공략 던전 보스
	AGIT_DUNGEON_RAID_BOSS = 52,

	MAX = 53,
};

UENUM(BlueprintType)
// 공통 액션 타입
enum class CommonActionType : uint8
{

	// 스폰
	SPAWN = 0,

	// 죽음
	DYING = 1,

	// 평화 모드
	INPEACE = 2,

	// 텔레포트
	TELEPORT = 3,

	// 소셜
	SOCIAL = 4,

	// 무기 교체(클래스 체인지)
	WEAPON_CHANGE = 5,

	// 프랍 오브젝트 터치
	TOUCHPROP = 6,

	// 시공의 틈새 포탈 터치
	SPACECRACK = 7,

	// 기사단 캠프 인터렉션
	INTERACTION_CAMP = 8,

	// 소셜 액션
	IDLE_SOCIAL = 9,

	MAX = 10,
};

UENUM(BlueprintType)
// 서버에서 스폰하는 크리쳐 타입
enum class CreatureSpawnType : uint8
{

	// Default
	NONE = 0,

	// NPC
	NPC = 1,

	// PROP
	PROP = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 크리쳐 이동 타입
enum class CreatureMoveType : uint8
{

	// 일반 이동형
	NORMAL = 0,

	// 이동 불가, 회전 가능형
	IMMOVED = 1,

	// 이동, 회전 불가형
	IMMOVED_NOT_ROTATED = 2,

	MAX = 3,
};

UENUM(BlueprintType)
enum class NpcSpawnGroupType : uint8
{

	// SpawnGroup 내의 개체들간 아무런 기능없음, 기본값
	NONE = 0,

	// SpawnGroup 내의 개체들간 타겟 공유 및 전투 지원(LEADER-MEMBER)(미구현)
	NORMAL = 1,

	// 종속 관계 (MASTER-SUMMONEE)
	SUBORDINATION = 2,

	// SpawnGroup 내의 개체들중 임의 객체를 선택함
	RANDOM = 3,

	// 확률적 스폰(만분율 기반으로 확률적 스폰, 스폰되지 않으면 Respawn Interval 이후 다시 확률 스폰 시도) *spawnGroup 구분의 의미는 아니지만 요청으로 인해 해당 타입으로 신규 추가
	RANDOM_RATE = 4,

	MAX = 5,
};

UENUM(BlueprintType)
enum class NpcSpecialType : uint8
{

	// 없음
	NONE = 0,

	// 충돌하면 서버한테 이벤트 보내줌
	COLLIDE = 1,

	MAX = 2,
};

UENUM(BlueprintType)
enum class NpcSpecialStatusType : uint8
{

	// 없음
	NONE = 0,

	// 정지된 상태
	STOP = 1,

	MAX = 2,
};

UENUM(BlueprintType)
enum class NpcSpawnGroupMemberType : uint8
{

	// 기본값
	NONE = 0,

	// [NpcSpawnGroupType::NORMAL]

	// 그룹장
	LEADER = 1,

	// 그룹원
	MEMBER = 2,

	// [NpcSpawnGroupType::SUBORDINATION]

	// 소환자
	MASTER = 3,

	// 소환수(master 사망 시 같이 소멸, 소환수만 사망 시 일정 간격으로 리스폰)
	SUMMONEE = 4,

	MAX = 5,
};

UENUM(BlueprintType)
enum class RandomGroupSpawnType : uint8
{

	// 균동 랜덤
	NORMAL = 0,

	// 가중치 랜덤
	WEIGHT = 1,

	MAX = 2,
};

UENUM(BlueprintType)
enum class MonsterElement : uint8
{

	// 기본값
	NONE = 0,

	// 화염
	FLAME = 1,

	// 냉기
	FROST = 2,

	// 신성
	HOLY = 3,

	// 암흑
	DARK = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 문지기 콘텐츠타입
enum class GateKeeperContens : uint8
{

	// 던전이동
	DUNGEON = 0,

	MAX = 1,
};

UENUM(BlueprintType)
// 스탯 보정 타입
enum class StatCorrectionType : uint8
{

	// 등급
	GRADE = 0,

	// 속성
	ELEMENT = 1,

	// 종족
	RACE = 2,

	// 특성 - 강점
	STRENGTH = 3,

	// 특성 - 약점
	WEAKNESS = 4,

	// 등장 컨텐츠
	CONTENTS = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// 스탯 보정 연산 타입
enum class StatCalcType : uint8
{

	// 합연산
	PLUS = 0,

	// 곱연산
	MULTIFLY = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// NPC 스폰 컨텐츠 타입
enum class NpcSpawnContents : uint8
{

	// 없음
	NONE = 0,

	// 시공의 틈새
	SPACE_CRACK = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// NPC 정찰타입
enum class PatrolType : uint8
{

	// 없음
	NONE = 0,
	LINE = 1,
	CIRCLE = 2,
	SPOT = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 클라이언트 이모션 관련
enum class UserSocialEmotion : uint8
{
	NONE = 0,

	// 웃음
	LAUGH = 1,

	// 슬픔
	SADNESS = 2,

	// 부정
	DENY = 3,

	// 인사
	GREETING = 4,

	// 부탁
	FAVOR = 5,

	// 동의
	AGREE = 6,

	// 우쭐
	BRAG = 7,

	// 파이팅
	FIGHTING = 8,

	// 의욕 충만
	HIGHMOTIVATE = 9,

	// 하트
	HEART = 10,

	// 춤
	DANCE = 11,

	// 놀람
	SURPRISE = 12,

	// 기타 Idle 동작
	IDLE_EMOTION = 13,

	MAX = 14,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline CreatureGenderType CreatureGenderTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("ALL"), ESearchCase::IgnoreCase) == 0)	return CreatureGenderType::ALL;
	if (str.Compare(TEXT("MALE"), ESearchCase::IgnoreCase) == 0)	return CreatureGenderType::MALE;
	if (str.Compare(TEXT("FEMALE"), ESearchCase::IgnoreCase) == 0)	return CreatureGenderType::FEMALE;
	return CreatureGenderType::MAX;
}
inline CreatureRaceType CreatureRaceTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CreatureRaceType::NONE;
	if (str.Compare(TEXT("HUMAN"), ESearchCase::IgnoreCase) == 0)	return CreatureRaceType::HUMAN;
	if (str.Compare(TEXT("NORMAL_CREATURE"), ESearchCase::IgnoreCase) == 0)	return CreatureRaceType::NORMAL_CREATURE;
	if (str.Compare(TEXT("UNDEAD"), ESearchCase::IgnoreCase) == 0)	return CreatureRaceType::UNDEAD;
	if (str.Compare(TEXT("ELEMENTAL"), ESearchCase::IgnoreCase) == 0)	return CreatureRaceType::ELEMENTAL;
	if (str.Compare(TEXT("DEMON"), ESearchCase::IgnoreCase) == 0)	return CreatureRaceType::DEMON;
	if (str.Compare(TEXT("DEMIHUMAN"), ESearchCase::IgnoreCase) == 0)	return CreatureRaceType::DEMIHUMAN;
	if (str.Compare(TEXT("MAGICAL_CREATURE"), ESearchCase::IgnoreCase) == 0)	return CreatureRaceType::MAGICAL_CREATURE;
	if (str.Compare(TEXT("MARBLE"), ESearchCase::IgnoreCase) == 0)	return CreatureRaceType::MARBLE;
	if (str.Compare(TEXT("PLANT"), ESearchCase::IgnoreCase) == 0)	return CreatureRaceType::PLANT;
	return CreatureRaceType::MAX;
}
inline CreatureWeaponType CreatureWeaponTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CreatureWeaponType::NONE;
	if (str.Compare(TEXT("SWORD"), ESearchCase::IgnoreCase) == 0)	return CreatureWeaponType::SWORD;
	if (str.Compare(TEXT("DAGGER"), ESearchCase::IgnoreCase) == 0)	return CreatureWeaponType::DAGGER;
	if (str.Compare(TEXT("BOW"), ESearchCase::IgnoreCase) == 0)	return CreatureWeaponType::BOW;
	if (str.Compare(TEXT("WAND"), ESearchCase::IgnoreCase) == 0)	return CreatureWeaponType::WAND;
	if (str.Compare(TEXT("ORB"), ESearchCase::IgnoreCase) == 0)	return CreatureWeaponType::ORB;
	if (str.Compare(TEXT("SHOTGUN"), ESearchCase::IgnoreCase) == 0)	return CreatureWeaponType::SHOTGUN;
	if (str.Compare(TEXT("HAND"), ESearchCase::IgnoreCase) == 0)	return CreatureWeaponType::HAND;
	if (str.Compare(TEXT("ALL"), ESearchCase::IgnoreCase) == 0)	return CreatureWeaponType::ALL;
	return CreatureWeaponType::MAX;
}
inline CreatureRankType CreatureRankTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CreatureRankType::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return CreatureRankType::NORMAL;
	if (str.Compare(TEXT("ELITE"), ESearchCase::IgnoreCase) == 0)	return CreatureRankType::ELITE;
	if (str.Compare(TEXT("NAMED"), ESearchCase::IgnoreCase) == 0)	return CreatureRankType::NAMED;
	if (str.Compare(TEXT("BOSS"), ESearchCase::IgnoreCase) == 0)	return CreatureRankType::BOSS;
	if (str.Compare(TEXT("RAID"), ESearchCase::IgnoreCase) == 0)	return CreatureRankType::RAID;
	return CreatureRankType::MAX;
}
inline CommonBehaviorResolverType CommonBehaviorResolverTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::NONE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BLEED"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BLEED;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_POISON"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_POISON;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BURN"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BURN;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_HOT"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_HOT;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_SENSITIVE"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_SENSITIVE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_STUN"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_STUN;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_SILENCE"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_SILENCE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_BIND"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_BIND;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_CANNOT_WARP"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_CANNOT_WARP;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_BURY"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_BURY;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_DOWN"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_DOWN;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_BACK"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_BACK;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_FEAR"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_FEAR;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_TAUNT"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_TAUNT;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_MOVE_LOCATION"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_MOVE_LOCATION;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_POLYMORP"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_POLYMORP;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_COCOON"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_COCOON;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_FREEZE"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_FREEZE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_ROOT_WINDING"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_ROOT_WINDING;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_STAT"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_STAT_SLOW"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_SLOW;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_STAT_DASH"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_DASH;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_STAT_IMMUNE"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_IMMUNE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_STAT_VENT"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_VENT;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ARCAS_FAIRY_BUFF"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ARCAS_FAIRY_BUFF;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ON_REMOVE_EXE_SKILL_NOTIFY"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ON_REMOVE_EXE_SKILL_NOTIFY;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_NOT_AI_CHANGE_PHASE"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_NOT_AI_CHANGE_PHASE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_IMMORTAL"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_IMMORTAL;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_HIDING"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_HIDING;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BERSERKER_RAGE"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BERSERKER_RAGE;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_POINTED_ARROW"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_POINTED_ARROW;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_WEDGE_ARROW"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_WEDGE_ARROW;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_JAGGED_ARROW"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_JAGGED_ARROW;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_FINAL_HOWL"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_FINAL_HOWL;
	if (str.Compare(TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BONUS_DAMAGE"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BONUS_DAMAGE;
	if (str.Compare(TEXT("MOTION_CONTROLLER_TYPE_USER"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_USER;
	if (str.Compare(TEXT("MOTION_CONTROLLER_TYPE_JUMP"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_JUMP;
	if (str.Compare(TEXT("MOTION_CONTROLLER_TYPE_FALL"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_FALL;
	if (str.Compare(TEXT("MOTION_CONTROLLER_TYPE_SKILL"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_SKILL;
	if (str.Compare(TEXT("MOTION_CONTROLLER_TYPE_AI"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_AI;
	if (str.Compare(TEXT("MOTION_CONTROLLER_TYPE_REACTION"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_REACTION;
	if (str.Compare(TEXT("MOTION_CONTROLLER_TYPE_FEAR"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_FEAR;
	if (str.Compare(TEXT("MOTION_CONTROLLER_TYPE_CHARM"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_CHARM;
	if (str.Compare(TEXT("USER_BEHAVIOR_STATUS_TYPE_CHANGE_WEAPON_TYPE"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_CHANGE_WEAPON_TYPE;
	if (str.Compare(TEXT("USER_BEHAVIOR_STATUS_TYPE_FIRST_WEIGHT_PENALTY"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_FIRST_WEIGHT_PENALTY;
	if (str.Compare(TEXT("USER_BEHAVIOR_STATUS_TYPE_SECOND_WEIGHT_PENALTY"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_SECOND_WEIGHT_PENALTY;
	if (str.Compare(TEXT("USER_BEHAVIOR_STATUS_TYPE_IN_SAFE_ZONE"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_IN_SAFE_ZONE;
	if (str.Compare(TEXT("USER_BEHAVIOR_STATUS_TYPE_IN_SPAWN_ACTION"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_IN_SPAWN_ACTION;
	if (str.Compare(TEXT("USER_BEHAVIOR_STATUS_TYPE_IN_PERSONAL_TRADING"), ESearchCase::IgnoreCase) == 0)	return CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_IN_PERSONAL_TRADING;
	return CommonBehaviorResolverType::MAX;
}
inline CreatureActionType CreatureActionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_MOVE_DEFAULT"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_MOVE_ROOT_MOTION"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_MOVE_ROOT_MOTION;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_MOVE_LOCATON"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_MOVE_LOCATON;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_MOVE_FEAR"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_MOVE_FEAR;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_SKILL_BASIC"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_SKILL_BASIC;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_SKILL"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_SKILL;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_DASH_SKILL"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_DASH_SKILL;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_BUFF_SKILL"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_BUFF_SKILL;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_ITEM"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_ITEM;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_SOCIAL"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_SOCIAL;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_WARP"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_WARP;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_TAKE_DAMAGE"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_TAKE_DAMAGE;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_CHANGE_PAHSE"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_CHANGE_PAHSE;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_RECOVERY"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_RECOVERY;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_CHANGE_TARGET"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_CHANGE_TARGET;
	if (str.Compare(TEXT("CREATURE_ACTION_TYPE_MOUNT_VEHICLE"), ESearchCase::IgnoreCase) == 0)	return CreatureActionType::CREATURE_ACTION_TYPE_MOUNT_VEHICLE;
	return CreatureActionType::MAX;
}
inline UserBehaviorStatusType UserBehaviorStatusTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return UserBehaviorStatusType::NONE;
	if (str.Compare(TEXT("CHANGE_WEAPON_TYPE"), ESearchCase::IgnoreCase) == 0)	return UserBehaviorStatusType::CHANGE_WEAPON_TYPE;
	if (str.Compare(TEXT("FIRST_WEIGHT_PENALTY"), ESearchCase::IgnoreCase) == 0)	return UserBehaviorStatusType::FIRST_WEIGHT_PENALTY;
	if (str.Compare(TEXT("SECOND_WEIGHT_PENALTY"), ESearchCase::IgnoreCase) == 0)	return UserBehaviorStatusType::SECOND_WEIGHT_PENALTY;
	if (str.Compare(TEXT("IN_SAFE_ZONE"), ESearchCase::IgnoreCase) == 0)	return UserBehaviorStatusType::IN_SAFE_ZONE;
	if (str.Compare(TEXT("ARENA_WAITING"), ESearchCase::IgnoreCase) == 0)	return UserBehaviorStatusType::ARENA_WAITING;
	if (str.Compare(TEXT("SPAWN_ACTION"), ESearchCase::IgnoreCase) == 0)	return UserBehaviorStatusType::SPAWN_ACTION;
	if (str.Compare(TEXT("PERSONAL_TRADING"), ESearchCase::IgnoreCase) == 0)	return UserBehaviorStatusType::PERSONAL_TRADING;
	return UserBehaviorStatusType::MAX;
}
inline EReactionType EReactionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return EReactionType::NONE;
	if (str.Compare(TEXT("STUN"), ESearchCase::IgnoreCase) == 0)	return EReactionType::STUN;
	if (str.Compare(TEXT("HIT"), ESearchCase::IgnoreCase) == 0)	return EReactionType::HIT;
	if (str.Compare(TEXT("KNOCKDOWN"), ESearchCase::IgnoreCase) == 0)	return EReactionType::KNOCKDOWN;
	if (str.Compare(TEXT("KNOCKBACK"), ESearchCase::IgnoreCase) == 0)	return EReactionType::KNOCKBACK;
	if (str.Compare(TEXT("MOVELOCATION"), ESearchCase::IgnoreCase) == 0)	return EReactionType::MOVELOCATION;
	return EReactionType::MAX;
}
inline NpcFunctionType NpcFunctionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::NONE;
	if (str.Compare(TEXT("SHOP"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SHOP;
	if (str.Compare(TEXT("REVIVAL"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::REVIVAL;
	if (str.Compare(TEXT("PURCHASE"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::PURCHASE;
	if (str.Compare(TEXT("QUEST_DIALOG"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::QUEST_DIALOG;
	if (str.Compare(TEXT("SANDBAG"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SANDBAG;
	if (str.Compare(TEXT("DEPOT"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::DEPOT;
	if (str.Compare(TEXT("COMMUNITY"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::COMMUNITY;
	if (str.Compare(TEXT("EVENT_SHOP"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::EVENT_SHOP;
	if (str.Compare(TEXT("SHOP_EQUIP"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SHOP_EQUIP;
	if (str.Compare(TEXT("SHOP_SKILL_BOOK"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SHOP_SKILL_BOOK;
	if (str.Compare(TEXT("FIELD_BOSS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::FIELD_BOSS;
	if (str.Compare(TEXT("TREASURE_MONSTER"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::TREASURE_MONSTER;
	if (str.Compare(TEXT("CAMP"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::CAMP;
	if (str.Compare(TEXT("PARTY_DUNGEON_BOSS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::PARTY_DUNGEON_BOSS;
	if (str.Compare(TEXT("SPACE_TRACER"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SPACE_TRACER;
	if (str.Compare(TEXT("GATEKEEPER"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::GATEKEEPER;
	if (str.Compare(TEXT("DOOR"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::DOOR;
	if (str.Compare(TEXT("QUEST_BOARD"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::QUEST_BOARD;
	if (str.Compare(TEXT("MISSION_WAVE"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::MISSION_WAVE;
	if (str.Compare(TEXT("EVENT_PRAYER"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::EVENT_PRAYER;
	if (str.Compare(TEXT("SPAWN_MONSTER"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SPAWN_MONSTER;
	if (str.Compare(TEXT("WORLD_BOSS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::WORLD_BOSS;
	if (str.Compare(TEXT("INVADE_GUARDIAN"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::INVADE_GUARDIAN;
	if (str.Compare(TEXT("INVADE_WAGON"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::INVADE_WAGON;
	if (str.Compare(TEXT("INVADE_BOSS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::INVADE_BOSS;
	if (str.Compare(TEXT("MAGIC_OBJECT"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::MAGIC_OBJECT;
	if (str.Compare(TEXT("SPACE_CRACK_MONSTER"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SPACE_CRACK_MONSTER;
	if (str.Compare(TEXT("GUARD"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::GUARD;
	if (str.Compare(TEXT("INVADE_WAGON_GUARD"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::INVADE_WAGON_GUARD;
	if (str.Compare(TEXT("INVADE_BANDITS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::INVADE_BANDITS;
	if (str.Compare(TEXT("CART"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::CART;
	if (str.Compare(TEXT("SPACE_CRACK_SUPPLYMONSTER"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SPACE_CRACK_SUPPLYMONSTER;
	if (str.Compare(TEXT("SPACE_CRACK_ELITE"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SPACE_CRACK_ELITE;
	if (str.Compare(TEXT("SPACE_CRACK_CONTAMINANT"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SPACE_CRACK_CONTAMINANT;
	if (str.Compare(TEXT("SPACE_CRACK_BOSS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SPACE_CRACK_BOSS;
	if (str.Compare(TEXT("SPACE_CRACK_SPECIALBOSS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SPACE_CRACK_SPECIALBOSS;
	if (str.Compare(TEXT("SPACE_CRACK_HUMANDEPON"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SPACE_CRACK_HUMANDEPON;
	if (str.Compare(TEXT("MIMIC_NORMAL"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::MIMIC_NORMAL;
	if (str.Compare(TEXT("MIMIC_ELITE"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::MIMIC_ELITE;
	if (str.Compare(TEXT("SANCTUM"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SANCTUM;
	if (str.Compare(TEXT("SANCTUM_NEXUS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SANCTUM_NEXUS;
	if (str.Compare(TEXT("BATTLE_ARENA_BOSS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::BATTLE_ARENA_BOSS;
	if (str.Compare(TEXT("BATTLE_ARENA_TOMBSTONE"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::BATTLE_ARENA_TOMBSTONE;
	if (str.Compare(TEXT("BATTLE_ARENA_GIMMICKMONSTER"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::BATTLE_ARENA_GIMMICKMONSTER;
	if (str.Compare(TEXT("GUILD_BOSS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::GUILD_BOSS;
	if (str.Compare(TEXT("GUILDARENA_SHOP"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::GUILDARENA_SHOP;
	if (str.Compare(TEXT("AGIT_DUNGEON"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::AGIT_DUNGEON;
	if (str.Compare(TEXT("HP_RECOVER"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::HP_RECOVER;
	if (str.Compare(TEXT("SECRECT_AREA_PORTAL"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::SECRECT_AREA_PORTAL;
	if (str.Compare(TEXT("MIDDLE_AREA_PORTAL"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::MIDDLE_AREA_PORTAL;
	if (str.Compare(TEXT("FINAL_AREA_PORTAL"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::FINAL_AREA_PORTAL;
	if (str.Compare(TEXT("AGIT_DUNGEON_RAID_BOSS"), ESearchCase::IgnoreCase) == 0)	return NpcFunctionType::AGIT_DUNGEON_RAID_BOSS;
	return NpcFunctionType::MAX;
}
inline CommonActionType CommonActionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("SPAWN"), ESearchCase::IgnoreCase) == 0)	return CommonActionType::SPAWN;
	if (str.Compare(TEXT("DYING"), ESearchCase::IgnoreCase) == 0)	return CommonActionType::DYING;
	if (str.Compare(TEXT("INPEACE"), ESearchCase::IgnoreCase) == 0)	return CommonActionType::INPEACE;
	if (str.Compare(TEXT("TELEPORT"), ESearchCase::IgnoreCase) == 0)	return CommonActionType::TELEPORT;
	if (str.Compare(TEXT("SOCIAL"), ESearchCase::IgnoreCase) == 0)	return CommonActionType::SOCIAL;
	if (str.Compare(TEXT("WEAPON_CHANGE"), ESearchCase::IgnoreCase) == 0)	return CommonActionType::WEAPON_CHANGE;
	if (str.Compare(TEXT("TOUCHPROP"), ESearchCase::IgnoreCase) == 0)	return CommonActionType::TOUCHPROP;
	if (str.Compare(TEXT("SPACECRACK"), ESearchCase::IgnoreCase) == 0)	return CommonActionType::SPACECRACK;
	if (str.Compare(TEXT("INTERACTION_CAMP"), ESearchCase::IgnoreCase) == 0)	return CommonActionType::INTERACTION_CAMP;
	if (str.Compare(TEXT("IDLE_SOCIAL"), ESearchCase::IgnoreCase) == 0)	return CommonActionType::IDLE_SOCIAL;
	return CommonActionType::MAX;
}
inline CreatureSpawnType CreatureSpawnTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CreatureSpawnType::NONE;
	if (str.Compare(TEXT("NPC"), ESearchCase::IgnoreCase) == 0)	return CreatureSpawnType::NPC;
	if (str.Compare(TEXT("PROP"), ESearchCase::IgnoreCase) == 0)	return CreatureSpawnType::PROP;
	return CreatureSpawnType::MAX;
}
inline CreatureMoveType CreatureMoveTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return CreatureMoveType::NORMAL;
	if (str.Compare(TEXT("IMMOVED"), ESearchCase::IgnoreCase) == 0)	return CreatureMoveType::IMMOVED;
	if (str.Compare(TEXT("IMMOVED_NOT_ROTATED"), ESearchCase::IgnoreCase) == 0)	return CreatureMoveType::IMMOVED_NOT_ROTATED;
	return CreatureMoveType::MAX;
}
inline NpcSpawnGroupType NpcSpawnGroupTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnGroupType::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnGroupType::NORMAL;
	if (str.Compare(TEXT("SUBORDINATION"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnGroupType::SUBORDINATION;
	if (str.Compare(TEXT("RANDOM"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnGroupType::RANDOM;
	if (str.Compare(TEXT("RANDOM_RATE"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnGroupType::RANDOM_RATE;
	return NpcSpawnGroupType::MAX;
}
inline NpcSpecialType NpcSpecialTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return NpcSpecialType::NONE;
	if (str.Compare(TEXT("COLLIDE"), ESearchCase::IgnoreCase) == 0)	return NpcSpecialType::COLLIDE;
	return NpcSpecialType::MAX;
}
inline NpcSpecialStatusType NpcSpecialStatusTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return NpcSpecialStatusType::NONE;
	if (str.Compare(TEXT("STOP"), ESearchCase::IgnoreCase) == 0)	return NpcSpecialStatusType::STOP;
	return NpcSpecialStatusType::MAX;
}
inline NpcSpawnGroupMemberType NpcSpawnGroupMemberTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnGroupMemberType::NONE;
	if (str.Compare(TEXT("LEADER"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnGroupMemberType::LEADER;
	if (str.Compare(TEXT("MEMBER"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnGroupMemberType::MEMBER;
	if (str.Compare(TEXT("MASTER"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnGroupMemberType::MASTER;
	if (str.Compare(TEXT("SUMMONEE"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnGroupMemberType::SUMMONEE;
	return NpcSpawnGroupMemberType::MAX;
}
inline RandomGroupSpawnType RandomGroupSpawnTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return RandomGroupSpawnType::NORMAL;
	if (str.Compare(TEXT("WEIGHT"), ESearchCase::IgnoreCase) == 0)	return RandomGroupSpawnType::WEIGHT;
	return RandomGroupSpawnType::MAX;
}
inline MonsterElement MonsterElementStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return MonsterElement::NONE;
	if (str.Compare(TEXT("FLAME"), ESearchCase::IgnoreCase) == 0)	return MonsterElement::FLAME;
	if (str.Compare(TEXT("FROST"), ESearchCase::IgnoreCase) == 0)	return MonsterElement::FROST;
	if (str.Compare(TEXT("HOLY"), ESearchCase::IgnoreCase) == 0)	return MonsterElement::HOLY;
	if (str.Compare(TEXT("DARK"), ESearchCase::IgnoreCase) == 0)	return MonsterElement::DARK;
	return MonsterElement::MAX;
}
inline GateKeeperContens GateKeeperContensStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("DUNGEON"), ESearchCase::IgnoreCase) == 0)	return GateKeeperContens::DUNGEON;
	return GateKeeperContens::MAX;
}
inline StatCorrectionType StatCorrectionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("GRADE"), ESearchCase::IgnoreCase) == 0)	return StatCorrectionType::GRADE;
	if (str.Compare(TEXT("ELEMENT"), ESearchCase::IgnoreCase) == 0)	return StatCorrectionType::ELEMENT;
	if (str.Compare(TEXT("RACE"), ESearchCase::IgnoreCase) == 0)	return StatCorrectionType::RACE;
	if (str.Compare(TEXT("STRENGTH"), ESearchCase::IgnoreCase) == 0)	return StatCorrectionType::STRENGTH;
	if (str.Compare(TEXT("WEAKNESS"), ESearchCase::IgnoreCase) == 0)	return StatCorrectionType::WEAKNESS;
	if (str.Compare(TEXT("CONTENTS"), ESearchCase::IgnoreCase) == 0)	return StatCorrectionType::CONTENTS;
	return StatCorrectionType::MAX;
}
inline StatCalcType StatCalcTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("PLUS"), ESearchCase::IgnoreCase) == 0)	return StatCalcType::PLUS;
	if (str.Compare(TEXT("MULTIFLY"), ESearchCase::IgnoreCase) == 0)	return StatCalcType::MULTIFLY;
	return StatCalcType::MAX;
}
inline NpcSpawnContents NpcSpawnContentsStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnContents::NONE;
	if (str.Compare(TEXT("SPACE_CRACK"), ESearchCase::IgnoreCase) == 0)	return NpcSpawnContents::SPACE_CRACK;
	return NpcSpawnContents::MAX;
}
inline PatrolType PatrolTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return PatrolType::NONE;
	if (str.Compare(TEXT("LINE"), ESearchCase::IgnoreCase) == 0)	return PatrolType::LINE;
	if (str.Compare(TEXT("CIRCLE"), ESearchCase::IgnoreCase) == 0)	return PatrolType::CIRCLE;
	if (str.Compare(TEXT("SPOT"), ESearchCase::IgnoreCase) == 0)	return PatrolType::SPOT;
	return PatrolType::MAX;
}
inline UserSocialEmotion UserSocialEmotionStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::NONE;
	if (str.Compare(TEXT("LAUGH"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::LAUGH;
	if (str.Compare(TEXT("SADNESS"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::SADNESS;
	if (str.Compare(TEXT("DENY"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::DENY;
	if (str.Compare(TEXT("GREETING"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::GREETING;
	if (str.Compare(TEXT("FAVOR"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::FAVOR;
	if (str.Compare(TEXT("AGREE"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::AGREE;
	if (str.Compare(TEXT("BRAG"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::BRAG;
	if (str.Compare(TEXT("FIGHTING"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::FIGHTING;
	if (str.Compare(TEXT("HIGHMOTIVATE"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::HIGHMOTIVATE;
	if (str.Compare(TEXT("HEART"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::HEART;
	if (str.Compare(TEXT("DANCE"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::DANCE;
	if (str.Compare(TEXT("SURPRISE"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::SURPRISE;
	if (str.Compare(TEXT("IDLE_EMOTION"), ESearchCase::IgnoreCase) == 0)	return UserSocialEmotion::IDLE_EMOTION;
	return UserSocialEmotion::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString CreatureGenderTypeEnumToString(CreatureGenderType value)
{
	switch (value)
	{
		case CreatureGenderType::ALL: return TEXT("ALL");
		case CreatureGenderType::MALE: return TEXT("MALE");
		case CreatureGenderType::FEMALE: return TEXT("FEMALE");
	}
	return FString();
}
inline FString CreatureRaceTypeEnumToString(CreatureRaceType value)
{
	switch (value)
	{
		case CreatureRaceType::NONE: return TEXT("NONE");
		case CreatureRaceType::HUMAN: return TEXT("HUMAN");
		case CreatureRaceType::NORMAL_CREATURE: return TEXT("NORMAL_CREATURE");
		case CreatureRaceType::UNDEAD: return TEXT("UNDEAD");
		case CreatureRaceType::ELEMENTAL: return TEXT("ELEMENTAL");
		case CreatureRaceType::DEMON: return TEXT("DEMON");
		case CreatureRaceType::DEMIHUMAN: return TEXT("DEMIHUMAN");
		case CreatureRaceType::MAGICAL_CREATURE: return TEXT("MAGICAL_CREATURE");
		case CreatureRaceType::MARBLE: return TEXT("MARBLE");
		case CreatureRaceType::PLANT: return TEXT("PLANT");
	}
	return FString();
}
inline FString CreatureWeaponTypeEnumToString(CreatureWeaponType value)
{
	switch (value)
	{
		case CreatureWeaponType::NONE: return TEXT("NONE");
		case CreatureWeaponType::SWORD: return TEXT("SWORD");
		case CreatureWeaponType::DAGGER: return TEXT("DAGGER");
		case CreatureWeaponType::BOW: return TEXT("BOW");
		case CreatureWeaponType::WAND: return TEXT("WAND");
		case CreatureWeaponType::ORB: return TEXT("ORB");
		case CreatureWeaponType::SHOTGUN: return TEXT("SHOTGUN");
		case CreatureWeaponType::HAND: return TEXT("HAND");
		case CreatureWeaponType::ALL: return TEXT("ALL");
	}
	return FString();
}
inline FString CreatureRankTypeEnumToString(CreatureRankType value)
{
	switch (value)
	{
		case CreatureRankType::NONE: return TEXT("NONE");
		case CreatureRankType::NORMAL: return TEXT("NORMAL");
		case CreatureRankType::ELITE: return TEXT("ELITE");
		case CreatureRankType::NAMED: return TEXT("NAMED");
		case CreatureRankType::BOSS: return TEXT("BOSS");
		case CreatureRankType::RAID: return TEXT("RAID");
	}
	return FString();
}
inline FString CommonBehaviorResolverTypeEnumToString(CommonBehaviorResolverType value)
{
	switch (value)
	{
		case CommonBehaviorResolverType::NONE: return TEXT("NONE");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BLEED: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BLEED");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_POISON: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_POISON");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BURN: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BURN");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_HOT: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_HOT");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_SENSITIVE: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_SENSITIVE");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_STUN: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_STUN");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_SILENCE: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_SILENCE");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_BIND: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_BIND");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_CANNOT_WARP: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_CANNOT_WARP");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_BURY: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_BURY");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_DOWN: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_DOWN");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_BACK: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_BACK");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_FEAR: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_FEAR");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_TAUNT: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_TAUNT");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_MOVE_LOCATION: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_MOVE_LOCATION");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_POLYMORP: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_POLYMORP");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_COCOON: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_COCOON");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_FREEZE: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_FREEZE");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_ROOT_WINDING: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_CC_ROOT_WINDING");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_STAT");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_SLOW: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_STAT_SLOW");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_DASH: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_STAT_DASH");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_IMMUNE: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_STAT_IMMUNE");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_VENT: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_STAT_VENT");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ARCAS_FAIRY_BUFF: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ARCAS_FAIRY_BUFF");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ON_REMOVE_EXE_SKILL_NOTIFY: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ON_REMOVE_EXE_SKILL_NOTIFY");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_NOT_AI_CHANGE_PHASE: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_NOT_AI_CHANGE_PHASE");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_IMMORTAL: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_IMMORTAL");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_HIDING: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_HIDING");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BERSERKER_RAGE: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BERSERKER_RAGE");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_POINTED_ARROW: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_POINTED_ARROW");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_WEDGE_ARROW: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_WEDGE_ARROW");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_JAGGED_ARROW: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_JAGGED_ARROW");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_FINAL_HOWL: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_FINAL_HOWL");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BONUS_DAMAGE: return TEXT("ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BONUS_DAMAGE");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_USER: return TEXT("MOTION_CONTROLLER_TYPE_USER");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_JUMP: return TEXT("MOTION_CONTROLLER_TYPE_JUMP");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_FALL: return TEXT("MOTION_CONTROLLER_TYPE_FALL");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_SKILL: return TEXT("MOTION_CONTROLLER_TYPE_SKILL");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_AI: return TEXT("MOTION_CONTROLLER_TYPE_AI");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_REACTION: return TEXT("MOTION_CONTROLLER_TYPE_REACTION");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_FEAR: return TEXT("MOTION_CONTROLLER_TYPE_FEAR");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_CHARM: return TEXT("MOTION_CONTROLLER_TYPE_CHARM");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_CHANGE_WEAPON_TYPE: return TEXT("USER_BEHAVIOR_STATUS_TYPE_CHANGE_WEAPON_TYPE");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_FIRST_WEIGHT_PENALTY: return TEXT("USER_BEHAVIOR_STATUS_TYPE_FIRST_WEIGHT_PENALTY");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_SECOND_WEIGHT_PENALTY: return TEXT("USER_BEHAVIOR_STATUS_TYPE_SECOND_WEIGHT_PENALTY");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_IN_SAFE_ZONE: return TEXT("USER_BEHAVIOR_STATUS_TYPE_IN_SAFE_ZONE");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_IN_SPAWN_ACTION: return TEXT("USER_BEHAVIOR_STATUS_TYPE_IN_SPAWN_ACTION");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_IN_PERSONAL_TRADING: return TEXT("USER_BEHAVIOR_STATUS_TYPE_IN_PERSONAL_TRADING");
	}
	return FString();
}
inline FString CreatureActionTypeEnumToString(CreatureActionType value)
{
	switch (value)
	{
		case CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT: return TEXT("CREATURE_ACTION_TYPE_MOVE_DEFAULT");
		case CreatureActionType::CREATURE_ACTION_TYPE_MOVE_ROOT_MOTION: return TEXT("CREATURE_ACTION_TYPE_MOVE_ROOT_MOTION");
		case CreatureActionType::CREATURE_ACTION_TYPE_MOVE_LOCATON: return TEXT("CREATURE_ACTION_TYPE_MOVE_LOCATON");
		case CreatureActionType::CREATURE_ACTION_TYPE_MOVE_FEAR: return TEXT("CREATURE_ACTION_TYPE_MOVE_FEAR");
		case CreatureActionType::CREATURE_ACTION_TYPE_SKILL_BASIC: return TEXT("CREATURE_ACTION_TYPE_SKILL_BASIC");
		case CreatureActionType::CREATURE_ACTION_TYPE_SKILL: return TEXT("CREATURE_ACTION_TYPE_SKILL");
		case CreatureActionType::CREATURE_ACTION_TYPE_DASH_SKILL: return TEXT("CREATURE_ACTION_TYPE_DASH_SKILL");
		case CreatureActionType::CREATURE_ACTION_TYPE_BUFF_SKILL: return TEXT("CREATURE_ACTION_TYPE_BUFF_SKILL");
		case CreatureActionType::CREATURE_ACTION_TYPE_ITEM: return TEXT("CREATURE_ACTION_TYPE_ITEM");
		case CreatureActionType::CREATURE_ACTION_TYPE_SOCIAL: return TEXT("CREATURE_ACTION_TYPE_SOCIAL");
		case CreatureActionType::CREATURE_ACTION_TYPE_WARP: return TEXT("CREATURE_ACTION_TYPE_WARP");
		case CreatureActionType::CREATURE_ACTION_TYPE_TAKE_DAMAGE: return TEXT("CREATURE_ACTION_TYPE_TAKE_DAMAGE");
		case CreatureActionType::CREATURE_ACTION_TYPE_CHANGE_PAHSE: return TEXT("CREATURE_ACTION_TYPE_CHANGE_PAHSE");
		case CreatureActionType::CREATURE_ACTION_TYPE_RECOVERY: return TEXT("CREATURE_ACTION_TYPE_RECOVERY");
		case CreatureActionType::CREATURE_ACTION_TYPE_CHANGE_TARGET: return TEXT("CREATURE_ACTION_TYPE_CHANGE_TARGET");
		case CreatureActionType::CREATURE_ACTION_TYPE_MOUNT_VEHICLE: return TEXT("CREATURE_ACTION_TYPE_MOUNT_VEHICLE");
	}
	return FString();
}
inline FString UserBehaviorStatusTypeEnumToString(UserBehaviorStatusType value)
{
	switch (value)
	{
		case UserBehaviorStatusType::NONE: return TEXT("NONE");
		case UserBehaviorStatusType::CHANGE_WEAPON_TYPE: return TEXT("CHANGE_WEAPON_TYPE");
		case UserBehaviorStatusType::FIRST_WEIGHT_PENALTY: return TEXT("FIRST_WEIGHT_PENALTY");
		case UserBehaviorStatusType::SECOND_WEIGHT_PENALTY: return TEXT("SECOND_WEIGHT_PENALTY");
		case UserBehaviorStatusType::IN_SAFE_ZONE: return TEXT("IN_SAFE_ZONE");
		case UserBehaviorStatusType::ARENA_WAITING: return TEXT("ARENA_WAITING");
		case UserBehaviorStatusType::SPAWN_ACTION: return TEXT("SPAWN_ACTION");
		case UserBehaviorStatusType::PERSONAL_TRADING: return TEXT("PERSONAL_TRADING");
	}
	return FString();
}
inline FString EReactionTypeEnumToString(EReactionType value)
{
	switch (value)
	{
		case EReactionType::NONE: return TEXT("NONE");
		case EReactionType::STUN: return TEXT("STUN");
		case EReactionType::HIT: return TEXT("HIT");
		case EReactionType::KNOCKDOWN: return TEXT("KNOCKDOWN");
		case EReactionType::KNOCKBACK: return TEXT("KNOCKBACK");
		case EReactionType::MOVELOCATION: return TEXT("MOVELOCATION");
	}
	return FString();
}
inline FString NpcFunctionTypeEnumToString(NpcFunctionType value)
{
	switch (value)
	{
		case NpcFunctionType::NONE: return TEXT("NONE");
		case NpcFunctionType::SHOP: return TEXT("SHOP");
		case NpcFunctionType::REVIVAL: return TEXT("REVIVAL");
		case NpcFunctionType::PURCHASE: return TEXT("PURCHASE");
		case NpcFunctionType::QUEST_DIALOG: return TEXT("QUEST_DIALOG");
		case NpcFunctionType::SANDBAG: return TEXT("SANDBAG");
		case NpcFunctionType::DEPOT: return TEXT("DEPOT");
		case NpcFunctionType::COMMUNITY: return TEXT("COMMUNITY");
		case NpcFunctionType::EVENT_SHOP: return TEXT("EVENT_SHOP");
		case NpcFunctionType::SHOP_EQUIP: return TEXT("SHOP_EQUIP");
		case NpcFunctionType::SHOP_SKILL_BOOK: return TEXT("SHOP_SKILL_BOOK");
		case NpcFunctionType::FIELD_BOSS: return TEXT("FIELD_BOSS");
		case NpcFunctionType::TREASURE_MONSTER: return TEXT("TREASURE_MONSTER");
		case NpcFunctionType::CAMP: return TEXT("CAMP");
		case NpcFunctionType::PARTY_DUNGEON_BOSS: return TEXT("PARTY_DUNGEON_BOSS");
		case NpcFunctionType::SPACE_TRACER: return TEXT("SPACE_TRACER");
		case NpcFunctionType::GATEKEEPER: return TEXT("GATEKEEPER");
		case NpcFunctionType::DOOR: return TEXT("DOOR");
		case NpcFunctionType::QUEST_BOARD: return TEXT("QUEST_BOARD");
		case NpcFunctionType::MISSION_WAVE: return TEXT("MISSION_WAVE");
		case NpcFunctionType::EVENT_PRAYER: return TEXT("EVENT_PRAYER");
		case NpcFunctionType::SPAWN_MONSTER: return TEXT("SPAWN_MONSTER");
		case NpcFunctionType::WORLD_BOSS: return TEXT("WORLD_BOSS");
		case NpcFunctionType::INVADE_GUARDIAN: return TEXT("INVADE_GUARDIAN");
		case NpcFunctionType::INVADE_WAGON: return TEXT("INVADE_WAGON");
		case NpcFunctionType::INVADE_BOSS: return TEXT("INVADE_BOSS");
		case NpcFunctionType::MAGIC_OBJECT: return TEXT("MAGIC_OBJECT");
		case NpcFunctionType::SPACE_CRACK_MONSTER: return TEXT("SPACE_CRACK_MONSTER");
		case NpcFunctionType::GUARD: return TEXT("GUARD");
		case NpcFunctionType::INVADE_WAGON_GUARD: return TEXT("INVADE_WAGON_GUARD");
		case NpcFunctionType::INVADE_BANDITS: return TEXT("INVADE_BANDITS");
		case NpcFunctionType::CART: return TEXT("CART");
		case NpcFunctionType::SPACE_CRACK_SUPPLYMONSTER: return TEXT("SPACE_CRACK_SUPPLYMONSTER");
		case NpcFunctionType::SPACE_CRACK_ELITE: return TEXT("SPACE_CRACK_ELITE");
		case NpcFunctionType::SPACE_CRACK_CONTAMINANT: return TEXT("SPACE_CRACK_CONTAMINANT");
		case NpcFunctionType::SPACE_CRACK_BOSS: return TEXT("SPACE_CRACK_BOSS");
		case NpcFunctionType::SPACE_CRACK_SPECIALBOSS: return TEXT("SPACE_CRACK_SPECIALBOSS");
		case NpcFunctionType::SPACE_CRACK_HUMANDEPON: return TEXT("SPACE_CRACK_HUMANDEPON");
		case NpcFunctionType::MIMIC_NORMAL: return TEXT("MIMIC_NORMAL");
		case NpcFunctionType::MIMIC_ELITE: return TEXT("MIMIC_ELITE");
		case NpcFunctionType::SANCTUM: return TEXT("SANCTUM");
		case NpcFunctionType::SANCTUM_NEXUS: return TEXT("SANCTUM_NEXUS");
		case NpcFunctionType::BATTLE_ARENA_BOSS: return TEXT("BATTLE_ARENA_BOSS");
		case NpcFunctionType::BATTLE_ARENA_TOMBSTONE: return TEXT("BATTLE_ARENA_TOMBSTONE");
		case NpcFunctionType::BATTLE_ARENA_GIMMICKMONSTER: return TEXT("BATTLE_ARENA_GIMMICKMONSTER");
		case NpcFunctionType::GUILD_BOSS: return TEXT("GUILD_BOSS");
		case NpcFunctionType::GUILDARENA_SHOP: return TEXT("GUILDARENA_SHOP");
		case NpcFunctionType::AGIT_DUNGEON: return TEXT("AGIT_DUNGEON");
		case NpcFunctionType::HP_RECOVER: return TEXT("HP_RECOVER");
		case NpcFunctionType::SECRECT_AREA_PORTAL: return TEXT("SECRECT_AREA_PORTAL");
		case NpcFunctionType::MIDDLE_AREA_PORTAL: return TEXT("MIDDLE_AREA_PORTAL");
		case NpcFunctionType::FINAL_AREA_PORTAL: return TEXT("FINAL_AREA_PORTAL");
		case NpcFunctionType::AGIT_DUNGEON_RAID_BOSS: return TEXT("AGIT_DUNGEON_RAID_BOSS");
	}
	return FString();
}
inline FString CommonActionTypeEnumToString(CommonActionType value)
{
	switch (value)
	{
		case CommonActionType::SPAWN: return TEXT("SPAWN");
		case CommonActionType::DYING: return TEXT("DYING");
		case CommonActionType::INPEACE: return TEXT("INPEACE");
		case CommonActionType::TELEPORT: return TEXT("TELEPORT");
		case CommonActionType::SOCIAL: return TEXT("SOCIAL");
		case CommonActionType::WEAPON_CHANGE: return TEXT("WEAPON_CHANGE");
		case CommonActionType::TOUCHPROP: return TEXT("TOUCHPROP");
		case CommonActionType::SPACECRACK: return TEXT("SPACECRACK");
		case CommonActionType::INTERACTION_CAMP: return TEXT("INTERACTION_CAMP");
		case CommonActionType::IDLE_SOCIAL: return TEXT("IDLE_SOCIAL");
	}
	return FString();
}
inline FString CreatureSpawnTypeEnumToString(CreatureSpawnType value)
{
	switch (value)
	{
		case CreatureSpawnType::NONE: return TEXT("NONE");
		case CreatureSpawnType::NPC: return TEXT("NPC");
		case CreatureSpawnType::PROP: return TEXT("PROP");
	}
	return FString();
}
inline FString CreatureMoveTypeEnumToString(CreatureMoveType value)
{
	switch (value)
	{
		case CreatureMoveType::NORMAL: return TEXT("NORMAL");
		case CreatureMoveType::IMMOVED: return TEXT("IMMOVED");
		case CreatureMoveType::IMMOVED_NOT_ROTATED: return TEXT("IMMOVED_NOT_ROTATED");
	}
	return FString();
}
inline FString NpcSpawnGroupTypeEnumToString(NpcSpawnGroupType value)
{
	switch (value)
	{
		case NpcSpawnGroupType::NONE: return TEXT("NONE");
		case NpcSpawnGroupType::NORMAL: return TEXT("NORMAL");
		case NpcSpawnGroupType::SUBORDINATION: return TEXT("SUBORDINATION");
		case NpcSpawnGroupType::RANDOM: return TEXT("RANDOM");
		case NpcSpawnGroupType::RANDOM_RATE: return TEXT("RANDOM_RATE");
	}
	return FString();
}
inline FString NpcSpecialTypeEnumToString(NpcSpecialType value)
{
	switch (value)
	{
		case NpcSpecialType::NONE: return TEXT("NONE");
		case NpcSpecialType::COLLIDE: return TEXT("COLLIDE");
	}
	return FString();
}
inline FString NpcSpecialStatusTypeEnumToString(NpcSpecialStatusType value)
{
	switch (value)
	{
		case NpcSpecialStatusType::NONE: return TEXT("NONE");
		case NpcSpecialStatusType::STOP: return TEXT("STOP");
	}
	return FString();
}
inline FString NpcSpawnGroupMemberTypeEnumToString(NpcSpawnGroupMemberType value)
{
	switch (value)
	{
		case NpcSpawnGroupMemberType::NONE: return TEXT("NONE");
		case NpcSpawnGroupMemberType::LEADER: return TEXT("LEADER");
		case NpcSpawnGroupMemberType::MEMBER: return TEXT("MEMBER");
		case NpcSpawnGroupMemberType::MASTER: return TEXT("MASTER");
		case NpcSpawnGroupMemberType::SUMMONEE: return TEXT("SUMMONEE");
	}
	return FString();
}
inline FString RandomGroupSpawnTypeEnumToString(RandomGroupSpawnType value)
{
	switch (value)
	{
		case RandomGroupSpawnType::NORMAL: return TEXT("NORMAL");
		case RandomGroupSpawnType::WEIGHT: return TEXT("WEIGHT");
	}
	return FString();
}
inline FString MonsterElementEnumToString(MonsterElement value)
{
	switch (value)
	{
		case MonsterElement::NONE: return TEXT("NONE");
		case MonsterElement::FLAME: return TEXT("FLAME");
		case MonsterElement::FROST: return TEXT("FROST");
		case MonsterElement::HOLY: return TEXT("HOLY");
		case MonsterElement::DARK: return TEXT("DARK");
	}
	return FString();
}
inline FString GateKeeperContensEnumToString(GateKeeperContens value)
{
	switch (value)
	{
		case GateKeeperContens::DUNGEON: return TEXT("DUNGEON");
	}
	return FString();
}
inline FString StatCorrectionTypeEnumToString(StatCorrectionType value)
{
	switch (value)
	{
		case StatCorrectionType::GRADE: return TEXT("GRADE");
		case StatCorrectionType::ELEMENT: return TEXT("ELEMENT");
		case StatCorrectionType::RACE: return TEXT("RACE");
		case StatCorrectionType::STRENGTH: return TEXT("STRENGTH");
		case StatCorrectionType::WEAKNESS: return TEXT("WEAKNESS");
		case StatCorrectionType::CONTENTS: return TEXT("CONTENTS");
	}
	return FString();
}
inline FString StatCalcTypeEnumToString(StatCalcType value)
{
	switch (value)
	{
		case StatCalcType::PLUS: return TEXT("PLUS");
		case StatCalcType::MULTIFLY: return TEXT("MULTIFLY");
	}
	return FString();
}
inline FString NpcSpawnContentsEnumToString(NpcSpawnContents value)
{
	switch (value)
	{
		case NpcSpawnContents::NONE: return TEXT("NONE");
		case NpcSpawnContents::SPACE_CRACK: return TEXT("SPACE_CRACK");
	}
	return FString();
}
inline FString PatrolTypeEnumToString(PatrolType value)
{
	switch (value)
	{
		case PatrolType::NONE: return TEXT("NONE");
		case PatrolType::LINE: return TEXT("LINE");
		case PatrolType::CIRCLE: return TEXT("CIRCLE");
		case PatrolType::SPOT: return TEXT("SPOT");
	}
	return FString();
}
inline FString UserSocialEmotionEnumToString(UserSocialEmotion value)
{
	switch (value)
	{
		case UserSocialEmotion::NONE: return TEXT("NONE");
		case UserSocialEmotion::LAUGH: return TEXT("LAUGH");
		case UserSocialEmotion::SADNESS: return TEXT("SADNESS");
		case UserSocialEmotion::DENY: return TEXT("DENY");
		case UserSocialEmotion::GREETING: return TEXT("GREETING");
		case UserSocialEmotion::FAVOR: return TEXT("FAVOR");
		case UserSocialEmotion::AGREE: return TEXT("AGREE");
		case UserSocialEmotion::BRAG: return TEXT("BRAG");
		case UserSocialEmotion::FIGHTING: return TEXT("FIGHTING");
		case UserSocialEmotion::HIGHMOTIVATE: return TEXT("HIGHMOTIVATE");
		case UserSocialEmotion::HEART: return TEXT("HEART");
		case UserSocialEmotion::DANCE: return TEXT("DANCE");
		case UserSocialEmotion::SURPRISE: return TEXT("SURPRISE");
		case UserSocialEmotion::IDLE_EMOTION: return TEXT("IDLE_EMOTION");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString CreatureGenderTypeEnumToDisplayKey(CreatureGenderType value)
{
	switch (value)
	{
		case CreatureGenderType::ALL: return TEXT("");
		case CreatureGenderType::MALE: return TEXT("");
		case CreatureGenderType::FEMALE: return TEXT("");
	}
	return FString();
}
inline FString CreatureRaceTypeEnumToDisplayKey(CreatureRaceType value)
{
	switch (value)
	{
		case CreatureRaceType::NONE: return TEXT("");
		case CreatureRaceType::HUMAN: return TEXT("");
		case CreatureRaceType::NORMAL_CREATURE: return TEXT("");
		case CreatureRaceType::UNDEAD: return TEXT("");
		case CreatureRaceType::ELEMENTAL: return TEXT("");
		case CreatureRaceType::DEMON: return TEXT("");
		case CreatureRaceType::DEMIHUMAN: return TEXT("");
		case CreatureRaceType::MAGICAL_CREATURE: return TEXT("");
		case CreatureRaceType::MARBLE: return TEXT("");
		case CreatureRaceType::PLANT: return TEXT("");
	}
	return FString();
}
inline FString CreatureWeaponTypeEnumToDisplayKey(CreatureWeaponType value)
{
	switch (value)
	{
		case CreatureWeaponType::NONE: return TEXT("");
		case CreatureWeaponType::SWORD: return TEXT("");
		case CreatureWeaponType::DAGGER: return TEXT("");
		case CreatureWeaponType::BOW: return TEXT("");
		case CreatureWeaponType::WAND: return TEXT("");
		case CreatureWeaponType::ORB: return TEXT("");
		case CreatureWeaponType::SHOTGUN: return TEXT("");
		case CreatureWeaponType::HAND: return TEXT("");
		case CreatureWeaponType::ALL: return TEXT("");
	}
	return FString();
}
inline FString CreatureRankTypeEnumToDisplayKey(CreatureRankType value)
{
	switch (value)
	{
		case CreatureRankType::NONE: return TEXT("");
		case CreatureRankType::NORMAL: return TEXT("");
		case CreatureRankType::ELITE: return TEXT("");
		case CreatureRankType::NAMED: return TEXT("");
		case CreatureRankType::BOSS: return TEXT("");
		case CreatureRankType::RAID: return TEXT("");
	}
	return FString();
}
inline FString CommonBehaviorResolverTypeEnumToDisplayKey(CommonBehaviorResolverType value)
{
	switch (value)
	{
		case CommonBehaviorResolverType::NONE: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BLEED: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_POISON: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_BURN: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_HOT: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_OVERTIME_SENSITIVE: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_STUN: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_SILENCE: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_BIND: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_CANNOT_WARP: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_BURY: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_DOWN: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_KNOCK_BACK: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_FEAR: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_TAUNT: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_MOVE_LOCATION: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_POLYMORP: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_COCOON: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_FREEZE: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_CC_ROOT_WINDING: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_SLOW: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_DASH: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_IMMUNE: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_STAT_VENT: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ARCAS_FAIRY_BUFF: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_ON_REMOVE_EXE_SKILL_NOTIFY: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_NOT_AI_CHANGE_PHASE: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_IMMORTAL: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_HIDING: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BERSERKER_RAGE: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_POINTED_ARROW: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_WEDGE_ARROW: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_JAGGED_ARROW: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_FINAL_HOWL: return TEXT("");
		case CommonBehaviorResolverType::ABNORMALITY_EFFECT_SUB_TYPE_UTIL_BONUS_DAMAGE: return TEXT("");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_USER: return TEXT("");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_JUMP: return TEXT("");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_FALL: return TEXT("");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_SKILL: return TEXT("");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_AI: return TEXT("");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_REACTION: return TEXT("");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_FEAR: return TEXT("");
		case CommonBehaviorResolverType::MOTION_CONTROLLER_TYPE_CHARM: return TEXT("");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_CHANGE_WEAPON_TYPE: return TEXT("");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_FIRST_WEIGHT_PENALTY: return TEXT("");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_SECOND_WEIGHT_PENALTY: return TEXT("");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_IN_SAFE_ZONE: return TEXT("");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_IN_SPAWN_ACTION: return TEXT("");
		case CommonBehaviorResolverType::USER_BEHAVIOR_STATUS_TYPE_IN_PERSONAL_TRADING: return TEXT("");
	}
	return FString();
}
inline FString CreatureActionTypeEnumToDisplayKey(CreatureActionType value)
{
	switch (value)
	{
		case CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_MOVE_ROOT_MOTION: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_MOVE_LOCATON: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_MOVE_FEAR: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_SKILL_BASIC: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_SKILL: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_DASH_SKILL: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_BUFF_SKILL: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_ITEM: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_SOCIAL: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_WARP: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_TAKE_DAMAGE: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_CHANGE_PAHSE: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_RECOVERY: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_CHANGE_TARGET: return TEXT("");
		case CreatureActionType::CREATURE_ACTION_TYPE_MOUNT_VEHICLE: return TEXT("");
	}
	return FString();
}
inline FString UserBehaviorStatusTypeEnumToDisplayKey(UserBehaviorStatusType value)
{
	switch (value)
	{
		case UserBehaviorStatusType::NONE: return TEXT("");
		case UserBehaviorStatusType::CHANGE_WEAPON_TYPE: return TEXT("");
		case UserBehaviorStatusType::FIRST_WEIGHT_PENALTY: return TEXT("");
		case UserBehaviorStatusType::SECOND_WEIGHT_PENALTY: return TEXT("");
		case UserBehaviorStatusType::IN_SAFE_ZONE: return TEXT("");
		case UserBehaviorStatusType::ARENA_WAITING: return TEXT("");
		case UserBehaviorStatusType::SPAWN_ACTION: return TEXT("");
		case UserBehaviorStatusType::PERSONAL_TRADING: return TEXT("");
	}
	return FString();
}
inline FString EReactionTypeEnumToDisplayKey(EReactionType value)
{
	switch (value)
	{
		case EReactionType::NONE: return TEXT("");
		case EReactionType::STUN: return TEXT("");
		case EReactionType::HIT: return TEXT("");
		case EReactionType::KNOCKDOWN: return TEXT("");
		case EReactionType::KNOCKBACK: return TEXT("");
		case EReactionType::MOVELOCATION: return TEXT("");
	}
	return FString();
}
inline FString NpcFunctionTypeEnumToDisplayKey(NpcFunctionType value)
{
	switch (value)
	{
		case NpcFunctionType::NONE: return TEXT("");
		case NpcFunctionType::SHOP: return TEXT("");
		case NpcFunctionType::REVIVAL: return TEXT("");
		case NpcFunctionType::PURCHASE: return TEXT("");
		case NpcFunctionType::QUEST_DIALOG: return TEXT("");
		case NpcFunctionType::SANDBAG: return TEXT("");
		case NpcFunctionType::DEPOT: return TEXT("");
		case NpcFunctionType::COMMUNITY: return TEXT("");
		case NpcFunctionType::EVENT_SHOP: return TEXT("");
		case NpcFunctionType::SHOP_EQUIP: return TEXT("");
		case NpcFunctionType::SHOP_SKILL_BOOK: return TEXT("");
		case NpcFunctionType::FIELD_BOSS: return TEXT("");
		case NpcFunctionType::TREASURE_MONSTER: return TEXT("");
		case NpcFunctionType::CAMP: return TEXT("");
		case NpcFunctionType::PARTY_DUNGEON_BOSS: return TEXT("");
		case NpcFunctionType::SPACE_TRACER: return TEXT("");
		case NpcFunctionType::GATEKEEPER: return TEXT("");
		case NpcFunctionType::DOOR: return TEXT("");
		case NpcFunctionType::QUEST_BOARD: return TEXT("");
		case NpcFunctionType::MISSION_WAVE: return TEXT("");
		case NpcFunctionType::EVENT_PRAYER: return TEXT("");
		case NpcFunctionType::SPAWN_MONSTER: return TEXT("");
		case NpcFunctionType::WORLD_BOSS: return TEXT("");
		case NpcFunctionType::INVADE_GUARDIAN: return TEXT("");
		case NpcFunctionType::INVADE_WAGON: return TEXT("");
		case NpcFunctionType::INVADE_BOSS: return TEXT("");
		case NpcFunctionType::MAGIC_OBJECT: return TEXT("");
		case NpcFunctionType::SPACE_CRACK_MONSTER: return TEXT("");
		case NpcFunctionType::GUARD: return TEXT("");
		case NpcFunctionType::INVADE_WAGON_GUARD: return TEXT("");
		case NpcFunctionType::INVADE_BANDITS: return TEXT("");
		case NpcFunctionType::CART: return TEXT("");
		case NpcFunctionType::SPACE_CRACK_SUPPLYMONSTER: return TEXT("");
		case NpcFunctionType::SPACE_CRACK_ELITE: return TEXT("");
		case NpcFunctionType::SPACE_CRACK_CONTAMINANT: return TEXT("");
		case NpcFunctionType::SPACE_CRACK_BOSS: return TEXT("");
		case NpcFunctionType::SPACE_CRACK_SPECIALBOSS: return TEXT("");
		case NpcFunctionType::SPACE_CRACK_HUMANDEPON: return TEXT("");
		case NpcFunctionType::MIMIC_NORMAL: return TEXT("");
		case NpcFunctionType::MIMIC_ELITE: return TEXT("");
		case NpcFunctionType::SANCTUM: return TEXT("");
		case NpcFunctionType::SANCTUM_NEXUS: return TEXT("");
		case NpcFunctionType::BATTLE_ARENA_BOSS: return TEXT("");
		case NpcFunctionType::BATTLE_ARENA_TOMBSTONE: return TEXT("");
		case NpcFunctionType::BATTLE_ARENA_GIMMICKMONSTER: return TEXT("");
		case NpcFunctionType::GUILD_BOSS: return TEXT("");
		case NpcFunctionType::GUILDARENA_SHOP: return TEXT("");
		case NpcFunctionType::AGIT_DUNGEON: return TEXT("");
		case NpcFunctionType::HP_RECOVER: return TEXT("");
		case NpcFunctionType::SECRECT_AREA_PORTAL: return TEXT("");
		case NpcFunctionType::MIDDLE_AREA_PORTAL: return TEXT("");
		case NpcFunctionType::FINAL_AREA_PORTAL: return TEXT("");
		case NpcFunctionType::AGIT_DUNGEON_RAID_BOSS: return TEXT("");
	}
	return FString();
}
inline FString CommonActionTypeEnumToDisplayKey(CommonActionType value)
{
	switch (value)
	{
		case CommonActionType::SPAWN: return TEXT("");
		case CommonActionType::DYING: return TEXT("");
		case CommonActionType::INPEACE: return TEXT("");
		case CommonActionType::TELEPORT: return TEXT("");
		case CommonActionType::SOCIAL: return TEXT("");
		case CommonActionType::WEAPON_CHANGE: return TEXT("");
		case CommonActionType::TOUCHPROP: return TEXT("");
		case CommonActionType::SPACECRACK: return TEXT("");
		case CommonActionType::INTERACTION_CAMP: return TEXT("");
		case CommonActionType::IDLE_SOCIAL: return TEXT("");
	}
	return FString();
}
inline FString CreatureSpawnTypeEnumToDisplayKey(CreatureSpawnType value)
{
	switch (value)
	{
		case CreatureSpawnType::NONE: return TEXT("");
		case CreatureSpawnType::NPC: return TEXT("");
		case CreatureSpawnType::PROP: return TEXT("");
	}
	return FString();
}
inline FString CreatureMoveTypeEnumToDisplayKey(CreatureMoveType value)
{
	switch (value)
	{
		case CreatureMoveType::NORMAL: return TEXT("");
		case CreatureMoveType::IMMOVED: return TEXT("");
		case CreatureMoveType::IMMOVED_NOT_ROTATED: return TEXT("");
	}
	return FString();
}
inline FString NpcSpawnGroupTypeEnumToDisplayKey(NpcSpawnGroupType value)
{
	switch (value)
	{
		case NpcSpawnGroupType::NONE: return TEXT("");
		case NpcSpawnGroupType::NORMAL: return TEXT("");
		case NpcSpawnGroupType::SUBORDINATION: return TEXT("");
		case NpcSpawnGroupType::RANDOM: return TEXT("");
		case NpcSpawnGroupType::RANDOM_RATE: return TEXT("");
	}
	return FString();
}
inline FString NpcSpecialTypeEnumToDisplayKey(NpcSpecialType value)
{
	switch (value)
	{
		case NpcSpecialType::NONE: return TEXT("");
		case NpcSpecialType::COLLIDE: return TEXT("");
	}
	return FString();
}
inline FString NpcSpecialStatusTypeEnumToDisplayKey(NpcSpecialStatusType value)
{
	switch (value)
	{
		case NpcSpecialStatusType::NONE: return TEXT("");
		case NpcSpecialStatusType::STOP: return TEXT("");
	}
	return FString();
}
inline FString NpcSpawnGroupMemberTypeEnumToDisplayKey(NpcSpawnGroupMemberType value)
{
	switch (value)
	{
		case NpcSpawnGroupMemberType::NONE: return TEXT("");
		case NpcSpawnGroupMemberType::LEADER: return TEXT("");
		case NpcSpawnGroupMemberType::MEMBER: return TEXT("");
		case NpcSpawnGroupMemberType::MASTER: return TEXT("");
		case NpcSpawnGroupMemberType::SUMMONEE: return TEXT("");
	}
	return FString();
}
inline FString RandomGroupSpawnTypeEnumToDisplayKey(RandomGroupSpawnType value)
{
	switch (value)
	{
		case RandomGroupSpawnType::NORMAL: return TEXT("");
		case RandomGroupSpawnType::WEIGHT: return TEXT("");
	}
	return FString();
}
inline FString MonsterElementEnumToDisplayKey(MonsterElement value)
{
	switch (value)
	{
		case MonsterElement::NONE: return TEXT("");
		case MonsterElement::FLAME: return TEXT("");
		case MonsterElement::FROST: return TEXT("");
		case MonsterElement::HOLY: return TEXT("");
		case MonsterElement::DARK: return TEXT("");
	}
	return FString();
}
inline FString GateKeeperContensEnumToDisplayKey(GateKeeperContens value)
{
	switch (value)
	{
		case GateKeeperContens::DUNGEON: return TEXT("");
	}
	return FString();
}
inline FString StatCorrectionTypeEnumToDisplayKey(StatCorrectionType value)
{
	switch (value)
	{
		case StatCorrectionType::GRADE: return TEXT("");
		case StatCorrectionType::ELEMENT: return TEXT("");
		case StatCorrectionType::RACE: return TEXT("");
		case StatCorrectionType::STRENGTH: return TEXT("");
		case StatCorrectionType::WEAKNESS: return TEXT("");
		case StatCorrectionType::CONTENTS: return TEXT("");
	}
	return FString();
}
inline FString StatCalcTypeEnumToDisplayKey(StatCalcType value)
{
	switch (value)
	{
		case StatCalcType::PLUS: return TEXT("");
		case StatCalcType::MULTIFLY: return TEXT("");
	}
	return FString();
}
inline FString NpcSpawnContentsEnumToDisplayKey(NpcSpawnContents value)
{
	switch (value)
	{
		case NpcSpawnContents::NONE: return TEXT("");
		case NpcSpawnContents::SPACE_CRACK: return TEXT("");
	}
	return FString();
}
inline FString PatrolTypeEnumToDisplayKey(PatrolType value)
{
	switch (value)
	{
		case PatrolType::NONE: return TEXT("");
		case PatrolType::LINE: return TEXT("");
		case PatrolType::CIRCLE: return TEXT("");
		case PatrolType::SPOT: return TEXT("");
	}
	return FString();
}
inline FString UserSocialEmotionEnumToDisplayKey(UserSocialEmotion value)
{
	switch (value)
	{
		case UserSocialEmotion::NONE: return TEXT("");
		case UserSocialEmotion::LAUGH: return TEXT("");
		case UserSocialEmotion::SADNESS: return TEXT("");
		case UserSocialEmotion::DENY: return TEXT("");
		case UserSocialEmotion::GREETING: return TEXT("");
		case UserSocialEmotion::FAVOR: return TEXT("");
		case UserSocialEmotion::AGREE: return TEXT("");
		case UserSocialEmotion::BRAG: return TEXT("");
		case UserSocialEmotion::FIGHTING: return TEXT("");
		case UserSocialEmotion::HIGHMOTIVATE: return TEXT("");
		case UserSocialEmotion::HEART: return TEXT("");
		case UserSocialEmotion::DANCE: return TEXT("");
		case UserSocialEmotion::SURPRISE: return TEXT("");
		case UserSocialEmotion::IDLE_EMOTION: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidCreatureGenderType(CreatureGenderType value) noexcept
{
	return (value >= CreatureGenderType::ALL && value < CreatureGenderType::MAX);
}

inline bool IsValidCreatureRaceType(CreatureRaceType value) noexcept
{
	return (value >= CreatureRaceType::NONE && value < CreatureRaceType::MAX);
}

inline bool IsValidCreatureWeaponType(CreatureWeaponType value) noexcept
{
	return (value >= CreatureWeaponType::NONE && value < CreatureWeaponType::MAX);
}

inline bool IsValidCreatureRankType(CreatureRankType value) noexcept
{
	return (value >= CreatureRankType::NONE && value < CreatureRankType::MAX);
}

inline bool IsValidCommonBehaviorResolverType(CommonBehaviorResolverType value) noexcept
{
	return (value >= CommonBehaviorResolverType::NONE && value < CommonBehaviorResolverType::MAX);
}

inline bool IsValidCreatureActionType(CreatureActionType value) noexcept
{
	return (value >= CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT && value < CreatureActionType::MAX);
}

inline bool IsValidUserBehaviorStatusType(UserBehaviorStatusType value) noexcept
{
	return (value >= UserBehaviorStatusType::NONE && value < UserBehaviorStatusType::MAX);
}

inline bool IsValidEReactionType(EReactionType value) noexcept
{
	return (value >= EReactionType::NONE && value < EReactionType::MAX);
}

inline bool IsValidNpcFunctionType(NpcFunctionType value) noexcept
{
	return (value >= NpcFunctionType::NONE && value < NpcFunctionType::MAX);
}

inline bool IsValidCommonActionType(CommonActionType value) noexcept
{
	return (value >= CommonActionType::SPAWN && value < CommonActionType::MAX);
}

inline bool IsValidCreatureSpawnType(CreatureSpawnType value) noexcept
{
	return (value >= CreatureSpawnType::NONE && value < CreatureSpawnType::MAX);
}

inline bool IsValidCreatureMoveType(CreatureMoveType value) noexcept
{
	return (value >= CreatureMoveType::NORMAL && value < CreatureMoveType::MAX);
}

inline bool IsValidNpcSpawnGroupType(NpcSpawnGroupType value) noexcept
{
	return (value >= NpcSpawnGroupType::NONE && value < NpcSpawnGroupType::MAX);
}

inline bool IsValidNpcSpecialType(NpcSpecialType value) noexcept
{
	return (value >= NpcSpecialType::NONE && value < NpcSpecialType::MAX);
}

inline bool IsValidNpcSpecialStatusType(NpcSpecialStatusType value) noexcept
{
	return (value >= NpcSpecialStatusType::NONE && value < NpcSpecialStatusType::MAX);
}

inline bool IsValidNpcSpawnGroupMemberType(NpcSpawnGroupMemberType value) noexcept
{
	return (value >= NpcSpawnGroupMemberType::NONE && value < NpcSpawnGroupMemberType::MAX);
}

inline bool IsValidRandomGroupSpawnType(RandomGroupSpawnType value) noexcept
{
	return (value >= RandomGroupSpawnType::NORMAL && value < RandomGroupSpawnType::MAX);
}

inline bool IsValidMonsterElement(MonsterElement value) noexcept
{
	return (value >= MonsterElement::NONE && value < MonsterElement::MAX);
}

inline bool IsValidGateKeeperContens(GateKeeperContens value) noexcept
{
	return (value >= GateKeeperContens::DUNGEON && value < GateKeeperContens::MAX);
}

inline bool IsValidStatCorrectionType(StatCorrectionType value) noexcept
{
	return (value >= StatCorrectionType::GRADE && value < StatCorrectionType::MAX);
}

inline bool IsValidStatCalcType(StatCalcType value) noexcept
{
	return (value >= StatCalcType::PLUS && value < StatCalcType::MAX);
}

inline bool IsValidNpcSpawnContents(NpcSpawnContents value) noexcept
{
	return (value >= NpcSpawnContents::NONE && value < NpcSpawnContents::MAX);
}

inline bool IsValidPatrolType(PatrolType value) noexcept
{
	return (value >= PatrolType::NONE && value < PatrolType::MAX);
}

inline bool IsValidUserSocialEmotion(UserSocialEmotion value) noexcept
{
	return (value >= UserSocialEmotion::NONE && value < UserSocialEmotion::MAX);
}


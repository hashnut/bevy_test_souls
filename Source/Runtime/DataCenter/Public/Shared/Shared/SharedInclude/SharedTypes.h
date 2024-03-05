#pragma once

#ifndef _SERVER
#include "SharedEnums_client.h"
#include "SharedDefine.h"
#include "SharedPrimitiveTypes.h"
#else
#include <wchar.h>
#include "SharedEnums_server.h"
#include "SharedDefine.h"
#include "SharedPrimitiveTypes.h"
#include "ServerEngine/Utils/DateTime.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PrimitiveType containers
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contents primitive types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 플래닛 아이디
using PlanetId = uint16;
constexpr PlanetId INVALID_PLANET_ID = 0;
constexpr PlanetId MAX_PLANET_ID = 64;

// 월드 아이디
using WorldId = uint16;
constexpr WorldId INVALID_WORLD_ID = 0;
constexpr WorldId MAX_WORLD_ID = 1024;

using WorldGroupId = uint16;
constexpr WorldGroupId INVALID_WORLDGROUP_ID = 0;




// Continent 아이디
using ContinentId = int;
constexpr ContinentId INVALID_CONTINENT_ID = -1;

// Area 아이디
using AreaId = int;
constexpr AreaId INVALID_AREA_ID = -1;

// Map 아이디
using MapId = int;
constexpr MapId INVALID_MAP_ID = -1;

constexpr uint32 MAP_LOCK_IDX_COUNT = 1'000;

// Spot 아이디
using SpotId = uint32;	// DataSheet json 상의 spotIndex
constexpr SpotId INVALID_SPOT_ID = 0;

// DungeonGroup 아이디 (MapType::PUBLIC_DUNGEON인 맵은, DungeonGroup을 참조한다)
using DungeonGroupId = int;
constexpr DungeonGroupId INVALID_DUNGEON_GROUP_ID = -1;

// DungeonListIdx (MapType::PUBLIC_DUNGEON인 맵은, DungeonGroup의 DungeonList중 하나와 매칭된다)
using DungeonListIdx = int;
constexpr DungeonListIdx INVALID_DUNGEON_LIST_IDX = -1;

// Warp 아이디
using WarpId = uint32;
constexpr WarpId INVALID_WARP_ID = 0;

// Channel 아이디
using ChannelId = int;
constexpr ChannelId INVALID_CHANNEL_ID = -1;
constexpr ChannelId PERSISTANT_CHANNEL_ID = 0;
constexpr ChannelId DEFAULT_CHANNEL_ID = 1;   // 채널 ID 시작 

// Territory 아이디
using TerritoryId = int;
constexpr TerritoryId INVALID_TERRITORY_ID = -1;

// Fence 아이디
using FenceId = int;
constexpr FenceId INVALID_FENCE_ID = -1;

// AuthTicket (인증서버가 발급하는 임시토큰, 플랫폼 로그인 토큰과 다르다)
using AuthTicket = uint64;
constexpr AuthTicket INVALID_AUTH_TICKET = 0;

// ReconectTicket (Login/Reconnect 성공할 때마다 다음 재연결을 위해 새로 발금됨)
using ReconnectTicket = uint64;
constexpr ReconnectTicket INVALID_RECONNECT_TICKET = 0;

// Vid (하이브 플랫폼 플레이어Id - 인증v4의 PlayerId 와 동일)
using Vid = uint64;
constexpr Vid INVALID_VID = 0;

using Did = uint64;
constexpr Did INVALID_DID = 0;

// AccountDB ID
using AccountDBId = uint64;

constexpr AccountDBId INVALID_ACCOUNT_DB_ID = 0;

// 유저(캐릭터)의 DB ID
using UserDBId = uint64;
constexpr UserDBId INVALID_USER_DB_ID = 0;

// Actor ID
using ActorId = uint32;
constexpr ActorId INVALID_ACTOR_ID = 0;

// Creature 공통 속성 정보 ID
using CreatureCommonInfoId = uint32;
constexpr CreatureCommonInfoId INVALID_CREATURE_COMMON_INFO_ID = 0;

// Creature ID
using CreatureId = uint32;
constexpr CreatureId INVALID_CREATURE_ID = 0;


// User Shape(Model) ID
using UserShapeId = uint32;
constexpr UserShapeId INVALID_USER_SHAPE_ID = 0;

// 레벨 수치
using Level = uint16;
constexpr Level INVALID_LEVEL = 0;
constexpr Level MIN_LEVEL = 1;
constexpr Level MAX_LEVEL = 100;	// [M3 - 2020.12.14] 레벨 확장 (30 -> 40), [M5 - 2021.07.28 80으로 확장(손한석)], [B1 - 2023.04.12 100으로 확장(원지호님)] --- UserLevelData.json 에 가데이터라도 넣어주어야 함.

constexpr Level MIN_FAIRY_LEVEL = 0;	// 페어리 최소 레벨
constexpr Level MAX_FAIRY_LEVEL = 5;	// 페어리 최대 레벨

// 경험치
using Exp = uint64;
constexpr Exp MAX_EXP = 2'000'000'000'000'000;	// 2000조

// 재화
using Currency = uint64;
constexpr Currency MIN_CURRENCY = 0;

// 재화 변화량
using CurrencyDelta = uint64;
using CurrencyDeltaSigned = int64;

// 머니 (유저에 귀속되는 재화)
using Money = Currency;
constexpr Money MIN_MONEY = 0;
constexpr Money MAX_MONEY = 20'000'000'000'000;	// 20조

// 다이아 (실제 캐쉬)
using Gem = Currency;
constexpr Gem MAX_GEM = 100'000'000'000;	// 100억
constexpr Gem MAX_FREE_GEM = 30'000'000;	// 3000만원


// SeasonId 시즌아이디
using SeasonId = int;
constexpr SeasonId INVALID_SEASON_ID = -1;


// GuildWarPoint 
using GuildWarPoint = uint32;

// GuildWarPoint 
using GuildWarMatchPoint = int32;

// 타임 티켓 
using TimeTicket = Money;
using TimeTicketReceiveTime = int64_t;
using TimeTicketResetHour = int8_t;

// 튜토리얼
using TutorialId = int32;
constexpr TutorialId INVALID_TUTORIAL_ID = -1;

// 우정포인트
using SocialPoint = uint32;

// 커스터마이징
constexpr uint8 DEFAULT_CUSTOM_VALUE = 50;
constexpr uint8 DEFAULT_CUSTOM_COLOR_ID = 0;

using CustomPartId = uint32;
constexpr CustomPartId INVALID_CUSTOM_PART_ID = 0;

using CustomTattooId = uint32;
constexpr CustomTattooId INVALID_CUSTOM_TATTOO_ID = 0;

// 스탯 수치
using StatValue = int32;
constexpr StatValue MIN_STAT_VALUE = 0;
constexpr StatValue MAX_STAT_VALUE = 1'000'000; // 100만
constexpr StatValue MAX_USER_STAT_POINT = UINT16_MAX; // 유저 추가 스탯 포인트 최대값

using PresetId = uint8;
constexpr PresetId INVALID_PRESET_ID = 0;
constexpr PresetId MIN_PRESET_ID = 1;
constexpr PresetId MAX_PRESET_ID = 3;
constexpr PresetId MAX_PRESET_COUNT = 3;

using PresetValue = uint8;
constexpr PresetValue INVALID_PRESET_VALUE = 0;

using SocketId = uint8;
constexpr SocketId INVALID_SOCKET_ID = 0;
constexpr SocketId MIN_SOCKET_ID = 1;
constexpr SocketId MAX_SOCKET_ID = 3;
constexpr SocketId MAX_SOCKET_COUNT = 3;

// 스킬 ID
using SkillId = uint32;
constexpr SkillId INVALID_SKILL_ID = 0;

// 스킬 그룹 ID
using SkillGroupId = uint32;
constexpr SkillGroupId INVALID_SKILL_GROUP_ID = 0;

// 스킬 효과 ID
using SkillNotifyId = uint32;
constexpr SkillNotifyId INVALID_SKILL_NOTIFY_ID = 0;

// 스킬 사용 조건 ID
using SkillExecuteConditionId = uint32;
constexpr SkillExecuteConditionId INVALID_SKILL_EXECUTE_CONDITION_ID = 0;

// 스킬 레벨 수치
using SkillLevel = uint8;
constexpr SkillLevel INVALID_SKILL_LEVEL = 0;
constexpr SkillLevel MIN_SKILL_LEVEL = 1;
constexpr SkillLevel MAX_SKILL_LEVEL = 15;

// Skill Upgrade Id
using SkillUpgradeId = uint32;
constexpr SkillUpgradeId INVALID_SKILL_UPGRADE_ID = 0;

// Skill Upgrade Step
using SkillUpgradeStep = int8;
constexpr SkillUpgradeStep INVALID_SKILL_UPGRADE_STEP = -1;
constexpr SkillUpgradeStep ROOT_SKILL_UPGRADE_STEP = 0;
//constexpr SkillUpgradeStep MAX_SKILL_UPGRADE_STEP = 5;	불필요한것 같아서 일단 주석

// Skill Upgrade Passivity count
using SkillUpgradeStepPassivityCount = int8;
constexpr SkillUpgradeStepPassivityCount MIN_SKILL_UPGRADE_STEP_PASSIVITY_COUNT = 0;

// 스킬 슬롯 ID
using SkillSlotId = uint8;
constexpr SkillSlotId INVALID_SKILL_SLOT_ID = 0;
constexpr SkillSlotId MAX_ACTIVE_SKILL_SLOT_ID = 7;		// 즉발형 스킬의 등록 가능한 최대 슬롯 Id
constexpr SkillSlotId MAX_BUFF_SKILL_SLOT_ID = MAX_ACTIVE_SKILL_SLOT_ID + 5;		// 지속형 스킬의 등록 가능한 최대 슬롯 Id
constexpr SkillSlotId MAX_COSTUME_SKILL_SLOT_ID = MAX_BUFF_SKILL_SLOT_ID + 1;	// 코스튬 스킬의 등록 가능한 최대 슬롯 Id

// 스킬 사용 Default 값
constexpr bool DEFAULT_SKILL_AUTO_USE = false;

// 스킬 & 아이템 쿨다운 그룹 ID
using CooldownGroupId = int;
constexpr CooldownGroupId INVALID_COOLDOWN_GROUP_ID = 0;

// 스킬 강화, 업그레이드 비용 테이블 ID : Cost
using SkillUpCostId = uint32;
constexpr SkillUpCostId INVALID_SKILL_UP_COST_ID = 0;

// 아이템 DB ID
using ItemDBId = uint64;
constexpr ItemDBId INVALID_ITEM_DB_ID = 0;

// 아이템 Datasheet ID
using ItemId = uint32;
constexpr ItemId INVALID_ITEM_ID = 0;

// 아이템 상세 Datasheet ID
using ItemDetailId = uint32;
constexpr ItemDetailId INVALID_ITEM_DETAIL_ID = 0;

// 아이템 세트 ID
using ItemSetId = uint32;
constexpr ItemSetId INVALID_ITEM_SET_ID = 0;

// 아이템 쿨다운 그룹 ID - 동일 그룹 ID의 아이템은 쿨다운타임을 공유한다
using ItemCooldownGroupId = uint32;
constexpr ItemCooldownGroupId INVALID_ITEM_COOLDOWN_GROUP_ID = 0;

// 아이템 그룹 ID
using ItemGroupId = ItemId;
constexpr ItemGroupId INVALID_ITEM_GROUP_ID = 0;

// 아이템 수량
using ItemAmount = int64;
constexpr ItemAmount MIN_ITEM_AMOUNT = 0;
constexpr ItemAmount MAX_ITEM_AMOUNT = 9'999'999'999;
constexpr ItemAmount MAX_FAIRY_STACK_AMOUNT = 9'999;					// 페어리 최대 중첩 개수
constexpr ItemAmount MAX_COSTUME_STACK_AMOUNT = MAX_FAIRY_STACK_AMOUNT;	// 코스튬 최대 중첩 개수

// 아이템 티어
using ItemTier = uint32;

using ItemSubTypeGroupId = uint32;
constexpr ItemSubTypeGroupId INVALID_ITEM_SUB_TYPE_GROUP_ID = 0;

//인벤토리 슬롯
constexpr ItemAmount DEFAULT_INVENTORY_SLOT = 100;	// 기본 인벤토리 슬롯 수
constexpr ItemAmount MAX_INVENTORY_SLOT = 9999;		// 최대 인벤토리 슬롯 수
constexpr ItemAmount INVENTORY_EXTEND_SLOT = 10;	// 인벤토리 확장시 한번에 늘어날 슬롯 수

//창고 슬롯
constexpr ItemAmount DEFAULT_DEPOT_SLOT = 60;		// 기본 창고 슬롯 수
constexpr ItemAmount MAX_DEPOT_SLOT = 9999;			// 최대 창고 슬롯 수

using SlotExtendNumber = uint8;		//창고,인벤 슬롯 확장 횟수

constexpr SlotExtendNumber INVALID_SLOTEXTEND_NUMBER = 0;

// 장비 아이템 슬롯 인덱스
using ItemEquipSlotIndex = uint8;

// 인벤토리 순서 값
using InventorySortValue = uint16;
constexpr InventorySortValue MAX_INVENTORY_SORT_VALUE = MAX_INVENTORY_SLOT;

// 장비 내구도
using EnchantShield = uint8;
constexpr EnchantShield INVALID_ENCHANT_SHIELD = UINT8_MAX;

// 강화 보너스 인덱스
using EnchantBonusIndex = uint8;
constexpr EnchantBonusIndex INVALID_ENCHANT_BONUS_INDEX = UINT8_MAX;

constexpr EnchantShield UNCONFIRMED_ENCHANT_SHIELD = 255;				// 확정되지 않은 내구도
constexpr EnchantBonusIndex UNCONFIRMED_ENCHANT_BONUS_INDEX = 255;		// 확정되지 않은 강화 보너스 인덱스

constexpr ItemAmount MAX_ITEM_ENCHANT_MULTI_SLOT = 7 * 3; // 다중 강화 최대 갯수

// 소모성 아이템 사용 효과 Id
using ItemEffectId = uint32;
constexpr ItemEffectId INVALID_ITEM_EFFECT_ID = 0;

// 발동 효과 Id
using CommonEffectId = uint32;
constexpr CommonEffectId INVALID_COMMON_EFFECT_ID = 0;

// 임의사용 데이타값 VALUE
using ParamValue = int64;

// 타입별 임의사용 데이타값 VALUE
using TypeValue = int64;


// 무게
using Weight = StatValue;
constexpr Weight MIN_WEIGHT_VALUE = 0;
constexpr Weight MAX_WEIGHT_VALUE = INT32_MAX;

// 보상 ID
using RewardId = int64;
constexpr RewardId INVALID_REWARD_ID = 0;

// 보상상자 ID
using RewardBoxId = int64;
constexpr RewardBoxId INVALID_REWARD_BOX_ID = 0;

// 아이템꾸러미 인덱스
using ItemBagIndex = int32;
constexpr ItemBagIndex INVALID_ITEM_BAG_INDEX = -1;
constexpr ItemBagIndex MAX_ITEM_BAG_INDEX = 1500;

// 탈것 ID
using VehicleId = uint32;
constexpr VehicleId INVALID_VEHICLE_ID = 0;

// 우편
using MailDBId = uint64;
constexpr MailDBId INVALID_MAIL_DB_ID = 0;
using MailTypeId = uint32;
constexpr MailTypeId INVALID_MAIL_TYPE_ID = 0;
using MailStringId = uint32;
constexpr MailStringId INVALID_MAIL_STRING_ID = 0;
// 우편 첨부물 아이디
using MailAttachmentId = ItemId;						//CurrencyType 또는 ItemId 둘 중 큰 타입 사용
constexpr MailAttachmentId INVALID_MAIL_ATTACHMENT_ID = 0;
// 우편 첨부물 수량
using MailAttachmentAmount = ItemAmount;				//CurrencyDelta 또는 ItemAmount 둘 중 큰 타입 사용

// 스폰
using SpawnGroupId = uint32;
constexpr SpawnGroupId INVALID_SPAWNGROUP_ID = 0;

using SpawnId = int64;
constexpr SpawnId INVALID_SPAWN_ID = 0;


using MapEventSpawnGroupId = int64;
constexpr MapEventSpawnGroupId INVALID_MAP_EVENT_SPAWN_GROUP_ID = 0;

constexpr int INVALID_RESPAWN_INTERVAL = 0;

using RandomGroupSpawnId = int64;
constexpr RandomGroupSpawnId INVALID_RANDOMGROUPSPAWN_ID = 0;

// Npc 외형 ID
using NpcShapeId = int;
constexpr NpcShapeId INVALID_NPC_SHAPE_ID = 0;

// Npc 밸런스 ID
using NpcBalanceId = uint32;
constexpr NpcBalanceId INVALID_NPC_BALANCE_ID = 0;

// Npc Faction ID
using NpcFactionId = uint32;
constexpr NpcFactionId INVALID_NPC_FACTION_ID = 0;

// 프로젝타일 아이디
using ProjectileId = uint32;
constexpr ProjectileId INVALID_PROJECTILE_ID = 0;

// 유효기간 아이디
using ExpiredTimeId = uint32;
constexpr ExpiredTimeId INVALID_EXPIRED_TIME_ID = 0;

// 대기열 번호표
using WaitingNumberTicket = uint16;
constexpr WaitingNumberTicket INVALID_WAITING_NUMBER_TICKET = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 퀵슬롯 관련 types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 퀵슬롯 아이디
using QuickSlotId = uint8;
constexpr QuickSlotId INVALID_QUICK_SLOT_ID = 0;

// 퀵슬롯 한 페이지당 사이즈
constexpr uint8 MAX_QUICK_SLOT_PAGE_SIZE = 5;

// 퀵슬롯 최대 사이즈
// 5 * 6
constexpr uint8 MAX_QUICK_SLOT_SIZE = MAX_QUICK_SLOT_PAGE_SIZE * 12;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 리액션 관련 types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 리액션 테이블 아이디
using ReactionTableId = uint32;
constexpr ReactionTableId INVALID_REACTION_TABLE_ID = 0;

// 리액션 아이디
using ReactionItemId = uint32;
constexpr ReactionItemId INVALID_REACTION_ITEM_ID = 0;

// 리액션 템플릿 아이디
using ReactionId = uint32;
constexpr ReactionId INVALID_REACTION_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 일반 액션 관련 types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 커먼 액션 리스트 ID
using CommonActionListId = uint32;
constexpr CommonActionListId INVALID_COMMON_ACTION_LIST_ID = 0;

// 커먼 액션 ID
using CommonActionId = uint32;
constexpr CommonActionId INVALID_COMMON_ACTION_ID = 0;

using ControlFactorId = uint32;
constexpr ControlFactorId INVALID_CONTROL_FACTOR_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 이상상태 관련 user types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 이상상태 ID
using AbnormalityId = uint32;
constexpr AbnormalityId INVALID_ABNORMALITY_ID = 0;

#ifdef _SERVER
using AbnormalityIdList = std::xlist<AbnormalityId>;
#endif

using AbnormalityCategoryId = uint32;
constexpr AbnormalityCategoryId INVALID_ABNORMALITY_CATEGORY_ID = 0;

// 이상상태 중첩 횟수
using AbnormalityStackCount = uint8;
constexpr AbnormalityStackCount MIN_ABNORMALITY_STACK = 0;
constexpr AbnormalityStackCount MAX_ABNORMALITY_STACK = 10;
constexpr AbnormalityStackCount ABNORMALITY_STACK_VALUE = 1;	// Stack 될 때 조절되는 갯수

// 이상상태 객체의 고유 인스턴스 ID (객체당 유니크한 값. 글로벌로 유니크하지는 않음)
// 매 초당 100씩 추가되도 1년 정도 유지 가능
using AbnormalityInstanceId = uint32;
constexpr AbnormalityInstanceId INVALID_ABNORMALITY_INSTANCE_ID = 0;

// 이상상태 그룹 ID
using AbnormalityGroupId = int32;
constexpr AbnormalityGroupId INVALID_ABNORMALITY_GROUP_ID = 0;
#ifdef _SERVER
using AbnormalityGroupIdList = std::xlist<AbnormalityGroupId>;
#endif

// 이상상태 효과 아이디
using AbnormalityEffectId = uint32;
constexpr AbnormalityEffectId INVALID_ABNORMALITY_EFFECT_ID = 0;

// 이상상태 효과 Level
using AbnormalityEffectLevel = uint8;
constexpr AbnormalityEffectLevel INVALID_ABNORMALITY_EFFECT_LEVEL = 0;
constexpr AbnormalityEffectLevel MIN_ABNORMALITY_EFFECT_LEVEL = MIN_SKILL_LEVEL;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 패시비티 관련 user types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 패시비티 아이디
using PassivityId = uint32;
constexpr PassivityId INVALID_PASSIVITY_ID = 0;

// 패시비티 아이디
using PassivityEffectId = uint32;
constexpr PassivityEffectId INVALID_PASSIVITY_EFFECT_ID = 0;

// 패시비티 소유개체 아이디 enum PassivityOwnerType 에 따라서 다르게 해석된다.
using PassivityOwnerId = uint32;
const PassivityOwnerId INVALID_PASSIVITY_OWNER_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AI 관련 types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// AI ID  
using AiId = uint32;
constexpr AiId INVALID_AI_ID = 0;

// AI 액션 ID
using AiActionId = uint32;
constexpr AiActionId INVALID_AI_ACTION_ID = 0;

// AI Phase ID
using AiPhaseId = uint32;
constexpr AiPhaseId INVALID_AI_PHASE_ID = 0;

// AI 조건행동의 조건 ID
using AiConditionId = uint32;
constexpr AiConditionId INVALID_AI_CONDITION_ID = 0;

// 어그로 수치, Stat 기반으로 평가됨으로 StatValue 와 동일한 자료형입니다.
using AggroValue = long;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 변신 관련 types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 변신 ID
using TransformId = uint32;
constexpr TransformId INVALID_TRANSFORM_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Quest 관련 types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 퀘스트 챕터 ID
using ChapterId = uint32;
constexpr ChapterId INVALID_CHAPTER_ID = 0;

// 퀘스트 스토리 아이디
using StoryId = uint32;
constexpr StoryId INVALID_STORY_ID = 0;

// 퀘스트 아이디
using QuestId = int64;
constexpr QuestId INVALID_QUEST_ID = 0;

// 퀘스트 오브젝티브 아이디
using QuestObjectiveId = int64;
constexpr QuestObjectiveId INVALID_QUEST_OBJECTIVE_ID = 0;

// 퀘스트 오브젝티브 벨류
using QuestObjectiveValue = int32; // 클라에서 통일성있게 int32로 변경.
constexpr QuestObjectiveValue MIN_QUEST_OBJECTIVE_VALUE = 0;

// 퀘스트 게시판
using QuestBoardGroupId = uint64;
constexpr QuestBoardGroupId INVALID_QUEST_BOARD_GROUP_ID = 0;

// 유저별 퀘스트 고유 Index
using QuestIndex = uint64;
constexpr QuestIndex INVALID_QUEST_INDEX = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EventAction 관련 types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EventActionGroupId
using EventActionGroupId = uint32;
constexpr EventActionGroupId INVALID_EVENT_ACTION_GROUP_ID = 0;

//EvnectActionGroup 내의 Index
using EventActionIndex = int8;
constexpr EventActionIndex FIRST_EVENT_ACTION_INDEX = 0;	// Index의 처음시작.
constexpr EventActionIndex INVALID_EVENT_ACTION_INDEX = -1;	// 유효하지 않은 Index


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Contents Event 관리 시스템
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using ContentsEventRowId = uint64;			//DB에서 중복되지 않는 키
using ContentsEventGroupId = uint32;		//ContentsEventCommon테이블용 id
using ContentsEventId = uint32;
constexpr ContentsEventRowId INVALID_CONTENTS_EVENT_ROW_ID = ContentsEventRowId(-1);
constexpr ContentsEventId INVALID_CONTENTS_EVENT_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bookmark 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using BookmarkId = uint64;
constexpr BookmarkId INVALID_BOOKMARK_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Party
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using PartyId = uint64;
constexpr PartyId INVALID_PARTY_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fairy
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 페어리 ID
using FairyId = uint32;
constexpr FairyId INVALID_FAIRY_ID = 0;

// 페어리 레벨 ID
using FairyLevelId = uint32;
constexpr FairyLevelId INVALID_FAIRY_LEVEL_ID = 0;

// 페어리 소환 시 사용할 데이터 ID
using FairySummonId = uint32;
constexpr FairySummonId INVALID_FAIRY_SUMMON_ID = 0;

// 페어리 소환 대기 ID
using FairyConfirmId = uint64;
constexpr FairyConfirmId INVALID_FAIRY_CONFIRM_ID = 0;

// 페어리 소환 대기 관련 정보 ID
using FairySummonConfirmId = uint64;
constexpr FairySummonConfirmId INVALID_FAIRY_SUMMON_CONFIRM_ID = 0;

// 페어리 등장 지분 데이터 Id
using FairyProbabilityId = uint32;
constexpr FairyProbabilityId INVALID_FAIRY_PROBABILITY_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Costume
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 코스튬 ID
using CostumeId = uint32;
constexpr CostumeId INVALID_COSTUME_ID = 0;

// 코스튬 소환 대기 ID
using CostumeConfirmId = uint64;
constexpr CostumeConfirmId INVALID_COSTUME_CONFIRM_ID = 0;

// 코스튬 소환 시 사용할 데이터 ID
using CostumeSummonId = uint32;
constexpr CostumeSummonId INVALID_COSTUME_SUMMON_ID = 0;

// 코스튬 소환 대기 ID
using CostumeSummonConfirmId = uint64;
constexpr CostumeSummonConfirmId INVALID_COSTUME_SUMMON_CONFIRM_ID = 0;

// 코스튬 등장 지분 데이터 Id
using CostumeProbabilityId = uint32;
constexpr CostumeProbabilityId INVALID_COSTUME_PROBABILITY_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Collection
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 컬렉션 ID
using CollectionId = uint32;
constexpr CollectionId INVALID_COLLECTION_ID = 0;

// 컬렉션 하위 조건 ID
using CollectionConditionId = uint32;
constexpr CollectionConditionId INVALID_COLLECTION_CONDITION_ID = 0;

// 컬렉션 랜덤 데이터 추출용 ID
using CollectionRandomDataId = int64;
constexpr CollectionRandomDataId INVALID_COLLECTION_RANDOM_DATA_ID = 0;

// 길드 컬렉션 최대 조건 갯수
constexpr uint8 MAX_GUILD_ITEM_COLLECTION_CONDITION_COUNT = 6;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Npc Shop
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Npc Shop Group ID
using NpcShopGroupId = uint32;
constexpr NpcShopGroupId INVALID_NPCSHOPGROUP_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// chat types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 채팅 신고타입
using ReportBadChatType = uint8;
//일일 신고 횟수 
constexpr uint8 MAX_REPORT_BADCHAT_CNT = 5;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//기여도
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using ContributePoint = uint64;		// 기여도
constexpr ContributePoint MIN_CONTRIBUTE_POINT = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Guild(기사단)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GuildDB ID
using GuildDBId = uint64;
constexpr GuildDBId INVALID_GUILD_DB_ID = 0;
//기사단 문장 ID
using GuildEmblemId = uint8;	//데이터형은 아직 미논의됨
constexpr GuildEmblemId INVALID_GUILD_EMBLEM_ID = 0;

//기사단 맴버 인원수 최대값 일단 크게 잡자.
constexpr uint8 MAX_GUILD_MEMBERS = 255;

//기사단별 가입 지원 자 수 제한 
constexpr uint8 MAX_GUILD_JOIN_APPLICANTS = 50;

//기사단 권한별 등급
using GuildRightsLevel = uint32;
using GuildLevel = uint16;
constexpr GuildLevel INVALID_GUILD_LEVEL = 0;
using GuildContribution = uint64;	//기사단 기여도

// 기사단 건물 
using GuildBuildingDBId = uint64;
using GuildBuildingId = uint32;
constexpr GuildBuildingId INVALID_GUILD_BUILDING_ID = 0;
using GuildBuildingLevel = uint16;
constexpr GuildBuildingLevel INVALID_GUILD_BUILDING_LEVEL = 1;

// 기사단 버프 
using GuildAbnormalityDBId = uint64;
using GuildAbnormalityId = uint32;
constexpr GuildAbnormalityId INVALID_GUILD_ABNORMALITY_ID = 0;





// 세율(만분율)
using TaxRate = uint16;

// 백분율
using TaxPercent = uint16;

//// 기사단 능력치 값 
//using GuildStatTypeId = uint16;
//using GuildStatValue = int64;

// 기사단 채팅
using GuildChatDBId = uint64;
constexpr GuildChatDBId INVALID_GUILD_CHAT_ID = 0;

// 기사단 그룹 채팅 룸 아이디 
using GuildGroupChatRoomId = uint64;
constexpr GuildGroupChatRoomId INVALID_GUILD_GROUP_CHAT_ROOM_ID = 0;
//using GuildGroupChatRoomIdSet = std::xset<GuildGroupChatRoomId>;	//
//using GuildDBIdByroupChatRoomIdMap = std::xset<GuildDBId , GuildGroupChatRoomIdSet>;	//길드별 가입된 그룹 채팅ID
// 기사단 그룹 채팅 
using GuildGroupChatDBId = uint64;
constexpr GuildGroupChatDBId INVALID_GUILD_GROUP_CHAT_ID = 0;

//기사단 난이도
using GuildDunGeonLevel = uint16;
constexpr GuildDunGeonLevel INVALID_GUILD_DUNGEON_LEVEL = 0;

//기사단 던전 이용권
using GuildDunGeonTicket = uint8;

//기사단 열성 포인트
using GuildZealPoint = uint32;
//기사단 열성 포인트 변동
using GuildZealPointDelta = int32;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 부활 관련  types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using ResurrectIdx = uint32;    // 부활 인덱스
using ResurrectId = uint32;   // 부활 정보

constexpr ResurrectId INVALID_RESURRECT_ID = 0;

using PaneltyId = uint32;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 사망 패널티 관련  types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using DeathId = uint64;
using DeathPenaltyId = uint64;
using ExpRevoceryCount = int16;

constexpr DeathPenaltyId INVALID_DEATH_PENALTY_ID = 0;

using DeathPenaltyFreeCount = Currency;		// 일일 무료 복구 횟수

constexpr int MAX_DEATH_PENALTY_ITEMS = 3;

constexpr DeathPenaltyFreeCount MAX_DEATH_PENALTY_FREE = 50;	// 일일 최대 50

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Arcas - 경험치 버프 (feat.L2M아인하사드)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using ArcasTemplateId = uint32;							// TemplateId
constexpr ArcasTemplateId INVALID_ARCAS_ID = 0;

using ArcasPoint = float;										// 유저가 보유하고 있는 Point
constexpr ArcasPoint MIN_ARCAS_POINT = 0.f;						// 초기값(최소)
//constexpr ArcasPoint MAX_ARCAS_POINT = 50000;	// 최대 값, DataSheet(arcas.json 의  maxArcasPoint값 + BONUS_BLESS_MAX_INCREASE_VALUE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PvpAlignment - 카오틱 - PVP성향 시스템
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using PVPAlignmentTemplateId = uint32;
constexpr PVPAlignmentTemplateId INVALID_PVP_ALIGNMENT_ID = 0;

using PVPAlignmentPoint = uint32;
constexpr PVPAlignmentPoint MIN_PVP_ALIGNMENT_POINT = 0;
//constexpr PVPAlignmentPoint MAX_PVP_ALIGNMENT_POINT = 100000; // 최대 값, DataSheet(PVPAlignmentData.json 의  pvpAlignmentPointMax 값

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 아이템 컬렉션 관련 types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using ItemCollectionCategoryId = uint32;
constexpr ItemCollectionCategoryId INVALID_ITEM_COLLECTION_CATEGORY_ID = 0;

using ItemCollectionUnlockValue = uint32;
constexpr ItemCollectionUnlockValue INVALID_ITEM_COLLECTION_UNLOCK_VALUE = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Contents user types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 시스템 메세지 관련 types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using SystemMessageConditionId = uint32;
constexpr SystemMessageConditionId INVALID_SYSTEM_MESSAGE_CONDITION_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Map Event types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using MapEventId = uint32;
constexpr MapEventId INVALID_MAP_EVENT_ID = 0;

using MapEventComponentId = uint32;
constexpr MapEventComponentId INVALID_MAP_EVENT_COMPONENT_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// EventTimeContent types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using EventTimeContentId = int; //MapId나 DungeonGroupId로 변환
constexpr EventTimeContentId INVALID_EVENT_TIME_CONTENT_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 오프라인  types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using OffLinePresetId = uint32;
constexpr OffLinePresetId INVALID_OFF_LINE_PRESET_ID = 0;
using OffLinePlayDataTemplateId = uint32;
using OffLineExp = int64_t;    // 오프라인 결과값 사용할 경험치 (경험치 손실에 대한 - 포함될수 있음)


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 시공의틈새 SpaceCrack   
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using SpaceCrackEnemyKill = int8;   // 몬스터 사냥수
using SpaceCrackEntryCount = int8;		// 입장한수 
using SpaceCrackTemplateId = uint32;
using SpaceCrackPortalGroupId = uint32;
using SpaceCrackWeakReward = int8;
using SpaceCrackConfigTemplateId = uint32;



constexpr SpaceCrackTemplateId INVALID_SPACECRACK_ID = 0;
constexpr SpaceCrackTemplateId INVALID_SPACECRACK_PORTALGROUP_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BM Store
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using BMStoreTabId = int8;	//BM상점탭Id
using BMProductId = uint32; // BM상품Id
using BMHiveIdentifier = uint64; //Hive구매전 트랜잭션으로 사용 될 키 값

constexpr BMProductId INVALID_BMPRODUCT_ID = 0;

using BMItemDBId = uint64;	//bm상점에서 캐쉬로 구입한 아이템 고유키.(t_bmdepot:c_bm_item_id <-> t_bm_shop_transaction: c_hiveiap_transactionId 연관관계)
constexpr BMItemDBId INVALID_BM_ITEM_DB_ID = 0;
constexpr BMHiveIdentifier INVALID_BMHIVE_IDENTIFIER = 0;

using BMStepGroupId = uint32; // 스텝업 GroupId
constexpr BMStepGroupId INVALID_STEP_GROUP_ID = 0;

using BMStepLevel = uint32; // 스텝업 Level
constexpr BMStepLevel INVALID_STEP_LEVEL = 0;

using BMPopupConditionId = uint32; // 팝업조건 Id
constexpr BMPopupConditionId INVALID_BM_POPUP_CONDITION_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 다단계 시스템 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 다단계 랭크
using MultiLevelRank = uint32;
constexpr MultiLevelRank INVALID_MULTI_LEVEL_RANK = 0;

// 다단계 랭크 아이디
using MultiLevelRankId = uint32;
constexpr MultiLevelRankId INVALID_MULTI_LEVEL_RANK_ID = 0;

// 다단계 랭크 그룹 아이디 (분기)
using MultiLevelRankGroupId = uint32;
constexpr MultiLevelRankGroupId INVALID_MULTI_LEVEL_RANK_GROUP_ID = 0;

// 서버에서 사용하는 알림 시스템 
using AlarmId = int64;
constexpr AlarmId INVALID_ALARM_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 거래소 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using MarketTransactionId = uint64;
constexpr MarketTransactionId INVALID_MARKET_TRANSACTION_ID = 0;
//아이템 단가 이기 때문에 소수를 사용한다. (stackable 아이템 묶음으로 판매 시 개별 가격으로 사용해야 함)
using MarketPrice = double;

// 월드 거래소 구분자
using WorldMarketId = uint32;
constexpr WorldMarketId INVALID_WORLD_MARKET_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NPC 이벤트 상점
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using NpcEventShopGroupId = uint32;
constexpr NpcEventShopGroupId INVALID_NPC_EVENT_SHOP_GROUP_ID = 0;
using EventProductId = uint32; // event상품Id
constexpr EventProductId INVALID_NPC_EVENT_SHOP_PRODUCT_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 아이템 제작 관련 types
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using CraftId = uint32;
const CraftId INVALID_CRAFT_ID = 0;

using CraftTabId = uint32;
const CraftTabId INVALID_CRAFT_TAB_ID = 0;

using CraftMaterialId = uint32;
const CraftMaterialId INVALID_CRAFT_MATERIAL_ID = 0;

const uint8 MAX_CRAFT_MATERIAL_COUNT = 10; // 아이템 재료 최대 수량
const uint8 CRAFT_MATERIAL_MAIN_INDEX = 0; // 대체 아이템이 아닌 메인 아이템의 인덱스
const uint8 CRAFT_RESTRICT_CONDITION_MAX_NUM = 2; // 제작 제한 조건 갯수

const uint8 CRAFT_REWARD_SUCCESS_INDEX = 0;
const uint8 CRAFT_REWARD_GREAT_SUCCESS_INDEX = 1;
const uint8 CRAFT_REWARD_FAIL_INDEX = 2;

const uint16 CRAFT_MAX_COUNT = 999; // 한번에 제작할 수 있는 최대 횟수


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 커뮤니티 이벤트 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using CommunityEventId = uint64;
constexpr CommunityEventId INVALID_COMMUNITY_EVENT_ID = 0;

// 커뮤니티 이벤트 미션 ID
using CommunityEventMissionId = uint64;
constexpr CommunityEventMissionId INVALID_COMMUNITY_EVENT_MISSION_ID = 0;

// 커뮤니티 이벤트 미션 타입 ID
using CommunityEventMissionCommonId = uint64;
constexpr CommunityEventMissionCommonId INVALID_COMMUNITY_EVENT_MISSION_COMMON_ID = 0;

// 커뮤니티 이벤트 미션 탭 ID
using CommunityEventMissionTabId = uint64;
constexpr CommunityEventMissionTabId INVALID_COMMUNITY_EVENT_MISSION_TAB_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 출석보상 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//출석부ID
using AttendanceId = uint64;
constexpr AttendanceId INVALID_ATTENDANCE_ID = 0;

//출석 횟수
using AttendanceCount = uint8;
constexpr AttendanceCount INVALID_ATTENDANCE_COUNT = 0;
constexpr AttendanceCount DEAULT_ATTENDANCE_COUNT = 0;

//출석체크할 Index
using AttendanceCheckIdx = uint8;
constexpr AttendanceCheckIdx INVALID_ATTENDANCE_CHECK_IDX = 0;

// 게임이벤트
using GameEventId = uint64;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 업적 시스템
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using AchievementThemeId = uint64;
constexpr AchievementThemeId INVALID_ACHIEVEMENT_THEME_ID = 0;

using AchievementCategoryId = uint64;
constexpr AchievementCategoryId INVALID_ACHIEVEMENT_CATEGORY_ID = 0;

using AchievementGroupId = uint64;
constexpr AchievementGroupId INVALID_ACHIEVEMENT_GROUP_ID = 0;

using AchievementId = uint64;
constexpr AchievementId INVALID_ACHIEVEMENT_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 몬스터 도감
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//몬스터 도감 지식 메뉴(UI) 아이디
using MonsterKnowledgeCategoryMenuId = uint32;
constexpr MonsterKnowledgeCategoryMenuId INVALID_MONSTER_KNOWLEDGE_CATEGORY_MENU_ID = 0;

//몬스터 지식 아이디
using MonsterKnowledgeId = CreatureId;
constexpr MonsterKnowledgeId INVALID_MONSTER_KNOWLEDGE_ID = 0;

using MonsterKnowledgeExp = Exp;
constexpr MonsterKnowledgeExp INVALID_MONSTER_KNOWLEDGE_EXP = 0;

using MonsterKnowledgeLevel = Level;

//지식 석판 아이디
using MonsterKnowledgeSlateId = uint32;
constexpr MonsterKnowledgeSlateId INVALID_MONSTER_KNOWLEDGE_SLATE_ID = 0;

//지식 석판 슬롯 아이디
using MonsterKnowledgeSlateSlotId = uint32;
constexpr MonsterKnowledgeSlateSlotId INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 도감 시스템 개선 MonsterKnowledge -> Codex 명칭 수정
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//몬스터 일람 대륙(카테고리)
using CodexCategoryId = uint32;
using CodexCategoryLevel = Level;

constexpr CodexCategoryId INVALID_CODEX_CATEGORY_ID = 0;
//

//몬스터 일람 맵 그룹
using CodexMapGroupId = uint32;
using CodexMapGroupExp = Exp;
using CodexMapGroupLevel = Level;

constexpr CodexMapGroupId INVALID_CODEX_MAP_GROUP_ID = 0;
//

//지식 석판 특수 노드
using SlateSpecialNodeId = uint32;

constexpr SlateSpecialNodeId INVALID_SLATE_SPECIAL_NODE_ID = 0;
//


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 칭호 시스템 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using TitleId = uint32;
constexpr TitleId INVALID_TITLE_ID = 0;

using TitleCategoryId = uint32;
constexpr TitleCategoryId INVALID_TITLE_CATEGORY_ID = 0;


constexpr uint16 MAX_VISUAL_BOT = 1000;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 통계 시스템 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 통계 아이디
using StatisticsId = uint32;
const StatisticsId INVALID_STATISTICS_ID = 0;

using StatisticsTabId = uint8;
const StatisticsTabId INVALID_STATISTICS_TAB_ID = 0;

// 통계 값
using StatisticsValue = uint64;
const StatisticsValue MAX_STATISTICS_VALUE = 999'999'999'999;

// 요일 최댓 값(일요일 = 0, 월요일 = 1, ... 토요일 = 6)
const uint8 DAY_OF_THE_WEEK_COUNT = 7;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 콘텐츠 잠금 해제 (Unlock Contents)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using UnlockContentsId = uint16;
constexpr UnlockContentsId INVALID_UNLOCK_CONTENTS_ID = 0;

// 서버에서 사용하기 위한 Id값(데이터에서 해당 Id가 변경되면 다음값을도 변경이 필요합니다.)
constexpr UnlockContentsId TITLE_UNLOCK_CONTENTS_ID = 26; // 칭호 Unlock
constexpr UnlockContentsId PARTY_UNLOCK_CONTENTS_ID = 31; // 파티 Unlock
constexpr UnlockContentsId BM_UNLOCK_CONTENTS_ID = 17; // BM상점 Unlock

constexpr UnlockContentsId OFFLINE_MODE_UNLOCK_CONTENTS_ID = 13; // 오프라인 플레이 Unlock
constexpr UnlockContentsId DECOMPOSE_UNLOCK_CONTENTS_ID = 77; // 분해 & 자동분해
constexpr UnlockContentsId PG_UNLOCK_CONTENTS_ID = 80; // PG 결제 언락

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 캠프 시스템 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using CampId = uint32;
constexpr CampId INVALID_CAMP_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//성소
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using SanctumAreaId = uint32;
constexpr SanctumAreaId INVALID_SANCTUM_AREA_ID = 0;
using SanctumNexusId = CampId;
constexpr SanctumNexusId INVALID_SANCTUM_NEXUS_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 침공 (플레이어킬포인트)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using PlayerKillPoint = uint64;
constexpr PlayerKillPoint INVALID_PLAYER_KILL_POINT = 0;

using InvadeBattleLevel = uint32;
constexpr InvadeBattleLevel INVALID_INVADE_BATTLE_LEVEL = 0;

constexpr float		INVADE_PORTAL_SPAWN_RADIUS = 200.f;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//필드 보스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//필드 보스 Id
using FieldBossId = uint32;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//월드 보스
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//월드 보스 Id
using WorldBossId = uint32;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 파티던전 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
using PartyDungeonKey = std::tuple<DungeonGroupId, DungeonDifficulty>;
#endif
using PartyDungeonFreeKeyCount = uint32;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 대결시스템 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 결투장 ID
using ArenaId = int64;
constexpr ArenaId INVALID_ARENA_ID = -1;

using ArenaTeamId = uint8;
constexpr ArenaTeamId INVALID_ARENA_TEAM_ID = 0;

constexpr time_t WAIT_DUEL_INVITE_TIME = 15 * 1000;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 배틀 패스 시스템 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 배틀 패스 ID
using BattlePassId = uint64;
constexpr BattlePassId INVALID_BATTLE_PASS_ID = 0;

// 배틀 패스 레벨 그룹 ID
using BattlePassLevelGroupId = uint64;
constexpr BattlePassLevelGroupId INVALID_BATTLE_PASS_LEVEL_GROUP_ID = 0;

// 배틀 패스 보상 ID
using BattlePassRewardId = uint64;
constexpr BattlePassRewardId INVALID_BATTLE_PASS_REWARD_ID = 0;

// 배틀 패스 보상 그룹 ID
using BattlePassRewardGroupId = uint64;
constexpr BattlePassRewardGroupId INVALID_BATTLE_PASS_REWARD_GROUP_ID = 0;

// 배틀 패스 미션 ID
using BattlePassMissionId = uint64;
constexpr BattlePassMissionId INVALID_BATTLE_PASS_MISSION_ID = 0;

// 배틀 패스 미션 그룹 ID
using BattlePassMissionGroupId = uint64;
constexpr BattlePassMissionGroupId INVALID_BATTLE_PASS_MISSION_GROUP_ID = 0;

// 배틀 패스 미션 탭 ID
using BattlePassMissionTabId = uint64;
constexpr BattlePassMissionTabId INVALID_BATTLE_PASS_MISSION_TAB_ID = 0;

// 맵 이벤트 관련
using EventSubDialogId = uint32;
using EvenParameterValue = int64;
using EvenParameterCount = int16;

constexpr EventSubDialogId INVALID_EVENT_SUB_DIALOG_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HIVE 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using  DeviceId = uint64_t;
using  TimeZone = int32_t;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PK Book
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using PKRecordId = uint64;
constexpr PKRecordId INVALID_PK_RECORD_ID = 0;

// PK 기여자 ID(User일경우 UserDBId, NPC일경우 CreatureId)
using PKContributorId = uint64;
constexpr PKContributorId INVALID_PK_CONTRIBUTOR_ID = 0;

using PKPoint = uint64;

constexpr time_t UNKNOWN_LOGOUT_TIME = -1;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 보스 소환 시스템
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 몬스터 소환 정보 ID
using SpawnMonsterItemId = uint64;
constexpr SpawnMonsterItemId INVALID_SPAWN_MONSTER_ITEM_ID = 0;

// 몬스터 소환 보상 정보 ID
using SpawnMonsterItemRewardId = uint64;
constexpr SpawnMonsterItemRewardId INVALID_SPAWN_MONSTER_ITEM_REWARD_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SOCIAL_EMOTION
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using UserSocialId = uint32;
constexpr UserSocialId INVALID_USER_SOCIAL_ID = 0;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 봉인(문양) 시스템
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 봉인 팩 아이디
using SealPackId = uint32;
constexpr SealPackId INVALID_SEAL_PACK_ID = 0;

// 봉인 노드 아이디
using SealId = uint32;
constexpr SealId INVALID_SEAL_ID = 0;

// 봉인 슬롯 아이디
using SealSlotId = uint32;
constexpr SealSlotId INVALID_SEAL_SLOT_ID = 0;

// 봉인 슬롯 스탯 풀 아이디
using SealSlotStatPoolId = uint32;
constexpr SealSlotStatPoolId INVALID_SEAL_SLOT_STAT_POOL_ID = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 랭킹 시스템 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 유저 랭크
using Rank = uint32;
constexpr Rank INVALID_RANK = 0;




// 기사단 경매장 관련 

using GuildStoreKey = uint64;  

using GuildAuctionId = uint64;
constexpr GuildAuctionId INVALID_GUILD_AUCTION_ID = 0;

using GuildAcquisitionSettingId = int32;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 네트워크 통신에 사용하는 공용 구조체
// 동일한 메모리 구조 보장을 위해 #pragma pack(push, 1) 을 사용해야 하고,
// #pragma warning(disable:4315) 로 C4315 Warning 을 막아주어야 합니다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable:4315)
#pragma pack(push, 1)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 서버 구성 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr PlanetId KOR_LIVE_PLANET_ID = 30;
constexpr PlanetId KOR_DEV_QA_PLANET_ID = 5;

struct PlanetWorldId
{
	bool operator == (const PlanetWorldId& rhs)const noexcept
	{
		return (this->value == rhs.value);
	}

	bool operator != (const PlanetWorldId& rhs)const noexcept
	{
		return (this->value != rhs.value);
	}

	union
	{
		uint16 value = 0;
		struct
		{
			//0-1023
			WorldId worldId : 10;   // Planet 내 World Id
			//0-31
			PlanetId planetId : 6;  // Planet Id
		} st;
	};

	PlanetWorldId() = default;
	PlanetWorldId(uint16 planetWorldValue)
	{
		value = planetWorldValue;
	}
	PlanetWorldId(PlanetId planetId, WorldId worldId)
	{
		st.planetId = planetId;
		st.worldId = worldId;
	}
};

// for set/map
struct ComparePlanetWorldId
{
	bool operator() (const PlanetWorldId& lhs, const PlanetWorldId& rhs) const noexcept
	{
		return lhs.value < rhs.value;
	}
};


#ifdef _SERVER
using PlanetWorldIdSet = std::set<PlanetWorldId, ComparePlanetWorldId>;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 유저 관련 구조체
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 재화
struct CurrencyTable
{
public:
	Money		mGold = 0;		// 골드
	Gem			mDia = 0;		// 다이아몬드
	Gem			mFreeDia = 0;	// 무료 다이아몬드(일본결제법용)
	Gem			mCashMileage = 0;	// 캐시마일리지(캐쉬 구매시)
	Gem			mMileage = 0;		// 마일리지(다이아 구매시)
	DeathPenaltyFreeCount	mExpDeathPenaltyFree = 0;	// 경험치 무료 복구 비용  (추후 Enum 변경할수도 있음)
	DeathPenaltyFreeCount 	mItemDeathPenaltyFree = 0;	// 아이템 무료 복구 비용
	Money					mMonsterBookPoint = 0;		// 몬스터 도감 포인트
	Money					mSummonFairyMileage = 0;	// 페어리 소환 마일리지
	Money					mSummonCostumeMileage = 0;// 코스튬 소환 마일리지
	Money					mSummonUniqueFairyMileage = 0;	//유니크 페어리 소환 마일리지
	Money					mSummonUniqueCostumeMileage = 0;// 유니크 코스튬 소환 마일리지

#ifdef _SERVER
	//재화의 종류가 늘어 날때마다 추가 작업을 해줘야합니다
	//DB에 넣기 좋도록 가져온다
	auto GetDBFormat()
	{
		return std::xvector<std::pair<CurrencyType, Currency>>({ {CurrencyType::GOLD,		mGold},
																{CurrencyType::MILEAGE,		mMileage},
																{CurrencyType::DIA,			mDia},
																{CurrencyType::FREE_DIA,	mFreeDia},
																{CurrencyType::EXP_DEATH_PENALTY_FREE,		mExpDeathPenaltyFree},
																{CurrencyType::ITEM_DEATH_PENALTY_FREE,		mItemDeathPenaltyFree},
																{CurrencyType::MONSTER_BOOK_POINT,			mMonsterBookPoint},
																{CurrencyType::SUMMON_FAIRY_MILEAGE,		mSummonFairyMileage},
																{CurrencyType::SUMMON_COSTUME_MILEAGE,		mSummonCostumeMileage},
			});

	}
#endif
};

// 유저 외형정보
// 인벤토리의 장착 아이템에 의존적 (Style 장비의 Visibility 옵션에 따라 채워질 예정)
struct UserLookInfo
{
public:
	ItemId				mWeapon = INVALID_ITEM_ID;
	//ItemId			mArmor = INVALID_ITEM_ID;							// 클라에서 사용하지 않기로 하여 삭제.
	FairyId				mFairy = INVALID_FAIRY_ID;
	bool				mViewHelmet = true;									// 투구를 보여줄 것인가?
	CostumeId			mCostumeId = INVALID_COSTUME_ID;
	CostumeId			mTransformCostumeId = INVALID_COSTUME_ID;			// 외형 변경 정보.

	Level				mWeaponLevel = INVALID_LEVEL;
	PVPAlignmentGrade	mPVPAlignmentGrade = PVPAlignmentGrade::NETURAL;	// pvp 성향 등급(이름색 표시용도)
};

// 커스터마이징 정보 (얼굴)
struct CustomizeFaceData
{
	/* 주의: 구조체의 내용을 변경 할 경우, 서버프로그래에게 알려주세요, CustomizeBodyJson 수정 대응이 필요합니다. */
public:
	CustomPartId mFaceId = INVALID_CUSTOM_PART_ID;
	CustomPartId mHairId = INVALID_CUSTOM_PART_ID;
	CustomTattooId mTattooId = INVALID_CUSTOM_TATTOO_ID;

	uint8 mHairColorIndex = DEFAULT_CUSTOM_COLOR_ID;
	uint8 mSkinColorIndex = DEFAULT_CUSTOM_COLOR_ID;
	uint8 mBrowColorIndex = DEFAULT_CUSTOM_COLOR_ID;
	uint8 mPupilColorIndex = DEFAULT_CUSTOM_COLOR_ID;

	uint8 mCheekSize = DEFAULT_CUSTOM_VALUE;
	uint8 mChinLength = DEFAULT_CUSTOM_VALUE;
	uint8 mChinSize = DEFAULT_CUSTOM_VALUE;
	uint8 mNosePosition = DEFAULT_CUSTOM_VALUE;
	uint8 mEyePosition = DEFAULT_CUSTOM_VALUE;
	uint8 mEyeSpacing = DEFAULT_CUSTOM_VALUE;
	uint8 mEyeSize = DEFAULT_CUSTOM_VALUE;
	uint8 mBrowPosition = DEFAULT_CUSTOM_VALUE;
	uint8 mBrowAngle = DEFAULT_CUSTOM_VALUE;
	uint8 mBrowLength = DEFAULT_CUSTOM_VALUE;
	uint8 mMouthPosition = DEFAULT_CUSTOM_VALUE;
	uint8 mMouthSize = DEFAULT_CUSTOM_VALUE;
};

// 커스터마이징 정보 (바디)
struct CustomizeBodyData
{
	/* 주의: 이 곳을 변경 할 경우, 서버프로그래에게 알려주세요, CustomizeBodyJson 수정 대응이 필요합니다. */
public:
	//CustomPartId mBodyId = INVALID_CUSTOM_PART_ID;	// 클라에서 사용하지 않기로 하여 삭제.

	uint8 mSkinColorIndex = DEFAULT_CUSTOM_COLOR_ID;	//

	uint8 mHeight = DEFAULT_CUSTOM_VALUE;				//
	uint8 mHeadSize = DEFAULT_CUSTOM_VALUE;				//
	uint8 mChestSize = DEFAULT_CUSTOM_VALUE;			//
	uint8 mWaistSize = DEFAULT_CUSTOM_VALUE;			//
	uint8 mPelvisSize = DEFAULT_CUSTOM_VALUE;			//
	uint8 mLegSize = DEFAULT_CUSTOM_VALUE;				//

	bool IsModify()
	{
		return (mHeight != DEFAULT_CUSTOM_VALUE) &&
			(mHeadSize != DEFAULT_CUSTOM_VALUE) &&
			(mChestSize != DEFAULT_CUSTOM_VALUE) &&
			(mWaistSize != DEFAULT_CUSTOM_VALUE) &&
			(mPelvisSize != DEFAULT_CUSTOM_VALUE) &&
			(mLegSize != DEFAULT_CUSTOM_VALUE);
	}
};


// 남에게 보여질 최소한의 정보
struct RemotePlayerBaseInfo
{
public:
	AccountDBId			mAccountDBId = INVALID_ACCOUNT_DB_ID;
	UserDBId			mUserDBId = INVALID_USER_DB_ID;
	CreatureGenderType	mGenderType = CreatureGenderType::MAX;
	Level				mLevel = MIN_LEVEL;


public:
	RemotePlayerBaseInfo() = default;
	explicit RemotePlayerBaseInfo(
		AccountDBId accountDBId, UserDBId userDBId, CreatureGenderType genderType, Level level
	)
		:
		mAccountDBId(accountDBId),
		mUserDBId(userDBId),
		mGenderType(genderType),
		mLevel(level)
	{
	}
};

// 남에게 보여질 정보 (RemotePlayerBaseInfo + 추가 정보)
struct RemotePlayerInfo
{
public:
	RemotePlayerBaseInfo	mBaseInfo;
	MapId					mMapId = INVALID_MAP_ID;
	time_t					mLastLoginTime = 0;


public:
	RemotePlayerInfo() = default;
	explicit RemotePlayerInfo(RemotePlayerBaseInfo& baseInfo, MapId mapId, time_t lastLoginTime)
		:
		mBaseInfo(baseInfo),
		mMapId(mapId),
		mLastLoginTime(lastLoginTime)
	{
	}
};

// 친구초대/길드가입신청 등에 사용되는 정보
struct PlayerInvitationInfo
{
public:
	RemotePlayerBaseInfo	mRemotePlayerBaseInfo;
	time_t					mCreatedTime;


public:
	explicit PlayerInvitationInfo(RemotePlayerBaseInfo& remotePlayerBaseInfo, time_t createdTime)
		:
		mRemotePlayerBaseInfo(remotePlayerBaseInfo),
		mCreatedTime(createdTime)
	{
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스탯 관련 구조체
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// CS_REQ_BONUS_STAT_INCREASE 패킷 수신 시 사용
struct StatInfo
{
public:
	StatType mType = StatType::NONE;
	StatValue mValue = MIN_STAT_VALUE;


public:
	StatInfo() noexcept : mType(StatType::NONE), mValue(MIN_STAT_VALUE) {}

	explicit StatInfo(StatType type, StatValue value) noexcept
		:
		mType(type),
		mValue(value)
	{
	}

	void Clear()
	{
		mType = StatType::NONE;
		mValue = 0;
	}
};

// SC_SKILL_DAMAGED 패킷에서 사용
struct DamageStatInfo
{
	DamageStatType mDamageStatType = DamageStatType::MAX;
	StatValue mValue = MIN_STAT_VALUE;

public:
	DamageStatInfo() noexcept : mDamageStatType(DamageStatType::MAX), mValue(MIN_STAT_VALUE) {}

	explicit DamageStatInfo(DamageStatType type, StatValue value) noexcept
		:
		mDamageStatType(type),
		mValue(value)
	{
	}

	void Clear()
	{
		mDamageStatType = DamageStatType::MAX;
		mValue = 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Item 관련 구조체
// 공용 정보와 장착 정보를 분리 검토 중
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using ItemRefineOptionCount = uint8;											// 제련 옵션 갯수 
constexpr ItemRefineOptionCount MAX_ITEM_REFINE_OPTION_COUNT = 3;				// 최대 제련 옵션 갯수

using RefineCost = Currency;													// 제련 비용

using RefineOptionId = uint64;													// 제련 옵션 아이디
constexpr RefineOptionId INVALID_REFINE_OPTION_ID = 0;							// 

using RefineOptionStatId = uint64;												// 제련 옵션 스탯 아이디
constexpr RefineOptionStatId INVALID_REFINE_OPTION_STAT_ID = 0;					// 

using RefineOptionStatNumber = uint8;											// 제련 옵션 획득 갯수

using RefineBonusId = uint64;													// 제련 보너스 아이디
constexpr RefineBonusId INVALID_REFINE_BONUS_ID = 0;							// 

using MagicalForgeId = uint32;													// 마법 부여Id	(=> ItemEffectId 와 같아서 uint32)
constexpr MagicalForgeId INVALID_MAGICAL_FORGE_ID = 0;							// 

using ItemOptionSkillGroupId = uint64;											// 아이템 옵션 스킬 그룹 아이디
const ItemOptionSkillGroupId INVALID_ITEM_OPTION_SKILL_GROUP_ID = 0;			//

using ItemOptionSkillLevelGroupId = uint64;										// 아이템 옵션 스킬 레벨 그룹 아이디
const ItemOptionSkillLevelGroupId INVALID_ITEM_OPTION_SKILL_LEVEL_GROUP_ID = 0;	//

constexpr uint8 MAX_ITEM_SKILL_COUNT = 3;										// 최대 스킬 옵션 수


// 아이템 제련 정보
struct ItemRefineStatInfo
{
public:
	RefineOptionStatId mRefineOptionStatId = INVALID_REFINE_OPTION_STAT_ID;	// DB에 저장하기 위한 RefineOptionStatId
	StatType mType = StatType::NONE;
	StatValue mValue = MIN_STAT_VALUE;


public:
	ItemRefineStatInfo() noexcept
		:
		mRefineOptionStatId(INVALID_REFINE_OPTION_STAT_ID),
		mType(StatType::NONE),
		mValue(MIN_STAT_VALUE)
	{}

	explicit ItemRefineStatInfo(RefineOptionStatId refineOptionStatId, StatType type, StatValue value) noexcept
		:
		mRefineOptionStatId(refineOptionStatId),
		mType(type),
		mValue(value)
	{}

	void Clear()
	{
		mRefineOptionStatId = INVALID_REFINE_OPTION_STAT_ID;
		mType = StatType::NONE;
		mValue = 0;
	}
};

// 제련 보너스 정보
struct RefineBonusInfo
{
	RefineBonusId mBonusId = INVALID_REFINE_BONUS_ID;
	StatValue mValue = MIN_STAT_VALUE;

	RefineBonusInfo() = default;
	explicit RefineBonusInfo(RefineBonusId bonusId, StatValue statValue)
		: mBonusId(bonusId), mValue(statValue) {}
};

// 마법부여 정보
struct MagicalForgeData
{
	MagicalForgeId	mMagicalForgeId = INVALID_MAGICAL_FORGE_ID;	// 
	Level			mLevel = INVALID_LEVEL;						// 

	MagicalForgeData() = default;
	explicit MagicalForgeData(MagicalForgeId magicalForgeId)
		: mMagicalForgeId(magicalForgeId) {}


public:
	bool IsMagicalForge() const { return mMagicalForgeId != INVALID_MAGICAL_FORGE_ID && mLevel != INVALID_LEVEL; }
};

// CurrencyTYpe
struct CurrencyTypeDeltaPair
{
public:
	CurrencyType			mCurrencyType = CurrencyType::NONE;
	CurrencyDelta			mCurrencyDelta = 0;


public:
	CurrencyTypeDeltaPair() = default;
	explicit CurrencyTypeDeltaPair(CurrencyType currencyType, CurrencyDelta currencyDelta)
		:
		mCurrencyType(currencyType),
		mCurrencyDelta(currencyDelta)
	{
	}
};

// 아이템 Datasheet Template ID와 갯수
struct ItemIdAmountPair
{
public:
	ItemId		mItemId = INVALID_ITEM_ID;
	ItemAmount	mItemAmount = 0;


public:
	ItemIdAmountPair() = default;
	explicit ItemIdAmountPair(ItemId id, ItemAmount amount)
		:
		mItemId(id),
		mItemAmount(amount)
	{
	}
};

// 아이템 DB ID와 갯수
struct ItemDBIdAmountPair
{
	ItemDBId	mItemDBId = INVALID_ITEM_DB_ID;
	ItemAmount	mItemAmount = MIN_ITEM_AMOUNT;


public:
	ItemDBIdAmountPair() = default;
	explicit ItemDBIdAmountPair(ItemDBId dbId, ItemAmount amount)
		:
		mItemDBId(dbId),
		mItemAmount(amount)
	{
	}
};

// 아이템 DB ID와 제작 성공 여부
struct ItemDBIdCraftResultPair
{
	ItemDBId	mItemDBId = INVALID_ITEM_DB_ID;
	CraftResult	mCraftResult = CraftResult::NONE;
	ItemAmount	mItemAmount = MIN_ITEM_AMOUNT;
	CurrencyType mCurrencyType = CurrencyType::NONE;


public:
	ItemDBIdCraftResultPair() = default;
	explicit ItemDBIdCraftResultPair(ItemDBId dbId, CraftResult craftResult, ItemAmount itemAmount)
		:
		mItemDBId(dbId),
		mCraftResult(craftResult),
		mItemAmount(itemAmount)
	{
	}
	explicit ItemDBIdCraftResultPair(CraftResult craftResult, CurrencyType currencyType)
		:
		mCraftResult(craftResult),
		mCurrencyType(currencyType)
	{
	}
};

// 제작 ID와 아이템 갯수
struct CraftIdAmountPair
{
	CraftId		mCraftId = INVALID_CRAFT_ID;
	ItemAmount	mItemAmount = 0;


public:
	CraftIdAmountPair() = default;
	explicit CraftIdAmountPair(CraftId craftId, ItemAmount itemAmount)
		:
		mCraftId(craftId),
		mItemAmount(itemAmount)
	{
	}
};

// 제작 재료 ID와 아이템 갯수
struct CraftMaterialIdAmountPair
{
	CraftMaterialId		mCraftMaterialId = INVALID_CRAFT_MATERIAL_ID;
	ItemAmount			mItemAmount = 0;


public:
	CraftMaterialIdAmountPair() = default;
	explicit CraftMaterialIdAmountPair(CraftMaterialId craftMaterialId, ItemAmount itemAmount)
		:
		mCraftMaterialId(craftMaterialId),
		mItemAmount(itemAmount)
	{
	}
};

//창고 그룹 저장 Id
using AutoDepotGroupId = uint16;
constexpr AutoDepotGroupId INVALID_AUTO_DEPOT_GROUP_ID = 0;

//아이템 자동 장착류 판매 설정
#ifdef _SERVER
struct AutoShopSellEquipPair : public Disposable
#else // _SERVER
struct AutoShopSellEquipPair
#endif // _SERVER
{
public:
	AutoShopSellEquipPair() = default;
	explicit AutoShopSellEquipPair(ItemGrade itemGrade, ItemCategoryMain itemCatergoryMain)
		:
		mItemGrade(itemGrade),
		mItemCatergoryMain(itemCatergoryMain)
	{
	}

	ItemGrade			mItemGrade = ItemGrade::NONE;
	ItemCategoryMain	mItemCatergoryMain = ItemCategoryMain::NONE;
};



#ifdef _SERVER
struct AutoDepotConfigPair : public Disposable
#else
struct AutoDepotConfigPair
#endif
{
public:
	AutoDepotConfigPair() = default;
	explicit AutoDepotConfigPair(AutoDepotGroupId autoDepotGroupId, ItemGrade itemGrade)
		:
		mAutoDepotGroupId(autoDepotGroupId),
		mItemGrade(itemGrade)		
	{
	}

	AutoDepotGroupId	mAutoDepotGroupId = INVALID_AUTO_DEPOT_GROUP_ID;
	ItemGrade			mItemGrade = ItemGrade::NONE;	
};


// 아이템 공용 정보
#ifdef _SERVER
struct ItemData : public Disposable
#else // _SERVER
struct ItemData
#endif // _SERVER
{
public:
	ItemDBId	mItemDBId = INVALID_ITEM_DB_ID;			// 아이템 고유의 DB ID (DB의 고유 Id)
	ItemId		mTemplateId = INVALID_ITEM_ID;			// 아이템 템플릿 ID (데이터 시트 고유 Id)
	ItemAmount	mItemAmount = 1;						// 아이템이 쌓인 갯수

	bool		mIsLocked = false;						// 잠금 여부

	time_t		mExpiredTime = 0;				// 아이템 만료 기간

	InventorySortValue mInventorySortValue = 0;		// 인벤토리 아이템 순서 정렬을 위한 값

	ItemData() = default;
	explicit ItemData(ItemDBId itemDBId, ItemId itemId, ItemAmount itemAmount) :
		mItemDBId(itemDBId), mTemplateId(itemId), mItemAmount(itemAmount)
	{}

	explicit ItemData(ItemId itemId, ItemAmount itemAmount) :
		mTemplateId(itemId), mItemAmount(itemAmount)
	{}

};
// 생성 아이템 정보
struct ItemDataCreated : public ItemData
{
	Level				mLevel = 0;						// 장비 레벨	
	EnchantShield		mEnchantShield = 0;				// 강화 내구도
	EnchantBonusIndex	mEnchantBonusIndex = 0;			// 강화 보너스 인덱스


public:
	ItemDataCreated() = default;

	explicit ItemDataCreated(ItemId itemId, ItemAmount itemAmount) :
		ItemData(itemId, itemAmount)
	{}
};

// 장비 아이템 전용 정보
struct ItemDataEquip : public ItemDataCreated
{
public:
	// 장비 아이템 전용 프리셋 정보
	PresetValue	mPresetValue = INVALID_PRESET_VALUE;	// 프리셋 비트필드 값 (복수의 프리셋에 속할 수 있다)

	// 장비 아이템 전용 소켓 정보
	ItemDBId	mParentItemDBId = INVALID_ITEM_DB_ID;	// 자신이 소켓팅된 부모 아이템
	SocketId	mSocketId = INVALID_SOCKET_ID;			// 자신이 소켓팅된 소켓 ID

	// 장비 아이템 전용 제련 정보
	StatInfo mRefineOptionArray[MAX_ITEM_REFINE_OPTION_COUNT] = {};
	RefineBonusInfo mRefineBonusInfo;

	// 장비 아이템 슬롯 인덱스
	// (기본 = 0, 귀걸이 같은 슬롯이 2개 이상인 악세사리일 경우 0b101 이면 1번 3번 프리셋은 두번째 슬롯, 2번 프리셋은 기본 슬롯)
	ItemEquipSlotIndex	mItemEquipSlotIndex = 0;

	// 마법부여 정보
	MagicalForgeData mMagicalForgeData;


public:
	ItemDataEquip() noexcept = default;
	ItemDataEquip(const ItemDataCreated& itemDataCreated)
	{
		memcpy(this, &itemDataCreated, sizeof(ItemDataCreated));
	}


public:
	bool IsMagicalForge() const { return mMagicalForgeData.IsMagicalForge(); }
};

// 장착할 장비 아이템 요청 정보
struct ItemDataEquipRequestInfo
{
	ItemEquipSlotIndex	mItemEquipSlotIndex = 0;
	ItemDBId			mItemDBId = INVALID_ITEM_DB_ID;
};


// 장착할 장비 아이템 요청 정보
struct ItemDbIdSortPair
{
	ItemDBId			mItemDBId = INVALID_ITEM_DB_ID;
	InventorySortValue	mInventorySortValue = 0;


	explicit ItemDbIdSortPair(ItemDBId itemDBId, InventorySortValue inventorySortValue)
		:
		mItemDBId(itemDBId),
		mInventorySortValue(inventorySortValue)
	{
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 스킬 슬롯 변경 결과 데이터
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
struct SkillSlotInfo : public Disposable
#else // _SERVER
struct SkillSlotInfo
#endif // _SERVER
{
public:
	SkillId		mRootSkillId = INVALID_SKILL_ID;			// Root스킬 ID
	SkillId		mActiveSkillId = INVALID_SKILL_ID;			// Active스킬 ID
	SkillSlotId	mSkillSlotId = INVALID_SKILL_SLOT_ID;		// 변경된 슬롯 정보
	bool		mAutoUse = false;							// 자동 사용


public:
	SkillSlotInfo() = default;
	explicit SkillSlotInfo(SkillId rootSkillId, SkillId activeSkillId, SkillSlotId slotId, bool autoUse)
		:
		mRootSkillId(rootSkillId),
		mActiveSkillId(activeSkillId),
		mSkillSlotId(slotId),
		mAutoUse(autoUse)
	{
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 클라이언트와 동기화할 Abnormality 데이터
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct AbnormalityData
{
public:
	AbnormalityId			mId = INVALID_ABNORMALITY_ID;					// ID
	AbnormalityEffectLevel	mLevel = INVALID_ABNORMALITY_EFFECT_LEVEL;		// 스킬 레벨, 아이템 등은 Default(MIN_ABNORMALITY_EFFECT_LEVEL=1)
	AbnormalityInstanceId	mInstanceId = INVALID_ABNORMALITY_INSTANCE_ID;	// InstanceID
	float					mRemainTime = 0.f;								// 만료되기까지 남은 시간 (초)
	AbnormalityStackCount	mStackCount = MIN_ABNORMALITY_STACK;			// 중첩 횟수
	int64					mCasterId = 0/*INVALID_GAME_ID*/;				// 발생시킨 오브젝트의 게임ID (GameId)


public:
	AbnormalityId GetId() const noexcept { return mId; }
	void SetId(AbnormalityId id) noexcept { mId = id; }

	AbnormalityEffectLevel GetLevel() const noexcept { return mLevel; }
	void SetLevel(AbnormalityEffectLevel level) noexcept { mLevel = level; }


	AbnormalityInstanceId GetInstanceId() const noexcept { return mInstanceId; }
	void SetInstanceId(AbnormalityInstanceId instanceId) noexcept { mInstanceId = instanceId; }

	float GetRemainTime() const noexcept { return mRemainTime; }
	void SetRemainTime(float value) noexcept { mRemainTime = value; }

	AbnormalityStackCount GetStackCount() const noexcept { return mStackCount; }
	void SetStackCount(AbnormalityStackCount value) noexcept { mStackCount = value; }

	int64 GetCasterId() const noexcept { return mCasterId; }
	void SetCasterId(int64 id) noexcept { mCasterId = id; }

	int Size() const noexcept { return sizeof(AbnormalityData); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 이동/전투 동기화 관련 구조체
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Creature의 충돌 체크를 위한 Cylinder 정보
#ifdef _SERVER
struct Cylinder
{
	float mRadius = 0.f;			// 반경
	float mHeight = 0.f;			// 높이
	float mLocalDistance = 0.f;		// Owner 중심점 기준 상대 거리
	float mLocalHeight = 0.f;		// Owner 중심점 기준 상대 높이
	FDir  mLocalAngle = 0;			// Owner 중심점 기준 상대 각도
	bool  mCheckForMove = false;	// 이동시 체크 여부
};
using CylinderList = std::xvector<Cylinder>;

#else // _SERVER
struct Cylinder
{
	float mRadius = 0.f;			// 반경
	float mHeight = 0.f;			// 높이
	float mLocalDistance = 0.f;		// Owner 중심점 기준 상대 거리
	float mLocalHeight = 0.f;		// Owner 중심점 기준 상대 높이
	int16 mLocalAngle = 0;			// Owner 중심점 기준 상대 각도
	bool  mCheckForMove = false;	// 이동시 체크 여부
};
#endif // _SERVER


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 퀘스트 관련 구조체
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 반복퀘스트 보상 상자 정보(RewardBoxId, ItemBagIndex)
struct RewardBoxItemIdPair
{
	RewardBoxId		mRewardBoxId;
	ItemBagIndex	mItemBagIndex;


public:
	RewardBoxItemIdPair() = default;
	explicit RewardBoxItemIdPair(RewardBoxId rewardBoxId, ItemBagIndex itemBagIndex)
		:
		mRewardBoxId(rewardBoxId),
		mItemBagIndex(itemBagIndex)
	{
	}

	bool operator<(const RewardBoxItemIdPair& rhs) const
	{
		if (mRewardBoxId == rhs.mRewardBoxId)
			return (mItemBagIndex < rhs.mItemBagIndex);

		return (mRewardBoxId < rhs.mRewardBoxId);
	}

	bool operator == (const RewardBoxItemIdPair& rhs)const noexcept
	{
		return (this->mRewardBoxId == rhs.mRewardBoxId) && (this->mItemBagIndex == rhs.mItemBagIndex);
	}
};

struct QuestUpdateObjectiveData
{
	QuestObjectiveValue	mValue = MIN_QUEST_OBJECTIVE_VALUE;	// 퀘스트 Objective Value
};

#ifdef _SERVER
using RewardBoxItemIdPairList = std::xvector<RewardBoxItemIdPair>;
using QuestRepeatRewardMap = std::xunordered_map<QuestIndex, RewardBoxItemIdPairList>;
using SelectedRewardPair = std::pair<RewardBoxItemIdPair, ItemBagIndex>;

using QuestUpdateObjectiveDataArray = std::xarray<QuestUpdateObjectiveData, MAX_QUEST_OBJECTIVE_COUNT>;

struct QuestUpdateData
{
	StoryId							mStoryId = INVALID_STORY_ID;	// 스토리ID
	QuestId							mQuestId = INVALID_QUEST_ID;	// 퀘스트ID
	QuestIndex						mQuestIndex = INVALID_QUEST_INDEX; // 퀘스트 인덱스
	QuestType						mQuestType = QuestType::MAX;	// QuestType
	QuestState						mState = QuestState::NONE;		// 퀘스트 상태
	QuestUpdateObjectiveDataArray	mObjectiveDataList;				// QuestObjective
	time_t							mTimeAccepted = 0;				//퀘스트 수락 시간
};

struct RepeatQuestUpdateData : public QuestUpdateData
{
	RepeatStoryType			mRepeatStoryType = RepeatStoryType::MAX;
	ChapterId				mScrollChapterId = INVALID_CHAPTER_ID;
	uint8					mRefreshCount = 0;
};

struct QuestRepeatRewardCacheData
{
	QuestIndex		mQuestIndex = INVALID_QUEST_INDEX;		// 퀘스트 인덱스
	RewardBoxId		mRewardBoxId = INVALID_REWARD_BOX_ID;
	ItemBagIndex	mItemBagIndex = INVALID_ITEM_BAG_INDEX;
};

struct QuestRepeatIdRewardPair : public RewardBoxItemIdPair
{
	QuestIndex mQuestIndex;
};

using QuestRepeatIdRewardPairList = std::xvector<QuestRepeatIdRewardPair>;

#else // _SERVER
struct QuestUpdateData
{
	StoryId						mStoryId = INVALID_STORY_ID;						// 스토리ID
	QuestId						mQuestId = INVALID_QUEST_ID;						// 퀘스트ID
	QuestIndex					mQuestIndex = INVALID_QUEST_INDEX;					// 퀘스트 인덱스
	QuestType					mQuestType = QuestType::MAX;						// QuestType
	QuestState					mState = QuestState::NONE;							// 퀘스트 상태
	QuestUpdateObjectiveData	mObjectiveDataList[MAX_QUEST_OBJECTIVE_COUNT] = {};	// QuestObjective
};
#endif // _SERVER

// 클라에서 퀘스트 달성(갱신) 요청할때 사용하는 구조체
struct QuestPreocessData
{
	QuestId				mQuestId = INVALID_QUEST_ID;
	QuestObjectiveId	mQuestObjectiveId = INVALID_QUEST_OBJECTIVE_ID;
	QuestIndex			mQuestIndex = INVALID_QUEST_INDEX;


public:
	QuestPreocessData() = default;
	explicit QuestPreocessData(
		QuestId inQuestId, QuestObjectiveId InObjectiveId, QuestIndex inQuestIndex = INVALID_QUEST_INDEX
	)
		:
		mQuestId(inQuestId),
		mQuestObjectiveId(InObjectiveId),
		mQuestIndex(inQuestIndex)
	{
	}
};

// QuestStory, QuestId Pair
struct QuestStoryIdPair
{
	StoryId	mStoryId = INVALID_STORY_ID;			// 스토리ID
	QuestId	mQuestId = INVALID_QUEST_ID;			// 퀘스트ID
	QuestIndex mQuestIndex = INVALID_QUEST_INDEX;	// 퀘스트 인덱스


public:
	QuestStoryIdPair() = default;
	explicit QuestStoryIdPair(StoryId storyId, QuestId questId, QuestIndex questIndex)
		:
		mStoryId(storyId),
		mQuestId(questId),
		mQuestIndex(questIndex)
	{
	}

	bool operator<(const QuestStoryIdPair& rhs) const
	{
		if (mQuestIndex != rhs.mQuestIndex)
			return (mQuestIndex < rhs.mQuestIndex);

		if (mStoryId != rhs.mStoryId)
			return (mStoryId < rhs.mStoryId);

		return (mQuestId < rhs.mQuestId);
	}

	bool operator == (const QuestStoryIdPair& rhs)const noexcept
	{
		return (this->mQuestId == rhs.mQuestId) &&
			(this->mStoryId == rhs.mStoryId) &&
			(this->mQuestIndex == rhs.mQuestIndex);
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 퀵슬롯 관련 구조체
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct QuickSlotInfo
{
	ItemDBId mItemDBId = INVALID_ITEM_DB_ID;
	ItemId mTemplateId = INVALID_ITEM_ID;
	bool mAutoUse = false;

	QuickSlotInfo() = default;
	explicit QuickSlotInfo(const ItemDBId itemDBId, const ItemId templateId, bool autoUse) noexcept
		:
		mItemDBId(itemDBId),
		mTemplateId(templateId),
		mAutoUse(autoUse)
	{
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Npc Shop 관련 구조체
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//재화 별 소비 수치 정보
struct CurrencyDeltaPair
{
public:
	CurrencyType	mCurrencyType = CurrencyType::NONE;
	CurrencyDelta	mCurrencyDelta = 0;

public:
	CurrencyDeltaPair() = default;
	explicit CurrencyDeltaPair(CurrencyType type, CurrencyDelta delta)
		:
		mCurrencyType(type),
		mCurrencyDelta(delta)
	{
	}
};

//재화 별 재화 수치 정보
struct CurrencyPair
{
public:
	CurrencyType	mCurrencyType = CurrencyType::NONE;
	Currency		mCurrency = 0;

public:
	CurrencyPair() = default;
	explicit CurrencyPair(CurrencyType type, Currency currency)
		:
		mCurrencyType(type),
		mCurrency(currency)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fairy
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 페어리 정보
#ifdef _SERVER
struct FairyData : public Disposable
#else // _SERVER
struct FairyData
#endif // _SERVER
{
	FairyId		mFairyId = INVALID_FAIRY_ID;	// 페어리 템플릿 정보
	Level		mLevel = INVALID_LEVEL;			// 페어리 레벨 정보
	Level		mHighestLevel = INVALID_LEVEL;	// 페어리 최대 달성 레벨 정보
	ItemAmount	mAmount = MIN_ITEM_AMOUNT;		// 페어리 보유 개수
	PresetValue	mPresetValue = 0;				// 페어리 프리셋 정보

	FairyData() = default;
	explicit FairyData(
		FairyId id, Level level, Level highestLevel, ItemAmount amount,
		PresetValue	presetValue
	)
		:
		mFairyId(id),
		mLevel(level),
		mHighestLevel(highestLevel),
		mAmount(amount),
		mPresetValue(presetValue)
	{
	}
};

// 페어리 Datasheet Template ID와 갯수
struct FairyIdAmountPair
{
public:
	FairyId		mFairyId = INVALID_FAIRY_ID;
	ItemAmount	mAmount = 0;


public:
	FairyIdAmountPair() = default;
	explicit FairyIdAmountPair(FairyId id, ItemAmount amount)
		:
		mFairyId(id),
		mAmount(amount)
	{
	}
};

#ifdef _SERVER
using HistoryFairyIdArray = std::xarray<CostumeId, MAX_FAIRY_CONFIRM_CHANGE_COUNT>;
#endif

// 페어리 확정 대기 정보
struct FairyConfirmData
{
public:
	FairyConfirmId			mFairyConfirmId = INVALID_FAIRY_CONFIRM_ID;					// 소환 대기 페어리 고유 정보 (DB에서 Id 발급)
	FairyId					mFairyId = INVALID_FAIRY_ID;								// 소환 대기 페어리 정보
	FairyProbabilityId		mFairyProbabilityId = INVALID_FAIRY_PROBABILITY_ID;			// 해당 소환 대기 건이 소환 교체 시 참조할 소환 교체 관련 정보 Id
	uint8					mRemainChangeCount = 0;										// 잔여 교체 횟수
	uint64					mRemainTime = 0;											// 잔여 시간(초)
	CostumeId				mHistoryFairyIdArray[MAX_FAIRY_CONFIRM_CHANGE_COUNT] = { 0, };	// 페어리 교체 이력


public:
	FairyConfirmData() = default;
	explicit FairyConfirmData(
		FairyConfirmId fairyConfirmId,
		FairyId fairyId,
		FairyProbabilityId fairyProbabilityId,
		uint8 remainChangeCount,
		uint64 remainTime
	)
		:
		mFairyConfirmId(fairyConfirmId),
		mFairyId(fairyId),
		mFairyProbabilityId(fairyProbabilityId),
		mRemainChangeCount(remainChangeCount),
		mRemainTime(remainTime)
	{
	}

#ifdef _SERVER
	explicit FairyConfirmData(
		FairyConfirmId fairyConfirmId,
		FairyId fairyId,
		FairyProbabilityId fairyProbabilityId,
		uint8 remainChangeCount,
		const HistoryFairyIdArray& historyFairyIdArray,
		uint64 remainTime
	)
		:
		mFairyConfirmId(fairyConfirmId),
		mFairyId(fairyId),
		mFairyProbabilityId(fairyProbabilityId),
		mRemainChangeCount(remainChangeCount),
		mRemainTime(remainTime)
	{
		uint8 index = 0;
		for (const FairyId& historyFairyId : historyFairyIdArray)
			mHistoryFairyIdArray[index++] = historyFairyId;
	}
#endif
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Costume
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 코스튬 정보
#ifdef _SERVER
struct CostumeData : public Disposable
#else // _SERVER
struct CostumeData
#endif // _SERVER
{
	CostumeId	mCostumeId = INVALID_COSTUME_ID;	// 코스튬 템플릿 정보
	ItemAmount	mAmount = MIN_ITEM_AMOUNT;			// 코스튭 보유 개수 : 0이면, 보유한 것 1이상은 합성의 재료로 소모
	PresetValue	mPresetValue = 0;
	PresetValue mPresetValueAutoUse = 0;			// 서버에서만 사용하는 변수.
	PresetValue mPresetValueSkillEquip = 0;			// 코스튬이 장착된 프리셋 번호 비트 조합(서버에서만 사용하는 변수.)
	bool		mIsLearnSkill = false;				// 스킬 습득 상태
	bool		mIsActivity = true;					// 활성 여부(false 라면 없는 것으로 취급)(서버에서만 사용하는 변수.)
	bool		mIsUseSkillInterval = false;		// 스킬 재사용 주기 설정 사용 여부 (서버에서만 사용하는 변수.)
	bool		mIsUseSkillIntervalInPVP = false;	// PVP 시, 스킬 재사용 주기 설정 사용 여부 (서버에서만 사용하는 변수.)
	int			mUseSkillIntervalSecond = 0;		// 재사용 시간(초 단위) (서버에서만 사용하는 변수.)


	CostumeData() = default;
	explicit CostumeData(
		CostumeId id, ItemAmount amount,
		PresetValue presetValue, PresetValue presetValueAutoUse, PresetValue presetValueSkillEquip
	)
		:
		mCostumeId(id),
		mAmount(amount),
		mPresetValue(presetValue),
		mPresetValueAutoUse(presetValueAutoUse),
		mPresetValueSkillEquip(presetValueSkillEquip)
	{

	}
};

// 코스튬 Id, amount 정보
struct CostumeIdAmountPair
{
public:
	CostumeId	mCostumeId = INVALID_COSTUME_ID;
	ItemAmount	mAmount = 0;


public:
	CostumeIdAmountPair() = default;
	explicit CostumeIdAmountPair(CostumeId id, ItemAmount amount)
		:
		mCostumeId(id),
		mAmount(amount)
	{
	}
};

#ifdef _SERVER
using HistoryCostumeIdArray = std::xarray<CostumeId, MAX_COSTUME_CONFIRM_CHANGE_COUNT>;
#endif

// 코스튬 확정 대기 정보
struct CostumeConfirmData
{
public:
	CostumeConfirmId		mCostumeConfirmId = INVALID_COSTUME_CONFIRM_ID;			// 소환 대기 코스튬 고유 정보 (DB에서 Id 발급)
	CostumeId				mCostumeId = INVALID_COSTUME_ID;						// 소환 대기 코스튬 정보
	CostumeProbabilityId	mCostumeProbabilityId = INVALID_COSTUME_PROBABILITY_ID;	// 해당 소환 대기 건이 소환 교체 시 참조할 소환 교체 관련 정보 Id
	uint8					mRemainChangeCount = 0;									// 잔여 교체 횟수
	uint64					mRemainTime = 0;										// 잔여 시간(초)
	CostumeId				mHistoryCostumeIdArray[MAX_COSTUME_CONFIRM_CHANGE_COUNT] = { 0, };	// 코스튬 교체 이력


public:
	CostumeConfirmData() = default;
	explicit CostumeConfirmData(
		CostumeConfirmId costumeConfirmId,
		CostumeId costumeId,
		CostumeProbabilityId costumeProbabilityId,
		uint8 remainChangeCount, uint64 remainTime
	)
		:
		mCostumeConfirmId(costumeConfirmId),
		mCostumeId(costumeId),
		mCostumeProbabilityId(costumeProbabilityId),
		mRemainChangeCount(remainChangeCount),
		mRemainTime(remainTime)
	{
	}

#ifdef _SERVER
	explicit CostumeConfirmData(
		CostumeConfirmId costumeConfirmId,
		CostumeId costumeId,
		CostumeProbabilityId costumeProbabilityId,
		uint8 remainChangeCount,
		const HistoryCostumeIdArray& historyCostumeIdArray,
		uint64 remainTime
	)
		:
		mCostumeConfirmId(costumeConfirmId),
		mCostumeId(costumeId),
		mCostumeProbabilityId(costumeProbabilityId),
		mRemainChangeCount(remainChangeCount),
		mRemainTime(remainTime)
	{
		uint8 index = 0;
		for (const CostumeId& historyCostumeId : historyCostumeIdArray)
			mHistoryCostumeIdArray[index++] = historyCostumeId;
	}
#endif
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Shop
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// NPCSHOP 아이템 구매정보
// 구매 ShopGroupID , 아이템 ID, 갯수
struct NpcShopItemBuyPair
{
	NpcShopGroupId	mnpcShopGroupId = INVALID_NPCSHOPGROUP_ID;
	ItemId			mItemId = INVALID_ITEM_DB_ID;
	ItemAmount		mItemAmount = 0;

public:
	NpcShopItemBuyPair() = default;
	explicit NpcShopItemBuyPair(NpcShopGroupId groupId, ItemId itemId, ItemAmount amount)
		:
		mnpcShopGroupId(groupId),
		mItemId(itemId),
		mItemAmount(amount)
	{
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 클라이언트 옵션
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
using QuestSubHudShowInfoArray = std::xarray<StoryId, MAX_ACTIVE_QUEST_SUB_COUNT>;
using QuestRepeatHudShowInfoArray = std::xarray<QuestIndex, MAX_ACTIVE_QUEST_REPEAT_COUNT>;
using QuestGuideHudShowInfoArray = std::xarray<StoryId, MAX_QUEST_GUIDE_UI_VIEW_OFF_COUNT>;
using QuestGuideUserSelectHudHideInfoArray = std::xarray<StoryId, MAX_QUEST_GUIDE_UI_VIEW_OFF_COUNT>;
using QuestBoardHudSortInfoArray = std::xarray<QuestStoryIdPair, MAX_ACTIVE_BOARD_QUEST_COUNT>;

struct ClientOptionData : Disposable
{
	QuestSubHudShowInfoArray				mSubQuestHudShowInfoList = { 0 };
	QuestRepeatHudShowInfoArray				mRepeatQuestHudShowInfoList = { 0 };
	QuestGuideHudShowInfoArray				mGuideQuestHudShowInfoList = { 0 };
	QuestGuideUserSelectHudHideInfoArray	mGuideQuestUserSelectHudHideInfoList = { 0 };
	QuestBoardHudSortInfoArray				mQuestBoardHudSortInfoArray = { QuestStoryIdPair{}, };
	// 추가 시 서버 GatewayServer 에서 DB에 input, output 작업 필요
};

#else // _SERVER
struct ClientOptionData
{
	StoryId				mSubQuestHudShowInfoList[MAX_ACTIVE_QUEST_SUB_COUNT] = { 0 };
	QuestIndex			mRepeatQuestHudShowInfoList[MAX_ACTIVE_QUEST_REPEAT_COUNT] = { 0 };
	StoryId				mGuideQuestHudShowInfoList[MAX_QUEST_GUIDE_UI_VIEW_OFF_COUNT] = { 0 };
	StoryId				mGuideQuestUserSelectHudHideInfoList[MAX_QUEST_GUIDE_UI_VIEW_OFF_COUNT] = { 0 };
	QuestStoryIdPair	mQuestBoardHudSortInfoArray[MAX_ACTIVE_BOARD_QUEST_COUNT] = { QuestStoryIdPair{} };
	// 추가 시 서버 GatewayServer 에서 DB에 input, output 작업 필요
};
#endif // _SERVER

// New 클라이언트 옵션
using UsePotionHpRate = uint8;

struct AutoUsePotionInfo
{
	UsePotionHpRate mUsePotionHpRate = 70;
	bool mIsAutoUsePotion = true;
};

struct ManualCombat
{
	bool mCounterAttack = false;	// 수동전투시 자동 반견
	bool mAutoSkill = true;		// 수동전투시 자동 스킬 사용
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 전투 옵션 관련 정보 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
using CombatTargetOptionArray = std::xarray<int32_t, MAX_COMBAT_OPTION_COUNT>;

struct CombatOptionInfo : Disposable
{
	CombatTargetOptionArray	mCombatTargetOptionList = { 0 };

	CombatTargetOptionArray	mAutoCombatTargetOptionList = { 0 };
	CombatTargetOptionArray	mManualCombatTargetOptionList = { 0 };

	AutoAttackRange			mAutoAttackRange = AutoAttackRange::METER_50;  // 자동 전투 범위
	AutoDropItemGrade		mAutoDropItemGrade = AutoDropItemGrade::ALL;	// 자동 루팅 설정. 장비 아이템에 한하여 적용
	int						mAutoDropItemWeight = 0;    // 아이템 자동 획득 무게 설정
	bool					mOtherPlayersDropItem = false;   // 다른 플레이어가 드랍한 아이템 획득 설정 
	int						mAutoSkillRate = 0;		//자동 스킬 사용 설정	자동 스킬 사용 MP 설정
	bool					mIsMpUseSkillSkill = false;  //지속형 스킬 사용 설정	지속형 스킬 상시 사용 설정	버튼(토글)
	bool					mAutoPartyAccept = false;   //자동 파티 수락	자동 파티 수락 설정
	ManualCombat			mManualCombat;

	MannerMode				mMannerMode = MannerMode::OFF;

	
	std::xwstring CombatTargetOptionListToString()
	{
		int idx = 0;
		std::wostringstream wosStr;
		for (int32_t option : mAutoCombatTargetOptionList)
		{
			// 비어있지 않으면, 콤마 추가
			//if (false == wosStr.str().empty())
			if (0 != wosStr.tellp())
				wosStr << L", ";

			wosStr << L"[" << idx << L": " << option << L"]";

			++idx;
		}
		wosStr.flush();
		return wosStr.str().c_str();
	}
};
#else // _SERVER
struct CombatOptionInfo
{
	int32_t					mCombatTargetOptionList[MAX_COMBAT_OPTION_COUNT] = {};
	int32_t					mAutoCombatTargetOptionList[MAX_COMBAT_OPTION_COUNT] = {};
	int32_t					mManualCombatTargetOptionList[MAX_COMBAT_OPTION_COUNT] = {};

	AutoAttackRange		mAutoAttackRange = AutoAttackRange::METER_50;  // 자동 전투 범위
	AutoDropItemGrade	mAutoDropItemGrade = AutoDropItemGrade::ALL;
	int					mAutoDropItemWeight = 0;    // 아이템 자동 획득 무게 설정
	bool				mOtherPlayersDropItem = false;   // 다른 플레이어가 드랍한 아이템 획득 설정 
	int					mAutoSkillRate = 0;		//자동 스킬 사용 설정	자동 스킬 사용 MP 설정
	bool				mIsMpUseSkillSkill = false;  //지속형 스킬 사용 설정	지속형 스킬 상시 사용 설정	버튼(토글)
	bool				mAutoPartyAccept = false;   //자동 파티 수락	자동 파티 수락 설정	
	ManualCombat		mManualCombat;
	MannerMode				mMannerMode = MannerMode::OFF;

};
#endif // _SERVER
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 오프라인 옵션 관련 정보 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct OffLineOptionInfo
{
	bool	mAutoPlay = false;				// 클라이언트 종료 시 오프라인 플레이 자동 진행	
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 알림 설정
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//HP 자동 물약 부족 알림
struct AutoPotionLowNotification
{
	uint8 mAlertCount = 10;
	bool mSoundOn = true;
	bool mVibrationOn = true;
};

//자동 사냥 중 다른 플레이어에게 피격 알림
struct AutoKillTakeDamageNotification
{
	bool mSoundOn = true;
	bool mVibrationOn = true;
};

//생명력 위험
struct HPDangerousNotification
{
	uint8 mAlertPercent = 30;
	bool mSoundOn = true;
	bool mVibrationOn = true;
};

//무게 알림
struct WeightNotification
{
	uint8 mAlertPercent = 70;
	bool mSoundOn = true;
	bool mVibrationOn = true;
};

//사망 알림
struct DeadNotification
{
	bool mSoundOn = true;
	bool mVibrationOn = true;
};



//게임 편의 알림(티커나 사운드 클라 알림)
struct ClientNotification
{
	bool							mCombatRepeat = true;				// 전투 반복 알림
	int								mCombatTimePeriodms = 60000;		// 전투 알람 시간 조절(ms 단위)
	bool							mRepeat = true;						// 게임 편의 알림 on/off
	int								mTimePeriodms = 1800000;			// 알람 시간 조절(ms 단위)
	AutoPotionLowNotification		mAutoPotionLowNotification;			// HP 자동 물약 부족 알림
	AutoKillTakeDamageNotification	mAutoKillTakeDamageNotification;	// 자동 사냥 중 다른 플레이어에게 피격 알림
	HPDangerousNotification			mHPDangerousNotification;			// 생명력 위험
	WeightNotification				mWeightNotification;				// 무게 알림
	DeadNotification				mDeadNotification;					// 사망 알림
};

//접속 알림
struct LoginAlram
{
	bool	mLoginAlramOn = true;
	bool	mBuddyLoginOn = true;				//친구 접속 알림
	bool	mBoundaryTargetLoginOn = true;		//경계 대상 접속 알림
	bool	mFriendShipTargetLoginOn = true;	//우호 대상 접속 알림
	bool	mGuildMemberLoginOn = true;			//기사 단원 접속 알림
};

//서버 푸시 알림(대체로 오프라인 플레이시)
struct HivePushNotification
{
	bool	mRepeat = true;					// 알림 전체 on/off
	int		mTimePeriodms = 1800000;		// 알람 시간 조절(ms단위)
	bool	mFullInventory = true;			// 인벤토리에 빈 슬롯이 없을 때
	bool	mWeightOver = true;				// 무게 알림여부
	uint8	mWeightOverPercent = 70;		// 무게 초과
	bool	mDead = true;					// 사망시
	bool	mFinishDungeon = true;			// 던전 제한 시간 종료
	bool	mFinishOfflinePlay = true;		// 오프라인 종료
	bool	mTakeDamageByPVP = true;		// 타유저에게 피격당할 때

	AutoDropItemGrade mNotiItemGrade = AutoDropItemGrade::RARE; // 해당 등급 이상 장비 아이템 알림 설정
	AutoDropItemGrade mNotiSkillBookGrade = AutoDropItemGrade::RARE; // 해당 등급 이상 스킬북 아이템 알림 설정

	bool	mMarketItemSell = true;				// 거래소 판매
};

// 드랍아이템 획득 시 상황 및 종류 별 자동분해할지 여부에 대한 GameOption
struct AutoDecompose
{
	bool mIsFieldBossDropItem = false;				// 필드보스 보상으로 인한 획득 아이템 자동분해 (기사단으로 들어가는 것 제외)
	bool mIsItemCollectAble = false;				// 아이템 컬렉션이 가능해도, 자동분해 할지 여부 : 서버) ItemTaskCreate::IsAutoDecompose - 추후 업데이트 여지가 있고, ItemTaskCreate 에 주석되서 사용 안함.
	//
	bool mIsEquipNormal = false;					// 장비아이템 ItemGrade::NORMAL
	bool mIsEquipMagic = false;						// 장비아이템 ItemGrade::MAGIC
	bool mIsEquipRare = false;						// 장비아이템 ItemGrade::RARE
	//
	bool mIsSkillBookNormal = false;				// 스킬북 ItemGrade::NORMAL
	bool mIsSkillBookMagic = false;					// 스킬북 ItemGrade::MAGIC
	bool mIsSkillBookRare = false;					// 스킬북 ItemGrade::RARE
	//
	bool mIsMonsterKnowwledgeSlateNormal = false;	// 석판 ItemGrade::NORMAL
	bool mIsMonsterKnowwledgeSlateMagic = false;	// 석판 ItemGrade::MAGIC
	bool mIsMonsterKnowwledgeSlateRare = false;		// 석판 ItemGrade::RARE
};

struct NotificationInfo
{
	AutoPotionLowNotification		mAutoPotionLowNotification;			//HP 자동 물약 부족 알림
	AutoKillTakeDamageNotification	mAutoKillTakeDamageNotification;	//자동 사냥 중 다른 플레이어에게 피격 알림
	HPDangerousNotification			mHPDangerousNotification;			//생명력 위험
	WeightNotification				mWeightNotification;				//무게 알림
	DeadNotification				mDeadNotification;					//사망 알림
};

struct SocialMediaInfo  // 정보 공개
{
	bool mNormal = true;
	bool mGuild = true;
	bool mFriend = true;
	bool mParty = true;
};

struct WorldMapFilterInfo
{
	bool mParty = true;
	bool mTeleportSpot = true;
	bool mBookMark = true;
	bool mSpaceCrackPortal = true;
	bool mFieldBoss = true;
	bool mSanctum = true;
};

// 레이다 옵션.
//  - 서버에서 사용하지 않아, 클라에서 사용하던 형식과 변수 이름에 맞추어 단순 저장하도록 작업.
//  - https://jira.com2us.com/wiki/pages/resumedraft.action?draftId=300557222&draftShareId=0c4dd537-67b1-4bf7-94c7-a897bfc17f99& 
struct RadarOptionInfo
{
	uint32 mDetectOption = (0x1 | 0x2 | 0x4 | 0x8 | 0x10 | 0x20 | 0x40 | 0x80 | 0x100 | 0x200 | 0x400 | 0x800 | 0x1000);
	uint32 mDetectGroupOption = 15;
	uint32 mAutoScan = 0;
	uint32 mScanTimerSecond = 0;
};

//경계대상 알림
struct MonitoringTargetNotification
{
	bool	mSoundOn = true;					// 소리 알림
	bool	mVibrationOn = true;				// 진동 알림
	uint8	mNotificationRange = 30;			// 알림 거리
	uint8	mNotificationCycleSecond = 1;		// 알림 주기
};



// 체크 박스 형식
struct OptionSelectTypeInfo
{
	bool			mItemGainCommonConsume = true;  //일반 소모품의 습득을
	bool			mItemGainSpecialConsume = true; //특수 소모품의 습득을
	bool			mItemGainCommonScroll = true;   //일반 주문서의 습득을
	bool			mItemGainSpecialScroll = true;  //특수 주문서의 습득을
	bool			mItemGainEnchantScroll = true;  //강화 주문서의 습득을

	//게임 옵션 아이템 습득 제어 	
	bool			mItemGainOtherNormal = true;  //일반
	bool			mItemGainOtherSpecial = true; //SPECIAL
	bool			mItemGainOtherEtc = true;  //ETC


};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 게임옵션 정보
//  - 현재는 캐릭터 생성 시 서버에서 기본 데이터를 생성하여 보내주지만 보내주지 않도록 최적화 하는 방안 검토 중.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct GameOption
{
	AutoUsePotionInfo				mAutoUsePotionInfo;						//
	CombatOptionInfo				mCombatOptionInfo;						// 
	bool							mSpiritShotOnOff = false;				// 천마석(정령탄) ON/OFF 여부
	bool							mViewHelmet = true;						// 투구를 보일 것인가?	
	OffLineOptionInfo				mOffLineOptionInfo;						// 오프라인 정보
	SocialMediaInfo					mSocialMediaInfo;						// 정보 공개 대상 
	WorldMapFilterInfo				mWorldMapFilterInfo;					// 월드 맙 필터 정보	
	RadarOptionInfo					mRadarOptionInfo;						// 레이다 옵션
	ClientNotification				mClientNotification;					// 게임 편의 알림(티커나 사운드 알림)
	HivePushNotification			mHivePushNotification;					// 서버 푸시 알림(대체로 오프라인 플레이시)
	bool							mHiddenNameGetItem = false;				// 글로벌 메세지에 이름 숨김	
	LoginAlram						mLoginAlram;							//접속 알림
	bool							mPersonalTradingAutoReject = false;		//개인거래 자동 거절 옵션
	bool							mIgnoreCinematicRewardPopup = false;	//시청 보상 팝업 안뜨는 옵션

	MonitoringTargetNotification	mMonitoringTargetNotification;			// 경계대상 알림
	OptionSelectTypeInfo			mOptionSelectTypeInfo;					//획득 가능 아이템 분류	

	bool							mEnableAutoQuestTeleport = false;		// 
	uint8							mPcRvo = 1;								// 유저간 회피 기능 추가

	AutoDecompose					mAutoDecompose;							// 드랍아이템 획득 시 상황 및 종류 별 자동분해할지 여부
	bool							mUseSlotExpansion = false;				// 퀵슬롯 확장 여부(on / off)
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 캠프 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
struct CampPosition : public Disposable
#else
struct CampPosition
#endif 
{
	float		mX, mY, mZ;
	int16		mDir = 0;
};

#ifdef _SERVER
struct GuildCampInfo : public Disposable
#else
struct GuildCampInfo
#endif 
{
	CampPosition		mCampPosition;
	GuildDBId			mGuildDBId = INVALID_GUILD_DB_ID;
	MapId				mMapId = INVALID_MAP_ID;
	ChannelId			mChannelId = INVALID_CHANNEL_ID;
	CampId				mCampId = INVALID_CAMP_ID;
	StatValue			mCurrentHp = 0;
	GuildCampStateType  mGuildCampStateType = GuildCampStateType::NONE;
	time_t				mStartDateTime = 0;
	time_t				mEndDateTime = 0;
	time_t				mRemainingTime = 0;	// 남은 시간 초 단위로 설정 
	time_t				mLifeTime = 0;
	GuildBuildingLevel  mLevel = 1;   // 설치 했을때 레벨
	PlanetWorldId		mPlanetWorldId = 0;

	void Clear()
	{
		mGuildDBId = INVALID_GUILD_DB_ID;
		mMapId = INVALID_MAP_ID;
		mChannelId = INVALID_CHANNEL_ID;
		mCampId = INVALID_CAMP_ID;
		mCurrentHp = 0;
		mGuildCampStateType = GuildCampStateType::NONE;
		mStartDateTime = 0;
		mEndDateTime = 0;
		mRemainingTime = 0;	// 남은 시간 초 단위로 설정 
		mLifeTime = 0;
		mLevel = 1;
		mPlanetWorldId = 0;
	}
};

#ifdef _SERVER
struct SanctumNexusInfo : public Disposable
#else
struct SanctumNexusInfo
#endif 
{
	SanctumAreaId		mSanctumAreaId = INVALID_SANCTUM_AREA_ID;
	GuildDBId			mGuildDBId = INVALID_GUILD_DB_ID;
	MapId				mMapId = INVALID_MAP_ID;
	SanctumNexusId		mSanctumNexusId = INVALID_SANCTUM_NEXUS_ID;
	StatValue			mCurrentHp = 0;
	GuildCampStateType  mGuildCampStateType = GuildCampStateType::NONE;
	time_t				mStartDateTime = 0;	
	GuildBuildingLevel  mLevel = 1;   // 설치 했을때 레벨
	PlanetWorldId		mPlanetWorldId = 0;

	void Clear()
	{
		mSanctumAreaId = INVALID_SANCTUM_AREA_ID;
		mGuildDBId = INVALID_GUILD_DB_ID;
		mSanctumNexusId = INVALID_SANCTUM_NEXUS_ID;
		mCurrentHp = 0;
		mGuildCampStateType = GuildCampStateType::NONE;
		mStartDateTime = 0;		
		mLevel = 1;
		mPlanetWorldId = 0;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 사망 경험치 패널티  정보
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
struct DeathPenaltyExp : public Disposable
#else // _SERVER
struct DeathPenaltyExp
#endif // _SERVER
{
public:
	DeathPenaltyId mDeathPenaltyId = 0;		// 사망 패널티 고유 번호
	Exp			   mExp = 0;				// 손실된 경험치	
	Exp			   mCurrentExp = 0;			// 손실된을때 나의 경험치
	Level		   mLevel = 0;				// 발생 했을때 레벨	
	CurrencyType   mCurrencyType = CurrencyType::NONE;   // 손실 재화 Type
	int32		   mCurrencyValue = 0;					// 손실한 재화 수
	time_t		   mExpireTime = 0;						// 만료 시간 
	DeathId		   mDeathId = 0;
	int64_t		   mRemainingTime = 0;			// 남은 시간 초 단위로 설정 


public:
	DeathPenaltyExp() = default;
	explicit DeathPenaltyExp(DeathPenaltyId deathPenaltyId, Level level, Exp exp, Exp currentExp,
		CurrencyType currencyType, int32 currencyValue, time_t expireTime, DeathId deathId)
		:
		mDeathPenaltyId(deathPenaltyId),
		mExp(exp),
		mCurrentExp(currentExp),
		mLevel(level),
		mCurrencyType(currencyType),
		mCurrencyValue(currencyValue),
		mExpireTime(expireTime),
		mDeathId(deathId)
	{
	}

	float GetExpPercentValue()
	{
		float result = static_cast<float>((float)mExp / (float)mCurrentExp * 100);

		return result;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 사망 아이템 패널티  정보
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
struct DeathPenaltyItem : public Disposable
#else // _SERVER
struct DeathPenaltyItem
#endif // _SERVER
{
public:
	DeathPenaltyId mDeathPenaltyId = 0;		// 사망 패널티 고유 번호
	time_t		   mExpireTime;				// 만료 시간 
	ItemDataEquip  mDeathPenaltyItem;		// 장착 아이템 패널티 정보
	DeathId		   mDeathId = 0;			// 사망 패널티에 발생된 번호
	int64_t		   mRemainingTime = 0;		// 남은 시간 초 단위로 설정 

public:
	DeathPenaltyItem() = default;
	explicit DeathPenaltyItem(DeathPenaltyId deathPenaltyId, time_t expireTime, DeathId deathId)
		:
		mDeathPenaltyId(deathPenaltyId),
		mExpireTime(expireTime),
		mDeathId(deathId)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 사망 아이템 복구  정보
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
struct DeathPenaltyRestore : public Disposable
#else // _SERVER
struct DeathPenaltyRestore
#endif // _SERVER
{
public:
	DeathPenaltyId mDeathPenaltyId = 0;
	CurrencyType   mCurrencyType;


public:
	DeathPenaltyRestore() = default;
	explicit DeathPenaltyRestore(DeathPenaltyId deathPenaltyId, CurrencyType currencyType)
		:
		mDeathPenaltyId(deathPenaltyId),
		mCurrencyType(currencyType)
	{

	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Guild Member
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
struct GuildMemberRightsbyGrade : public Disposable
#else
struct GuildMemberRightsbyGrade
#endif // _SERVER
{
public:
	GuildMemberGradeType mMemberGradeType = GuildMemberGradeType::UNKNOWN;
	GuildRightsLevel	 mMemeberRights = GUILD_RIGHTS_LEVEL_NONE;
public:
	GuildMemberRightsbyGrade() = default;
	explicit GuildMemberRightsbyGrade(GuildMemberGradeType memberGradeType, GuildRightsLevel  memberRights)
		:
		mMemberGradeType(memberGradeType),
		mMemeberRights(memberRights)
	{}
};


// 길드 스킬  정보 
#ifdef _SERVER
struct GuildPassivity : public Disposable
#else
struct GuildPassivity
#endif // _SERVER
{
public:
	GuildDBId				mGuildDBId = INVALID_GUILD_DB_ID;
	GuildBuildingId			mBuildingId = INVALID_GUILD_BUILDING_ID;	// 건물 테이블 번호
	PassivityId				mPassivityId = INVALID_SKILL_ID;
	int64_t					mEndTime = 0;								// 만료 시간 
	int64_t					mRemainingTime = 0;							// 남은 시간 초 단위로 설정 (서버만 사용하는것같음)
public:
	GuildPassivity() = default;

	explicit GuildPassivity(GuildBuildingId buildingId, PassivityId  passivityId, int64_t endTime, int64_t remainingTime)
		:mBuildingId(buildingId), mPassivityId(passivityId),
		mEndTime(endTime), mRemainingTime(remainingTime)
	{

	}

	void Clear()
	{
		mBuildingId = INVALID_GUILD_BUILDING_ID;	// 건물 테이블 번호
		mPassivityId = INVALID_SKILL_ID;
		mEndTime = 0;								// 만료 시간 
		mRemainingTime = 0;							// 남은 시간 초 단위로 설정 
	}
};

// 길드 건물 정보 
#ifdef _SERVER
using GuildPassivityArray = std::xarray<GuildPassivity, MAX_GUILD_PASSIVITY_COUNT>;
struct GuildBuildingData : public Disposable
#else
struct GuildBuildingData
#endif // _SERVER
{
public:
	GuildBuildingId			mBuildingId = INVALID_GUILD_BUILDING_ID;			// 건물 테이블 번호
	GuildBuildingLevel		mBuildingLevel = INVALID_GUILD_BUILDING_LEVEL;		// 현제 레벨 
	GuildBuildingStateType	mBuildingStateType;
	int64_t					mEndTime;											// 만료 시간 
	int64_t					mRemainingTime = 0;									// 남은 시간 초 단위로 설정 
	GuildDBId				mGuildDBId = INVALID_USER_DB_ID;					// 길드 고유 번호 
	uint8_t					mGuildPassivityCount = 0;
	int64_t					mLastLevelUpTime = 0;								// 마지막 레벨업 된 시간
#ifdef _SERVER
	GuildPassivityArray		mGuildPassivityArray;
#else
	GuildPassivity			mGuildPassivityArray[MAX_GUILD_PASSIVITY_COUNT] = {};
#endif // _SERVER

public:
	GuildBuildingData() = default;
	explicit GuildBuildingData(GuildBuildingId	buildingId, GuildBuildingLevel  buildingLevel,
		GuildBuildingStateType buildingStateType, int64_t endTime, int64_t remainingTime, uint8_t passivityCount)
		:mBuildingId(buildingId),
		mBuildingLevel(buildingLevel),
		mBuildingStateType(buildingStateType),
		mEndTime(endTime),
		mRemainingTime(remainingTime),
		mGuildPassivityCount(passivityCount)
	{

	}

	void Clear()
	{
		mBuildingId = INVALID_GUILD_BUILDING_ID;
		mBuildingLevel = INVALID_GUILD_BUILDING_LEVEL;  // 현제 레벨 
		mBuildingStateType = GuildBuildingStateType::GUILD_BUILDING_STATE_NONE;
		mEndTime = 0;					// 만료 시간 
		mRemainingTime = 0;			// 남은 시간 초 단위로 설정 
		mGuildDBId = INVALID_USER_DB_ID;   // 길드 고유 번호 
		mGuildPassivityCount = 0;
	}
};

//기사단 기본정보
#ifdef _SERVER
struct GuildDetail : public Disposable
#else
struct GuildDetail
#endif
{
	GuildDBId			mGuildDBId = INVALID_GUILD_DB_ID;			//길드DB ID
	Level				mLimitedUserLevel = 1;						//유저 제한레벨 (defalut:1)
	GuildEmblemId		mGuildEmblemId = 0;							//기사단 문장
	GuildJoinType		mGuildJoinType = GuildJoinType::UNABLE;		//기사단 가입 형태	
	Currency			mGuildAsset = 0;							//길드 운영자금
	time_t				mCreatedTime = 0;							//길드생성시간 
	time_t				mBuffReactivateTime = 0;					//길드 버프 재사용 남은 시간
	SlotExtendNumber	mSlotExtendNumber = 0;						//길드창고 확장 횟수
	GuildLevel			mGuildLevel = 1;
	GuildLevel			mCampLevel = 1;
	GuildDunGeonLevel	mDungeonBestLevel = 0;						//기사단 던전 베스트 레벨
	uint32				mBestDungeonCleartimeSec = 0;				//기사단 던전 베스트 클리어 시간
	GuildZealPoint		mGuildZealPoint = 0;						//열성 포인트
	GuildContribution	mGuildContribution = 0;						//기사단 기여도
	PlanetWorldId		mCurrentHomePlanetWorldId = { 0, };			//현재 기사단이 속한 월드

	time_t				mLastNoticeTime = 0;						//마지막 공지 변경 시간
	time_t				mLastHostileTime = 0;						//적대관련(추가/상태변경)마지막 처리 시간 
	time_t				mLastAllyTime = 0;							//동맹관련(추가/상태변경)마지막 처리 시간
	time_t				mLastAllyRequestTime = 0;					//동맹관련(신청/받음)마지막 처리 시간(삭제시 0)
	time_t				mGuildZealPointResetedTime = 0;				//기사단 퀘스트로 누적된 열성 포인트가 초기화 된 시간
	time_t				mAgitRentalEndDate = 0;							//아지트 대여료 만료시간

	void Init()
	{
		mGuildDBId = INVALID_GUILD_DB_ID;
		mLimitedUserLevel = 1;
		mGuildEmblemId = 0;
		mGuildJoinType = GuildJoinType::UNABLE;		//기사단 가입 형태
		mGuildAsset = 0;
		mCreatedTime = 0;	//길드생성시간 
		mBuffReactivateTime = 0;
		mSlotExtendNumber = 0;
		mGuildLevel = 1;
		mDungeonBestLevel = 0;
		mBestDungeonCleartimeSec = 0;
		mGuildZealPoint = 0;
		mGuildContribution = 0;
		mCurrentHomePlanetWorldId = 0;
	}
};

//기사단 레드닷 정보
struct GuildRedDotDetail
{
	GuildRedDotDetail() = default;
	GuildRedDotDetail(int64_t noticeCheckTime,
		int64_t buffCheckTime,
		int64_t buildingCheckTime,
		int64_t questCheckTime,
		int64_t diplomacyCheckTime,
		//int64_t mEnemyCheckTime= 0;
		//int64_t mAllyCheckTime= 0;
		//int64_t mAllyStateCheckTime= 0;
		int64_t invenCheckTime,
		int64_t acquiredCheckTime,
		int64_t distributedCheckTime,
		int64_t	sanctumCheckTime
	) :
		mNoticeCheckTime(noticeCheckTime),
		mBuffCheckTime(buffCheckTime),
		mBuildingCheckTime(buildingCheckTime),
		mQuestCheckTime(questCheckTime),
		mDiplomacyCheckTime(diplomacyCheckTime),
		mInvenCheckTime(invenCheckTime),
		mAcquiredCheckTime(acquiredCheckTime),
		mDistributedCheckTime(distributedCheckTime),
		mSanctumCheckTime(sanctumCheckTime)
	{
	}
	void Init()
	{
		mNoticeCheckTime = 0;
		mBuffCheckTime = 0;
		mBuildingCheckTime = 0;

		mQuestCheckTime = 0;

		mDiplomacyCheckTime = 0;
		/*mEnemyCheckTime= 0;
		mAllyCheckTime= 0;
		mAllyStateCheckTime= 0;*/

		mInvenCheckTime = 0;
		mAcquiredCheckTime = 0;
		mDistributedCheckTime = 0;
		mSanctumCheckTime = 0;

	}

	int64_t mNoticeCheckTime = 0;
	int64_t mBuffCheckTime = 0;
	int64_t mBuildingCheckTime = 0;

	int64_t mQuestCheckTime = 0;

	// 메인탭이 있어서 그냥 묶어서 써도 될 것 같기도?
	int64_t mDiplomacyCheckTime = 0;
	//int64_t mEnemyCheckTime= 0;
	//int64_t mAllyCheckTime= 0;
	//int64_t mAllyStateCheckTime= 0;

	int64_t mInvenCheckTime = 0;
	int64_t mAcquiredCheckTime = 0;
	int64_t mDistributedCheckTime = 0;
	int64_t mSanctumCheckTime = 0;
	//추가 시 GetJsonValueGuildRedDotDetailInfo(), GuildUtil::LoadJsonInfo에도 로딩 부분 추가
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IFF 를 구성하기 위해서 필요한 Class
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct IFFSanctumType
{
	bool operator == (const IFFSanctumType& rhs)const noexcept
	{
		return (this->value == rhs.value);
	}

	bool operator != (const IFFSanctumType& rhs)const noexcept
	{
		return (this->value != rhs.value);
	}

	union
	{
		bool value = false;
		struct
		{
			bool isSanctumNexus : 1;
			bool canBattlSanctum : 1;
		} st;
	};

	IFFSanctumType() = default;
	IFFSanctumType(bool _value)
	{
		value = _value;
	}
	IFFSanctumType(bool isSanctumNexus, bool canBattlSanctum)
	{
		st.isSanctumNexus = isSanctumNexus;
		st.canBattlSanctum = canBattlSanctum;
	}
};

struct IffFactionActor
{
	PartyId				mPartyId = 0;												// 파티 번호
	IffFactionType		mIffFactionType = IffFactionType::MAX;						// 종류
	UserDBId			mUserDBId = 0;												// 유저 정보
	GuildDBId			mGuildId = 0;												// 길드 번호
	PVPAlignmentGrade	mPVPAlignmentGrade = PVPAlignmentGrade::NETURAL;			// PVP 등급 
	GuildDBId			mEnemyGuildArray[MAX_DIPLOMACY_GUILDS_COUNT] = { 0 };			//
	GuildDBId			mAllianceGuildArray[MAX_DIPLOMACY_GUILDS_COUNT] = { 0 };	//
	int64				mTreasureMonsterGameId = 0;									// 보물 몬스터
	int64				mGameId = 0;												// 나의 게임 ID
	int32_t				mCombatTargetOptionList[MAX_COMBAT_OPTION_COUNT] = {};		//
	bool				mCanPK = true;												// PK 를 할수 있는지 판단 
	PlanetWorldId		mPlanetWorldId;												//
	ArenaId				mArenaId = INVALID_ARENA_ID;								// 진행중인 결투장 ID
	ArenaTeamId			mArenaTeam = INVALID_ARENA_TEAM_ID;							// 결투장의 팀 번호
	bool				mEnable = true;
	bool				mEnemyFactionTypeArray[static_cast<uint8_t>(IffCheckType::MAX)][static_cast<uint8_t>(IffFactionType::MAX)] = { {false,}, };
	TerritoryType		mTerritoryType = TerritoryType::NONE;						// 테리토리 정보
	bool				mCanPVP = true;												// PVP 를 할수 있는지 판단
	bool				mCanPVE = true;												// PVE 를 할수 있는지 판단
	bool				mCanEVE = true;												// EVE 를 할수 있는지 판단
	UserDBId			mMonitoringUserDBIdArray[MAX_COUNT_OF_DETECTION_TARGET_COUNT] = { 0 };	// 경계 등록 대상
	IffInvadeType		mIffInvadeType = IffInvadeType::NONE;						// 침공 타입
	IFFSanctumType		mIffSanctumType;											// 성소 타입
	bool				mIsAllianceNpc = false;										// 우호적인 NPC 			
	

	// 현재 침공전 수비인지, 공격인지 판단할 수 있는 데이터 필요

#ifdef _SERVER
	bool IsDiff(const IffFactionActor& dst)
	{
		if (mPartyId != dst.mPartyId)								return true;
		if (mIffFactionType != dst.mIffFactionType)					return true;
		//	if (mUserDBId != dst.mUserDBId)								return true;
		if (mGuildId != dst.mGuildId)								return true;
		if (mPVPAlignmentGrade != dst.mPVPAlignmentGrade)			return true;
		//	if (mTreasureMonsterGameId != dst.mTreasureMonsterGameId)	return true;
		//	if (mGameId != dst.mGameId)									return true;
		if (mCanPK != dst.mCanPK)									return true;
		//	if (mPlanetWorldId != dst.mPlanetWorldId)					return true;
		if (mArenaId != dst.mArenaId)								return true;
		if (mArenaTeam != dst.mArenaTeam)							return true;
		if (mEnable != dst.mEnable)									return true;
		if (mTerritoryType != dst.mTerritoryType)					return true;
		if (mCanPVP != dst.mCanPVP)									return true;
		if (mCanPVE != dst.mCanPVE)									return true;
		if (mCanEVE != dst.mCanEVE)									return true;
		if (mIffInvadeType != dst.mIffInvadeType)					return true;
		//if (mIsCampNpc != dst.mIsCampNpc)							return true;
		if (mIffSanctumType.st.isSanctumNexus != dst.mIffSanctumType.st.isSanctumNexus) return true;
		if (mIsAllianceNpc != dst.mIsAllianceNpc)					return true;

		if (false == std::equal(std::begin(mEnemyGuildArray), std::end(mEnemyGuildArray), std::begin(dst.mEnemyGuildArray)))
			return true;
		if (false == std::equal(std::begin(mAllianceGuildArray), std::end(mAllianceGuildArray), std::begin(dst.mAllianceGuildArray)))
			return true;
		if (false == std::equal(std::begin(mCombatTargetOptionList), std::end(mCombatTargetOptionList), std::begin(dst.mCombatTargetOptionList)))
			return true;
		if (false == std::equal(std::begin(mMonitoringUserDBIdArray), std::end(mMonitoringUserDBIdArray), std::begin(dst.mMonitoringUserDBIdArray)))
			return true;

		for (uint8_t x = 0; x < static_cast<uint8_t>(IffCheckType::MAX); ++x)
		{
			for (uint8 y = 0; y < static_cast<uint8_t>(IffFactionType::MAX); ++y)
			{
				if (mEnemyFactionTypeArray[x][y] != dst.mEnemyFactionTypeArray[x][y])
					return true;
			}
		}

		return false;
	}
#endif
};

// 침공 포털 정보
struct ActiveInvadePortalInfo
{
	MapId mapId = INVALID_MAP_ID;
	int64 gameId = 0;
	SpotId spotId = INVALID_SPOT_ID;
};
#ifdef _SERVER
using ActiveInvadePortalInfoList = std::xvector<ActiveInvadePortalInfo>;
#endif

struct SpaceCrackMission
{
	SpaceCrackEnemyKill	mCurZakoCount = 0;		//  일반 현재 잡은수 
	SpaceCrackEnemyKill	mMaxZakoCount = 0;		// 최대

	SpaceCrackEnemyKill mCurEliteCount = 0;		// 엘리트 현재
	SpaceCrackEnemyKill mMaxEliteCount = 0;		// 최대

	SpaceCrackEntryCount mCurEntryCount = 0;	// 입장수 
	SpaceCrackEntryCount mMaxEntryCount = 0;	// 입장수 
};

// 보상 정보
struct SpaceCrackReward
{
	uint32				mPlayTime;
	Money				mGold = 0;		// 골드
	Money				mMileage = 0;  // 마일리지 (아직 개념없음)
	SpaceCrackEnemyKill mZakoCount = 0;
	SpaceCrackEnemyKill mEliteCount = 0;
};
#ifdef _SERVER
struct SpaceCrackAlarm : public Disposable
#else
struct SpaceCrackAlarm
#endif
{
	ChannelId	mChannelId;
	int64		mGameId;
	float		mX, mY, mZ;
	MapId		mMapId;
	FenceId     mFenceId;
	bool		mTicker = false;
};

// 시공의 틈새 활성화 정보
#ifdef _SERVER
struct SpaceCrackActivation : public Disposable
#else
struct SpaceCrackActivation
#endif
{
	MapId		mMapId = INVALID_MAP_ID;
	time_t		mActivationIntervalMin = 0;
	time_t		mActivationIntervalMax = 0;
	time_t		mLastActivationTime = 0;
	FenceId     mFenceId = INVALID_FENCE_ID;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BM상점의 판매 아이템 정보
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using BMCouponConditionGroupId = uint32;
constexpr BMCouponConditionGroupId INVALID_BM_COUPON_CONDITION_GROUP_ID = 0;

#ifdef _SERVER
struct BMShopProductInfo : public Disposable
#else
struct BMShopProductInfo
#endif
{
	BMProductId					mBMProductId = INVALID_BMPRODUCT_ID;
	BMStoreTabId				bmShopParentTabId = 0;
	BMStoreTabId				bmShopSubTabId = 0;
	ItemId						mItemId = INVALID_ITEM_ID;
	ItemAmount					mPurchasedQuantity = 0;	//구입한 수량
	CostType					mPurchaseCostType = CostType::CURRENCY;
	CurrencyType				mPurchaseCurrencyType = CurrencyType::NONE;
	CurrencyDelta				mPurchaseCurrencyPrice = 0;
	ItemId						mPurchaseCurrencyItemId = INVALID_ITEM_ID;
	CurrencyType				mRebateCurrencyType = CurrencyType::NONE;
	CurrencyDelta				mRebateCurrencyAmount = 0;
	time_t						mSaleStartTime = 0;
	time_t						mSaleEndTime = 0;
	Level						mPCLevelLimit = INVALID_LEVEL;
	BMPurchaseLimitBuyerType	mBMShopLimitBuyerType = BMPurchaseLimitBuyerType::NONE;
	ItemAmount					mQuantityLimit = 0;	//제한 수량	
	ResetTimeTicket				mRTTtype = ResetTimeTicket::RTT_NONE;
	GuildLevel					mGuildLevelLimit = INVALID_GUILD_LEVEL;
	bool						mIsBulkSale = false;
	bool						mVisibilityFlag = false;
	BMPopupConditionId			mBMPopupConditionId = INVALID_BM_POPUP_CONDITION_ID;
	int							mPopupSaleDurationTime = 0;							//팝업 판매 시간(초단위)
	BMStepGroupId				mStepGroupId = INVALID_STEP_GROUP_ID;				//스텝업 그룹ID
	BMStepLevel					mStepLevel = INVALID_STEP_LEVEL;					//스텝업 단계
	ItemAmount					mStepBuyLimitCount = MIN_ITEM_AMOUNT;				//스텝업 구매 횟수 조건

};

//////bm 상점 아이템 보관함
#ifdef _SERVER
struct BMDepotDetail : public Disposable
#else
struct BMDepotDetail
#endif
{
	BMItemDBId	mBMItemDBId = INVALID_BM_ITEM_DB_ID;
	BMProductId	mBmProdutId = INVALID_BMPRODUCT_ID;
	ItemId		mItemId = INVALID_ITEM_ID;
	time_t		mPurchasedDateTime = 0;

	BMDepotDetail() = default;
	BMDepotDetail(BMItemDBId bmItemDBId, BMProductId bmProdutId, ItemId itemId, time_t purchasedDateTime) :
		mBMItemDBId(bmItemDBId), mBmProdutId(bmProdutId), mItemId(itemId), mPurchasedDateTime(purchasedDateTime)
	{
	}
};

//////bm 상점 Id / 개수 pair
/// db저장시 활용 된다
struct BMProductIdAmountPair
{
public:
	BMProductId		mBMProductId = INVALID_ITEM_ID;
	ItemAmount		mAmount = 0;


public:
	BMProductIdAmountPair() = default;
	explicit BMProductIdAmountPair(BMProductId bmProductId, ItemAmount amount)
		:
		mBMProductId(bmProductId),
		mAmount(amount)
	{
	}
};

//bm상품 구입 결과
struct BMPurchasedResult
{
	BMProductIdAmountPair	mBMProductIdAmountPair;
	ItemAmount				mTotalQuantity = 0;


public:
	BMPurchasedResult() = default;
	explicit BMPurchasedResult(const BMProductIdAmountPair& bmProductIdAmountPair, ItemAmount amount)
		:
		mBMProductIdAmountPair(bmProductIdAmountPair),
		mTotalQuantity(amount)
	{
	}
};

/////////////////////////////////////////////////////////////////////////////////////////
//bm 상점 일괄 구매 설정
struct BMProductBulkSale
{
	bool					mIncluding = false;		//일괄구매 포함하냐.
	BMProductIdAmountPair	mBMProductIdAmountPair;


public:
	BMProductBulkSale() = default;
	explicit BMProductBulkSale(bool including, const BMProductIdAmountPair& bmProductIdAmountPair)
		:
		mIncluding(including),
		mBMProductIdAmountPair(bmProductIdAmountPair)
	{
	}


};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 다단계 랭크 정보
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 슬롯 정보
struct MultiLevelRankSlotInfo
{
	MultiLevelRank			mRank = INVALID_MULTI_LEVEL_RANK;
	MultiLevelRankId		mRankId = INVALID_MULTI_LEVEL_RANK_ID;
	MultiLevelRankGroupId	mGroupId = INVALID_MULTI_LEVEL_RANK_GROUP_ID;
	MultiLevelRankSlotState	mSlotState = MultiLevelRankSlotState::MAX;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 출석체크보상 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _SERVER
struct AttendanceData : public Disposable
#else
struct AttendanceData
#endif
{
	AttendanceId	mId = 0;		// 출석부 ID
	AttendanceCount	mCount = 0;		// 출석체크 횟수
	bool			mChecked = false;	// 체크여부
	bool			mUsable = false;	// 이용가능한가?

	// expireData
	time_t			mExpiredTime = 0;	// 만료시간 (=시작 시간 + durationHoursReceive)
	time_t			mComplatedTime = 0;	// 데이터상 종료시간 or 모든 보상 받은시간


};


// 이벤트 상점 구매 NpcEventShopGroupID , 아이템 ID, 갯수
#ifdef _SERVER
struct NpcEventShopItemBuyPair : public Disposable
#else
struct NpcEventShopItemBuyPair
#endif
{
	NpcEventShopGroupId	mShopGroupId = INVALID_NPC_EVENT_SHOP_GROUP_ID;
	EventProductId		mEventProductId = INVALID_NPC_EVENT_SHOP_PRODUCT_ID;
	ItemAmount			mItemAmount = 0;


public:
	NpcEventShopItemBuyPair() = default;
	explicit NpcEventShopItemBuyPair(NpcShopGroupId groupId, EventProductId eventProductId, ItemAmount amount)
		:
		mShopGroupId(groupId),
		mEventProductId(eventProductId),
		mItemAmount(amount)
	{
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 기사단 협동 컬렉션 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 기사단 아이템 컬렉션 랜덤 슬롯 정보
struct GuildCollectionCondition
{
	CollectionConditionId		mCollectionConditionId = INVALID_COLLECTION_CONDITION_ID;
	CollectionRandomDataId		mCollectionRandomDataId = INVALID_COLLECTION_RANDOM_DATA_ID;
};

// 기사단 컬렉션 정보
#ifdef _SERVER
using GuildCollectionConditionArray = std::xarray<GuildCollectionCondition, MAX_GUILD_ITEM_COLLECTION_CONDITION_COUNT>;
struct GuildItemCollectionInfo
{
	CollectionId					mCollectionId = INVALID_COLLECTION_ID;
	GuildCollectionConditionArray	mGuildCollectionConditionArray;
};
#else // _SERVER
struct GuildItemCollectionInfo
{
	CollectionId					mCollectionId = INVALID_COLLECTION_ID;
	GuildCollectionCondition		mGuildCollectionCondition[MAX_GUILD_ITEM_COLLECTION_CONDITION_COUNT] = {};
};
#endif // _SERVER

struct GuildCollectionRegisterInfo
{
	// 등록한 컬렉션 컨디션 아이디
	CollectionConditionId		mCollectionConditionId = INVALID_COLLECTION_CONDITION_ID;
	// 등록한 유저 이름
	wchar						mUserName[MAX_CHAR_NAME_LEN_SYSTEM + 1] = { 0, };
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 기사단 창고
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// 아이템 획득 내역
struct AcquiredGuildItemHistory
{
	ItemDataCreated mItemDataCreated;
	time_t mAcquiredTime = 0;
	GuildItemAcquireType mItemAcquiredType;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 칭호 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
struct TitleInfo : public Disposable
#else
struct TitleInfo
#endif 
{
	TitleId		mTitleId = INVALID_TITLE_ID;
	TitleStatus	mStatus = TitleStatus::DISABLED;
	uint32		mConditionCount = 0;
	PresetValue mPresetValue = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 통계 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StatisticsInfo
{
	StatisticsId		mStatisticsId = INVALID_STATISTICS_ID;
	StatisticsValue		mThisWeekValue = 0; // 이번 주 통계 값
	StatisticsValue		mLastWeekValue = 0; // 지난 주 통계 값
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 유저 프로필
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _SERVER
// 만료된(삭제된) 한마디 작성 시간
constexpr time_t EXPIRED_COMMENT_WRITE_TIME = std::numeric_limits<time_t>::max();
#endif

// 패킷당 보내는 한마디 최대 개수
constexpr std::size_t MAX_COMMENT_SEND_COUNT = 20;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 파티던전 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
struct CoopPartyWaitMemberInfo : public Disposable
#else
struct CoopPartyWaitMemberInfo
#endif
{
	int						mSlotIndex = -1;
	CreatureWeaponType		mWeaponType = CreatureWeaponType::NONE;
	Level					mLevel = INVALID_LEVEL;
};

#ifdef _SERVER
struct CoopPartyInfo : public Disposable
#else
struct CoopPartyInfo
#endif
{
	PartyId						mPartyId;
	DungeonGroupId				mDungeonGroupId;
	DungeonDifficulty			mDungeonDifficulty;
	Level						mLeaderLevel;
	CreatureWeaponType			mLeaderWeaponType;

	PartyDungeonStepStatus		mPartyDungeonStepStatus;
	bool						mCanForcedEnter;
	CoopPartyWaitMemberInfo		mMemberInfoList[MAX_PARTY_MEMBER];
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 소셜 액션 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _SERVER
struct SocialEmotionInfo : public Disposable
#else
struct SocialEmotionInfo
#endif
{
	UserSocialEmotion			mUserSocialEmotion;
	bool						mEnable = true;   // 활성 비활성 
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 퀘스트 게시판 관련
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 게시판 새로고침 재화 정보
struct RefreshCurrencyInfo
{
	int				mRefreshNumber = 0;
	CurrencyType	mRefreshCurrencyType = CurrencyType::NONE;
	Currency		mRefreshCurrencyCost = 0;

	RefreshCurrencyInfo() = default;
	explicit RefreshCurrencyInfo(int refreshNumber, CurrencyType currencyType, Currency currency)
		:
		mRefreshNumber(refreshNumber),
		mRefreshCurrencyType(currencyType),
		mRefreshCurrencyCost(currency)
	{
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PK 북 관련(클라이언트 전달용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PK 북 기여자 정보
struct PKContributorRecord
{
	PKRecordId			mPKRecordId = INVALID_PK_RECORD_ID;
	PKBookResult		mPkBookResult = PKBookResult::MAX;
	CreatureWeaponType	mWeaponType = CreatureWeaponType::NONE;
	PKContributorType	mContributorType = PKContributorType::MAX;
	PKContributorId		mContributorId = INVALID_PK_CONTRIBUTOR_ID;
	GuildEmblemId		mGuildEmblemId = INVALID_GUILD_EMBLEM_ID;
	StatValue			mDamage = 0;
	WorldId				mWorldId = INVALID_WORLD_ID;
};

// PK 북 상세정보
struct PKRecordDetailInfo
{
	PKRecordId			mPKRecordId;
	PKBookResult		mPkBookResult = PKBookResult::MAX;
	Exp					mLostExp = 0;
	Exp					mCurrentMaxExp = 0;
	int					mRemainActionCount = 0;
	MapId				mMapId = INVALID_MAP_ID;
	time_t				mCreatedTime = 0;

	float GetExpPercent()
	{
		if (0 == mCurrentMaxExp)
			return 0.f;

		return static_cast<float>((float)mLostExp / (float)mCurrentMaxExp * 100);
	}
};

#ifdef _SERVER
using PKContributorRecordList = std::xvector<PKContributorRecord>;
using PKContributorRecordNamePair = std::tuple<PKContributorRecord, std::xwstring, std::xwstring>;
using PKContributorRecordNamePairList = std::xvector<std::tuple<PKContributorRecord, std::xwstring, std::xwstring>>;
using PKRecordDetailInfoList = std::xvector<PKRecordDetailInfo>;
#endif



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BM 팝업 상점
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct BMPopupInfo
{
	BMProductId				mBMProductId = INVALID_BMPRODUCT_ID;
	BMPopupConditionId		mBMPopupConditionId = INVALID_BM_POPUP_CONDITION_ID;
	BMPopupStatus			mStatus = BMPopupStatus::NONE;
	uint64					mConditionCount = 0;
	time_t					mAchieveTime = 0;
	time_t					mCloseTime = 0;

	BMPopupInfo() = default;
	explicit BMPopupInfo(BMProductId bmProductId, BMPopupConditionId conditionId, BMPopupStatus status, uint64 conditionCount,
		time_t achieveTime, time_t closeTime)
		:
		mBMProductId(bmProductId),
		mBMPopupConditionId(conditionId),
		mStatus(status),
		mConditionCount(conditionCount),
		mAchieveTime(achieveTime),
		mCloseTime(closeTime)
	{
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 아이템 강화 결과 정보
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct ItemEnchantResultData
{
	ItemEnchantResult		mItemEnchantResult = ItemEnchantResult::NONE;
	ItemDataEquip			mItemDataEquip;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 컬렉션 슬롯 정보
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CollectionConditionData
{
	CollectionConditionId		mCollectionConditionId = INVALID_COLLECTION_CONDITION_ID;
	ItemAmount					mRegisterCount = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 컬렉션 정보
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct CollectionData
{
	CollectionId			mCollectionId = INVALID_COLLECTION_ID;
	time_t					mExpiredTime = 0;

	CollectionData() = default;
	explicit CollectionData(CollectionId collectionId, time_t expiredTime) :
		mCollectionId(collectionId), mExpiredTime(expiredTime)
	{
	}
};

#ifdef _SERVER
using CollectionDataList = std::xvector<CollectionData>;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 봉인(문양) 시스템
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 봉인 정보
struct SealData
{
	SealId					mSealId = INVALID_SEAL_ID;
	Level					mLevel = INVALID_LEVEL;
	Exp						mExp = 0;

	SealData() = default;
	explicit SealData(SealId sealId, Level level, Exp exp) :
		 mSealId(sealId), mLevel(level), mExp(exp) {}
};

// 봉인 슬롯 정보
struct SealSlotData
{
	SealId					mSealId = INVALID_SEAL_ID;
	SealSlotId				mSealSlotId = INVALID_SEAL_SLOT_ID;
	SealSlotColor			mSealSlotColor = SealSlotColor::NONE;
	StatType				mStatType = StatType::NONE;
	StatValue				mStatValue = 0;

	SealSlotData() = default;
	explicit SealSlotData(
		SealId sealId, SealSlotId sealSlotId,
		SealSlotColor sealSlotColor, StatType statType, StatValue statValue
	) :
		mSealId(sealId), mSealSlotId(sealSlotId),
		mSealSlotColor(sealSlotColor), mStatType(statType), mStatValue(statValue) {}
	explicit SealSlotData(SealId sealId, SealSlotId sealSlotId) :
		mSealId(sealId), mSealSlotId(sealSlotId) {}
};

// 봉인 보너스 정보
struct SealBonusData
{
	SealSlotColor			mSealSlotColor = SealSlotColor::NONE;
	uint16					mCount = 0;

	SealBonusData() = default;
	explicit SealBonusData(SealSlotColor sealSlotColor, uint16 count) :
		mSealSlotColor(sealSlotColor), mCount(count) {}
};

// 몬스터 도감 지식 정보
struct MonsterKnowledge
{
	MonsterKnowledgeId		mId = INVALID_MONSTER_KNOWLEDGE_ID; //몬스터 지식 아이디(몬스터 npc Id)
	MonsterKnowledgeLevel	mLevel = INVALID_LEVEL;	//몬스터 지식 레벨
	MonsterKnowledgeExp		mExp = 0; //몬스터 지식 경험치

	MonsterKnowledge(MonsterKnowledgeId id, MonsterKnowledgeLevel level, MonsterKnowledgeExp exp)
		:mId(id), mLevel(level), mExp(exp)
	{

	}
};


// 정령탄 강화 Category
using SpiritShotCategoryId = uint32;
constexpr SpiritShotCategoryId INVALID_SPIRIT_SHOT_CATEGORY_ID = 0;

// 정령탄 강화 Level
using SpiritShotLevelId = uint32;
constexpr SpiritShotLevelId INVALID_SPIRIT_SHOT_LEVEL_ID = 0;

// 정령탄 강화 SumLevel
using SpiritShotEnchantSumLevelId = uint32;
constexpr SpiritShotEnchantSumLevelId INVALID_SPIRIT_SHOT_ENCHANT_SUM_LEVEL_ID = 0;

// 기사단 분배 설정 
//ItemGrade 사용 OFF 는 0 으로 사용 

/*
struct ItemAcquisitionSetting
{
	int ItemCategoryMain = 0;
	int ItemCategorySub[]
};*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct GuildAcquisitionSettings
{
	GuildDBId					mGuildDBId = INVALID_GUILD_DB_ID;
	bool						mAcquisition = false;		// 기사단 획득
	GuildAcquisitionSettingId	mAcquisitionSetting[MAX_GUILD_ACQUISITION_SETTING_COUNT] = {};
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 기사단 분배 & 경매 정보
struct GuildStoreItemData
{
	GuildAuctionId			mGuildAuctionId = 0 ;	// 경매장 고유 번호
	ItemDataEquip			mItemDataEquip;			// 아이템 정보
	CreatureId				mBossId = 0;			// 획득 보스 ID
	MapId					mMapId = 0;				// 맵 번호 	
	CurrencyType			mCurrencyType = CurrencyType::NONE;      // 입찰금
	Currency		  		mCurrency = 0;
	GuildDivisionState		mDivisionState = GuildDivisionState::NONE;    // 경매장 상태
	GuildAuctionType		mGuildAuctionType = GuildAuctionType::NONE;		// 분배 방식 	
	GuildAuctionResultType	mGuildAuctionResultType = GuildAuctionResultType::NORMAL; // 기사단 정산 타입
	time_t					mAuctionDateTime = 0;			// 경매장 등록시간 
	time_t					mExpiredDateTime = 0;			// 만료 시간 
	time_t					mAcquisitionDateTime = 0;		//  획득 일자
	bool					mAvailable = false;				// 입찰 가능한지 판단 
	GuildStoreAvailableResultType	mGuildStoreAvailableResultType;
	uint8_t					mContributorMemberCount = false;	// 참여자 멤버수
	GuildAuctionId			mGuildAuctionDoneId = 0;	// 경매장 완료 되는 경우 발급 되는 번호
	bool					mJoin = false;				// 내가 참여 했는지 판단 여부
/*

	GuildStoreItemData& operator=(const GuildStoreItemData& rhs) {
		mGuildAuctionId = rhs.mGuildAuctionId;
		mItemDataEquip = std::move(rhs.mItemDataEquip);
		mBossId = rhs.mBossId;
		mMapId = rhs.mMapId;
		mCurrencyType = rhs.mCurrencyType;
		mCurrency = rhs.mCurrency;
		mDivisionState = rhs.mDivisionState;
		mGuildAuctionType = rhs.mGuildAuctionType;
		mGuildAuctionResultType = rhs.mGuildAuctionResultType;
		mAuctionDateTime = rhs.mAuctionDateTime;
		mExpiredDateTime = rhs.mExpiredDateTime;
		mAcquisitionDateTime = rhs.mAcquisitionDateTime;
		return *this;
	}*/

};


#ifdef _SERVER
using GuildStoreItemDataList = std::xvector<GuildStoreItemData>;
using GuildAcquisitionSettingList = std::xvector<GuildAcquisitionSettings>;
#endif


using GuildAcquisitionId = uint32;
constexpr GuildAcquisitionId INVALID_GUILD_ACQUISITION_ID = 0;

// 기사단 창고, 경매 , 분배 한번에 보여주는 페이지 수량
constexpr uint32  MAX_GUILD_STORE_PAGE = 20;


//기사단 영지 공략 던전
//공략 던전 아이디(AgitDungeonRaid 데이터 테이블의 고유 아이디)
using AgitDungeonRaidId = uint32;
constexpr AgitDungeonRaidId INVALID_AGIT_DUNGEON_RAID_ID = 0;


// 그림자전장 랭킹 관련
using GuildWarSeasonId = uint32;
constexpr GuildWarSeasonId INVALID_GUILD_WAR_SEASON_ID = 0;

using GuildWarRewardId = uint32;
constexpr GuildWarRewardId INVALID_GUILD_WAR_REWARD_ID = 0;

using GuildWarTierDataId = uint32;
constexpr GuildWarTierDataId INVALID_GUILD_WAR_TIER_DATA_ID = 0;

#pragma pack(pop)
#pragma warning(pop)


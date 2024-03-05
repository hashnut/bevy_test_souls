// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumRange.h"

/**
 * 오브젝트 클래스 관련 EunmType/Define 모음
 */


/**
* Bit Mask Define
*/
#define CHECK_FLAG_TYPE(Type, Flag)     (((Type) & static_cast<uint32>(Flag)) > 0)
#define SET_FLAG_TYPE(Type, Flag)       (Type |= static_cast<uint32>(Flag))
#define REMOVE_FLAG_TYPE(Type, Flag)    (Type &= ~static_cast<uint32>(Flag))
#define CLEAR_FLAG_TYPE(Type)           (Type = 0x00)

//
// #define USE_GAMEOBJECT_DISPLAY_DATA
//

// GameObject Type 정의
enum class EGsGameObjectType : uint32
{
	Base			= 1,			// All  hierarchy : Root
	PropObject		= 1 << 1,		// hierarchy : Base - PropObject
	Projectile		= 1 << 2,		// hierarchy : Base - Projectile
	Vehicle			= 1 << 3,		// hierarchy : Base - Vehicle 
	Creature		= 1 << 4,		// hierarchy : Base - Creature 
	Warp			= 1 << 5,		// hierarchy : Base - PropObject - Warp
	Player			= 1 << 6,		// hierarchy : Base - Creature - Player
	NonPlayerBase	= 1 << 7,		// hierarchy : Base - Creature - NonPlayerBase
	NonPlayer		= 1 << 8,		// hierarchy : Base - Creature - NonPlayerBase - NonPlayer
	Camp			= 1 << 9,		// hierarchy : Base - Creature - NonPlayerBase - NonPlayer - Camp
	RemotePlayer	= 1 << 10,		// hierarchy : Base - Creature - Player - RemotePlayer
	LocalPlayer		= 1 << 11,		// hierarchy : Base - Creature - Player - LocalPlayer
	DropObject		= 1 << 12,		// hierarchy : Base - PropObject - DropItem
	PropInteraction = 1 << 13,		// hierarchy : Base - PropObject - PropInteraction
	Sandbag			= 1 << 14,		// hierarchy : Base - Creature - NonPlayerBase - Sandbag
	SpaceCrackNonPlayer = 1 << 15,	// hierarchy : Base - Creature - NonPlayerBase - SpaceCrackNonPlayer
	PropInteractInvasion = 1 << 16,	// hierarchy : Base - PropObject - PropInteraction - PropInteractInvasion
	Sanctum			= 1 << 17,		// hierarchy : Base - NonPlayerBase - Sanctum
};

ENUM_CLASS_FLAGS(EGsGameObjectType);

// [Todo] Enum Loop에 대한 더 좋은 방법이 있다면 수정한다.
ENUM_RANGE_BY_VALUES(EGsGameObjectType,
	EGsGameObjectType::Base,
	EGsGameObjectType::PropObject,
	EGsGameObjectType::Projectile,
	EGsGameObjectType::Vehicle,
	EGsGameObjectType::Creature,
	EGsGameObjectType::Warp,
	EGsGameObjectType::Player,
	EGsGameObjectType::NonPlayerBase,
	EGsGameObjectType::NonPlayer,
	EGsGameObjectType::Camp,
	EGsGameObjectType::RemotePlayer,
	EGsGameObjectType::LocalPlayer,
	EGsGameObjectType::DropObject,
	EGsGameObjectType::PropInteraction,
	EGsGameObjectType::Sandbag,
	EGsGameObjectType::SpaceCrackNonPlayer,
	EGsGameObjectType::PropInteractInvasion,
	EGsGameObjectType::Sanctum)

/**
* StateType
*/
UENUM()
enum class EGsStateBase : uint8
{
	None = 0,
	Spawn = 1,
	Idle = 2,

	ForwardWalk = 3,
	Run = 4,
	AutoMove = 5,
	AutoMoveStop = 6,
	
	Casting = 7,
	Attack = 8,
	AttackEnd = 9,
	Hit = 10,
	Reaction = 11,
	ReactionEnd = 12,		// local Player 전용으로 사용 
	CommonAction = 13,
	CommonActionEnd = 14,
	ChangeIdle = 15,
	ChangeWeapon = 16,
	
	Dying = 17,
	Dead = 18,
	
	Ride = 19,

	Roar = 20,

	Warp = 21,

	Social = 22,
	SocialAction = 23,

	Revive = 24,

	Freeze = 25,
	UnFreeze = 26,
	
	Interaction = 27,
	InteractionEnd = 28,

	Looting = 29,

	StateBaseMax = 30,

	//시공의 틈새 얼림
	SpaceCrackIdle = 31,
	//보물 몬스터 살아서 도망감
	RunAway = 32,

	// Camp
	BuildComplete = 33,

	ChangeWeaponEnd = 34,
};

/**
* 스킬 사용 체크 결과
*/
UENUM(BlueprintType)
enum class EGsSkillCheckResultType : uint8
{
	Pass,	
	Same,							// 같은 스킬
	CoolTime,						// 쿨타임중
	Target,							// 공격 불가능한 타겟
	NotCancel,						// 켄슬 가능 시점 아님
	Reaction,						// 행동 불가 상태
	Abnormality,					// 이상상태 확인
	NotSyncUserData,				// 서버에서 알려준 스킬과 맞지 않음
	ImpossibleState,				// 스킬 사용 불가능한 상태에 돌입해 있음
	NotEnoughRequireCost,			// 스킬 사용에 필요한 cost 부족(ex 원거리 소모품/마나 등)
	NotStageMode,					// 스킬 사용할수 없는 모드
	SkillSlotDisable,				// 스킬 슬롯이 비활성화
	ExecuteConditionAbnormality,	// 스킬 데이터의 ExecuteCondition 체크 (Abnormality)
	ExecuteConditionPassivity,		// 스킬 데이터의 ExecuteCondition 체크 (Passivity)
	ExecuteConditionStat,			// 스킬 데이터의 ExecuteCondition 체크 (Stat)
	ManualTargetRange,				// 수동 이동 스킬 사용 타겟 거리 체크
};

/**
* 지역 타입
*/
UENUM()
enum class EAreaType : uint8
{
	NonBattleArea = 0,
	BattleArea,
};

/**
* 상태 이상 메인 타입 정의
*/
enum class EGsAbnormalityEffectMainType : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// DOT(damage over time), HOT(Heal over time)
	OVERTIME = 1,

	// CC : Crowd Control
	CC = 2,

	// STAT 가감
	STAT = 3,

	// UTIL
	UTIL = 4,

	MAX = 5,
};


// GameObjectData 맴버의 변경 이벤트가 필요할때
// Type을 추가하고 Setter에서 OnSend() 호출
enum class EGsGameObjectDataMember
{
	Level,
};

// 페어리 대시 스킬 발동 조건 플레그
enum class EGsFairyVehicleCondition : uint16
{
	NONE = 0,
	USER_STATE_NORMAL = 1, // UserStatus::USER_STATE_NORMAL 평화 상태
	MOVE_STATE = 1 << 1,   // 자동 이동, 이동 상태
	VEHICLE_FAIRY = 1 << 2, // 이속업 발동 가능한 페어리
	VEHICLE_NOT_ABNORMALITY = 1 << 3, // 탈것 버프가 없는지
	VEHICLE_CREATURE_ACTION = 1 << 4, // 탑승 불가 버프 확인

	SUCCESS = USER_STATE_NORMAL | MOVE_STATE | VEHICLE_FAIRY | VEHICLE_NOT_ABNORMALITY | VEHICLE_CREATURE_ACTION
};

/*
  부착위치 : 소켓, 위치, 소켓기준 바운드박스 + 시전자 방향 고려
*/
UENUM(BlueprintType)
enum class ActionCategory : uint8
{
	SKILL = 0,
	REACTION,
	COMMON,
	MAX,
};


UENUM(BlueprintType)
enum class ETeleportEffectType : uint8
{
	DEPART,
	ARRIVE,
	MAX,
};

UENUM()
enum class EGsCoolTime : uint16
{
	Skill,
	Item,
	BMShopProduct,
	ItemPeriod, // 아이템 유효기간
	Preset,
	PersonalTrading,
	Change_Weapon,

	Max,
};

// enum 변경 시,DC - IconBpResData 수정해야됨
// String to Enum Converting 해서 사용중
UENUM(BlueprintType)
enum class EItemIconSize : uint8
{
	Small = 0,
	Medium,
	Large,
	ExtraLarge,
	Max,
};

UENUM()
enum class EPresetNumberBit : uint8
{
	Preset_None		= 0b00000000,
	Preset_First	= 0b00000001,
	Preset_Second	= 0b00000010,
	Preset_Third	= 0b00000100,
};

UENUM()
enum class EInventoryTab : uint8
{
	AllTab = 0 ,
	EquipmentTab,
	ConsumableTab,
	MaterialTab,

	TabMax,
};

UENUM()
enum class ENpcShopProductListTab : uint8
{
	ProductList_AllTab = 0,
	ProductList_Filter,
	ProductList_EquipmentTab,
	ProductList_ConsumableTab,
	ProductList_MaterialTab,
	
	ProductList_Weapon,
	ProductList_Armor,
	ProductList_Accessory,

	ProductList_Sword,
	ProductList_Dagger,
	ProductList_Bow,
	ProductList_SHOTGUN,
	ProductList_Wand,
	ProductList_Orb,

	ProductList_Max,
};

UENUM()
enum class EnchantInvenTab : uint8
{
	EquipmentTab = 0,
	EnchantMaterialTab,

};

UENUM()
enum class EnchantSlotType : uint8
{
	TargetSlot = 0,
	MaterialSlot,
};

UENUM()
enum class NpcShopAutoSellEquipmentType : uint8
{
	Weapon_NormalGrade = 0,
	Weapon_MagicGrade,
	Weapon_RareGrade,

	Armor_NormalGrade,
	Armor_MagicGrade,
	Armor_RareGrade,

	Accessory_NormalGrade,
	Accessory_MagicGrade,
	Accessory_RareGrade,

	EquipmentType_Max
};


// AutoTarget 검색 옵션 임시 설정
// 치트용
UENUM(BlueprintType)
enum class EGsAutoTargetSearchType : uint8
{
	Attacker,	// 마지막 공격 대상
	Quest,		// 퀘스트 몹 대상
	FirstEnemy, // 거리 기반 적 대상
};

// cc기에 따른 행동 제약 타입 정의
// enum class EGsCrowdControlType : uint32
// {
// 	None		= 0,
// 	MotionLock	= 1,			// 이동뿐 아니라, 모션을 멈춤(서버와 맞춤)
// 	MoveLock	= 1 << 1,		// 이동 불가형
// 	MoveForced	= 1 << 2,		// 이동 불가형 이나, 서버 강제 이동형(서버와 맞춤)
// 	NormalSkillLock = 1 << 3,	// 평타 스킬 사용 불가
// 	SkillLock	= 1 << 4,		// 스킬 사용 불가 형
// 	DashSkillLock = 1 << 5,		// 대시 스킬 사용 불가(서버와 맞춤)
// 	BuffSkillLock = 1 << 6,		// 버프 스킬 사용 불가(서버와 맞춤)
// 	ItemLock	= 1 << 7,		// 아이템 사용 불가형(내플레이어)
// 	SocialActionLock = 1 << 8,	// 소셜 액션(서버와 맞춤)
// 	WarpLock	= 1 << 9,		// 워프 사용 불가(서버와 맞춤)
// 	SkillCancel = 1 << 13,		// 사용중인 스킬 켄슬형
// 	LookInputLock  = 1 << 14,	// 조이스틱 방향 입력 막음 (내플레이어)
// 	SpectatorMode = 1 << 15,	// 관전자 모드로 변경
// 	FreezeLock = 1 << 16,		// 애니메이션 Pose
// 	ChangeMesh = 1 << 17,		// 특정 메시 변경
// 
// 	StunType = MoveLock | SkillLock | SkillCancel, // 범용적인 스턴 타입
// };

UENUM()
enum class EGsBillboardUpdateType : uint8
{
	All,
	NameColor,
	Guild,
	Party,
	Title, // 칭호정보
	Contribution,
	Aggro,
	//Stat,
};

UENUM()
enum class EGsCampType : uint8
{
	ConstructType = 0,
	CompleteType,
};


const float DEFAULT_CAPSULE_RADIUS = 50.0f;
const float DEFAULT_CAPSULE_HEIGHT = 100.0f;
const int DEFAULT_CYLINDER_INDEX = 0;

const FName TargetMarkerComponentName = TEXT("TargetMaker");
const FName InteractionMoveTargetMarkerComponentName = TEXT("InteractionMoveTargetMaker");
const FName ShadowDecalComponentName = TEXT("ShadowDecal");

const FName ShadowAttchSocketName = TEXT("ShadowSocket");
const FName InteractionButtonComponentName = TEXT("InteractionWidget");
const FName InteractionSphereComponentName = TEXT("InteractionCollisionSphere");
const FName InteractionButtonColliderPresetName = TEXT("InteractionButton");
const FName QuestTargetMarkName = TEXT("QuestTargetMark");
const FName NPCInteractionBlockCapsuleComponentName = TEXT("NPCInteractionBlockCollisionCapsule");
const FName NPCInteractionBlockPresetName = TEXT("NPCInteractionBlock");

const int SKILL_SLOT_ID_NORMAL = 0; // 클라 평타 스킬 슬롯ID. 서버에서는 슬롯 0번은 INVALID_SKILL_SLOT_ID로 판정/

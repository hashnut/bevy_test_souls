// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsQuestEnum.generated.h"

UENUM(BlueprintType)
enum class EGsQuestUIState : uint8
{
	QUEST_STATE_NONE,
	QUEST_STATE_READY,			// 퀘스트 시작 전 상태		(여기를 눌러 받으세요)
	QUEST_STATE_PROGRESS,		// 퀘스트 진행 중 상태		(오크 처치 0/10)
	QUEST_STATE_REWARD,			// 보상이 가능 한 상태		(여기를 눌러 보상을 받으세요.)
	QUEST_STATE_LEVEL_LOCKED,	// 레벨 제한					(25레벨 이후 진행이 가능합니다.)
	QUEST_STATE_INVADE_LOCKED,	// 내 서버에서 침공퀘스트는 제한	(침공전 상태에서 진행 가능한 퀘스트입니다.)
};

// 삭제 예정
UENUM(BlueprintType)
enum class EGsQuestActorType : uint8
{
	QUEST_ACTOR_GOAL_POS UMETA(DisplayName = "GoalPos"),			// 목표좌표
	QUEST_ACTOR_NPC_SPAWN_POS UMETA(DisplayName = "NpcSpawnPos"),	// 목표좌표
};

UENUM(BlueprintType)
enum class QuestWindowUISlotType : uint8
{
	CHAPTER_PROGRESS		= 0,	// 챕터가 진행 중인 상태
	CHAPTER_LOCKED			= 1,	// 아직 습득하지 못한 챕터 상태
	CHAPTER_COMPLETED		= 2,	// 챕터 완료
	STORY_COMPLETED			= 3,	// 완료 한 스토리(보상 수령전)
	STORY_PROGRESS			= 4,	// 진행 중인 스토리
	STORY_LOCKED			= 5,	// 레벨 제한에 걸린 스토리
	STORY_WATING			= 6,	// 수락 대기
	STORY_LOCKED_MAIN_STORY	= 7,	// 메인스토리 제한에 걸린 스토리
	STORY_LOCKED_SUB_STORY	= 8,	// 서브스토리 제한에 걸린 스토리
	STORY_LOCKED_EQUIRE_LEVEL = 9,	// 요구 레벨 제한에 걸린 스토리
	STORY_REWARDED			 = 10,	// 완료 한 스토리(보상 수령후)
};

enum class EQuestNpcState : int32
{
	STATE_NONE = 0,

	STATE_SUB_INVALID_ACCEPT = 1 << 0,			// 수락 조건이 충족하지 못했을 때 (전 스토리 수행 중일때)
	STATE_SUB_ACCEPT = 1 << 1,					// 진행중
	STATE_SUB_WATING = 1 << 2, 					// 대기 상태
	STATE_PROGRESS = 1 << 3,					// 퀘스트 완료 대상 일때,
	STATE_REWARD_WATING = 1 << 4,				// 퀘스트 보상 가능 상태 일때,
	STATE_SUB_INVALID_MAIN_STORY = 1 << 5,		// 수락 조건이 충족하지 못했을 때 (메인 스토리)
	STATE_SUB_INVALID_SUB_STORY = 1 << 6,		// 수락 조건이 충족하지 못했을 때 (서브 스토리)
	STATE_SUB_INVALID_REQUIRE_LEVEL = 1 << 7,	// 수락 조건이 충족하지 못했을 때 (서브 스토리)

	STATE_ALL = 0xff,
};
ENUM_CLASS_FLAGS(EQuestNpcState);

UENUM(BlueprintType)
enum class EGsQuestUIEffectType : uint8
{
	QUEST_UI_EFFECT_NONE,
	QUEST_UI_EFFECT_NEW,
	QUEST_UI_EFFECT_UPDATE,
	QUEST_UI_EFFECT_COMPLETED,
	QUEST_UI_EFFECT_REWARD,
};

UENUM(BlueprintType)
enum class QuestWindowUISubSlotType : uint8
{
	SLOT_TYPE_MAP = 0,	
	SLOT_TYPE_NPC = 1,
	SLOT_TYPE_STORY = 2,
};


UENUM(BlueprintType)
enum class QuestEventType : uint8
{
	EVENT_STROY_INIT_BEGIN = 0,

	EVENT_STROY_BEGIN = 1,
	EVENT_STROY_CLEAR = 2,	

	EVENT_PE_QUEST_BEGIN = 3,
	EVENT_QUEST_BEGIN = 4,
	EVENT_QUEST_CLEAR = 5,	

	EVENT_OBJECTIVE_CLEAR = 6
};

//(발급가능, 진행중, 진행불가)
UENUM(BlueprintType)
enum class QuestWindowUISubStateType : uint8
{
	STATE_PROGRESSING = 0,		// 진행중
	STATE_ACCEPTABLE = 1,		// 발급가능 (수행할수있는 퀘스트가 있다.)
	STATE_LOCK = 2,				// 수락할수없는 조건이 걸린경우
	STATE_COMPLETED = 3,		// 완료
};

UENUM(BlueprintType)
enum class QuestGuideHudRegisterType : uint8
{
	GUIDE_HUD_TYPE_SYSTEM = 0,
	GUIDE_HUD_TYPE_USER = 1,
};

UENUM(BlueprintType)
enum class QuestSubItemType : uint8
{
	SUB_AREA = 0,
	SUB_NPC = 1,
	SUB_STORY = 2,
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsTutorialEnumDefine.generated.h"

/**
 * 튜토리얼 타입. 별도로 구현할 항목이 없으면 DEFAULT, 별도구현이 필요하면 추가
 */
UENUM(BlueprintType) 
enum class EGsTutorialType : uint8
{
	NONE = 0 UMETA(Tooltip = "튜토리얼 제외"),
	DEFAULT = 1 UMETA(Tooltip = "기본 튜토리얼"),
	GUILD_JOIN = 2 UMETA(Tooltip = "길드 가입 튜토리얼"),
	GUILD_CONTENTS = 3 UMETA(Tooltip = "길드 컨텐츠 튜토리얼"),
	TITLE_SYSTEM = 4 UMETA(Tooltip = "칭호 튜토리얼"),
	PLAYER_STAT = 5 UMETA(Tooltip = "스탯 튜토리얼"),
	FAIRY = 6 UMETA(Tooltip = "페어리 튜토리얼"),
	COSTUME = 7 UMETA(Tooltip = "코스튬 튜토리얼"),
	SKILL = 8 UMETA(Tooltip = "스킬 튜토리얼. DTGsTutorialTargetId RewardId 입력"),
	WEAPON_EQUIP = 9 UMETA(Tooltip = "무기 장착 튜토리얼. DTGsTutorialTargetId RewardId 입력"),
	SPIRIT_SHOT = 10 UMETA(Tooltip = "천마석 튜토리얼"),
	TEST_TUTORIAL = 11 UMETA(Tooltip = "테스트용 튜토리얼"),
};

/**
 * 튜토리얼 발동 조건
 */
UENUM(BlueprintType)
enum class EGsTutorialConditionType : uint8
{
	NONE = 0 UMETA(Tooltip = "조건 불만족"),
	QUEST = 1 UMETA(Tooltip = "퀘스트 EventAction에 의한 발동"),
	QUEST_ID = 2 UMETA(Tooltip = "특정 QuestId 퀘스트 습득 시. DTGsTutorialTargetId QuestId 추가 입력"),
	QUEST_STORY_ID = 3 UMETA(Tooltip = "특정 StoryId의 퀘스트 수락 전상태. DTGsTutorialTargetId StoryId 추가 입력"),
	OPEN_UI = 4 UMETA(Tooltip = "특정 UI 열었을 때 발동. DTGsTutorialConditionOpenUI 추가 입력"),
	OPEN_UI_CHECK_PARAM = 5 UMETA(Tooltip = "중복UI나 탭 구별을 위함. DTGsTutorialConditionOpenUI 추가 입력"),
	UNLOCK_CONTENT = 6 UMETA(Tooltip = "특정 컨텐츠 Unlock 시. FGsSchemaTutorialTargetId 추가 입력"),
	FIRST_TUTORIAL = 7 UMETA(Tooltip = "첫 튜토리얼 전용. DTGsTutorialTargetId StoryId 추가 입력"),
};

/**
 * 튜토리얼 동작 타입
 */
UENUM(BlueprintType)
enum class EGsTutorialActionType : uint8
{
	NONE = 0 UMETA(Tooltip = "타겟 없음"),
	UI_INTERACTION = 1 UMETA(Tooltip = "UI 인터렉션"),
	NPC_SHOP_SELECT_POTION_ICON = 2 UMETA(Tooltip = "잡화상점 아이콘 선택. DTGsTutorialTargetId에 ItemId 추가 입력"),
	NPC_SHOP_SELECT_POTION_LIST = 3 UMETA(Tooltip = "잡화상점 리스트 선택. DTGsTutorialTargetId에 ItemId 추가 입력"),
	SKILL_SELECT = 4 UMETA(Tooltip = "스킬창 스킬 리스트 선택"),
	SKILL_LEARN = 5 UMETA(Tooltip = "스킬 획득, TutorialSkill이 넘겨준 데이터 사용"),
	SKILL_EQUIP = 6 UMETA(Tooltip = "스킬 장착, TutorialSkill이 넘겨준 데이터 사용"),
	INVENTORY_SELECT_ITEM = 7 UMETA(Tooltip = "인벤토리 아이템 선택. DTGsTutorialTargetId에 ItemId 추가 입력"),
	SUMMON_SHOW_IMMEDIATELY = 8 UMETA(Tooltip = "소환UI 바로보기or모두보기에 포커싱"),
	FAIRY_SELECT = 9 UMETA(Tooltip = "페어리 선택, DTGsTutorialTargetId FairyId 추가 입력"),
	COSTUME_SELECT = 10 UMETA(Tooltip = "코스튬 선택, DTGsTutorialTargetId CostumeId 추가 입력"),
	CRAFT_SELECT = 11 UMETA(Tooltip = "재작 선택, DTGsTutorialTargetId CraftId 추가 입력"),
	MONSTER_KNOWLEDGE_SELECT = 12 UMETA(Tooltip = "도감 중앙 지역 ListView의 첫번째 지역 선택"),
	HUD_SKILL_BTN_SELECT = 13 UMETA(Tooltip = "HUD 스킬버튼 선택, DTGsTutorialTargetId에 슬롯 번호 입력"),
	HUD_NPC_BTN_SELECT = 14 UMETA(Tooltip = "HUD NPC버튼 선택, DTGsTutorialTargetId에 슬롯 번호 입력"),
	HUD_QUICK_SLOT_SELECT = 15 UMETA(Tooltip = "HUD 퀵슬롯 선택, DTGsTutorialTargetId에 슬롯 번호 입력"),
	HUD_QUEST_SELECT = 16 UMETA(Tooltip = "HUD 퀘스트 슬롯 선택, 텔레포트 버튼 가림, DTGsTutorialTargetId에 QuestId 입력"),
	HUD_SCAN_SLOT_SELECT = 17 UMETA(Tooltip = "HUD 스캔슬롯 선택, DTGsTutorialTargetId에 슬롯 번호 입력"),
	HUD_POTION_ICON_SELECT = 18 UMETA(Tooltip = "HUD의 포션 아이콘 선택"),
	ITEM_ENCHANT_SELECT_ITEM = 19 UMETA(Tooltip = "인벤토리 강화팝업 아이템 선택. 현재 장착중인 무기를 선택함"),
	MULTI_LEVEL_RANK_SELECT = 20 UMETA(Tooltip = "업적. DTGsTutorialTargetId에 slotIndex 추가 입력"),
	BMSHOP_TAB_SELECT = 21 UMETA(Tooltip = "BMShop의 탭 선택. DTGsTutorialTargetId에 TabInfoId 추가 입력"),
	BMSHOP_PRODUCT_SELECT = 22 UMETA(Tooltip = "BMShop의 상품 선택. DTGsTutorialTargetId에 ProductId 추가 입력"),
	HUD_QUEST_TELEPORT = 23 UMETA(Tooltip = "HUD 퀘스트 슬롯의 퀵슬롯 선택, 텔레포트 버튼 보여줌, DTGsTutorialTargetId에 QuestId 입력"),
};

/**
 * 액션 종료 조건
 */
UENUM(BlueprintType)
enum class EGsTutorialActionConditionType : uint8
{
	UI_INTERACTION = 0 UMETA(Tooltip = "UI 인터렉션 후 바로 종료"),
	OPEN_UI = 1 UMETA(Tooltip = "특정 UI 열림. DTGsTutorialConditionOpenUI 추가입력"),
	LEARN_SKILL = 2 UMETA(Tooltip = "스킬 배움"),
	SUMMON_STATE_SHOW_RESULT = 3 UMETA(Tooltip = "소환 바로보기 or 모두보기 버튼 상태"),
	SUMMON_STATE_END = 4 UMETA(Tooltip = "소환 나가기 버튼 상태"),
};

/**
 * 튜토리얼UI 인터렉션 시 타입
 */
UENUM(BlueprintType)
enum class EGsTutorialUIInteractionType : uint8
{
	NONE = 0 UMETA(Tooltip = "에러. 인터렉션 없음"),
	CLICK_REGION = 1 UMETA(Tooltip = "영역 클릭"),
	CLICK_BUTTON = 2 UMETA(Tooltip = "버튼 클릭"),
	LONG_PRESS_ITEM_ICON = 3 UMETA(Tooltip = "아이콘 롱프레스"),
	INTERFACE_EVENT = 4 UMETA(Tooltip = "인터페이스 정의 객체의 이벤트"),
	CLICK_REGION_INTERFACE_EVENT = 5 UMETA(Tooltip = "영역 클릭 후 인터페이스 객체의 이벤트"),
};

/**
 * 튜토리얼UI 서브 다이얼로그 방향
 */
UENUM(BlueprintType)
enum class EGsTutorialUIDialogDirection : uint8
{
	NONE = 0 UMETA(Tooltip = "감춤"),
	LEFT,
	RIGHT
};

/**
 * 튜토리얼UI 화살표 이펙트 방향
 */
UENUM(BlueprintType)
enum class EGsTutorialUIArrowDirection : uint8
{
	NONE = 0 UMETA(Tooltip = "감춤"),
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
};

/**
 * 튜토리얼UI 영역에 추가로 보여줄 연출
 */
UENUM(BlueprintType)
enum class EGsTutorialUIEffectType : uint8
{
	NONE = 0 UMETA(Tooltip = "없음"),
	DRAG_DOWN,
	DRAG_LEFT,
	LONG_PRESS,
};

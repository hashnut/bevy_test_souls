// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TextKey.h"
#include "GsUIDefine.generated.h"

//----------------------------------------------------------------------------------------------------------------------
// UIDepth (ZOrder)정의
const int32 UI_DEPTH_DEFAULT = 0;
const int32 UI_DEPTH_HUD = 1;
const int32 UI_DEPTH_WINDOW = 10;
const int32 UI_DEPTH_DIALOGUE = 50; // 대화창
const int32 UI_DEPTH_POPUP = 100;
const int32 UI_DEPTH_LOADING = 849; // 로딩창
const int32 UI_DEPTH_TRAY = 500;
const int32 UI_DEPTH_TUTORIAL = 849; // 튜토리얼
const int32 UI_DEPTH_RECONNECT = 850; // 재연결UI
const int32 UI_DEPTH_SYSTEM_POPUP = 900;
const int32 UI_DEPTH_SYSTEM_TRAY = 1000;
const int32 UI_DEPTH_VFX_BTN_CLICK = 1000; // 버튼 클릭 시 출력할 VFX 기본값

//----------------------------------------------------------------------------------------------------------------------
// UGsUIWidgetBase 상속 UI Hide Flags
enum class EGsUIHideFlags : uint32
{
	UI_HIDE_NONE = 0,

	// ui flags
	UI_HIDE_DEFAULT = 1<<0,
	UI_HIDE_HUD = 1<<1,
	UI_HIDE_DEALSCROLL = 1<<2,
	UI_HIDE_DIALOG = 1<<3,
	UI_HIDE_WIDGETCOMPONENT = 1<<4,
	UI_HIDE_TRAY =  1<<5,
	UI_HIDE_SYSTEM_POPUP = 1<<6,
	UI_HIDE_SYSTEM_TRAY = 1<<7,
	UI_HIDE_SUB_DIALOG = 1<<8,
	UI_HIDE_ITEM_DETAIL = 1<<9,
	UI_HIDE_LOW_HP = 1 << 10,
	UI_HIDE_TRAY_SUMMON = 1 << 11,
	UI_HIDE_TRAY_CINEMATIC = 1 << 12, // 대화, 시네마틱에서 가려지는 TRAY
	UI_HIDE_SUMMON_MILEAGE_POPUP = 1 << 13,	// 소환 마일리지 팝업(뽑기 상태일때 보여야 함) 
	UI_HIDE_SUMMON_ITEM_DETAIL_POPUP = 1 << 14,	// 소환 아이템 상세 팝업(뽑기 상태일때 보여야 함) https://jira.com2us.com/jira/browse/C2URWQ-8110
	UI_HIDE_COSTUME_COLLECTION_POPUP = 1 << 15,	// 코스튬 콜랙션 팝업(아이템 상세에서 넘어옴)(뽑기 상태일때 보여야 함) https://jira.com2us.com/jira/browse/C2URWQ-8110
	UI_HIDE_FAIRY_COLLECTION_POPUP = 1 << 16,	// 페어리 콜랙션 팝업(아이템 상세에서 넘어옴)(뽑기 상태일때 보여야 함) https://jira.com2us.com/jira/browse/C2URWQ-8110

	// state flags 
	STATE_DIALOG = 1<<20,
	STATE_CINEMATIC = 1<<21,
	STATE_DIE = 1<<22,
	STATE_HUD_CAMERA_MODE = 1<<23,
	STATE_NPC_SHOP = 1<<24,	// NPC를 통해 여는 상점, 창고 등
	STATE_SCENE_WINDOW = 1<<25, // 다른 Scene을 활용하는 윈도우
	STATE_SUMMON = 1<<26,
	STATE_OFFLINE_PLAY = 1<<27,	// 2021/06/30 PKT - 오프라인 모드 Hide 설정
	STATE_MOVE_EFFECT = 1 << 28, 
	STATE_SAVE_BATTERY = 1 << 29,
	
	// settings (주의: 직접사용하지 말고 State를 사용)
	SETTING_DIALOG = STATE_DIALOG | UI_HIDE_DEFAULT | UI_HIDE_HUD | UI_HIDE_DEALSCROLL | UI_HIDE_WIDGETCOMPONENT | UI_HIDE_ITEM_DETAIL | UI_HIDE_TRAY_CINEMATIC,
	SETTING_CINEMATIC = STATE_CINEMATIC | UI_HIDE_DEFAULT | UI_HIDE_HUD | UI_HIDE_DEALSCROLL | UI_HIDE_WIDGETCOMPONENT | UI_HIDE_ITEM_DETAIL | UI_HIDE_LOW_HP | UI_HIDE_TRAY_SUMMON | UI_HIDE_TRAY_CINEMATIC,
	SETTING_DIE = STATE_DIE | UI_HIDE_DEFAULT | UI_HIDE_HUD,
	SETTING_HUD_CAMERA_MODE = STATE_HUD_CAMERA_MODE | UI_HIDE_DEFAULT | UI_HIDE_HUD | UI_HIDE_DEALSCROLL | UI_HIDE_WIDGETCOMPONENT | UI_HIDE_TRAY | UI_HIDE_SUB_DIALOG | UI_HIDE_ITEM_DETAIL | UI_HIDE_TRAY_CINEMATIC,
	SETTING_NPC_SHOP = STATE_NPC_SHOP | UI_HIDE_HUD | UI_HIDE_DEALSCROLL | UI_HIDE_WIDGETCOMPONENT,	
	SETTING_SCENE_WINDOW = STATE_SCENE_WINDOW | UI_HIDE_HUD | UI_HIDE_DEALSCROLL | UI_HIDE_DIALOG | UI_HIDE_WIDGETCOMPONENT | UI_HIDE_SUB_DIALOG,	
	SETTING_SUMMON = STATE_SUMMON | UI_HIDE_HUD | UI_HIDE_DEALSCROLL | UI_HIDE_DIALOG | UI_HIDE_WIDGETCOMPONENT | UI_HIDE_DEFAULT,
	SETTING_OFFLINE_PLAY = STATE_OFFLINE_PLAY | UI_HIDE_DEFAULT | UI_HIDE_HUD | UI_HIDE_WIDGETCOMPONENT | UI_HIDE_ITEM_DETAIL,
	SETTING_MOVE_EFFECT = STATE_MOVE_EFFECT | UI_HIDE_DEFAULT | UI_HIDE_HUD | UI_HIDE_DEALSCROLL | UI_HIDE_WIDGETCOMPONENT | UI_HIDE_SUB_DIALOG | UI_HIDE_ITEM_DETAIL | UI_HIDE_LOW_HP,
	SETTING_SAVE_BATTERY = STATE_SAVE_BATTERY | UI_HIDE_HUD | UI_HIDE_ITEM_DETAIL,

	UI_HIDE_ALL = 0xff,
};
ENUM_CLASS_FLAGS(EGsUIHideFlags);

//----------------------------------------------------------------------------------------------------------------------
// HUD 모드
UENUM(BlueprintType)
enum class EGsUIHUDMode : uint8
{
	// HUD 단순화 버튼으로 변경 가능한 모드 목록
	NORMAL,	// 기본 모드
	BATTLE, // 전투 모드
	CAMERA, // 촬영 모드

	MAX_MODE UMETA(Hidden),
	
	// HUD 단순화 버튼으로 변경 되지 않는 추가 모드 목록
	MENU,		// 메뉴 모드
	OBSERVER,	// 관전 모드
	INVADE,		// 침공전
	SPECTATOR,
};

//----------------------------------------------------------------------------------------------------------------------
// 시네마틱 자막 위치 타입
UENUM(BlueprintType)
enum class EGsCinematicCaptionType : uint8
{
	DEFAULT = 0, // 하단 출력
	CENTER, // 중앙 출력
};
//----------------------------------------------------------------------------------------------------------------------
// UIScale 타입
enum class EGsUIScaleFlags : uint32
{
	UI_SCALE_NONE = 0,

	UI_SCALE_HUD_STATE = 1 << 0, // 기본 HUD 상태. 이 경우만 UIScale을 적용하고 나머지는 1값 적용됨

	UI_SCALE_OTHER_STATE = 1 << 1, // HUD가 아닌 다른 창 상태
	UI_SCALE_LOADING = 1 << 2, // 로딩 상태
	UI_SCALE_SAVE_BATTERY = 1 << 3, // 절전모드 상태
	UI_SCALE_SUMMON = 1 << 4, // 소환 연출 상태
	UI_SCALE_NPC_DIALOG = 1 << 5, // NPC 대화 상태
	UI_SCALE_OTHER_GUILD = 1 << 6, // 다른 길드 정보보기 상태
};
ENUM_CLASS_FLAGS(EGsUIScaleFlags);
//----------------------------------------------------------------------------------------------------------------------
UENUM()
enum class  EGsMinimapAreaType : uint8
{
	PVP,
	SAFE,
	CHAOS,
};
//----------------------------------------------------------------------------------------------------------------------
const FTextKey RESURRECT_TEXT_KEY = TEXT("ResurrectionText");
const FTextKey LOSS_EXP_AMOUNT_TEXT_KEY = TEXT("LossExpAmount");
const FTextKey FIELD_BOSS_NAMESPACE_TEXT_KEY = TEXT("FieldBossText");
const FTextKey UI_COMMON_NAMESPACE_TEXT_KEY = TEXT("UICommonText");

const FName POPUP_STAT_DETAIL = TEXT("PopupStatDetail");
const FName POPUP_STAT_DISTRIBUTION = TEXT("PopupStatDistribution");

const FName WINDOW_BOSS_INFO = TEXT("WindowBossInfo");
const FName POPUP_BOSS_CONTRIBUTION = TEXT("PopupBossContribution");
const FName POPUP_BOSS_REWARD = TEXT("PopupBossReward");

const FName WINDOW_CONTINENTAL_MAP = TEXT("WindowContinentalMap");

const FTextKey CHANNEL_ALL_TEXT_KEY = TEXT("Channel_All");
const FTextKey CHANNEL_1ST_TEXT_KEY = TEXT("Channel_1st");
const FTextKey CHANNEL_EMPTY_TEXT_KEY = TEXT("Channel_Empty");

const FTextKey ELEMENT_FLAME_TEXT_KEY = TEXT("Element_Flame");
const FTextKey ELEMENT_FROST_TEXT_KEY = TEXT("Element_Frost");
const FTextKey ELEMENT_HOLY_TEXT_KEY = TEXT("Element_Holy");
const FTextKey ELEMENT_DARK_TEXT_KEY = TEXT("Element_Dark");

const FTextKey HM_TEXT_KEY = TEXT("HM");
const FTextKey HOUR_EVERY_TEXT_KEY = TEXT("Time_Hour_Every");
const FTextKey HOUR_SHARP_TEXT_KEY = TEXT("Time_Hour_Sharp");
const FTextKey TIME_MINUTE_N_TEXT_KEY = TEXT("Time_Minute_N");
const FText DOUBLE_ZERO = FText::FromString(TEXT("00"));

// 아이템 머터리얼: 파일 키
const FTextKey FILE_KEY_ITEM_MATERIAL = TEXT("ItemMaterialText");
// 아이템 머터리얼 - 없음: 텍스트 키
const FTextKey TEXT_KEY_ITEM_MATERIAL_NONE = TEXT("None");

// 속성: 파일 키
const FTextKey FILE_KEY_ELEMENT = TEXT("ElementText");
// 속성 - 없음: 텍스트 키
const FTextKey TEXT_KEY_ELEMENT_NONE = TEXT("Element_None");

const FTextKey GUILD_TEXT_KEY = FTextKey(TEXT("GuildText"));

const FString BOSS_SCENE_PATH = TEXT("/Game/Blueprint/Scene/BP_Scene_Boss.BP_Scene_Boss_C");
const FString GUILD_BOSS_SCENE_PATH = TEXT("/Game/Blueprint/Scene/BP_Scene_GuildBoss.BP_Scene_GuildBoss_C");

const FTextKey FIELD_BOSS_TEXT_KEY = TEXT("FieldBossText");

const FTextKey INVADE_TEXT_KEY = TEXT("InvadeText");

const FTextKey BOSS_SPAWN_AREA_TEXT_KEY = TEXT("UI_Text_BossSpawnArea");

const FTextKey UI_TEXT_NPERCENT = TEXT("UI_Text_NPercent");
const FTextKey UI_TEXT_CAN_TELEPORT = TEXT("CantTeleport");
const FTextKey UI_TEXT_CAN_USE_FUNCTION = TEXT("CantUseFunction");

const FTextKey FIELD_BOSS_SPAWN_TIME_TEXT_KEY = TEXT("SpawnTime");

const FTextKey WORLD_BOSS_TEXT_KEY = TEXT("WorldBossText");

// 주의: 선판정 동기화 이슈로 인해 자연스러워 보이기 위한 딜레이 값
// 빌보드(UGsUIBillboardNonPlayer), 골드경험치 획득(UGsUIHUDGainEffect), 타겟HP바(UGsUIHUDTargetHPBar)에서 사용
const float DELAY_HP_DISPLAY_TIME = 0.55f;

UENUM()
enum class EGsToggleType : uint8
{
	Active,
	Deactive,
	Auto,
};

//침공전에 입장했습니다. 
const int INVADE_ENTER_MESSAGE_ID = 10116;
//침공전에서 퇴장했습니다.
const int INVADE_EXIT_MESSAGE_ID = 10117;
//침공 지역에 입장했습니다.
const int INVADE_MAP_MESSAGE_ID = 10118;
//침공전이 시작됐습니다
const int INVADE_START_MESSAGE_ID = 10119;
//침공전이 끝났습니다
const int INVADE_END_MESSAGE_ID = 10120;
const FTextKey MAP_EVENT_TEXT_KEY = TEXT("MapEventText");
const FTextKey INVADE_MAP_DESC_TEXT_KEY = TEXT("Invade_Map_Desc");
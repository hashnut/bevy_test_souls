// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "DataSchema/GsSchemaEnums.h"
#include "../Message/GsMessageSystem.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "Camera/Define/GsCameraGameDefine.h"
#include "GameFramework/PlayerInput.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsGameUserSettings.generated.h"


UENUM()
enum class EGsOptionGraphicGrade : uint8
{
	CUSTOM = 0,		// 커스텀
	LOW = 1,		// 하
	MID = 2,		// 중
	HIGH = 3,		// 상 
	TOP = 4,		// 최상
};


/*
	옵션 - 그래픽 열거형
*/
UENUM()
enum class EGsOptionGraphic : uint8
{
	GRAPHIC_GRADE,			// 그래픽 등급 (커스텀, 하, 중, 상, 최상)
	RESOLUTION,				// 해상도 품질 (0~3)
	FRAME_RATE,				// 프레임제한 (20, 30, 45, 60)
	VIEW_DISTANCE,			// 시야거리 품질 (0~3)
	POST_PROCESS,			// 포스트프로세스 품질 (0~3)
	SHADOW,					// 그림자 품질 (0~3)O
	SSAO,					// 특수효과 - SSAO (0~2)	
	UISCALE,				// UI 배율
	PARTICLE_QUALITY,		// 파티클 시그니피컨트 컨트롤

	WINDOW_RESOLUTION,					// 윈도우 해상도 항목 타입 (0, 1, 2) 지정값 (3) 유저설정값
	WINDOW_RESOLUTION_X,				// 윈도우 해상도 X
	WINDOW_RESOLUTION_Y,				// 윈도우 해상도 Y

	WINDOW_SCREENMODE,					// 윈도우 스크린모드 (전체화면 / 전체창화면 /창화면)
	WINDOW_VSYNCENABLED,				// 윈도우 수직동기화 On/Off
	WINDOW_ANTIALIASING,				// 윈도우 안티얼라이어싱 (0~3)

	ANTIALIASING_QUALITY,	// 안티얼라이어싱 퀄리티
	SHADING_QUALITY,		// Shading 퀄리티
	EFFECT,					// 비주얼효과 품질 (0~3)
	TEXTURE,				// 텍스쳐 품질 (0~3)
	FOLIAGE,				// 수풀밀도 품질 (0~3)

	FSR_1_ONOFF, // FSR 1.0 On/Off

	OTHER_PC_RENDERING,		// 다른 캐릭터 표현 정도 : 10명, 25명, 50명, 100명(이번 2월 배포에는 페어리 날개 변환 기능만 추가, 차후에 네이밍 변경 필요)

	MAX
};

/*
	옵션 - 오디오 열거형
*/
UENUM()
enum class EGsOptionAudio : uint8
{
	WHOLE_ON,		// 전체 ON/OFF
	WHOLE_VOLUME,	// 전체 볼륨 (0~100)

	MUSIC_ON,		// 음악 ON/OF
	MUSIC_VOLUME,	// 음악 볼륨 (0~100)

	EFFECT_ON,		// 효과음 ON/OFF
	EFFECT_VOLUME,	// 효과음 볼륨 (0~100)

	NOTICE_ON,		// 알림음 ON/OFF
	NOTICE_VOLUME,	// 알림음 볼륨 (0~100)

	FOCUS_VOLUME_MULTIPLER, // 게임이 활성화되었을 때만 소리 재생

	MAX
};

/*
	옵션 - 전투 열거형
*/
UENUM()
enum class EGsOptionCombat : uint8
{
	POTION_PRIORITY,		// 0 : 낮은 등급부터, 1 : 높은 등급부터
	// 2021/09/14 PKT - 오프라인 관련 해당 옵션은 서버와 동기화 이슈로 이곳에서 제외
	//AUTO_COMBAT_RANGE,		// 자동사냥시 전투복귀 거리 타입 (0~3)
	// 2021/09/14 PKT - 오프라인 관련 해당 옵션은 서버와 동기화 이슈로 이곳에서 제외
	//ITEM_LOOT_GRADE,		// 0 : 모든, 1 : 고급이상, 2 : 희귀 이상
	HP_RATE_ALERT_LEVEL,	// HP 낮을 때 경고 레벨, 0 : 안함, 1 : 10%, 2 : 25%, 3 : 50%
	PVP_ATTACKED_ALERT,		// PVP 공격받을 때 경고 여부 ON/OFF
	QUICKSLOT_PAGE_INDEX,	// QuickSlot Page Index 저장
	INVENTROY_LONGPRESS_ENABLE,	// 인벤토리에서 롱프레스를 사용할것인지

	MANNER_MODE,					// 매너 모드(0: ON, 1: 파티, 기사단 에외, 3: OFF)
	NONAUTO_MOVE_KEEP_AUTO,			// 수동 이동시, 자동 전투 유지(0: OFF, 1: ON)

	DAMAGE_FLOATER,					// 데미지 플로터 출력(0: OFF, 1: ON)
	DROP_ITEM_NAME,					// 드랍 아이템 이름 출력(0: OFF, 1: ON)
	NPC_NAME,						// NPC 이름 출력(0: OFF, 1: ON)
	NPC_FUNCTION_ICON,				// NPC 기능 아이콘 출력(0: OFF, 1: ON)
	PLAYER_NAME,					// PC 이름 출력(0: 모두 끄기, 1: 내 이름만 표기, 2: 다른 유저만 표기, 3: 모두 표기)
	PLAYER_SCAN_EFFECT,				// 수동 스캔 시 스캔(음파) 이펙트 표현(0: OFF, 1: ON)
	NPC_TALK_BALLOON,				// NPC 말풍선 출력(0: OFF, 1: ON)
	NPC_FUNCTION_UI_NAME,			// npc function ui name(0: OFF, 1: ON)

	KEEP_TARGET_GROUND_TOUCH,		// Do you keep the target when touching the terrain ?(0: OFF, 1: ON)		
	AUTOSKILL_USE_ADDTARGETBUTTON,	//
	COMBAT_POWER_UI,
	ITEM_SLOT_EXPANSION,			// 아이템 슬롯 확장(0: OFF, 1: ON)

	MAX
};

UENUM()
enum class EGsOptionUserInfo : uint8
{
	VID_NAME,				// VID 정보
	//ACCOUNT_NAME,			// AccountID 정보
	LAST_ENTER_CHARACTER,	// 마지막으로 접속한 캐릭터
	AUTO_LOGIN,				// 자동로그인	
	LAST_ENTER_WORLD_NAME,	// 마지막 접속 서버
	BACKTO_LOBBY,			// 로비로
	LOGIN_ID,				// 아이디
	LOGIN_PASSWORD,			// 비밀번호
	LANGUAGE,				// 언어 설정
	LANGUAGE_VOICE,			// 음성 언어 설정

	MAX
};

UENUM()
enum class EGsOptionChat : uint8
{
	NORMAL_CHAT,
	SERVER_CHAT,
	PARTY_CHAT,
	ALLYGUILD_CHAT,
	GUILD_CHAT,
	WHISPER,
	INVADE_CHAT,
	SYSTEM,

	OPTCHAT_MAX = SYSTEM,

	EMOJI_POPUP,
	TALK_POPUP,

	OPTION_GROUP,

	MAX
};

UENUM()
enum class EGsOptionBlessItem : uint8
{
	USE_STGMATA = 1,
	USE_TEAR = 2,
};

// 출력
UENUM()
enum class EGsOptionOutput : uint8
{		
	SUMMON_SKIP				=		0,			// 소환 스킵 ON/OFF	
	QUEST_SKIP				=		1,			// 퀘스트 대화 자동 넘기기 ON/OFF
	IS_USE_CAMERA_SHAKE		=		2,			// 카메라 쉐이크 사용하는가
	WINDOW_KEY_MAPPTIN		=		3,			// 윈도우 키보드 맵핑
	SUMMON_SKIP_COMPOSE		=		4,			// 합성 소환 스킵 ON/OFF
	QUEST_AUTO_TELEPORT		=		5,			// 반복 퀘스트 자동진행 중 텔레포트 사용 유무 ON/OFF

	MAX,
};

/*
	옵션 - 알림
*/
UENUM()
enum class EGsOptionAlert : uint8
{
	ALL_ALERT = 0,				// 모든 알림 ON/OFF
	CONSOLE_ALERT = 1,			// 공지 알림 ON/OFF
	CONSOLE_ALERT_NIGHT = 2,	// 야간 알림 ON/OFF
	ITEM_ADD_ALERT = 3,			// 아이템 힉득 알림

	ITEMGAIN_GLOBAL_MESSGE_NAME_HIDE = 23,	// 획득 알림 글로벌 메시지에 이름 숨김
	
	MAX,
};

/*
	옵션 - 로그인 알림
*/
UENUM()
enum class EGsOptionLoginAlert : uint8
{
	CONNECT_ALERT = 0,			// 접속 알림
	GUILD_ALERT = 1,			// 길드 티커 알림 ON/OFF
	BUDDY_ALERT = 2,			// 친구 티커 알림 ON/OFF

	MAX,
};

/*
	옵션 - 알림
*/
UENUM()
enum class EGsOptionNotificationAlert : uint8
{
	COMBAT_STATE_ALERT = 0,					// 전투 관련 알림
	COMBAT_STATE_ALERT_INTERVALTIME = 1,		// 전투 관련 알림 시간 조절

	PVP_ALERT = 2,				// 자동 사냥 중 다른 플레이어에게 피격
	PVP_RINGER_ALERT = 3,		// 자동 사냥 중 다른 플레이어에게 피격 벨소리 알림
	PVP_VIBRATE_ALERT = 4,		// 자동 사냥 중 다른 플레이어에게 피격 진동 알림
	HP_RATE_ALERT = 5,			// 캐릭터 체력 위험 알림
	HP_RATE_RINGER_ALERT = 6,	// 캐릭터 체력 위험 벨소리 알림
	HP_RATE_VIBRATE_ALERT = 7,	// 캐릭터 체력 위험 진동 알림

	EQUANIMITY_ALERT = 8,					// 게임 편의 알림
	EQUANIMITY_ALERT_INTERVALTIME = 9,		// 게임 편의 알림 시간 조절

	AUTO_POTION_ALERT = 10,		// HP 자동 물약 부족 알림
	HP_POTION_ALERT = 11,			// 물약 부족 알림
	HP_POTION_RINGER_ALERT = 12,	// 물약 부족 벨소리 알림
	HP_POTION_VIBRATE_ALERT = 13,	// 물약 부족 진동 알림	

	WEIGHT_ALERT = 14,			// 무게 알림
	WEIGHT_RINGER_ALERT = 15,	// 무게 알림
	WEIGHT_VIBRATE_ALERT = 16,	// 무게 알림

	DEATH_ALERT = 17,				// 사망 알림 ON/OFF
	DEATH_RINGER_ALERT = 18,		// 사망 알림 ON/OFF	
	DEATH_VIBRATE_ALERT = 19,		// 사망 알림 ON/OFF		

	MAX,
	//CONNECT_INTERVATIME = 20,				// 접속 알림 시간 조절
};

/*
	옵션 - 알림
*/
UENUM()
enum class EGsOptionOfflineAlert : uint8
{
	OFFLINE_PLAY_ALERT = 0,					// 오프라인 접속 알림
	OFFLINE_PLAY_ALERT_INTERVALTIME = 1,	// 오프라인 알림 시간 조절
	OFFLINE_INVEN_ALERT = 2,				// 오프라인 인벤토리 알림
	OFFLINE_WEIGHT_ALERT = 3,				// 오프라인 무게 알림
	OFFLINE_PLAYFINISHED_ALERT = 4,			// 오프라인 플레이 종료 알림
	OFFLINE_ATTACKED_ALERT = 5,				// 오프라인 피격 알림
	OFFLINE_DEATHALERT = 6,					// 오프라인 사망 알림
	OFFLINE_DUNGEON_TIMEOUT_ALERT = 7,		// 오프라인 던전 제한 시간 종료 알림
	OFFLINE_EQUIP_ITEM_ACQUISITION_ALERT = 8,	// 2023/4/28 PKT - 장착 가능한 장비 아이템 획득 알림
	OFFLINE_SKILL_BOOK_ACQUISITION_ALERT = 9,	// 2023/4/28 PKT - 스킬북 획득 알림
	OFFLINE_MARKET_SALE_COMPLETED_ALERT = 10,	// 2023/4/28 PKT - 거래소 등록 물품 판매 완료

	MAX,
};


/**
 * 2022/09/30 PKT - PK Book 옵션 - 알림
 */
UENUM()
enum class EGsOptionPKBookAlert : uint8
{
	CLOSE_TARGET_DETECTION_ALERT,					// 경계대상 근접 알림
	// 2022/09/30 PKT - Scroll
	CLOSE_TARGET_DETECTION_ALERT_INTERVAL,			// 2022/09/30 PKT - 경계대상 근접 알림 - 주기
	CLOSE_TARGET_DETECTION_ALERT_DISTANCE,			// 2022/09/30 PKT - 경계대상 근접 알림 - 거리

	// 2022/09/30 PKT - Radio
	CLOSE_TARGET_DETECTION_RINGER_ALERT,			// 2022/09/30 PKT - 경계대상 근접 알림 - 소리
	CLOSE_TARGET_DETECTION_VIBRATE_ALERT,			// 2022/09/30 PKT - 경계대상 근접 알림 - 진동	
};


/*
	카메라 모드 백업 값
*/
UENUM()
enum class EGsOptionCameraModeBackup : uint8
{
	IS_BACKUP = 0,		// 백업 되었는지
	CAM_MODE = 1,		// 카메라 모드
	
	ZOOM_VAL_1 = 2,		// 줌값(1)
	ZOOM_VAL_2 = 3,		// 줌값(2)
	ZOOM_VAL_3 = 4,		// 줌값(3)

	PITCH_VAL_1 =	5,		// PITCH 값(1)
	PITCH_VAL_2 =	6,		// PITCH 값(1)
	PITCH_VAL_3 =	7,		// PITCH 값(1)

	QUAT_X = 8,					// Quat x
	QUAT_Y = 9,					// Quat y
	QUAT_Z = 10,				// Quat z
	QUAT_W = 11,				// Quat w

	DIFF_ROT_TO_PITCH = 12,	// back view에서 사용하는 차이값 pitch
	DIFF_ROT_TO_YAW = 13,	// back view에서 사용하는 차이값 yaw
	DIFF_ROT_TO_ROLL = 14,	// back view에서 사용하는 차이값 roll

	FIXED_QUARTER_YAW_TYPE = 15, // FIXED QUARTER YAW TYPE
};

/*
*  스캔 옵션 값
*/
UENUM()
enum class EGsOptionScan : uint8
{
	DETECT_TYPE				= 0,		// 감지 타입
	IS_AUTO_SCAN			= 1,		// 자동 스캔인가
	SCAN_TIMMER_SECOND		= 2,		// 스캔 타이머 시간
	DETECT_GROUP_TYPE		= 3,		// detect group
};

/*
*  절전 모드 옵션 값
*/
UENUM()
enum class EGsOptionSaveBattery : uint8
{
	SAVEBATTERY_TIMER_MINUTE = 0,	// 자동 진입 시간
	IS_PVP_AUTO_OFF = 1,			// PVP 피격 시, 자동 해제
	
	MAX
};

/*
* 오토 옵션 값
*/
UENUM()
enum class EGsOptionAuto : uint8
{
	AUTO_SKILL_MANA_LIMIT_RATE		=	0,		// 자동 스킬 마나 제한 RATE 값
	IS_ALWAYS_USE_AUTO_BUFF_SKILL	=	1,		// 자동 버프 스킬 항상 사용할지(ON이면 위에RATE 무시하고 사용)
	IS_AUTO_PARTY_ACCEPT			=	2,		// 자동 파티 수락
	
	AUTO_ITEM_LOOT_OTHER_USERS		=	3,		// 자동 아이템 루팅: 다른 유저꺼 획득할지
	PERSONAL_TRADING_AUTO_REJECT	=	4,		// 개인 거래 자동 거절

	MAX
};
/*
* 입력 옵션 값
*/
UENUM()
enum class EGsOptionInput : uint8
{
	IS_USE_TOUCH_MOVE			=	0,			// 터치 이동 사용할지

	MAX
};

/*
* 윈도우 해상도 버튼리스트
*/
UENUM()
enum class EGsOptionWindowResolution : uint8
{
	RESOLUTION_1 = 0,
	RESOLUTION_2 = 1,
	RESOLUTION_3 = 2,
	USER_SETTING = 3,

	MAX
};

/*
* HUD 출력 옵션 값
*/
UENUM()
enum class EGsOptionHud : uint8
{
	IS_SHOW_SERVER_REVISION_AND_LOCATION = 0,	// 서버 Revision 정보와 위치 정보를 출력할 것인가?
	MAX
};


/*
* server option: auto drop
*/

UENUM()
enum class EGsOptionServerAutoDrop
{
	ItemGrade		= 0,							// auto root drop item grade(all, magic, rare, off)
	ItemWeight		= 1,							// auto root drop item weight(off, 50%, 70%, 90%)
};


UENUM()
enum class EGsPickUpItemOption
{
	ItemGainCommonConsume = 0,	//일반 소모품의 습득을
	ItemGainSpecialConsume,		//특수 소모품의 습득을
	ItemGainCommonScroll,		//일반 주문서의 습득을
	ItemGainSpecialScroll,		//특수 주문서의 습득을
	ItemGainEnchantScroll,		//강화 주문서의 습득을
	ItemGainOtherNormal,
	ItemGainOtherSpecial,
	ItemGainOtherEtc,
};

UENUM()
enum class EGsOptionAutoDecompose
{
	FIELD_BOSS = 0, // 필드 보스 아이템 적용 여부

	ITEM_COLLECTION, // 컬렉션 가능한 아이템 자동 분해 여부

	EQUIP_NORMAL, // 장비 아이템 노멀
	EQUIP_MAGIC, // 장비 아이템 매직
	EQUIP_RARE, // 장비 아이템 레어

	SKILLBOOK_NORMAL, // 스킬북 아이템 노멀
	SKILLBOOK_MAGIC, // 스킬북 아이템 매직
	SKILLBOOK_RARE, // 스킬북 아이템 레어

	SLATE_NORMAL, // 석판 아이템 노멀
	SLATE_MAGIC, // 석판 아이템 매직
	SLATE_RARE, // 석판 아이템 레어

};

UENUM()
enum class OptionSystemMessage : uint8
{
	MIN	   = static_cast<uint8>(EGsOptionItem::OptionSystemMessageGradeNormal),
	NORAML = MIN,
	MAGIC  = static_cast<uint8>(EGsOptionItem::OptionSystemMessageGradeMagic),
	RARE   = static_cast<uint8>(EGsOptionItem::OptionSystemMessageGradeRare),
	EPIC   = static_cast<uint8>(EGsOptionItem::OptionSystemMessageGradeEpic),
	UNIQUE = static_cast<uint8>(EGsOptionItem::OptionSystemMessageGradeUnique),
	LEGEND = static_cast<uint8>(EGsOptionItem::OptionSystemMessageGradeLegend),
	MAX   = LEGEND,
};

UENUM()
enum class EGsCombatPower : uint8
{
	Off,
	ShowStatOnly,
	ShowAll,
	MAX
};

struct FGsSchemaOptionConfig;
/*
	그래픽옵션의 기본항목들은 UE4 - UGameUserSettings의 것을 사용한다.
	따라서, 이 클래스에서는 기본 제공하지 않는 그래픽옵션들만 별도 제어하고, 기본 제공하는 것들은 Bridge 해주는 기능만 제공

	그래픽옵션 외의 것들은 당연히 별도 구현

	TODO)
		추후 그래픽 옵션 외 다른 옵션들이 생길 경우 ini 저장이 중복적으로 발생치 않도록 추가 수정해 주어야 함
		UGameUserSettings::ApplySetting의 경우 내부 코드에 SaveSetting 호출이 포함되어 있다.
		 - 이를 override 하던 해서 옵션창을 닫을 때 모든 ini 들이 한번에 기록되도록 수정해야 함!!!
		
 */
UCLASS(config = GameUserSettings, meta = (DisplayName = "GameUserSettings"))
class T1PROJECT_API UGsGameUserSettings : public UGameUserSettings
{
	GENERATED_UCLASS_BODY()

private:
	static const int8	MAX_OPT_GRAPHIC_LEVELS = 4;
	static const int8	MAX_OPT_SSAO_LEVELS = 2;	
	static const int32	MAX_OPT_AUDIO_VOLUME = 100;	

	const TCHAR* INI_SECTION_GRAPHIC = TEXT("GsSettingGraphicGroup");
	const TCHAR* INI_SECTION_GRAPHIC_SCALABILITY = TEXT("ScalabilityGroups");
	const TCHAR* INI_SECTION_AUDIO = TEXT("GsSettingAudioGroup");
	const TCHAR* INI_SECTION_USERINFO = TEXT("GsSettingUserInfoGroup");
	const TCHAR* INI_SECTION_COMBAT = TEXT("GsSettingCombatGroup");
	const TCHAR* INI_SECTION_CHATOPT = TEXT("GsSettingChatGroupts");
	const TCHAR* INI_SECTION_BLESSOPT = TEXT("GsSettingBlessGroupts");
	const TCHAR* INI_SECTION_CHATMACRO = TEXT("GsSettingChatMacros");
	const TCHAR* INI_SECTION_OUTPUT = TEXT("GsSettingOutputGroup");
	const TCHAR* INI_SECTION_NOTICE = TEXT("GsSettingNotice");
	const TCHAR* INI_SECTION_MARKET = TEXT("GsSettingMarket");
	const TCHAR* INI_SECTION_CAMERA_MODE_BACKUP = TEXT("GsSettingCameraModeBackup");
	const TCHAR* INI_SECTION_SCAN = TEXT("GsSettingScan");
	const TCHAR* INI_SECTION_SAVEBATTERY = TEXT("GsSettingSaveBattery");
	const TCHAR* INI_SECTION_AUTO = TEXT("GsSettingAuto");
	const TCHAR* INI_SECTION_INPUT = TEXT("GsSettingInput");
	const TCHAR* INI_SECTION_HUD = TEXT("GsSettingHud");
	const TCHAR* INI_SECTION_COMMUNITYEVENT_REDDOT = TEXT("GsSettingCommunityEventRedDot");
	const TCHAR* INI_SECTION_COMPOSITION = TEXT("GsSettingComposition");
	const TCHAR* INI_SECTION_SYSTEMTICKER = TEXT("GsOptionSystemMessage");

	const int32 ResolutionValueArray[MAX_OPT_GRAPHIC_LEVELS] = { 50, 67, 77, 100 };
	const int32 FrameRateValueArray[MAX_OPT_GRAPHIC_LEVELS] = { 30, 45, 60, 60 };

	const int32 OtherPCRenderingValueArray[MAX_OPT_GRAPHIC_LEVELS] = { 10, 25, 50, 100 };
	
private:
	// Graphic 옵션
	TMap<EGsOptionGraphic, int32>		_graphicTable;
	int32								_graphicAutoTuneLastTempLevel = -1;
	int32								_graphicGrade = 3;

	// Audio 옵션
	TMap<EGsOptionAudio, int32>			_audioTable;

	// userInfo 옵션
	TMap<EGsOptionUserInfo, FString>	_userInfoTable;

	// Combat 옵션
	TMap<EGsOptionCombat, int32>		_combatTable;

	// Chat 옵션
	TMap<EGsOptionChat, bool>			_chatTable; //array..
	TArray<uint64>						_chatGroupTable;

	TMap<EGsOptionBlessItem, bool>		_autoUseBlessRestore;

	// 출력 옵션
	TMap<EGsOptionOutput, int32>		_outputTable;

	// 알림 옵션
	TMap<EGsOptionAlert, int32>			_alertTable;	

	// 오프라인 알림 옵션
	TMap<EGsOptionOfflineAlert, int32>		_offlineAlertTable;

	TMap<int8, FText>	_chatMacro;
	
	// 거래소 검색 옵션
	TArray<FString> _marketSearchList;

	// 카메라 모드 백업
	TMap<EGsOptionCameraModeBackup, float> _cameraModeBackup;
	// 스캔 옵션 값
	TMap<EGsOptionScan, int32>	_scanTable;

	// 절전 모드 옵션 값
	TMap<EGsOptionSaveBattery, int32> _saveBatteryTable;

	// 오토 옵션 값
	TMap<EGsOptionAuto, float> _autoTable;
	// 입력 옵션 값
	TMap<EGsOptionInput, int32> _inputTable;

	// HUD 출력 옵션 값
	TMap<EGsOptionHud, int32> _hudTable;

	// 커뮤니티소녀 배너 레드닷 정보
	TArray<FString> _communityEventRedDot;
	//TMap<CommunityEventId, FString> _communityRedDotDatas;

	MsgSystemHandle _systemMsgHandle;

#if WITH_EDITOR || PLATFORM_WINDOWS
	FDelegateHandle _handleResize;
#endif

	const FGsSchemaOptionConfig* _optionConfig;

	bool _isResolutionDirty{ false };

	bool _allGameAlert = true;

	float _resolutionScale = 0.0f;

	struct CompositionOpt
	{
		bool leftLock{ false };
		bool rightLock{ false };
	};
	CompositionOpt _composeCostume;
	CompositionOpt _composeFairy;
	// GsGameInstance::Initialize로부터 호출

protected:
	void ApplyVsync();
	void SaveConfigVsync();

public:
	void OnInitialize();
	void Finalize();

protected:
	void RegisterMessages();
	void UnRegisterMessages();
	void OnHivePush(const IGsMessageParam* InParam);

public:
	int GetPushData();

	// GsOptionManager::Update로부터 호출
public:
	void Update();
	void Update(float inTick);

	// OnInitialize 시점에 데이터 로드
private:
	void LoadGraphicSettings();
	void LoadAudioSettings();
	void LoadUserInfoSettings();
	void LoadCombatSetting();
	void LoadChatOptionSetting();
	void LoadBlessItemOptionSetting();
	void LoadChatMacro();
	void LoadOutputSetting();
	void LoadAlertSetting();
	void LoadMarketSetting();
	void LoadCameraModeBackupSetting();
	void LoadScanSetting();
	void LoadSaveBatterySetting();
	void LoadAutoSetting();
	void LoadInputSetting();
	void LoadHudSetting();
	void LoadCommunityEventRedDotSetting();
	void LoadCompositionOption();

	// GConfig에 각종 데이터 쓰기
private:
	void SaveGraphicSetting(const TCHAR* key, int32 val);
	void SaveAudioSetting(const TCHAR* key, int32 val);
	void SaveUserInfoSetting(const TCHAR* key, const FString& val);
	void SaveUserCombatSetting(const TCHAR* key, int32 val);
	void SaveChatOptionSetting(const TCHAR* key, bool isVal);
	void SaveChatMacro(const TCHAR* key, const FText& macro);
	void SaveOutputSetting(const TCHAR* key, int32 val);
	void SaveAlertSetting(const TCHAR* key, int32 val);
	void SaveMarketSetting();
	void SaveSaveBatterySetting(const TCHAR* key, int32 val);
	void SaveAutoSetting(const TCHAR* key, float val);
	void SaveInputSetting(const TCHAR* key, int32 val);
	void SaveHudSetting(const TCHAR* key, int32 val);
	void SaveCommunityEventRedDotSetting();
	void SaveCompositionSetting(const TCHAR* key, bool isVal);
	void SaveCombatUI();
	
public:
	void SaveCameraModeBackupSetting(EGsCameraMode In_mode, TMap<EGsCameraMode, float> In_dicZoomVal, 
		TMap<EGsCameraMode, float> In_dicPitchVal,
		FQuat In_quat, 
		EGsCameraFixedQuarterYawType In_fixedQaurterYawType);

	void SaveScanSetting(uint32 In_detectGroupType ,uint32 In_detectType, bool In_isAutoScan, int In_scanTimmerIndex);

	// GConfig에 기록한 데이터들을 ini로 내보내기
	// 주의) UI에서 "저장하기" 버튼을 누르거나, 옵션 창을 닫을때 호출해 주어야 함
public:
	virtual void SaveUserSettings() ;	
//	virtual void ApplySettings(bool bCheckForCommandLineOverrides) override;

	/*
		Graphic
	*/
private:
	void ApplyGraphicSettings();

//#if PLATFORM_ANDROID || PLATFORM_IOS
//public:
//	void SetUserApplyGraphicSettings();
//#endif

	// FSR은 그래픽 옵션이지만 별도로 제어하는게 맞아 보인다 그리고 기본적으로 지원한다면 On상태를 권장한다.
	/*
	* FidelityFX Super Resolution(FSR)은 저해상도 입력에서 고해상도 프레임을 생성하기 위한 오픈 소스 고품질 솔루션입니다. 
	고품질 가장자리 생성에 특히 중점을 둔 최첨단 알고리즘 모음을 사용하여 기본 해상도에서 직접 렌더링하는 것과 비교하여 성능이 크게 향상됩니다. 
	FSR은 하드웨어 광선 추적과 같은 비용이 많이 드는 렌더링 작업을 위한 "실용적인 성능"을 가능하게 합니다
	*/
#if PLATFORM_ANDROID || PLATFORM_IOS
	void ApplyFSRSetting();
#endif

private:
#if WITH_EDITOR || PLATFORM_WINDOWS
	void FirstRunSetting();	
#endif
#if PLATFORM_ANDROID || PLATFORM_IOS
	void FirstRunSettingMobile();
#endif
	// 해상도 변경 함수
	void ApplyGraphicResolutionSetting();
	// 프레임 수 제한
	void ApplyFrameRate();
	// Graphic 옵션 중 WZ 전용 Additional 항목들 적용
	void ApplyGraphicAdditionalSettings();
	void ApplyGraphicScalabiltySettings();

#if WITH_EDITOR || PLATFORM_WINDOWS
	// r.DefaultFeature.AntiAliasing
	// 0: off (no anti-aliasing)
	// 1: FXAA (faster than TemporalAA but much more shimmering for non static cases)
	// 2: TemporalAA (default)
	// 3: MSAA (Forward shading only)
	void ApplyAntiAliasing();
	void ApplyVSyncEnabled();
#endif

	// Graphic - Getter/Setter
public:
	void GetGraphicSettings(OUT TMap<EGsOptionGraphic, int32>& outGraphicData);
	void GetGraphicSettingByType(EGsOptionGraphic optType, OUT int32& outOptVal);
	/* const { return _graphicTable; }*/
	float GetNormalizedUIScaleValue();

	// Graphic 옵션은 외부에서 이 함수로 값 설정
	// 참고) 개별 UI 조작이 아니라 옵션창을 닫을 때 한번에 적용
	void SetGraphicSettings(const TMap<EGsOptionGraphic, int32>& graphicOpts, bool bImmediateApply = true);
	void SetGraphicSettings(EGsOptionGraphicGrade GraphicGrade);

	/*
	*
		screenPercentage
		VisualEffectQuality
		ParticleQuality
	*/
	// 코스튬, 페어리 UI 오픈시 그래픽옵션이 낮아서 퀄리티 낮아보이는걸 올리기 위해 캐릭터 관련 셋팅만 Get하게
	int32 undoResolution;
	int32 undoParticleQuality;
	int32 undoEffect;

	void ApplyCharacterTopGraphicSettings();
	void UndoApplyCharacterTopGraphicSettings();
#if WITH_EDITOR || PLATFORM_WINDOWS
	void SetWindowResolution(int32 optVal);
	void SetWindowScreenMode(int32 optVal);
	void UpdateApplyResolutionSettings(bool bCheckForCommandLineOverrides);

	int GetWindowScreenMode();
	int GetWindowResolution();

	/**
	 * Called when the game window is moved
	 */
	void OnGameWindowMoved(const TSharedRef<SWindow>& WindowBeingMoved);
#endif
private:
	void SetGraphicSettingByType(EGsOptionGraphic optType, int32 optVal);

	/*
		Audio
	*/
public:
	// 초기화할 때 모든 Sound 옵션 적용
	void ApplyAudioSettings();
private:
	// 개별 적용 함수들
	void ApplyAudioSettingWholeOn();
	void ApplyAudioSettingWholeVolume();
	void ApplyAudioSettingMusicOn();
	void ApplyAudioSettingMusicVolume();
	void ApplyAudioSettingEffectOn();
	void ApplyAudioSettingEffectVolume();
	void ApplyAudioSettingNoticeOn();
	void ApplyAudioSettingNoticeVolume();
	void ApplyAudioFocusVolumeMultipler();

#if WITH_EDITOR || PLATFORM_WINDOWS
	FIntPoint CheckSupportedFullscreenResolution(FIntPoint In_CurResolution);
	FIntPoint GetMaxFullscreenResolution(FIntPoint In_CurResolution);

	// 해당 PC에서 지원하는 전체화면 해상도 리스트를 셋팅한다.
	/*TArray<FIntPoint> _fullscreenResolutions;
	TArray<FIntPoint> _windowedResolutions;*/

	//void LoadWindowResolutionList();
	void GetResolutions(TArray<FIntPoint>& OutResolutions);

	void GetFullscreenResolutions(TArray<FIntPoint>& OutFullscreenResolutions);
	void GetWindowedResolutions(TArray<FIntPoint>& OutWindowedResolutions);

public:
	int GetResolutionsNum();
#endif

public:
	const TMap<EGsOptionAudio, int32>& GetAudioSettings() const { return _audioTable; }

	// Sound 옵션은 외부에서 아래 함수들로 개별값 설정
	// 참고) 옵션 UI에서 개별을 조작할 때마다 즉시 ini에 저장 및 적용
	void SetAudioSettingByType(EGsOptionAudio optType, int32 optVal);
	int32 GetAudioSettingByType(EGsOptionAudio optType);

	bool SetGraphicGrade(EGsOptionGraphic optType, int32 optVal);
	bool SetGraphicCustom(EGsOptionGraphic optType, int32 optVal);

	bool SetAlert(EGsOptionAlert optType, int32 optVal, bool inIsAllChange = false);

	// UserInfo - Getter/Setter
public:
	const TMap<EGsOptionUserInfo, FString>& GetUserInfoSettings() const { return _userInfoTable; }
	const FString GetUserInfoSetting(EGsOptionUserInfo optType) const;

	// 유저 정보 저장
	void SetUserInfoSettingByType(EGsOptionUserInfo optType, FString optVal, bool bImmediate = true);

	// combat
public:
	//개별 적용 함수들
private:

	//	POTION_PRIORITY,		// 0 : 낮은 등급부터, 1 : 높은 등급부터
	//	AUTO_COMBAT_RANGE,		// 자동사냥시 전투복귀 거리 타입 (0~3)
	//	MANNER_MODE,			// ON/OFF
	//	ITEM_LOOT_GRADE,		// 0 : 모든, 1 : 고급이상, 2 : 희귀 이상
	//	HP_RATE_ALERT_LEVEL,	// HP 낮을 때 경고 레벨, 0 : 안함, 1 : 10%, 2 : 25%, 3 : 50%
	//	PVP_ATTACKED_ALERT,		// PVP 공격받을 때 경고 여부 ON/OFF
	//	QUICKSLOT_PAGE_INDEX,	// QuickSlot Page Index 저장

	void ApplyAutoCombatRange();
	void ApplyItemLootGrade();

	void ApplyTargetList();
	
public:
	const TMap<EGsOptionCombat, int32>& GetCombatSettings() const { return _combatTable; }

	// Combat 정보
	const int32 GetCombatSetting(EGsOptionCombat optType) const;
	void SetCombatSettingByType(EGsOptionCombat optType, int32 optVal, bool bImmediate = true);

public:
	// 자동 로그인 상태 체크가 자주 불려 따로 뺌
	const bool IsAutoLogin() const
	{
#if WITH_EDITOR
		return GetUserInfoSetting(EGsOptionUserInfo::AUTO_LOGIN).ToBool();
#else
		return false;
#endif
	}

public:
	const TMap<EGsOptionChat, bool>& GetChatOptions() { return _chatTable; }
	const bool GetChatOption(EGsOptionChat type) const;
	void SetChatOption(EGsOptionChat type, bool optVal, bool isSave = false);	

	const int GetGroupChatOptions(TArray<uint64>& options) const;
	const bool GetGroupChatOption(uint64 roomId) const;
	void SetGroupChatOption(uint64 roomId, bool optVal, bool isSave = false);
	void ClearGroupChatOption();

	const TMap<int8, FText>& GetChatMacros() { return _chatMacro; }
	const FText& GetChatMacro(int8 index) const;
	void SetChatMacro(int8 type, const FText& text, bool isSave = false);

public:
	const bool GetAutoUseTear() const;
	const bool GetAuroUseStigmata() const;
	void SetAutoUseBlessItem(bool tearOptVal, bool stigmataOptVal);

public:
	const TMap<EGsOptionOutput, int32>& GetOutputSettings() const { return _outputTable; }
	void SetOutputSetting(EGsOptionOutput In_optionType, int32 In_val, bool bImmediate = true);
	const int32 GetOutputSetting(EGsOptionOutput In_optionType)const;

public:
	// 주의: true: 알림 OFF, false: ON (기본을 true로 두기 위해 반대로 사용함)
	const TMap<EGsOptionAlert, int32>& GetAlertSettings() const { return _alertTable; }
	void SetAllAlertSetting(bool InValue);
	void SetAlertSetting(int32 InConsoleAlertValue, int32 InConsoleAlertNightValue);
	const int32 GetAlertSetting(EGsOptionAlert InType) const;

	const TMap<EGsOptionOfflineAlert, int32>& GetOfflineAlertSettings() const { return _offlineAlertTable; }
	void SetOfflineAlertSetting(EGsOptionOfflineAlert InType, int32 InVal, bool bImmediate = true);
	const int32 GetOfflineAlertSetting(EGsOptionOfflineAlert InType) const;

public:
	const TArray<FString>& GetMarketSearchList() { return _marketSearchList; }
	void SetSearchString(const FString& InStr, bool bIsAdd);

	// 카메라 모드 백업 데이터
public:
	const float GetCameraModeBackupData(EGsOptionCameraModeBackup In_type) const;

	// 스캔 백업 데이터
public:
	const int32 GetScanBackupData(EGsOptionScan In_type)const;

	// 절전 모드
public:
	const TMap<EGsOptionSaveBattery, int32>& GetSaveBatterySettings() const { return _saveBatteryTable; }
	void SetSaveBatterySetting(EGsOptionSaveBattery InType, int32 InVal, bool bImmediate = true);
	const int32 GetSaveBatterySetting(EGsOptionSaveBattery InType) const;


	// 오토
public:
	const TMap<EGsOptionAuto, float>& GetAutoSettings() const { return _autoTable; }
	void SetAutoSetting(EGsOptionAuto In_optionType, float In_val, bool bImmediate = true);
	const float GetAutoSetting(EGsOptionAuto In_optionType)const;

	// 입력
public:
	const TMap<EGsOptionInput, int32>& GetInputSettings() const { return _inputTable; }
	void SetInputSetting(EGsOptionInput In_optionType, int32 In_val, bool bImmediate = true);
	const int32 GetInputSetting(EGsOptionInput In_optionType)const;

	// HUD
public:
	const TMap<EGsOptionHud, int32>& GetHudSettings() const { return _hudTable; }
	void SetHudSetting(EGsOptionHud In_optionType, int32 In_val, bool bImmediate = true);
	const int32 GetHudSetting(EGsOptionHud In_optionType) const;

#if WITH_EDITOR || PLATFORM_WINDOWS
private:
	void OnPopupYesNoResolution(int In_Index, int In_SelectIndex);
	void OnPopupYesNoResolution(EGsOptionWindowResolution In_Type, FIntPoint In_IntPoint, FIntPoint In_SelectIntPoint);
#endif

#if WITH_EDITOR || PLATFORM_WINDOWS
protected:
	void OnResizeViewport(class FViewport* InViewport, uint32 InNum);
public:
	void TestWindowScreenMode();
	void TestGameWindowMoved();
	void CreateWindowMovedEvent();
#endif

public:
	const FGsSchemaOptionConfig* GetOptionConfig();
private:
	bool GetMainResolutionIndex(IN FIntPoint inResolutionSize, OUT int& outIndex);

public:
	struct FBannerInfo
	{
		CommunityEventId id;
		FDateTime tmStart;
		FDateTime tmEnd;

		bool valid{ false };
		bool reddot{ true };
	};

	int GetAllCommunityInfo(TArray<FBannerInfo>& dataList);
	void SetAllCommunityInfo(TArray<FBannerInfo> dataList);

#if PLATFORM_ANDROID || PLATFORM_IOS
	void SetMobileFSR(int32 inFlag);
	bool GetMobileFSR();
#endif

	bool GetAllGameAlert() { return _allGameAlert; }
	void SetAllGameAlert(bool InValue);

public:
	bool GetFairyLockOption(bool isLeft);
	bool GetCostumeLockOption(bool isLeft);

	void SetFairyLockOption(bool left, bool right);
	void SetCostumeLockOption(bool left, bool right);

private:
	void ApplyOtherPCRendering();
	void OtherPCRenderingUpdate(float InDelta);
	void ResetOtherPCRendering();

	float OPCRLastAccessTime;
};
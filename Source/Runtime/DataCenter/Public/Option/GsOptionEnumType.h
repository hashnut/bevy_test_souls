// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsOptionEnumType.generated.h"
//
//UENUM(BlueprintType)
//enum class EGsOptionItem : uint8
//{
//	None = 0 UMETA(Tooltip = "없음"),
//
//	Resolution = 1 UMETA(Tooltip = "해상도 품질 (0~3)"),
//	FrameRate = 2 UMETA(Tooltip = "프레임제한 (20, 30, 45, 60)"),
//	AutoTuning = 3 UMETA(Tooltip = "성능 자동조절 여부 ON/OFF"),
//	ViewDistance = 4 UMETA(Tooltip = "시야거리 품질 (0~3)"),
//	PostProcess = 5 UMETA(Tooltip = "포스트프로세스 품질 (0~3)"),
//	Shadow = 6 UMETA(Tooltip = "그림자 품질 (0~3)"),
//	Effect = 7 UMETA(Tooltip = "비주얼효과 품질 (0~3)"),
//	Texture = 8 UMETA(Tooltip = "텍스쳐 품질 (0~3)"),
//	Foliage = 9 UMETA(Tooltip = "수풀밀도 품질 (0~3)"),
//	SSAO = 10 UMETA(Tooltip = "특수효과 - SSAO (0~2)"),
//	Outline = 11 UMETA(Tooltip = "특수효과 - 캐릭터 아웃라인 ON/OFF"),
//
//	MasterOn = 12 UMETA(Tooltip = "전체 ON/OFF"),
//	MasterVolume = 13 UMETA(Tooltip = "전체 볼륨 (0~100)"),
//	BgmOn = 14 UMETA(Tooltip = "bgm ON/OFF"),
//	BgmVolume = 15 UMETA(Tooltip = "bgm 볼륨 (0~100)"),
//	EffectOn = 16 UMETA(Tooltip = "효과음 ON/OFF"),
//	EffectVolume = 17 UMETA(Tooltip = "효과음 볼륨 (0~100)"),
//
//	HpPotionAotoRate = 18 UMETA(Tooltip = "HP 포션 자동사용 HP/MAX HP 비율"),
//	MpPotionAotoRate = 19 UMETA(Tooltip = "MP 포션 자동사용 MP/MAX MP 비율"),
//	PotionPriority = 20 UMETA(Tooltip = "0 : 낮은 등급부터, 1 : 높은 등급부터"),
//	AutoCombatGrade = 21 UMETA(Tooltip = "자동사냥시 전투복귀 거리 타입 (0~3)"),
//	MannerMode = 22 UMETA(Tooltip = "ON/OFF"),
//	ItemLootGrade = 23 UMETA(Tooltip = "0 : 모든, 1 : 고급이상, 2 : 희귀 이상"),
//	HpRateAlertLevel = 24 UMETA(Tooltip = "HP 낮을 때 경고 레벨, 0 : 안함, 1 : 10%, 2 : 25%, 3 : 50%"),
//	PvpAttackedAlert = 25 UMETA(Tooltip = "PVP 공격받을 때 경고 여부 ON/OFF"),
//	AutoLogin = 26 UMETA(Tooltip = "자동로그인"),
//	DisableDistortion = 27 UMETA(Tooltip = "굴절 ON/OFF"),
//	TerrainNormalDistance = 28 UMETA(Tooltip = "Terrain Normal Distance"),
//	AutoBattleRange = 29 UMETA(Tooltip = "자동 전투 범위"),
//	AutoHPPortionUseRate = 30 UMETA(Tooltip = "체력 물약 자동 사용"),
//	AutoDropItemRootMinimalGrade = 31 UMETA(Tooltip = "장비 자동 줍기 최소 등급"),
//	EmptySpace = 32 UMETA(Tooltip = "공백"),
//	DevideLine = 33 UMETA(Tooltip = "라인"),
//	AutoSearchMainDesc = 34 UMETA(Tooltip = "대상 탐색 탭 자동 사냥 설명"),
//	AutoSearchSubDesc = 35 UMETA(Tooltip = "대상 탐색 탭 서브설명"),
//	AutoSearch = 36 UMETA(Tooltip = "대상 탐색"),
//	GuildNotice = 37 UMETA(Tooltip = "길드 티커 알림 ON/OFF"),
//	SummonSkip = 38 UMETA(Tooltip = "소환 스킵 ON/OFF"),
//	BuddyNotice = 39 UMETA(Tooltip = "친구 티커 알림 ON/OFF"),
//	AllNotice = 40 UMETA(Tooltip = "모든 알림 ON/OFF"),
//	CostumeHairOn = 41 UMETA(Tooltip = "코스튬 헤어 보이기 ON/OFF"),
//	Logout = 42 UMETA(Tooltip = "계정로그아웃"),
//	InventoryLongPressEnable = 43 UMETA(Tooltip = "인벤토리에서 롱프레스 사용 유무"),
//	Battle_MannerMode = 44 UMETA(Tooltip = "매너 모드(ON/파티, 기사단 예외/OFF)"),
//	Battle_NonAutoMoveKeepTarget = 45 UMETA(Tooltip = "수동 이동 시, 공격 대상 유지"),
//	Battle_NonAutoMoveKeepAuto = 46 UMETA(Tooltip = "수동 이동 시, 자동 전투 유지"),
//	Battle_DamageFloater = 47 UMETA(Tooltip = "데미지 플로터 출력"),
//	Battle_DropItemName = 48 UMETA(Tooltip = "드랍 아이템 이름 출력"),
//	Battle_NPCName = 49 UMETA(Tooltip = "NPC 이름 출력"),
//	Battle_NPCFunctionIcon = 50 UMETA(Tooltip = "NPC 기능 아이콘 출력"),
//	Battle_PlayerName = 51 UMETA(Tooltip = "PC 이름 출력"),
//	Battle_ScanEffect = 52 UMETA(Tooltip = "수동 스캔시 스캔(음파) 이펙트 표현"),
//	AutoItemGain = 53 UMETA(Tooltip = "아이템 자동 획득 설정"),
//	AutoItemGainWeight = 54 UMETA(Tooltip = "아이템 자동 획득 무게 설정"),
//	AnotherUserItemGain = 55 UMETA(Tooltip = "다른 플레이어가 드랍한 아이템 획득 설정"),
//	AutoPotionUse = 56 UMETA(Tooltip = "자동 물약 사용 설정"),
//	AutoSkillUse = 57 UMETA(Tooltip = "자동 스킬 사용 설정"),
//	BuffAlwaysUse = 58 UMETA(Tooltip = "MP 관계 없이, 지속형 스킬 항상 사용"),
//	SaveBattery_Start = 59 UMETA(Tooltip = "절전모드 진입 시간"),
//	SaveBattery_AutoQuit = 60 UMETA(Tooltip = "다른 플레이어에게 피격 시, 절전 모드 자동 해제"),
//	TouchMove = 61 UMETA(Tooltip = "지면 터치 시 캐릭터 이동 기능"),
//	ItemTooltip = 62 UMETA(Tooltip = "아이템 상세창 확인 방법 변경"),
//	AutoPartyConfirm = 63 UMETA(Tooltip = "자동 파티 수락"),
//	CameraSetting = 64 UMETA(Tooltip = "카메라 모드 설정"),
//	Camera_Shake = 65 UMETA(Tooltip = "카메라 흔들림 연출"),
//	Costume_Hair = 66 UMETA(Tooltip = "코스튬 머리 외형 보이기"),
//	Graphic_Total = 67 UMETA(Tooltip = "그래픽 일괄 설정"),
//	Graphic_Total_Desc = 68 UMETA(Tooltip = "그래픽 설정을 선택한 사양으로 모두 변경 합니다."),
//	TargetSearchArea = 69 UMETA(Tooltip = "대상 탐색 범위"),
//	NPC_BattleMon = 70 UMETA(Tooltip = "1순위 나를 공격 중인 몬스터"),
//	NPC_QuestMon = 71 UMETA(Tooltip = "2순위 퀘스트 목표 몬스터"),
//	NPC_AgrresiveMon = 72 UMETA(Tooltip = "3순위 선공 몬스터"),
//
//	AlertVolumeOn = 73 UMETA(Tooltip = "알림 볼륨 전체 ON/OFF"),
//	AlertVolume = 74 UMETA(Tooltip = "알림 볼륨"),
//	DeathAlert = 75 UMETA(Tooltip = "사망 알림"),
//	FriendAlert = 76 UMETA(Tooltip = "친구 접속 알림"),
//	GuildAlert = 77 UMETA(Tooltip = "기사단 접속 알림"),
//	PuchAlert = 78 UMETA(Tooltip = "Push 알림 설정"),
//	PuchAlert_Night = 79 UMETA(Tooltip = "야간 Pusu 알림 설정"),
//	Quest_Skip = 80 UMETA(Tooltip = "퀘스트 대화 자동 넘기기"),
//	Battle_NPCTalkBalloon = 81 UMETA(Tooltip = "NPC 말풍선 출력"),
//	AllAlert = 82 UMETA(Tooltip = "모든 알림"),
//	ConsoleAlert = 83 UMETA(Tooltip = "공지 알림"),
//	ConsoleAlert_Night = 84 UMETA(Tooltip = "야간 알림"),
//	HPRateAlert = 85 UMETA(Tooltip = "hp 부족 알림"),
//	HPRateRingerAlert = 86 UMETA(Tooltip = "hp 부족 벨 알림"),
//	HPRateVibrateAlert = 87 UMETA(Tooltip = "hp 부족 진동 알림"),
//	WeightAlert = 88 UMETA(Tooltip = "무게 알림"),
//
//	Graphic_Total_Mobile = 89 UMETA(Tooltip = "그래픽 일괄 설정(모바일)"),
//	Effect_Mobile = 90 UMETA(Tooltip = "비주얼효과 품질 (0~2)(모바일)"),
//	Foliage_Mobile = 91 UMETA(Tooltip = "수풀밀도 품질 (0~2)(모바일)"),
//	PostProcess_Mobile = 92 UMETA(Tooltip = "포스트프로세스 품질 (0~2)(모바일)"),
//	Shadow_Mobile = 93 UMETA(Tooltip = "그림자 품질 (0~2)(모바일)"),
//	Texture_Mobile = 94 UMETA(Tooltip = "텍스쳐 품질 (0~2)(모바일)"),
//	ViewDistance_Mobile = 95 UMETA(Tooltip = "시야거리 품질 (0~2)(모바일)"),
//	Resolution_Mobile = 96 UMETA(Tooltip = "해상도 품질 (0~2)(모바일)"),
//	FrameRate_Mobile = 97 UMETA(Tooltip = "프레임제한 (20, 30, 45)(모바일)"),		
//	ConsoleAlert_Night_Desc = 98 UMETA(Tooltip = "야간 알림 설명"),
//	AutoPotionAlert = 99 UMETA(Tooltip = "자동 물약 잔여 개수 알림"),
//	PC_BattleCharacter_Manual = 100 UMETA(Tooltip = "1순위 나를 공격 중인 캐릭터"),
//	PC_Enermy_manual = 101 UMETA(Tooltip = "2순위 나와 적대 관계인 캐릭터"),
//	NPC_BattleMon_Manual = 102 UMETA(Tooltip = "3순위 나를 공격 중인 몬스터"),
//	NPC_QuestMon_Manual = 103 UMETA(Tooltip = "4순위 퀘스트 목표 몬스터"),
//	NPC_AgrresiveMon_Manual = 104 UMETA(Tooltip = "5순위 선공 몬스터"),
//	ManualSearchMainDesc = 105 UMETA(Tooltip = "대상 탐색 수동 사냥 설명"),
//	PvpAttackedRingerAlert = 106 UMETA(Tooltip = "PVP 공격받을 때 경고 벨알림"),
//	PvpAttackedVibrateAlert = 107 UMETA(Tooltip = "PVP 공격받을 때 경고 진동알림"),
//	UIScale = 108 UMETA(Tooltip = "UIScale"),
//	Language = 109 UMETA(Tooltip = "언어 설정"),
//	LanguageVoice = 110 UMETA(Tooltip = "음성 언어 설정"),
//	ManualBattle_AutoCounterAttack = 111 UMETA(Tooltip = "자동 반격"),
//	ManualBattle_AutoSkillUse = 112 UMETA(Tooltip = "자동 스킬사용"),
//	ParticleQuality = 113 UMETA(Tooltip = "파티클 시그니피컨트 컨트롤"),
//	HPPotionRingerAlert = 114 UMETA(Tooltip = "HP 자동물약 부족 소리알림"),
//	HPPotionVibrateAlert = 115 UMETA(Tooltip = "HP 자동물약 부족 진동알림"),
//	EquanimityAlert = 116 UMETA(Tooltip = "게임 편의 알림"),
//	EquanimityAlertIntervalTime = 117 UMETA(Tooltip = "게임 편의 알림 시간 조절"),
//	ConnectAlert = 118 UMETA(Tooltip = "접속 알림"),
//	ConnectAlertIntervalTime = 119 UMETA(Tooltip = "접속 알림 시간 조절"),
//	ItemGainGlobalMessgeNameHide = 120 UMETA(Tooltip = "획득 알림 글로벌 메시지에 이름 숨김"),
//	OfflinePlayAlert = 121 UMETA(Tooltip = "오프라인 접속 알림 시간 조절"),
//	OfflinePlayAlertIntervalTime = 122 UMETA(Tooltip = "오프라인 알림 시간 조절"),
//	OfflineInvenAlert = 123 UMETA(Tooltip = "오프라인 인벤토리 알림"),
//	OfflineWeightAlert = 124 UMETA(Tooltip = "오프라인 무게 알림"),
//	OfflinePlayfinishedAlert = 125 UMETA(Tooltip = "오프라인 플레이 종료 알림"),
//	OfflineAttackedAlert = 126 UMETA(Tooltip = "오프라인 피격 알림"),
//	OfflineDeathAlert = 127 UMETA(Tooltip = "오프라인 사망 알림"),
//	OfflineDungeonTimeOutAlert = 128 UMETA(Tooltip = "오프라인 던전 제한 시간 종료 알림"),
//	OfflineEquipItemAcquisitionAlert = 129 UMETA(Tooltip = "장착 가능한 장비 아이템 획득 알림"),
//	OfflineSkillBookAcquisitionAlert = 130 UMETA(Tooltip = "스킬북 획득 알림"),
//	OfflineMarketSaleCompletedAlert = 131 UMETA(Tooltip = "거래소 등록 물품 판매 완료"),	
//
//	WeightRingerAlert = 132 UMETA(Tooltip = "무게 벨소리 알림"),
//	WeightVibrateAlert = 133 UMETA(Tooltip = "무게 진동 알림"),
//	DeathRingerAlert = 134 UMETA(Tooltip = "사망 벨소리 알림"),
//	DeathVibrateAlert = 135 UMETA(Tooltip = "사망 진동 알림"),
//
//	Window_Key_Mapping = 136 UMETA(Tooltip = "윈도우 카메라 맵핑"),
//	Window_Resolution = 137 UMETA(Tooltip = "윈도우 해상도"),
//	Window_ScreenMode = 138 UMETA(Tooltip = "윈도우 스크린 모드"),
//	Window_AntiAliasing = 139 UMETA(Tooltip = "윈도우 안티얼라이어싱"),
//	Window_SetVSyncEnabled = 140 UMETA(Tooltip = "윈도우 수직동기화"),
//
//	Focus_Volume_Multipler = 141 UMETA(Tooltip = "게임이 활성화되었을 때만 소리 재생"),
//	Npc_Function_UI_Name = 142 UMETA(Tooltip = "npc function ui name(0: OFF, 1: ON)"),
//
//	Target_Detection_Alert = 143 UMETA(Tooltip = "경계 대상 근접 탐지 거리 알림"),
//	Target_Detection_Ringer_Alert = 144 UMETA(Tooltip = "경계 대상 근접 탐지 거리"),
//	Target_Detection_Vibrate_Alert = 145 UMETA(Tooltip = "경계 대상 근접 진동 알림"),
//
//	Target_Detection_Alert_Distance = 146 UMETA(Tooltip = "경계 대상 근접 탐지 거리"),
//	Target_Detection_Alert_Interval = 147 UMETA(Tooltip = "경계 대상 근접 알림 주기"),
//
//	KeepTarget_GroundTouch = 148 UMETA(Tooltip = "지면 터치 시 타겟 유지 [on/off]"),
//	AutoSkillUse_AddTargetButton = 149 UMETA(Tooltip = "스킬 대상 선택 UI 출력 시, 자동 사용 [on/off]"),
//
//	AntiAliasingQuality = 150 UMETA(Tooltip = "안티얼라이어싱 퀄리티"),
//	ShadingQuality = 151 UMETA(Tooltip = "Shading 퀄리티"),
//	ParticleQuality_Mobile = 152 UMETA(Tooltip = "파티클 시그니피컨트 컨트롤"),
//
//	CombatStateAlert = 153 UMETA(Tooltip = "전투 편의 알림"),
//	CombatStateAlertIntervalTime = 154 UMETA(Tooltip = "전투 편의 알림 시간 조절"),
//
//	OptionItemGainNoneEqupConsume = 155 UMETA(Tooltip = "Item 자동 획득 설정"),
//	OptionItemGainNoneEqupConsume_Normal = 156 UMETA(Tooltip = "Item 자동 획득 설정 - 일반"),
//	OptionItemGainNoneEqupConsume_Special = 157 UMETA(Tooltip = "Item 자동 획득 설정 - 특수"),
//	
//	OptionItemGainScrollConsume = 158 UMETA(Tooltip = "주문서 자동 획득 설정"),
//	OptionItemGainScrollConsume_Normal = 159 UMETA(Tooltip = "주문서 자동 획득 설정 - 일반"),
//	OptionItemGainScrollConsume_Special = 160 UMETA(Tooltip = "주문서 자동 획득 설정 - 특수"),
//	OptionItemGainScrollConsume_Enchant = 161 UMETA(Tooltip = "주문서 자동 획득 설정 - 강화"),
//
//	PersonalTradingAutoReject = 162 UMETA(Tooltip = "개인 거래 자동 거절"),
//	TestCheckBox = 163 UMETA(Tooltip = "체크박스테스트"),
//	TestCheckBox0 = 164 UMETA(Tooltip = "체크박스0"),
//	TestCheckBox1 = 165 UMETA(Tooltip = "체크박스1"),
//
//	Max = 166,
//};

UENUM(BlueprintType)
enum class EGsSliderTextType : uint8
{
	NONE = 0,
	PERCENT = 1,
	COUNT = 2,
	COUNT_LIMIT = 3,
	COUNT_RANGE = 4,
	COUNT_RANGE_LITMIT = 5,
	TIME = 6,
	DISTANCE_M,
};

UENUM(BlueprintType)
enum class EGsOptionPlatformType : uint8
{
	OPTION_ALL = 0,
	OPTION_MOBILE_AOS = 1,
	OPTION_MOBILE_IOS = 2,
	OPTION_WINDOW = 3,
};

UENUM(BlueprintType)
enum class EGsOptionKeyBindType : uint8
{
	None,
	Action,
	Axis,
	Touch, 
};

enum class EGsOptionKeyActionFlags : uint8
{
	NONE = 0,

	SHIFT = 1 << 0,
	CTRL = 1 << 1,
	ALT = 1 << 2,
	CMD = 1 << 3,
};
ENUM_CLASS_FLAGS(EGsOptionKeyActionFlags);

UENUM(BlueprintType)
enum class EGsOptionKeyResType : uint8
{
	None,
	Res_Circle,
	Res_Rect,
};

UENUM(BlueprintType)
enum class EGsIdpConnectType : uint8
{
	IDP_DISCONNECT,
	IDP_CONNECT,	
};
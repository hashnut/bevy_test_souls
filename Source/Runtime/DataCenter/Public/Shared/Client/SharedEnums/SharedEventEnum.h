#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedEventEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 퀘스트 진행 시 수락 전, 완료 후에 이루어져야 하는 Event Action
enum class EventActionType : uint8
{

	// 컷씬 재생 - 클라이언트에서 시작과 끝을 서버에게 알려줌 : 클라사용(SequenceResData.Id)
	PLAY_CINEMATIC = 0,

	// 몬스터 Spawn - 클라에게 요청받아서, 서버에서 처리 후 클라에게 결과를 알려줌
	SPAWN = 1,

	// 몬스터 DeSpawn - 클라에서 요청받아서, 서버에서 처리 후 클라에게 결과를 알려줌
	DESPAWN = 2,

	// 화면 딤드 ON - 화면 가리기
	DIMMED_ON = 3,

	// 화면 딤드 OFF - 화면 가리기 해제
	DIMMED_OFF = 4,

	// 위치이동 - 동일한 맵에서 위치(Spot)만 이동
	TELEPORT = 5,

	// (클라전용)튜토리얼 - 시작되면, 튜토리얼을 발동시키고, 튜토리얼 완료 시 종료되는 Type
	TUTORIAL = 6,

	// 퍼스널 맵(또는 싱글 필드)에서 몬스터를 직접 처치하는 것이 아닌 간접적으로 처치할 수 있는 연출을 하기 위함.
	MONSTER_KILL = 7,

	// (클라전용)대화 - 클라이언트에서 시작과 끝을 서버에게 알려줌 : 클라사용 (NpcDalogData.Id)
	DIALOG = 8,

	// (클라전용)동영상 플레이 - 클라이언트에서 시작과 끝을 서버에게 알려줌 : 클라사용 (MovieData.Id)
	PLAY_MOVIE = 9,

	// (클라전용)사운드 플레이 - 클라이언트에서 시작과 끝을 서버에게 알려줌 : 클라사용 (SoundResData.Id)
	PLAY_SOUND = 10,

	// (클라전용)사운드 정지 - 클라이언트에서 시작과 끝을 서버에게 알려줌 : 클라사용 (SoundResData.Id)
	STOP_SOUND = 11,

	// 외형 변경 - LookInfo 변경
	TRANSFORM_LOOK_INFO = 12,

	// 외형 변경 - 스텟을 변경
	TRANSFORM_STAT = 13,

	// 외형 변경 -  변형되어 있는 TRANSFORM_LOOK_INFO 와 TRANSFORM_STAT 내용을 깨끗하게 청소한다
	TRANSFORM_CLEAR = 14,

	// 랜덤 워프 - 랜덤 워프(랜덤 순간이동 주문서 사용) 못하는 구간 설정
	RANDOM_WARP_DISABLE = 15,

	// 랜덤 워프 - 랜덤 워프(랜덤 순간이동 주문서 사용) 못하는 구간 해제
	RANDOM_WARP_ABLE = 16,

	MAX = 17,
};

UENUM(BlueprintType)
// 퀘스트 진행 시 수락 전, 완료 후에 이루어져야 하는 Event Action의 상태
enum class EventActionState : uint8
{

	// EventAction 없음
	NONE = 0,

	// Accept 전에 EventAction이 진행 되어야하는 상태
	PRE = 1,

	// COMPLETE 후에 EventAction이 진행 되어야하는 상태
	POST = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 이벤트관리 시스템 컨텐츠 유형
enum class ContentsEventType : uint8
{

	// 서버에서만 사용하는 데이터로드용 정보 
	UNUSED_COMMON = 0,

	// 드랍 이벤트, ContentsEventReward 테이블 참조
	REWARD = 1,

	// 맵 패시비트 이벤트, ContentsEventMapPasivity 테이블 참조
	MAP_PASSIVITY = 2,

	// 텔레포트 비용 할인 이벤트, ContentsEventTeleportCost 테이블 참조
	TELEPORT_COST = 3,

	// 이벤트 던전 오픈 및 던전 플레이타임 이벤트, ContentsEventDungeon 테이블 참조
	DUNGEON = 4,

	// 이벤트 상인, ContentsEventNpcShop 테이블 참조
	NPC_SHOP = 5,

	// 기도사, ContentsEventNpcPrayer 테이블 참조
	NPC_PRAYER = 6,

	// 장비 아이템 복구, 페어리/코스튬 합성 재시도 이벤트, ContentsDeliveryBox 테이블 참조
	DELIVERY_BOX = 7,

	MAX = 8,
};

UENUM(BlueprintType)
// 이벤트 텔레포트 비용할인
enum class ContentsEventTeleportCost : uint8
{

	// 이벤트 텔레포트 비용할인 VALUE 만큼 차감
	VALUE = 0,

	// 이벤트 텔레포트 비용할인 비율로 차감 
	RATE = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 이벤트 참여 조건 종류
enum class ContentsEventCondition : uint8
{

	// 없다
	NONE = 0,

	// 레벨 조건
	LEVEL = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 복구 이벤트 종류
enum class ContentsEventDeliveryBoxType : uint8
{

	// 없다
	NONE = 0,

	// 장비 아이템 복구
	EQUIP = 1,

	// 페어리 합성 재시도
	FAIRY = 2,

	// 코스튬 합성 재시도
	COSTUME = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 이벤트 대화 종류
enum class EventDialogSyle : uint8
{
	TICKER = 0,
	STATEBOARD = 1,
	PROLOGUE_TICKER = 2,
	BATTLE_TICKER = 3,

	// 침공전 알림 티커 1타입
	INVADE_TYPE_ONE_TEXT_TICKER = 4,

	// 침공전 알림 티커 2타입
	INVADE_TYPE_TWO_TEXT_TICKER = 5,

	// 시간 타이머 종류
	TIMER_BOARD = 6,

	// 시간 타이머 종류
	TIMER_BOARD_EX = 7,

	// PKT 요청
	FIELD_BOSS_SPAWN_TICKER = 8,

	// PKT 요청
	FIELD_BOSS_DIE_TICKER = 9,

	// 침공전에서 서버이름 함께 표기알림 티커 타입
	INVADE_TYPE_SERVERNAME_TEXT_TICKER = 10,

	// 채팅을 이용한 티커
	CHAT_TICKER = 11,

	MAX = 12,
};

UENUM(BlueprintType)
// 이벤트 사용할 파라미터 정보들
enum class MissionEventParam : uint8
{
	NONE_PARAM = 0,

	//  MIN / MAX 
	PAIR_VALUE = 1,

	// 시간관련
	TIME_LIMT = 2,

	MAX = 3,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline EventActionType EventActionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("PLAY_CINEMATIC"), ESearchCase::IgnoreCase) == 0)	return EventActionType::PLAY_CINEMATIC;
	if (str.Compare(TEXT("SPAWN"), ESearchCase::IgnoreCase) == 0)	return EventActionType::SPAWN;
	if (str.Compare(TEXT("DESPAWN"), ESearchCase::IgnoreCase) == 0)	return EventActionType::DESPAWN;
	if (str.Compare(TEXT("DIMMED_ON"), ESearchCase::IgnoreCase) == 0)	return EventActionType::DIMMED_ON;
	if (str.Compare(TEXT("DIMMED_OFF"), ESearchCase::IgnoreCase) == 0)	return EventActionType::DIMMED_OFF;
	if (str.Compare(TEXT("TELEPORT"), ESearchCase::IgnoreCase) == 0)	return EventActionType::TELEPORT;
	if (str.Compare(TEXT("TUTORIAL"), ESearchCase::IgnoreCase) == 0)	return EventActionType::TUTORIAL;
	if (str.Compare(TEXT("MONSTER_KILL"), ESearchCase::IgnoreCase) == 0)	return EventActionType::MONSTER_KILL;
	if (str.Compare(TEXT("DIALOG"), ESearchCase::IgnoreCase) == 0)	return EventActionType::DIALOG;
	if (str.Compare(TEXT("PLAY_MOVIE"), ESearchCase::IgnoreCase) == 0)	return EventActionType::PLAY_MOVIE;
	if (str.Compare(TEXT("PLAY_SOUND"), ESearchCase::IgnoreCase) == 0)	return EventActionType::PLAY_SOUND;
	if (str.Compare(TEXT("STOP_SOUND"), ESearchCase::IgnoreCase) == 0)	return EventActionType::STOP_SOUND;
	if (str.Compare(TEXT("TRANSFORM_LOOK_INFO"), ESearchCase::IgnoreCase) == 0)	return EventActionType::TRANSFORM_LOOK_INFO;
	if (str.Compare(TEXT("TRANSFORM_STAT"), ESearchCase::IgnoreCase) == 0)	return EventActionType::TRANSFORM_STAT;
	if (str.Compare(TEXT("TRANSFORM_CLEAR"), ESearchCase::IgnoreCase) == 0)	return EventActionType::TRANSFORM_CLEAR;
	if (str.Compare(TEXT("RANDOM_WARP_DISABLE"), ESearchCase::IgnoreCase) == 0)	return EventActionType::RANDOM_WARP_DISABLE;
	if (str.Compare(TEXT("RANDOM_WARP_ABLE"), ESearchCase::IgnoreCase) == 0)	return EventActionType::RANDOM_WARP_ABLE;
	return EventActionType::MAX;
}
inline EventActionState EventActionStateStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return EventActionState::NONE;
	if (str.Compare(TEXT("PRE"), ESearchCase::IgnoreCase) == 0)	return EventActionState::PRE;
	if (str.Compare(TEXT("POST"), ESearchCase::IgnoreCase) == 0)	return EventActionState::POST;
	return EventActionState::MAX;
}
inline ContentsEventType ContentsEventTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("UNUSED_COMMON"), ESearchCase::IgnoreCase) == 0)	return ContentsEventType::UNUSED_COMMON;
	if (str.Compare(TEXT("REWARD"), ESearchCase::IgnoreCase) == 0)	return ContentsEventType::REWARD;
	if (str.Compare(TEXT("MAP_PASSIVITY"), ESearchCase::IgnoreCase) == 0)	return ContentsEventType::MAP_PASSIVITY;
	if (str.Compare(TEXT("TELEPORT_COST"), ESearchCase::IgnoreCase) == 0)	return ContentsEventType::TELEPORT_COST;
	if (str.Compare(TEXT("DUNGEON"), ESearchCase::IgnoreCase) == 0)	return ContentsEventType::DUNGEON;
	if (str.Compare(TEXT("NPC_SHOP"), ESearchCase::IgnoreCase) == 0)	return ContentsEventType::NPC_SHOP;
	if (str.Compare(TEXT("NPC_PRAYER"), ESearchCase::IgnoreCase) == 0)	return ContentsEventType::NPC_PRAYER;
	if (str.Compare(TEXT("DELIVERY_BOX"), ESearchCase::IgnoreCase) == 0)	return ContentsEventType::DELIVERY_BOX;
	return ContentsEventType::MAX;
}
inline ContentsEventTeleportCost ContentsEventTeleportCostStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("VALUE"), ESearchCase::IgnoreCase) == 0)	return ContentsEventTeleportCost::VALUE;
	if (str.Compare(TEXT("RATE"), ESearchCase::IgnoreCase) == 0)	return ContentsEventTeleportCost::RATE;
	return ContentsEventTeleportCost::MAX;
}
inline ContentsEventCondition ContentsEventConditionStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ContentsEventCondition::NONE;
	if (str.Compare(TEXT("LEVEL"), ESearchCase::IgnoreCase) == 0)	return ContentsEventCondition::LEVEL;
	return ContentsEventCondition::MAX;
}
inline ContentsEventDeliveryBoxType ContentsEventDeliveryBoxTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ContentsEventDeliveryBoxType::NONE;
	if (str.Compare(TEXT("EQUIP"), ESearchCase::IgnoreCase) == 0)	return ContentsEventDeliveryBoxType::EQUIP;
	if (str.Compare(TEXT("FAIRY"), ESearchCase::IgnoreCase) == 0)	return ContentsEventDeliveryBoxType::FAIRY;
	if (str.Compare(TEXT("COSTUME"), ESearchCase::IgnoreCase) == 0)	return ContentsEventDeliveryBoxType::COSTUME;
	return ContentsEventDeliveryBoxType::MAX;
}
inline EventDialogSyle EventDialogSyleStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("TICKER"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::TICKER;
	if (str.Compare(TEXT("STATEBOARD"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::STATEBOARD;
	if (str.Compare(TEXT("PROLOGUE_TICKER"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::PROLOGUE_TICKER;
	if (str.Compare(TEXT("BATTLE_TICKER"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::BATTLE_TICKER;
	if (str.Compare(TEXT("INVADE_TYPE_ONE_TEXT_TICKER"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::INVADE_TYPE_ONE_TEXT_TICKER;
	if (str.Compare(TEXT("INVADE_TYPE_TWO_TEXT_TICKER"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::INVADE_TYPE_TWO_TEXT_TICKER;
	if (str.Compare(TEXT("TIMER_BOARD"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::TIMER_BOARD;
	if (str.Compare(TEXT("TIMER_BOARD_EX"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::TIMER_BOARD_EX;
	if (str.Compare(TEXT("FIELD_BOSS_SPAWN_TICKER"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::FIELD_BOSS_SPAWN_TICKER;
	if (str.Compare(TEXT("FIELD_BOSS_DIE_TICKER"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::FIELD_BOSS_DIE_TICKER;
	if (str.Compare(TEXT("INVADE_TYPE_SERVERNAME_TEXT_TICKER"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::INVADE_TYPE_SERVERNAME_TEXT_TICKER;
	if (str.Compare(TEXT("CHAT_TICKER"), ESearchCase::IgnoreCase) == 0)	return EventDialogSyle::CHAT_TICKER;
	return EventDialogSyle::MAX;
}
inline MissionEventParam MissionEventParamStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE_PARAM"), ESearchCase::IgnoreCase) == 0)	return MissionEventParam::NONE_PARAM;
	if (str.Compare(TEXT("PAIR_VALUE"), ESearchCase::IgnoreCase) == 0)	return MissionEventParam::PAIR_VALUE;
	if (str.Compare(TEXT("TIME_LIMT"), ESearchCase::IgnoreCase) == 0)	return MissionEventParam::TIME_LIMT;
	return MissionEventParam::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString EventActionTypeEnumToString(EventActionType value)
{
	switch (value)
	{
		case EventActionType::PLAY_CINEMATIC: return TEXT("PLAY_CINEMATIC");
		case EventActionType::SPAWN: return TEXT("SPAWN");
		case EventActionType::DESPAWN: return TEXT("DESPAWN");
		case EventActionType::DIMMED_ON: return TEXT("DIMMED_ON");
		case EventActionType::DIMMED_OFF: return TEXT("DIMMED_OFF");
		case EventActionType::TELEPORT: return TEXT("TELEPORT");
		case EventActionType::TUTORIAL: return TEXT("TUTORIAL");
		case EventActionType::MONSTER_KILL: return TEXT("MONSTER_KILL");
		case EventActionType::DIALOG: return TEXT("DIALOG");
		case EventActionType::PLAY_MOVIE: return TEXT("PLAY_MOVIE");
		case EventActionType::PLAY_SOUND: return TEXT("PLAY_SOUND");
		case EventActionType::STOP_SOUND: return TEXT("STOP_SOUND");
		case EventActionType::TRANSFORM_LOOK_INFO: return TEXT("TRANSFORM_LOOK_INFO");
		case EventActionType::TRANSFORM_STAT: return TEXT("TRANSFORM_STAT");
		case EventActionType::TRANSFORM_CLEAR: return TEXT("TRANSFORM_CLEAR");
		case EventActionType::RANDOM_WARP_DISABLE: return TEXT("RANDOM_WARP_DISABLE");
		case EventActionType::RANDOM_WARP_ABLE: return TEXT("RANDOM_WARP_ABLE");
	}
	return FString();
}
inline FString EventActionStateEnumToString(EventActionState value)
{
	switch (value)
	{
		case EventActionState::NONE: return TEXT("NONE");
		case EventActionState::PRE: return TEXT("PRE");
		case EventActionState::POST: return TEXT("POST");
	}
	return FString();
}
inline FString ContentsEventTypeEnumToString(ContentsEventType value)
{
	switch (value)
	{
		case ContentsEventType::UNUSED_COMMON: return TEXT("UNUSED_COMMON");
		case ContentsEventType::REWARD: return TEXT("REWARD");
		case ContentsEventType::MAP_PASSIVITY: return TEXT("MAP_PASSIVITY");
		case ContentsEventType::TELEPORT_COST: return TEXT("TELEPORT_COST");
		case ContentsEventType::DUNGEON: return TEXT("DUNGEON");
		case ContentsEventType::NPC_SHOP: return TEXT("NPC_SHOP");
		case ContentsEventType::NPC_PRAYER: return TEXT("NPC_PRAYER");
		case ContentsEventType::DELIVERY_BOX: return TEXT("DELIVERY_BOX");
	}
	return FString();
}
inline FString ContentsEventTeleportCostEnumToString(ContentsEventTeleportCost value)
{
	switch (value)
	{
		case ContentsEventTeleportCost::VALUE: return TEXT("VALUE");
		case ContentsEventTeleportCost::RATE: return TEXT("RATE");
	}
	return FString();
}
inline FString ContentsEventConditionEnumToString(ContentsEventCondition value)
{
	switch (value)
	{
		case ContentsEventCondition::NONE: return TEXT("NONE");
		case ContentsEventCondition::LEVEL: return TEXT("LEVEL");
	}
	return FString();
}
inline FString ContentsEventDeliveryBoxTypeEnumToString(ContentsEventDeliveryBoxType value)
{
	switch (value)
	{
		case ContentsEventDeliveryBoxType::NONE: return TEXT("NONE");
		case ContentsEventDeliveryBoxType::EQUIP: return TEXT("EQUIP");
		case ContentsEventDeliveryBoxType::FAIRY: return TEXT("FAIRY");
		case ContentsEventDeliveryBoxType::COSTUME: return TEXT("COSTUME");
	}
	return FString();
}
inline FString EventDialogSyleEnumToString(EventDialogSyle value)
{
	switch (value)
	{
		case EventDialogSyle::TICKER: return TEXT("TICKER");
		case EventDialogSyle::STATEBOARD: return TEXT("STATEBOARD");
		case EventDialogSyle::PROLOGUE_TICKER: return TEXT("PROLOGUE_TICKER");
		case EventDialogSyle::BATTLE_TICKER: return TEXT("BATTLE_TICKER");
		case EventDialogSyle::INVADE_TYPE_ONE_TEXT_TICKER: return TEXT("INVADE_TYPE_ONE_TEXT_TICKER");
		case EventDialogSyle::INVADE_TYPE_TWO_TEXT_TICKER: return TEXT("INVADE_TYPE_TWO_TEXT_TICKER");
		case EventDialogSyle::TIMER_BOARD: return TEXT("TIMER_BOARD");
		case EventDialogSyle::TIMER_BOARD_EX: return TEXT("TIMER_BOARD_EX");
		case EventDialogSyle::FIELD_BOSS_SPAWN_TICKER: return TEXT("FIELD_BOSS_SPAWN_TICKER");
		case EventDialogSyle::FIELD_BOSS_DIE_TICKER: return TEXT("FIELD_BOSS_DIE_TICKER");
		case EventDialogSyle::INVADE_TYPE_SERVERNAME_TEXT_TICKER: return TEXT("INVADE_TYPE_SERVERNAME_TEXT_TICKER");
		case EventDialogSyle::CHAT_TICKER: return TEXT("CHAT_TICKER");
	}
	return FString();
}
inline FString MissionEventParamEnumToString(MissionEventParam value)
{
	switch (value)
	{
		case MissionEventParam::NONE_PARAM: return TEXT("NONE_PARAM");
		case MissionEventParam::PAIR_VALUE: return TEXT("PAIR_VALUE");
		case MissionEventParam::TIME_LIMT: return TEXT("TIME_LIMT");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString EventActionTypeEnumToDisplayKey(EventActionType value)
{
	switch (value)
	{
		case EventActionType::PLAY_CINEMATIC: return TEXT("");
		case EventActionType::SPAWN: return TEXT("");
		case EventActionType::DESPAWN: return TEXT("");
		case EventActionType::DIMMED_ON: return TEXT("");
		case EventActionType::DIMMED_OFF: return TEXT("");
		case EventActionType::TELEPORT: return TEXT("");
		case EventActionType::TUTORIAL: return TEXT("");
		case EventActionType::MONSTER_KILL: return TEXT("");
		case EventActionType::DIALOG: return TEXT("");
		case EventActionType::PLAY_MOVIE: return TEXT("");
		case EventActionType::PLAY_SOUND: return TEXT("");
		case EventActionType::STOP_SOUND: return TEXT("");
		case EventActionType::TRANSFORM_LOOK_INFO: return TEXT("");
		case EventActionType::TRANSFORM_STAT: return TEXT("");
		case EventActionType::TRANSFORM_CLEAR: return TEXT("");
		case EventActionType::RANDOM_WARP_DISABLE: return TEXT("");
		case EventActionType::RANDOM_WARP_ABLE: return TEXT("");
	}
	return FString();
}
inline FString EventActionStateEnumToDisplayKey(EventActionState value)
{
	switch (value)
	{
		case EventActionState::NONE: return TEXT("");
		case EventActionState::PRE: return TEXT("");
		case EventActionState::POST: return TEXT("");
	}
	return FString();
}
inline FString ContentsEventTypeEnumToDisplayKey(ContentsEventType value)
{
	switch (value)
	{
		case ContentsEventType::UNUSED_COMMON: return TEXT("");
		case ContentsEventType::REWARD: return TEXT("");
		case ContentsEventType::MAP_PASSIVITY: return TEXT("");
		case ContentsEventType::TELEPORT_COST: return TEXT("");
		case ContentsEventType::DUNGEON: return TEXT("");
		case ContentsEventType::NPC_SHOP: return TEXT("");
		case ContentsEventType::NPC_PRAYER: return TEXT("");
		case ContentsEventType::DELIVERY_BOX: return TEXT("");
	}
	return FString();
}
inline FString ContentsEventTeleportCostEnumToDisplayKey(ContentsEventTeleportCost value)
{
	switch (value)
	{
		case ContentsEventTeleportCost::VALUE: return TEXT("");
		case ContentsEventTeleportCost::RATE: return TEXT("");
	}
	return FString();
}
inline FString ContentsEventConditionEnumToDisplayKey(ContentsEventCondition value)
{
	switch (value)
	{
		case ContentsEventCondition::NONE: return TEXT("");
		case ContentsEventCondition::LEVEL: return TEXT("");
	}
	return FString();
}
inline FString ContentsEventDeliveryBoxTypeEnumToDisplayKey(ContentsEventDeliveryBoxType value)
{
	switch (value)
	{
		case ContentsEventDeliveryBoxType::NONE: return TEXT("");
		case ContentsEventDeliveryBoxType::EQUIP: return TEXT("");
		case ContentsEventDeliveryBoxType::FAIRY: return TEXT("");
		case ContentsEventDeliveryBoxType::COSTUME: return TEXT("");
	}
	return FString();
}
inline FString EventDialogSyleEnumToDisplayKey(EventDialogSyle value)
{
	switch (value)
	{
		case EventDialogSyle::TICKER: return TEXT("");
		case EventDialogSyle::STATEBOARD: return TEXT("");
		case EventDialogSyle::PROLOGUE_TICKER: return TEXT("");
		case EventDialogSyle::BATTLE_TICKER: return TEXT("");
		case EventDialogSyle::INVADE_TYPE_ONE_TEXT_TICKER: return TEXT("");
		case EventDialogSyle::INVADE_TYPE_TWO_TEXT_TICKER: return TEXT("");
		case EventDialogSyle::TIMER_BOARD: return TEXT("");
		case EventDialogSyle::TIMER_BOARD_EX: return TEXT("");
		case EventDialogSyle::FIELD_BOSS_SPAWN_TICKER: return TEXT("");
		case EventDialogSyle::FIELD_BOSS_DIE_TICKER: return TEXT("");
		case EventDialogSyle::INVADE_TYPE_SERVERNAME_TEXT_TICKER: return TEXT("");
		case EventDialogSyle::CHAT_TICKER: return TEXT("");
	}
	return FString();
}
inline FString MissionEventParamEnumToDisplayKey(MissionEventParam value)
{
	switch (value)
	{
		case MissionEventParam::NONE_PARAM: return TEXT("");
		case MissionEventParam::PAIR_VALUE: return TEXT("");
		case MissionEventParam::TIME_LIMT: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidEventActionType(EventActionType value) noexcept
{
	return (value >= EventActionType::PLAY_CINEMATIC && value < EventActionType::MAX);
}

inline bool IsValidEventActionState(EventActionState value) noexcept
{
	return (value >= EventActionState::NONE && value < EventActionState::MAX);
}

inline bool IsValidContentsEventType(ContentsEventType value) noexcept
{
	return (value >= ContentsEventType::UNUSED_COMMON && value < ContentsEventType::MAX);
}

inline bool IsValidContentsEventTeleportCost(ContentsEventTeleportCost value) noexcept
{
	return (value >= ContentsEventTeleportCost::VALUE && value < ContentsEventTeleportCost::MAX);
}

inline bool IsValidContentsEventCondition(ContentsEventCondition value) noexcept
{
	return (value >= ContentsEventCondition::NONE && value < ContentsEventCondition::MAX);
}

inline bool IsValidContentsEventDeliveryBoxType(ContentsEventDeliveryBoxType value) noexcept
{
	return (value >= ContentsEventDeliveryBoxType::NONE && value < ContentsEventDeliveryBoxType::MAX);
}

inline bool IsValidEventDialogSyle(EventDialogSyle value) noexcept
{
	return (value >= EventDialogSyle::TICKER && value < EventDialogSyle::MAX);
}

inline bool IsValidMissionEventParam(MissionEventParam value) noexcept
{
	return (value >= MissionEventParam::NONE_PARAM && value < MissionEventParam::MAX);
}


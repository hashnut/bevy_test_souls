#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedPropEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// Prop 구분 타입
enum class PropType : uint8
{

	// 유효하지 않은 타입
	NONE = 0,

	// 퀘스트 에서 사용
	QUEST = 1,

	// 침공 포탈
	INVADE_PORTAL = 2,

	// 고레벨필드던전 층이동 포탈
	ELITE_DUNGEON_PORTAL_UP = 3,

	// 발동효과를 사용함
	COMMON_EFFECT = 4,

	// 인터렉션 완료 시 버프(Abnormality) 지급
	NORMAL = 5,

	// 침공 보물상자
	INVADE_TREASUREBOX = 6,

	// 고레벨필드던전 숨겨진장소로 이동 포탈
	ELITE_DUNGEON_PORTAL_HIDDEN = 7,

	MAX = 8,
};

UENUM(BlueprintType)
// 침공포털닫힘타입
enum class InvadePropDespawnType : uint8
{

	// 유효하지 않은 타입
	NONE = 0,

	// 포털 워프 MAX도달
	WARP_USER_MAX_COUNT = 1,

	// 침공시긴종료
	TIME_OVER = 2,

	MAX = 3,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline PropType PropTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return PropType::NONE;
	if (str.Compare(TEXT("QUEST"), ESearchCase::IgnoreCase) == 0)	return PropType::QUEST;
	if (str.Compare(TEXT("INVADE_PORTAL"), ESearchCase::IgnoreCase) == 0)	return PropType::INVADE_PORTAL;
	if (str.Compare(TEXT("ELITE_DUNGEON_PORTAL_UP"), ESearchCase::IgnoreCase) == 0)	return PropType::ELITE_DUNGEON_PORTAL_UP;
	if (str.Compare(TEXT("COMMON_EFFECT"), ESearchCase::IgnoreCase) == 0)	return PropType::COMMON_EFFECT;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return PropType::NORMAL;
	if (str.Compare(TEXT("INVADE_TREASUREBOX"), ESearchCase::IgnoreCase) == 0)	return PropType::INVADE_TREASUREBOX;
	if (str.Compare(TEXT("ELITE_DUNGEON_PORTAL_HIDDEN"), ESearchCase::IgnoreCase) == 0)	return PropType::ELITE_DUNGEON_PORTAL_HIDDEN;
	return PropType::MAX;
}
inline InvadePropDespawnType InvadePropDespawnTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return InvadePropDespawnType::NONE;
	if (str.Compare(TEXT("WARP_USER_MAX_COUNT"), ESearchCase::IgnoreCase) == 0)	return InvadePropDespawnType::WARP_USER_MAX_COUNT;
	if (str.Compare(TEXT("TIME_OVER"), ESearchCase::IgnoreCase) == 0)	return InvadePropDespawnType::TIME_OVER;
	return InvadePropDespawnType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString PropTypeEnumToString(PropType value)
{
	switch (value)
	{
		case PropType::NONE: return TEXT("NONE");
		case PropType::QUEST: return TEXT("QUEST");
		case PropType::INVADE_PORTAL: return TEXT("INVADE_PORTAL");
		case PropType::ELITE_DUNGEON_PORTAL_UP: return TEXT("ELITE_DUNGEON_PORTAL_UP");
		case PropType::COMMON_EFFECT: return TEXT("COMMON_EFFECT");
		case PropType::NORMAL: return TEXT("NORMAL");
		case PropType::INVADE_TREASUREBOX: return TEXT("INVADE_TREASUREBOX");
		case PropType::ELITE_DUNGEON_PORTAL_HIDDEN: return TEXT("ELITE_DUNGEON_PORTAL_HIDDEN");
	}
	return FString();
}
inline FString InvadePropDespawnTypeEnumToString(InvadePropDespawnType value)
{
	switch (value)
	{
		case InvadePropDespawnType::NONE: return TEXT("NONE");
		case InvadePropDespawnType::WARP_USER_MAX_COUNT: return TEXT("WARP_USER_MAX_COUNT");
		case InvadePropDespawnType::TIME_OVER: return TEXT("TIME_OVER");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString PropTypeEnumToDisplayKey(PropType value)
{
	switch (value)
	{
		case PropType::NONE: return TEXT("");
		case PropType::QUEST: return TEXT("");
		case PropType::INVADE_PORTAL: return TEXT("");
		case PropType::ELITE_DUNGEON_PORTAL_UP: return TEXT("");
		case PropType::COMMON_EFFECT: return TEXT("");
		case PropType::NORMAL: return TEXT("");
		case PropType::INVADE_TREASUREBOX: return TEXT("");
		case PropType::ELITE_DUNGEON_PORTAL_HIDDEN: return TEXT("");
	}
	return FString();
}
inline FString InvadePropDespawnTypeEnumToDisplayKey(InvadePropDespawnType value)
{
	switch (value)
	{
		case InvadePropDespawnType::NONE: return TEXT("");
		case InvadePropDespawnType::WARP_USER_MAX_COUNT: return TEXT("");
		case InvadePropDespawnType::TIME_OVER: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidPropType(PropType value) noexcept
{
	return (value >= PropType::NONE && value < PropType::MAX);
}

inline bool IsValidInvadePropDespawnType(InvadePropDespawnType value) noexcept
{
	return (value >= InvadePropDespawnType::NONE && value < InvadePropDespawnType::MAX);
}


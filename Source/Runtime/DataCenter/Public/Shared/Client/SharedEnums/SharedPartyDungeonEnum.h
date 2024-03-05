#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedPartyDungeonEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 파티던전 진행 단계
enum class PartyDungeonStepStatus : uint8
{

	// 대기중 상태
	WAITING = 0,

	// 클라언트 로딩 대기 상태
	LOADING = 1,

	// 시작 카운트다운 상태
	COUNTDOWN = 2,

	// 진행중 상태
	PLAYING = 3,

	// 종료직전 상태
	JUST_BEFORE_END = 4,

	// 종료 상태
	END = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// 파티 멤버 준비상태
enum class PartyReadyStatus : uint8
{

	// 대기중
	WAIT = 0,

	// 준비상태
	READY = 1,

	MAX = 2,
};

UENUM(BlueprintType)
// 파티 던전 난이도
enum class DungeonDifficulty : uint8
{

	// 없음
	NONE = 0,

	// 쉬움
	EASY = 1,

	// 노말
	NORMAL = 2,

	// 어려움
	DIFFICULT = 3,

	// 악몽
	NIGHTMARE = 4,

	MAX = 5,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline PartyDungeonStepStatus PartyDungeonStepStatusStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("WAITING"), ESearchCase::IgnoreCase) == 0)	return PartyDungeonStepStatus::WAITING;
	if (str.Compare(TEXT("LOADING"), ESearchCase::IgnoreCase) == 0)	return PartyDungeonStepStatus::LOADING;
	if (str.Compare(TEXT("COUNTDOWN"), ESearchCase::IgnoreCase) == 0)	return PartyDungeonStepStatus::COUNTDOWN;
	if (str.Compare(TEXT("PLAYING"), ESearchCase::IgnoreCase) == 0)	return PartyDungeonStepStatus::PLAYING;
	if (str.Compare(TEXT("JUST_BEFORE_END"), ESearchCase::IgnoreCase) == 0)	return PartyDungeonStepStatus::JUST_BEFORE_END;
	if (str.Compare(TEXT("END"), ESearchCase::IgnoreCase) == 0)	return PartyDungeonStepStatus::END;
	return PartyDungeonStepStatus::MAX;
}
inline PartyReadyStatus PartyReadyStatusStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("WAIT"), ESearchCase::IgnoreCase) == 0)	return PartyReadyStatus::WAIT;
	if (str.Compare(TEXT("READY"), ESearchCase::IgnoreCase) == 0)	return PartyReadyStatus::READY;
	return PartyReadyStatus::MAX;
}
inline DungeonDifficulty DungeonDifficultyStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return DungeonDifficulty::NONE;
	if (str.Compare(TEXT("EASY"), ESearchCase::IgnoreCase) == 0)	return DungeonDifficulty::EASY;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return DungeonDifficulty::NORMAL;
	if (str.Compare(TEXT("DIFFICULT"), ESearchCase::IgnoreCase) == 0)	return DungeonDifficulty::DIFFICULT;
	if (str.Compare(TEXT("NIGHTMARE"), ESearchCase::IgnoreCase) == 0)	return DungeonDifficulty::NIGHTMARE;
	return DungeonDifficulty::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString PartyDungeonStepStatusEnumToString(PartyDungeonStepStatus value)
{
	switch (value)
	{
		case PartyDungeonStepStatus::WAITING: return TEXT("WAITING");
		case PartyDungeonStepStatus::LOADING: return TEXT("LOADING");
		case PartyDungeonStepStatus::COUNTDOWN: return TEXT("COUNTDOWN");
		case PartyDungeonStepStatus::PLAYING: return TEXT("PLAYING");
		case PartyDungeonStepStatus::JUST_BEFORE_END: return TEXT("JUST_BEFORE_END");
		case PartyDungeonStepStatus::END: return TEXT("END");
	}
	return FString();
}
inline FString PartyReadyStatusEnumToString(PartyReadyStatus value)
{
	switch (value)
	{
		case PartyReadyStatus::WAIT: return TEXT("WAIT");
		case PartyReadyStatus::READY: return TEXT("READY");
	}
	return FString();
}
inline FString DungeonDifficultyEnumToString(DungeonDifficulty value)
{
	switch (value)
	{
		case DungeonDifficulty::NONE: return TEXT("NONE");
		case DungeonDifficulty::EASY: return TEXT("EASY");
		case DungeonDifficulty::NORMAL: return TEXT("NORMAL");
		case DungeonDifficulty::DIFFICULT: return TEXT("DIFFICULT");
		case DungeonDifficulty::NIGHTMARE: return TEXT("NIGHTMARE");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString PartyDungeonStepStatusEnumToDisplayKey(PartyDungeonStepStatus value)
{
	switch (value)
	{
		case PartyDungeonStepStatus::WAITING: return TEXT("");
		case PartyDungeonStepStatus::LOADING: return TEXT("");
		case PartyDungeonStepStatus::COUNTDOWN: return TEXT("");
		case PartyDungeonStepStatus::PLAYING: return TEXT("");
		case PartyDungeonStepStatus::JUST_BEFORE_END: return TEXT("");
		case PartyDungeonStepStatus::END: return TEXT("");
	}
	return FString();
}
inline FString PartyReadyStatusEnumToDisplayKey(PartyReadyStatus value)
{
	switch (value)
	{
		case PartyReadyStatus::WAIT: return TEXT("");
		case PartyReadyStatus::READY: return TEXT("");
	}
	return FString();
}
inline FString DungeonDifficultyEnumToDisplayKey(DungeonDifficulty value)
{
	switch (value)
	{
		case DungeonDifficulty::NONE: return TEXT("");
		case DungeonDifficulty::EASY: return TEXT("");
		case DungeonDifficulty::NORMAL: return TEXT("");
		case DungeonDifficulty::DIFFICULT: return TEXT("");
		case DungeonDifficulty::NIGHTMARE: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidPartyDungeonStepStatus(PartyDungeonStepStatus value) noexcept
{
	return (value >= PartyDungeonStepStatus::WAITING && value < PartyDungeonStepStatus::MAX);
}

inline bool IsValidPartyReadyStatus(PartyReadyStatus value) noexcept
{
	return (value >= PartyReadyStatus::WAIT && value < PartyReadyStatus::MAX);
}

inline bool IsValidDungeonDifficulty(DungeonDifficulty value) noexcept
{
	return (value >= DungeonDifficulty::NONE && value < DungeonDifficulty::MAX);
}


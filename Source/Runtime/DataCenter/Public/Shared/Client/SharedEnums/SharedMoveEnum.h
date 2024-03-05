#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedMoveEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
// 이동 컨트롤러 타입. 코드에서만 사용
enum class MotionControllerType : uint8
{

	// 유저
	USER = 0,

	// 점프
	JUMP = 1,

	// 낙하
	FALL = 2,

	// 스킬
	SKILL = 3,

	// AI
	AI = 4,

	// 리액션
	REACTION = 5,

	// 공포
	FEAR = 6,

	// 매혹
	CHARM = 7,

	MAX = 8,
};

UENUM()
enum class MotionType : uint8
{
	// 공통

	// 정지
	STOP = 0,

	// 걷기 이동
	WALK = 1,

	// 일반 이동
	NORMAL = 2,

	// 대시 이동
	DASH = 3,

	// 스킬로 인한 이동
	SKILL = 4,

	// 리액션으로 인한 이동
	REACTION = 5,

	// 막힌 위치를 향해 헛걸음
	BLOCK = 6,

	// FROM SERVER - 서버 제어 이동

	// 서버 제어 이동 - 오프라인 플레이
	NORMAL_OFFLINE_PLAY = 7,

	// 서버 제어 이동 - 공포 ABNORMALITY
	NORMAL_FEAR = 8,

	// 탈 것을 탄 상태에서 이동
	VEHICLE = 9,

	// 신규 추가 - 막힌 위치를 향해 헛걸음 
	 VEHICLE_BLOCK = 10,

	MAX = 11,
};

UENUM()
enum class MotionStopReason : uint8
{

	// 정상 종료 (시간 만료, 목적지 도착)
	NORMAL = 0,

	// 디스폰 되었음
	DESPAWN = 1,

	// 잠시 접속이 끊어졌음
	DISCONNECT = 2,

	// 클라와 위치가 동기화가 완전 어그러졌음
	POS_SYNC_FAIL = 3,

	// 없는 컨트롤러
	NO_CONTROLLER = 4,

	// 죽어서 종료됨
	DIE = 5,

	// 유저 자유 이동 입력
	USER = 6,

	// 유저만 이동 가능
	ONLY_USER = 7,

	// 점프
	JUMP = 8,

	// 유저 스킬 발동 (Blink, ...)
	SKILL = 9,

	// 리액션 발생
	REACTION = 10,

	// 이상상태 발생
	ABNORMALITY = 11,

	// NPC 이동 요청
	NPC = 12,

	// Warp로 인한 중지
	WARP = 13,

	// 행위 불가로 변경됨
	BEHAVIOR_DISABLED = 14,

	// 공포(FEAR) 종료 : OnRemove 등
	FEAR = 15,

	MAX = 16,
};

UENUM()
enum class FallingMethod : uint8
{

	// 낙하 없음
	NONE = 0,

	// 루트모션 낙하처리
	MOTION = 1,

	// 시스템 낙하처리
	SYSTEM = 2,

	MAX = 3,
};

UENUM()
enum class MotionAIState : uint8
{

	// Nav 길찾기
	PATH_FIND = 0,

	// 반경 체크
	DETOUR = 1,

	// 회전
	ROTATE = 2,

	// 일직선으로
	STRAIGHT = 3,

	MAX = 4,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline MotionControllerType MotionControllerTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("USER"), ESearchCase::IgnoreCase) == 0)	return MotionControllerType::USER;
	if (str.Compare(TEXT("JUMP"), ESearchCase::IgnoreCase) == 0)	return MotionControllerType::JUMP;
	if (str.Compare(TEXT("FALL"), ESearchCase::IgnoreCase) == 0)	return MotionControllerType::FALL;
	if (str.Compare(TEXT("SKILL"), ESearchCase::IgnoreCase) == 0)	return MotionControllerType::SKILL;
	if (str.Compare(TEXT("AI"), ESearchCase::IgnoreCase) == 0)	return MotionControllerType::AI;
	if (str.Compare(TEXT("REACTION"), ESearchCase::IgnoreCase) == 0)	return MotionControllerType::REACTION;
	if (str.Compare(TEXT("FEAR"), ESearchCase::IgnoreCase) == 0)	return MotionControllerType::FEAR;
	if (str.Compare(TEXT("CHARM"), ESearchCase::IgnoreCase) == 0)	return MotionControllerType::CHARM;
	return MotionControllerType::MAX;
}
inline MotionType MotionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("STOP"), ESearchCase::IgnoreCase) == 0)	return MotionType::STOP;
	if (str.Compare(TEXT("WALK"), ESearchCase::IgnoreCase) == 0)	return MotionType::WALK;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return MotionType::NORMAL;
	if (str.Compare(TEXT("DASH"), ESearchCase::IgnoreCase) == 0)	return MotionType::DASH;
	if (str.Compare(TEXT("SKILL"), ESearchCase::IgnoreCase) == 0)	return MotionType::SKILL;
	if (str.Compare(TEXT("REACTION"), ESearchCase::IgnoreCase) == 0)	return MotionType::REACTION;
	if (str.Compare(TEXT("BLOCK"), ESearchCase::IgnoreCase) == 0)	return MotionType::BLOCK;
	if (str.Compare(TEXT("NORMAL_OFFLINE_PLAY"), ESearchCase::IgnoreCase) == 0)	return MotionType::NORMAL_OFFLINE_PLAY;
	if (str.Compare(TEXT("NORMAL_FEAR"), ESearchCase::IgnoreCase) == 0)	return MotionType::NORMAL_FEAR;
	if (str.Compare(TEXT("VEHICLE"), ESearchCase::IgnoreCase) == 0)	return MotionType::VEHICLE;
	if (str.Compare(TEXT(" VEHICLE_BLOCK"), ESearchCase::IgnoreCase) == 0)	return MotionType:: VEHICLE_BLOCK;
	return MotionType::MAX;
}
inline MotionStopReason MotionStopReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::NORMAL;
	if (str.Compare(TEXT("DESPAWN"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::DESPAWN;
	if (str.Compare(TEXT("DISCONNECT"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::DISCONNECT;
	if (str.Compare(TEXT("POS_SYNC_FAIL"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::POS_SYNC_FAIL;
	if (str.Compare(TEXT("NO_CONTROLLER"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::NO_CONTROLLER;
	if (str.Compare(TEXT("DIE"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::DIE;
	if (str.Compare(TEXT("USER"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::USER;
	if (str.Compare(TEXT("ONLY_USER"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::ONLY_USER;
	if (str.Compare(TEXT("JUMP"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::JUMP;
	if (str.Compare(TEXT("SKILL"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::SKILL;
	if (str.Compare(TEXT("REACTION"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::REACTION;
	if (str.Compare(TEXT("ABNORMALITY"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::ABNORMALITY;
	if (str.Compare(TEXT("NPC"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::NPC;
	if (str.Compare(TEXT("WARP"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::WARP;
	if (str.Compare(TEXT("BEHAVIOR_DISABLED"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::BEHAVIOR_DISABLED;
	if (str.Compare(TEXT("FEAR"), ESearchCase::IgnoreCase) == 0)	return MotionStopReason::FEAR;
	return MotionStopReason::MAX;
}
inline FallingMethod FallingMethodStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return FallingMethod::NONE;
	if (str.Compare(TEXT("MOTION"), ESearchCase::IgnoreCase) == 0)	return FallingMethod::MOTION;
	if (str.Compare(TEXT("SYSTEM"), ESearchCase::IgnoreCase) == 0)	return FallingMethod::SYSTEM;
	return FallingMethod::MAX;
}
inline MotionAIState MotionAIStateStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("PATH_FIND"), ESearchCase::IgnoreCase) == 0)	return MotionAIState::PATH_FIND;
	if (str.Compare(TEXT("DETOUR"), ESearchCase::IgnoreCase) == 0)	return MotionAIState::DETOUR;
	if (str.Compare(TEXT("ROTATE"), ESearchCase::IgnoreCase) == 0)	return MotionAIState::ROTATE;
	if (str.Compare(TEXT("STRAIGHT"), ESearchCase::IgnoreCase) == 0)	return MotionAIState::STRAIGHT;
	return MotionAIState::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString MotionControllerTypeEnumToString(MotionControllerType value)
{
	switch (value)
	{
		case MotionControllerType::USER: return TEXT("USER");
		case MotionControllerType::JUMP: return TEXT("JUMP");
		case MotionControllerType::FALL: return TEXT("FALL");
		case MotionControllerType::SKILL: return TEXT("SKILL");
		case MotionControllerType::AI: return TEXT("AI");
		case MotionControllerType::REACTION: return TEXT("REACTION");
		case MotionControllerType::FEAR: return TEXT("FEAR");
		case MotionControllerType::CHARM: return TEXT("CHARM");
	}
	return FString();
}
inline FString MotionTypeEnumToString(MotionType value)
{
	switch (value)
	{
		case MotionType::STOP: return TEXT("STOP");
		case MotionType::WALK: return TEXT("WALK");
		case MotionType::NORMAL: return TEXT("NORMAL");
		case MotionType::DASH: return TEXT("DASH");
		case MotionType::SKILL: return TEXT("SKILL");
		case MotionType::REACTION: return TEXT("REACTION");
		case MotionType::BLOCK: return TEXT("BLOCK");
		case MotionType::NORMAL_OFFLINE_PLAY: return TEXT("NORMAL_OFFLINE_PLAY");
		case MotionType::NORMAL_FEAR: return TEXT("NORMAL_FEAR");
		case MotionType::VEHICLE: return TEXT("VEHICLE");
		case MotionType:: VEHICLE_BLOCK: return TEXT(" VEHICLE_BLOCK");
	}
	return FString();
}
inline FString MotionStopReasonEnumToString(MotionStopReason value)
{
	switch (value)
	{
		case MotionStopReason::NORMAL: return TEXT("NORMAL");
		case MotionStopReason::DESPAWN: return TEXT("DESPAWN");
		case MotionStopReason::DISCONNECT: return TEXT("DISCONNECT");
		case MotionStopReason::POS_SYNC_FAIL: return TEXT("POS_SYNC_FAIL");
		case MotionStopReason::NO_CONTROLLER: return TEXT("NO_CONTROLLER");
		case MotionStopReason::DIE: return TEXT("DIE");
		case MotionStopReason::USER: return TEXT("USER");
		case MotionStopReason::ONLY_USER: return TEXT("ONLY_USER");
		case MotionStopReason::JUMP: return TEXT("JUMP");
		case MotionStopReason::SKILL: return TEXT("SKILL");
		case MotionStopReason::REACTION: return TEXT("REACTION");
		case MotionStopReason::ABNORMALITY: return TEXT("ABNORMALITY");
		case MotionStopReason::NPC: return TEXT("NPC");
		case MotionStopReason::WARP: return TEXT("WARP");
		case MotionStopReason::BEHAVIOR_DISABLED: return TEXT("BEHAVIOR_DISABLED");
		case MotionStopReason::FEAR: return TEXT("FEAR");
	}
	return FString();
}
inline FString FallingMethodEnumToString(FallingMethod value)
{
	switch (value)
	{
		case FallingMethod::NONE: return TEXT("NONE");
		case FallingMethod::MOTION: return TEXT("MOTION");
		case FallingMethod::SYSTEM: return TEXT("SYSTEM");
	}
	return FString();
}
inline FString MotionAIStateEnumToString(MotionAIState value)
{
	switch (value)
	{
		case MotionAIState::PATH_FIND: return TEXT("PATH_FIND");
		case MotionAIState::DETOUR: return TEXT("DETOUR");
		case MotionAIState::ROTATE: return TEXT("ROTATE");
		case MotionAIState::STRAIGHT: return TEXT("STRAIGHT");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString MotionControllerTypeEnumToDisplayKey(MotionControllerType value)
{
	switch (value)
	{
		case MotionControllerType::USER: return TEXT("");
		case MotionControllerType::JUMP: return TEXT("");
		case MotionControllerType::FALL: return TEXT("");
		case MotionControllerType::SKILL: return TEXT("");
		case MotionControllerType::AI: return TEXT("");
		case MotionControllerType::REACTION: return TEXT("");
		case MotionControllerType::FEAR: return TEXT("");
		case MotionControllerType::CHARM: return TEXT("");
	}
	return FString();
}
inline FString MotionTypeEnumToDisplayKey(MotionType value)
{
	switch (value)
	{
		case MotionType::STOP: return TEXT("");
		case MotionType::WALK: return TEXT("");
		case MotionType::NORMAL: return TEXT("");
		case MotionType::DASH: return TEXT("");
		case MotionType::SKILL: return TEXT("");
		case MotionType::REACTION: return TEXT("");
		case MotionType::BLOCK: return TEXT("");
		case MotionType::NORMAL_OFFLINE_PLAY: return TEXT("");
		case MotionType::NORMAL_FEAR: return TEXT("");
		case MotionType::VEHICLE: return TEXT("");
		case MotionType:: VEHICLE_BLOCK: return TEXT("");
	}
	return FString();
}
inline FString MotionStopReasonEnumToDisplayKey(MotionStopReason value)
{
	switch (value)
	{
		case MotionStopReason::NORMAL: return TEXT("");
		case MotionStopReason::DESPAWN: return TEXT("");
		case MotionStopReason::DISCONNECT: return TEXT("");
		case MotionStopReason::POS_SYNC_FAIL: return TEXT("");
		case MotionStopReason::NO_CONTROLLER: return TEXT("");
		case MotionStopReason::DIE: return TEXT("");
		case MotionStopReason::USER: return TEXT("");
		case MotionStopReason::ONLY_USER: return TEXT("");
		case MotionStopReason::JUMP: return TEXT("");
		case MotionStopReason::SKILL: return TEXT("");
		case MotionStopReason::REACTION: return TEXT("");
		case MotionStopReason::ABNORMALITY: return TEXT("");
		case MotionStopReason::NPC: return TEXT("");
		case MotionStopReason::WARP: return TEXT("");
		case MotionStopReason::BEHAVIOR_DISABLED: return TEXT("");
		case MotionStopReason::FEAR: return TEXT("");
	}
	return FString();
}
inline FString FallingMethodEnumToDisplayKey(FallingMethod value)
{
	switch (value)
	{
		case FallingMethod::NONE: return TEXT("");
		case FallingMethod::MOTION: return TEXT("");
		case FallingMethod::SYSTEM: return TEXT("");
	}
	return FString();
}
inline FString MotionAIStateEnumToDisplayKey(MotionAIState value)
{
	switch (value)
	{
		case MotionAIState::PATH_FIND: return TEXT("");
		case MotionAIState::DETOUR: return TEXT("");
		case MotionAIState::ROTATE: return TEXT("");
		case MotionAIState::STRAIGHT: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidMotionControllerType(MotionControllerType value) noexcept
{
	return (value >= MotionControllerType::USER && value < MotionControllerType::MAX);
}

inline bool IsValidMotionType(MotionType value) noexcept
{
	return (value >= MotionType::STOP && value < MotionType::MAX);
}

inline bool IsValidMotionStopReason(MotionStopReason value) noexcept
{
	return (value >= MotionStopReason::NORMAL && value < MotionStopReason::MAX);
}

inline bool IsValidFallingMethod(FallingMethod value) noexcept
{
	return (value >= FallingMethod::NONE && value < FallingMethod::MAX);
}

inline bool IsValidMotionAIState(MotionAIState value) noexcept
{
	return (value >= MotionAIState::PATH_FIND && value < MotionAIState::MAX);
}


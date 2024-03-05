#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// AiDataEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class AiConditionType : uint8
{

	// None
	NONE = 0,

	// 대상이 주어진 거리내에 있는지 체크
	IN_DISTANCE = 1,

	// 대상의 HP 비율을 체크
	HP_RATE = 2,

	// 스폰 완료 이후 경과된 시간을 체크
	SPAWN_TIME = 3,

	// 전투 시작 이후 경과된 시간을 체크
	BATTLE_TIME = 4,

	// 타겟에게 적용 중인 이상상태를 체크
	APPLIED_ABNORMALITY = 5,

	MAX = 6,
};

UENUM(BlueprintType)
// Ai Action 타입
enum class AiActionType : uint8
{

	// Idle 상태
	IDLE = 0,

	// 스킬 사용
	USE_SKILL = 1,

	MAX = 2,
};

UENUM(BlueprintType)
enum class AiConditionTargetMethod : uint8
{

	// None
	NONE = 0,

	// 자기 자신
	SELF = 1,

	// 타겟팅 대상
	TARGET = 2,

	MAX = 3,
};

UENUM(BlueprintType)
enum class AiConditionMethod : uint8
{

	// None
	NONE = 0,

	// 이하
	BELOW = 1,

	// 이상
	ABOVE = 2,

	// 동일
	EQUAL = 3,

	// 범위
	RANGE = 4,

	MAX = 5,
};

UENUM(BlueprintType)
enum class AiActionRunToRepeatType : uint8
{

	// 매 회차마다 실행(기본값)
	NONE = 0,

	// RunToRepeatCount 에 맞는 실행 회차에 일회성 실행
	ONCENESS = 1,

	// RunToRepeatCount 에 맞는 실행 회차에 주기적 실행(스펙 예외)
	CYCLE = 2,

	// RunToRepeatCount 이상의 실행 회차에 실행(스펙 예외)
	OVER_TIME = 3,

	// AI 전투중 한번만 실행
	BATTLE_ONCE = 4,

	MAX = 5,
};

UENUM(BlueprintType)
enum class AiActionTargetConditionType : uint8
{

	// None
	NONE = 0,

	// 총 어그로 리스트에서 특정 혹은 범위 안의 순위를 검증
	TOTAL_AGGRO_RANK = 1,

	// 힐 어그로 리스트에서 특정 혹은 범위 안의 순위를 검증
	HEAL_AGGRO_RANK = 2,

	// 대미지 어그로 리스트에서 특정 혹은 범위 안의 순위를 검증
	DEAL_AGGRO_RANK = 3,

	// 특정 이상상태에 대한 검증
	ABNORMALITY_ID = 4,

	// 어그로 무시, 어그로 리스트 중 랜덤대상
	RANDOM = 5,

	MAX = 6,
};

UENUM(BlueprintType)
enum class AiActionTargetConditionMethod : uint8
{

	// None
	NONE = 0,

	// 범위
	RANGE = 1,

	// 이하
	BELOW = 2,

	// 이상
	ABOVE = 3,

	// 제외 - AiActionTargetConditionType::ABNORMALITY_ID 일 경우 사용. ex) 특정 이상상태 ID 가 적용되어 있지 않을 경우
	EXCLUDE = 4,

	// 포함 - AiActionTargetConditionType::ABNORMALITY_ID 일 경우 사용. ex) 특정 이상상태 ID 가 적용되어 있을 경우
	INCLUDE = 5,

	MAX = 6,
};

UENUM(BlueprintType)
enum class AiActionTriggerConditionType : uint8
{

	// None
	NONE = 0,

	// 오너의 바라보는 방향을 기준으로 특정 각도를 검증
	ANGLE = 1,

	// 오너의 위치를 기준으로 특정 거리를 검증
	DISTANCE = 2,

	// 오너의 어그로에 등록된 대상 수를 검증
	REGISTED_AGGRO_COUNT = 3,

	MAX = 4,
};

UENUM(BlueprintType)
enum class AiActionTriggerConditionMethod : uint8
{

	// None
	NONE = 0,

	// 범위
	RANGE = 1,

	// 이하
	BELOW = 2,

	// 이상
	ABOVE = 3,

	MAX = 4,
};

UENUM(BlueprintType)
enum class AiTargetRenewalConditionType : uint8
{

	// None
	NONE = 0,

	// 대상 추적 지속 시간 비교
	MOVE_TO_TARGET_TIMER = 1,

	// 전투 대기 지속 시간 비교
	BATTLE_IDLE_TIMER = 2,

	MAX = 3,
};

UENUM(BlueprintType)
enum class AiTargetRenewalConditionMethod : uint8
{

	// None
	NONE = 0,

	// 이하(같거나 클 경우)
	ABOVE = 1,

	MAX = 2,
};

UENUM(BlueprintType)
enum class AggroPenaltyConditionType : uint8
{

	// None
	NONE = 0,

	// AiTargetRenewal에 의한 대상 변경이 발생한 경우.
	TARGET_RENEWAL_CHANGED_TARGET = 1,

	// MOVE_TO_TARGET_TIMER 에 의한 대상 갱신이 발생한 경우.
	AGGRO_PENALTY_MOVE_TO_TARGET_TIMER = 2,

	// BATTLE_IDLE_IMTER 에 의한 대상 갱신이 발생한 경우.
	AGGRO_PENALTY_BATTLE_IDLE_TIMER = 3,

	MAX = 4,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline AiConditionType AiConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AiConditionType::NONE;
	if (str.Compare(TEXT("IN_DISTANCE"), ESearchCase::IgnoreCase) == 0)	return AiConditionType::IN_DISTANCE;
	if (str.Compare(TEXT("HP_RATE"), ESearchCase::IgnoreCase) == 0)	return AiConditionType::HP_RATE;
	if (str.Compare(TEXT("SPAWN_TIME"), ESearchCase::IgnoreCase) == 0)	return AiConditionType::SPAWN_TIME;
	if (str.Compare(TEXT("BATTLE_TIME"), ESearchCase::IgnoreCase) == 0)	return AiConditionType::BATTLE_TIME;
	if (str.Compare(TEXT("APPLIED_ABNORMALITY"), ESearchCase::IgnoreCase) == 0)	return AiConditionType::APPLIED_ABNORMALITY;
	return AiConditionType::MAX;
}
inline AiActionType AiActionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("IDLE"), ESearchCase::IgnoreCase) == 0)	return AiActionType::IDLE;
	if (str.Compare(TEXT("USE_SKILL"), ESearchCase::IgnoreCase) == 0)	return AiActionType::USE_SKILL;
	return AiActionType::MAX;
}
inline AiConditionTargetMethod AiConditionTargetMethodStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AiConditionTargetMethod::NONE;
	if (str.Compare(TEXT("SELF"), ESearchCase::IgnoreCase) == 0)	return AiConditionTargetMethod::SELF;
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return AiConditionTargetMethod::TARGET;
	return AiConditionTargetMethod::MAX;
}
inline AiConditionMethod AiConditionMethodStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AiConditionMethod::NONE;
	if (str.Compare(TEXT("BELOW"), ESearchCase::IgnoreCase) == 0)	return AiConditionMethod::BELOW;
	if (str.Compare(TEXT("ABOVE"), ESearchCase::IgnoreCase) == 0)	return AiConditionMethod::ABOVE;
	if (str.Compare(TEXT("EQUAL"), ESearchCase::IgnoreCase) == 0)	return AiConditionMethod::EQUAL;
	if (str.Compare(TEXT("RANGE"), ESearchCase::IgnoreCase) == 0)	return AiConditionMethod::RANGE;
	return AiConditionMethod::MAX;
}
inline AiActionRunToRepeatType AiActionRunToRepeatTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AiActionRunToRepeatType::NONE;
	if (str.Compare(TEXT("ONCENESS"), ESearchCase::IgnoreCase) == 0)	return AiActionRunToRepeatType::ONCENESS;
	if (str.Compare(TEXT("CYCLE"), ESearchCase::IgnoreCase) == 0)	return AiActionRunToRepeatType::CYCLE;
	if (str.Compare(TEXT("OVER_TIME"), ESearchCase::IgnoreCase) == 0)	return AiActionRunToRepeatType::OVER_TIME;
	if (str.Compare(TEXT("BATTLE_ONCE"), ESearchCase::IgnoreCase) == 0)	return AiActionRunToRepeatType::BATTLE_ONCE;
	return AiActionRunToRepeatType::MAX;
}
inline AiActionTargetConditionType AiActionTargetConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionType::NONE;
	if (str.Compare(TEXT("TOTAL_AGGRO_RANK"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionType::TOTAL_AGGRO_RANK;
	if (str.Compare(TEXT("HEAL_AGGRO_RANK"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionType::HEAL_AGGRO_RANK;
	if (str.Compare(TEXT("DEAL_AGGRO_RANK"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionType::DEAL_AGGRO_RANK;
	if (str.Compare(TEXT("ABNORMALITY_ID"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionType::ABNORMALITY_ID;
	if (str.Compare(TEXT("RANDOM"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionType::RANDOM;
	return AiActionTargetConditionType::MAX;
}
inline AiActionTargetConditionMethod AiActionTargetConditionMethodStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionMethod::NONE;
	if (str.Compare(TEXT("RANGE"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionMethod::RANGE;
	if (str.Compare(TEXT("BELOW"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionMethod::BELOW;
	if (str.Compare(TEXT("ABOVE"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionMethod::ABOVE;
	if (str.Compare(TEXT("EXCLUDE"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionMethod::EXCLUDE;
	if (str.Compare(TEXT("INCLUDE"), ESearchCase::IgnoreCase) == 0)	return AiActionTargetConditionMethod::INCLUDE;
	return AiActionTargetConditionMethod::MAX;
}
inline AiActionTriggerConditionType AiActionTriggerConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AiActionTriggerConditionType::NONE;
	if (str.Compare(TEXT("ANGLE"), ESearchCase::IgnoreCase) == 0)	return AiActionTriggerConditionType::ANGLE;
	if (str.Compare(TEXT("DISTANCE"), ESearchCase::IgnoreCase) == 0)	return AiActionTriggerConditionType::DISTANCE;
	if (str.Compare(TEXT("REGISTED_AGGRO_COUNT"), ESearchCase::IgnoreCase) == 0)	return AiActionTriggerConditionType::REGISTED_AGGRO_COUNT;
	return AiActionTriggerConditionType::MAX;
}
inline AiActionTriggerConditionMethod AiActionTriggerConditionMethodStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AiActionTriggerConditionMethod::NONE;
	if (str.Compare(TEXT("RANGE"), ESearchCase::IgnoreCase) == 0)	return AiActionTriggerConditionMethod::RANGE;
	if (str.Compare(TEXT("BELOW"), ESearchCase::IgnoreCase) == 0)	return AiActionTriggerConditionMethod::BELOW;
	if (str.Compare(TEXT("ABOVE"), ESearchCase::IgnoreCase) == 0)	return AiActionTriggerConditionMethod::ABOVE;
	return AiActionTriggerConditionMethod::MAX;
}
inline AiTargetRenewalConditionType AiTargetRenewalConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AiTargetRenewalConditionType::NONE;
	if (str.Compare(TEXT("MOVE_TO_TARGET_TIMER"), ESearchCase::IgnoreCase) == 0)	return AiTargetRenewalConditionType::MOVE_TO_TARGET_TIMER;
	if (str.Compare(TEXT("BATTLE_IDLE_TIMER"), ESearchCase::IgnoreCase) == 0)	return AiTargetRenewalConditionType::BATTLE_IDLE_TIMER;
	return AiTargetRenewalConditionType::MAX;
}
inline AiTargetRenewalConditionMethod AiTargetRenewalConditionMethodStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AiTargetRenewalConditionMethod::NONE;
	if (str.Compare(TEXT("ABOVE"), ESearchCase::IgnoreCase) == 0)	return AiTargetRenewalConditionMethod::ABOVE;
	return AiTargetRenewalConditionMethod::MAX;
}
inline AggroPenaltyConditionType AggroPenaltyConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return AggroPenaltyConditionType::NONE;
	if (str.Compare(TEXT("TARGET_RENEWAL_CHANGED_TARGET"), ESearchCase::IgnoreCase) == 0)	return AggroPenaltyConditionType::TARGET_RENEWAL_CHANGED_TARGET;
	if (str.Compare(TEXT("AGGRO_PENALTY_MOVE_TO_TARGET_TIMER"), ESearchCase::IgnoreCase) == 0)	return AggroPenaltyConditionType::AGGRO_PENALTY_MOVE_TO_TARGET_TIMER;
	if (str.Compare(TEXT("AGGRO_PENALTY_BATTLE_IDLE_TIMER"), ESearchCase::IgnoreCase) == 0)	return AggroPenaltyConditionType::AGGRO_PENALTY_BATTLE_IDLE_TIMER;
	return AggroPenaltyConditionType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString AiConditionTypeEnumToString(AiConditionType value)
{
	switch (value)
	{
		case AiConditionType::NONE: return TEXT("NONE");
		case AiConditionType::IN_DISTANCE: return TEXT("IN_DISTANCE");
		case AiConditionType::HP_RATE: return TEXT("HP_RATE");
		case AiConditionType::SPAWN_TIME: return TEXT("SPAWN_TIME");
		case AiConditionType::BATTLE_TIME: return TEXT("BATTLE_TIME");
		case AiConditionType::APPLIED_ABNORMALITY: return TEXT("APPLIED_ABNORMALITY");
	}
	return FString();
}
inline FString AiActionTypeEnumToString(AiActionType value)
{
	switch (value)
	{
		case AiActionType::IDLE: return TEXT("IDLE");
		case AiActionType::USE_SKILL: return TEXT("USE_SKILL");
	}
	return FString();
}
inline FString AiConditionTargetMethodEnumToString(AiConditionTargetMethod value)
{
	switch (value)
	{
		case AiConditionTargetMethod::NONE: return TEXT("NONE");
		case AiConditionTargetMethod::SELF: return TEXT("SELF");
		case AiConditionTargetMethod::TARGET: return TEXT("TARGET");
	}
	return FString();
}
inline FString AiConditionMethodEnumToString(AiConditionMethod value)
{
	switch (value)
	{
		case AiConditionMethod::NONE: return TEXT("NONE");
		case AiConditionMethod::BELOW: return TEXT("BELOW");
		case AiConditionMethod::ABOVE: return TEXT("ABOVE");
		case AiConditionMethod::EQUAL: return TEXT("EQUAL");
		case AiConditionMethod::RANGE: return TEXT("RANGE");
	}
	return FString();
}
inline FString AiActionRunToRepeatTypeEnumToString(AiActionRunToRepeatType value)
{
	switch (value)
	{
		case AiActionRunToRepeatType::NONE: return TEXT("NONE");
		case AiActionRunToRepeatType::ONCENESS: return TEXT("ONCENESS");
		case AiActionRunToRepeatType::CYCLE: return TEXT("CYCLE");
		case AiActionRunToRepeatType::OVER_TIME: return TEXT("OVER_TIME");
		case AiActionRunToRepeatType::BATTLE_ONCE: return TEXT("BATTLE_ONCE");
	}
	return FString();
}
inline FString AiActionTargetConditionTypeEnumToString(AiActionTargetConditionType value)
{
	switch (value)
	{
		case AiActionTargetConditionType::NONE: return TEXT("NONE");
		case AiActionTargetConditionType::TOTAL_AGGRO_RANK: return TEXT("TOTAL_AGGRO_RANK");
		case AiActionTargetConditionType::HEAL_AGGRO_RANK: return TEXT("HEAL_AGGRO_RANK");
		case AiActionTargetConditionType::DEAL_AGGRO_RANK: return TEXT("DEAL_AGGRO_RANK");
		case AiActionTargetConditionType::ABNORMALITY_ID: return TEXT("ABNORMALITY_ID");
		case AiActionTargetConditionType::RANDOM: return TEXT("RANDOM");
	}
	return FString();
}
inline FString AiActionTargetConditionMethodEnumToString(AiActionTargetConditionMethod value)
{
	switch (value)
	{
		case AiActionTargetConditionMethod::NONE: return TEXT("NONE");
		case AiActionTargetConditionMethod::RANGE: return TEXT("RANGE");
		case AiActionTargetConditionMethod::BELOW: return TEXT("BELOW");
		case AiActionTargetConditionMethod::ABOVE: return TEXT("ABOVE");
		case AiActionTargetConditionMethod::EXCLUDE: return TEXT("EXCLUDE");
		case AiActionTargetConditionMethod::INCLUDE: return TEXT("INCLUDE");
	}
	return FString();
}
inline FString AiActionTriggerConditionTypeEnumToString(AiActionTriggerConditionType value)
{
	switch (value)
	{
		case AiActionTriggerConditionType::NONE: return TEXT("NONE");
		case AiActionTriggerConditionType::ANGLE: return TEXT("ANGLE");
		case AiActionTriggerConditionType::DISTANCE: return TEXT("DISTANCE");
		case AiActionTriggerConditionType::REGISTED_AGGRO_COUNT: return TEXT("REGISTED_AGGRO_COUNT");
	}
	return FString();
}
inline FString AiActionTriggerConditionMethodEnumToString(AiActionTriggerConditionMethod value)
{
	switch (value)
	{
		case AiActionTriggerConditionMethod::NONE: return TEXT("NONE");
		case AiActionTriggerConditionMethod::RANGE: return TEXT("RANGE");
		case AiActionTriggerConditionMethod::BELOW: return TEXT("BELOW");
		case AiActionTriggerConditionMethod::ABOVE: return TEXT("ABOVE");
	}
	return FString();
}
inline FString AiTargetRenewalConditionTypeEnumToString(AiTargetRenewalConditionType value)
{
	switch (value)
	{
		case AiTargetRenewalConditionType::NONE: return TEXT("NONE");
		case AiTargetRenewalConditionType::MOVE_TO_TARGET_TIMER: return TEXT("MOVE_TO_TARGET_TIMER");
		case AiTargetRenewalConditionType::BATTLE_IDLE_TIMER: return TEXT("BATTLE_IDLE_TIMER");
	}
	return FString();
}
inline FString AiTargetRenewalConditionMethodEnumToString(AiTargetRenewalConditionMethod value)
{
	switch (value)
	{
		case AiTargetRenewalConditionMethod::NONE: return TEXT("NONE");
		case AiTargetRenewalConditionMethod::ABOVE: return TEXT("ABOVE");
	}
	return FString();
}
inline FString AggroPenaltyConditionTypeEnumToString(AggroPenaltyConditionType value)
{
	switch (value)
	{
		case AggroPenaltyConditionType::NONE: return TEXT("NONE");
		case AggroPenaltyConditionType::TARGET_RENEWAL_CHANGED_TARGET: return TEXT("TARGET_RENEWAL_CHANGED_TARGET");
		case AggroPenaltyConditionType::AGGRO_PENALTY_MOVE_TO_TARGET_TIMER: return TEXT("AGGRO_PENALTY_MOVE_TO_TARGET_TIMER");
		case AggroPenaltyConditionType::AGGRO_PENALTY_BATTLE_IDLE_TIMER: return TEXT("AGGRO_PENALTY_BATTLE_IDLE_TIMER");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString AiConditionTypeEnumToDisplayKey(AiConditionType value)
{
	switch (value)
	{
		case AiConditionType::NONE: return TEXT("");
		case AiConditionType::IN_DISTANCE: return TEXT("");
		case AiConditionType::HP_RATE: return TEXT("");
		case AiConditionType::SPAWN_TIME: return TEXT("");
		case AiConditionType::BATTLE_TIME: return TEXT("");
		case AiConditionType::APPLIED_ABNORMALITY: return TEXT("");
	}
	return FString();
}
inline FString AiActionTypeEnumToDisplayKey(AiActionType value)
{
	switch (value)
	{
		case AiActionType::IDLE: return TEXT("");
		case AiActionType::USE_SKILL: return TEXT("");
	}
	return FString();
}
inline FString AiConditionTargetMethodEnumToDisplayKey(AiConditionTargetMethod value)
{
	switch (value)
	{
		case AiConditionTargetMethod::NONE: return TEXT("");
		case AiConditionTargetMethod::SELF: return TEXT("");
		case AiConditionTargetMethod::TARGET: return TEXT("");
	}
	return FString();
}
inline FString AiConditionMethodEnumToDisplayKey(AiConditionMethod value)
{
	switch (value)
	{
		case AiConditionMethod::NONE: return TEXT("");
		case AiConditionMethod::BELOW: return TEXT("");
		case AiConditionMethod::ABOVE: return TEXT("");
		case AiConditionMethod::EQUAL: return TEXT("");
		case AiConditionMethod::RANGE: return TEXT("");
	}
	return FString();
}
inline FString AiActionRunToRepeatTypeEnumToDisplayKey(AiActionRunToRepeatType value)
{
	switch (value)
	{
		case AiActionRunToRepeatType::NONE: return TEXT("");
		case AiActionRunToRepeatType::ONCENESS: return TEXT("");
		case AiActionRunToRepeatType::CYCLE: return TEXT("");
		case AiActionRunToRepeatType::OVER_TIME: return TEXT("");
		case AiActionRunToRepeatType::BATTLE_ONCE: return TEXT("");
	}
	return FString();
}
inline FString AiActionTargetConditionTypeEnumToDisplayKey(AiActionTargetConditionType value)
{
	switch (value)
	{
		case AiActionTargetConditionType::NONE: return TEXT("");
		case AiActionTargetConditionType::TOTAL_AGGRO_RANK: return TEXT("");
		case AiActionTargetConditionType::HEAL_AGGRO_RANK: return TEXT("");
		case AiActionTargetConditionType::DEAL_AGGRO_RANK: return TEXT("");
		case AiActionTargetConditionType::ABNORMALITY_ID: return TEXT("");
		case AiActionTargetConditionType::RANDOM: return TEXT("");
	}
	return FString();
}
inline FString AiActionTargetConditionMethodEnumToDisplayKey(AiActionTargetConditionMethod value)
{
	switch (value)
	{
		case AiActionTargetConditionMethod::NONE: return TEXT("");
		case AiActionTargetConditionMethod::RANGE: return TEXT("");
		case AiActionTargetConditionMethod::BELOW: return TEXT("");
		case AiActionTargetConditionMethod::ABOVE: return TEXT("");
		case AiActionTargetConditionMethod::EXCLUDE: return TEXT("");
		case AiActionTargetConditionMethod::INCLUDE: return TEXT("");
	}
	return FString();
}
inline FString AiActionTriggerConditionTypeEnumToDisplayKey(AiActionTriggerConditionType value)
{
	switch (value)
	{
		case AiActionTriggerConditionType::NONE: return TEXT("");
		case AiActionTriggerConditionType::ANGLE: return TEXT("");
		case AiActionTriggerConditionType::DISTANCE: return TEXT("");
		case AiActionTriggerConditionType::REGISTED_AGGRO_COUNT: return TEXT("");
	}
	return FString();
}
inline FString AiActionTriggerConditionMethodEnumToDisplayKey(AiActionTriggerConditionMethod value)
{
	switch (value)
	{
		case AiActionTriggerConditionMethod::NONE: return TEXT("");
		case AiActionTriggerConditionMethod::RANGE: return TEXT("");
		case AiActionTriggerConditionMethod::BELOW: return TEXT("");
		case AiActionTriggerConditionMethod::ABOVE: return TEXT("");
	}
	return FString();
}
inline FString AiTargetRenewalConditionTypeEnumToDisplayKey(AiTargetRenewalConditionType value)
{
	switch (value)
	{
		case AiTargetRenewalConditionType::NONE: return TEXT("");
		case AiTargetRenewalConditionType::MOVE_TO_TARGET_TIMER: return TEXT("");
		case AiTargetRenewalConditionType::BATTLE_IDLE_TIMER: return TEXT("");
	}
	return FString();
}
inline FString AiTargetRenewalConditionMethodEnumToDisplayKey(AiTargetRenewalConditionMethod value)
{
	switch (value)
	{
		case AiTargetRenewalConditionMethod::NONE: return TEXT("");
		case AiTargetRenewalConditionMethod::ABOVE: return TEXT("");
	}
	return FString();
}
inline FString AggroPenaltyConditionTypeEnumToDisplayKey(AggroPenaltyConditionType value)
{
	switch (value)
	{
		case AggroPenaltyConditionType::NONE: return TEXT("");
		case AggroPenaltyConditionType::TARGET_RENEWAL_CHANGED_TARGET: return TEXT("");
		case AggroPenaltyConditionType::AGGRO_PENALTY_MOVE_TO_TARGET_TIMER: return TEXT("");
		case AggroPenaltyConditionType::AGGRO_PENALTY_BATTLE_IDLE_TIMER: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidAiConditionType(AiConditionType value) noexcept
{
	return (value >= AiConditionType::NONE && value < AiConditionType::MAX);
}

inline bool IsValidAiActionType(AiActionType value) noexcept
{
	return (value >= AiActionType::IDLE && value < AiActionType::MAX);
}

inline bool IsValidAiConditionTargetMethod(AiConditionTargetMethod value) noexcept
{
	return (value >= AiConditionTargetMethod::NONE && value < AiConditionTargetMethod::MAX);
}

inline bool IsValidAiConditionMethod(AiConditionMethod value) noexcept
{
	return (value >= AiConditionMethod::NONE && value < AiConditionMethod::MAX);
}

inline bool IsValidAiActionRunToRepeatType(AiActionRunToRepeatType value) noexcept
{
	return (value >= AiActionRunToRepeatType::NONE && value < AiActionRunToRepeatType::MAX);
}

inline bool IsValidAiActionTargetConditionType(AiActionTargetConditionType value) noexcept
{
	return (value >= AiActionTargetConditionType::NONE && value < AiActionTargetConditionType::MAX);
}

inline bool IsValidAiActionTargetConditionMethod(AiActionTargetConditionMethod value) noexcept
{
	return (value >= AiActionTargetConditionMethod::NONE && value < AiActionTargetConditionMethod::MAX);
}

inline bool IsValidAiActionTriggerConditionType(AiActionTriggerConditionType value) noexcept
{
	return (value >= AiActionTriggerConditionType::NONE && value < AiActionTriggerConditionType::MAX);
}

inline bool IsValidAiActionTriggerConditionMethod(AiActionTriggerConditionMethod value) noexcept
{
	return (value >= AiActionTriggerConditionMethod::NONE && value < AiActionTriggerConditionMethod::MAX);
}

inline bool IsValidAiTargetRenewalConditionType(AiTargetRenewalConditionType value) noexcept
{
	return (value >= AiTargetRenewalConditionType::NONE && value < AiTargetRenewalConditionType::MAX);
}

inline bool IsValidAiTargetRenewalConditionMethod(AiTargetRenewalConditionMethod value) noexcept
{
	return (value >= AiTargetRenewalConditionMethod::NONE && value < AiTargetRenewalConditionMethod::MAX);
}

inline bool IsValidAggroPenaltyConditionType(AggroPenaltyConditionType value) noexcept
{
	return (value >= AggroPenaltyConditionType::NONE && value < AggroPenaltyConditionType::MAX);
}


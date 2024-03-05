#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedProjectileEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 발사체 스폰 타입
enum class ProjectileSpawnType : uint8
{

	// 캐스터 인접 위치 스폰
	CASTER = 0,

	// 타겟 위치 스폰
	TARGET = 1,

	// 스킬 영역 안의 랜덤 위치 스폰
	RANDOM = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 발사체 이동 타입
enum class ProjectileMoveType : uint8
{

	// 이동하지 않는 발사체
	FIXED = 0,

	// 직선/등속 운동하는 발사체
	MOVE_LINE = 1,

	// 타겟 지점으로 이동
	MOVE_TARGET = 2,

	// 폭발형 발사체
	EXPLOSION = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 발사체 트리거
enum class ProjectileTriggerType : uint8
{

	// 없음
	NONE = 0,

	// Action 기능 사용 + 효과가 발동되는 시점에 LifeTime의 잔여 시간과는 상관없이 해당 프로젝타일 제거
	EFFECT_END_POINT_ONLY = 1,

	// Action 기능 사용 + 프로젝타일이 LifeTime이 끝나서 사라지는 시점에 Tick Interval과는 상관없이 ActionInfo에 꼽힌 스킬노티파이 한번 더 발동
	EFFECT_END_POINT_DES = 2,

	MAX = 3,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ProjectileSpawnType ProjectileSpawnTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("CASTER"), ESearchCase::IgnoreCase) == 0)	return ProjectileSpawnType::CASTER;
	if (str.Compare(TEXT("TARGET"), ESearchCase::IgnoreCase) == 0)	return ProjectileSpawnType::TARGET;
	if (str.Compare(TEXT("RANDOM"), ESearchCase::IgnoreCase) == 0)	return ProjectileSpawnType::RANDOM;
	return ProjectileSpawnType::MAX;
}
inline ProjectileMoveType ProjectileMoveTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("FIXED"), ESearchCase::IgnoreCase) == 0)	return ProjectileMoveType::FIXED;
	if (str.Compare(TEXT("MOVE_LINE"), ESearchCase::IgnoreCase) == 0)	return ProjectileMoveType::MOVE_LINE;
	if (str.Compare(TEXT("MOVE_TARGET"), ESearchCase::IgnoreCase) == 0)	return ProjectileMoveType::MOVE_TARGET;
	if (str.Compare(TEXT("EXPLOSION"), ESearchCase::IgnoreCase) == 0)	return ProjectileMoveType::EXPLOSION;
	return ProjectileMoveType::MAX;
}
inline ProjectileTriggerType ProjectileTriggerTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return ProjectileTriggerType::NONE;
	if (str.Compare(TEXT("EFFECT_END_POINT_ONLY"), ESearchCase::IgnoreCase) == 0)	return ProjectileTriggerType::EFFECT_END_POINT_ONLY;
	if (str.Compare(TEXT("EFFECT_END_POINT_DES"), ESearchCase::IgnoreCase) == 0)	return ProjectileTriggerType::EFFECT_END_POINT_DES;
	return ProjectileTriggerType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString ProjectileSpawnTypeEnumToString(ProjectileSpawnType value)
{
	switch (value)
	{
		case ProjectileSpawnType::CASTER: return TEXT("CASTER");
		case ProjectileSpawnType::TARGET: return TEXT("TARGET");
		case ProjectileSpawnType::RANDOM: return TEXT("RANDOM");
	}
	return FString();
}
inline FString ProjectileMoveTypeEnumToString(ProjectileMoveType value)
{
	switch (value)
	{
		case ProjectileMoveType::FIXED: return TEXT("FIXED");
		case ProjectileMoveType::MOVE_LINE: return TEXT("MOVE_LINE");
		case ProjectileMoveType::MOVE_TARGET: return TEXT("MOVE_TARGET");
		case ProjectileMoveType::EXPLOSION: return TEXT("EXPLOSION");
	}
	return FString();
}
inline FString ProjectileTriggerTypeEnumToString(ProjectileTriggerType value)
{
	switch (value)
	{
		case ProjectileTriggerType::NONE: return TEXT("NONE");
		case ProjectileTriggerType::EFFECT_END_POINT_ONLY: return TEXT("EFFECT_END_POINT_ONLY");
		case ProjectileTriggerType::EFFECT_END_POINT_DES: return TEXT("EFFECT_END_POINT_DES");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString ProjectileSpawnTypeEnumToDisplayKey(ProjectileSpawnType value)
{
	switch (value)
	{
		case ProjectileSpawnType::CASTER: return TEXT("");
		case ProjectileSpawnType::TARGET: return TEXT("");
		case ProjectileSpawnType::RANDOM: return TEXT("");
	}
	return FString();
}
inline FString ProjectileMoveTypeEnumToDisplayKey(ProjectileMoveType value)
{
	switch (value)
	{
		case ProjectileMoveType::FIXED: return TEXT("");
		case ProjectileMoveType::MOVE_LINE: return TEXT("");
		case ProjectileMoveType::MOVE_TARGET: return TEXT("");
		case ProjectileMoveType::EXPLOSION: return TEXT("");
	}
	return FString();
}
inline FString ProjectileTriggerTypeEnumToDisplayKey(ProjectileTriggerType value)
{
	switch (value)
	{
		case ProjectileTriggerType::NONE: return TEXT("");
		case ProjectileTriggerType::EFFECT_END_POINT_ONLY: return TEXT("");
		case ProjectileTriggerType::EFFECT_END_POINT_DES: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidProjectileSpawnType(ProjectileSpawnType value) noexcept
{
	return (value >= ProjectileSpawnType::CASTER && value < ProjectileSpawnType::MAX);
}

inline bool IsValidProjectileMoveType(ProjectileMoveType value) noexcept
{
	return (value >= ProjectileMoveType::FIXED && value < ProjectileMoveType::MAX);
}

inline bool IsValidProjectileTriggerType(ProjectileTriggerType value) noexcept
{
	return (value >= ProjectileTriggerType::NONE && value < ProjectileTriggerType::MAX);
}


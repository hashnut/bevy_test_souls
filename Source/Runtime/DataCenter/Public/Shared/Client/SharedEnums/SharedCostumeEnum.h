#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedCostumeEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 코스튬 등급
enum class CostumeGrade : uint8
{

	// 에러값
	NONE = 0,

	// 등급 - NORMAL
	NORMAL = 1,

	// 등급 - MAGIC
	MAGIC = 2,

	// 등급 - RARE
	RARE = 3,

	// 등급 - EPIC
	EPIC = 4,

	// 등급 - UNIQUE
	UNIQUE = 5,

	// 등급 - LEGEND
	LEGEND = 6,

	MAX = 7,
};

UENUM()
// 코스튬 획득 Reason
enum class CostumeObtainReason : uint8
{

	// 에러값
	NONE = 0,

	// 소환 아이템 사용으로 인한 획득
	USE_ITEM = 1,

	// 합성으로 인한 획득
	COMPOSE = 2,

	// 천장으로 인한 획득
	CEILING = 3,

	// 교환으로 인한 획득
	EXCHANGE = 4,

	MAX = 5,
};

UENUM(BlueprintType)
// 코스튬 합성 결과
enum class CostumeComposeResult : uint8
{

	// 에러값
	NONE = 0,

	// 합성 성공
	SUCCEEDED = 1,

	// 합성 실패
	FAILED = 2,

	MAX = 3,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline CostumeGrade CostumeGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CostumeGrade::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return CostumeGrade::NORMAL;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return CostumeGrade::MAGIC;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return CostumeGrade::RARE;
	if (str.Compare(TEXT("EPIC"), ESearchCase::IgnoreCase) == 0)	return CostumeGrade::EPIC;
	if (str.Compare(TEXT("UNIQUE"), ESearchCase::IgnoreCase) == 0)	return CostumeGrade::UNIQUE;
	if (str.Compare(TEXT("LEGEND"), ESearchCase::IgnoreCase) == 0)	return CostumeGrade::LEGEND;
	return CostumeGrade::MAX;
}
inline CostumeObtainReason CostumeObtainReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CostumeObtainReason::NONE;
	if (str.Compare(TEXT("USE_ITEM"), ESearchCase::IgnoreCase) == 0)	return CostumeObtainReason::USE_ITEM;
	if (str.Compare(TEXT("COMPOSE"), ESearchCase::IgnoreCase) == 0)	return CostumeObtainReason::COMPOSE;
	if (str.Compare(TEXT("CEILING"), ESearchCase::IgnoreCase) == 0)	return CostumeObtainReason::CEILING;
	if (str.Compare(TEXT("EXCHANGE"), ESearchCase::IgnoreCase) == 0)	return CostumeObtainReason::EXCHANGE;
	return CostumeObtainReason::MAX;
}
inline CostumeComposeResult CostumeComposeResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return CostumeComposeResult::NONE;
	if (str.Compare(TEXT("SUCCEEDED"), ESearchCase::IgnoreCase) == 0)	return CostumeComposeResult::SUCCEEDED;
	if (str.Compare(TEXT("FAILED"), ESearchCase::IgnoreCase) == 0)	return CostumeComposeResult::FAILED;
	return CostumeComposeResult::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString CostumeGradeEnumToString(CostumeGrade value)
{
	switch (value)
	{
		case CostumeGrade::NONE: return TEXT("NONE");
		case CostumeGrade::NORMAL: return TEXT("NORMAL");
		case CostumeGrade::MAGIC: return TEXT("MAGIC");
		case CostumeGrade::RARE: return TEXT("RARE");
		case CostumeGrade::EPIC: return TEXT("EPIC");
		case CostumeGrade::UNIQUE: return TEXT("UNIQUE");
		case CostumeGrade::LEGEND: return TEXT("LEGEND");
	}
	return FString();
}
inline FString CostumeObtainReasonEnumToString(CostumeObtainReason value)
{
	switch (value)
	{
		case CostumeObtainReason::NONE: return TEXT("NONE");
		case CostumeObtainReason::USE_ITEM: return TEXT("USE_ITEM");
		case CostumeObtainReason::COMPOSE: return TEXT("COMPOSE");
		case CostumeObtainReason::CEILING: return TEXT("CEILING");
		case CostumeObtainReason::EXCHANGE: return TEXT("EXCHANGE");
	}
	return FString();
}
inline FString CostumeComposeResultEnumToString(CostumeComposeResult value)
{
	switch (value)
	{
		case CostumeComposeResult::NONE: return TEXT("NONE");
		case CostumeComposeResult::SUCCEEDED: return TEXT("SUCCEEDED");
		case CostumeComposeResult::FAILED: return TEXT("FAILED");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString CostumeGradeEnumToDisplayKey(CostumeGrade value)
{
	switch (value)
	{
		case CostumeGrade::NONE: return TEXT("");
		case CostumeGrade::NORMAL: return TEXT("");
		case CostumeGrade::MAGIC: return TEXT("");
		case CostumeGrade::RARE: return TEXT("");
		case CostumeGrade::EPIC: return TEXT("");
		case CostumeGrade::UNIQUE: return TEXT("");
		case CostumeGrade::LEGEND: return TEXT("");
	}
	return FString();
}
inline FString CostumeObtainReasonEnumToDisplayKey(CostumeObtainReason value)
{
	switch (value)
	{
		case CostumeObtainReason::NONE: return TEXT("");
		case CostumeObtainReason::USE_ITEM: return TEXT("");
		case CostumeObtainReason::COMPOSE: return TEXT("");
		case CostumeObtainReason::CEILING: return TEXT("");
		case CostumeObtainReason::EXCHANGE: return TEXT("");
	}
	return FString();
}
inline FString CostumeComposeResultEnumToDisplayKey(CostumeComposeResult value)
{
	switch (value)
	{
		case CostumeComposeResult::NONE: return TEXT("");
		case CostumeComposeResult::SUCCEEDED: return TEXT("");
		case CostumeComposeResult::FAILED: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidCostumeGrade(CostumeGrade value) noexcept
{
	return (value >= CostumeGrade::NONE && value < CostumeGrade::MAX);
}

inline bool IsValidCostumeObtainReason(CostumeObtainReason value) noexcept
{
	return (value >= CostumeObtainReason::NONE && value < CostumeObtainReason::MAX);
}

inline bool IsValidCostumeComposeResult(CostumeComposeResult value) noexcept
{
	return (value >= CostumeComposeResult::NONE && value < CostumeComposeResult::MAX);
}


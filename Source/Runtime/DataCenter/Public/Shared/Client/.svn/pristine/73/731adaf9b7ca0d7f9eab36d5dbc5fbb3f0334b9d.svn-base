#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedFairyEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 페어리 등급
enum class FairyGrade : uint8
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

UENUM(BlueprintType)
// 페어리 성장 결과
enum class FairyEnchantResult : uint8
{

	// 에러값
	NONE = 0,

	// 레벨 상승
	INCREASE = 1,

	// 레벨 유지
	MAINTAIN = 2,

	// 레벨 하락
	DECREASE = 3,

	MAX = 4,
};

UENUM()
// 페어리 획득 Reason
enum class FairyObtainReason : uint8
{

	// 에러값
	NONE = 0,

	// 소환석 아이템 사용으로 인한 획득
	USE_ITEM = 1,

	// 페어리 합성으로 인한 획득
	COMPOSE = 2,

	// 페어리 천장으로 인한 획득
	CEILING = 3,

	MAX = 4,
};

UENUM(BlueprintType)
// 페어리 합성 결과
enum class FairyComposeResult : uint8
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

inline FairyGrade FairyGradeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return FairyGrade::NONE;
	if (str.Compare(TEXT("NORMAL"), ESearchCase::IgnoreCase) == 0)	return FairyGrade::NORMAL;
	if (str.Compare(TEXT("MAGIC"), ESearchCase::IgnoreCase) == 0)	return FairyGrade::MAGIC;
	if (str.Compare(TEXT("RARE"), ESearchCase::IgnoreCase) == 0)	return FairyGrade::RARE;
	if (str.Compare(TEXT("EPIC"), ESearchCase::IgnoreCase) == 0)	return FairyGrade::EPIC;
	if (str.Compare(TEXT("UNIQUE"), ESearchCase::IgnoreCase) == 0)	return FairyGrade::UNIQUE;
	if (str.Compare(TEXT("LEGEND"), ESearchCase::IgnoreCase) == 0)	return FairyGrade::LEGEND;
	return FairyGrade::MAX;
}
inline FairyEnchantResult FairyEnchantResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return FairyEnchantResult::NONE;
	if (str.Compare(TEXT("INCREASE"), ESearchCase::IgnoreCase) == 0)	return FairyEnchantResult::INCREASE;
	if (str.Compare(TEXT("MAINTAIN"), ESearchCase::IgnoreCase) == 0)	return FairyEnchantResult::MAINTAIN;
	if (str.Compare(TEXT("DECREASE"), ESearchCase::IgnoreCase) == 0)	return FairyEnchantResult::DECREASE;
	return FairyEnchantResult::MAX;
}
inline FairyObtainReason FairyObtainReasonStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return FairyObtainReason::NONE;
	if (str.Compare(TEXT("USE_ITEM"), ESearchCase::IgnoreCase) == 0)	return FairyObtainReason::USE_ITEM;
	if (str.Compare(TEXT("COMPOSE"), ESearchCase::IgnoreCase) == 0)	return FairyObtainReason::COMPOSE;
	if (str.Compare(TEXT("CEILING"), ESearchCase::IgnoreCase) == 0)	return FairyObtainReason::CEILING;
	return FairyObtainReason::MAX;
}
inline FairyComposeResult FairyComposeResultStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return FairyComposeResult::NONE;
	if (str.Compare(TEXT("SUCCEEDED"), ESearchCase::IgnoreCase) == 0)	return FairyComposeResult::SUCCEEDED;
	if (str.Compare(TEXT("FAILED"), ESearchCase::IgnoreCase) == 0)	return FairyComposeResult::FAILED;
	return FairyComposeResult::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString FairyGradeEnumToString(FairyGrade value)
{
	switch (value)
	{
		case FairyGrade::NONE: return TEXT("NONE");
		case FairyGrade::NORMAL: return TEXT("NORMAL");
		case FairyGrade::MAGIC: return TEXT("MAGIC");
		case FairyGrade::RARE: return TEXT("RARE");
		case FairyGrade::EPIC: return TEXT("EPIC");
		case FairyGrade::UNIQUE: return TEXT("UNIQUE");
		case FairyGrade::LEGEND: return TEXT("LEGEND");
	}
	return FString();
}
inline FString FairyEnchantResultEnumToString(FairyEnchantResult value)
{
	switch (value)
	{
		case FairyEnchantResult::NONE: return TEXT("NONE");
		case FairyEnchantResult::INCREASE: return TEXT("INCREASE");
		case FairyEnchantResult::MAINTAIN: return TEXT("MAINTAIN");
		case FairyEnchantResult::DECREASE: return TEXT("DECREASE");
	}
	return FString();
}
inline FString FairyObtainReasonEnumToString(FairyObtainReason value)
{
	switch (value)
	{
		case FairyObtainReason::NONE: return TEXT("NONE");
		case FairyObtainReason::USE_ITEM: return TEXT("USE_ITEM");
		case FairyObtainReason::COMPOSE: return TEXT("COMPOSE");
		case FairyObtainReason::CEILING: return TEXT("CEILING");
	}
	return FString();
}
inline FString FairyComposeResultEnumToString(FairyComposeResult value)
{
	switch (value)
	{
		case FairyComposeResult::NONE: return TEXT("NONE");
		case FairyComposeResult::SUCCEEDED: return TEXT("SUCCEEDED");
		case FairyComposeResult::FAILED: return TEXT("FAILED");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString FairyGradeEnumToDisplayKey(FairyGrade value)
{
	switch (value)
	{
		case FairyGrade::NONE: return TEXT("");
		case FairyGrade::NORMAL: return TEXT("");
		case FairyGrade::MAGIC: return TEXT("");
		case FairyGrade::RARE: return TEXT("");
		case FairyGrade::EPIC: return TEXT("");
		case FairyGrade::UNIQUE: return TEXT("");
		case FairyGrade::LEGEND: return TEXT("");
	}
	return FString();
}
inline FString FairyEnchantResultEnumToDisplayKey(FairyEnchantResult value)
{
	switch (value)
	{
		case FairyEnchantResult::NONE: return TEXT("");
		case FairyEnchantResult::INCREASE: return TEXT("");
		case FairyEnchantResult::MAINTAIN: return TEXT("");
		case FairyEnchantResult::DECREASE: return TEXT("");
	}
	return FString();
}
inline FString FairyObtainReasonEnumToDisplayKey(FairyObtainReason value)
{
	switch (value)
	{
		case FairyObtainReason::NONE: return TEXT("");
		case FairyObtainReason::USE_ITEM: return TEXT("");
		case FairyObtainReason::COMPOSE: return TEXT("");
		case FairyObtainReason::CEILING: return TEXT("");
	}
	return FString();
}
inline FString FairyComposeResultEnumToDisplayKey(FairyComposeResult value)
{
	switch (value)
	{
		case FairyComposeResult::NONE: return TEXT("");
		case FairyComposeResult::SUCCEEDED: return TEXT("");
		case FairyComposeResult::FAILED: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidFairyGrade(FairyGrade value) noexcept
{
	return (value >= FairyGrade::NONE && value < FairyGrade::MAX);
}

inline bool IsValidFairyEnchantResult(FairyEnchantResult value) noexcept
{
	return (value >= FairyEnchantResult::NONE && value < FairyEnchantResult::MAX);
}

inline bool IsValidFairyObtainReason(FairyObtainReason value) noexcept
{
	return (value >= FairyObtainReason::NONE && value < FairyObtainReason::MAX);
}

inline bool IsValidFairyComposeResult(FairyComposeResult value) noexcept
{
	return (value >= FairyComposeResult::NONE && value < FairyComposeResult::MAX);
}


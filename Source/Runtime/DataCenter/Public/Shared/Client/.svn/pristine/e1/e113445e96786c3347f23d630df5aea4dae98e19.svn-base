#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedBMShopEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 팝업 조건 타입
enum class BMPopupConditionType : uint8
{

	// 분류 없음. 에러값
	NONE = 0,

	// 레벨업
	CHARACTER_LEVEL = 1,

	// 특정 퀘스트 n회 완료
	QUEST_COMPLETE = 2,

	MAX = 3,
};

UENUM()
// 팝업 상점 상태
enum class BMPopupStatus : uint8
{

	// 기본값
	NONE = 0,

	// 진행 중
	IN_PROGRESS = 1,

	// 완료
	COMPLETED = 2,

	MAX = 3,
};

UENUM(BlueprintType)
// 구매제한 리셋조건
enum class BMResetConditionType : uint8
{

	// 없음
	NONE = 0,

	// 해당 상품의 판매 종료 기간에 도달하면 구매 제한 초기화
	SALE_END = 1,

	MAX = 2,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline BMPopupConditionType BMPopupConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return BMPopupConditionType::NONE;
	if (str.Compare(TEXT("CHARACTER_LEVEL"), ESearchCase::IgnoreCase) == 0)	return BMPopupConditionType::CHARACTER_LEVEL;
	if (str.Compare(TEXT("QUEST_COMPLETE"), ESearchCase::IgnoreCase) == 0)	return BMPopupConditionType::QUEST_COMPLETE;
	return BMPopupConditionType::MAX;
}
inline BMPopupStatus BMPopupStatusStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return BMPopupStatus::NONE;
	if (str.Compare(TEXT("IN_PROGRESS"), ESearchCase::IgnoreCase) == 0)	return BMPopupStatus::IN_PROGRESS;
	if (str.Compare(TEXT("COMPLETED"), ESearchCase::IgnoreCase) == 0)	return BMPopupStatus::COMPLETED;
	return BMPopupStatus::MAX;
}
inline BMResetConditionType BMResetConditionTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NONE"), ESearchCase::IgnoreCase) == 0)	return BMResetConditionType::NONE;
	if (str.Compare(TEXT("SALE_END"), ESearchCase::IgnoreCase) == 0)	return BMResetConditionType::SALE_END;
	return BMResetConditionType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString BMPopupConditionTypeEnumToString(BMPopupConditionType value)
{
	switch (value)
	{
		case BMPopupConditionType::NONE: return TEXT("NONE");
		case BMPopupConditionType::CHARACTER_LEVEL: return TEXT("CHARACTER_LEVEL");
		case BMPopupConditionType::QUEST_COMPLETE: return TEXT("QUEST_COMPLETE");
	}
	return FString();
}
inline FString BMPopupStatusEnumToString(BMPopupStatus value)
{
	switch (value)
	{
		case BMPopupStatus::NONE: return TEXT("NONE");
		case BMPopupStatus::IN_PROGRESS: return TEXT("IN_PROGRESS");
		case BMPopupStatus::COMPLETED: return TEXT("COMPLETED");
	}
	return FString();
}
inline FString BMResetConditionTypeEnumToString(BMResetConditionType value)
{
	switch (value)
	{
		case BMResetConditionType::NONE: return TEXT("NONE");
		case BMResetConditionType::SALE_END: return TEXT("SALE_END");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString BMPopupConditionTypeEnumToDisplayKey(BMPopupConditionType value)
{
	switch (value)
	{
		case BMPopupConditionType::NONE: return TEXT("");
		case BMPopupConditionType::CHARACTER_LEVEL: return TEXT("");
		case BMPopupConditionType::QUEST_COMPLETE: return TEXT("");
	}
	return FString();
}
inline FString BMPopupStatusEnumToDisplayKey(BMPopupStatus value)
{
	switch (value)
	{
		case BMPopupStatus::NONE: return TEXT("");
		case BMPopupStatus::IN_PROGRESS: return TEXT("");
		case BMPopupStatus::COMPLETED: return TEXT("");
	}
	return FString();
}
inline FString BMResetConditionTypeEnumToDisplayKey(BMResetConditionType value)
{
	switch (value)
	{
		case BMResetConditionType::NONE: return TEXT("");
		case BMResetConditionType::SALE_END: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidBMPopupConditionType(BMPopupConditionType value) noexcept
{
	return (value >= BMPopupConditionType::NONE && value < BMPopupConditionType::MAX);
}

inline bool IsValidBMPopupStatus(BMPopupStatus value) noexcept
{
	return (value >= BMPopupStatus::NONE && value < BMPopupStatus::MAX);
}

inline bool IsValidBMResetConditionType(BMResetConditionType value) noexcept
{
	return (value >= BMResetConditionType::NONE && value < BMResetConditionType::MAX);
}


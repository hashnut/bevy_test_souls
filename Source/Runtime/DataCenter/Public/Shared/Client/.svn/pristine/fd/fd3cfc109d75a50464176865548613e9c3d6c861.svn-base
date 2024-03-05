#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do not modify this file because it was generated automatically
// SharedSystemMessageEnumClient.h
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enums
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
// 시스템 메세지 타입
enum class SystemMessageType : uint8
{

	// 시스템 공지
	NOTICE = 0,

	// 고 등급 장비 획득(드랍) 알림
	ITEM_GET_DROP = 1,

	// 고 등급 장비 획득(상자) 알림
	ITEM_GET_BOX = 2,

	// 9강화 이상 고 강화 성공 알림
	ITEM_ENCHANT = 3,

	// 고 등급 페어리 획득 알림
	FAIRY_GET = 4,

	// 고 등급 코스튬 획득 알림
	COSTUME_GET = 5,

	// 고 등급 아이템 획득(필드 보스) 알림
	ITEM_GET_FIELD_BOSS = 6,

	// 파티 상황에서 고 등급 장비 획득(드랍) 알림
	ITEM_GET_DROP_ON_PARTY = 7,

	MAX = 8,
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// String -> Enum
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline SystemMessageType SystemMessageTypeStringToEnum(const FString& str)
{
	if (str.Compare(TEXT("NOTICE"), ESearchCase::IgnoreCase) == 0)	return SystemMessageType::NOTICE;
	if (str.Compare(TEXT("ITEM_GET_DROP"), ESearchCase::IgnoreCase) == 0)	return SystemMessageType::ITEM_GET_DROP;
	if (str.Compare(TEXT("ITEM_GET_BOX"), ESearchCase::IgnoreCase) == 0)	return SystemMessageType::ITEM_GET_BOX;
	if (str.Compare(TEXT("ITEM_ENCHANT"), ESearchCase::IgnoreCase) == 0)	return SystemMessageType::ITEM_ENCHANT;
	if (str.Compare(TEXT("FAIRY_GET"), ESearchCase::IgnoreCase) == 0)	return SystemMessageType::FAIRY_GET;
	if (str.Compare(TEXT("COSTUME_GET"), ESearchCase::IgnoreCase) == 0)	return SystemMessageType::COSTUME_GET;
	if (str.Compare(TEXT("ITEM_GET_FIELD_BOSS"), ESearchCase::IgnoreCase) == 0)	return SystemMessageType::ITEM_GET_FIELD_BOSS;
	if (str.Compare(TEXT("ITEM_GET_DROP_ON_PARTY"), ESearchCase::IgnoreCase) == 0)	return SystemMessageType::ITEM_GET_DROP_ON_PARTY;
	return SystemMessageType::MAX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> String
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString SystemMessageTypeEnumToString(SystemMessageType value)
{
	switch (value)
	{
		case SystemMessageType::NOTICE: return TEXT("NOTICE");
		case SystemMessageType::ITEM_GET_DROP: return TEXT("ITEM_GET_DROP");
		case SystemMessageType::ITEM_GET_BOX: return TEXT("ITEM_GET_BOX");
		case SystemMessageType::ITEM_ENCHANT: return TEXT("ITEM_ENCHANT");
		case SystemMessageType::FAIRY_GET: return TEXT("FAIRY_GET");
		case SystemMessageType::COSTUME_GET: return TEXT("COSTUME_GET");
		case SystemMessageType::ITEM_GET_FIELD_BOSS: return TEXT("ITEM_GET_FIELD_BOSS");
		case SystemMessageType::ITEM_GET_DROP_ON_PARTY: return TEXT("ITEM_GET_DROP_ON_PARTY");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enum -> DisplayKey (클라이언트 로컬라이징 번역 키를 처리하기 위해 활용)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline FString SystemMessageTypeEnumToDisplayKey(SystemMessageType value)
{
	switch (value)
	{
		case SystemMessageType::NOTICE: return TEXT("");
		case SystemMessageType::ITEM_GET_DROP: return TEXT("");
		case SystemMessageType::ITEM_GET_BOX: return TEXT("");
		case SystemMessageType::ITEM_ENCHANT: return TEXT("");
		case SystemMessageType::FAIRY_GET: return TEXT("");
		case SystemMessageType::COSTUME_GET: return TEXT("");
		case SystemMessageType::ITEM_GET_FIELD_BOSS: return TEXT("");
		case SystemMessageType::ITEM_GET_DROP_ON_PARTY: return TEXT("");
	}
	return FString();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Validators
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool IsValidSystemMessageType(SystemMessageType value) noexcept
{
	return (value >= SystemMessageType::NOTICE && value < SystemMessageType::MAX);
}


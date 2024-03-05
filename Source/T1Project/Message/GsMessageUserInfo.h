#pragma once

// bak1210 
// 사용자의 기존정보와 관련된 메시지만 추가한다.
//
UENUM()
enum class MessageUserInfo : uint8
{
	LEVEL,
	EXP,
	LOOK_INFO,
	CURRENCY,			// 재화갱신. (Param: CurrencyType)
	CURRENCY_MAX,
	BLESS_POINT,
	PVP_POINT,
	INCREASE_EXP,
	INCREASE_GOLD,
	INCREASE_MONSTER_BOOK_PT,
};



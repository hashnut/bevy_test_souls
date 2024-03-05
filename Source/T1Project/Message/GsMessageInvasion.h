#pragma once

// bak1210 
// 사용자의 기존정보와 관련된 메시지만 추가한다.
//
UENUM()
enum class MessageInvasion : uint8
{
	INVALID_INVASION_USER_COUNT,

	INVALID_INVASION_BATTLE_RANK,	// 2022/08/24 PKT - 침공 관련 칭호 정보 변경

	INVALID_INVADER_COUNT_VIAPORTAL, 
	INVALID_INVASION_ACTIVEPORTAL,

	INVALID_TOWN_CHAOS_FENCE,
	INVAION_START,
	INVAION_FINISH,
};

using	MsgInvasionHandle = TPair<MessageInvasion, FDelegateHandle>;
using	MsgInvasionHandleArray = TArray<MsgInvasionHandle>;

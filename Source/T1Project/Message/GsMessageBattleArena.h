#pragma once
#include "Classes/GsMessageHandlerOneParam.h"

UENUM()
enum class MessageContentBattleArena : uint8
{
	HUD_SHOW_ENTER_BUTTON,
	HUD_HIDE_ENTER_BUTTON,

	HUD_SHOW_TEAM_SCORE_BOARD,
	HUD_HIDE_TEAM_SCORE_BOARD,

	UPDATE_STATE_INFO,

	ENTER_ARENA,			// 2023/11/22 PKT - Arena 입장
	EXIT_ARENA,				// 2023/11/22 PKT - Arena 퇴장
};

// 공유 선언
using	MsgContentBattleArena = TPair<MessageContentBattleArena, FDelegateHandle>;
using	MsgContentBattleArenaArray = TArray<MsgContentBattleArena>;
using	MBattleArena = TGsMessageHandlerOneParam<MessageContentBattleArena, const struct IGsMessageParam*>;


UENUM()
enum class MessageContentBattleRank : uint8
{
	LOCAL_RANK_DATA,
	RESET_RANK_DATA,
	CREATE_RANK_DATA,
	UPDATE_RANK_DATA,
};

// 공유 선언
using	MsgContentBattleArenaRank = TPair<MessageContentBattleRank, FDelegateHandle>;
using	MsgContentBattleArenaRankArray = TArray<MsgContentBattleArenaRank>;
using	MBattleArenaRank = TGsMessageHandlerOneParam<MessageContentBattleRank, const struct IGsMessageParam*>;
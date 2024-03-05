#pragma once

#include "Classes/GsMessageHandler.h"
#include "Classes/GsMessageHandlerOneParam.h"

UENUM()
enum class MessageBoss : uint8
{
	SHOW_BOSS_SPAWN_REGION,
	SELECT_BOSS,
	OPEN_BOSS_CONTRIBUTION_POPUP,
	CHANGE_CONTRIBUTION,
	CHANGE_AGGRO,
	HIDE_AGRRO,
	HIDE_CONTRIBUTION,		
	HIDE_BOSS_BILLBOARD,
	ADD_FIELD_BOSS_INFO,
	DEL_FIELD_BOSS_INFO,
	INVALID_COMPETITION_BOSS,
	INVALID_BOSS_INFO,		

	//world boss
	INFORM_WORLD_BOSS_OPEN,
	WORLD_BOSS_SPAWN_READY,
	ALARM_WORLD_BOSS_SPAWN,
	WORLD_BOSS_CLEAR,
	ALARM_WORLD_BOSS_TIME_OVER,
	WORLD_BOSS_NOT_RECOMMEND,
};

using	MsgBossHandle		= TPair<MessageBoss, FDelegateHandle>;
using	MsgBossHandleArray	= TArray<MsgBossHandle>;

using	MBoss = TGsMessageHandlerOneParam<MessageBoss, const struct IGsMessageParam*>;
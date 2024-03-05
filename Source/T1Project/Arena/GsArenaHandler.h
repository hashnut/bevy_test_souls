// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Arena/GsArenaDefine.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"


class FGsArenaHandler
{
protected:
	bool _isClickedAccept = false;

public:
	void Initialize();
	void Finalize();

public:
	void Set(PD::SC::PKT_SC_ACK_DUEL_INVITE_READ& InPacket);
	void Set(PD::SC::PKT_SC_ACK_DUEL_INVITE_CANCEL_READ& InPacket);
	void Set(PD::SC::PKT_SC_ACK_DUEL_ACCEPT_READ& InPacket);
	void Set(PD::SC::PKT_SC_NOTIFY_DUEL_INVITE_CANCEL_READ& InPacket);
	void Set(PD::SC::PKT_SC_NOTIFY_DUEL_INVITED_READ& InPacket);
	void Set(PD::SC::PKT_SC_NOTIFY_DUEL_INVITE_RESULT_READ& InPacket);
	void Set(PD::SC::PKT_SC_NOTIFY_DUEL_COUNT_START_READ& InPacket);
	void Set(PD::SC::PKT_SC_NOTIFY_DUEL_START_READ& InPacket);
	void Set(PD::SC::PKT_SC_NOTIFY_DUEL_RESULT_READ& InPacket);
	void SetReconnectInfo(PD::SC::PKT_SC_NOTIFY_DUEL_RECONNECT_INFO_READ& InPacket);

protected:
	bool CheckPacketError(int64 InPacketResult);

public:
	void SetArenaInState(EGsArenaInState InArenaInState);
	void SetArenaOutState(EGsArenaOutState InArenaOutState);

	// 대결 Accept, Request 를 취소시킨다.
	void CancelArenaRequest(bool InShowTicker);

public:
	bool TryReqDuelInvite(UserDBId InUserDBId, Level InLevel, FString InUserName, bool InIsVisible);
	bool TryReqDuelAccept(bool InIsAccept);

public:
	void PlayArenaResult(DuelResult InResult);
	void PlayArenaResultIfLose();

public:
	bool CheckArenaAvailability(bool InShowErrorTicker);

};

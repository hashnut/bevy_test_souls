// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyCheckNetHandshakeComplet.h"
#include "Net/GsNetSendService.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Message/GsMessageUI.h"
#include "Message/GsMessageStage.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Engine/World.h"
#include "TimerManager.h"

FGsStateLobbyCheckNetHandshakeComplet::FGsStateLobbyCheckNetHandshakeComplet() : FGsContentsLobbyBase(FGsContentsMode::InLobby::NETCHECK_HANDSHAKECOMPLETED)
{
}

FGsStateLobbyCheckNetHandshakeComplet::~FGsStateLobbyCheckNetHandshakeComplet()
{
}

void FGsStateLobbyCheckNetHandshakeComplet::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyCheckNetHandshakeComplet::Enter()"));	

	_reTryCount = 10;

	// 메세지 등록
	RegisterMessages();

	if (true == GNet()->IsLobbyHandshakeCompleted())
	{
		GSLOG(Log, TEXT("FGsStateLobbyCheckNetHandshakeComplet::Enter GNet()->IsLobbyHandshakeCompleted() true"));		

		FGsLobbyFlowMsgParam param(MessageStage::COMPLETED_NETCHECK_HANDSHAKE);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
	else
	{
		GSLOG(Log, TEXT("FGsStateLobbyCheckNetHandshakeComplet::Enter GNet()->IsLobbyHandshakeCompleted() false"));

		Blocking();
		// 서버를 골랐으니 서버선택 요청
		GMessage()->GetSystem().SendMessage(MessageSystem::AUTH_SERVER_CONNECTION_START);		

		StartTimer();
	}
}

void FGsStateLobbyCheckNetHandshakeComplet::Exit()
{
	UnBlocking();
	StopTimer();
	UnregisterMessages();

	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyCheckNetHandshakeComplet::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MUI& mUI = messageMgr->GetUI();
	MSystem& mSystem = messageMgr->GetSystem();

	_msgHandleUIList.Emplace(mUI.AddRaw(MessageUI::TITLE_CLICK_START, this, &FGsStateLobbyCheckNetHandshakeComplet::OnClick));

	_msgHandleLobbySystemList.Emplace(mSystem.AddRaw(MessageSystem::NET_AUTH_SERVER_CONNECTION_SUCCEEDED,
		this, &FGsStateLobbyCheckNetHandshakeComplet::NetAuthServerConnectionSucceeded));
}

void FGsStateLobbyCheckNetHandshakeComplet::UnregisterMessages()
{
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		MUI& mUI = messageMgr->GetUI();
		for (MsgUIHandle& handleUI : _msgHandleUIList)
		{
			mUI.Remove(handleUI);
		}	

		MSystem& mSystem = messageMgr->GetSystem();
		for (MsgSystemHandle& handleLobbySystem : _msgHandleLobbySystemList)
		{
			mSystem.Remove(handleLobbySystem);
		}
	}

	_msgHandleLobbySystemList.Empty();
	_msgHandleUIList.Empty();
}

void FGsStateLobbyCheckNetHandshakeComplet::OnClick(const IGsMessageParam* InParam)
{
	// 재시도 Block 상태체크
	if (IsBlocking())
	{
		return;
	}

	GSLOG(Log, TEXT("FGsStateLobbyCheckNetHandshakeComplet::OnClick() call"));

	if (true == GNet()->IsLobbyHandshakeCompleted())
	{
		GSLOG(Log, TEXT("FGsStateLobbyCheckNetHandshakeComplet::OnClick true"));
		
		FGsLobbyFlowMsgParam param(MessageStage::COMPLETED_NETCHECK_HANDSHAKE);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
	else
	{
		GSLOG(Log, TEXT("FGsStateLobbyCheckNetHandshakeComplet::OnClick true"));

		if (_reTryCount > 0)
		{
			Blocking();

			// 서버를 골랐으니 서버선택 요청
			GMessage()->GetSystem().SendMessage(MessageSystem::AUTH_SERVER_CONNECTION_START);

			// 요청 후 터치를 막는다.
			--_reTryCount;
			StartTimer();
		}
		else
		{
			UnBlocking();
			FGsLobbyFlowMsgParam param(MessageStage::FAILED_NETCHECK_HANDSHAKE);
			GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);			
		}
	}
}

void FGsStateLobbyCheckNetHandshakeComplet::StartTimer()
{
	FGsContentsLobbyBase::StartTimer();

	if (!GScope())
		return;

	UWorld* world = GLevel()->GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}

	if (nullptr != world)
	{
		world->GetTimerManager().SetTimer(_timerHandle,
			FTimerDelegate::CreateRaw(this, &FGsStateLobbyCheckNetHandshakeComplet::EndTimer),
			7.0f, false);
	}

	Blocking();
}

void FGsStateLobbyCheckNetHandshakeComplet::EndTimer()
{
	FGsContentsLobbyBase::EndTimer();

	if (!GScope())
		return;

	UWorld* world = GLevel()->GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}

	UnBlocking();
}

void FGsStateLobbyCheckNetHandshakeComplet::StopTimer()
{
	FGsContentsLobbyBase::StopTimer();

	if (!GScope())
		return;

	UWorld* world = GLevel()->GetWorld();
	if (nullptr == world)
		return;

	if (_timerHandle.IsValid())
	{
		world->GetTimerManager().ClearTimer(_timerHandle);
	}

	UnBlocking();
}

void FGsStateLobbyCheckNetHandshakeComplet::NetAuthServerConnectionSucceeded()
{
	GSLOG(Log, TEXT("FGsStateLobbyCheckNetHandshakeComplet::NetAuthServerConnectionSucceeded() true"));
	// 성공 시

	FGsLobbyFlowMsgParam param(MessageStage::COMPLETED_NETCHECK_HANDSHAKE);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);

	UnBlocking();
}
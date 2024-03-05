// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyHiveInit.h"

#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageStage.h"

// test
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Text.h"


FGsStateLobbyHiveInit::FGsStateLobbyHiveInit() : FGsContentsLobbyBase(FGsContentsMode::InLobby::HIVE_INIT)
{
}

FGsStateLobbyHiveInit::~FGsStateLobbyHiveInit()
{
}

void FGsStateLobbyHiveInit::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyHiveInit::Enter()"));

#if FLOW_DUBUG_TEST
	FGsLobbyFlowMsgParam paramTest(MessageStage::SUCCEEDED_HIVE_INIT);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &paramTest);
	return;
#endif

	// 메세지 등록
	RegisterMessages();
	// 하이브 init
	if (true == GHive()->IsInit())
	{
		GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::Enter() true == GHive()->IsSignIn()"));

		FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HIVE_INIT);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
	else
	{
		// hive Init
		HiveInit();
	}	
}

void FGsStateLobbyHiveInit::Exit()
{
	// 메세지 삭제
	UnregisterMessages();
	UnBlocking();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyHiveInit::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MUI& mUI = messageMgr->GetUI();
	MSystemParam& mLobbySystem = messageMgr->GetSystemParam();

	_msgHandleUIList.Emplace(mUI.AddRaw(MessageUI::TITLE_CLICK_START, this, &FGsStateLobbyHiveInit::OnClick));

	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::HIVE_INIT_FALSE,
		this, &FGsStateLobbyHiveInit::HiveInitResultFailed));
	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::HIVE_INIT_TRUE,
		this, &FGsStateLobbyHiveInit::HiveInitResultSucceeded));
}

void FGsStateLobbyHiveInit::UnregisterMessages()
{
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		MUI& mUI = messageMgr->GetUI();
		for (MsgUIHandle& handleUI : _msgHandleUIList)
		{
			mUI.Remove(handleUI);
		}

		MSystemParam& mLobbySystem = messageMgr->GetSystemParam();
		for (MsgSystemHandle& handleLobbySystem : _msgHandleLobbySystemList)
		{
			mLobbySystem.Remove(handleLobbySystem);
		}
	}

	_msgHandleUIList.Empty();
	_msgHandleLobbySystemList.Empty();
}

void FGsStateLobbyHiveInit::HiveInit()
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::HiveInit()"));

	// hive init
	GHive()->Init();
	// 요청 후 터치를 막는다.
	Blocking();
}

void FGsStateLobbyHiveInit::OnClick(const IGsMessageParam* InParam)
{
	// 하이브 요청 후 재요청 Block 상태체크
	if (IsBlocking())
	{
		return;
	}

	if (false == GHive()->IsInit())
	{
		// hive init
		HiveInit();
	}
}

void FGsStateLobbyHiveInit::HiveInitResultFailed(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::HiveInitResultFailed() false"));
	// 실패 시 팝업 공지 하거나 터치 기다림
	UnBlocking();
}

void FGsStateLobbyHiveInit::HiveInitResultSucceeded(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::HiveInitResultSucceeded() true"));
	// 성공 시

	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HIVE_INIT);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}
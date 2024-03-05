// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyDivergence.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsDivergenceManager.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsStageMessageParam.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Text.h"
#include "Management/ScopeGlobal/GsNetManager.h"

#include "Management/ScopeGlobal/GsHiveManager.h"


FGsStateLobbyDivergence::FGsStateLobbyDivergence() : FGsContentsLobbyBase(FGsContentsMode::InLobby::DIVERGENCE)
{
}

FGsStateLobbyDivergence::~FGsStateLobbyDivergence()
{
}

void FGsStateLobbyDivergence::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyDivergence::Enter()"));

#if FLOW_DUBUG_TEST
	GNet()->SetServerIndex(0);

	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HIVE_DIVERGENCE);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);

	return;
#endif

	// 메세지 등록
	RegisterMessages();
	TryDivergence();
}

void FGsStateLobbyDivergence::Exit()
{
	// 메세지 삭제
	UnregisterMessages();
	UnBlocking();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyDivergence::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MUI& mUI = messageMgr->GetUI();
	MSystemParam& mLobbySystem = messageMgr->GetSystemParam();

	_msgHandleUIList.Emplace(mUI.AddRaw(MessageUI::TITLE_CLICK_START, this, &FGsStateLobbyDivergence::OnClick));

	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::HIVE_DIVERGENCE_FALSE,
		this, &FGsStateLobbyDivergence::DivergenceResultFailed));
	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::HIVE_DIVERGENCE_TRUE,
		this, &FGsStateLobbyDivergence::DivergenceResultSucceeded));
}

void FGsStateLobbyDivergence::TryDivergence()
{
	GDivergence()->TryLoadDivergenceFile();
	// 요청 후 터치를 막는다.
	Blocking();
}

void FGsStateLobbyDivergence::UnregisterMessages()
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

void FGsStateLobbyDivergence::OnClick(const IGsMessageParam* InParam)
{
	// 하이브 요청 Block 상태체크
	if (IsBlocking())
	{
		return;
	}

	TryDivergence();
}

// Divergence 요청 실패
void FGsStateLobbyDivergence::DivergenceResultFailed(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyDivergence::DivergenceResultFailed() call"));

	// 실패 시 팝업 공지 하거나 터치 기다림
	UnBlocking();

	FText errorText;
	FText::FindText(TEXT("LauncherText"), TEXT("DivergenceResultFailed"), errorText);
	FGsUIHelper::PopupNetError(errorText);
}

// Divergence 요청 성공
void FGsStateLobbyDivergence::DivergenceResultSucceeded(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyDivergence::DivergenceResultSucceeded() call"));

#if !WITH_EDITOR
	GHive()->SetConfig();
#endif

	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HIVE_DIVERGENCE);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}
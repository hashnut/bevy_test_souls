// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyHiveCheckMaintenance.h"

#include "Net/GsNetSendService.h"

#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsPatchManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/GsMessageStage.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsStageMessageParam.h"

#include "Text.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupDownload.h"

#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"


FGsStateLobbyHiveCheckMaintenance::FGsStateLobbyHiveCheckMaintenance() : FGsContentsLobbyBase(FGsContentsMode::InLobby::HIVECHECK_MAINTENANCE)
{
}

FGsStateLobbyHiveCheckMaintenance::~FGsStateLobbyHiveCheckMaintenance()
{
}

void FGsStateLobbyHiveCheckMaintenance::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyHiveCheckMaintenance::Enter()"));	

#if FLOW_DUBUG_TEST
	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_CHECK_MAINTENANCE);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	return;
#endif

	// 메세지 등록
	RegisterMessages();

	// 요청 후 터치를 막는다.
	Blocking();	

	if (false == GHive()->IsInit())
	{
		FGsLobbyFlowMsgParam param(MessageStage::RETRY_HIVE_INIT);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
	else if (false == GHive()->IsSignIn())
	{
		FGsLobbyFlowMsgParam param(MessageStage::RETRY_HIVE_AUTH_SINGNIN);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
	else
	{
		GHive()->CheckMaintenance();
	}
}

void FGsStateLobbyHiveCheckMaintenance::Exit()
{
	// 메세지 삭제
	UnregisterMessages();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyHiveCheckMaintenance::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MUI& mUI = messageMgr->GetUI();
	MSystem& mSystem = messageMgr->GetSystem();

	_msgHandleUIList.Emplace(mUI.AddRaw(MessageUI::TITLE_CLICK_START, this, &FGsStateLobbyHiveCheckMaintenance::OnClick));

	_msgHandleSystemList.Emplace(mSystem.AddRaw(MessageSystem::HIVE_CHECK_MAINTENANCE_TRUE,
		this, &FGsStateLobbyHiveCheckMaintenance::HiveCheckMaintenanceResultSucceeded));
	_msgHandleSystemList.Emplace(mSystem.AddRaw(MessageSystem::HIVE_CHECK_MAINTENANCE_FALSE,
		this, &FGsStateLobbyHiveCheckMaintenance::HiveCheckMaintenanceResultFailed));
}

void FGsStateLobbyHiveCheckMaintenance::UnregisterMessages()
{
	if (FGsMessageHolder* messageMgr = GMessage())
	{	
		MUI& mUI = messageMgr->GetUI();
		for (MsgUIHandle& handleUI : _msgHandleUIList)
		{
			mUI.Remove(handleUI);
		}

		MSystem& mSystem = messageMgr->GetSystem();
		for (MsgSystemHandle& handleSystem : _msgHandleSystemList)
		{
			mSystem.Remove(handleSystem);
		}		
	}

	_msgHandleUIList.Empty();
	_msgHandleSystemList.Empty();
}

void FGsStateLobbyHiveCheckMaintenance::OnClick(const IGsMessageParam* InParam)
{
	if (IsBlocking())
	{
		return;
	}

	GSLOG(Log, TEXT("FGsStateLobbyHiveCheckMaintenance::OnClick() GHive()->CheckMaintenance() call"));

	// 요청 후 터치를 막는다.
	Blocking();
	GHive()->CheckMaintenance();
}

// 하이브 HiveCheckMaintenance 요청 실패
void FGsStateLobbyHiveCheckMaintenance::HiveCheckMaintenanceResultFailed()
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveCheckMaintenance::HiveCheckMaintenanceResultFailed() false"));
	
	// 실패 시 팝업 공지 하거나 터치 기다림
	UnBlocking();
}

// 하이브 HiveCheckMaintenance 요청 성공
void FGsStateLobbyHiveCheckMaintenance::HiveCheckMaintenanceResultSucceeded()
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveCheckMaintenance::HiveCheckMaintenanceResultSucceeded() true"));
	UnBlocking();

	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_CHECK_MAINTENANCE);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}
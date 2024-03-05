// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyHiveCheckAuthMaintenance.h"

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


FGsStateLobbyHiveCheckAuthMaintenance::FGsStateLobbyHiveCheckAuthMaintenance() : FGsContentsLobbyBase(FGsContentsMode::InLobby::HIVECHECK_AUTH_MAINTENANCE)
{
}

FGsStateLobbyHiveCheckAuthMaintenance::~FGsStateLobbyHiveCheckAuthMaintenance()
{
}

void FGsStateLobbyHiveCheckAuthMaintenance::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyHiveCheckMaintenance::Enter()"));	

#if FLOW_DUBUG_TEST
	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_CHECK_AUTH_MAINTENANCE);
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

void FGsStateLobbyHiveCheckAuthMaintenance::Exit()
{
	// 메세지 삭제
	UnregisterMessages();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyHiveCheckAuthMaintenance::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MUI& mUI = messageMgr->GetUI();
	MSystem& mSystem = messageMgr->GetSystem();

	_msgHandleUIList.Emplace(mUI.AddRaw(MessageUI::TITLE_CLICK_START, this, &FGsStateLobbyHiveCheckAuthMaintenance::OnClick));

	_msgHandleSystemList.Emplace(mSystem.AddRaw(MessageSystem::HIVE_CHECK_MAINTENANCE_TRUE,
		this, &FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultSucceeded));
	_msgHandleSystemList.Emplace(mSystem.AddRaw(MessageSystem::HIVE_CHECK_MAINTENANCE_FALSE,
		this, &FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultFailed));
	_msgHandleSystemList.Emplace(mSystem.AddRaw(MessageSystem::HIVE_CHECK_MAINTENANCE_RETRY,
		this, &FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultRetry));
	_msgHandleSystemList.Emplace(mSystem.AddRaw(MessageSystem::HIVE_CHECK_MAINTENANCE_RETRY_FAILED,
		this, &FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultRetryFailed));
}

void FGsStateLobbyHiveCheckAuthMaintenance::UnregisterMessages()
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

void FGsStateLobbyHiveCheckAuthMaintenance::OnClick(const IGsMessageParam* InParam)
{
	if (IsBlocking())
	{
		return;
	}

	GSLOG(Log, TEXT("FGsStateLobbyHiveCheckAuthMaintenance::OnClick() GHive()->CheckMaintenance() call"));

	// 요청 후 터치를 막는다.
	Blocking();
	GHive()->CheckMaintenance();
}

// 하이브 HiveCheckMaintenance 요청 실패
void FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultFailed()
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultFailed() false"));
	
	// 실패 시 팝업 공지 하거나 터치 기다림
	UnBlocking();
}

// 하이브 HiveCheckMaintenance 요청 성공
void FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultSucceeded()
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultSucceeded() true"));
	UnBlocking();

	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_CHECK_AUTH_MAINTENANCE);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}

void FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultRetry()
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultRetry()"));
	if (UGsHiveManager* hiveMgr = GHive())
	{
		Blocking();		
		hiveMgr->CheckMaintenance(true);
	}
}

void FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultRetryFailed()
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveCheckAuthMaintenance::HiveCheckMaintenanceResultRetryFailed()"));

	FText findText;
	FText::FindText(TEXT("launcherNetText"), TEXT("MAINTENANCE_NETWORK_CONNECT_ERROR"), findText);
	FGsUIHelper::PopupSystemMsg(findText, []()
	{
		if (UGsHiveManager* hiveMgr = GHive())
		{
			hiveMgr->ExitGame();
		}
	});
}
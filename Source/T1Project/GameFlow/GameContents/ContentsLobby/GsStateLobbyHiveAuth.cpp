// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyHiveAuth.h"

#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsDivergenceManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageStage.h"

#include "Text.h"
#include "UI/UIContent/Helper/GsUIHelper.h"



FGsStateLobbyHiveAuth::FGsStateLobbyHiveAuth() : FGsContentsLobbyBase(FGsContentsMode::InLobby::HIVE_AUTH)
{
}

FGsStateLobbyHiveAuth::~FGsStateLobbyHiveAuth()
{
}

void FGsStateLobbyHiveAuth::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::Enter()"));

#if FLOW_DUBUG_TEST
	FGsLobbyFlowMsgParam paramTest(MessageStage::SUCCEEDED_HIVE_AUTH);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &paramTest);
	return;
#endif

	// 메세지 등록
	RegisterMessages();	

	if (false == GHive()->IsInit())
	{
		FGsLobbyFlowMsgParam param(MessageStage::RETRY_HIVE_INIT);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
	else
	{
		if (GDivergence())
		{
			GHive()->SetServerIdByUserSelectServer(GDivergence()->GetRegionServerId());
		}
		

		if (true == GHive()->IsSignIn())
		{
			GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::Enter() true == GHive()->IsSignIn()"));			

			FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HIVE_AUTH);
			GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
		}
		else
		{
			// hive SignIn
			HiveSignIn();
		}
	}
}

void FGsStateLobbyHiveAuth::Exit()
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::Exit()"));

	// 메세지 삭제
	UnregisterMessages();
	UnBlocking();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyHiveAuth::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MUI& mUI = messageMgr->GetUI();
	MSystemParam& mLobbySystem = messageMgr->GetSystemParam();

	_msgHandleUIList.Emplace(mUI.AddRaw(MessageUI::TITLE_CLICK_START, this, &FGsStateLobbyHiveAuth::OnClick));

	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::HIVE_AUTH_FALSE, 
		this, &FGsStateLobbyHiveAuth::HiveAuthResultFailed));
	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::HIVE_AUTH_TRUE, 
		this, &FGsStateLobbyHiveAuth::HiveAuthResultSucceeded));

	/*_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::HIVE_Show_SignIn,
		this, &FGsStateLobbyHiveAuth::HIVEShowSignIn));*/
}

void FGsStateLobbyHiveAuth::UnregisterMessages()
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

void FGsStateLobbyHiveAuth::HiveSignIn()
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::HiveSignIn()"));

	// hive SignIn
	GHive()->SignInHelper();
	// 요청 후 터치를 막는다.
	Blocking();
}

void FGsStateLobbyHiveAuth::OnClick(const IGsMessageParam* InParam)
{
	// 하이브 요청 Block 상태체크
	if (IsBlocking())
	{
		return;
	}

	if (false == GHive()->IsInit())
	{
		FGsLobbyFlowMsgParam param(MessageStage::RETRY_HIVE_INIT);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);		
	}
	else if (false == GHive()->IsSignIn())
	{
		// hive SignIn
		HiveSignIn();
	}
}

// 하이브 SignIn 요청 실패
void FGsStateLobbyHiveAuth::HiveAuthResultFailed(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::HiveAuthResultFailed() false"));
	// 실패 시 팝업 공지 하거나 터치 기다림
	UnBlocking();
}

void FGsStateLobbyHiveAuth::HIVEShowSignIn(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::HIVEShowSignIn()"));
	if (UGsHiveManager* hiveMgr = GHive())
	{
		hiveMgr->ShowSignIn();
	}
}

// 하이브 SignIn 요청 성공
void FGsStateLobbyHiveAuth::HiveAuthResultSucceeded(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::HiveAuthResultSucceeded() true"));
	// 성공 시

	// 무조건 로그인 이후 실행해야한다고함
	UGsHiveManager* hiveMgr = GHive();
	if (nullptr != hiveMgr && hiveMgr->IsSignIn())
	{
#if PLATFORM_IOS || PLATFORM_ANDROID
		hiveMgr->SetEngagementHandlerCB();
#else
		hiveMgr->SetWindowsEngagementHandlerCB();
#endif
		hiveMgr->SetEngagementReady(true);
		
		GSLOG(Log, TEXT("FGsStateLobbyHiveAuth::HiveAuthResultSucceeded() PushGetRemote"));
		// hiveMgr->PushGetRemote();
		// 푸시관련 상태값 불러오기
		hiveMgr->PushGetRemote();
	}

	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HIVE_AUTH);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}
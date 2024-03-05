// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyHerculesAuth.h"

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


FGsStateLobbyHerculesAuth::FGsStateLobbyHerculesAuth() : FGsContentsLobbyBase(FGsContentsMode::InLobby::HERCULES_AUTH)
{
}

FGsStateLobbyHerculesAuth::~FGsStateLobbyHerculesAuth()
{
}

void FGsStateLobbyHerculesAuth::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyHerculesAuth::Enter()"));

#if FLOW_DUBUG_TEST
	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HERCULES_AUTH);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	return;
#endif

	// 메세지 등록
	RegisterMessages();

	HerculesAuth();
}

void FGsStateLobbyHerculesAuth::Exit()
{
	// 메세지 삭제
	UnregisterMessages();
	UnBlocking();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyHerculesAuth::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MSystemParam& mLobbySystem = messageMgr->GetSystemParam();

	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::HERCULES_AUTH_FAILED,
		this, &FGsStateLobbyHerculesAuth::HerculesAuthResultFailed));
	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::HERCULES_AUTH_SUCCEEDED,
		this, &FGsStateLobbyHerculesAuth::HerculesAuthResultSucceeded));
}

void FGsStateLobbyHerculesAuth::UnregisterMessages()
{
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		MSystemParam& mLobbySystem = messageMgr->GetSystemParam();
		for (MsgSystemHandle& handleLobbySystem : _msgHandleLobbySystemList)
		{
			mLobbySystem.Remove(handleLobbySystem);
		}
	}

	_msgHandleLobbySystemList.Empty();
}

void CallBackHerculesResult()
{
#if USE_HERCULES
	GSLOG(Log, TEXT("FGsStateLobbyHerculesAuth::HerculesAuth() Callback"));
	CHerculesPlugin::SetOpenAuthSuccess();
#endif

	GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HERCULES_AUTH_SUCCEEDED, nullptr);
}

void FGsStateLobbyHerculesAuth::HerculesAuth()
{
	GSLOG(Log, TEXT("FGsStateLobbyHerculesAuth::HerculesAuth()"));

#if USE_HERCULES_AUTH
	//// 요청 후 터치를 막는다.
	//Blocking();

	//if (HerculesOpenAuth(3, CallBackHerculesResult) != 0)
	//{

	//}
	//else
	//{
	//	GMessage()->GetSystemParam().SendMessage(MessageSystem::HERCULES_AUTH_FAILED, nullptr);
	//}

#if USE_HERCULES
	if (GHive()->IsInitializedHercules())
	{
		CHerculesPlugin::OpenAuth(3, CallBackHerculesResult);
	}
	else
	{
		GMessage()->GetSystemParam().SendMessage(MessageSystem::HERCULES_AUTH_FAILED, nullptr);
	}
#else
	CallBackHerculesResult();
#endif

#else

	GMessage()->GetSystemParam().SendMessage(MessageSystem::HERCULES_AUTH_SUCCEEDED, nullptr);
#endif	
	
}

void FGsStateLobbyHerculesAuth::HerculesAuthResultFailed(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyHerculesAuth::HerculesAuthResultFailed() false"));
	// 실패 시 팝업 공지 하거나 터치 기다림
	//UnBlocking();

	FGsLobbyFlowMsgParam param(MessageStage::FAILED_HERCULES_AUTH);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}

void FGsStateLobbyHerculesAuth::HerculesAuthResultSucceeded(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyHerculesAuth::HerculesAuthResultSucceeded() true"));
	// 성공 시	
	//UnBlocking();

	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HERCULES_AUTH);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}
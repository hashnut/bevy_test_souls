// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyOfflinePlayBackGround.h"
#include "Message/GsMessageStage.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGlobal.h"
#include "Management/ScopeGlobal/GsOfflinePlayManager.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#if PLATFORM_IOS
#include "Runtime/ApplicationCore/Public/IOS/IOSAppDelegate.h"
#endif // PLATFORM_IOS
#include "Message/MessageParam/GsStageMessageParam.h"

FGsStateLobbyOfflinePlayBackGround::FGsStateLobbyOfflinePlayBackGround() : FGsContentsLobbyBase(FGsContentsMode::InLobby::OFFLINE_PLAY_BACKGROUND)
{
}

FGsStateLobbyOfflinePlayBackGround::~FGsStateLobbyOfflinePlayBackGround()
{
}

void FGsStateLobbyOfflinePlayBackGround::Enter()
{
	FGsContentsLobbyBase::Enter();

	FGsOfflinePlayManager* offlinePlay = GSOfflinePlay();
	if (nullptr != offlinePlay && true == offlinePlay->InOnFlag())
	{
		// 2021/08/25 PKT - 오프라인 플레이 플레그 해제
		offlinePlay->FlagReset();

		TWeakObjectPtr<UGsScopeHolder> scopeHolder = Cast<UGsScopeHolder>(GScope());
		TWeakObjectPtr<UGsScopeGlobal> scopeGlobal = (true == scopeHolder.IsValid())
			? Cast<UGsScopeGlobal>(scopeHolder->GetGlobalScope()) : nullptr;
		if (true == scopeGlobal.IsValid())
		{	// 2021/08/25 PKT - ANDROID : 종료, IOS : 백그라운드
			GSLOG(Log, TEXT("scopeGlobal->QuitGame() after"));
			scopeGlobal->QuitGame();
			GSLOG(Log, TEXT("scopeGlobal->QuitGame() before"));
		}
	}
	else
	{	
		GSLOG(Error, TEXT("-- bad call --"));
		FGsLobbyFlowMsgParam param(MessageStage::ERROR_OFFLINE_PLAY_BACKGROUND);
		GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
}

void FGsStateLobbyOfflinePlayBackGround::Exit()
{
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyOfflinePlayBackGround::Update(float In_deltaTime)
{
	FGsContentsLobbyBase::Update(In_deltaTime);
}

void FGsStateLobbyOfflinePlayBackGround::Init()
{
	FGsContentsLobbyBase::Init();
}

void FGsStateLobbyOfflinePlayBackGround::Close()
{
	FGsContentsLobbyBase::Close();
}
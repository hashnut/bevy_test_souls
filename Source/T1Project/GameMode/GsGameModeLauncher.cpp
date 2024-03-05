// Fill out your copyright notice in the Description page of Project Settings.
#include "GsGameModeLauncher.h"

//#include "GameFrameWork/PlayerController.h"
//#include "GameFramework/Character.h"
//
//#include "Management/ScopeGlobal/GsSoundManager.h"
//
//#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsPatchManager.h"
#include "Management/ScopeGlobal/GsPopupSystemManager.h"
#include "management/GsMessageHolder.h"

//#include "UI/UIContent/Window/GsUIWindowTitle.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
//
//#include "Option/GsOptionManager.h"
//#include "Option/GsGameUserSettings.h"
//#include "Net/GsNetSendService.h"
//#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
//
#include "ControllerEx/GsPlayerController.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../GameFlow/GsGameFlowLobby.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsLobby/GsContentsManagerLobby.h"

// BSAM
#include "Option/GsGameUserSettings.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageStage.h"

//------------------------------------------------------------------
AGsGameModeLauncher::AGsGameModeLauncher()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = AGsPlayerController::StaticClass();
}

//------------------------------------------------------------------
void AGsGameModeLauncher::StartPlay()
{
	Super::StartPlay();	

#ifdef LOBBY_FLOW_UI_HIVE_1ST
#else
	if (nullptr != _scopeHolder)
	{
		_scopeHolder->StartPlayGameModeLobby();
	}
#endif

	if (APlayerController* playerController = GetWorld()->GetFirstPlayerController())
	{
		playerController->bShowMouseCursor = true;
		playerController->ActivateTouchInterface(nullptr);
		
		if (playerController->InputComponent)
		{
			// Back 이벤트 바인딩
			playerController->InputComponent->BindAction(TEXT("Back"), EInputEvent::IE_Released, this,
				&AGsGameModeLauncher::OnBackKey);
		}
	}

	if (UGsUIManager* uiManager = GUI())
	{
		if (UGsLobbyDataManager* lobbyDataMgr = LSLobbyData())
		{
			uiManager->OnLauncherStart(lobbyDataMgr->GetLobbyConstantData());
		}

		uiManager->SetTouchVfxPath();

		uiManager->HideLoading();
		uiManager->SetHUDMode(EGsUIHUDMode::NORMAL, false);
		uiManager->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);
	}

	if (UGsPopupSystemManager* popupSystemMgr = GPopupSystem())
	{
		popupSystemMgr->OnLauncherStart();
	}

	FGsMessageHolder* msg = GMessage();

	if (class FGsGameFlowLobby* flowLobby = GMode()->GetLobbyFlow())
	{
		if (FGsContentsManagerLobby* contents = flowLobby->GetContentsManagerLobby())
		{
			if (FGsContentsMode::InLobby::MAX == contents->GetCurrentContentsMode())
			{
				FGsLobbyFlowMsgParam param(MessageStage::COMPLETED_MODELAUNCHER);
				msg->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
				return;
			}
		}
	}

	if (FGsNetManager* netManager = GNet())
	{		
		int32 id = netManager->GetReturnToStageType();
		if((int32)MessageSystem::BACKTO_LOGIN == id)
		{
			netManager->SetReturnToStageType(-1);
		}

		FGsLobbyFlowMsgParam param(MessageStage::COMPLETED_BACKTO_LAUNCHER);
		msg->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
	}
}

void AGsGameModeLauncher::BeginDestroy()
{	
	Super::BeginDestroy();
}

void AGsGameModeLauncher::OnBackKey()
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->Back();
	}
}

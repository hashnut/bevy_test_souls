// Fill out your copyright notice in the Description page of Project Settings.
#include "GsGameModeLobby.h"

#include "Net/GsNetSendService.h"

#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

#include "ControllerEx/GsPlayerController.h"
#include "GameFrameWork/PlayerController.h"
#include "GameFramework/Character.h"

#include "GameObject/HUD/GsHUDPlayableEvent.h"

#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Option/GsGameUserSettings.h"

#include "Data/GsDataContainManager.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageStage.h"

#include "UI/UIContent/Tray/GsUITrayFadeEffect.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


//------------------------------------------------------------------
AGsGameModeLobby::AGsGameModeLobby()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = AGsPlayerController::StaticClass();

	HUDClass = AGsHUDPlayableEvent::StaticClass();
}

//------------------------------------------------------------------
void AGsGameModeLobby::StartPlay()
{
	Super::StartPlay();

	if (UGsUIManager* uiManager = GUI())
	{

		uiManager->SetHUDMode(EGsUIHUDMode::NORMAL, false);
		uiManager->ClearHideFlags(EGsUIHideFlags::UI_HIDE_ALL);

		/*if (GScope())
		{
			if (UGsDataContainManager* dataContainManager = GData())
			{
				FGsUIHelper::TrayFadeIn(2.0);
			}
			else
			{
				FGsUIHelper::TrayLobbyFadeIn(2.0);
			}
		}*/

		//https://jira.com2us.com/jira/browse/C2URWQ-1685
		// move to FGsStateLobbyCharacterSelect::LoadChangeCharacter last line
		//uiManager->HideLoading();
	}

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
				&AGsGameModeLobby::OnBackKey);
		}
	}

	if (FGsNetManager* netManager = GNet())
	{
		// Send를 막은게 있다면 풀어준다
		netManager->ClearPacketSendBlock();

		FGsMessageHolder* msg = GMessage();
		int32 id = netManager->GetReturnToStageType();
		if ((int32)MessageSystem::BACKTO_LOBBY == id)
		{
			FGsNetSendService::SendGatewayReqUserList();
			return;
		}
	}
}

void AGsGameModeLobby::BeginDestroy()
{
#ifdef LOBBY_FLOW_UI_HIVE_1ST
#else
	//로비모드 시작~
	if (nullptr != _scopeHolder)
	{
		_scopeHolder->BeginDestroyGameModeLobby();
	}
#endif

	Super::BeginDestroy();
}

void AGsGameModeLobby::OnBackKey()
{
	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->Back();
	}
}
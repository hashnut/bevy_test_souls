// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStateLobbyCharacterCustomizing.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/ScopeLobby/GsLobbyCharacterPresetManager.h"

#include "UI/UIContent/Window/GsUIWindowCustomize.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "ActorEx/GsCharacterPlayer.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsCharacterLobbyPlayer.h"

#include "GameObject/Customize/GsCustomizeHandler.h"

#include "Animation/GsAnimInstanceState.h"

#include "DataSchema/GsSchemaEnums.h"

#include "UTIL/GsCameraUtil.h"
#include "UTIL/GsClassUtil.h"

#include "Engine/Engine.h"
#include "Engine/Classes/GameFramework/Character.h"
#include "Engine/Classes/Camera/CameraComponent.h"

#include "GameFlow/Stage/StageLobby/GsStageLobbyCharacterCustomizing.h"
#include "GameFlow/Stage/StageLobby/GsStageManagerLobby.h"
#include "GameFlow/GsGameFlowLobby.h"


FGsStateLobbyCharacterCustomizing::FGsStateLobbyCharacterCustomizing() : FGsContentsLobbyBase(FGsContentsMode::InLobby::CHARACTER_CUSTOMIZING)
{	
}

FGsStateLobbyCharacterCustomizing::~FGsStateLobbyCharacterCustomizing()
{

}

void FGsStateLobbyCharacterCustomizing::Enter()
{
	FGsContentsLobbyBase::Enter();

	_customizeHandler = nullptr;
	_customizeHandler = new FGsCustomizeHandler();

	UGsLobbyCharacterPresetManager* characterPresetMgr = LSLobbyCharacterPreset();
	if (nullptr == characterPresetMgr)
		return;

	// UGsLobbyCharacterManager* characterMgr = LSLobbyCharacter();
	FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow();
	FGsStageManagerLobby* stageManagerLobby = lobbyFlow->GetStageManagerLobby();
	if(auto lobbyStage = stageManagerLobby->GetCurrentState().Pin())
	{
		if (AGsCharacterLobbyPlayer* targetCharacter = lobbyStage->SetCharacter(characterPresetMgr->GetGenderType()))
		{
			// stageCustomizing->ShowCharacter(true);
		//}
			targetCharacter->GetAnim()->ChangeState(static_cast<uint8>(EGsStateBase::Idle), (int)CreatureWeaponType::NONE, false);
			// characterMgr->InitState();

			_customizeHandler->SetActor(targetCharacter);
			_customizeHandler->InitializeScaleDisplacementAllBone(characterPresetMgr->GetGenderType());
			_customizeHandler->SetWeaponChange();

			// init camera fov& position offset
			if (AGsCharacterLocalPlayer* localPlayer = Cast<AGsCharacterLocalPlayer>(targetCharacter))
			{
				if (UCameraComponent* cameraComponent = localPlayer->GetFollowCamera())
				{
					UGsCameraUtil::SetContentsTypeCameraData(EGsContentsCameraType::LobbyCustomizing, cameraComponent);
				}
			}

			//lobbyStage->ReCalcCapsuleSize();
		}
	}

	// UI 열기
	if (UGsUIManager* uiMgr = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("WindowCustomize"));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowCustomize>(widget.Get());
			_windowUI->SetData(characterPresetMgr->GetSelectIndex(), _customizeHandler);
		}
	}
}

void FGsStateLobbyCharacterCustomizing::Exit()
{
	/*UGsLobbyCharacterPresetManager* characterPresetMgr = LSLobbyCharacterPreset();
	if (nullptr != characterPresetMgr)
	{
		characterPresetMgr->OnStartRoamingLuna();
	}*/

	delete _customizeHandler;
	_customizeHandler = nullptr;

	// UI 닫기
	if (_windowUI.IsValid())
	{
		_windowUI->Close();
	}

	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyCharacterCustomizing::Update(float In_deltaTime)
{
	FGsContentsLobbyBase::Update(In_deltaTime);
}


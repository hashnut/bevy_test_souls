// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyCharacterCreate.h"

#include "Engine/World.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraActor.h"
#include "Engine/Classes/Camera/CameraComponent.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "ActorEx/GsActorLobbyObject.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeLobby/GsLobbyCharacterManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeLobby/GsLobbyCharacterPresetManager.h"

#include "UTIL/GsCameraUtil.h"

#include "UI/UIContent/Window/GsUIWindowCharacterCreate.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageSystem.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/GsSchemaEnums.h"

#include "Sequence/GsSequencePlayer.h"
#include "Sequence/GsSequenceManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"


FGsStateLobbyCharacterCreate::FGsStateLobbyCharacterCreate() : FGsContentsLobbyBase(FGsContentsMode::InLobby::CHARACTER_CREATE)
{
}

FGsStateLobbyCharacterCreate::~FGsStateLobbyCharacterCreate()
{
}

void FGsStateLobbyCharacterCreate::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyCharacterCreate::Enter()"));

 	_actorPc = nullptr;

	if (!GEngine || !GEngine->GameViewport)
		return;

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (nullptr == world)
		return;
		
	if (AGsActorLobbyObject* actor = Cast<AGsActorLobbyObject>(UGameplayStatics::GetActorOfClass(world, AGsActorLobbyObject::StaticClass())))
	{
		// 배우
		_actorPc = actor;
		_actorPc->ResetAllEffect();
		_actorPc->SetIsEffectStart(true);
	}
	
	// 다른 카메라를 가져오는 이슈때문에 태그로 찾기
	TArray<AActor*> findActors;
	UGameplayStatics::GetAllActorsWithTag(world, TEXT("LobbyCharacterCreate"), findActors);

	if (findActors.Num() == 0)
	{
		return;
	}
	ACameraActor* cameraActor = Cast<ACameraActor>(findActors[0]);
	if (cameraActor == nullptr)
	{
		return;
	}
	//if (ACameraActor* cameraActor = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(world, ACameraActor::StaticClass())))
	{
		if (APlayerController* controller = world->GetFirstPlayerController())
		{
			controller->SetViewTarget(cameraActor);
		}

		UGsCameraUtil::SetContentsTypeCameraData(EGsContentsCameraType::LobbyCharacterCreate, cameraActor->GetCameraComponent());
	}

	// UI 출력
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->SetHideFlags(EGsUIHideFlags::UI_HIDE_WIDGETCOMPONENT);

		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(FName(TEXT("WindowCharacterCreate")));
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowCharacterCreate>(widget);
			_windowUI->_onClickCustomizing.BindRaw(this, &FGsStateLobbyCharacterCreate::OnClickCustomizing);
			_windowUI->_onSelectPreset.BindRaw(this, &FGsStateLobbyCharacterCreate::OnChangeyCharacter);

			FGsUIHelper::TrayFadeIn(1.5f);
		}
	}
	_selectChIndex = 0;

	GUI()->SetHideFlags(EGsUIHideFlags::UI_HIDE_WIDGETCOMPONENT);

	UGsLobbyCharacterPresetManager* characterPresetMgr = LSLobbyCharacterPreset();
	if (nullptr == characterPresetMgr)
		return;

	characterPresetMgr->LoadCharacters();
	characterPresetMgr->OnStopSequencePlayer();
}

void FGsStateLobbyCharacterCreate::Exit()
{
	// UI 닫기
	if (_windowUI.IsValid())
	{
		_windowUI->Close();
	}

	if (nullptr != _actorPc)
	{
		_actorPc->ResetAllEffect();
		_actorPc->SetIsEffectStart(false);		
	}
	_actorPc = nullptr;

	UGsLobbyCharacterPresetManager* characterPresetMgr = LSLobbyCharacterPreset();
	if (nullptr == characterPresetMgr)
		return;

	characterPresetMgr->SetCharacterUnSelect();
	_selectChIndex = 0;	

	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyCharacterCreate::OnChangeGender(CreatureGenderType inType)
{
	if (nullptr == _actorPc)
		return;

	_actorPc->ChangeActor((CreatureGenderType::MALE == inType) ? true : false);
}

void FGsStateLobbyCharacterCreate::OnClickCustomizing()
{
	if (nullptr != _actorPc)
	{
		_actorPc->ResetAllEffect();
	}
}

void FGsStateLobbyCharacterCreate::OnChangeyCharacter(int inIndex)
{
	UGsLobbyCharacterPresetManager* characterPresetMgr = LSLobbyCharacterPreset();
	if (nullptr == characterPresetMgr)
		return;

	if (_selectChIndex == inIndex)
		return;	

	characterPresetMgr->SetCharacterSelect(_selectChIndex = inIndex, true);
}

void FGsStateLobbyCharacterCreate::OnResetUI(UGsUIManager::FOnUIManagerResetUI& resetHandler)
{
	resetHandler.AddLambda([](int resetType) {
			if (resetType == 1) //connection 단절
			{
				GUI()->SetHideFlags(EGsUIHideFlags::UI_HIDE_WIDGETCOMPONENT);
			}
		});
}

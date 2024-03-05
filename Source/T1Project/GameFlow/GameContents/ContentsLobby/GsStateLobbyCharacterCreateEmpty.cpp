// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyCharacterCreateEmpty.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeLobby/GsLobbyCharacterPresetManager.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageContents.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "UI/UIContent/Tray/GsUITrayMovie.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "PlayableEvent/GsPlayableEventDefine.h"
#include "Kismet/GameplayStatics.h"

#include "ActorEx/GsAreaEnvController.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#if WITH_EDITOR
#include "Cheat/GsCheatManager.h"
#endif


FGsStateLobbyCharacterCreateEmpty::FGsStateLobbyCharacterCreateEmpty() : FGsContentsLobbyBase(FGsContentsMode::InLobby::CHARACTER_CREATE_EMPTY_STATE)
{
}

FGsStateLobbyCharacterCreateEmpty::~FGsStateLobbyCharacterCreateEmpty()
{
}

void FGsStateLobbyCharacterCreateEmpty::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyCharacterCreateEmpty::Enter()"));

	// 메세지 등록
	RegisterMessages();

	if (EGsPlayableEventServerCheckPointType::CharacterEmptyMovieComplete >
		GGameData()->GetPlayableEventCheckPoint())
	{		
#if WITH_EDITOR
		if (UGsCheatManager::IsEffectEmptyPlayer == false)
		{
			GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERCREATE_STAGE);
			return;
		}
#endif
		int sequenceResId = GData()->GetGlobalData()->LobbyCharacterPresetSequenceId;
		if (0 < sequenceResId)
		{
			FGsSequenceMessageParam param(sequenceResId, true, true);
			GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &param);
			
			if (UGsLobbyCharacterPresetManager* characterPresetMgr = LSLobbyCharacterPreset())
			{
				characterPresetMgr->LoadCharacters();
			}
		}		
	}
	else
	{
		GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERCREATE_STAGE);
	}	
}

void FGsStateLobbyCharacterCreateEmpty::Exit()
{
	if (EGsPlayableEventServerCheckPointType::CharacterEmptyMovieComplete > GGameData()->GetPlayableEventCheckPoint())
	{
		GGameData()->SetPlayableEventCheckPoint(EGsPlayableEventServerCheckPointType::CharacterEmptyMovieComplete);
	}

	if (UGsLevelManager* levelMgr = GLevel())
	{
		if (UWorld* world = levelMgr->GetWorld())
		{
			TArray<AActor*> outEnvList;
			UGameplayStatics::GetAllActorsOfClass(world, AGsAreaEnvController::StaticClass(), outEnvList);
			for (AActor* el : outEnvList)
			{
				if (el && el->IsA<AGsAreaEnvController>())
				{
					AGsAreaEnvController* castEnvActor = Cast<AGsAreaEnvController>(el);
					castEnvActor->EndSeparateLightSource();
				}
			}
		}
	}

	UnregisterMessages();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyCharacterCreateEmpty::RegisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		_SequencePlayerStopHandle
			= MessageHolder->GetSystemParam().AddRaw(MessageSystem::SEQUENCE_PLAYER_END
				, this, &FGsStateLobbyCharacterCreateEmpty::OnStopMovie);
	}
}

void FGsStateLobbyCharacterCreateEmpty::UnregisterMessages()
{
	if(!_SequencePlayerStopHandle.Value.IsValid())
	{
		return;
	}

	if (nullptr == GScope())
	{
		return;
	}

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		MessageHolder->GetSystemParam().Remove(_SequencePlayerStopHandle);
		_SequencePlayerStopHandle.Value.Reset();
	}
}

void FGsStateLobbyCharacterCreateEmpty::OnStopMovie(const IGsMessageParam* InParam)
{
	FGsUIHelper::TrayFadeIn(1.5f);
	GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTERCREATE_STAGE);
}
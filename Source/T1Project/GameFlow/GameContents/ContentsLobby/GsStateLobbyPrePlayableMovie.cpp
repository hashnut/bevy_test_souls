// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyPrePlayableMovie.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageContents.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "UI/UIContent/Tray/GsUITrayMovie.h"
#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#if WITH_EDITOR
#include "Cheat/GsCheatManager.h"
#endif


FGsStateLobbyPrePlayableMovie::FGsStateLobbyPrePlayableMovie() : FGsContentsLobbyBase(FGsContentsMode::InLobby::PRE_PLAYABLE_MOVIE)
{
}

FGsStateLobbyPrePlayableMovie::~FGsStateLobbyPrePlayableMovie()
{
}

void FGsStateLobbyPrePlayableMovie::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyPrePlayableMovie::Enter()"));

#if WITH_EDITOR
	if (UGsCheatManager::IsEffectEmptyPlayer == false)
	{
		OnStopMovie(nullptr);
		return;
	}
#endif

	if (EGsPlayableEventServerCheckPointType::PrePlayableMovieComplete > 
		GGameData()->GetPlayableEventCheckPoint())
	{
		FString movieName = GData()->GetGlobalData()->LobbyPrePlayableMovieName;
		if (!movieName.IsEmpty())
		{	
			UGsUITrayMovie::PlayMovie(movieName);
			// 메세지 등록
			RegisterMessages();
		}
		else
		{
			GMessage()->GetStage().SendMessage(MessageStage::ENTER_PLAYABLE_EVENT);
		}
	}
	else
	{
		GMessage()->GetStage().SendMessage(MessageStage::ENTER_PLAYABLE_EVENT);
	}
}

void FGsStateLobbyPrePlayableMovie::Exit()
{
	if (EGsPlayableEventServerCheckPointType::PrePlayableMovieComplete > GGameData()->GetPlayableEventCheckPoint())
	{
		GGameData()->SetPlayableEventCheckPoint(EGsPlayableEventServerCheckPointType::PrePlayableMovieComplete);
	}

	//float fadeInTime = GData()->GetGlobalData()->_playableEventFadeInTime;
	//FGsUIHelper::TrayFadeIn(fadeInTime, false);

	UnregisterMessages();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyPrePlayableMovie::RegisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		_stopMovieHandle = MessageHolder->GetUI().AddRaw(MessageUI::STOP_MOVIE, this, &FGsStateLobbyPrePlayableMovie::OnStopMovie);

		_listSystemDelegate.Emplace(
			MessageHolder->GetSystem().AddRaw(MessageSystem::EXIT_GAME, this, &FGsStateLobbyPrePlayableMovie::OnExitGame));
		_listSystemDelegate.Emplace(
			MessageHolder->GetSystem().AddRaw(MessageSystem::RESTART_GAME, this, &FGsStateLobbyPrePlayableMovie::OnExitGame));
	}
}

void FGsStateLobbyPrePlayableMovie::UnregisterMessages()
{
	if (nullptr == GScope())
	{
		return;
	}

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		if (true == _stopMovieHandle.Value.IsValid())
		{
			MessageHolder->GetUI().Remove(_stopMovieHandle);
			_stopMovieHandle.Value.Reset();
		}

		if (_listSystemDelegate.Num() != 0)
		{
			for (auto iter : _listSystemDelegate)
			{
				MessageHolder->GetSystem().Remove(iter);
			}
			_listSystemDelegate.Empty();
		}
	}
}

void FGsStateLobbyPrePlayableMovie::OnStopMovie(const IGsMessageParam* InParam)
{
	GMessage()->GetStage().SendMessage(MessageStage::ENTER_PLAYABLE_EVENT);
}

void FGsStateLobbyPrePlayableMovie::OnExitGame()
{
	if (nullptr == GScope())
	{
		return;
	}

	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		// exit game -> close movie -> not go playable event
		if (true == _stopMovieHandle.Value.IsValid())
		{
			MessageHolder->GetUI().Remove(_stopMovieHandle);
			_stopMovieHandle.Value.Reset();
		}
	}
}
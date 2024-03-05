// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyPostPlayableMovie.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"

#include "Message/GsMessageSystem.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageContents.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "UI/UIContent/Tray/GsUITrayMovie.h"
#if WITH_EDITOR
#include "Cheat/GsCheatManager.h"
#endif
#include "Management/ScopeGlobal/GsGameDataManager.h"


FGsStateLobbyPostPlayableMovie::FGsStateLobbyPostPlayableMovie() : FGsContentsLobbyBase(FGsContentsMode::InLobby::POST_PLAYABLE_MOVIE)
{
}

FGsStateLobbyPostPlayableMovie::~FGsStateLobbyPostPlayableMovie()
{
}

void FGsStateLobbyPostPlayableMovie::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyPostPlayableMovie::Enter()"));	

#if WITH_EDITOR
	if (UGsCheatManager::IsEffectEmptyPlayer == false)
	{
		OnStopMovie(nullptr);
		return;
	}
#endif
	bool isCorrectServerCheckPoint = EGsPlayableEventServerCheckPointType::PostPlayableMovieComplete >
		GGameData()->GetPlayableEventCheckPoint();

	// 천마대전 2 영상 테스트용
	// 해당 값이 true면 서버 체크포인트 지나가도 틀수있게 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-1921
	bool isPlayableTestEnter = false;
	if (UGsLobbyPlayableEventManager* playableEventManager = LSLobbyPlayableEvent())
	{
		isPlayableTestEnter = playableEventManager->GetAllIsTestEnterVal();
	}

	if (isCorrectServerCheckPoint || isPlayableTestEnter)
	{
		FString movieName = GData()->GetGlobalData()->LobbyPostPlayableMovieName;
		if (!movieName.IsEmpty())
		{
			UGsUITrayMovie::PlayMovie(movieName);

			// 메세지 등록
			RegisterMessages();
		}
		else
		{
			GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTER_EMPTY_STATE);
		}
	}
	else
	{
		GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTER_EMPTY_STATE);
	}
}

void FGsStateLobbyPostPlayableMovie::Exit()
{
	if (EGsPlayableEventServerCheckPointType::PostPlayableMovieComplete > GGameData()->GetPlayableEventCheckPoint())
	{
		GGameData()->SetPlayableEventCheckPoint(EGsPlayableEventServerCheckPointType::PostPlayableMovieComplete);
	}
		
	UnregisterMessages();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyPostPlayableMovie::RegisterMessages()
{
	FGsMessageHolder* MessageHolder = GMessage();
	if (nullptr != MessageHolder)
	{
		_stopMovieHandle = MessageHolder->GetUI().AddRaw(MessageUI::STOP_MOVIE, this, &FGsStateLobbyPostPlayableMovie::OnStopMovie);

		_listSystemDelegate.Emplace(
			MessageHolder->GetSystem().AddRaw(MessageSystem::EXIT_GAME, this, &FGsStateLobbyPostPlayableMovie::OnExitGame));
		_listSystemDelegate.Emplace(
			MessageHolder->GetSystem().AddRaw(MessageSystem::RESTART_GAME, this, &FGsStateLobbyPostPlayableMovie::OnExitGame));
	}
}

void FGsStateLobbyPostPlayableMovie::UnregisterMessages()
{
	if (!_stopMovieHandle.Value.IsValid())
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

void FGsStateLobbyPostPlayableMovie::OnStopMovie(const IGsMessageParam* InParam)
{
	GMessage()->GetStage().SendMessage(MessageStage::ENTER_CHARACTER_EMPTY_STATE);
}

void FGsStateLobbyPostPlayableMovie::OnExitGame()
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
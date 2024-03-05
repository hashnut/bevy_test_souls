#include "GsStageLobbyPlayableEvent.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "GameFlow/Stage/GsStageMode.h"

#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/GameContents/ContentsLobby/GsContentsManagerLobby.h"
#include "GameFlow/GameContents/ContentsLobby/Hud/GsStateLobbyHud.h"

#include "GameFlow/GameContents/ContentsLobby/Hud/PlayableEvent/GsPlayableEventHandler.h"

#include "Sound/GsSoundPlayer.h"

FGsStageLobbyPlayableEvent::FGsStageLobbyPlayableEvent() : FGsStageLobbyBase(FGsStageMode::Lobby::PLAYABLE_EVENT)
{
}

void FGsStageLobbyPlayableEvent::Enter()
{
	Super::Enter();

	if (GMode())
	{
		if (FGsGameFlowLobby* lobbyFlow = GMode()->GetLobbyFlow())
		{
			if (FGsContentsManagerLobby* contents = lobbyFlow->GetContentsManagerLobby())
			{
				FGsStateLobbyHud* hud =
					contents->GetContentsStateObject<FGsStateLobbyHud>(FGsContentsMode::InLobby::HUD);

				if (nullptr == hud)
				{
					return;
				}

				if (FGsPlayableEventHandler* handler = hud->GetPlayableEventHandler())
				{
					handler->StartEvent();
				}
			}
		}
	}

	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->PlaySoundBgm("BGM_Playable");
		_envSoundId = soundPlayer->PlaySoundEnv("ENV_Playable");
	}
}

void FGsStageLobbyPlayableEvent::Exit()
{
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		soundPlayer->StopSoundEnv(_envSoundId);
	}

	Super::Exit();
}

void FGsStageLobbyPlayableEvent::Update(float In_deltaTime)
{
	Super::Update(In_deltaTime);
}
#include "GsStateLobbyHud.h"

#include "GameFlow/GameContents/GsContentsMode.h"

#include "GameFlow/GameContents/ContentsLobby/Hud/PlayableEvent/GsPlayableEventHandler.h"
#include "GameFlow/GameContents/ContentsLobby/Hud/Skill/GsLobbySkillUIHandler.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Data/GsDataContainManager.h"

FGsStateLobbyHud::FGsStateLobbyHud() : FGsContentsLobbyBase(FGsContentsMode::InLobby::HUD)
{
	_playableEventHandler = TUniquePtr<FGsPlayableEventHandler>(new FGsPlayableEventHandler());
	if (_playableEventHandler.IsValid())
	{
		_playableEventHandler->Init();
	}

	_lobbySkillUIHandler = TUniquePtr<FGsLobbySkillUIHandler>(new FGsLobbySkillUIHandler());
	if (_lobbySkillUIHandler.IsValid())
	{
		_lobbySkillUIHandler->Init();
	}
}
FGsStateLobbyHud::~FGsStateLobbyHud()
{
	_playableEventHandler.Reset();
	_lobbySkillUIHandler.Reset();
}
void FGsStateLobbyHud::Close()
{
	if (_playableEventHandler.IsValid())
	{
		_playableEventHandler->Close();
	}

	if (_lobbySkillUIHandler.IsValid())
	{
		_lobbySkillUIHandler->Close();
	}
}

void FGsStateLobbyHud::Update(float In_deltaTime)
{
	if (_playableEventHandler.IsValid())
	{
		_playableEventHandler->Update(In_deltaTime);
	}
}

void FGsStateLobbyHud::Enter()
{
	// hide screen when sub level load finished
}
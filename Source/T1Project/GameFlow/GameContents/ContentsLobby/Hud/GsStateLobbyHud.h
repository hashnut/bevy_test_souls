#pragma once
#include "CoreMinimal.h"

#include "GameFlow/GameContents/ContentsLobby/GsContentsLobbyBase.h"

//-----------------------------------------------------
// Lobby 에서 Hud 컨텐츠를 담당하는 Logic Class
//-----------------------------------------------------

class FGsPlayableEventHandler;
class FGsLobbySkillUIHandler;

class FGsStateLobbyHud : public FGsContentsLobbyBase
{
private:
	TUniquePtr <FGsPlayableEventHandler> _playableEventHandler;
	TUniquePtr <FGsLobbySkillUIHandler> _lobbySkillUIHandler;

	// 생성자, 소멸자
public:
	FGsStateLobbyHud();
	virtual ~FGsStateLobbyHud();

public:
	virtual void Close() override;
	virtual void Update(float In_deltaTime) override;

	virtual void Enter() override;

	// get
public:

	FGsPlayableEventHandler* GetPlayableEventHandler() { return _playableEventHandler.Get(); }
};
#pragma once

#include "GsContentsLobbyBase.h"
#include "../Message/MessageParam/GsMessageParam.h"

class T1PROJECT_API FGsStateLobbyOfflinePlayBackGround : public FGsContentsLobbyBase
{

public:
	FGsStateLobbyOfflinePlayBackGround();
	virtual ~FGsStateLobbyOfflinePlayBackGround();

protected:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

	virtual void Init() override;
	virtual void Close() override;
};
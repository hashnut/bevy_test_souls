// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsContentsLobbyBase.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsMessageParam.h"

/**
 * 천마대전1 무비 플레이
 */
class AActor;
class AGsActorLobbyObject;

class T1PROJECT_API FGsStateLobbyPrePlayableMovie : public FGsContentsLobbyBase
{
public:
	FGsStateLobbyPrePlayableMovie();
	virtual ~FGsStateLobbyPrePlayableMovie();

private:
	MsgUIHandle _stopMovieHandle;
	MsgSystemHandleArray	_listSystemDelegate;
public:
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

private:
	void OnStopMovie(const IGsMessageParam* Param);

	void OnExitGame();
};

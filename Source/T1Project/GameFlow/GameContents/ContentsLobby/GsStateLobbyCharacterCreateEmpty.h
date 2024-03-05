// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsContentsLobbyBase.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsMessageParam.h"

/**
 * 캐릭터 생성 된게 하나도 없을 때 시네마틱 연출
 */
class AActor;
class AGsActorLobbyObject;

class T1PROJECT_API FGsStateLobbyCharacterCreateEmpty : public FGsContentsLobbyBase
{
public:
	FGsStateLobbyCharacterCreateEmpty();
	virtual ~FGsStateLobbyCharacterCreateEmpty();

private:
	MsgSystemHandle _SequencePlayerStopHandle;

public:
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

private:
	void OnStopMovie(const IGsMessageParam* Param);
};

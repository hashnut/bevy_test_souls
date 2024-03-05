// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsContentsLobbyBase.h"
#include "../Message/GsMessageUI.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/GsMessageSystem.h"

struct FTimerHandle;
/**
 *
 */
class T1PROJECT_API FGsStateLobbyHerculesAuth : public FGsContentsLobbyBase
{	
	MsgSystemHandleArray	_msgHandleLobbySystemList;

public:
	FGsStateLobbyHerculesAuth();
	virtual ~FGsStateLobbyHerculesAuth();

public:
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

private:
	void HerculesAuth();
	void HerculesAuthResultFailed(const IGsMessageParam* InParam);
	void HerculesAuthResultSucceeded(const IGsMessageParam* InParam);
};


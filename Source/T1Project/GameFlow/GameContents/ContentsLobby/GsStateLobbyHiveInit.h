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
class T1PROJECT_API FGsStateLobbyHiveInit : public FGsContentsLobbyBase
{	
	MsgUIHandleArray		_msgHandleUIList;
	MsgSystemHandleArray	_msgHandleLobbySystemList;

public:
	FGsStateLobbyHiveInit();
	virtual ~FGsStateLobbyHiveInit();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Update(float In_deltaTime) override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

private:
	void HiveInit();
	void OnClick(const IGsMessageParam* InParam);
	void HiveInitResultFailed(const IGsMessageParam* InParam);
	void HiveInitResultSucceeded(const IGsMessageParam* InParam);
};


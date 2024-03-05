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
class T1PROJECT_API FGsStateLobbyHiveAuth : public FGsContentsLobbyBase
{

protected:
	MsgUIHandleArray		_msgHandleUIList;
	MsgSystemHandleArray	_msgHandleLobbySystemList;

public:
	FGsStateLobbyHiveAuth();
	virtual ~FGsStateLobbyHiveAuth();

public:
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

private:
	void HiveSignIn();
	// UI 터치 이벤트
	void OnClick(const IGsMessageParam* InParam);
	// 하이브 SignIn 요청 실패
	void HiveAuthResultFailed(const IGsMessageParam* InParam);
	// 하이브 SignIn 요청 성공
	void HiveAuthResultSucceeded(const IGsMessageParam* InParam);

	// 하이브 SignIn 재요청
	void HIVEShowSignIn(const IGsMessageParam* InParam);	
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsContentsLobbyBase.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/GsMessageSystem.h"


/**
 *
 */
class T1PROJECT_API FGsStateLobbyLogin : public FGsContentsLobbyBase
{
private:
	MsgSystemHandleArray	_msgHandleLobbySystemList;
	MsgUIHandleArray		_msgHandleUIList;
	TWeakObjectPtr<class UGsUIWindowLogin>	_windowUI;

public:
	FGsStateLobbyLogin();
	virtual ~FGsStateLobbyLogin();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	void OnClickLogin(const FString& inVid);

protected:
	// UI 터치 이벤트
	void OnClick(const IGsMessageParam* InParam);
	void RegisterMessages();
	void UnregisterMessages();

private:
	void OpenUI(const FString& inUserLoginId);
	void StartLogIn();
	void NetLogInSucceeded(const IGsMessageParam* InParam);
	void NetLogInFailed(const IGsMessageParam* InParam);
};


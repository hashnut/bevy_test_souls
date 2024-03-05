// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsContentsLobbyBase.h"
#include "../Message/GsMessageSystem.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/GsMessageUI.h"


/**
 *
 */
class T1PROJECT_API FGsStateLobbyDivergence : public FGsContentsLobbyBase
{
protected:
	MsgUIHandleArray _msgHandleUIList;
	MsgSystemHandleArray _msgHandleLobbySystemList;

public:
	FGsStateLobbyDivergence();
	virtual ~FGsStateLobbyDivergence();

public:
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

private:
	void TryDivergence();
	// UI 터치 이벤트
	void OnClick(const IGsMessageParam* InParam);
	// Divergence 요청 실패
	void DivergenceResultFailed(const IGsMessageParam* InParam);
	// Divergence 요청 성공
	void DivergenceResultSucceeded(const IGsMessageParam* InParam);
};


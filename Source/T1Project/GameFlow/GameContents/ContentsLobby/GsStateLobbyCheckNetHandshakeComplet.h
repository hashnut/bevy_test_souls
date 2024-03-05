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
class T1PROJECT_API FGsStateLobbyCheckNetHandshakeComplet : public FGsContentsLobbyBase
{
	MsgSystemHandleArray	_msgHandleLobbySystemList;
	MsgUIHandleArray		_msgHandleUIList;
	FTimerHandle			_timerHandle;

	int						_reTryCount = 10;

public:
	FGsStateLobbyCheckNetHandshakeComplet();
	virtual ~FGsStateLobbyCheckNetHandshakeComplet();

public:
	virtual void Enter() override;
	virtual void Exit() override;

	// 네트워크 연결 시도 후 타이머 시작
	virtual void StartTimer() override;
	// 타이머 끝이 불리면 다시 클릭할수있는 상태
	virtual void EndTimer() override;
	// 스테이트 나갈 때 타이머 삭제
	virtual void StopTimer() override;

protected:
	// UI 터치 이벤트
	void OnClick(const IGsMessageParam* InParam);
	void RegisterMessages();
	void UnregisterMessages();

private:
	void NetAuthServerConnectionSucceeded();
};
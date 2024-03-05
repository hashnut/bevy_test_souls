// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsContentsLobbyBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "../Message/MessageParam/GsMessageParam.h"

/**
 *
 */
class T1PROJECT_API FGsStateLobbyNetAddressSelect : public FGsContentsLobbyBase
{
private:
	TWeakObjectPtr<class UGsUIWindowNetAddress>	_windowUI;
	FTimerHandle _timerHandle;
	MsgUIHandleArray _msgHandleUIList;

public:
	FGsStateLobbyNetAddressSelect();
	virtual ~FGsStateLobbyNetAddressSelect();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

	// 커넥트 후 타이머 시작
	virtual void StartTimer() override;
	// 타이머 끝이 불리면 다시 타이틀로 이동
	virtual void EndTimer() override;
	// 스테이트 나갈 때 타이머 삭제
	virtual void StopTimer() override;

protected:
	// UI 터치 이벤트
	void OnClick(const IGsMessageParam* InParam);
	void RegisterMessages();
	void UnregisterMessages();

public:
	void OnClickSelectServer(int inIndex);
};

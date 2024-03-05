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
class T1PROJECT_API FGsStateLobbyHiveCheckGateWayMaintenance : public FGsContentsLobbyBase
{

protected:
	MsgUIHandleArray	 _msgHandleUIList;
	MsgSystemHandleArray _msgHandleSystemList;

	bool _isBlock = false;

public:
	FGsStateLobbyHiveCheckGateWayMaintenance();
	virtual ~FGsStateLobbyHiveCheckGateWayMaintenance();

public:
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

private:
	// UI 터치 이벤트
	void OnClick(const IGsMessageParam* InParam);

	// 하이브 CheckMaintenance 요청 실패
	void HiveCheckMaintenanceResultFailed();
	// 하이브 CheckMaintenance 요청 성공
	void HiveCheckMaintenanceResultSucceeded();

	void HiveCheckMaintenanceResultRetry();
	void HiveCheckMaintenanceResultRetryFailed();
};
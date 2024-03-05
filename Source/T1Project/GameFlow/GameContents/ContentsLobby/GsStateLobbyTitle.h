// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsContentsLobbyBase.h"
#include "Message/GsMessageUI.h"
#include "../Message/GsMessageSystem.h"
#include "../Message/GsMessageUI.h"

/**
 * ≈∏¿Ã∆≤ STATE
 */
class UGsUIWindowTitle;
struct IGsMessageParam;

class T1PROJECT_API FGsStateLobbyTitle : public FGsContentsLobbyBase
{	

protected:
	MsgUIHandleArray		_msgHandleList;
	MsgSystemHandleArray	_msgHandleLobbySystemList;

public:
	FGsStateLobbyTitle();
	virtual ~FGsStateLobbyTitle();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

	virtual void Init() override;
	virtual void Close() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

private:
	void OnClick(const struct IGsMessageParam* InParam);
	//void OnClickHiveLogOut(const struct IGsMessageParam* InParam);

	/*void HiveLogOutFailed(const struct IGsMessageParam* InParam);
	void HiveLogOutSucceeded(const struct IGsMessageParam* InParam);*/
};

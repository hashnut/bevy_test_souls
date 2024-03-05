// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsContentsLobbyBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsBMShopMessageParam.h"


/**
 * 
 */
class T1PROJECT_API FGsStateLobbyZpay : public FGsContentsLobbyBase
{
private:
	TArray<TPair<MessageContentStoreBM, FDelegateHandle>> _bmShopMsgHandlerList;

private:
	bool _checkWaitProc = false;

public:
	FGsStateLobbyZpay();
	virtual ~FGsStateLobbyZpay();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;
	virtual void Close() override;

public:
	virtual void OnReconectionStart() override;
	virtual void OnReconectionEnd() override;

public:
	void WaitProc();


private:
	void OnKickUser(struct FGsSharedParam& InMessageParam);


protected:
	void RegisterMessages();
	void UnregisterMessages();

};

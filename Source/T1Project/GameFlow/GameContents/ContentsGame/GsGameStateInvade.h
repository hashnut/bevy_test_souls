// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "../Message/GsMessageInvasion.h"
#include "../UI/UIContent/Window/GsUIWindowInvade.h"

/**
 *
 */
class T1PROJECT_API FGsGameStateInvade : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<UGsUIWindowInvade>	_windowUI;

protected:
	MsgInvasionHandleArray _invadeDelegateMessage;

public:
	FGsGameStateInvade();
	virtual ~FGsGameStateInvade();

protected:
	void RegisterMessages();
	void UnregisterMessages();

	void InvalidateBattleBadge(const struct IGsMessageParam*);

	void InvasionPortalSpawnInfo(const IGsMessageParam* InParam);
public:
	virtual void Enter() override;
	virtual void Exit() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsContentsLobbyBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/**
 * 
 */
class T1PROJECT_API FGsStateLobbyServerSelect : public FGsContentsLobbyBase
{
private:
	TWeakObjectPtr<class UGsUIWindowServerSelect>	_windowUI;

public:
	FGsStateLobbyServerSelect();
	virtual ~FGsStateLobbyServerSelect();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

public:
	void OnClickSelectServer(const WorldId inWorldID);
};

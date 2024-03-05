// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GsContentsLobbyBase.h"

/**
 * 
 */
class T1PROJECT_API FGsStateLobbyIntro : public FGsContentsLobbyBase
{
public:
	FGsStateLobbyIntro();
	virtual ~FGsStateLobbyIntro();

public:
	virtual void Enter() override;
	virtual void Exit() override;
};

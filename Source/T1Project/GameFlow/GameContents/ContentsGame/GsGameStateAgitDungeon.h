// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"

/**
 * 기사단 영지 던전 State
 */

class T1PROJECT_API FGsGameStateAgitDungeon : public FGsContentsGameBase
{
public:
	FGsGameStateAgitDungeon();
	virtual ~FGsGameStateAgitDungeon();

public:
	virtual void Enter() override;
	virtual void Exit() override;	
};

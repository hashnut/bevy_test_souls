// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"

struct IGsMessageParam;

/**
 * 공성전 State
 */
class T1PROJECT_API FGsGameStateSiegeWarfare : public FGsContentsGameBase
{
public:
	FGsGameStateSiegeWarfare();
	virtual ~FGsGameStateSiegeWarfare();

public:
	virtual void Enter() override;
	virtual void Exit() override;
};

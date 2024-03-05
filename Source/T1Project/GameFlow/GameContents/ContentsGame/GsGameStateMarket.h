// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"

struct IGsMessageParam;

/**
 * 거래소 State
 */
class T1PROJECT_API FGsGameStateMarket : public FGsContentsGameBase
{
public:
	FGsGameStateMarket();
	virtual ~FGsGameStateMarket();

public:
	virtual void Enter() override;
	virtual void Exit() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"

struct IGsMessageParam;

/**
 * 코스튬 및 페어리 재합성 로직
 */
class T1PROJECT_API FGsGameStateRecompose : public FGsContentsGameBase
{
public:
	FGsGameStateRecompose();
	virtual ~FGsGameStateRecompose();

public:
	virtual void Enter() override;
	virtual void Exit() override;

};

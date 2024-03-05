// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"

struct IGsMessageParam;

/**
 * 아이템 장비 복구 로직
 */
class T1PROJECT_API FGsGameStateRepair : public FGsContentsGameBase
{
public:
	FGsGameStateRepair();
	virtual ~FGsGameStateRepair();

public:
	virtual void Enter() override;
	virtual void Exit() override;

};

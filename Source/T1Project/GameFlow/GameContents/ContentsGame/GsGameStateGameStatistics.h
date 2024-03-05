// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"

/**
 * 
 */
class T1PROJECT_API FGsGameStateGameStatistics : public FGsContentsGameBase
{
public:
	FGsGameStateGameStatistics();
	virtual ~FGsGameStateGameStatistics();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void OnReconectionEnd() override;
};

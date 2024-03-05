// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"

/**
 *
 */
class T1PROJECT_API FGsGameStateQuest : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<class UGsUIWindowQuest>	_windowUI;

public:
	FGsGameStateQuest();
	virtual ~FGsGameStateQuest();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

	virtual void Init() override;
	virtual void Close() override;
};

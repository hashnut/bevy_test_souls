
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GsTerrEdModeStateBase.h"
#include "GsTerrEdModeStateManagerBase.h"

class TERRITORYEDITOR_API FGsTerrEdEditorModeState : public FGsTerrEdModeStateBase, public FGsTerrEdModeStateManagerBase<FGsTerrEdModeStateBase>
{
public:
	FGsTerrEdEditorModeState();
	virtual ~FGsTerrEdEditorModeState();

public:
	virtual void Initialize() override;
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Tick(float inTick) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GsTerrEdEditorModeLevelStateBase.h"
#include "Mode/GsTerrEdModeStateBase.h"
#include "Mode/GsTerrEdModeStateManagerBase.h"

class TERRITORYEDITOR_API FGsTerrEdEditorModeLevelEdModeState : public FGsTerrEdEditorModeLevelStateBase, public FGsTerrEdModeStateManagerBase<FGsTerrEdModeStateBase>
{
public:
	FGsTerrEdEditorModeLevelEdModeState();
	virtual ~FGsTerrEdEditorModeLevelEdModeState();
	virtual void Enter() override;
	virtual void Exit() override;

public:
	virtual void MapChange() override;
	virtual void MapOpened(const FString& inFilename) override;
	virtual void NewCurrentLevel() override;
};
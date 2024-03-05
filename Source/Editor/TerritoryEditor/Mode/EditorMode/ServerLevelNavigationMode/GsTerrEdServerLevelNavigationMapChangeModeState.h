
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeLevelStateBase.h"

class TERRITORYEDITOR_API FGsTerrEdServerLevelNavigationMapChangeModeState : public FGsTerrEdModeStateBase
{
private:
	class FGsTerrEdEditorModeServerLevelNavigationModeState* _parents;

public:
	FGsTerrEdServerLevelNavigationMapChangeModeState();
	virtual ~FGsTerrEdServerLevelNavigationMapChangeModeState();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	void SetParents(class FGsTerrEdEditorModeServerLevelNavigationModeState* inParents) { _parents = inParents; }
};
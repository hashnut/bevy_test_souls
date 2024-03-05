
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeLevelStateBase.h"
#include "Mode/EditorMode/GsTerrEdEditorModeDesignLevelNavigationModeState.h"

class TERRITORYEDITOR_API FGsTerrEdDesignLevelNavigationPostSaveModeState : public FGsTerrEdModeStateBase
{
private:
	class FGsTerrEdEditorModeDesignLevelNavigationModeState* _parents;

public:
	FGsTerrEdDesignLevelNavigationPostSaveModeState();
	virtual ~FGsTerrEdDesignLevelNavigationPostSaveModeState();

public:
	virtual void Enter() override;
	virtual void Tick(float inTick) override;
	virtual void Exit() override;

public:
	void SetParents(class FGsTerrEdEditorModeDesignLevelNavigationModeState* inParents) { _parents = inParents; }
};
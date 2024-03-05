
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeLevelStateBase.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeDesignLevelNavigationModeState.h"

class TERRITORYEDITOR_API FGsTerrEdDesignLevelNavigationMapIdleModeState : public FGsTerrEdModeStateBase
{
private:
	class FGsTerrEdEditorModeDesignLevelNavigationModeState* _parents;

public:
	FGsTerrEdDesignLevelNavigationMapIdleModeState();
	virtual ~FGsTerrEdDesignLevelNavigationMapIdleModeState();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	void SetParents(class FGsTerrEdEditorModeDesignLevelNavigationModeState* inParents) { _parents = inParents; }
};
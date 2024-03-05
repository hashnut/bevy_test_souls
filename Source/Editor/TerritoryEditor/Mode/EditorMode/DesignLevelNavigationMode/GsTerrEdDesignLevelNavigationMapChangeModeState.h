
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeLevelStateBase.h"

class TERRITORYEDITOR_API FGsTerrEdDesignLevelNavigationMapChangeModeState : public FGsTerrEdModeStateBase
{
private:
	class FGsTerrEdEditorModeDesignLevelNavigationModeState* _parents;

public:
	FGsTerrEdDesignLevelNavigationMapChangeModeState();
	virtual ~FGsTerrEdDesignLevelNavigationMapChangeModeState();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	void SetParents(class FGsTerrEdEditorModeDesignLevelNavigationModeState* inParents) { _parents = inParents; }
};
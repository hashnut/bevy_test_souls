
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeLevelStateBase.h"

class TERRITORYEDITOR_API FGsTerrEdLevelNavigationMapIdleModeState : public FGsTerrEdModeStateBase
{
private:
	class FGsTerrEdEditorModeLevelNavigationModeState* _parents;

public:
	FGsTerrEdLevelNavigationMapIdleModeState();
	virtual ~FGsTerrEdLevelNavigationMapIdleModeState();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	void SetParents(class FGsTerrEdEditorModeLevelNavigationModeState* inParents) { _parents = inParents; }
};
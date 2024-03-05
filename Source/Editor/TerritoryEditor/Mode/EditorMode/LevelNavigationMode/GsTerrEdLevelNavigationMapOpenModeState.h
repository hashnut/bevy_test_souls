
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeLevelStateBase.h"

/// <summary>
/// MapOpen 호출되면 호출되는 스테이트
/// </summary>
class TERRITORYEDITOR_API FGsTerrEdLevelNavigationMapOpenModeState : public FGsTerrEdModeStateBase
{
public:
	FGsTerrEdLevelNavigationMapOpenModeState();
	virtual ~FGsTerrEdLevelNavigationMapOpenModeState();

private:
	class FGsTerrEdEditorModeLevelNavigationModeState* _parents;

public:
	virtual void Enter() override;
	virtual void Tick(float inTick) override;
	virtual void Exit() override;

public:
	void SetParents(class FGsTerrEdEditorModeLevelNavigationModeState* inParents) { _parents = inParents; }
};
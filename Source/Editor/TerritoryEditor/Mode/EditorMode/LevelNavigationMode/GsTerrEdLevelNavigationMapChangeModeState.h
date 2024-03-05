
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeLevelStateBase.h"

/// <summary>
/// 맵 Change 호출시 대기 상태, MapOpen 상태 호출
/// </summary>
class TERRITORYEDITOR_API FGsTerrEdLevelNavigationMapChangeModeState : public FGsTerrEdModeStateBase
{
private:
	class FGsTerrEdEditorModeLevelNavigationModeState* _parents;

public:
	FGsTerrEdLevelNavigationMapChangeModeState();
	virtual ~FGsTerrEdLevelNavigationMapChangeModeState();

public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	void SetParents(class FGsTerrEdEditorModeLevelNavigationModeState* inParents) { _parents = inParents; }
};
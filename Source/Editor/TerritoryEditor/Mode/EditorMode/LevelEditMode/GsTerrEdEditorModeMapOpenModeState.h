
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeLevelStateBase.h"

class TERRITORYEDITOR_API FGsTerrEdEditorModeMapOpenModeState : public FGsTerrEdModeStateBase
{
public:
	FGsTerrEdEditorModeMapOpenModeState();
	virtual ~FGsTerrEdEditorModeMapOpenModeState();

public:
	virtual void Enter() override;
	virtual void Exit() override;
};
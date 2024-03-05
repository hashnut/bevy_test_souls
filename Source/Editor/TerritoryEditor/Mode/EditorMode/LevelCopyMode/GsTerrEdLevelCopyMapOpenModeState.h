
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "TerritoryEditor/Mode/EditorMode/GsTerrEdEditorModeLevelStateBase.h"

class TERRITORYEDITOR_API FGsTerrEdLevelCopyMapOpenModeState : public FGsTerrEdModeStateBase
{
public:
	FGsTerrEdLevelCopyMapOpenModeState();
	virtual ~FGsTerrEdLevelCopyMapOpenModeState();

public:
	virtual void Enter() override;
	virtual void Exit() override;	
};
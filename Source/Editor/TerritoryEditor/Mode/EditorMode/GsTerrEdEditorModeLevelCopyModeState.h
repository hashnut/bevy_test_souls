
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateManagerBase.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"
#include "GsTerrEdEditorModeLevelStateBase.h"

class TERRITORYEDITOR_API FGsTerrEdEditorModeLevelCopyModeState : public FGsTerrEdEditorModeLevelStateBase, public FGsTerrEdModeStateManagerBase<FGsTerrEdModeStateBase>
{
public:
	FGsTerrEdEditorModeLevelCopyModeState();
	virtual ~FGsTerrEdEditorModeLevelCopyModeState();
public:
	virtual void Enter() override;
	virtual void Exit() override;

public:
	virtual void MapChange() override;
	virtual void MapOpened(const FString& inFilename) override;
	virtual void NewCurrentLevel() override;
};
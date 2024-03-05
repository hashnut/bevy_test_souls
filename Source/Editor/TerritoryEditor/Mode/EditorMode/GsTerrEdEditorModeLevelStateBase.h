
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "TerritoryEditor/Mode/GsTerrEdModeStateBase.h"

class TERRITORYEDITOR_API FGsTerrEdEditorModeLevelStateBase : public FGsTerrEdModeStateBase
{
public:	
	FGsTerrEdEditorModeLevelStateBase(){}
	virtual ~FGsTerrEdEditorModeLevelStateBase(){}
	virtual void MapChange(){}
	virtual void MapOpened(const FString& inFilename){}
	virtual void NewCurrentLevel(){}
	virtual void PostSave(){}
};
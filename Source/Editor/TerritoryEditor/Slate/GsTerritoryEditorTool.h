
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SDockTab.h"
#include "SDockableTab.h"
#include "SDockTabStack.h"
#include "SlateApplication.h"
#include "SGsTerrEdToolCompoundWidget.h"

class SGsTerrEdToolCompoundWidget;

const FName TERRITORY_TOOL_NAME = TEXT("TerritoryWindowTabTool");

class FGsTerritoryEditorTool
{
public:
	TWeakPtr<SGsTerrEdToolCompoundWidget> _mainTab;

public:
	FGsTerritoryEditorTool();  
	~FGsTerritoryEditorTool();

public:
	void ReigsterTab();
	void UnregisterTab();

public:
	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& inTabSpawnArgs);
};
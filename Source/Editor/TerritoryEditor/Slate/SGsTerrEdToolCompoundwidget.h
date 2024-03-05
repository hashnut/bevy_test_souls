
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SDockTab.h"
#include "SDockableTab.h"
#include "SDockTabStack.h"
#include "SlateApplication.h"
#include "SCompoundWidget.h"
#include "TabManager.h"
#include "GsTerritoryEditorTool.h"

const FName TERRITORY_FINDER_TOOL_NAME = TEXT("TerritoryFinderTool");
const FName TERRITORY_EXPORT_TOOL_NAME = TEXT("TerritoryExportTool");
const FName TERRITORY_RECOVERY_TOOL_NAME = TEXT("TerritoryRecoveryTool");
const FName TERRITORY_UTIL_TOOL_NAME = TEXT("TerritoryUtilTool");
const FName TERRITORY_LEVEL_TOOL_NAME = TEXT("TerritoryLevelTool");

class SGsTerrEdToolCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdToolCompoundWidget) {}		
	SLATE_END_ARGS()

public:
	TSharedPtr<FTabManager> _tabManager;
	TSharedPtr<SWindow> _window;

public:
	void Construct(const FArguments& inArgs, const TSharedRef<SDockTab>& inConstructUnderMajorTab
		, const TSharedPtr<SWindow>& inConstructUnderWindow);

public:
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
		, const FName inTabIdentifier);
};
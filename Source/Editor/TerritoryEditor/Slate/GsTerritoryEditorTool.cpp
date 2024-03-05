// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritoryEditorTool.h"
#include "SGsTerrEdToolCompoundWidget.h"
#include "Internationalization.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"

FGsTerritoryEditorTool::FGsTerritoryEditorTool()
{

}

FGsTerritoryEditorTool::~FGsTerritoryEditorTool()
{

}

void FGsTerritoryEditorTool::ReigsterTab()
{	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TERRITORY_TOOL_NAME, FOnSpawnTab::CreateRaw(this, &FGsTerritoryEditorTool::SpawnTab))
		.SetDisplayName(LOCTEXT("TerritoryToolTabName", "Territory Editor"))		
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FGsTerritoryEditorTool::UnregisterTab()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TERRITORY_TOOL_NAME);
}

TSharedRef<SDockTab> FGsTerritoryEditorTool::SpawnTab(const FSpawnTabArgs& inTabSpawnArgs)
{
	const TSharedRef<SDockTab> mainTab = SNew(SDockTab).TabRole(ETabRole::MajorTab);
	TSharedRef<SGsTerrEdToolCompoundWidget> widget = SNew(SGsTerrEdToolCompoundWidget, mainTab, inTabSpawnArgs.GetOwnerWindow());
	_mainTab = widget;
	mainTab->SetContent(widget);

	return mainTab;
}

#undef LOCTEXT_NAMESPACE
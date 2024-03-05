// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdToolCompoundWidget.h"
#include "GsTerritoryEditorTool.h"
#include "Finder/SGsTerrEdFinderCompoundWidget.h"
#include "Export/SGsTerrEdExportCompoundwidget.h"
#include "Recovery/SGsTerrEdRecoveryCompoundwidget.h"
#include "Level/SGsTerrEdToolLevelCompoundWidget.h"
#include "Util/SGsTerrEdUtilCompoundwidget.h"
#include "TabManager.h"
#include "LogMacros.h"
#include "DelegateSignatureImpl.inl"
#include "Internationalization.h"
#include "NameTypes.h"
#include "TerritoryEditor.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdToolCompoundWidget::Construct(const FArguments& inArgs
	, const TSharedRef<SDockTab>& inConstructUnderMajorTab
	, const TSharedPtr<SWindow>& inConstructUnderWindow)
{
	_window = inConstructUnderWindow;
	_tabManager = FGlobalTabmanager::Get()->NewTabManager(inConstructUnderMajorTab);
	TSharedRef<FWorkspaceItem> appMenuGroup = _tabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("TerritoyToolGroup", "TerritoryTool"));

	//Finder
	_tabManager->RegisterTabSpawner(TERRITORY_FINDER_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdToolCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_FINDER_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryToolFinder", "Finder"))		
		.SetGroup(appMenuGroup);
	_tabManager->RegisterTabSpawner(TERRITORY_EXPORT_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdToolCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_EXPORT_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryToolExport", "Export"))
		.SetGroup(appMenuGroup);
	_tabManager->RegisterTabSpawner(TERRITORY_RECOVERY_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdToolCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_RECOVERY_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryToolRecovery", "Recovery"))
		.SetGroup(appMenuGroup);
	_tabManager->RegisterTabSpawner(TERRITORY_UTIL_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdToolCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_UTIL_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryToolUtil", "Util"))
		.SetGroup(appMenuGroup);
	_tabManager->RegisterTabSpawner(TERRITORY_LEVEL_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdToolCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_LEVEL_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryToolLevel", "Level"))
		.SetGroup(appMenuGroup);

	const TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("territoryEditorToolLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				// applications
				FTabManager::NewStack()
				->AddTab(TERRITORY_FINDER_TOOL_NAME, ETabState::OpenedTab)
				->AddTab(TERRITORY_EXPORT_TOOL_NAME, ETabState::OpenedTab)
				->AddTab(TERRITORY_RECOVERY_TOOL_NAME, ETabState::OpenedTab)
				->AddTab(TERRITORY_UTIL_TOOL_NAME, ETabState::OpenedTab)				
				->AddTab(TERRITORY_LEVEL_TOOL_NAME, ETabState::OpenedTab)
				->SetSizeCoefficient(0.75f)
				->SetForegroundTab(TERRITORY_FINDER_TOOL_NAME)
			)
		);

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			_tabManager->RestoreFrom(layout, inConstructUnderWindow).ToSharedRef()
		]
	];
}

TSharedRef<SDockTab> SGsTerrEdToolCompoundWidget::HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
	, const FName inTabIdentifier)
{
	TSharedRef<SDockTab> tab = SNew(SDockTab).TabRole(ETabRole::PanelTab);
	TSharedPtr<SWidget> widget = SNullWidget::NullWidget;
	if (inTabIdentifier == TERRITORY_FINDER_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdFinderCompoundWidget, tab, inArgs.GetOwnerWindow());
	}
	else if (inTabIdentifier == TERRITORY_EXPORT_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdExportCompoundWidget);
	}
	else if (inTabIdentifier == TERRITORY_RECOVERY_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdRecoveryCompoundWidget);
	}
	else if (inTabIdentifier == TERRITORY_UTIL_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdUtilCompoundWidget);
	}	
	else if (inTabIdentifier == TERRITORY_LEVEL_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdToolLevelCompoundWidget, tab, inArgs.GetOwnerWindow());
	}
	else
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("Invalid TabIdentifier: '%s'"), *inTabIdentifier.ToString());
	}

	tab->SetContent(widget.ToSharedRef());

	return tab;
}

#undef LOCTEXT_NAMESPACE
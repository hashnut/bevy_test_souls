// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdToolLevelCompoundWidget.h"
#include "LogMacros.h"
#include "TabManager.h"
#include "SHeaderRow.h"
#include "EditorStyleSet.h"
#include "STextBlock.h"
#include "SButton.h"
#include "Reply.h"
#include "Util/GsTerrEdRecoveryUtil.h"
#include "Data/GsTerritoryDataManager.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "TerritoryEditor.h"
#include "SNumericEntryBox.h"
#include "DetailLayoutBuilder.h"
#include "SGsTerrEdToolLevelCopyCompoundWidget.h"
#include "SGsTerrEdToolLevelViewSettingCompoundWidget.h"
#include "Slate/SGsTerrEdToolCompoundwidget.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdToolLevelCompoundWidget::Construct(const FArguments& inArgs, const TSharedRef<SDockTab>& inConstructUnderMajorTab, const TSharedPtr<SWindow>& inConstructUnderWindow)
{
	_tabManager = FGlobalTabmanager::Get()->NewTabManager(inConstructUnderMajorTab);
	TSharedRef<FWorkspaceItem> appMenuGroup = _tabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("TerritoyLevelToolGroup", "TerritoryLevelTool"));

	_tabManager->RegisterTabSpawner(TERRITORY_LEVEL_VIEW_SETTING_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdToolLevelCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_LEVEL_VIEW_SETTING_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryLevelViewSetting", "Level View Setting"))
		.SetGroup(appMenuGroup);
	_tabManager->RegisterTabSpawner(TERRITORY_LEVEL_COPY_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdToolLevelCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_LEVEL_COPY_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryLevelCopy", "Level Copy"))
		.SetGroup(appMenuGroup);

	const TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("territoryEditorLevelToolLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				// applications
				FTabManager::NewStack()
				->AddTab(TERRITORY_LEVEL_VIEW_SETTING_TOOL_NAME, ETabState::OpenedTab)
				->AddTab(TERRITORY_LEVEL_COPY_TOOL_NAME, ETabState::OpenedTab)
				->SetSizeCoefficient(0.75f)
				->SetForegroundTab(TERRITORY_LEVEL_VIEW_SETTING_TOOL_NAME)
			)
		);

	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			SNew(SBorder)
			.BorderBackgroundColor(FColor(192, 192, 192, 255))
		[
			_tabManager->RestoreFrom(layout, inConstructUnderWindow).ToSharedRef()
		]
		]
		];

	_selectTabType = EGsTerrEdLevel::LevelMax;
}

SGsTerrEdToolLevelCompoundWidget::~SGsTerrEdToolLevelCompoundWidget()
{
	FGlobalTabmanager::Get()->OnActiveTabChanged_Unsubscribe(_openHandle);

	if (_levelViewSettingTab.IsValid())
	{
		_levelViewSettingTab.Pin()->SetOnTabActivated(nullptr);
	}

	if (_levelCopyTab.IsValid())
	{
		_levelCopyTab.Pin()->SetOnTabActivated(nullptr);
	}
}

TSharedRef<SDockTab> SGsTerrEdToolLevelCompoundWidget::HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs, const FName inTabIdentifier)
{
	TSharedRef<SDockTab> tab = SNew(SDockTab).TabRole(ETabRole::PanelTab);

	TSharedPtr<SWidget> widget = SNullWidget::NullWidget;
	if (inTabIdentifier == TERRITORY_LEVEL_VIEW_SETTING_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdToolLevelViewSettingCompoundWidget);
		_levelViewSetting = widget;
		_levelViewSettingTab = tab;

		tab->SetOnTabActivated(SDockTab::FOnTabActivatedCallback::CreateRaw(this, &SGsTerrEdToolLevelCompoundWidget::OnActiveLevelViewSeetingTabChanged));
	}
	else if (inTabIdentifier == TERRITORY_LEVEL_COPY_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdToolLevelCopyCompoundWidget);
		_levelCopy = widget;
		_levelCopyTab = tab;

		tab->SetOnTabActivated(SDockTab::FOnTabActivatedCallback::CreateRaw(this, &SGsTerrEdToolLevelCompoundWidget::OnActiveLevelCopyTabChanged));
	}
	else
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("Invalid TabIdentifier: '%s'"), *inTabIdentifier.ToString());
	}

	tab->SetContent(widget.ToSharedRef());

	return tab;
}

void SGsTerrEdToolLevelCompoundWidget::OnActiveLevelViewSeetingTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause)
{
	if (_levelViewSetting.IsValid())
	{
		SGsTerrEdToolLevelViewSettingCompoundWidget* widget = static_cast<SGsTerrEdToolLevelViewSettingCompoundWidget*>(_levelViewSetting.Pin().Get());
		if (widget)
		{
			if (_selectTabType != EGsTerrEdLevel::ViewSetting)
			{
				_selectTabType = EGsTerrEdLevel::ViewSetting;
				//finder->ClearListView();
			}
		}
	}
}

void SGsTerrEdToolLevelCompoundWidget::OnActiveLevelCopyTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause)
{
	if (_levelCopy.IsValid())
	{
		SGsTerrEdToolLevelCopyCompoundWidget* widget = static_cast<SGsTerrEdToolLevelCopyCompoundWidget*>(_levelCopy.Pin().Get());
		if (widget)
		{
			if (_selectTabType != EGsTerrEdLevel::Copy)
			{
				_selectTabType = EGsTerrEdLevel::Copy;				
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdFinderCompoundWidget.h"
#include "LogMacros.h"
#include "TabManager.h"
#include "TerritoryEditor.h"
#include "Framework/Docking/TabManager.h"
#include "SDockTab.h"
#include "DelegateSignatureImpl.inl"
#include "SGsTerrEdZoneFinderCompoundwidget.h"
#include "SGsTerrEdNpcFinderCompoundWidget.h"
#include "SGsTerrEdPropFinderCompoundWidget.h"
#include "SGsTerrEdSpotFinderCompoundwidget.h"
#include "SGsTerrEdFenceFinderCompoundwidget.h"
#include "Editor/TerritoryEditor/Util/GsTerritoryLevelUtil.h"
#include "Slate/Finder/SGsTerrEdLineFinderCompoundWidget.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdFinderCompoundWidget::Construct(const FArguments& inArgs
	, const TSharedRef<SDockTab>& inConstructUnderMajorTab
	, const TSharedPtr<SWindow>& inConstructUnderWindow)
{
	_newCurrentLevelHandle = FTerritoryEditor::_territoryEditorClearFinderEvent.AddRaw(this, &SGsTerrEdFinderCompoundWidget::OnChagneNewServerLevel);

	_tabManager = FGlobalTabmanager::Get()->NewTabManager(inConstructUnderMajorTab);
	TSharedRef<FWorkspaceItem> appMenuGroup = _tabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("TerritoyFinderToolGroup", "TerritoryFinderTool"));

	_tabManager->RegisterTabSpawner(TERRITORY_ZONE_FINDER_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_ZONE_FINDER_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryZoneFinderTool", "Zone Finder"))
		.SetGroup(appMenuGroup);
	_tabManager->RegisterTabSpawner(TERRITORY_NPC_FINDER_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_NPC_FINDER_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryNpcFinderTool", "Npc Finder"))
		.SetGroup(appMenuGroup);			
	_tabManager->RegisterTabSpawner(TERRITORY_PROP_FINDER_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_PROP_FINDER_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryPropFinderTool", "Prop Finder"))
		.SetGroup(appMenuGroup);	
	_tabManager->RegisterTabSpawner(TERRITORY_SPOT_FINDER_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_SPOT_FINDER_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritorySpotFinderTool", "Spot Finder"))
		.SetGroup(appMenuGroup);
	_tabManager->RegisterTabSpawner(TERRITORY_LINE_FINDER_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_LINE_FINDER_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryLineFinderTool", "Line Finder"))
		.SetGroup(appMenuGroup);
	_tabManager->RegisterTabSpawner(TERRITORY_FENCE_FINDER_TOOL_NAME
		, FOnSpawnTab::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::HandleTabManagerSpawnTab, TERRITORY_FENCE_FINDER_TOOL_NAME))
		.SetDisplayName(LOCTEXT("TerritoryFenceFinderTool", "Fence Finder"))
		.SetGroup(appMenuGroup);

	const TSharedRef<FTabManager::FLayout> layout = FTabManager::NewLayout("territoryEditorFinderToolLayout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
			->SetOrientation(Orient_Vertical)
			->Split
			(
				// applications
				FTabManager::NewStack()
				->AddTab(TERRITORY_ZONE_FINDER_TOOL_NAME, ETabState::OpenedTab)
				->AddTab(TERRITORY_NPC_FINDER_TOOL_NAME, ETabState::OpenedTab)
				->AddTab(TERRITORY_PROP_FINDER_TOOL_NAME, ETabState::OpenedTab)						
				->AddTab(TERRITORY_SPOT_FINDER_TOOL_NAME, ETabState::OpenedTab)
				->AddTab(TERRITORY_LINE_FINDER_TOOL_NAME, ETabState::OpenedTab)
				->AddTab(TERRITORY_FENCE_FINDER_TOOL_NAME, ETabState::OpenedTab)
				->SetSizeCoefficient(0.9f)
				->SetForegroundTab(TERRITORY_ZONE_FINDER_TOOL_NAME)
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

	_selectTabType = EGsTerrEdFinder::Max;

	_mapOpenedHandle = FEditorDelegates::OnMapOpened.AddRaw(this, &SGsTerrEdFinderCompoundWidget::ClearFinder);
}

SGsTerrEdFinderCompoundWidget::~SGsTerrEdFinderCompoundWidget()
{
	FTerritoryEditor::_territoryEditorClearFinderEvent.Remove(_newCurrentLevelHandle);

	FGlobalTabmanager::Get()->OnActiveTabChanged_Unsubscribe(_openHandle);

	if (_zoneFinderTab.IsValid())
	{
		_zoneFinderTab.Pin()->SetOnTabActivated(nullptr);
	}

	if (_npcFinderTab.IsValid())
	{
		_npcFinderTab.Pin()->SetOnTabActivated(nullptr);
	}

	if (_propFinderTab.IsValid())
	{
		_propFinderTab.Pin()->SetOnTabActivated(nullptr);
	}

	if (_spotFinderTab.IsValid())
	{
		_spotFinderTab.Pin()->SetOnTabActivated(nullptr);
	}

	if(_lineFinderTab.IsValid())
	{
		_lineFinderTab.Pin()->SetOnTabActivated(nullptr);
	}

	if (_fenceFinderTab.IsValid())
	{
		_fenceFinderTab.Pin()->SetOnTabActivated(nullptr);
	}

	if (_mapOpenedHandle.IsValid())
	{
		FEditorDelegates::OnMapOpened.Remove(_mapOpenedHandle);
	}	
}

TSharedRef<SDockTab> SGsTerrEdFinderCompoundWidget::HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
	, const FName inTabIdentifier)
{
	TSharedRef<SDockTab> tab = SNew(SDockTab).TabRole(ETabRole::PanelTab);

	TSharedPtr<SWidget> widget = SNullWidget::NullWidget;
	if (inTabIdentifier == TERRITORY_ZONE_FINDER_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdZoneFinderCompoundwidget);
		_zoneFinder = widget;
		_zoneFinderTab = tab;

		tab->SetOnTabActivated(SDockTab::FOnTabActivatedCallback::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::OnActiveZoneTabChanged));
	}
	else if (inTabIdentifier == TERRITORY_NPC_FINDER_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdNpcFinderCompoundWidget);
		_npcFinder = widget;
		_npcFinderTab = tab;
		
		tab->SetOnTabActivated(SDockTab::FOnTabActivatedCallback::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::OnActiveNpcTabChanged));
	}	
	else if (inTabIdentifier == TERRITORY_PROP_FINDER_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdPropFinderCompoundWidget);
		_propFinder = widget;
		_propFinderTab = tab;

		tab->SetOnTabActivated(SDockTab::FOnTabActivatedCallback::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::OnActivePropTabChanged));
	}
	else if (inTabIdentifier == TERRITORY_SPOT_FINDER_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdSpotFinderCompoundWidget);
		_spotFinder = widget;
		_spotFinderTab = tab;

		tab->SetOnTabActivated(SDockTab::FOnTabActivatedCallback::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::OnActiveSpotTabChanged));
	}
	else if (inTabIdentifier == TERRITORY_LINE_FINDER_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdLineFinderCompoundWidget);
		_lineFinder = widget;
		_lineFinderTab = tab;

		tab->SetOnTabActivated(SDockTab::FOnTabActivatedCallback::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::OnActiveLineTabChanged));
	}
	else if (inTabIdentifier == TERRITORY_FENCE_FINDER_TOOL_NAME)
	{
		widget = SNew(SGsTerrEdFenceFinderCompoundwidget);
		_fenceFinder = widget;
		_fenceFinderTab = tab;

		tab->SetOnTabActivated(SDockTab::FOnTabActivatedCallback::CreateRaw(this, &SGsTerrEdFinderCompoundWidget::OnActiveFenceTabChanged));
	}
	else
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("Invalid TabIdentifier: '%s'"), *inTabIdentifier.ToString());
	}

	tab->SetContent(widget.ToSharedRef());

	return tab;
}

void SGsTerrEdFinderCompoundWidget::OnActiveZoneTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause)
{
	if (_zoneFinder.IsValid())
	{
		SGsTerrEdZoneFinderCompoundwidget* finder = static_cast<SGsTerrEdZoneFinderCompoundwidget*>(_zoneFinder.Pin().Get());
		if (finder)
		{
			if (_selectTabType != Zone)
			{
				_selectTabType = Zone;
				finder->ClearListView();
			}
		}
	}
}

void SGsTerrEdFinderCompoundWidget::OnActivePropTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause)
{
	if (_propFinder.IsValid())
	{
		SGsTerrEdPropFinderCompoundWidget* finder = static_cast<SGsTerrEdPropFinderCompoundWidget*>(_propFinder.Pin().Get());
		if (finder)
		{
			if (_selectTabType != Npc)
			{
				_selectTabType = Npc;
				finder->ClearListView();
			}			
		}
	}
}

void SGsTerrEdFinderCompoundWidget::OnActiveNpcTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause)
{
	if (_npcFinder.IsValid())
	{
		SGsTerrEdNpcFinderCompoundWidget* finder = static_cast<SGsTerrEdNpcFinderCompoundWidget*>(_npcFinder.Pin().Get());
		if (finder)
		{
			if (_selectTabType != Prop)
			{
				_selectTabType = Prop;
				finder->ClearListView();
			}
		}
	}
}

void SGsTerrEdFinderCompoundWidget::OnActiveSpotTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause)
{
	if (_spotFinder.IsValid())
	{
		SGsTerrEdSpotFinderCompoundWidget* finder = static_cast<SGsTerrEdSpotFinderCompoundWidget*>(_spotFinder.Pin().Get());
		if (finder)
		{
			if (_selectTabType != Spot)
			{
				_selectTabType = Spot;
				finder->ClearListView();
			}
		}
	}
}

void SGsTerrEdFinderCompoundWidget::OnActiveLineTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause)
{
	if (_lineFinder.IsValid())
	{
		SGsTerrEdLineFinderCompoundWidget* finder = static_cast<SGsTerrEdLineFinderCompoundWidget*>(_lineFinder.Pin().Get());
		if (finder)
		{
			if (_selectTabType != Line)
			{
				_selectTabType = Line;
				finder->ClearListView();
			}
		}
	}
}

void SGsTerrEdFinderCompoundWidget::OnActiveFenceTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause)
{
	if (_fenceFinder.IsValid())
	{
		SGsTerrEdFenceFinderCompoundwidget* finder = static_cast<SGsTerrEdFenceFinderCompoundwidget*>(_fenceFinder.Pin().Get());
		if (finder)
		{
			if (_selectTabType != Fence)
			{
				_selectTabType = Fence;
				finder->ClearListView();
			}
		}
	}
}

void SGsTerrEdFinderCompoundWidget::OnChagneNewServerLevel()
{
	//chagne new server level
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}
	
	const FString& path = world->GetCurrentLevel()->GetPathName();
	if (false == UGsTerritoryLevelUtil::IsServerLevel(path))
	{
		return;
	}

	const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(path);
	FString persistantLevelName;	
	FString levelKey;
	//is server level
	if (false == UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevelName, levelKey))
	{
		return;
	}

	ClearFinderInternal();
}

void SGsTerrEdFinderCompoundWidget::ClearFinder(const FString& inFilename, bool inIsTemplate)
{
	ClearFinderInternal();
}

void SGsTerrEdFinderCompoundWidget::ClearFinderInternal()
{
	//reset npc id
	if (_zoneFinderTab.Pin()->IsForeground())
	{
		SGsTerrEdZoneFinderCompoundwidget* finder = static_cast<SGsTerrEdZoneFinderCompoundwidget*>(_zoneFinder.Pin().Get());
		if (finder)
		{
			finder->ClearListView();
		}
	}
	if (_npcFinderTab.Pin()->IsForeground())
	{
		SGsTerrEdNpcFinderCompoundWidget* finder = static_cast<SGsTerrEdNpcFinderCompoundWidget*>(_npcFinder.Pin().Get());
		if (finder)
		{
			finder->ClearListView();
		}
	}
	if (_propFinderTab.Pin()->IsForeground())
	{
		SGsTerrEdPropFinderCompoundWidget* finder = static_cast<SGsTerrEdPropFinderCompoundWidget*>(_propFinder.Pin().Get());
		if (finder)
		{
			finder->ClearListView();
		}
	}
	if (_spotFinderTab.Pin()->IsForeground())
	{
		SGsTerrEdSpotFinderCompoundWidget* finder = static_cast<SGsTerrEdSpotFinderCompoundWidget*>(_spotFinder.Pin().Get());
		if (finder)
		{
			finder->ClearListView();
		}
	}
	if (_lineFinderTab.Pin()->IsForeground())
	{
		SGsTerrEdLineFinderCompoundWidget* finder = static_cast<SGsTerrEdLineFinderCompoundWidget*>(_lineFinder.Pin().Get());
		if (finder)
		{
			finder->ClearListView();
		}
	}
	if (_fenceFinderTab.Pin()->IsForeground())
	{
		SGsTerrEdFenceFinderCompoundwidget* finder = static_cast<SGsTerrEdFenceFinderCompoundwidget*>(_fenceFinder.Pin().Get());
		if (finder)
		{
			finder->ClearListView();
		}
	}
}

#undef LOCTEXT_NAMESPACE

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SDockTab.h"
#include "SDockableTab.h"
#include "SDockTabStack.h"
#include "SlateApplication.h"

const FName TERRITORY_ZONE_FINDER_TOOL_NAME = TEXT("TerritoryZoneFinderTool");
const FName TERRITORY_NPC_FINDER_TOOL_NAME = TEXT("TerritoryNpcFinderTool");
const FName TERRITORY_PROP_FINDER_TOOL_NAME = TEXT("TerritoryPropFinderTool");
const FName TERRITORY_SPOT_FINDER_TOOL_NAME = TEXT("TerritorySpotFinderTool");
const FName TERRITORY_LINE_FINDER_TOOL_NAME = TEXT("TerritoryLineFinderTool");
const FName TERRITORY_FENCE_FINDER_TOOL_NAME = TEXT("TerritoryFenceFinderTool");
class SWindow;

class IGsTerrEdFinderInterface
{
public :
	virtual void ClearListView()=0;
	virtual ~IGsTerrEdFinderInterface(){}
};

enum EGsTerrEdFinder
{
	Zone,
	Npc,
	Prop,
	Spot,
	Line,
	Fence,
	Max
};

class SGsTerrEdFinderCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdFinderCompoundWidget){}	
	SLATE_ARGUMENT(TSharedPtr<SWindow>, window)
	SLATE_END_ARGS()

private:
	TSharedPtr<FTabManager> _tabManager;
	FDelegateHandle _openHandle;
	FDelegateHandle _newCurrentLevelHandle;

private:
	TWeakPtr<SWidget> _zoneFinder;
	TWeakPtr<SDockTab> _zoneFinderTab;

	TWeakPtr<SWidget> _npcFinder;
	TWeakPtr<SDockTab> _npcFinderTab;

	TWeakPtr<SWidget> _propFinder;	
	TWeakPtr<SDockTab> _propFinderTab;

	TWeakPtr<SWidget> _spotFinder;
	TWeakPtr<SDockTab> _spotFinderTab;

	TWeakPtr<SWidget> _lineFinder;
	TWeakPtr<SDockTab> _lineFinderTab;

	TWeakPtr<SWidget> _fenceFinder;
	TWeakPtr<SDockTab> _fenceFinderTab;

	EGsTerrEdFinder _selectTabType = EGsTerrEdFinder::Max;
	FDelegateHandle _mapOpenedHandle;

public:
	void Construct(const FArguments& inArgs
		, const TSharedRef<SDockTab>& inConstructUnderMajorTab
		, const TSharedPtr<SWindow>& inConstructUnderWindow);
	virtual ~SGsTerrEdFinderCompoundWidget();

public:
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
		, const FName inTabIdentifier);
	void OnActiveZoneTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause);
	void OnActiveNpcTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause);
	void OnActivePropTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause);
	void OnActiveSpotTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause);
	void OnActiveLineTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause);
	void OnActiveFenceTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause);
	void OnChagneNewServerLevel();
	void ClearFinder(const FString& inFilename, bool inIsTemplate);

private:
	void ClearFinderInternal();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SDockTab.h"
#include "SDockableTab.h"
#include "SDockTabStack.h"
#include "SlateApplication.h"
#include "GsTerritoryLevelManager.h"

const FName TERRITORY_LEVEL_VIEW_SETTING_TOOL_NAME = TEXT("TerritoryLevelViewSettingTool");
const FName TERRITORY_LEVEL_COPY_TOOL_NAME = TEXT("TerritoryLevelCopyTool");
class SWindow;

enum EGsTerrEdLevel
{
	ViewSetting,
	Copy,
	LevelMax
};

class SGsTerrEdToolLevelCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdToolLevelCompoundWidget){}
	SLATE_ARGUMENT(TSharedPtr<SWindow>, window)
	SLATE_END_ARGS()

private:
	TSharedPtr<FTabManager> _tabManager;
	FDelegateHandle _openHandle;

private:
	TWeakPtr<SWidget> _levelViewSetting;
	TWeakPtr<SDockTab> _levelViewSettingTab;

	TWeakPtr<SWidget> _levelCopy;
	TWeakPtr<SDockTab> _levelCopyTab;

	EGsTerrEdLevel _selectTabType = EGsTerrEdLevel::LevelMax;

public:
	void Construct(const FArguments& inArgs
		, const TSharedRef<SDockTab>& inConstructUnderMajorTab
		, const TSharedPtr<SWindow>& inConstructUnderWindow);
	virtual ~SGsTerrEdToolLevelCompoundWidget();

public:
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
		, const FName inTabIdentifier);
	void OnActiveLevelViewSeetingTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause);
	void OnActiveLevelCopyTabChanged(TSharedRef<SDockTab> inTab, ETabActivationCause inCause);	
};
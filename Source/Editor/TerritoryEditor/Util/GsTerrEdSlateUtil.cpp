// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerrEdSlateUtil.h"
#include "STextBlock.h"
#include "ModuleManager.h"
#include "LevelEditor.h"
#include "SceneOutliner/Public/SSceneOutliner.h"
#include "SceneOutliner/Public/ISceneOutliner.h"
#include "SSceneOutliner.h"
#include "ILevelEditor.h"
#include "Private/SceneOutlinerSettings.h"
#include "ICustomSceneOutliner.h"

TSharedPtr<SHorizontalBox> FGsTerrEdSlateUtil::CreateFieldText(const FString& inFieldName, FMargin inPadding)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.AutoWidth()
		.Padding(inPadding)
		[
			SNew(STextBlock).Text(FText::FromString(inFieldName))
		];
}

void FGsTerrEdSlateUtil::SetShowOnlySelectActor(bool isActive)
{
	TWeakPtr<class ILevelEditor> levelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor")).GetLevelEditorInstance();
	if (false == levelEditor.IsValid())
	{
		return;
	}
	
	TSharedPtr<class ISceneOutliner> sceneOutlinerPtr = levelEditor.Pin()->GetSceneOutliner();
	if (false == sceneOutlinerPtr.IsValid())
	{
		return;
	}

	SceneOutliner::SSceneOutliner* outliner = static_cast<SceneOutliner::SSceneOutliner*>(sceneOutlinerPtr.Get());
	if (nullptr == outliner)
	{
		return;
	}
	
	outliner->SetShownOnlySelected(isActive);
}

bool FGsTerrEdSlateUtil::IsShowOnlySelectActor()
{
	TWeakPtr<class ILevelEditor> levelEditor = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor")).GetLevelEditorInstance();
	if (false == levelEditor.IsValid())
	{
		return false;
	}

	TSharedPtr<class ISceneOutliner> sceneOutlinerPtr = levelEditor.Pin()->GetSceneOutliner();
	if (false == sceneOutlinerPtr.IsValid())
	{
		return false;
	}

	ICustomSceneOutliner* outliner = static_cast<ICustomSceneOutliner*>(sceneOutlinerPtr.Get());
	if (nullptr == outliner)
	{
		return false;
	}

	return outliner->IsShowingOnlySelected();
}

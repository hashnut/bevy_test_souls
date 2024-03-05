// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdRecoveryCompoundWidget.h"
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

#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdRecoveryCompoundWidget::Construct(const FArguments& inArgs)
{
	_height = 25.f;
	_descriptionColumnSize = 800;
	_excuteColumnSize = 400;

	//"Recovery mesh using TID"
	TSharedPtr<SWidget> recoveryNpcTidFromMeshWidget = CreateRecoveryWidget(TEXT("Recovery mesh using TID")
		, FOnClicked::CreateLambda([]()->FReply {
			UGsTerrEdRecoveryUtil::RecoveryNpcTidFromMesh();
		return FReply::Handled();
	}));
	//"Recovery territory actor label"
	TSharedPtr<SWidget> recoveryTerritoryActorsLabelWidget = CreateRecoveryWidget(TEXT("Recovery territory actor label")
		, FOnClicked::CreateLambda([]()->FReply {
		UGsTerrEdRecoveryUtil::RecoveryTerritoryActorsLabel();
		return FReply::Handled();
	}));
	//"Resolve multi unique key"
	TSharedPtr<SWidget> resolveMultiUniqueIDWidget = CreateRecoveryWidget(TEXT("Resolve multi unique key")
		, FOnClicked::CreateLambda([]()->FReply {
		UGsTerrEdRecoveryUtil::ResolveMultiUniqueID();
		return FReply::Handled();
	}));
	//"Recovery RTT data from territory data table"
	TSharedPtr<SWidget> recoveryRTTFromTerritoryDataTableWidget = CreateRecoveryWidget(TEXT("Recovery RTT data from territory data table")
		, FOnClicked::CreateLambda([]()->FReply {
			UGsTerrEdRecoveryUtil::RecoveryRTTFromTerritoryDataTable();
		return FReply::Handled();
	}));
	//"Recovery warp source index"
	TSharedPtr<SWidget> recoveryWarpSourceIndexWidget = CreateRecoveryWidget(TEXT("Recovery warp source index")
		, FOnClicked::CreateLambda([]()->FReply {
			UGsTerrEdRecoveryUtil::RecoveryWarpSourceIndex();
			return FReply::Handled();
			}));

	ChildSlot
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			SNew(SHeaderRow)
			+ SHeaderRow::Column(TEXT("Description"))
		.FillWidth(0.8f)
		.DefaultLabel(LOCTEXT("Description", "Description"))

		+ SHeaderRow::Column(TEXT("Excute"))
		.FillWidth(0.2f)
		.DefaultLabel(LOCTEXT("Excute", "Excute"))
		]
		//npc id range column
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				recoveryNpcTidFromMeshWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				recoveryTerritoryActorsLabelWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				resolveMultiUniqueIDWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				recoveryRTTFromTerritoryDataTableWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				recoveryWarpSourceIndexWidget.ToSharedRef()
			]
	];
}

TSharedRef<SDockTab> SGsTerrEdRecoveryCompoundWidget::HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
	, const FName inTabIdentifier)
{
	TSharedRef<SDockTab> tab = SNew(SDockTab).TabRole(ETabRole::PanelTab);

	return tab;
}

TSharedPtr<SWidget> SGsTerrEdRecoveryCompoundWidget::CreateRecoveryWidget(FString inDescription, FOnClicked InOnClicked)
{
	FMargin padding;

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)
		+ SSplitter::Slot()
		.Value(_descriptionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.AutoWidth()
			.Padding(padding)
			[
				SNew(STextBlock).Text(FText::FromString(inDescription))
			]
			
		]
		+ SSplitter::Slot()
		.Value(_excuteColumnSize)
		[			
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.AutoWidth()
			.MaxWidth(400)
			[
				SNew(SButton)
				.Text(LOCTEXT("Excute", "Excute"))
			.OnClicked(InOnClicked)
			]
		];
}

#undef LOCTEXT_NAMESPACE
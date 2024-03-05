// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdExportCompoundWidget.h"
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
#include "Editor/TerritoryEditor/Util/GsTerritoryEditorFileUtil.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdExportCompoundWidget::Construct(const FArguments& inArgs)
{
	_height = 25.f;
	_descriptionColumnSize = 800;
	_excuteColumnSize = 400;

	//Save manual territory table success
	TSharedPtr<SWidget> exportWorldmapMonsterData = CreateUtilWidget(TEXT("Working with tables after build")
		, FOnClicked::CreateLambda([]()->FReply {

			FTerritoryEditor::GetInstance()->GetDataManager()->SaveManually();
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Save table"));

		return FReply::Handled();
	}));
	
	//Register server level to uncook success
	TSharedPtr<SWidget> registerServerLevelToUncook = CreateUtilWidget(TEXT("Register server level to uncook")
		, FOnClicked::CreateLambda([]()->FReply {

			UGsTerritoryEditorFileUtil::RegisterServerLevelDirectorysToNeverCook();
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Register server level to uncook success"));

			return FReply::Handled();
			}));

	//Register level to pakage
	TSharedPtr<SWidget> registerLevelToPakage = CreateUtilWidget(TEXT("Register level to pakage")
		, FOnClicked::CreateLambda([]()->FReply {

			UGsTerritoryEditorFileUtil::RegisterLevelDirectorysToPakage();
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Register level to pakage success"));

			return FReply::Handled();
			}));

	//Save force
	TSharedPtr<SWidget> saveForce = CreateUtilWidget(TEXT("Save force")
		, FOnClicked::CreateLambda([]()->FReply {

			if (FTerritoryEditor::GetInstance()->SaveForce())
			{
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Save force success"));
			}
			else
			{
				FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Save force fail"));
			}			

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
				exportWorldmapMonsterData.ToSharedRef()
			]		
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				registerServerLevelToUncook.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				registerLevelToPakage.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				saveForce.ToSharedRef()
			]
	];
}

TSharedRef<SDockTab> SGsTerrEdExportCompoundWidget::HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
	, const FName inTabIdentifier)
{
	TSharedRef<SDockTab> tab = SNew(SDockTab).TabRole(ETabRole::PanelTab);

	return tab;
}

TSharedPtr<SWidget> SGsTerrEdExportCompoundWidget::CreateUtilWidget(FString inDescription, FOnClicked InOnClicked)
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
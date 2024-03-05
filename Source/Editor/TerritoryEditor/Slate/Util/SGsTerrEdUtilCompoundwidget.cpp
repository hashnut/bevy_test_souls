// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdUtilCompoundWidget.h"
#include "LogMacros.h"
#include "TabManager.h"
#include "SHeaderRow.h"
#include "EditorStyleSet.h"
#include "STextBlock.h"
#include "SButton.h"
#include "Reply.h"
#include "MessageDialog.h"

#include "TerritoryEditor.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "Data/GsTerritoryDataManager.h"
#include "Util/GsTerrEdRecoveryUtil.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Util/GsTerritoryEditorFileUtil.h"
#include "FileHelper.h"
#include "GsTerritoryLevelManager.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdUtilCompoundWidget::Construct(const FArguments& inArgs)
{
	_height = 25.f;
	_descriptionColumnSize = 800;
	_excuteColumnSize = 400;

	//Set actor location randomly
	TSharedPtr<SWidget> setActorLocationRandomWidget = CreateUtilWidget(TEXT("Set all actor location randomly")
		, FOnClicked::CreateLambda([]()->FReply {
		UGsTerrEdRecoveryUtil::SetAllNpcRandomLocation();
		return FReply::Handled();
	}));
	//Delete all actors
	TSharedPtr<SWidget> deleteAllTerritoryActorWidget = CreateUtilWidget(TEXT("Delete all territory actor")
		, FOnClicked::CreateLambda([]()->FReply {
		FTerritoryEditor::GetInstance()->GetLevelManager()->DeleteAllTerritoryActor();		
		return FReply::Handled();
	}));
	//Check all actor is on ground
	TSharedPtr<SWidget> checkAllActorIsOnGroundWidget = CreateUtilWidget(TEXT("Check all actor is on gorund")
		, FOnClicked::CreateLambda([]()->FReply {

		TArray<AGsTerritorySpawnActor*> spawnList;
		if (false == UGsTerritoryLevelUtil::CheckAllSpawnActorCanSpawnOnNavmesh(spawnList))
		{
			FString spawnListText;
			for (AGsTerritorySpawnActor* actor : spawnList)
			{
				if (actor)
				{
					spawnListText += (actor->GetActorLabel() + TEXT("\n"));
				}
			}

			FText text = FText::FromString(spawnListText);
			FMessageDialog::Open(EAppMsgType::Ok, text);
		}
		else
		{
			FText text = FText::FromString(TEXT("All actor on navmesh"));
			FMessageDialog::Open(EAppMsgType::Ok, text);
		}

		return FReply::Handled();
	}));
	//Save all map
	TSharedPtr<SWidget> saveAllServerMapsWidget = CreateUtilWidget(TEXT("Save all server maps")
		, FOnClicked::CreateLambda([]()->FReply {

			FText titleText = FText::FromString("Save all server maps");
			FText messageText = FText::FromString("Save all server maps");
			EAppReturnType::Type returnType = FMessageDialog::Open(EAppMsgType::YesNo, EAppReturnType::Cancel, messageText, &titleText);
			if (returnType == EAppReturnType::Yes)
			{
				FTerritoryEditor::GetInstance()->GetLevelManager()->NavigationAllServerLevel();
			}		

		return FReply::Handled();
	}));
	//Save all map
	TSharedPtr<SWidget> saveAllDesignMapsWidget = CreateUtilWidget(TEXT("Save all design maps")
		, FOnClicked::CreateLambda([]()->FReply {

			FText titleText = FText::FromString("Save all design maps");
			FText messageText = FText::FromString("Save all design maps");
			EAppReturnType::Type returnType = FMessageDialog::Open(EAppMsgType::YesNo, EAppReturnType::Cancel, messageText, &titleText);
			if (returnType == EAppReturnType::Yes)
			{
				FTerritoryEditor::GetInstance()->GetLevelManager()->NavigationAllDesignLevel();
			}

			return FReply::Handled();
			}));
	//Find dummy npc
	TSharedPtr<SWidget> findDummyNpcWidget = CreateUtilWidget(TEXT("Find Dummy Npc")
		, FOnClicked::CreateLambda([]()->FReply {

			FText titleText = FText::FromString("Find Dummy Npc");
			FText messageText = FText::FromString("Find Dummy Npc");
			EAppReturnType::Type returnType = FMessageDialog::Open(EAppMsgType::YesNo, EAppReturnType::Cancel, titleText, &messageText);
			if (returnType == EAppReturnType::Yes)
			{
				UGsDataTable* npcDummyTable = UGsTerritoryEditorFileUtil::FindNpcDummyData();
				FString fullPath = FPaths::ProjectDir() + TEXT("Saved/DataCenter/Npc/NcpMapData.csv");
				FString relativePath = IFileManager::Get().ConvertToRelativePath(*fullPath);
				FPaths::NormalizeFilename(relativePath);
				FFileHelper::SaveStringToFile(npcDummyTable->GetTableAsCSV(), *relativePath);

				FText okTitleText = FText::FromString("Find Dummy Npc");
				FText okMessageText = FText::FromString("Find success");
				FMessageDialog::Open(EAppMsgType::Ok, EAppReturnType::Ok, okTitleText, &okMessageText);
			}			

			return FReply::Handled();
			}));
	//Convert mapData to MapTerritory
	TSharedPtr<SWidget> convertMapTerritory = CreateUtilWidget(TEXT("Convert MapTerritroy")
		, FOnClicked::CreateLambda([]()->FReply {

			FText titleText = FText::FromString("Convert MapTerritroy");
			FText messageText = FText::FromString("Convert MapTerritroy");
			EAppReturnType::Type returnType = FMessageDialog::Open(EAppMsgType::YesNo, EAppReturnType::Cancel, titleText, &messageText);
			if (returnType == EAppReturnType::Yes)
			{
				UGsTerritoryEditorFileUtil::ConvertMapTerritory();
				
				FText okTitleText = FText::FromString("Convert MapTerritroy");
				FText okMessageText = FText::FromString("Convert MapTerritroy Success");
				FMessageDialog::Open(EAppMsgType::Ok, EAppReturnType::Ok, okTitleText, &okMessageText);
			}

			return FReply::Handled();
			}));
	//Convert RTT to RTTList
	TSharedPtr<SWidget> convertRTTtoRTTListWidget = CreateUtilWidget(TEXT("Convert RTT to RTTTList")
		, FOnClicked::CreateLambda([]()->FReply {

			FText titleText = FText::FromString("Convert RTT to RTTTList");
			FText messageText = FText::FromString("Convert RTT to RTTTList");
			EAppReturnType::Type returnType = FMessageDialog::Open(EAppMsgType::YesNo, EAppReturnType::Cancel, messageText, &titleText);
			if (returnType == EAppReturnType::Yes)
			{
				UGsTerritoryEditorFileUtil::ConvertRTTtoRTTList();
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
				setActorLocationRandomWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				deleteAllTerritoryActorWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				checkAllActorIsOnGroundWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				saveAllServerMapsWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				saveAllDesignMapsWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				findDummyNpcWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				convertMapTerritory.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				convertRTTtoRTTListWidget.ToSharedRef()
			]		
	];
}

TSharedRef<SDockTab> SGsTerrEdUtilCompoundWidget::HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
	, const FName inTabIdentifier)
{
	TSharedRef<SDockTab> tab = SNew(SDockTab).TabRole(ETabRole::PanelTab);

	return tab;
}

TSharedPtr<SWidget> SGsTerrEdUtilCompoundWidget::CreateUtilWidget(FString inDescription, FOnClicked InOnClicked)
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
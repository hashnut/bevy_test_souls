// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdToolLevelCopyCompoundWidget.h"
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
#include "TerritoryEditor/Util/GsTerrEdSlateUtil.h"
#include "TerritoryEditor/Blutility/Input/GsTerritorySpawnActorInput.h"
#include "TerritoryEditor/Data/GsTerrEdWorldMapMonsterData.h"
#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdToolLevelCopyCompoundWidget::Construct(const FArguments& inArgs)
{
	_sourceServerLevelSearchableComboBox.Initialize();
	_destServerLevelSearchableComboBox.Initialize();	

	_isSpawnActorCopy = true;
	_isSpotCopy = true;
	_isFenceCopy = true;
	_isLineCopy = true;

	_sourceServerLevelSearchableComboBox._selectedString = TEXT("No id in table");
	_destServerLevelSearchableComboBox._selectedString = TEXT("No id in table");
	_sourceServerLevelKey = INDEX_NONE;
	_destServerLevelKey = INDEX_NONE;

	_fieldLeftColumnSize = 100;
	_fieldRightColumnSize = 800;
	_conditionColumnSize = 100;
	_height = 25.f;
	_descriptionColumnSize = 800;
	_excuteColumnSize = 400;
	_padding = 0;
	_fieldLeftRowPadding = FMargin(0.0f, 2.5f, 2.0f, 2.5f);

	//Init npc id list
	InitServerLevelList();

	TSharedPtr<SWidget> sourceServerLevelWidget = CreateSourceServerLevelWidget();
	TSharedPtr<SWidget> destServerLevelWidget = CreateDestServerLevelWidget();
	TSharedPtr<SWidget> spawnActorCopyWidget = CreateSpawnActorCopyWidget();
	TSharedPtr<SWidget> spotCpoyWidget = CreateSpotCopyWidget();
	TSharedPtr<SWidget> fenceCopyWidget = CreateFenceCopyWidget();	
	TSharedPtr<SWidget> lineCopyWidget = CreateLineCopyWidget();

	ChildSlot
		[
			//Header
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				SNew(SHeaderRow)
				+ SHeaderRow::Column(TEXT("Field"))
				.FillWidth(0.2f)
				.DefaultLabel(LOCTEXT("Field", "Field"))

				+ SHeaderRow::Column(TEXT("Value"))
				.FillWidth(0.6f)
				.DefaultLabel(LOCTEXT("Value", "Value"))
			]
			//npc id range column
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				sourceServerLevelWidget.ToSharedRef()
			]
			//tid column
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				destServerLevelWidget.ToSharedRef()
			]			
			//max spawn num
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				spawnActorCopyWidget.ToSharedRef()
			]
			//init spawn delay
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				spotCpoyWidget.ToSharedRef()
			]	
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				fenceCopyWidget.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				lineCopyWidget.ToSharedRef()
			]
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Right)
					.MaxWidth(400)
					[
						SNew(SButton)
						.Text(LOCTEXT("Copy", "Copy"))
						.OnClicked(this, &SGsTerrEdToolLevelCopyCompoundWidget::OnCopyLevel)
					]
			]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelCopyCompoundWidget::CreateSourceServerLevelWidget()
{
	TSharedPtr<SWidget> fieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Source server level"));	
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			fieldName.ToSharedRef()
		]

		+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_sourceServerLevelSearchableComboBox._searchableComboBox, SSearchableComboBox)
				.OptionsSource(&_serverLevelIdList)
				.OnSelectionChanged(this, &SGsTerrEdToolLevelCopyCompoundWidget::OnSourceServerLevelSelectionChanged)
				.OnGenerateWidget(this, &SGsTerrEdToolLevelCopyCompoundWidget::OnGenerateSearchBoxWidget)
				.InitiallySelectedItem(_sourceServerLevelSearchableComboBox._selectStringPtr)
				.Content()
				[
					SNew(STextBlock).Text(this, &SGsTerrEdToolLevelCopyCompoundWidget::GetSelectSourceServerIdText)
				]
			]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelCopyCompoundWidget::CreateDestServerLevelWidget()
{
	TSharedPtr<SWidget> fieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Dest server level"));
	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			fieldName.ToSharedRef()
		]

		+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_destServerLevelSearchableComboBox._searchableComboBox, SSearchableComboBox)
				.OptionsSource(&_serverLevelIdList)
				.OnSelectionChanged(this, &SGsTerrEdToolLevelCopyCompoundWidget::OnDestServerLevelSelectionChanged)
				.OnGenerateWidget(this, &SGsTerrEdToolLevelCopyCompoundWidget::OnGenerateSearchBoxWidget)
				.InitiallySelectedItem(_destServerLevelSearchableComboBox._selectStringPtr)
				.Content()
				[
					SNew(STextBlock).Text(this, &SGsTerrEdToolLevelCopyCompoundWidget::GetSelectDestServerIdText)
				]
			]	
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelCopyCompoundWidget::CreateSpawnActorCopyWidget()
{
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
			.Padding(_padding)
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Spawn actor copy")))
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
				SNew(SCheckBox)
				.IsChecked_Lambda([this](void)->ECheckBoxState {return (_isSpawnActorCopy == true ? ECheckBoxState::Checked : ECheckBoxState::Unchecked); })
				.OnCheckStateChanged_Lambda([this](ECheckBoxState inState) {OnSpawnActorCopyCheckChanged(inState); })
			]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelCopyCompoundWidget::CreateSpotCopyWidget()
{
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
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Spot copy")))
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
			SNew(SCheckBox)
			.IsChecked_Lambda([this](void)->ECheckBoxState {return (_isSpotCopy == true ? ECheckBoxState::Checked : ECheckBoxState::Unchecked); })
		.OnCheckStateChanged_Lambda([this](ECheckBoxState inState) {OnSpotCopyCheckChanged(inState); })
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelCopyCompoundWidget::CreateFenceCopyWidget()
{
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
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Fence copy")))
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
			SNew(SCheckBox)
			.IsChecked_Lambda([this](void)->ECheckBoxState {return (_isFenceCopy == true ? ECheckBoxState::Checked : ECheckBoxState::Unchecked); })
		.OnCheckStateChanged_Lambda([this](ECheckBoxState inState) {OnFenceCopyCheckChanged(inState); })
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdToolLevelCopyCompoundWidget::CreateLineCopyWidget()
{
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
		.Padding(_padding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Line copy")))
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
			SNew(SCheckBox)
			.IsChecked_Lambda([this](void)->ECheckBoxState {return (_isLineCopy ? ECheckBoxState::Checked : ECheckBoxState::Unchecked); })
		.OnCheckStateChanged_Lambda([this](ECheckBoxState inState) {OnLineCopyCheckChanged(inState); })
		]
		];
}

void SGsTerrEdToolLevelCopyCompoundWidget::InitServerLevelList()
{
	const UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaMapData::StaticStruct());
	TArray<const FGsSchemaMapData*> mapDatas;
	table->GetAllRows<FGsSchemaMapData>(mapDatas);
	TArray<FGsTerritoryNpcIDInfo> levelIDArray;	

	//add sort id
	for (const FGsSchemaMapData* data : mapDatas)
	{
		FGsTerritoryNpcIDInfo temp;
		temp.id = data->id;
		temp.desc = data->nameText;

		if (data->id == BEGIN_LEVEL_KEY
			|| data->id == LAUNCHER_LEVEL_KEY)
		{
			continue;
		}

		levelIDArray.Add(temp);
	}

	levelIDArray.Sort([=](const FGsTerritoryNpcIDInfo& source, const FGsTerritoryNpcIDInfo& dest)->bool {
		return source.id < dest.id;
		});

	//add npc id 
	_serverLevelIdList.Empty();
	for (const FGsTerritoryNpcIDInfo& data : levelIDArray)
	{
		FString temp = FString::FromInt(data.id) + TEXT("-") + data.desc;
		_serverLevelIdList.Add(MakeShareable(new FString(temp)));
	}
}

FText SGsTerrEdToolLevelCopyCompoundWidget::GetSelectSourceServerIdText() const
{
	return FText::FromString(*_sourceServerLevelSearchableComboBox._selectedString);
}

FText SGsTerrEdToolLevelCopyCompoundWidget::GetSelectDestServerIdText() const
{
	return FText::FromString(*_destServerLevelSearchableComboBox._selectedString);
}

void SGsTerrEdToolLevelCopyCompoundWidget::OnSourceServerLevelSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		FString value = *InItem.Get();
		_sourceServerLevelSearchableComboBox._selectedString = value;
		_sourceServerLevelKey = GetLevelId(value);
		
		UE_LOG(LogTemp, Log, TEXT("Change source level : %d"), _sourceServerLevelKey);
	}
}

void SGsTerrEdToolLevelCopyCompoundWidget::OnDestServerLevelSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		FString value = *InItem.Get();
		_destServerLevelSearchableComboBox._selectedString = value;
		_destServerLevelKey = GetLevelId(value);

		UE_LOG(LogTemp, Log, TEXT("Change dest level : %d"), _destServerLevelKey);
	}
}

TSharedRef<SWidget> SGsTerrEdToolLevelCopyCompoundWidget::OnGenerateSearchBoxWidget(TSharedPtr<FString> inItem)
{
	return SNew(STextBlock).Text(FText::FromString(*inItem)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void SGsTerrEdToolLevelCopyCompoundWidget::OnSpawnActorCopyCheckChanged(ECheckBoxState inState)
{
	_isSpawnActorCopy = (ECheckBoxState::Checked == inState ? true : false);
}

void SGsTerrEdToolLevelCopyCompoundWidget::OnSpotCopyCheckChanged(ECheckBoxState inState)
{
	_isSpotCopy = (ECheckBoxState::Checked == inState ? true : false);
}

void SGsTerrEdToolLevelCopyCompoundWidget::OnFenceCopyCheckChanged(ECheckBoxState inState)
{
	_isFenceCopy = (ECheckBoxState::Checked == inState ? true : false);
}

void SGsTerrEdToolLevelCopyCompoundWidget::OnLineCopyCheckChanged(ECheckBoxState inState)
{
	_isLineCopy = (ECheckBoxState::Checked == inState ? true : false);
}

FReply SGsTerrEdToolLevelCopyCompoundWidget::OnCopyLevel()
{
	FString message;

	const UGsTableManagerEditor& tableManager = UGsTableManagerEditor::GetInstance();
	const UGsTable* mapTable = tableManager.GetTable(FGsSchemaMapData::StaticStruct());
	const UGsTableMapData* castMapTable = Cast<UGsTableMapData>(mapTable);
	const FGsSchemaMapData* sourceMapData = nullptr;
	const FGsSchemaMapData* destMapData = nullptr;
	if (false == castMapTable->FindRowById(_sourceServerLevelKey, sourceMapData))
	{
		message = FString::Printf(TEXT("Invalid source server level"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(message));
		return FReply::Handled();
	}

	if (false == castMapTable->FindRowById(_destServerLevelKey, destMapData))
	{
		message = FString::Printf(TEXT("Invalid dest server level"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(message));
		return FReply::Handled();
	}

	message = FString::Printf(TEXT("Do you want to copy %d level to %d level?"), _sourceServerLevelKey, _destServerLevelKey);	
	if (EAppReturnType::Ok == FMessageDialog::Open(EAppMsgType::OkCancel, FText::FromString(message)))
	{
		if (FGsTerritoryLevelManager * levelManager = FTerritoryEditor::GetInstance()->GetLevelManager())
		{			
			levelManager->CopyLevel(_sourceServerLevelKey, _destServerLevelKey, _isSpawnActorCopy, _isSpotCopy, _isFenceCopy, _isLineCopy);
		}
	}

	return FReply::Handled();
}

int32 SGsTerrEdToolLevelCopyCompoundWidget::GetLevelId(const FString& inServerId)
{
	FString levelId = inServerId;
	TArray<FString> tIdParses;
	levelId.ParseIntoArray(tIdParses, TEXT("-"), true);
	if (tIdParses.Num() > 0)
	{
		return  FCString::Atoi(*tIdParses[0]);
	}

	return 0;
}

#undef LOCTEXT_NAMESPACE
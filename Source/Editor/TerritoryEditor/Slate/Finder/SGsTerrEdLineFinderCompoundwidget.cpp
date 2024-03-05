// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdLineFinderCompoundWidget.h"
#include "Util/GsTerrEdSlateUtil.h"
#include "Actor/Line/GsTerritoryLine.h"
#include "Public/DetailLayoutBuilder.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "TerritoryEditor.h"
#include "UTIL/GsText.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdLineFinderCompoundWidget::Construct(const FArguments& InArgs)
{
	//layout
	_fieldLeftColumnSize = 100;
	_fieldRightColumnSize = 300;
	_conditionColumnSize = 100;
	_height = 25.0f;
	_fieldLeftRowPadding = FMargin(0.0f, 2.5f, 2.0f, 2.5f);

	//initialize condition
	_lineTypeComboBox.Initialize();	
	_lineShapeComboBox.Initialize();	

	//actor list
	_findActorListView.Initialize();

	_selectCount = 0;
	_totalCount = 0;

	_isCheckOnlyShowSelectActor = FGsTerrEdSlateUtil::IsShowOnlySelectActor();

	TSharedPtr<SWidget> lineTypeComboBoxWidget = CreateLineTypeComboBoxWidget();
	TSharedPtr<SWidget> lineShapeComboBoxWidget = CreateLineShapeComboBoxWidget();
	TSharedPtr<SWidget> lineActorListViewWidget = CreateLineListViewWidget();
	TSharedPtr<SWidget> selectCountWidget = CreateSelectCountWidget();
	TSharedPtr<SWidget> totalCountWidget = CreateTotalCountWidget();

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

				+ SHeaderRow::Column(TEXT("IncludeCondition"))
				.FillWidth(0.2f)
				.DefaultLabel(LOCTEXT("IncludeCondition", "Include condition"))
			]
			//line type
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				lineTypeComboBoxWidget.ToSharedRef()
			]			
			//line shape
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				lineShapeComboBoxWidget.ToSharedRef()
			]
			//setting
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				SNew(SHeaderRow)
				+ SHeaderRow::Column(TEXT("Setting"))
				.FillWidth(0.8f)
				.DefaultLabel(LOCTEXT("Field", "Field"))
		
				+ SHeaderRow::Column(TEXT("Value"))
				.FillWidth(0.2f)
				.DefaultLabel(LOCTEXT("Value", "Value"))
			]
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[			
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Show only selected actors")))
				]
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				[
					SAssignNew(_showOnlySelectedActorCheckBox, SCheckBox)
					.IsChecked(this, &SGsTerrEdLineFinderCompoundWidget::GetOnlySelectActorSpawnCheckState)
					.OnCheckStateChanged(this, &SGsTerrEdLineFinderCompoundWidget::OnOnlySelectActorSpawnChanged)
				]				
			]
			//button
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(0.8f)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.MaxWidth(400)
				[
					SNew(SButton)
					.Text(LOCTEXT("Find", "Find"))
					.OnClicked(this, &SGsTerrEdLineFinderCompoundWidget::OnFindLine)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.2f)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.MaxWidth(400)
				[
					SNew(SButton)
					.Text(LOCTEXT("DeselectAll", "Deselect All"))
					.OnClicked(this, &SGsTerrEdLineFinderCompoundWidget::OnDelectAll)
				]
			]
			+ SVerticalBox::Slot()
				.MaxHeight(_height)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock).Text(FText::FromString(TEXT("Select All")))
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					[
						SAssignNew(_selectAllActorCheckBox, SCheckBox)
						.IsChecked(this, &SGsTerrEdLineFinderCompoundWidget::GetSelectAllActorState)
						.OnCheckStateChanged(this, &SGsTerrEdLineFinderCompoundWidget::OnSelectAllActorChanged)
					]
				]
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				SNew(SHeaderRow)
				+ SHeaderRow::Column(TEXT("Find line list"))
				.FillWidth(1.f)
				.DefaultLabel(LOCTEXT("Find line list", "Find line list"))
			]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			lineActorListViewWidget.ToSharedRef()
		]
		+ SVerticalBox::Slot()
			.AutoHeight()
			[
				totalCountWidget.ToSharedRef()
			]
		+ SVerticalBox::Slot()
			.AutoHeight()
			[
				selectCountWidget.ToSharedRef()
			]
	];
}

TSharedRef<SWidget> SGsTerrEdLineFinderCompoundWidget::OnGenerateNpcIdWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

// ============================================== line type ==============================================
TSharedPtr<SWidget> SGsTerrEdLineFinderCompoundWidget::CreateLineTypeComboBoxWidget()
{
	TSharedPtr<SWidget> lineTypeFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("line type"));
	_lineTypeComboBox._valueList = GetLineTypeList();
	_lineTypeComboBox._selectedValue = _lineTypeComboBox._valueList[0];

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			lineTypeFieldName.ToSharedRef()
		]

		+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			//min npc id searchbox
		+ SHorizontalBox::Slot()
		.Padding(_fieldLeftRowPadding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("LineType")))
		]
		+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_lineTypeComboBox._comboBox, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&_lineTypeComboBox._valueList)
		.OnSelectionChanged(this, &SGsTerrEdLineFinderCompoundWidget::OnLineTypeSelectionChanged)
		.OnGenerateWidget(this, &SGsTerrEdLineFinderCompoundWidget::OnGenerateNpcIdWidget)
		.InitiallySelectedItem(_lineTypeComboBox._selectedValue)
		.Content()
		[
			SNew(STextBlock).Text(this, &SGsTerrEdLineFinderCompoundWidget::GetSelectedLineTypeText)
		]
		]
		]
		+ SSplitter::Slot()
		.Value(_conditionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_lineTypeComboBox._checkBox, SCheckBox)
				.IsChecked(this, &SGsTerrEdLineFinderCompoundWidget::GetLineTypeConditionCheckState)
				.OnCheckStateChanged(this, &SGsTerrEdLineFinderCompoundWidget::OnLineTypeConditionCheckChanged)
			]
		];
}

TArray<TSharedPtr<FString>> SGsTerrEdLineFinderCompoundWidget::GetLineTypeList()
{
	TArray<TSharedPtr<FString>> lineTypeNameList;
	FString lineTypeName;
	for (int i = 0; i < (int32)EGsTerritoryLineType::Max; ++i)
	{
		lineTypeName = FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryLineType"), (EGsTerritoryLineType)i);
		lineTypeNameList.Add(MakeShareable(new FString(lineTypeName)));
	}

	return lineTypeNameList;
}

void SGsTerrEdLineFinderCompoundWidget::OnLineTypeSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		_lineTypeComboBox._selectedValue = InItem;
	}
}

FText SGsTerrEdLineFinderCompoundWidget::GetSelectedLineTypeText() const
{
	return FText::FromString(*_lineTypeComboBox._selectedValue.Get());
}

ECheckBoxState SGsTerrEdLineFinderCompoundWidget::GetLineTypeConditionCheckState() const
{
	return (_lineTypeComboBox._isCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdLineFinderCompoundWidget::OnLineTypeConditionCheckChanged(ECheckBoxState inState)
{
	_lineTypeComboBox._isCheck = (inState == ECheckBoxState::Checked ? true : false);
}

bool SGsTerrEdLineFinderCompoundWidget::IsValidLineType(class AGsTerritoryLine* inLine)
{
	if (_lineTypeComboBox._isCheck)
	{
		FString lineType = FGsTextUtil::GetStringFromEnum<EGsTerritoryLineType>(TEXT("EGsTerritoryLineType"), inLine->GetLineType());
		if (_lineTypeComboBox._selectedValue.IsValid())
		{
			return (*_lineTypeComboBox._selectedValue).Equals(lineType);
		}		
	}

	return true;	
}

//============================================ line shape ========================================================
TSharedPtr<SWidget> SGsTerrEdLineFinderCompoundWidget::CreateLineShapeComboBoxWidget()
{
	TSharedPtr<SWidget> shapeTypeFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("line Shape type"));
	_lineShapeComboBox._valueList = GetLineShapeList();
	_lineShapeComboBox._selectedValue = _lineShapeComboBox._valueList[0];

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			shapeTypeFieldName.ToSharedRef()
		]

	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			//min npc id searchbox
		+ SHorizontalBox::Slot()
		.Padding(_fieldLeftRowPadding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("LineShape")))
		]
	+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_lineTypeComboBox._comboBox, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&_lineShapeComboBox._valueList)
		.OnSelectionChanged(this, &SGsTerrEdLineFinderCompoundWidget::OnLineShapeSelectionChanged)
		.OnGenerateWidget(this, &SGsTerrEdLineFinderCompoundWidget::OnGenerateNpcIdWidget)
		.InitiallySelectedItem(_lineShapeComboBox._selectedValue)
		.Content()
		[
			SNew(STextBlock).Text(this, &SGsTerrEdLineFinderCompoundWidget::GetSelectedLineShapeText)
		]
		]
		]
	+ SSplitter::Slot()
		.Value(_conditionColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_lineShapeComboBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdLineFinderCompoundWidget::GetLineShapeConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdLineFinderCompoundWidget::OnLineShapeConditionCheckChanged)
		]
		];
}

TArray<TSharedPtr<FString>> SGsTerrEdLineFinderCompoundWidget::GetLineShapeList()
{
	TArray<TSharedPtr<FString>> lineShapeTypeNameList;
	FString lineShapeTypeName;
	for (int i = 0; i < (int32)EGsTerritoryShapeType::Max; ++i)
	{
		lineShapeTypeName = FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryShapeType"), (EGsTerritoryShapeType)i);
		lineShapeTypeNameList.Add(MakeShareable(new FString(lineShapeTypeName)));
	}

	return lineShapeTypeNameList;
}

void SGsTerrEdLineFinderCompoundWidget::OnLineShapeSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		_lineShapeComboBox._selectedValue = InItem;
	}
}

FText SGsTerrEdLineFinderCompoundWidget::GetSelectedLineShapeText() const
{
	return FText::FromString(*_lineShapeComboBox._selectedValue.Get());
}

ECheckBoxState SGsTerrEdLineFinderCompoundWidget::GetLineShapeConditionCheckState() const
{
	return (_lineShapeComboBox._isCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdLineFinderCompoundWidget::OnLineShapeConditionCheckChanged(ECheckBoxState inState)
{
	_lineShapeComboBox._isCheck = (inState == ECheckBoxState::Checked ? true : false);
}

bool SGsTerrEdLineFinderCompoundWidget::IsValidLineShape(class AGsTerritoryLine* inLine)
{
	if (_lineShapeComboBox._isCheck)
	{
		FString shapeType = FGsTextUtil::GetStringFromEnum<EGsTerritoryShapeType>(TEXT("EGsTerritoryShapeType"), inLine->GetShapeType());
		if (_lineShapeComboBox._selectedValue.IsValid())
		{
			return (*_lineShapeComboBox._selectedValue).Equals(shapeType);
		}
	}

	return true;
}

//============================================ setting ========================================================
ECheckBoxState SGsTerrEdLineFinderCompoundWidget::GetOnlySelectActorSpawnCheckState() const
{
	return FGsTerrEdSlateUtil::IsShowOnlySelectActor() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdLineFinderCompoundWidget::OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const
{
	bool isCheckted = (ECheckBoxState::Checked == inState);
	
	FGsTerrEdSlateUtil::SetShowOnlySelectActor(isCheckted);
}

ECheckBoxState SGsTerrEdLineFinderCompoundWidget::GetSelectAllActorState() const
{
	return _isCheckSelectAllActor ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdLineFinderCompoundWidget::OnSelectAllActorChanged(ECheckBoxState inState)
{
	_isCheckSelectAllActor = (ECheckBoxState::Checked == inState);

	SetSelectAllActor(_isCheckSelectAllActor);
}

void SGsTerrEdLineFinderCompoundWidget::SetSelectAllActor(bool inSelectAll)
{
	if (inSelectAll)
	{
		//모든 npc id 선택						
		_findActorListView._listView->SetItemSelection(_findActorListView._valueList, true);
		_findActorListView._listView->RequestListRefresh();
	}
	else
	{
		_findActorListView._listView->SetItemSelection(_findActorListView._valueList, false);
		_findActorListView._listView->RequestListRefresh();
	}
}

//========================================== list view ========================================================
TSharedPtr<SWidget> SGsTerrEdLineFinderCompoundWidget::CreateLineListViewWidget()
{
	return SNew(SBox)
		.Padding(FMargin(4.0f, 4.0f))
		.HeightOverride(300.0f)
		[
			SAssignNew(_findActorListView._listView, SListView<TSharedPtr<FString>>)
			.ListItemsSource(&_findActorListView._valueList) //The Items array is the source of this listview
		.OnGenerateRow(this, &SGsTerrEdLineFinderCompoundWidget::OnGenerateLineActorWidget)
		.OnSelectionChanged(this, &SGsTerrEdLineFinderCompoundWidget::OnSelectionChangeLineActor)
		.ItemHeight(24)
		.SelectionMode(ESelectionMode::Multi)
		.ScrollbarVisibility(EVisibility::All)
		];
}

void SGsTerrEdLineFinderCompoundWidget::InitLineActorList()
{
	_findActorListView._valueList.Empty();
	for (AActor* actor : _findActorListView._actorList)
	{
		_findActorListView._valueList.Add(MakeShareable(new FString(actor->GetActorLabel())));
	}
}

TSharedRef<ITableRow> SGsTerrEdLineFinderCompoundWidget::OnGenerateLineActorWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable)
{
	//Create the row
	return	SNew(STableRow< TSharedPtr<FString> >, inOwnerTable)
		.Padding(2.0f)
		[
			SNew(STextBlock).Text(FText::FromString(*inItem.Get()))
		];
}

void SGsTerrEdLineFinderCompoundWidget::OnDoubleClickLineActor(TSharedPtr<FString> InNewSelection)
{

}

void SGsTerrEdLineFinderCompoundWidget::OnSelectionChangeLineActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo)
{
	TArray<TSharedPtr<FString>> selectItems = _findActorListView._listView->GetSelectedItems();
	if (selectItems.Num() == 0)
	{
		return;
	}

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		_findActorListView._listView->ClearSelection();
		return;
	}
	
	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryLine::StaticClass(), actorList);

	UGsTerritoryLevelUtil::SetSelectedLevelActors(actorList, selectItems);

	_isCheckSelectAllActor = (selectItems.Num() == _findActorListView._valueList.Num());
	_selectAllActorCheckBox->Invalidate(EInvalidateWidgetReason::Layout);

	GUnrealEd->Exec(world, TEXT("CAMERA ALIGN ACTIVEVIEWPORTONLY"));

	_selectCount = selectItems.Num();
	if (_selectCountTextBlock.IsValid())
	{
		_selectCountTextBlock->SetText(FText::FromString(FString::FromInt(_selectCount)));
	}
}

void SGsTerrEdLineFinderCompoundWidget::OnFieldColumnResized(const float inWidth)
{
	_fieldColumnSize = inWidth;

	UE_LOG(LogTerritoryEditor, Log, TEXT("Set field width : %d"), _fieldColumnSize);
}

void SGsTerrEdLineFinderCompoundWidget::OnIncludeConditionColumnResized(const float inWidth)
{
	_conditionColumnSize = inWidth;

	UE_LOG(LogTerritoryEditor, Log, TEXT("Set condition width : %d"), _conditionColumnSize);
}

FReply SGsTerrEdLineFinderCompoundWidget::OnFindLine()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();	
	if (nullptr == world)
	{
		return FReply::Handled();
	}

	if (false == CanFindLine())
	{
		return FReply::Handled();
	}

	ULevel* currentLevel = world->GetCurrentLevel();
	if (nullptr == currentLevel)
	{
		return FReply::Handled();
	}
	
	_findActorListView._actorList.Empty();
	AGsTerritoryLine* line  = nullptr;
	for (AActor* actor : currentLevel->Actors)
	{
		line = Cast<AGsTerritoryLine>(actor);
		if (line)
		{
			if (IsValidLineType(line)
			&& IsValidLineShape(line))
			{
				_findActorListView._actorList.Add(line);
			}
		}
	}

	USelection* select = GEditor->GetSelectedActors();
	if (select)
	{
		UGsTerritoryLevelUtil::SetSelectedLevelActors(_findActorListView._actorList);
	}

	InitLineActorList();
	_isCheckSelectAllActor = true;
	_selectAllActorCheckBox->Invalidate(EInvalidateWidgetReason::Layout);
	SetSelectAllActor(_isCheckSelectAllActor);

	_totalCount = _findActorListView._actorList.Num();
	if (_totalCountTextBlock.IsValid())
	{
		_totalCountTextBlock->SetText(FText::FromString(FString::FromInt(_totalCount)));
	}

	return FReply::Handled();
}

FReply SGsTerrEdLineFinderCompoundWidget::OnDelectAll()
{
	_isCheckSelectAllActor = false;
	_selectAllActorCheckBox.Get()->Invalidate(EInvalidateWidgetReason::Layout);

	SetSelectAllActor(false);

	return FReply::Handled();
}

bool SGsTerrEdLineFinderCompoundWidget::CanFindLine()
{
	return _lineTypeComboBox.IsValid()
	&& _lineShapeComboBox.IsValid();		
}


#undef LOCTEXT_NAMESPACE
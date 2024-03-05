// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdFenceFinderCompoundwidget.h"
#include "Util/GsTerrEdSlateUtil.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "UTIL/GsText.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Actor/Fence/GsTerritoryFenceBase.h"
#include "DetailLayoutBuilder.h"


#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdFenceFinderCompoundwidget::Construct(const FArguments& InArgs)
{
	//layout
	_fieldLeftColumnSize = 100;
	_fieldRightColumnSize = 300;
	_conditionColumnSize = 100;
	_height = 25.0f;
	_fieldLeftRowPadding = FMargin(0.0f, 2.5f, 2.0f, 2.5f);

	_selectCount = 0;
	_totalCount = 0;

	//initialize condition
	_uniqueIdNumericEntryBox.Initialize();
	_fenceTypeComboBox.Initialize();
	_radiusRangeNumericEntryBox.Initialize();

	_name = "";
	_isCheckNameCondition = false;

	_isCheckOnlyShowSelectActor = FGsTerrEdSlateUtil::IsShowOnlySelectActor();

	//create widget
	TSharedPtr<SWidget> uniqueIdWidget = CreateUniqueIdWidget();
	TSharedPtr<SWidget> fenceTypeWidget = CreateFenceTypeComboBoxWidget();
	TSharedPtr<SWidget> radiusWidget = CreateRadiusNumericEntryBoxWidget();
	TSharedPtr<SWidget> nameWidget = CreateNameWidget();
	TSharedPtr<SWidget> finderWidget = CreateFenceListViewWidget();
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
			//npc id range column
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				uniqueIdWidget.ToSharedRef()
			]
			//tid column
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				fenceTypeWidget.ToSharedRef()
			]
			//tid column
			+ SVerticalBox::Slot()
				.MaxHeight(_height)
				[
					radiusWidget.ToSharedRef()
				]
			//tid column
			+ SVerticalBox::Slot()
				.MaxHeight(_height)
				[
					nameWidget.ToSharedRef()
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
					.IsChecked(this, &SGsTerrEdFenceFinderCompoundwidget::GetOnlySelectActorSpawnCheckState)
					.OnCheckStateChanged(this, &SGsTerrEdFenceFinderCompoundwidget::OnOnlySelectActorSpawnChanged)
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
					.OnClicked(this, &SGsTerrEdFenceFinderCompoundwidget::OnFindFence)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.2f)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.MaxWidth(400)
				[
					SNew(SButton)
					.Text(LOCTEXT("DeselectAll", "Deselect All"))
					.OnClicked(this, &SGsTerrEdFenceFinderCompoundwidget::OnDelectAll)
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
						.IsChecked(this, &SGsTerrEdFenceFinderCompoundwidget::GetSelectAllActorState)
						.OnCheckStateChanged(this, &SGsTerrEdFenceFinderCompoundwidget::OnSelectAllActorChanged)
					]
				]
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				SNew(SHeaderRow)
				+ SHeaderRow::Column(TEXT("Find spot list"))
				.FillWidth(1.f)
				.DefaultLabel(LOCTEXT("Find spot list", "Find spot list"))
			]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			finderWidget.ToSharedRef()
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


TSharedRef<SWidget> SGsTerrEdFenceFinderCompoundwidget::OnGenerateComboWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

//=================================================== unique type =============================================================================================
TSharedPtr<SWidget> SGsTerrEdFenceFinderCompoundwidget::CreateUniqueIdWidget()
{
	TSharedPtr<SWidget> uniqueIdFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Unique id"));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			uniqueIdFieldName.ToSharedRef()
		]

	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_uniqueIdNumericEntryBox._numericEntryBox, SNumericEntryBox<int64>)
			.Value(this, &SGsTerrEdFenceFinderCompoundwidget::OnGetUniqueId)
		.OnValueCommitted(this, &SGsTerrEdFenceFinderCompoundwidget::OnSetUniqueId)
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
			SAssignNew(_uniqueIdNumericEntryBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdFenceFinderCompoundwidget::GetUniqueIdConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdFenceFinderCompoundwidget::OnUniqueIdConditionCheckChanged)
		]
		];
}

TOptional<int64> SGsTerrEdFenceFinderCompoundwidget::OnGetUniqueId() const
{
	return _uniqueIdNumericEntryBox._value;
}

void SGsTerrEdFenceFinderCompoundwidget::OnSetUniqueId(int64 inValue, ETextCommit::Type inType)
{
	_uniqueIdNumericEntryBox._value = inValue;
}

ECheckBoxState SGsTerrEdFenceFinderCompoundwidget::GetUniqueIdConditionCheckState() const
{
	return _uniqueIdNumericEntryBox.GetCheckState();
}

void SGsTerrEdFenceFinderCompoundwidget::OnUniqueIdConditionCheckChanged(ECheckBoxState inState)
{
	_uniqueIdNumericEntryBox.SetConditionCheckState(inState);
}


// ================================================== fence type ==============================================
TSharedPtr<SWidget> SGsTerrEdFenceFinderCompoundwidget::CreateFenceTypeComboBoxWidget()
{
	TSharedPtr<SWidget> fenceTypeFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Fence type"));
	_fenceTypeComboBox._valueList = GetFenceTypeList();
	_fenceTypeComboBox._selectedValue = _fenceTypeComboBox._valueList[0];

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			fenceTypeFieldName.ToSharedRef()
		]

		+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			//min npc id searchbox
			+ SHorizontalBox::Slot()
			.Padding(_fieldLeftRowPadding)
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("FenceType")))
			]
			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_fenceTypeComboBox._comboBox, SComboBox<TSharedPtr<FString>>)
				.OptionsSource(&_fenceTypeComboBox._valueList)
				.OnSelectionChanged(this, &SGsTerrEdFenceFinderCompoundwidget::OnFenceTypeSelectionChanged)
				.OnGenerateWidget(this, &SGsTerrEdFenceFinderCompoundwidget::OnGenerateComboWidget)
				.InitiallySelectedItem(_fenceTypeComboBox._selectedValue)
				.Content()
				[
					SNew(STextBlock).Text(this, &SGsTerrEdFenceFinderCompoundwidget::GetSelectedFenceTypeText)
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
				SAssignNew(_fenceTypeComboBox._checkBox, SCheckBox)
				.IsChecked(this, &SGsTerrEdFenceFinderCompoundwidget::GetFenceTypeConditionCheckState)
				.OnCheckStateChanged(this, &SGsTerrEdFenceFinderCompoundwidget::OnFenceTypeConditionCheckChanged)
			]
		];
}

TArray<TSharedPtr<FString>> SGsTerrEdFenceFinderCompoundwidget::GetFenceTypeList()
{
	TArray<TSharedPtr<FString>> typeList;
	for (int i = 0; i < (int32)TerritoryType::MAX; ++i)
	{
		FString fenceType = FGsTextUtil::GetStringFromEnum(TEXT("TerritoryType"), (TerritoryType)i);
		typeList.Add(MakeShareable(new FString(fenceType)));
	}

	return typeList;
}

void SGsTerrEdFenceFinderCompoundwidget::OnFenceTypeSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		_fenceTypeComboBox._selectedValue = InItem;
	}
}

FText SGsTerrEdFenceFinderCompoundwidget::GetSelectedFenceTypeText() const
{
	return FText::FromString(*_fenceTypeComboBox._selectedValue.Get());
}

ECheckBoxState SGsTerrEdFenceFinderCompoundwidget::GetFenceTypeConditionCheckState() const
{
	return (_fenceTypeComboBox._isCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdFenceFinderCompoundwidget::OnFenceTypeConditionCheckChanged(ECheckBoxState inState)
{
	_fenceTypeComboBox._isCheck = (inState == ECheckBoxState::Checked ? true : false);
}

bool SGsTerrEdFenceFinderCompoundwidget::IsValidUniqueId(class AGsTerritoryFenceBase* inFence)
{
	return _uniqueIdNumericEntryBox._isCheck ? inFence->GetTerritoryUniqueID() == _uniqueIdNumericEntryBox._value : true;
}

bool SGsTerrEdFenceFinderCompoundwidget::IsValidFenceType(class AGsTerritoryFenceBase* inFence)
{
	if (_fenceTypeComboBox._isCheck)
	{
		FString fenceType = FGsTextUtil::GetStringFromEnum<TerritoryType>(TEXT("TerritoryType"), inFence->GetFenceType());
		if (_fenceTypeComboBox._selectedValue.IsValid())
		{
			return (*_fenceTypeComboBox._selectedValue).Equals(fenceType);
		}		
	}

	return true;	
}

// ============================================== radius ==============================================
TSharedPtr<SWidget> SGsTerrEdFenceFinderCompoundwidget::CreateRadiusNumericEntryBoxWidget()
{
	TSharedPtr<SWidget> tidFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Radius"));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			tidFieldName.ToSharedRef()
		]

		+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.Padding(_fieldLeftRowPadding)
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Min")))
			]

			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_radiusRangeNumericEntryBox._minNumericEntryBox, SNumericEntryBox<float>)
				.Value(this, &SGsTerrEdFenceFinderCompoundwidget::OnGetMinRadius)
				.OnValueCommitted(this, &SGsTerrEdFenceFinderCompoundwidget::OnSetMinRadius)
			]

			+ SHorizontalBox::Slot()
				.Padding(_fieldLeftRowPadding)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Max")))
				]

			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_radiusRangeNumericEntryBox._maxNumericEntryBox, SNumericEntryBox<float>)
				.Value(this, &SGsTerrEdFenceFinderCompoundwidget::OnGetMaxRadius)
				.OnValueCommitted(this, &SGsTerrEdFenceFinderCompoundwidget::OnSetMaxRadius)
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
					SAssignNew(_radiusRangeNumericEntryBox._checkBox, SCheckBox)
					.IsChecked(this, &SGsTerrEdFenceFinderCompoundwidget::GetRadiusConditionCheckState)
					.OnCheckStateChanged(this, &SGsTerrEdFenceFinderCompoundwidget::OnRadiusConditionCheckChanged)
			]			
		];
}

TOptional<float> SGsTerrEdFenceFinderCompoundwidget::OnGetMinRadius() const
{
	return TOptional<float>(_radiusRangeNumericEntryBox._minValue);
}

void SGsTerrEdFenceFinderCompoundwidget::OnSetMinRadius(float inValue, ETextCommit::Type inType)
{
	_radiusRangeNumericEntryBox._minValue = inValue;
}


TOptional<float> SGsTerrEdFenceFinderCompoundwidget::OnGetMaxRadius() const
{
	return TOptional<float>(_radiusRangeNumericEntryBox._maxValue);
}

void SGsTerrEdFenceFinderCompoundwidget::OnSetMaxRadius(float inValue, ETextCommit::Type inType)
{
	_radiusRangeNumericEntryBox._maxValue = inValue;
}

ECheckBoxState SGsTerrEdFenceFinderCompoundwidget::GetRadiusConditionCheckState() const
{
	return _radiusRangeNumericEntryBox.GetCheckState();
}

void SGsTerrEdFenceFinderCompoundwidget::OnRadiusConditionCheckChanged(ECheckBoxState inState)
{
	_radiusRangeNumericEntryBox._isCheck = (inState == ECheckBoxState::Checked ? true : false);
}

bool SGsTerrEdFenceFinderCompoundwidget::IsValidRadius(AGsTerritoryFenceBase* inFence)
{
	if (_radiusRangeNumericEntryBox._isCheck)
	{
		return _radiusRangeNumericEntryBox._minValue <= inFence->_radius
			&& _radiusRangeNumericEntryBox._maxValue >= inFence->_radius;
	}
	
	return true;
}

bool SGsTerrEdFenceFinderCompoundwidget::IsValidName(class AGsTerritoryFenceBase* inFence)
{
	if (_isCheckNameCondition)
	{
		return inFence->_name.ToString().Contains(_name);
	}

	return true;
}

//============================================ name ========================================================

TSharedPtr<SWidget> SGsTerrEdFenceFinderCompoundwidget::CreateNameWidget()
{
	TSharedPtr<SWidget> noteFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Name"));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			noteFieldName.ToSharedRef()
		]
	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_nameEditableTextBox, SEditableTextBox)
			.OnTextChanged(this, &SGsTerrEdFenceFinderCompoundwidget::OnNameTextChanged)
		.OnTextCommitted(this, &SGsTerrEdFenceFinderCompoundwidget::OnNameTextCommit)
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
			SAssignNew(_nameTextCheckBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdFenceFinderCompoundwidget::GetNameConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdFenceFinderCompoundwidget::OnNameConditionCheckChanged)
		]
		];
}


void SGsTerrEdFenceFinderCompoundwidget::OnNameTextChanged(const FText& inText)
{

}

void SGsTerrEdFenceFinderCompoundwidget::OnNameTextCommit(const FText& inText, ETextCommit::Type inCommitType)
{
	if (inCommitType == ETextCommit::OnUserMovedFocus)
	{
		_name = inText.ToString();
	}
}

ECheckBoxState SGsTerrEdFenceFinderCompoundwidget::GetNameConditionCheckState() const
{
	return _isCheckNameCondition ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdFenceFinderCompoundwidget::OnNameConditionCheckChanged(ECheckBoxState inState)
{
	_isCheckNameCondition = (inState == ECheckBoxState::Checked ? true : false);
}


//============================================ setting ========================================================
ECheckBoxState SGsTerrEdFenceFinderCompoundwidget::GetOnlySelectActorSpawnCheckState() const
{
	return FGsTerrEdSlateUtil::IsShowOnlySelectActor() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdFenceFinderCompoundwidget::OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const
{
	bool isCheckted = (ECheckBoxState::Checked == inState);
	
	FGsTerrEdSlateUtil::SetShowOnlySelectActor(isCheckted);
}

ECheckBoxState SGsTerrEdFenceFinderCompoundwidget::GetSelectAllActorState() const
{
	return _isCheckSelectAllActor ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdFenceFinderCompoundwidget::OnSelectAllActorChanged(ECheckBoxState inState)
{
	_isCheckSelectAllActor = (ECheckBoxState::Checked == inState);

	SetSelectAllActor(_isCheckSelectAllActor);
}

void SGsTerrEdFenceFinderCompoundwidget::SetSelectAllActor(bool inSelectAll)
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
TSharedPtr<SWidget> SGsTerrEdFenceFinderCompoundwidget::CreateFenceListViewWidget()
{
	return SNew(SBox)
		.Padding(FMargin(4.0f, 4.0f))
		.HeightOverride(300.0f)
		[
			SAssignNew(_findActorListView._listView, SListView<TSharedPtr<FString>>)
			.ListItemsSource(&_findActorListView._valueList) //The Items array is the source of this listview
		.OnGenerateRow(this, &SGsTerrEdFenceFinderCompoundwidget::OnGenerateFenceActorWidget)
		.OnSelectionChanged(this, &SGsTerrEdFenceFinderCompoundwidget::OnSelectionChangeFenceActor)
		.ItemHeight(24)
		.SelectionMode(ESelectionMode::Multi)
		.ScrollbarVisibility(EVisibility::All)
		];
}

void SGsTerrEdFenceFinderCompoundwidget::InitFenceActorList()
{
	_findActorListView._valueList.Empty();
	for (AActor* actor : _findActorListView._actorList)
	{
		_findActorListView._valueList.Add(MakeShareable(new FString(actor->GetActorLabel())));
	}
}

TSharedRef<ITableRow> SGsTerrEdFenceFinderCompoundwidget::OnGenerateFenceActorWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable)
{
	//Create the row
	return	SNew(STableRow< TSharedPtr<FString> >, inOwnerTable)
		.Padding(2.0f)
		[
			SNew(STextBlock).Text(FText::FromString(*inItem.Get()))
		];
}

void SGsTerrEdFenceFinderCompoundwidget::OnDoubleClickFenceActor(TSharedPtr<FString> InNewSelection)
{

}

void SGsTerrEdFenceFinderCompoundwidget::OnSelectionChangeFenceActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo)
{
	TArray<TSharedPtr<FString>> selectItems = _findActorListView._listView->GetSelectedItems();
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		_findActorListView._listView->ClearSelection();
		return;
	}
	
	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryFenceBase::StaticClass(), actorList);

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

void SGsTerrEdFenceFinderCompoundwidget::OnFieldColumnResized(const float inWidth)
{
	_fieldColumnSize = inWidth;

	UE_LOG(LogTerritoryEditor, Log, TEXT("Set field width : %d"), _fieldColumnSize);
}

void SGsTerrEdFenceFinderCompoundwidget::OnIncludeConditionColumnResized(const float inWidth)
{
	_conditionColumnSize = inWidth;

	UE_LOG(LogTerritoryEditor, Log, TEXT("Set condition width : %d"), _conditionColumnSize);
}

FReply SGsTerrEdFenceFinderCompoundwidget::OnFindFence()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();	
	if (nullptr == world)
	{
		return FReply::Handled();
	}

	if (false == CanFindFence())
	{
		return FReply::Handled();
	}

	ULevel* currentLevel = world->GetCurrentLevel();
	if (nullptr == currentLevel)
	{
		return FReply::Handled();
	}
	
	_findActorListView._actorList.Empty();
	for (AActor* actor : currentLevel->Actors)
	{
		AGsTerritoryFenceBase* fence = Cast<AGsTerritoryFenceBase>(actor);
		if (fence)
		{
			if (IsValidUniqueId(fence)
				&&IsValidFenceType(fence)
				&& IsValidRadius(fence)
				&& IsValidName(fence))
			{
				_findActorListView._actorList.Add(fence);
			}
		}
	}

	USelection* select = GEditor->GetSelectedActors();
	if (select)
	{
		UGsTerritoryLevelUtil::SetSelectedLevelActors(_findActorListView._actorList);
	}

	InitFenceActorList();
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

FReply SGsTerrEdFenceFinderCompoundwidget::OnDelectAll()
{
	_isCheckSelectAllActor = false;
	_selectAllActorCheckBox.Get()->Invalidate(EInvalidateWidgetReason::Layout);

	SetSelectAllActor(false);

	return FReply::Handled();
}

bool SGsTerrEdFenceFinderCompoundwidget::CanFindFence()
{
	return _fenceTypeComboBox.IsValid()
		&& _radiusRangeNumericEntryBox.IsValid();
}

#undef LOCTEXT_NAMESPACE
// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdSpotFinderCompoundWidget.h"
#include "SScrollBox.h"
#include "STextBlock.h"
#include "SBorder.h"
#include "SButton.h"
#include "SComboBox.h"
#include "GsTableManagerEditor.h"
#include "GameObject/Npc/GsSchemaNpcData.h"
#include "LogMacros.h"
#include "Kismet/GameplayStatics.h"
#include "Editor.h"
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryNpc.h"
#include "DetailLayoutBuilder.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Util/GsTerrEdSlateUtil.h"
#include "SNumericEntryBox.h"
#include "TerritoryEditor.h"
#include "ActorEx/GsCharacterNonPlayer.h"
#include "UTIL/GsText.h"
#include "Actor/Spot/GsTerritorySpot.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdSpotFinderCompoundWidget::Construct(const FArguments& InArgs)
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
	_spotTypeComboBox.Initialize();
	_spotRadiusRangeNumericEntryBox.Initialize();
	_findActorListView.Initialize();

	_isCheckOnlyShowSelectActor = FGsTerrEdSlateUtil::IsShowOnlySelectActor();

	TSharedPtr<SWidget> spotTypeComboBoxWidget = CreateSpotTypeComboBoxWidget();
	TSharedPtr<SWidget> spotRadiusNumericEntryBoxWidget = CreateRadiusNumericEntryBoxWidget();
	TSharedPtr<SWidget> spotActorListViewWidget = CreateSpotListViewWidget();
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
				spotTypeComboBoxWidget.ToSharedRef()
			]
			//tid column
			+ SVerticalBox::Slot()
			.MaxHeight(_height)
			[
				spotRadiusNumericEntryBoxWidget.ToSharedRef()
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
					.IsChecked(this, &SGsTerrEdSpotFinderCompoundWidget::GetOnlySelectActorSpawnCheckState)
					.OnCheckStateChanged(this, &SGsTerrEdSpotFinderCompoundWidget::OnOnlySelectActorSpawnChanged)
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
					.OnClicked(this, &SGsTerrEdSpotFinderCompoundWidget::OnFindSpot)
				]
				+ SHorizontalBox::Slot()
				.FillWidth(0.2f)
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.MaxWidth(400)
				[
					SNew(SButton)
					.Text(LOCTEXT("DeselectAll", "Deselect All"))
					.OnClicked(this, &SGsTerrEdSpotFinderCompoundWidget::OnDelectAll)
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
						.IsChecked(this, &SGsTerrEdSpotFinderCompoundWidget::GetSelectAllActorState)
						.OnCheckStateChanged(this, &SGsTerrEdSpotFinderCompoundWidget::OnSelectAllActorChanged)
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
			spotActorListViewWidget.ToSharedRef()
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


TSharedRef<SWidget> SGsTerrEdSpotFinderCompoundWidget::OnGenerateNpcIdWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

// ============================================== spot type ==============================================
TSharedPtr<SWidget> SGsTerrEdSpotFinderCompoundWidget::CreateSpotTypeComboBoxWidget()
{
	TSharedPtr<SWidget> spotTypeFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("spot type"));
	_spotTypeComboBox._valueList = GetSpotTypeList();
	_spotTypeComboBox._selectedValue = _spotTypeComboBox._valueList[0];

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			spotTypeFieldName.ToSharedRef()
		]

		+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			//min npc id searchbox
		+ SHorizontalBox::Slot()
		.Padding(_fieldLeftRowPadding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("SpotType")))
		]
		+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_spotTypeComboBox._comboBox, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&_spotTypeComboBox._valueList)
		.OnSelectionChanged(this, &SGsTerrEdSpotFinderCompoundWidget::OnSpotTypeSelectionChanged)
		.OnGenerateWidget(this, &SGsTerrEdSpotFinderCompoundWidget::OnGenerateNpcIdWidget)
		.InitiallySelectedItem(_spotTypeComboBox._selectedValue)
		.Content()
		[
			SNew(STextBlock).Text(this, &SGsTerrEdSpotFinderCompoundWidget::GetSelectedSpotTypeText)
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
				SAssignNew(_spotTypeComboBox._checkBox, SCheckBox)
				.IsChecked(this, &SGsTerrEdSpotFinderCompoundWidget::GetSpotTypeConditionCheckState)
				.OnCheckStateChanged(this, &SGsTerrEdSpotFinderCompoundWidget::OnSpotTypeConditionCheckChanged)
			]
		];
}

TArray<TSharedPtr<FString>> SGsTerrEdSpotFinderCompoundWidget::GetSpotTypeList()
{
	TArray<TSharedPtr<FString>> spotTypeNameList;
	for (int i = 0; i < (int32)EGsTerritorySpotType::Max; ++i)
	{
		FString spotTypeName = FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritorySpotType"), (EGsTerritorySpotType)i);
		spotTypeNameList.Add(MakeShareable(new FString(spotTypeName)));
	}

	return spotTypeNameList;
}

void SGsTerrEdSpotFinderCompoundWidget::OnSpotTypeSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		_spotTypeComboBox._selectedValue = InItem;
	}
}

FText SGsTerrEdSpotFinderCompoundWidget::GetSelectedSpotTypeText() const
{
	return FText::FromString(*_spotTypeComboBox._selectedValue.Get());
}

ECheckBoxState SGsTerrEdSpotFinderCompoundWidget::GetSpotTypeConditionCheckState() const
{
	return (_spotTypeComboBox._isCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdSpotFinderCompoundWidget::OnSpotTypeConditionCheckChanged(ECheckBoxState inState)
{
	_spotTypeComboBox._isCheck = (inState == ECheckBoxState::Checked ? true : false);
}

bool SGsTerrEdSpotFinderCompoundWidget::IsValidSpotType(AGsTerritorySpot* inSpot)
{
	if (_spotTypeComboBox._isCheck)
	{
		FString spotType = FGsTextUtil::GetStringFromEnum<EGsTerritorySpotType>(TEXT("EGsTerritorySpotType"), inSpot->GetSpotType());
		if (_spotTypeComboBox._selectedValue.IsValid())
		{
			return (*_spotTypeComboBox._selectedValue).Equals(spotType);
		}		
	}

	return true;	
}


// ============================================== radius ==============================================
TSharedPtr<SWidget> SGsTerrEdSpotFinderCompoundWidget::CreateRadiusNumericEntryBoxWidget()
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
				SAssignNew(_spotRadiusRangeNumericEntryBox._minNumericEntryBox, SNumericEntryBox<float>)
				.Value(this, &SGsTerrEdSpotFinderCompoundWidget::OnGetMinSpotRadius)
				.OnValueCommitted(this, &SGsTerrEdSpotFinderCompoundWidget::OnSetMinSpotRadius)
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
				SAssignNew(_spotRadiusRangeNumericEntryBox._maxNumericEntryBox, SNumericEntryBox<float>)
				.Value(this, &SGsTerrEdSpotFinderCompoundWidget::OnGetMaxSpotRadius)
				.OnValueCommitted(this, &SGsTerrEdSpotFinderCompoundWidget::OnSetMaxSpotRadius)
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
					SAssignNew(_spotRadiusRangeNumericEntryBox._checkBox, SCheckBox)
					.IsChecked(this, &SGsTerrEdSpotFinderCompoundWidget::GetSpotRadiusConditionCheckState)
					.OnCheckStateChanged(this, &SGsTerrEdSpotFinderCompoundWidget::OnSpotRadiusConditionCheckChanged)
			]			
		];
}

TOptional<float> SGsTerrEdSpotFinderCompoundWidget::OnGetMinSpotRadius() const
{
	return TOptional<float>(_spotRadiusRangeNumericEntryBox._minValue);
}

void SGsTerrEdSpotFinderCompoundWidget::OnSetMinSpotRadius(float inValue, ETextCommit::Type inType)
{
	_spotRadiusRangeNumericEntryBox._minValue = inValue;
}


TOptional<float> SGsTerrEdSpotFinderCompoundWidget::OnGetMaxSpotRadius() const
{
	return TOptional<float>(_spotRadiusRangeNumericEntryBox._maxValue);
}

void SGsTerrEdSpotFinderCompoundWidget::OnSetMaxSpotRadius(float inValue, ETextCommit::Type inType)
{
	_spotRadiusRangeNumericEntryBox._maxValue = inValue;
}

ECheckBoxState SGsTerrEdSpotFinderCompoundWidget::GetSpotRadiusConditionCheckState() const
{
	return _spotRadiusRangeNumericEntryBox.GetCheckState();
}

void SGsTerrEdSpotFinderCompoundWidget::OnSpotRadiusConditionCheckChanged(ECheckBoxState inState)
{
	_spotRadiusRangeNumericEntryBox._isCheck = (inState == ECheckBoxState::Checked ? true : false);
}

bool SGsTerrEdSpotFinderCompoundWidget::IsValidSpotRadius(AGsTerritorySpot* inSpot)
{
	if (_spotRadiusRangeNumericEntryBox._isCheck)
	{
		return _spotRadiusRangeNumericEntryBox._minValue <= inSpot->_radius
			&& _spotRadiusRangeNumericEntryBox._maxValue >= inSpot->_radius;
	}
	
	return true;
}

//============================================ setting ========================================================
ECheckBoxState SGsTerrEdSpotFinderCompoundWidget::GetOnlySelectActorSpawnCheckState() const
{
	return FGsTerrEdSlateUtil::IsShowOnlySelectActor() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdSpotFinderCompoundWidget::OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const
{
	bool isCheckted = (ECheckBoxState::Checked == inState);
	
	FGsTerrEdSlateUtil::SetShowOnlySelectActor(isCheckted);
}

ECheckBoxState SGsTerrEdSpotFinderCompoundWidget::GetSelectAllActorState() const
{
	return _isCheckSelectAllActor ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdSpotFinderCompoundWidget::OnSelectAllActorChanged(ECheckBoxState inState)
{
	_isCheckSelectAllActor = (ECheckBoxState::Checked == inState);

	SetSelectAllActor(_isCheckSelectAllActor);
}

void SGsTerrEdSpotFinderCompoundWidget::SetSelectAllActor(bool inSelectAll)
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
TSharedPtr<SWidget> SGsTerrEdSpotFinderCompoundWidget::CreateSpotListViewWidget()
{
	return SNew(SBox)
		.Padding(FMargin(4.0f, 4.0f))
		.HeightOverride(300.0f)
		[
			SAssignNew(_findActorListView._listView, SListView<TSharedPtr<FString>>)
			.ListItemsSource(&_findActorListView._valueList) //The Items array is the source of this listview
		.OnGenerateRow(this, &SGsTerrEdSpotFinderCompoundWidget::OnGenerateSpotActorWidget)
		.OnSelectionChanged(this, &SGsTerrEdSpotFinderCompoundWidget::OnSelectionChangeSpotActor)
		.ItemHeight(24)
		.SelectionMode(ESelectionMode::Multi)
		.ScrollbarVisibility(EVisibility::All)
		];
}

void SGsTerrEdSpotFinderCompoundWidget::InitSpotActorList()
{
	_findActorListView._valueList.Empty();
	for (AActor* actor : _findActorListView._actorList)
	{
		_findActorListView._valueList.Add(MakeShareable(new FString(actor->GetActorLabel())));
	}
}

TSharedRef<ITableRow> SGsTerrEdSpotFinderCompoundWidget::OnGenerateSpotActorWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable)
{
	//Create the row
	return	SNew(STableRow< TSharedPtr<FString> >, inOwnerTable)
		.Padding(2.0f)
		[
			SNew(STextBlock).Text(FText::FromString(*inItem.Get()))
		];
}

void SGsTerrEdSpotFinderCompoundWidget::OnDoubleClickSpotActor(TSharedPtr<FString> InNewSelection)
{

}

void SGsTerrEdSpotFinderCompoundWidget::OnSelectionChangeSpotActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo)
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
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritorySpot::StaticClass(), actorList);

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

void SGsTerrEdSpotFinderCompoundWidget::OnFieldColumnResized(const float inWidth)
{
	_fieldColumnSize = inWidth;

	UE_LOG(LogTerritoryEditor, Log, TEXT("Set field width : %d"), _fieldColumnSize);
}

void SGsTerrEdSpotFinderCompoundWidget::OnIncludeConditionColumnResized(const float inWidth)
{
	_conditionColumnSize = inWidth;

	UE_LOG(LogTerritoryEditor, Log, TEXT("Set condition width : %d"), _conditionColumnSize);
}

FReply SGsTerrEdSpotFinderCompoundWidget::OnFindSpot()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();	
	if (nullptr == world)
	{
		return FReply::Handled();
	}

	if (false == CanFindSpot())
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
		AGsTerritorySpot* spot = Cast<AGsTerritorySpot>(actor);
		if (spot)
		{
			if (IsValidSpotType(spot)
				&& IsValidSpotRadius(spot))
			{
				_findActorListView._actorList.Add(spot);
			}
		}
	}

	USelection* select = GEditor->GetSelectedActors();
	if (select)
	{
		UGsTerritoryLevelUtil::SetSelectedLevelActors(_findActorListView._actorList);
	}

	InitSpotActorList();
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

FReply SGsTerrEdSpotFinderCompoundWidget::OnDelectAll()
{
	_isCheckSelectAllActor = false;
	_selectAllActorCheckBox.Get()->Invalidate(EInvalidateWidgetReason::Layout);

	SetSelectAllActor(false);

	return FReply::Handled();
}

bool SGsTerrEdSpotFinderCompoundWidget::CanFindSpot()
{
	return _spotTypeComboBox.IsValid()
		&& _spotRadiusRangeNumericEntryBox.IsValid();
}


#undef LOCTEXT_NAMESPACE
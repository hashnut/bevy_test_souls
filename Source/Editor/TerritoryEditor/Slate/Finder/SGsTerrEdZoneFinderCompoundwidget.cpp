// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdZoneFinderCompoundwidget.h"
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

void SGsTerrEdZoneFinderCompoundwidget::Construct(const FArguments& InArgs)
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
	_spawnGroupComboBox.Initialize();
	_zoneRadiusNumericEntryBox.Initialize();
	_zoneSectionNumericEntryBox.Initialize();

	_isCheckByClient = false;
	_isCheckByClientCondition = false;

	_isCheckByDefault = false;
	_isCheckByDefaultCondition = false;

	_note = "";
	_isCheckNoteCondition = false;

	_isCheckSelectAllActor = FGsTerrEdSlateUtil::IsShowOnlySelectActor();

	//create widget
	TSharedPtr<SWidget> uniqueIdWidget = CreateUniqueIdWidget();
	TSharedPtr<SWidget> spawnGroupComboBox = CreateSpawnGroupWidget();
	TSharedPtr<SWidget> byClientWidget = CreateByClientWidget();
	TSharedPtr<SWidget> byDefualtWidget = CreateByDefualtWidget();
	TSharedPtr<SWidget> radiusWidget = CreateRadiusRangeWidget();
	TSharedPtr<SWidget> secionWidget = CreateSectionWidget();
	TSharedPtr<SWidget> noteWidget = CreateNoteWidget();
	TSharedPtr<SWidget> zoneActorFindWidget = CreateZoneFindWidget(); 
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
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			uniqueIdWidget.ToSharedRef()
		]	
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			spawnGroupComboBox.ToSharedRef()
		]	
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			byClientWidget.ToSharedRef()
		]	
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			byDefualtWidget.ToSharedRef()
		]	
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			radiusWidget.ToSharedRef()
		]	
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			secionWidget.ToSharedRef()
		]	
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			noteWidget.ToSharedRef()
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
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetOnlySelectActorSpawnCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnOnlySelectActorSpawnChanged)
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
		.OnClicked(this, &SGsTerrEdZoneFinderCompoundwidget::OnFindZone)
		]
	+ SHorizontalBox::Slot()
		.FillWidth(0.2f)
		+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.MaxWidth(400)
		[
			SNew(SButton)
			.Text(LOCTEXT("DeselectAll", "Deselect All"))
		.OnClicked(this, &SGsTerrEdZoneFinderCompoundwidget::OnDelectAll)
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
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetSelectAllActorState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnSelectAllActorChanged)
		]
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			SNew(SHeaderRow)
			+ SHeaderRow::Column(TEXT("Find zone list"))
		.FillWidth(1.f)
		.DefaultLabel(LOCTEXT("Find zone list", "Find zone list"))
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			zoneActorFindWidget.ToSharedRef()
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

TSharedPtr<SWidget> SGsTerrEdZoneFinderCompoundwidget::CreateUniqueIdWidget()
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
			.Value(this, &SGsTerrEdZoneFinderCompoundwidget::OnGetUniqueId)
		.OnValueCommitted(this, &SGsTerrEdZoneFinderCompoundwidget::OnSetUniqueId)
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
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetUniqueIdConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnUniqueIdConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdZoneFinderCompoundwidget::CreateSpawnGroupWidget()
{
	TSharedPtr<SWidget> spawnGroupTypeFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Spawn group type"));
	_spawnGroupComboBox._valueList = GetSpawnGroupTypeList();
	_spawnGroupComboBox._selectedValue = _spawnGroupComboBox._valueList[0];

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			spawnGroupTypeFieldName.ToSharedRef()
		]

	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			//min npc id searchbox
		+ SHorizontalBox::Slot()
		.Padding(_fieldLeftRowPadding)
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("SpawnGroupType")))
		]
	+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_spawnGroupComboBox._comboBox, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&_spawnGroupComboBox._valueList)
		.OnSelectionChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnSpawnGroupTypeSelectionChanged)
		.OnGenerateWidget(this, &SGsTerrEdZoneFinderCompoundwidget::OnGenerateComboWidget)
		.InitiallySelectedItem(_spawnGroupComboBox._selectedValue)
		.Content()
		[
			SNew(STextBlock).Text(this, &SGsTerrEdZoneFinderCompoundwidget::GetSelectedSpawnGroupTypeText)
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
			SAssignNew(_spawnGroupComboBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetSpawnGroupTypeConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnSpawnGroupTypeConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdZoneFinderCompoundwidget::CreateByClientWidget()
{
	TSharedPtr<SWidget> byClientFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("By Client"));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			byClientFieldName.ToSharedRef()
		]
	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_byClientCheckBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetByClientCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnByClientCheckChanged)
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
			SAssignNew(_byClientConditionCheckBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetByClientConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnByClientConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdZoneFinderCompoundwidget::CreateByDefualtWidget()
{
	TSharedPtr<SWidget> randomSpawnFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("By Default"));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			randomSpawnFieldName.ToSharedRef()
		]
	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_byDefaultCheckBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetByDefaultCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnByDefaultCheckChanged)
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
			SAssignNew(_byDefaultConditionCheckBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetByDefaultConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnByDefaultConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdZoneFinderCompoundwidget::CreateRadiusRangeWidget()
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
			SAssignNew(_zoneRadiusNumericEntryBox._minNumericEntryBox, SNumericEntryBox<float>)
			.Value(this, &SGsTerrEdZoneFinderCompoundwidget::OnGetMinZoneRadius)
		.OnValueCommitted(this, &SGsTerrEdZoneFinderCompoundwidget::OnSetMinZoneRadius)
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
			SAssignNew(_zoneRadiusNumericEntryBox._maxNumericEntryBox, SNumericEntryBox<float>)
			.Value(this, &SGsTerrEdZoneFinderCompoundwidget::OnGetMaxZoneRadius)
		.OnValueCommitted(this, &SGsTerrEdZoneFinderCompoundwidget::OnSetMaxZoneRadius)
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
			SAssignNew(_zoneRadiusNumericEntryBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetZoneRadiusConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnZoneRadiusConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdZoneFinderCompoundwidget::CreateSectionWidget()
{
	TSharedPtr<SWidget> tidFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Section"));

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
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_zoneSectionNumericEntryBox._numericEntryBox, SNumericEntryBox<int32>)
			.Value(this, &SGsTerrEdZoneFinderCompoundwidget::OnGetZoneSection)
		.OnValueCommitted(this, &SGsTerrEdZoneFinderCompoundwidget::OnSetZoneSection)
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
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetZoneSectionConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnZoneSectionConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdZoneFinderCompoundwidget::CreateNoteWidget()
{
	TSharedPtr<SWidget> noteFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Note"));

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
			SAssignNew(_noteEditableTextBox, SEditableTextBox)
			.OnTextChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnNoteTextChanged)
			.OnTextCommitted(this, &SGsTerrEdZoneFinderCompoundwidget::OnNoteTextCommit)
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
			SAssignNew(_noteTextCheckBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdZoneFinderCompoundwidget::GetNoteConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnNoteConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdZoneFinderCompoundwidget::CreateZoneFindWidget()
{
	return SNew(SBox)
		.Padding(FMargin(4.0f, 4.0f))
		.HeightOverride(300.0f)
		[
			SAssignNew(_findActorListView._listView, SListView<TSharedPtr<FString>>)
			.ListItemsSource(&_findActorListView._valueList) //The Items array is the source of this listview
		.OnGenerateRow(this, &SGsTerrEdZoneFinderCompoundwidget::OnGenerateTableWidget)
		.OnSelectionChanged(this, &SGsTerrEdZoneFinderCompoundwidget::OnSelectionChangeZoneActor)
		.ItemHeight(24)
		.SelectionMode(ESelectionMode::Multi)
		.ScrollbarVisibility(EVisibility::All)
		];
}


TOptional<int64> SGsTerrEdZoneFinderCompoundwidget::OnGetUniqueId() const
{
	return _uniqueIdNumericEntryBox._value;
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetUniqueIdConditionCheckState() const
{
	return _uniqueIdNumericEntryBox.GetCheckState();
}

void SGsTerrEdZoneFinderCompoundwidget::OnUniqueIdConditionCheckChanged(ECheckBoxState inState)
{
	_uniqueIdNumericEntryBox.SetConditionCheckState(inState);
}

TArray<TSharedPtr<FString>> SGsTerrEdZoneFinderCompoundwidget::GetSpawnGroupTypeList()
{
	TArray<TSharedPtr<FString>> spawnGroupTypeNameList;
	FString spawnGroupTypeText;
	int32 num = (int32)EGsTerritorySpotType::Max;
	for (int i = 0; i < num; ++i)
	{
		spawnGroupTypeText = FGsTextUtil::GetStringFromEnum(TEXT("NpcSpawnGroupType"), (NpcSpawnGroupType)i);
		spawnGroupTypeNameList.Add(MakeShareable(new FString(spawnGroupTypeText)));
	}

	return spawnGroupTypeNameList;
}

void SGsTerrEdZoneFinderCompoundwidget::OnSpawnGroupTypeSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		_spawnGroupComboBox._selectedValue = InItem;
	}
}

TSharedRef<SWidget> SGsTerrEdZoneFinderCompoundwidget::OnGenerateComboWidget(TSharedPtr<FString> inSelect)
{
	//Create the row
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

FText SGsTerrEdZoneFinderCompoundwidget::GetSelectedSpawnGroupTypeText() const
{
	return FText::FromString(*_spawnGroupComboBox._selectedValue.Get());
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetSpawnGroupTypeConditionCheckState() const
{
	return (_spawnGroupComboBox._isCheck ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdZoneFinderCompoundwidget::OnSpawnGroupTypeConditionCheckChanged(ECheckBoxState inState)
{
	_spawnGroupComboBox._isCheck = (inState == ECheckBoxState::Checked ? true : false);
}

void SGsTerrEdZoneFinderCompoundwidget::OnSetUniqueId(int64 inValue, ETextCommit::Type inType)
{
	_uniqueIdNumericEntryBox._value = inValue;
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetOnlySelectActorSpawnCheckState() const
{
	return FGsTerrEdSlateUtil::IsShowOnlySelectActor() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdZoneFinderCompoundwidget::OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const
{
	bool isCheckted = (ECheckBoxState::Checked == inState);

	FGsTerrEdSlateUtil::SetShowOnlySelectActor(isCheckted);
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetSelectAllActorState() const
{
	return _isCheckSelectAllActor ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdZoneFinderCompoundwidget::SetSelectAllActor(bool inAllSelect)
{
	if (inAllSelect)
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

void SGsTerrEdZoneFinderCompoundwidget::InitZoneActorList()
{
	_findActorListView._valueList.Empty();
	for (AActor* actor : _findActorListView._actorList)
	{
		_findActorListView._valueList.Add(MakeShareable(new FString(actor->GetActorLabel())));
	}
}

FText SGsTerrEdZoneFinderCompoundwidget::GetSelectedZoneActorText() const
{
	return FText::FromString(_findActorListView._selectedString);
}

TSharedRef<ITableRow> SGsTerrEdZoneFinderCompoundwidget::OnGenerateTableWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable)
{
	//Create the row
	return	SNew(STableRow< TSharedPtr<FString> >, inOwnerTable)
		.Padding(2.0f)
		[
			SNew(STextBlock).Text(FText::FromString(*inItem.Get()))
		];
}

void SGsTerrEdZoneFinderCompoundwidget::OnDoubleClickZoneActor(TSharedPtr<FString> InNewSelection)
{

}

void SGsTerrEdZoneFinderCompoundwidget::OnSelectionChangeZoneActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo)
{
	TArray<TSharedPtr<FString>> selectItems = _findActorListView._listView->GetSelectedItems();
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		_findActorListView._listView->ClearSelection();
		return;
	}

	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryZone::StaticClass(), actorList);

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

int32 SGsTerrEdZoneFinderCompoundwidget::GetZoneId(const FString& inZoneId)
{
	FString zoneId = inZoneId;
	TArray<FString> tIdParses;
	zoneId.ParseIntoArray(tIdParses, TEXT("-"), true);
	if (tIdParses.Num() > 0)
	{
		return  FCString::Atoi(*tIdParses[0]);
	}

	return 0;
}

FReply SGsTerrEdZoneFinderCompoundwidget::OnFindZone()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return FReply::Handled();
	}

	if (false == CanFindZone())
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
		AGsTerritoryZone* zone = Cast<AGsTerritoryZone>(actor);
		if (zone)
		{
			if (IsValidUniqueId(zone)
				&& IsValidSpawnType(zone)
				&& IsValidZoneRadius(zone)
				&& IsValidByClient(zone)
				&& IsValidByDefault(zone)
				&& IsValidSection(zone)
				&& IsValidNote(zone))
			{
				_findActorListView._actorList.Add(zone);
			}
		}
	}

	USelection* select = GEditor->GetSelectedActors();
	if (select)
	{
		UGsTerritoryLevelUtil::SetSelectedLevelActors(_findActorListView._actorList);
	}

	InitZoneActorList();
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

FReply SGsTerrEdZoneFinderCompoundwidget::OnDelectAll()
{
	_isCheckSelectAllActor = false;
	_selectAllActorCheckBox.Get()->Invalidate(EInvalidateWidgetReason::Layout);

	SetSelectAllActor(false);

	return FReply::Handled();
}

bool SGsTerrEdZoneFinderCompoundwidget::CanFindZone()
{
	return _uniqueIdNumericEntryBox.IsValid()
		&& _spawnGroupComboBox.IsValid()
		&& _zoneRadiusNumericEntryBox.IsValid()
		&& _zoneSectionNumericEntryBox.IsValid();
}

bool SGsTerrEdZoneFinderCompoundwidget::IsValidUniqueId(class AGsTerritoryZone* inZone)
{
	return _uniqueIdNumericEntryBox._isCheck ? inZone->GetTerritoryUniqueID() == _uniqueIdNumericEntryBox._value : true;
}

bool SGsTerrEdZoneFinderCompoundwidget::IsValidSpawnType(class AGsTerritoryZone* inZone)
{
	if (_spawnGroupComboBox._isCheck)
	{
		FString spawnType = FGsTextUtil::GetStringFromEnum<NpcSpawnGroupType>(TEXT("NpcSpawnGroupType"), inZone->GetSpawnGroupType());
		if (_spawnGroupComboBox._selectedValue.IsValid())
		{
			return (*_spawnGroupComboBox._selectedValue).Equals(spawnType);
		}
	}

	return true;
}

bool SGsTerrEdZoneFinderCompoundwidget::IsValidZoneRadius(class AGsTerritoryZone* inZone)
{
	if (_zoneRadiusNumericEntryBox._isCheck)
	{
		return _zoneRadiusNumericEntryBox._minValue <= inZone->_radius
			&& _zoneRadiusNumericEntryBox._maxValue >= inZone->_radius;
	}

	return true;
}

bool SGsTerrEdZoneFinderCompoundwidget::IsValidByClient(class AGsTerritoryZone* inZone)
{
	return _isCheckByClientCondition ? inZone->_byClient == _isCheckByClient : true;
}

bool SGsTerrEdZoneFinderCompoundwidget::IsValidByDefault(class AGsTerritoryZone* inZone)
{
	return _isCheckByDefaultCondition ? inZone->_byDefault == _isCheckByDefault : true;
}

bool SGsTerrEdZoneFinderCompoundwidget::IsValidSection(class AGsTerritoryZone* inZone)
{
	if (_zoneSectionNumericEntryBox._isCheck)
	{
		return _zoneSectionNumericEntryBox._value == inZone->_section;
	}

	return true;
}

bool SGsTerrEdZoneFinderCompoundwidget::IsValidNote(class AGsTerritoryZone* inZone)
{
	if (_isCheckNoteCondition)
	{
		return inZone->_note.Contains(_note);
	}

	return true;
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetByClientCheckState() const
{
	return (_isCheckByClient ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdZoneFinderCompoundwidget::OnByClientCheckChanged(ECheckBoxState inState)
{
	_isCheckByClient = (inState == ECheckBoxState::Checked ? true : false);
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetByClientConditionCheckState() const
{
	return (_isCheckByClientCondition ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdZoneFinderCompoundwidget::OnByClientConditionCheckChanged(ECheckBoxState inState)
{
	_isCheckByClientCondition = (inState == ECheckBoxState::Checked ? true : false);
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetByDefaultCheckState() const
{
	return (_isCheckByDefault ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdZoneFinderCompoundwidget::OnByDefaultCheckChanged(ECheckBoxState inState)
{
	_isCheckByDefault = (inState == ECheckBoxState::Checked ? true : false);
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetByDefaultConditionCheckState() const
{
	return (_isCheckByDefaultCondition ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdZoneFinderCompoundwidget::OnByDefaultConditionCheckChanged(ECheckBoxState inState)
{
	_isCheckByDefaultCondition = (inState == ECheckBoxState::Checked ? true : false);
}

TOptional<float> SGsTerrEdZoneFinderCompoundwidget::OnGetMinZoneRadius() const
{
	return TOptional<float>(_zoneRadiusNumericEntryBox._minValue);
}

void SGsTerrEdZoneFinderCompoundwidget::OnSetMinZoneRadius(float inValue, ETextCommit::Type inType)
{
	_zoneRadiusNumericEntryBox._minValue = inValue;
}

TOptional<float> SGsTerrEdZoneFinderCompoundwidget::OnGetMaxZoneRadius() const
{
	return TOptional<float>(_zoneRadiusNumericEntryBox._maxValue);
}

void SGsTerrEdZoneFinderCompoundwidget::OnSetMaxZoneRadius(float inValue, ETextCommit::Type inType)
{
	_zoneRadiusNumericEntryBox._maxValue = inValue;
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetZoneRadiusConditionCheckState() const
{
	return _zoneRadiusNumericEntryBox.GetCheckState();
}

void SGsTerrEdZoneFinderCompoundwidget::OnZoneRadiusConditionCheckChanged(ECheckBoxState inState)
{
	_zoneRadiusNumericEntryBox._isCheck = (inState == ECheckBoxState::Checked ? true : false);
}

TOptional<int32> SGsTerrEdZoneFinderCompoundwidget::OnGetZoneSection() const
{
	return TOptional<int32>(_zoneSectionNumericEntryBox._value);
}

void SGsTerrEdZoneFinderCompoundwidget::OnSetZoneSection(int32 inValue, ETextCommit::Type inType)
{
	_zoneSectionNumericEntryBox._value = inValue;
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetZoneSectionConditionCheckState() const
{
	return _zoneSectionNumericEntryBox.GetCheckState();
}

void SGsTerrEdZoneFinderCompoundwidget::OnZoneSectionConditionCheckChanged(ECheckBoxState inState)
{
	_zoneSectionNumericEntryBox._isCheck = (inState == ECheckBoxState::Checked ? true : false);
}

void SGsTerrEdZoneFinderCompoundwidget::OnNoteTextChanged(const FText& inText)
{
	
}

void SGsTerrEdZoneFinderCompoundwidget::OnNoteTextCommit(const FText& inText, ETextCommit::Type inCommitType)
{
	if (inCommitType == ETextCommit::OnUserMovedFocus)
	{
		_note = inText.ToString();
	}	
}

ECheckBoxState SGsTerrEdZoneFinderCompoundwidget::GetNoteConditionCheckState() const
{
	return _isCheckNoteCondition ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdZoneFinderCompoundwidget::OnNoteConditionCheckChanged(ECheckBoxState inState)
{
	_isCheckNoteCondition = (inState == ECheckBoxState::Checked ? true : false);
}

void SGsTerrEdZoneFinderCompoundwidget::OnSelectAllActorChanged(ECheckBoxState inState)
{
	_isCheckSelectAllActor = (ECheckBoxState::Checked == inState);

	SetSelectAllActor(_isCheckSelectAllActor);
}

#undef LOCTEXT_NAMESPACE
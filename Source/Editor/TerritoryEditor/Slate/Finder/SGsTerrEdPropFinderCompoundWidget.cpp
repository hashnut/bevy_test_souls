// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdPropFinderCompoundWidget.h"
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
#include "Actor/Spawn/SpawnActor/GsTerritoryProp.h"
#include "GameObject/Prop/GsSchemaPropData.h"
#include "TerritoryEditor.h"

#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdPropFinderCompoundWidget::Construct(const FArguments& InArgs)
{
	//layout 
	_fieldLeftColumnSize = 100;
	_fieldRightColumnSize = 300;
	_conditionColumnSize = 100;
	_height = 25.0f;
	_fieldLeftRowPadding = FMargin(0.0f, 2.5f, 2.0f, 2.5f);

	//initialize condition
	_propIdRangeSearchableComboBox.Initialize();
	_propIdSearchableComboBox.Initialize();
	_uniqueIdNumericEntryBox.Initialize();
	_interactionTimeRangeNumericEntryBox.Initialize();
	_maxSpawnNumRangeNumericEntryBox.Initialize();
	_interactionRangeRangeNumericEntryBox.Initialize();
	_initiSpawnDelayRangeNumericEntryBox.Initialize();
	_findActorListView.Initialize();
				
	_isCheckRandomSpawnCondition = false;			
	_isCheckRandomSpawn = false;

	_selectCount = 0;
	_totalCount = 0;

	InitPropIdList();
	InitPropId();
	InitPropIdRange();
	InitPropActorList();

	TSharedPtr<SWidget> propIdRangeWidget = CreatePropIdRangeWidget();
	TSharedPtr<SWidget> propIdWidget = CreatePropIdWidget();
	TSharedPtr<SWidget> uniqueIdWidget = CreateUniqueIdWidget();
	TSharedPtr<SWidget> interactionTimeWidget = CreateInteractionTimeWidget();
	TSharedPtr<SWidget> interactionRangeWidget = CreateInteractionRangeWidget();
	TSharedPtr<SWidget> maxSpawnNumWidget = CreateMaxSpawnNumWidget();
	TSharedPtr<SWidget> initSpawnDelayWidget = CreateInitSpawnDelayWidget();
	TSharedPtr<SWidget> randomSpawnWidget = CreateRandomSpawnWidget();
	TSharedPtr<SWidget> propActorFindWidget = CreatePropActorFindWidget();
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
			propIdRangeWidget.ToSharedRef()
		]
	//tid column
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			propIdWidget.ToSharedRef()
		]
	//unique id widget
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			uniqueIdWidget.ToSharedRef()
		]
	//unique id widget
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			interactionTimeWidget.ToSharedRef()
		]
	//unique id widget
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			interactionRangeWidget.ToSharedRef()
		]
	//max spawn num
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			maxSpawnNumWidget.ToSharedRef()
		]
	//init spawn delay
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			initSpawnDelayWidget.ToSharedRef()
		]
	//unique id widget
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			randomSpawnWidget.ToSharedRef()
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
			.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetOnlySelectActorSpawnCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnOnlySelectActorSpawnChanged)
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
				.OnClicked(this, &SGsTerrEdPropFinderCompoundWidget::OnFindProp)
			]
			+ SHorizontalBox::Slot()
			.FillWidth(0.2f)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.MaxWidth(400)
			[
				SNew(SButton)
				.Text(LOCTEXT("DeselectAll", "Deselect All"))
				.OnClicked(this, &SGsTerrEdPropFinderCompoundWidget::OnDelectAll)
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
				.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetSelectAllActorState)
				.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnSelectAllActorChanged)
			]
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			SNew(SHeaderRow)
			+ SHeaderRow::Column(TEXT("Find prop list"))
			.FillWidth(1.f)
			.DefaultLabel(LOCTEXT("Find prop list", "Find prop list"))
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			propActorFindWidget.ToSharedRef()
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

void SGsTerrEdPropFinderCompoundWidget::InitPropIdList()
{
	const UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaPropData::StaticStruct());
	TArray<const FGsSchemaPropData*> propDataList;
	table->GetAllRows<FGsSchemaPropData>(propDataList);
	TArray<FGsTerritorySpawnIDInfo> propList;

	//add sort id
	for (const FGsSchemaPropData* data : propDataList)
	{
		FGsTerritorySpawnIDInfo temp;

		temp.id = data->id;
		temp.desc = data->nameString.ToString();

		propList.Add(temp);
	}

	propList.Sort([=](const FGsTerritorySpawnIDInfo& source, const FGsTerritorySpawnIDInfo& dest)->bool {
		return source.id < dest.id;
	});

	//add npc id 
	_propIdList.Empty();

	for (const FGsTerritorySpawnIDInfo& data : propList)
	{
		FString temp = FString::FromInt(data.id) + TEXT("-") + data.desc;
		_propIdList.Add(MakeShareable(new FString(temp)));
	}
}

void SGsTerrEdPropFinderCompoundWidget::InitPropId()
{
	if (_propIdList.Num() > 0)
	{
		_propIdSearchableComboBox._selectStringPtr = _propIdList[0];
		
		OnPropIdSelectionChanged(_propIdSearchableComboBox._selectStringPtr, ESelectInfo::OnMouseClick);
	}
	else
	{
		_propIdSearchableComboBox._selectedString = TEXT("No id in table");
	}
}

void SGsTerrEdPropFinderCompoundWidget::InitPropIdRange()
{
	if (_propIdList.Num() > 0)
	{
		int32 max = _propIdList.Num() - 1;

		_propIdRangeSearchableComboBox._minSelectStringPtr = _propIdList[0];
		_propIdRangeSearchableComboBox._maxSelectStringPtr = _propIdList[max];

		OnMinPropIdSelectionChanged(_propIdRangeSearchableComboBox._minSelectStringPtr, ESelectInfo::OnMouseClick);
		OnMaxPropIdSelectionChanged(_propIdRangeSearchableComboBox._maxSelectStringPtr, ESelectInfo::OnMouseClick);
	}
	else
	{
		_propIdRangeSearchableComboBox._maxSelectedString = TEXT("No id in table");
		_propIdRangeSearchableComboBox._maxSelectedString = TEXT("No id in table");
	}
}


FText SGsTerrEdPropFinderCompoundWidget::GetSelectedPropIdText() const
{
	return FText::FromString(_propIdSearchableComboBox._selectedString);
}

FText SGsTerrEdPropFinderCompoundWidget::GetSelectedMinPropIdText() const
{
	return FText::FromString(_propIdRangeSearchableComboBox._minSelectedString);
}

FText SGsTerrEdPropFinderCompoundWidget::GetSelectedMaxPropIdText() const
{
	return FText::FromString(_propIdRangeSearchableComboBox._maxSelectedString);
}

TSharedRef<SWidget> SGsTerrEdPropFinderCompoundWidget::OnGeneratePropIdWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void SGsTerrEdPropFinderCompoundWidget::OnPropIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		FString value = *InItem.Get();
		_propIdSearchableComboBox._selectedString = value;

		_selectPropId = GetPropId(_propIdSearchableComboBox._selectedString);
	
		UE_LOG(LogTerritoryEditor, Log, TEXT("SGsTerrEdPropFinderCompoundWidget - OnPropIdSelectionChanged - Select prop id : %d"), _selectPropId);
	}
}

void SGsTerrEdPropFinderCompoundWidget::OnMinPropIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		FString value = *InItem.Get();
		_propIdRangeSearchableComboBox._minSelectedString = value;
		_selectMinPropId = GetPropId(value);

		UE_LOG(LogTerritoryEditor, Log, TEXT("SGsTerrEdPropFinderCompoundWidget - OnPropIdSelectionChanged - Select min prop id : %d"), _selectMinPropId);
	}
}

void SGsTerrEdPropFinderCompoundWidget::OnMaxPropIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		FString value = *InItem.Get();
		_propIdRangeSearchableComboBox._maxSelectedString = value;
		_selectMaxPropId = GetPropId(value);

		UE_LOG(LogTerritoryEditor, Log, TEXT("SGsTerrEdPropFinderCompoundWidget - OnPropIdSelectionChanged - Select prop id : %d"), _selectMaxPropId);
	}
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetPropIdRangeConditionCheckState() const
{
	return _propIdRangeSearchableComboBox.GetCheckState();
}

void SGsTerrEdPropFinderCompoundWidget::OnPropIdRangeConditionCheckChanged(ECheckBoxState inState)
{
	_propIdRangeSearchableComboBox.SetConditionCheckState(inState);
}

bool SGsTerrEdPropFinderCompoundWidget::IsValidPropIdRange(AGsTerritoryProp* inProp)
{
	if(_propIdRangeSearchableComboBox._isCheck)
	{ 
		return (inProp->_spawnInfo.TargetID >= _selectMinPropId 
			&& inProp->_spawnInfo.TargetID <= _selectMaxPropId);
	}

	return true;
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetPropIdConditionCheckState() const
{
	return _propIdSearchableComboBox.GetCheckState();
}

void SGsTerrEdPropFinderCompoundWidget::OnPropIdConditionCheckChanged(ECheckBoxState inState)
{
	_propIdSearchableComboBox.SetConditionCheckState(inState);	
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetUniqueIdConditionCheckState() const
{
	return _uniqueIdNumericEntryBox.GetCheckState();
}

void SGsTerrEdPropFinderCompoundWidget::OnUniqueIdConditionCheckChanged(ECheckBoxState inState)
{
	_uniqueIdNumericEntryBox.SetConditionCheckState(inState);	
}

bool SGsTerrEdPropFinderCompoundWidget::IsValidUniqueId(AGsTerritoryProp* inProp)
{
	return _uniqueIdNumericEntryBox._isCheck ? inProp->GetTerritoryUniqueID() == _uniqueIdNumericEntryBox._value : true;
}

TOptional<int64> SGsTerrEdPropFinderCompoundWidget::OnGetUniqueId() const
{
	return TOptional<int64>(_uniqueIdNumericEntryBox._value);
}

void SGsTerrEdPropFinderCompoundWidget::OnSetUniqueId(int64 inValue, ETextCommit::Type inType)
{
	_uniqueIdNumericEntryBox._value = inValue;
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetInteractionTimeConditionCheckState() const
{
	return _interactionTimeRangeNumericEntryBox.GetCheckState();
}

void SGsTerrEdPropFinderCompoundWidget::OnInteractionTimeConditionCheckChanged(ECheckBoxState inState)
{
	_interactionTimeRangeNumericEntryBox.SetConditionCheckState(inState);
}

TOptional<int32> SGsTerrEdPropFinderCompoundWidget::OnGetMinInteractionTime() const
{
	return TOptional<int32>(_interactionTimeRangeNumericEntryBox._minValue);
}

void SGsTerrEdPropFinderCompoundWidget::OnSetMinInteractionTime(int32 inValue, ETextCommit::Type inType)
{
	_interactionTimeRangeNumericEntryBox._minValue = inValue;
}

TOptional<int32> SGsTerrEdPropFinderCompoundWidget::OnGetMaxInteractionTime() const
{
	return TOptional<int32>(_interactionTimeRangeNumericEntryBox._maxValue);
}

void SGsTerrEdPropFinderCompoundWidget::OnSetMaxInteractionTime(int32 inValue, ETextCommit::Type inType)
{
	_interactionTimeRangeNumericEntryBox._maxValue = inValue;
}

bool SGsTerrEdPropFinderCompoundWidget::IsValidInteractionTime(AGsTerritoryProp* inProp)
{
	if (_interactionTimeRangeNumericEntryBox._isCheck)
	{
		const UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaPropData::StaticStruct());
		const UGsTablePropData* propTable = Cast<UGsTablePropData>(table);
		if (propTable)
		{
			const FGsSchemaPropData* find;
			propTable->FindRowById(inProp->_spawnInfo.TargetID, find);		
			if (find)
			{
				return _interactionTimeRangeNumericEntryBox.IsValid(find->touchInteractionTime);
			}
		}		
	}

	return true;
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetInteractionRangeConditionCheckState() const
{
	return _interactionRangeRangeNumericEntryBox.GetCheckState();
}

void SGsTerrEdPropFinderCompoundWidget::OnInteractionRangeConditionCheckChanged(ECheckBoxState inState)
{
	_interactionRangeRangeNumericEntryBox.SetConditionCheckState(inState);
}

TOptional<int32> SGsTerrEdPropFinderCompoundWidget::OnGetMinInteractionRange() const
{
	return TOptional<int32>(_interactionRangeRangeNumericEntryBox._minValue);
}

void SGsTerrEdPropFinderCompoundWidget::OnSetMinInteractionRange(int32 inValue, ETextCommit::Type inType)
{
	_interactionRangeRangeNumericEntryBox._minValue = inValue;
}

TOptional<int32> SGsTerrEdPropFinderCompoundWidget::OnGetMaxInteractionRange() const
{
	return TOptional<int32>(_interactionRangeRangeNumericEntryBox._maxValue);
}

void SGsTerrEdPropFinderCompoundWidget::OnSetMaxInteractionRange(int32 inValue, ETextCommit::Type inType)
{
	_interactionRangeRangeNumericEntryBox._maxValue = inValue;
}

bool SGsTerrEdPropFinderCompoundWidget::IsValidInteractionRange(AGsTerritoryProp* inProp)
{
	if (_interactionRangeRangeNumericEntryBox._isCheck)
	{
		const UGsTable* table = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaPropData::StaticStruct());
		const UGsTablePropData* propTable = Cast<UGsTablePropData>(table);
		if (propTable)
		{
			const FGsSchemaPropData* find;
			propTable->FindRowById(inProp->_spawnInfo.TargetID, find);
			if (find)
			{
				return _interactionRangeRangeNumericEntryBox.IsValid(find->touchInteractionRange);				
			}
		}
	}

	return true;
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetMaxSpawnNumConditionCheckState() const
{
	return _maxSpawnNumRangeNumericEntryBox.GetCheckState();
}

void SGsTerrEdPropFinderCompoundWidget::OnMaxSpawnNumConditionCheckChanged(ECheckBoxState inState)
{
	_maxSpawnNumRangeNumericEntryBox.SetConditionCheckState(inState);	
}

bool SGsTerrEdPropFinderCompoundWidget::IsValidMaxSpawnNum(AGsTerritoryProp* inProp)
{
	if (_maxSpawnNumRangeNumericEntryBox._isCheck)
	{
		return _maxSpawnNumRangeNumericEntryBox.IsValid(inProp->_spawnInfo.MaxSpawnNum);
	}

	return true;
}

TOptional<int32> SGsTerrEdPropFinderCompoundWidget::OnGetMinMaxSpawnNum() const
{
	return TOptional<int32>(_maxSpawnNumRangeNumericEntryBox._minValue);
}

void SGsTerrEdPropFinderCompoundWidget::OnSetMinMaxSpawnNum(int32 inValue, ETextCommit::Type inType)
{
	_maxSpawnNumRangeNumericEntryBox._minValue = inValue;
}

TOptional<int32> SGsTerrEdPropFinderCompoundWidget::OnGetMaxMaxSpawnNum() const
{
	return TOptional<int32>(_maxSpawnNumRangeNumericEntryBox._maxValue);
}

void SGsTerrEdPropFinderCompoundWidget::OnSetMaxMaxSpawnNum(int32 inValue, ETextCommit::Type inType)
{
	_maxSpawnNumRangeNumericEntryBox._maxValue = inValue;
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetInitSpawnDelayConditionCheckState() const
{
	return _initiSpawnDelayRangeNumericEntryBox.GetCheckState();
}

void SGsTerrEdPropFinderCompoundWidget::OnInitSpawnDelayConditionCheckChanged(ECheckBoxState inState)
{
	_initiSpawnDelayRangeNumericEntryBox.SetConditionCheckState(inState);	
}

bool SGsTerrEdPropFinderCompoundWidget::IsValidInitSpawnDelay(AGsTerritoryProp* inProp)
{
	if (_initiSpawnDelayRangeNumericEntryBox._isCheck)
	{
		return _initiSpawnDelayRangeNumericEntryBox.IsValid(inProp->_spawnInfo.InitSpawnDelay);
	}

	return true;
}

TOptional<int32> SGsTerrEdPropFinderCompoundWidget::OnGetMinInitSpawnDelay() const
{
	return TOptional<int32>(_initiSpawnDelayRangeNumericEntryBox._minValue);
}

void SGsTerrEdPropFinderCompoundWidget::OnSetMinInitSpawnDelay(int32 inValue, ETextCommit::Type inType)
{
	_initiSpawnDelayRangeNumericEntryBox._minValue = inValue;
}

TOptional<int32> SGsTerrEdPropFinderCompoundWidget::OnGetMaxInitSpawnDelay() const
{
	return TOptional<int32>(_initiSpawnDelayRangeNumericEntryBox._maxValue);
}

void SGsTerrEdPropFinderCompoundWidget::OnSetMaxInitSpawnDelay(int32 inValue, ETextCommit::Type inType)
{
	_initiSpawnDelayRangeNumericEntryBox._maxValue = inValue;
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetRandomSpawnCheckState() const
{
	return (_isCheckRandomSpawn ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdPropFinderCompoundWidget::OnRandomSpawnCheckChanged(ECheckBoxState inState)
{
	_isCheckRandomSpawn = (inState == ECheckBoxState::Checked ? true : false);
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetRandomSpawnConditionCheckState() const
{
	return (_isCheckRandomSpawnCondition ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdPropFinderCompoundWidget::OnRandomSpawnConditionCheckChanged(ECheckBoxState inState)
{
	_isCheckRandomSpawnCondition = (inState == ECheckBoxState::Checked ? true : false);
}

bool SGsTerrEdPropFinderCompoundWidget::IsValidRandomSpawn(AGsTerritoryProp* inProp)
{
	return _isCheckRandomSpawnCondition ? inProp->_spawnInfo.CanRandomLocationSpawn == _isCheckRandomSpawn : true;
}

bool SGsTerrEdPropFinderCompoundWidget::IsValidPropId(AGsTerritoryProp* inProp)
{
	return _propIdSearchableComboBox._isCheck ? inProp->_spawnInfo.TargetID == _selectPropId : true;
}

void SGsTerrEdPropFinderCompoundWidget::OnFieldColumnResized(const float inWidth)
{
	_fieldColumnSize = inWidth;

	UE_LOG(LogTerritoryEditor, Log, TEXT("Set field width : %d"), _fieldColumnSize);
}

void SGsTerrEdPropFinderCompoundWidget::OnIncludeConditionColumnResized(const float inWidth)
{
	_conditionColumnSize = inWidth;

	UE_LOG(LogTerritoryEditor, Log, TEXT("Set condition width : %d"), _conditionColumnSize);
}

TSharedPtr<SWidget> SGsTerrEdPropFinderCompoundWidget::CreatePropIdRangeWidget()
{
	TSharedPtr<SWidget> tidRangeFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Table id range"));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			tidRangeFieldName.ToSharedRef()
		]

	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			//min prop id searchbox
			+ SHorizontalBox::Slot()			
			.Padding(_fieldLeftRowPadding)
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Min")))
			]

			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_propIdRangeSearchableComboBox._minSearchableComboBox, SSearchableComboBox)
				.OptionsSource(&_propIdList)
				.OnSelectionChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnMinPropIdSelectionChanged)
				.OnGenerateWidget(this, &SGsTerrEdPropFinderCompoundWidget::OnGeneratePropIdWidget)
				.InitiallySelectedItem(_propIdRangeSearchableComboBox._minSelectStringPtr)
				.Content()
				[
					SNew(STextBlock).Text(this, &SGsTerrEdPropFinderCompoundWidget::GetSelectedMinPropIdText)
				]
			]

			//min npc id searchbox
			+ SHorizontalBox::Slot()			
			.Padding(_fieldLeftRowPadding)
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Max")))
			]

			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_propIdRangeSearchableComboBox._maxSearchableComboBox, SSearchableComboBox)
				.OptionsSource(&_propIdList)
				.OnSelectionChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnMaxPropIdSelectionChanged)
				.OnGenerateWidget(this, &SGsTerrEdPropFinderCompoundWidget::OnGeneratePropIdWidget)
				.InitiallySelectedItem(_propIdRangeSearchableComboBox._maxSelectStringPtr)
				.Content()
				[
					SNew(STextBlock).Text(this, &SGsTerrEdPropFinderCompoundWidget::GetSelectedMaxPropIdText)
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
				SAssignNew(_propIdRangeSearchableComboBox._checkBox, SCheckBox)
				.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetPropIdRangeConditionCheckState)
				.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnPropIdRangeConditionCheckChanged)
			]
		];
}

TSharedPtr<SWidget> SGsTerrEdPropFinderCompoundWidget::CreatePropIdWidget()
{
	TSharedPtr<SWidget> tidFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Table id"));

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
			SAssignNew(_propIdSearchableComboBox._searchableComboBox, SSearchableComboBox)
			.OptionsSource(&_propIdList)
		.OnSelectionChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnPropIdSelectionChanged)
		.OnGenerateWidget(this, &SGsTerrEdPropFinderCompoundWidget::OnGeneratePropIdWidget)
		.InitiallySelectedItem(_propIdSearchableComboBox._selectStringPtr)
		.Content()
		[
			SNew(STextBlock).Text(this, &SGsTerrEdPropFinderCompoundWidget::GetSelectedPropIdText)
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
			SAssignNew(_propIdSearchableComboBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetPropIdConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnPropIdConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdPropFinderCompoundWidget::CreateUniqueIdWidget()
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
			.Value(this, &SGsTerrEdPropFinderCompoundWidget::OnGetUniqueId)
			.OnValueCommitted(this, &SGsTerrEdPropFinderCompoundWidget::OnSetUniqueId)
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
			.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetUniqueIdConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnUniqueIdConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdPropFinderCompoundWidget::CreateInteractionTimeWidget()
{
	TSharedPtr<SWidget> fieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Interaction time"));

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
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Min")))
		]
	+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_interactionTimeRangeNumericEntryBox._minNumericEntryBox, SNumericEntryBox<int32>)
			.Value(this, &SGsTerrEdPropFinderCompoundWidget::OnGetMinInteractionTime)
		.OnValueCommitted(this, &SGsTerrEdPropFinderCompoundWidget::OnSetMinInteractionTime)
		]
	+ SHorizontalBox::Slot()
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Max")))
		]
	+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_interactionTimeRangeNumericEntryBox._maxNumericEntryBox, SNumericEntryBox<int32>)
			.Value(this, &SGsTerrEdPropFinderCompoundWidget::OnGetMaxInteractionTime)
		.OnValueCommitted(this, &SGsTerrEdPropFinderCompoundWidget::OnSetMaxInteractionTime)
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
			SAssignNew(_interactionTimeRangeNumericEntryBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetInteractionTimeConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnInteractionTimeConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdPropFinderCompoundWidget::CreateInteractionRangeWidget()
{
	TSharedPtr<SWidget> fieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Interaction range"));

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
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Min")))
		]
	+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_interactionRangeRangeNumericEntryBox._minNumericEntryBox, SNumericEntryBox<int32>)
			.Value(this, &SGsTerrEdPropFinderCompoundWidget::OnGetMinInteractionRange)
		.OnValueCommitted(this, &SGsTerrEdPropFinderCompoundWidget::OnSetMinInteractionRange)
		]
	+ SHorizontalBox::Slot()
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Max")))
		]
	+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_interactionRangeRangeNumericEntryBox._maxNumericEntryBox, SNumericEntryBox<int32>)
			.Value(this, &SGsTerrEdPropFinderCompoundWidget::OnGetMaxInteractionRange)
		.OnValueCommitted(this, &SGsTerrEdPropFinderCompoundWidget::OnSetMaxInteractionRange)
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
			SAssignNew(_interactionRangeRangeNumericEntryBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetInteractionRangeConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnInteractionRangeConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdPropFinderCompoundWidget::CreateMaxSpawnNumWidget()
{
	TSharedPtr<SWidget> maxSpawnNumFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Max spawn num"));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			maxSpawnNumFieldName.ToSharedRef()
		]

		+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Min")))
			]
			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_maxSpawnNumRangeNumericEntryBox._minNumericEntryBox, SNumericEntryBox<int32>)
				.Value(this, &SGsTerrEdPropFinderCompoundWidget::OnGetMinMaxSpawnNum)
				.OnValueCommitted(this, &SGsTerrEdPropFinderCompoundWidget::OnSetMinMaxSpawnNum)
			]
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Max")))
			]
			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_maxSpawnNumRangeNumericEntryBox._maxNumericEntryBox, SNumericEntryBox<int32>)
				.Value(this, &SGsTerrEdPropFinderCompoundWidget::OnGetMaxMaxSpawnNum)
				.OnValueCommitted(this, &SGsTerrEdPropFinderCompoundWidget::OnSetMaxMaxSpawnNum)
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
				SAssignNew(_maxSpawnNumRangeNumericEntryBox._checkBox, SCheckBox)
				.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetMaxSpawnNumConditionCheckState)
				.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnMaxSpawnNumConditionCheckChanged)
			]
		];
}

TSharedPtr<SWidget> SGsTerrEdPropFinderCompoundWidget::CreateInitSpawnDelayWidget()
{
	TSharedPtr<SWidget> initSpawnDelayFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Init spawn delay"));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			initSpawnDelayFieldName.ToSharedRef()
		]
	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Min")))
			]
			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_initiSpawnDelayRangeNumericEntryBox._minNumericEntryBox, SNumericEntryBox<int32>)
				.Value(this, &SGsTerrEdPropFinderCompoundWidget::OnGetMinInitSpawnDelay)
				.OnValueCommitted(this, &SGsTerrEdPropFinderCompoundWidget::OnSetMinInitSpawnDelay)
			]
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Max")))
			]
			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_initiSpawnDelayRangeNumericEntryBox._maxNumericEntryBox, SNumericEntryBox<int32>)
				.Value(this, &SGsTerrEdPropFinderCompoundWidget::OnGetMaxInitSpawnDelay)
				.OnValueCommitted(this, &SGsTerrEdPropFinderCompoundWidget::OnSetMaxInitSpawnDelay)
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
				SAssignNew(_initiSpawnDelayRangeNumericEntryBox._checkBox, SCheckBox)
				.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetInitSpawnDelayConditionCheckState)
				.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnInitSpawnDelayConditionCheckChanged)
			]

		];
}

TSharedPtr<SWidget> SGsTerrEdPropFinderCompoundWidget::CreateRandomSpawnWidget()
{
	TSharedPtr<SWidget> randomSpawnFieldName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Random spawn"));

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
			SAssignNew(_randomSpawnCheckBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetRandomSpawnCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnRandomSpawnCheckChanged)
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
			SAssignNew(_randomSpawnConditionCheckBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdPropFinderCompoundWidget::GetRandomSpawnConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnRandomSpawnConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdPropFinderCompoundWidget::CreatePropActorFindWidget()
{
	return SNew(SBox)
		.Padding(FMargin(4.0f, 4.0f))
		.HeightOverride(300.0f)
		[
			SAssignNew(_findActorListView._listView, SListView<TSharedPtr<FString>>)
			.ListItemsSource(&_findActorListView._valueList) //The Items array is the source of this listview
		.OnGenerateRow(this, &SGsTerrEdPropFinderCompoundWidget::OnGeneratePropActorWidget)
		.OnSelectionChanged(this, &SGsTerrEdPropFinderCompoundWidget::OnSelectionChangePropActor)
		.ItemHeight(24)
		.SelectionMode(ESelectionMode::Multi)
		.ScrollbarVisibility(EVisibility::All)
		];
}

FReply SGsTerrEdPropFinderCompoundWidget::OnFindProp()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();	
	if (nullptr == world)
	{
		return FReply::Handled();
	}

	if (false == CanFindProp())
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
		AGsTerritoryProp* prop = Cast<AGsTerritoryProp>(actor);
		if (prop)
		{
			if (IsValidPropIdRange(prop)
				&& IsValidPropId(prop)
				&& IsValidUniqueId(prop)
				&& IsValidInteractionTime(prop)
				&& IsValidInteractionRange(prop)
				&& IsValidMaxSpawnNum(prop)
				&& IsValidInitSpawnDelay(prop)
				&& IsValidRandomSpawn(prop))
			{
				_findActorListView._actorList.Add(actor);
			}
		}
	}

	USelection* select = GEditor->GetSelectedActors();
	if (select)
	{
		UGsTerritoryLevelUtil::SetSelectedLevelActors(_findActorListView._actorList);
	}

	InitPropActorList();

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

FReply SGsTerrEdPropFinderCompoundWidget::OnDelectAll()
{
	_isCheckSelectAllActor = false;
	_selectAllActorCheckBox.Get()->Invalidate(EInvalidateWidgetReason::Layout);

	SetSelectAllActor(false);

	return FReply::Handled();
}

bool SGsTerrEdPropFinderCompoundWidget::CanFindProp()
{
	bool isValidPropIdRange = true;
	if (_propIdRangeSearchableComboBox.IsValid())
	{
		isValidPropIdRange = (_selectMinPropId <= _selectMaxPropId);
		if (false == isValidPropIdRange)
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Prop finder"), TEXT("Prop id range is invalid"));
		}
	}

	bool isValidInteractionTime = true;
	if (_interactionTimeRangeNumericEntryBox._isCheck)
	{
		isValidInteractionTime = _interactionTimeRangeNumericEntryBox.IsValid();
		if (false == isValidInteractionTime)
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Prop finder"), TEXT("Interaction time range is invalid"));
		}
	}

	bool isValidInteractionRange = true;
	if (_interactionRangeRangeNumericEntryBox._isCheck)
	{
		isValidInteractionRange = _interactionRangeRangeNumericEntryBox.IsValid();
		if (false == isValidInteractionRange)
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Prop finder"), TEXT("Interaction range range is invalid"));
		}
	}

	bool isValidMaxSpawnNum = true;
	if (_maxSpawnNumRangeNumericEntryBox._isCheck)
	{
		isValidMaxSpawnNum = _maxSpawnNumRangeNumericEntryBox.IsValid();
		if (false == isValidMaxSpawnNum)
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Prop finder"), TEXT("Max spawn range is invalid"));
		}
	}

	bool isValidInitSpawnDelay = true;
	if (_initiSpawnDelayRangeNumericEntryBox._isCheck)
	{
		isValidInitSpawnDelay = _initiSpawnDelayRangeNumericEntryBox.IsValid();
		if (false == isValidMaxSpawnNum)
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Prop finder"), TEXT("Init spawn range is invalid"));
		}
	}

	return isValidPropIdRange
		&& isValidMaxSpawnNum
		&& isValidInitSpawnDelay
		&& isValidInteractionTime
		&& isValidInteractionRange;
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetOnlySelectActorSpawnCheckState() const
{
	return FGsTerrEdSlateUtil::IsShowOnlySelectActor() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdPropFinderCompoundWidget::OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const
{
	bool isCheckted = (ECheckBoxState::Checked == inState);

	FGsTerrEdSlateUtil::SetShowOnlySelectActor(isCheckted);
}

ECheckBoxState SGsTerrEdPropFinderCompoundWidget::GetSelectAllActorState() const
{
	return _isCheckSelectAllActor ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdPropFinderCompoundWidget::OnSelectAllActorChanged(ECheckBoxState inState)
{
	_isCheckSelectAllActor = (ECheckBoxState::Checked == inState);

	SetSelectAllActor(_isCheckSelectAllActor);
}

void SGsTerrEdPropFinderCompoundWidget::SetSelectAllActor(bool inAllSelect)
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

void SGsTerrEdPropFinderCompoundWidget::InitPropActorList()
{
	_findActorListView._valueList.Empty();
	for (AActor* actor : _findActorListView._actorList)
	{
		_findActorListView._valueList.Add(MakeShareable(new FString(actor->GetActorLabel())));
	}
}

TSharedRef<ITableRow> SGsTerrEdPropFinderCompoundWidget::OnGeneratePropActorWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable)
{
	//Create the row
	return	SNew(STableRow< TSharedPtr<FString> >, inOwnerTable)
		.Padding(2.0f)
		[
			SNew(STextBlock).Text(FText::FromString(*inItem.Get()))
		];
}

void SGsTerrEdPropFinderCompoundWidget::OnDoubleClickPropActor(TSharedPtr<FString> InNewSelection)
{

}

void SGsTerrEdPropFinderCompoundWidget::OnSelectionChangePropActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo)
{
	TArray<TSharedPtr<FString>> selectItems = _findActorListView._listView->GetSelectedItems();
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		_findActorListView._listView->ClearSelection();
		return;
	}

	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryProp::StaticClass(), actorList);

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

int32 SGsTerrEdPropFinderCompoundWidget::GetPropId(const FString& inPropId)
{
	FString propId = inPropId;
	TArray<FString> tIdParses;
	propId.ParseIntoArray(tIdParses, TEXT("-"), true);
	if (tIdParses.Num() > 0)
	{
		return  FCString::Atoi(*tIdParses[0]);
	}

	return 0;
}


#undef LOCTEXT_NAMESPACE
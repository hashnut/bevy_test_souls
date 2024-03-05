// Fill out your copyright notice in the Description page of Project Settings.

#include "SGsTerrEdNpcFinderCompoundWidget.h"
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

#define LOCTEXT_NAMESPACE "TerritoryEditor"

void SGsTerrEdNpcFinderCompoundWidget::Construct(const FArguments& InArgs)
{
	_npcIdRangeSearchableComboBox.Initialize();
	_npcIdSearchableComboBox.Initialize();
	_uniqueIdNumericEntryBox.Initialize();
	_spawnNumRangeNumericEntryBox.Initialize();
	_initSpawnDelayRangeNumericEntryBox.Initialize();	
	_respawnIntervalRangeNumericEntryBox.Initialize();
	
	_isCheckRandomSpawnCondition = false;	
	_isCheckOnlyShowSelectActor = FGsTerrEdSlateUtil::IsShowOnlySelectActor();

	_selectNpcId = 0;
	_selectMinNpcId = 0;
	_selectMaxNpcId = 0;

	InitNpcIdList();
	InitNpcId();
	InitNpcIdRange();
	InitNpcActorList();

	_isCheckRandomSpawn = false;

	_fieldLeftColumnSize = 100;
	_fieldRightColumnSize = 300;
	_conditionColumnSize = 100;
	_height = 25.0f;
	_fieldLeftRowPadding = FMargin(0.0f, 2.5f, 2.0f, 2.5f);	

	_selectCount = 0;
	_totalCount = 0;

	TSharedPtr<SWidget> npcIdRangeWidget = CreateNpcIdRangeWidget();
	TSharedPtr<SWidget> npcIdWidget = CreateNpcIdWidget();
	TSharedPtr<SWidget> uniqueIdWidget = CreateUniqueIdWidget();
	TSharedPtr<SWidget> maxSpawnNumWidget = CreateMaxSpawnNumWidget();
	TSharedPtr<SWidget> initSpawnDelayWidget = CreateInitSpawnDelayWidget();
	TSharedPtr<SWidget> randomSpawnWidget = CreateRandomSpawnWidget();
	TSharedPtr<SWidget> respawnIntervalWidget = CreateRespawnInterval();
	TSharedPtr<SWidget> spawnLevelWidget = CreateSpawnLevel();
	TSharedPtr<SWidget> npcActorFindWidget = CreateNpcActorFindWidget();
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
			npcIdRangeWidget.ToSharedRef()
		]
	//tid column
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			npcIdWidget.ToSharedRef()
		]
	//unique id widget
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			uniqueIdWidget.ToSharedRef()
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
	//random spawn 
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			randomSpawnWidget.ToSharedRef()
		]
	//random spawn 
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			respawnIntervalWidget.ToSharedRef()
		]
	//spawnLevel
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			spawnLevelWidget.ToSharedRef()
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
				.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetOnlySelectActorSpawnCheckState)
				.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnOnlySelectActorSpawnChanged)
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
				.OnClicked(this, &SGsTerrEdNpcFinderCompoundWidget::OnFindNpc)
			]
			+ SHorizontalBox::Slot()
			.FillWidth(0.2f)
			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Right)
			.MaxWidth(400)
			[
				SNew(SButton)
				.Text(LOCTEXT("DeselectAll", "Deselect All"))
				.OnClicked(this, &SGsTerrEdNpcFinderCompoundWidget::OnDelectAll)
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
				.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetSelectAllActorState)
				.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnSelectAllActorChanged)
			]
		]
	+ SVerticalBox::Slot()
		.MaxHeight(_height)
		[
			SNew(SHeaderRow)
			+ SHeaderRow::Column(TEXT("Find npc list"))
			.FillWidth(1.f)
			.DefaultLabel(LOCTEXT("Find npc list", "Find npc list"))
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			npcActorFindWidget.ToSharedRef()
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

//npc id, npc id range 를 위한 npc id list 설정
void SGsTerrEdNpcFinderCompoundWidget::InitNpcIdList()
{
	const UGsTable* npcTbl = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaNpcData::StaticStruct());
	TArray<const FGsSchemaNpcData*> npcDatas;
	npcTbl->GetAllRows<FGsSchemaNpcData>(npcDatas);
	TArray<FGsTerritoryNpcIDInfo> npcIDs;

	//add sort id
	for (const FGsSchemaNpcData* data : npcDatas)
	{
		FGsTerritoryNpcIDInfo temp;
		temp.id = data->id;
		temp.desc = data->nameText.ToString();

		npcIDs.Add(temp);
	}

	npcIDs.Sort([=](const FGsTerritoryNpcIDInfo& source, const FGsTerritoryNpcIDInfo& dest)->bool {
		return source.id < dest.id;
	});

	//add npc id 
	_npcIdList.Empty();	
	for (const FGsTerritoryNpcIDInfo& data : npcIDs)
	{
		FString temp = FString::FromInt(data.id) + TEXT("-") + data.desc;
		_npcIdList.Add(MakeShareable(new FString(temp)));
	}
}

void SGsTerrEdNpcFinderCompoundWidget::InitNpcId()
{
	if (_npcIdList.Num() > 0)
	{
		_npcIdSearchableComboBox._selectStringPtr = _npcIdList[0];		
		OnNpcIdSelectionChanged(_npcIdSearchableComboBox._selectStringPtr, ESelectInfo::OnMouseClick);
	}
	else
	{
		_npcIdSearchableComboBox._selectedString = TEXT("No id in table");
	}
}

void SGsTerrEdNpcFinderCompoundWidget::InitNpcIdRange()
{
	if (_npcIdList.Num() > 0)
	{
		int32 max = _npcIdList.Num() - 1;

		_npcIdRangeSearchableComboBox._minSelectStringPtr = _npcIdList[0];
		_npcIdRangeSearchableComboBox._maxSelectStringPtr = _npcIdList[max];

		OnMinNpcIdSelectionChanged(_npcIdRangeSearchableComboBox._minSelectStringPtr, ESelectInfo::OnMouseClick);
		OnMaxNpcIdSelectionChanged(_npcIdRangeSearchableComboBox._maxSelectStringPtr, ESelectInfo::OnMouseClick);
	}
	else
	{
		_npcIdRangeSearchableComboBox._minSelectedString = TEXT("No id in table");
		_npcIdRangeSearchableComboBox._maxSelectedString = TEXT("No id in table");		
	}
}


FText SGsTerrEdNpcFinderCompoundWidget::GetSelectedNpcIdText() const
{
	return FText::FromString(*_npcIdSearchableComboBox._selectedString);
}

FText SGsTerrEdNpcFinderCompoundWidget::GetSelectedMinNpcIdText() const
{
	return FText::FromString(_npcIdRangeSearchableComboBox._minSelectedString);
}

FText SGsTerrEdNpcFinderCompoundWidget::GetSelectedMaxNpcIdText() const
{
	return FText::FromString(_npcIdRangeSearchableComboBox._maxSelectedString);
}

TSharedRef<SWidget> SGsTerrEdNpcFinderCompoundWidget::OnGenerateNpcIdWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void SGsTerrEdNpcFinderCompoundWidget::OnNpcIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		FString value = *InItem.Get();
		_npcIdSearchableComboBox._selectedString = value;
		_selectNpcId = GetNpcId(value);
		UE_LOG(LogTerritoryEditor, Log, TEXT("SGsTerrEdNpcFinderCompoundWidget - OnNpcIdSelectionChanged - Select npc id : %d"), _selectNpcId);
	}
}

void SGsTerrEdNpcFinderCompoundWidget::OnMinNpcIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		FString value = *InItem.Get();
		_npcIdRangeSearchableComboBox._minSelectedString = value;
		_selectMinNpcId = GetNpcId(value);
		UE_LOG(LogTerritoryEditor, Log, TEXT("SGsTerrEdNpcFinderCompoundWidget - OnNpcIdSelectionChanged - Select min npc id : %d"), _selectMinNpcId);
	}
}

void SGsTerrEdNpcFinderCompoundWidget::OnMaxNpcIdSelectionChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo)
{
	if (InSeletionInfo != ESelectInfo::Direct)
	{
		FString value = *InItem.Get();
		_npcIdRangeSearchableComboBox._maxSelectedString = value;		
		_selectMaxNpcId = GetNpcId(value);		
		UE_LOG(LogTerritoryEditor, Log, TEXT("SGsTerrEdNpcFinderCompoundWidget - OnNpcIdSelectionChanged - Select npc id : %d"), _selectMaxNpcId);
	}
}

ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetNpcIdRangeConditionCheckState() const
{
	return _npcIdRangeSearchableComboBox.GetCheckState();
}

void SGsTerrEdNpcFinderCompoundWidget::OnNpcIdRangeConditionCheckChanged(ECheckBoxState inState)
{
	_npcIdRangeSearchableComboBox.SetConditionCheckState(inState);	
}

bool SGsTerrEdNpcFinderCompoundWidget::IsValidNpcIdRange(AGsTerritoryNpc* inNpc)
{
	if(_npcIdRangeSearchableComboBox._isCheck)
	{ 
		return (inNpc->_spawnInfo.TargetID >= _selectMinNpcId
			&& inNpc->_spawnInfo.TargetID <= _selectMaxNpcId);
	}

	return true;
}

ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetNpcIdConditionCheckState() const
{
	return _npcIdSearchableComboBox.GetCheckState();
}

void SGsTerrEdNpcFinderCompoundWidget::OnNpcIdConditionCheckChanged(ECheckBoxState inState)
{
	_npcIdSearchableComboBox.SetConditionCheckState(inState);
}
//=========================== unique id ====================================
ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetUniqueIdConditionCheckState() const
{
	return _uniqueIdNumericEntryBox.GetCheckState();
}

void SGsTerrEdNpcFinderCompoundWidget::OnUniqueIdConditionCheckChanged(ECheckBoxState inState)
{
	_uniqueIdNumericEntryBox.SetConditionCheckState(inState);
}

bool SGsTerrEdNpcFinderCompoundWidget::IsValidUniqueId(AGsTerritoryNpc* inNpc)
{
	return _uniqueIdNumericEntryBox._isCheck ? inNpc->GetTerritoryUniqueID() == _uniqueIdNumericEntryBox._value : true;
}

TOptional<int64> SGsTerrEdNpcFinderCompoundWidget::OnGetUniqueId() const
{
	return _uniqueIdNumericEntryBox._value;
}

void SGsTerrEdNpcFinderCompoundWidget::OnSetUniqueId(int64 inValue, ETextCommit::Type inType)
{
	_uniqueIdNumericEntryBox._value = inValue;
}

ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetMaxSpawnNumConditionCheckState() const
{
	return _spawnNumRangeNumericEntryBox.GetCheckState();
}

void SGsTerrEdNpcFinderCompoundWidget::OnMaxSpawnNumConditionCheckChanged(ECheckBoxState inState)
{
	_spawnNumRangeNumericEntryBox.SetConditionCheckState(inState);
}

bool SGsTerrEdNpcFinderCompoundWidget::IsValidMaxSpawnNum(AGsTerritoryNpc* inNpc)
{
	if (_spawnNumRangeNumericEntryBox._isCheck)
	{
		return _spawnNumRangeNumericEntryBox.IsValid(inNpc->_spawnInfo.MaxSpawnNum);		
	}

	return true;
}

TOptional<int32> SGsTerrEdNpcFinderCompoundWidget::OnGetMinMaxSpawnNum() const
{
	return TOptional<int32>(_spawnNumRangeNumericEntryBox._minValue);
}

void SGsTerrEdNpcFinderCompoundWidget::OnSetMinMaxSpawnNum(int32 inValue, ETextCommit::Type inType)
{
	_spawnNumRangeNumericEntryBox._minValue = inValue;	
}

TOptional<int32> SGsTerrEdNpcFinderCompoundWidget::OnGetMaxMaxSpawnNum() const
{
	return TOptional<int32>(_spawnNumRangeNumericEntryBox._maxValue);
}

void SGsTerrEdNpcFinderCompoundWidget::OnSetMaxMaxSpawnNum(int32 inValue, ETextCommit::Type inType)
{	
	_spawnNumRangeNumericEntryBox._maxValue = inValue;
}

ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetInitSpawnDelayConditionCheckState() const
{
	return _initSpawnDelayRangeNumericEntryBox.GetCheckState();
}

void SGsTerrEdNpcFinderCompoundWidget::OnInitSpawnDelayConditionCheckChanged(ECheckBoxState inState)
{
	_initSpawnDelayRangeNumericEntryBox.SetConditionCheckState(inState);
}

bool SGsTerrEdNpcFinderCompoundWidget::IsValidInitSpawnDelay(AGsTerritoryNpc* inNpc)
{
	if (_initSpawnDelayRangeNumericEntryBox._isCheck)
	{
		return _initSpawnDelayRangeNumericEntryBox.IsValid(inNpc->_spawnInfo.InitSpawnDelay);
	}

	return true;
}

//======================= init spawn delay ==================================
TOptional<int32> SGsTerrEdNpcFinderCompoundWidget::OnGetMinInitSpawnDelay() const
{
	return _initSpawnDelayRangeNumericEntryBox._minValue;
}

void SGsTerrEdNpcFinderCompoundWidget::OnSetMinInitSpawnDelay(int32 inValue, ETextCommit::Type inType)
{
	_initSpawnDelayRangeNumericEntryBox._minValue = inValue;	
}

TOptional<int32> SGsTerrEdNpcFinderCompoundWidget::OnGetMaxInitSpawnDelay() const
{
	return _initSpawnDelayRangeNumericEntryBox._maxValue;
}

void SGsTerrEdNpcFinderCompoundWidget::OnSetMaxInitSpawnDelay(int32 inValue, ETextCommit::Type inType)
{
	_initSpawnDelayRangeNumericEntryBox._maxValue = inValue;	
}

//==================== random spawn ===================================

ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetRandomSpawnCheckState() const
{
	return (_isCheckRandomSpawn ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdNpcFinderCompoundWidget::OnRandomSpawnCheckChanged(ECheckBoxState inState)
{
	_isCheckRandomSpawn = (inState == ECheckBoxState::Checked ? true : false);
}

ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetRandomSpawnConditionCheckState() const
{
	return (_isCheckRandomSpawnCondition ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void SGsTerrEdNpcFinderCompoundWidget::OnRandomSpawnConditionCheckChanged(ECheckBoxState inState)
{
	_isCheckRandomSpawnCondition = (inState == ECheckBoxState::Checked ? true : false);
}

bool SGsTerrEdNpcFinderCompoundWidget::IsValidRandomSpawn(AGsTerritoryNpc* inNpc)
{
	return _isCheckRandomSpawnCondition ? inNpc->_spawnInfo.CanRandomLocationSpawn == _isCheckRandomSpawn : true;
}
//======================= respawn interval ==================================
ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetRespawnIntervalConditionCheckState() const
{
	return _respawnIntervalRangeNumericEntryBox.GetCheckState();
}

void SGsTerrEdNpcFinderCompoundWidget::OnRespawnIntervalConditionCheckChanged(ECheckBoxState inState)
{
	_respawnIntervalRangeNumericEntryBox.SetConditionCheckState(inState);
}

TOptional<int32> SGsTerrEdNpcFinderCompoundWidget::OnGetMinRespawnInterval() const
{
	return _respawnIntervalRangeNumericEntryBox._minValue;
}

void SGsTerrEdNpcFinderCompoundWidget::OnSetMinRespawnInterval(int32 inValue, ETextCommit::Type inType)
{
	_respawnIntervalRangeNumericEntryBox._minValue = inValue;
}

TOptional<int32> SGsTerrEdNpcFinderCompoundWidget::OnGetMaxRespawnInterval() const
{
	return _respawnIntervalRangeNumericEntryBox._maxValue;
}

void SGsTerrEdNpcFinderCompoundWidget::OnSetMaxRespawnInterval(int32 inValue, ETextCommit::Type inType)
{
	_respawnIntervalRangeNumericEntryBox._maxValue = inValue;
}

bool SGsTerrEdNpcFinderCompoundWidget::IsValidRespawnInterval(AGsTerritoryNpc* inNpc)
{
	if (_respawnIntervalRangeNumericEntryBox._isCheck)
	{
		return _respawnIntervalRangeNumericEntryBox.IsValid(inNpc->_spawnInfo.RespawnInterval);
	}

	return true;
}

bool SGsTerrEdNpcFinderCompoundWidget::IsValidSpawnLevel(class AGsTerritoryNpc* inNpc)
{
	if (_spawnLevelNumericEntryBox._isCheck)
	{
		return _spawnLevelNumericEntryBox._value == inNpc->_spawnInfo.SpawnLevel;
	}

	return true;
}

ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetOnlySelectActorSpawnCheckState() const
{
	return FGsTerrEdSlateUtil::IsShowOnlySelectActor() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdNpcFinderCompoundWidget::OnOnlySelectActorSpawnChanged(ECheckBoxState inState) const
{
	bool isChecked = (ECheckBoxState::Checked == inState);
	
	FGsTerrEdSlateUtil::SetShowOnlySelectActor(isChecked);
}

ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetSelectAllActorState() const
{
	return _isCheckSelectAllActor ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

void SGsTerrEdNpcFinderCompoundWidget::OnSelectAllActorChanged(ECheckBoxState inState)
{
	_isCheckSelectAllActor = (ECheckBoxState::Checked == inState);

	SetSelectAllActor(_isCheckSelectAllActor);
}

void SGsTerrEdNpcFinderCompoundWidget::InitNpcActorList()
{	
	_npcFinderActorListView._valueList.Empty();	
	for (AActor* actor : _npcFinderActorListView._actorList)
	{
		_npcFinderActorListView._valueList.Add(MakeShareable(new FString(actor->GetActorLabel())));
	}
}

FText SGsTerrEdNpcFinderCompoundWidget::GetSelectedNpcActorText() const
{
	return FText::FromString(_selectedNpcActorString);
}

TSharedRef<ITableRow> SGsTerrEdNpcFinderCompoundWidget::OnGenerateNpcActorWidget(TSharedPtr<FString> inItem, const TSharedRef<STableViewBase>& inOwnerTable)
{
	//Create the row
	return	SNew(STableRow< TSharedPtr<FString> >, inOwnerTable)
		.Padding(2.0f)
		[
			SNew(STextBlock).Text(FText::FromString(*inItem.Get()))
		];
}

void SGsTerrEdNpcFinderCompoundWidget::OnDoubleClickNpcActor(TSharedPtr<FString> InNewSelection)
{

}

void SGsTerrEdNpcFinderCompoundWidget::OnSelectionChangeNpcActor(TSharedPtr<FString> InNewSelection, ESelectInfo::Type InSelectInfo)
{
	TArray<TSharedPtr<FString>> selectItems = _npcFinderActorListView._listView->GetSelectedItems();
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		_npcFinderActorListView._listView->ClearSelection();
		return;
	}
	
	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritoryNpc::StaticClass(), actorList);

	UGsTerritoryLevelUtil::SetSelectedLevelActors(actorList, selectItems);

	_isCheckSelectAllActor = (selectItems.Num() == _npcFinderActorListView._valueList.Num());
	_selectAllActorCheckBox->Invalidate(EInvalidateWidgetReason::Layout);

	GUnrealEd->Exec(world, TEXT("CAMERA ALIGN ACTIVEVIEWPORTONLY"));

	_selectCount = selectItems.Num();
	if (_selectCountTextBlock.IsValid())
	{
		_selectCountTextBlock->SetText(FText::FromString(FString::FromInt(_selectCount)));
	}
}

void SGsTerrEdNpcFinderCompoundWidget::SetSelectAllActor(bool inAllSelect)
{
	if (inAllSelect)
	{
		//모든 npc id 선택						
		_npcFinderActorListView._listView->SetItemSelection(_npcFinderActorListView._valueList, true);
		_npcFinderActorListView._listView->RequestListRefresh();
	}	
	else
	{
		_npcFinderActorListView._listView->SetItemSelection(_npcFinderActorListView._valueList, false);
		_npcFinderActorListView._listView->RequestListRefresh();
	}
}

int32 SGsTerrEdNpcFinderCompoundWidget::GetNpcId(const FString& inNpcId)
{
	FString npcId = inNpcId;
	TArray<FString> tIdParses;
	npcId.ParseIntoArray(tIdParses, TEXT("-"), true);
	if (tIdParses.Num() > 0)
	{
		return  FCString::Atoi(*tIdParses[0]);		
	}

	return 0;
}

bool SGsTerrEdNpcFinderCompoundWidget::IsValidNpcId(AGsTerritoryNpc* inNpc)
{
	return _npcIdSearchableComboBox._isCheck ? inNpc->_spawnInfo.TargetID == _selectNpcId : true;
}

void SGsTerrEdNpcFinderCompoundWidget::OnFieldColumnResized(const float inWidth)
{
	_fieldColumnSize = inWidth;

	UE_LOG(LogTerritoryEditor, Log, TEXT("Set field width : %d"), _fieldColumnSize);
}

void SGsTerrEdNpcFinderCompoundWidget::OnIncludeConditionColumnResized(const float inWidth)
{
	_conditionColumnSize = inWidth;

	UE_LOG(LogTerritoryEditor, Log, TEXT("Set condition width : %d"), _conditionColumnSize);
}

TSharedPtr<SWidget> SGsTerrEdNpcFinderCompoundWidget::CreateNpcIdRangeWidget()
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
			//min npc id searchbox
			+ SHorizontalBox::Slot()			
			.Padding(_fieldLeftRowPadding)
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Min")))
			]

			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_npcIdRangeSearchableComboBox._minSearchableComboBox, SSearchableComboBox)
				.OptionsSource(&_npcIdList)
				.OnSelectionChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnMinNpcIdSelectionChanged)
				.OnGenerateWidget(this, &SGsTerrEdNpcFinderCompoundWidget::OnGenerateNpcIdWidget)
				.InitiallySelectedItem(_npcIdRangeSearchableComboBox._maxSelectStringPtr)
				.Content()
				[
					SNew(STextBlock).Text(this, &SGsTerrEdNpcFinderCompoundWidget::GetSelectedMinNpcIdText)
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
				SAssignNew(_npcIdRangeSearchableComboBox._maxSearchableComboBox, SSearchableComboBox)
				.OptionsSource(&_npcIdList)
				.OnSelectionChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnMaxNpcIdSelectionChanged)
				.OnGenerateWidget(this, &SGsTerrEdNpcFinderCompoundWidget::OnGenerateNpcIdWidget)
				.InitiallySelectedItem(_npcIdRangeSearchableComboBox._minSelectStringPtr)
				.Content()
				[
					SNew(STextBlock).Text(this, &SGsTerrEdNpcFinderCompoundWidget::GetSelectedMaxNpcIdText)
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
				SAssignNew(_npcIdRangeSearchableComboBox._checkBox, SCheckBox)
				.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetNpcIdRangeConditionCheckState)
				.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnNpcIdRangeConditionCheckChanged)
			]
		];
}

TSharedPtr<SWidget> SGsTerrEdNpcFinderCompoundWidget::CreateNpcIdWidget()
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
			SAssignNew(_npcIdSearchableComboBox._searchableComboBox, SSearchableComboBox)
			.OptionsSource(&_npcIdList)
		.OnSelectionChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnNpcIdSelectionChanged)
		.OnGenerateWidget(this, &SGsTerrEdNpcFinderCompoundWidget::OnGenerateNpcIdWidget)
		.InitiallySelectedItem(_npcIdSearchableComboBox._selectStringPtr)
		.Content()
		[
			SNew(STextBlock).Text(this, &SGsTerrEdNpcFinderCompoundWidget::GetSelectedNpcIdText)
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
			SAssignNew(_npcIdSearchableComboBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetNpcIdConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnNpcIdConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdNpcFinderCompoundWidget::CreateUniqueIdWidget()
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
			.Value(this, &SGsTerrEdNpcFinderCompoundWidget::OnGetUniqueId)
			.OnValueCommitted(this, &SGsTerrEdNpcFinderCompoundWidget::OnSetUniqueId)
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
			.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetUniqueIdConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnUniqueIdConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdNpcFinderCompoundWidget::CreateMaxSpawnNumWidget()
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
				SAssignNew(_spawnNumRangeNumericEntryBox._minNumericEntryBox, SNumericEntryBox<int32>)
				.Value(this, &SGsTerrEdNpcFinderCompoundWidget::OnGetMinMaxSpawnNum)
				.OnValueCommitted(this, &SGsTerrEdNpcFinderCompoundWidget::OnSetMinMaxSpawnNum)
			]
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Max")))
			]
			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_spawnNumRangeNumericEntryBox._maxNumericEntryBox, SNumericEntryBox<int32>)
				.Value(this, &SGsTerrEdNpcFinderCompoundWidget::OnGetMaxMaxSpawnNum)
				.OnValueCommitted(this, &SGsTerrEdNpcFinderCompoundWidget::OnSetMaxMaxSpawnNum)
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
				SAssignNew(_spawnNumRangeNumericEntryBox._checkBox, SCheckBox)
				.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetMaxSpawnNumConditionCheckState)
				.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnMaxSpawnNumConditionCheckChanged)
			]
		];
}

TSharedPtr<SWidget> SGsTerrEdNpcFinderCompoundWidget::CreateInitSpawnDelayWidget()
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
				SAssignNew(_initSpawnDelayRangeNumericEntryBox._minNumericEntryBox, SNumericEntryBox<int32>)
				.Value(this, &SGsTerrEdNpcFinderCompoundWidget::OnGetMinInitSpawnDelay)
				.OnValueCommitted(this, &SGsTerrEdNpcFinderCompoundWidget::OnSetMinInitSpawnDelay)
			]
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock).Text(FText::FromString(TEXT("Max")))
			]
			+ SHorizontalBox::Slot()
			.MaxWidth(800)
			.Padding(_fieldLeftRowPadding)
			[
				SAssignNew(_initSpawnDelayRangeNumericEntryBox._maxNumericEntryBox, SNumericEntryBox<int32>)
				.Value(this, &SGsTerrEdNpcFinderCompoundWidget::OnGetMaxInitSpawnDelay)
				.OnValueCommitted(this, &SGsTerrEdNpcFinderCompoundWidget::OnSetMaxInitSpawnDelay)
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
			SAssignNew(_initSpawnDelayRangeNumericEntryBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetInitSpawnDelayConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnInitSpawnDelayConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdNpcFinderCompoundWidget::CreateRandomSpawnWidget()
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
			.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetRandomSpawnCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnRandomSpawnCheckChanged)
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
			.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetRandomSpawnConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnRandomSpawnConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdNpcFinderCompoundWidget::CreateRespawnInterval()
{
	TSharedPtr<SWidget> respawnIntervalName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Respawn Interval"));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			respawnIntervalName.ToSharedRef()
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
			SAssignNew(_respawnIntervalRangeNumericEntryBox._minNumericEntryBox, SNumericEntryBox<int32>)
			.Value(this, &SGsTerrEdNpcFinderCompoundWidget::OnGetMinRespawnInterval)
		.OnValueCommitted(this, &SGsTerrEdNpcFinderCompoundWidget::OnSetMinRespawnInterval)
		]
	+ SHorizontalBox::Slot()
		[
			SNew(STextBlock).Text(FText::FromString(TEXT("Max")))
		]
	+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_respawnIntervalRangeNumericEntryBox._maxNumericEntryBox, SNumericEntryBox<int32>)
			.Value(this, &SGsTerrEdNpcFinderCompoundWidget::OnGetMaxRespawnInterval)
		.OnValueCommitted(this, &SGsTerrEdNpcFinderCompoundWidget::OnSetMaxRespawnInterval)
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
			SAssignNew(_respawnIntervalRangeNumericEntryBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetRespawnIntervalConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnRespawnIntervalConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdNpcFinderCompoundWidget::CreateSpawnLevel()
{
	TSharedPtr<SWidget> spawnLevelName = FGsTerrEdSlateUtil::CreateFieldText(TEXT("Spawn level"));

	return SNew(SSplitter)
		.Style(FEditorStyle::Get(), "DetailsView.Splitter")
		.PhysicalSplitterHandleSize(1.0f)
		.HitDetectionSplitterHandleSize(5.0f)

		+ SSplitter::Slot()
		.Value(_fieldLeftColumnSize)
		[
			spawnLevelName.ToSharedRef()
		]

	+ SSplitter::Slot()
		.Value(_fieldRightColumnSize)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.MaxWidth(800)
		.Padding(_fieldLeftRowPadding)
		[
			SAssignNew(_spawnLevelNumericEntryBox._numericEntryBox, SNumericEntryBox<int32>)
			.Value(this, &SGsTerrEdNpcFinderCompoundWidget::OnGetSpawnLevel)
		.OnValueCommitted(this, &SGsTerrEdNpcFinderCompoundWidget::OnSetSpawnLevel)
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
			SAssignNew(_spawnLevelNumericEntryBox._checkBox, SCheckBox)
			.IsChecked(this, &SGsTerrEdNpcFinderCompoundWidget::GetSpawnLevelConditionCheckState)
		.OnCheckStateChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnSpawnLevelConditionCheckChanged)
		]
		];
}

TSharedPtr<SWidget> SGsTerrEdNpcFinderCompoundWidget::CreateNpcActorFindWidget()
{
	return SNew(SBox)
		.Padding(FMargin(4.0f, 4.0f))
		.HeightOverride(300.0f)
		[
			SAssignNew(_npcFinderActorListView._listView, SListView<TSharedPtr<FString>>)
			.ListItemsSource(&_npcFinderActorListView._valueList) //The Items array is the source of this listview
		.OnGenerateRow(this, &SGsTerrEdNpcFinderCompoundWidget::OnGenerateNpcActorWidget)
		.OnSelectionChanged(this, &SGsTerrEdNpcFinderCompoundWidget::OnSelectionChangeNpcActor)
		.ItemHeight(24)
		.SelectionMode(ESelectionMode::Multi)
		.ScrollbarVisibility(EVisibility::All)
		];
}


TOptional<int32> SGsTerrEdNpcFinderCompoundWidget::OnGetSpawnLevel() const
{
	return _spawnLevelNumericEntryBox._value;
}

void SGsTerrEdNpcFinderCompoundWidget::OnSetSpawnLevel(int32 inValue, ETextCommit::Type inType)
{
	_spawnLevelNumericEntryBox._value = inValue;
}

ECheckBoxState SGsTerrEdNpcFinderCompoundWidget::GetSpawnLevelConditionCheckState() const
{
	return _spawnLevelNumericEntryBox.GetCheckState();
}

void SGsTerrEdNpcFinderCompoundWidget::OnSpawnLevelConditionCheckChanged(ECheckBoxState inState)
{
	_spawnLevelNumericEntryBox.SetConditionCheckState(inState);
}

FReply SGsTerrEdNpcFinderCompoundWidget::OnFindNpc()
{
	UWorld* world = GEditor->GetEditorWorldContext().World();	
	if (nullptr == world)
	{
		return FReply::Handled();
	}

	if (false == CanFindNpc())
	{
		return FReply::Handled();
	}

	ULevel* currentLevel = world->GetCurrentLevel();
	if (nullptr == currentLevel)
	{
		return FReply::Handled();
	}
	
	_npcFinderActorListView._actorList.Empty();
	for (AActor* actor : currentLevel->Actors)
	{
		AGsTerritoryNpc* npc = Cast<AGsTerritoryNpc>(actor);
		if (npc)
		{
			if (IsValidNpcIdRange(npc)
				&& IsValidNpcId(npc)
				&& IsValidUniqueId(npc)
				&& IsValidMaxSpawnNum(npc)
				&& IsValidInitSpawnDelay(npc)
				&& IsValidRandomSpawn(npc)
				&& IsValidRespawnInterval(npc)
				&& IsValidSpawnLevel(npc))
			{
				_npcFinderActorListView._actorList.Add(actor);
			}
		}
	}

	USelection* select = GEditor->GetSelectedActors();
	if (select)
	{
		UGsTerritoryLevelUtil::SetSelectedLevelActors(_npcFinderActorListView._actorList);
	}

	InitNpcActorList();

	_isCheckSelectAllActor = true;
	_selectAllActorCheckBox->Invalidate(EInvalidateWidgetReason::Layout);
	SetSelectAllActor(_isCheckSelectAllActor);

	_totalCount = _npcFinderActorListView._actorList.Num();
	if (_totalCountTextBlock.IsValid())
	{
		_totalCountTextBlock->SetText(FText::FromString(FString::FromInt(_totalCount)));
	}

	return FReply::Handled();
}

FReply SGsTerrEdNpcFinderCompoundWidget::OnDelectAll()
{
	_isCheckSelectAllActor = false;
	_selectAllActorCheckBox.Get()->Invalidate(EInvalidateWidgetReason::Layout);

	SetSelectAllActor(false);

	return FReply::Handled();
}

bool SGsTerrEdNpcFinderCompoundWidget::CanFindNpc()
{
	bool isValidNpcIdRange = true;
	if (_npcIdRangeSearchableComboBox._isCheck)
	{
		isValidNpcIdRange = (_selectMinNpcId <= _selectMaxNpcId);
		if (false == isValidNpcIdRange)
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Npc finder"), TEXT("Npc id range is invalid"));
		}
	}

	bool isValidMaxSpawnNum = true;
	if (_spawnNumRangeNumericEntryBox._isCheck)
	{
		isValidMaxSpawnNum = _spawnNumRangeNumericEntryBox.IsValid();
		if (false == isValidMaxSpawnNum)
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Npc finder"), TEXT("Max spawn range is invalid"));
		}
	}

	bool isValidInitSpawnDelay = true;
	if (_initSpawnDelayRangeNumericEntryBox._isCheck)
	{
		isValidInitSpawnDelay = _initSpawnDelayRangeNumericEntryBox.IsValid();
		if (false == isValidInitSpawnDelay)
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Npc finder"), TEXT("Init spawn range is invalid"));
		}
	}

	bool isValidRespawnInterval = true;
	if (_respawnIntervalRangeNumericEntryBox._isCheck)
	{
		isValidRespawnInterval = _respawnIntervalRangeNumericEntryBox.IsValid();
		if (false == isValidRespawnInterval)
		{
			FGsTerritoryEditorUtil::OpenWarningDialog(TEXT("Npc finder"), TEXT("respawn interval is invalid"));
		}
	}

	return isValidNpcIdRange
		&& isValidMaxSpawnNum
		&& isValidInitSpawnDelay
		&& isValidRespawnInterval;
}

#undef LOCTEXT_NAMESPACE
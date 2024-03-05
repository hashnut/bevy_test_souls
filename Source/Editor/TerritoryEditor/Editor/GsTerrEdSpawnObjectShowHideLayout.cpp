// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrEdSpawnObjectShowHideLayout.h"

//UE4
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "SlateFwd.h"
#include "IDetailChildrenBuilder.h"
#include "SlateFwd.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SComboButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Toolkits/AssetEditorManager.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Subsystems/AssetEditorSubsystem.h"

//T1 Project
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "RSQLEditor/Public/RSQLEditorDelegates.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

//Territory editor
#include "Blutility/Input/GsTerritorySpawnActorInput.h"
#include "Editor/GsTerritoryLevelEvent.h"
#include "GameObject/Prop/GsSchemaPropData.h"
#include "Quest/GsSchemaQuest.h"
#include "TerritoryEditor.h"

#define LOCTEXT_NAMESPACE "TerritoryNpcDataDetail"

TSharedRef<IPropertyTypeCustomization> FGsTerrEdSpawnObjectShowHideLayout::MakeInstance()
{
	return MakeShareable(new FGsTerrEdSpawnObjectShowHideLayout());
}

void FGsTerrEdSpawnObjectShowHideLayout::CustomizeHeader(TSharedRef<IPropertyHandle> inPropertyHandle
	, FDetailWidgetRow& inHeaderRow
	, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle
	inHeaderRow.NameContent()
		[
			inPropertyHandle->CreatePropertyNameWidget()
		];
}

void FGsTerrEdSpawnObjectShowHideLayout::CustomizeChildren(TSharedRef<IPropertyHandle> inPropertyHandle, IDetailChildrenBuilder& inChildBuilder, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle			
	InvalidConditionQuestIDInfos();

	uint32 num = 0;
	inPropertyHandle->GetNumChildren(num);

	for (uint32 i = 0; i < num; ++i)
	{
		TSharedPtr<IPropertyHandle> handle = inPropertyHandle->GetChildHandle(i);

		if (handle.IsValid())
		{
			FString name = handle.Get()->GetPropertyDisplayName().ToString();

			if (name.Equals("Start Condition Quest Id"))
			{
				_startQuestConditionComboBox._selectStringHandle = handle;
				CreateStartConditionQuestIDSearchComboBox(inChildBuilder);
			}
			else if (name.Equals("End Condition Quest Id"))
			{
				_endQuestConditionComboBox._selectStringHandle = handle;
				CreateEndConditionQuestIDSearchComboBox(inChildBuilder);
			}
			else
			{
				inChildBuilder.AddProperty(handle.ToSharedRef());
			}
		}
	}
}
//general ==========================================================================================================================================================
TSharedRef<SWidget> FGsTerrEdSpawnObjectShowHideLayout::OnGenerateConditionQuestIDWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGsTerrEdSpawnObjectShowHideLayout::InvalidConditionQuestIDInfos()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdSpawnObjectShowHideLayout::InvalidConditionQuestIDInfos"));

	UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
	const UGsTable* table = mgr.GetTable(FGsSchemaQuest::StaticStruct());
	TArray<const FGsSchemaQuest*> dataList;
	table->GetAllRows<FGsSchemaQuest>(dataList);

	TArray<FGsTerritoryKeyDescInfo> keyDescList;

	//add sort id
	for (const FGsSchemaQuest* data : dataList)
	{
		FGsTerritoryKeyDescInfo temp;
		temp.id = data->id;
		temp.desc = data->desc;

		keyDescList.Add(temp);
	}

	keyDescList.Sort([=](const FGsTerritoryKeyDescInfo& source, const FGsTerritoryKeyDescInfo& dest)->bool {
		return source.id < dest.id;
		});

	//add npc id 
	_conditionQuestIdList.Empty();

	FString temp = FString::FromInt(0) + TEXT(" - ");
	_conditionQuestIdList.Add(MakeShareable(new FString(temp)));

	for (const FGsTerritoryKeyDescInfo& data : keyDescList)
	{
		temp = FString::Printf(TEXT("%lld"), data.id) + TEXT("-") + data.desc;
		_conditionQuestIdList.Add(MakeShareable(new FString(temp)));
	}
}

//start ==========================================================================================================================================================
void FGsTerrEdSpawnObjectShowHideLayout::OnChangedStartConditionQuestID(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
{
	if (inSelectInfo != ESelectInfo::Direct)
	{
		FString value = *inNewSelection.Get();
		_startQuestConditionComboBox._selectedString = value;

		TArray<FString> valueParses;
		value.ParseIntoArray(valueParses, TEXT("-"), true);

		if (valueParses.Num() > 0)
		{
			//int32 id = FCString::Atoi(*valueParses[0]);
			_startQuestConditionComboBox._selectStringHandle->SetValue(valueParses[0]);
		}
	}
}

void FGsTerrEdSpawnObjectShowHideLayout::InitStartConditionQuestID()
{
	if (_conditionQuestIdList.Num() > 0)
	{
		FString questId;
		_startQuestConditionComboBox._selectStringHandle->GetValue(questId);

		for (TSharedPtr<FString> stringPtr : _conditionQuestIdList)
		{
			if ((*stringPtr) == questId)
			{
				_startQuestConditionComboBox._selectStringPtr = stringPtr;
				break;
			}
		}

		_startQuestConditionComboBox._selectedString = questId;		
	}
	else
	{
		_startQuestConditionComboBox._selectStringPtr = MakeShareable(new FString(TEXT("No id in table")));
		_startQuestConditionComboBox._selectStringHandle->SetValue(TEXT("0"));
	}
}

void FGsTerrEdSpawnObjectShowHideLayout::CreateStartConditionQuestIDSearchComboBox(IDetailChildrenBuilder& inChildBuilder)
{
	InitStartConditionQuestID();

	inChildBuilder.AddProperty(_startQuestConditionComboBox._selectStringHandle.ToSharedRef())
		.CustomWidget()
		.NameContent()
		[
			_startQuestConditionComboBox._selectStringHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			SAssignNew(_startQuestConditionComboBox._searchableComboBox, SSearchableComboBox)
			.OptionsSource(&_conditionQuestIdList)
		.OnGenerateWidget(this, &FGsTerrEdSpawnObjectShowHideLayout::OnGenerateConditionQuestIDWidget)
		.OnSelectionChanged(this, &FGsTerrEdSpawnObjectShowHideLayout::OnChangedStartConditionQuestID)
		.InitiallySelectedItem(_startQuestConditionComboBox._selectStringPtr)
		.ContentPadding(2)
		.Content()
		[
			SNew(STextBlock)
			.Text(this, &FGsTerrEdSpawnObjectShowHideLayout::GetSelectedStartConditionQuestIDText)
		]
		];
}

FText FGsTerrEdSpawnObjectShowHideLayout::GetSelectedStartConditionQuestIDText() const
{
	return FText::FromString(_startQuestConditionComboBox._selectedString);
}

int32 FGsTerrEdSpawnObjectShowHideLayout::GetSelectedStartConditionQuestID() const
{
	FString value = _startQuestConditionComboBox._selectedString;
	TArray<FString> valueParses;
	value.ParseIntoArray(valueParses, TEXT("-"), true);
	if (valueParses.Num() == 0)
	{
		return 0;
	}

	return FCString::Atoi(*valueParses[0]);
}

//End===================================================================================

void FGsTerrEdSpawnObjectShowHideLayout::OnChangedEndConditionQuestID(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
{
	if (inSelectInfo != ESelectInfo::Direct)
	{
		FString value = *inNewSelection.Get();
		_endQuestConditionComboBox._selectedString = value;		

		TArray<FString> valueParses;
		value.ParseIntoArray(valueParses, TEXT("-"), true);
		if (valueParses.Num() > 0)
		{			
			_endQuestConditionComboBox._selectStringHandle->SetValue(valueParses[0]);
		}
	}
}

void FGsTerrEdSpawnObjectShowHideLayout::InitEndConditionQuestID()
{
	if (_conditionQuestIdList.Num() > 0)
	{

		FString questId;
		_endQuestConditionComboBox._selectStringHandle->GetValue(questId);

		for (TSharedPtr<FString> stringPtr : _conditionQuestIdList)
		{
			if ((*stringPtr) == questId)
			{
				_endQuestConditionComboBox._selectStringPtr = stringPtr;
				break;
			}
		}

		_endQuestConditionComboBox._selectedString = questId;				
	}
	else
	{
		_endQuestConditionComboBox._selectStringPtr = MakeShareable(new FString(TEXT("No id in table")));
		_endQuestConditionComboBox._selectStringHandle->SetValue(TEXT("0"));
	}
}

void FGsTerrEdSpawnObjectShowHideLayout::CreateEndConditionQuestIDSearchComboBox(IDetailChildrenBuilder& inChildBuilder)
{
	InitEndConditionQuestID();

	inChildBuilder.AddProperty(_endQuestConditionComboBox._selectStringHandle.ToSharedRef())
		.CustomWidget()
		.NameContent()
		[
			_endQuestConditionComboBox._selectStringHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			SAssignNew(_endQuestConditionComboBox._searchableComboBox, SSearchableComboBox)
			.OptionsSource(&_conditionQuestIdList)
		.OnGenerateWidget(this, &FGsTerrEdSpawnObjectShowHideLayout::OnGenerateConditionQuestIDWidget)
		.OnSelectionChanged(this, &FGsTerrEdSpawnObjectShowHideLayout::OnChangedEndConditionQuestID)
		.InitiallySelectedItem(_endQuestConditionComboBox._selectStringPtr)
		.ContentPadding(2)
		.Content()
		[
			SNew(STextBlock)
			.Text(this, &FGsTerrEdSpawnObjectShowHideLayout::GetSelectedEndConditionQuestIDText)
		]
		];
}

FText FGsTerrEdSpawnObjectShowHideLayout::GetSelectedEndConditionQuestIDText() const
{
	return FText::FromString(_endQuestConditionComboBox._selectedString);
}

int32 FGsTerrEdSpawnObjectShowHideLayout::GetSelectedEndConditionQuestID() const
{
	FString value = _endQuestConditionComboBox._selectedString;
	TArray<FString> valueParses;
	value.ParseIntoArray(valueParses, TEXT("-"), true);
	if (valueParses.Num() == 0)
	{
		return 0;
	}

	return FCString::Atoi(*valueParses[0]);
}

#undef LOCTEXT_NAMESPACE
// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritorySpawnLayoutBase.h"

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

#define LOCTEXT_NAMESPACE "TerritoryNpcDataDetail"

void FGsTerritorySpawnLayoutBase::CustomizeHeader(TSharedRef<IPropertyHandle> inPropertyHandle
	, FDetailWidgetRow& inHeaderRow
	, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle
	inHeaderRow.NameContent()
		[
			inPropertyHandle->CreatePropertyNameWidget()
		];
}

void FGsTerritorySpawnLayoutBase::CustomizeChildren(TSharedRef<IPropertyHandle> inPropertyHandle, IDetailChildrenBuilder& inChildBuilder, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle		
	InvalidTIdInfos();
	InvalidConditionQuestIDInfos();

	uint32 num = 0;
	inPropertyHandle->GetNumChildren(num);

	for (uint32 i = 0; i < num; ++i)
	{
		TSharedPtr<IPropertyHandle> handle = inPropertyHandle->GetChildHandle(i);

		if (handle.IsValid())
		{
			FString name = handle.Get()->GetPropertyDisplayName().ToString();

			if (name.Equals("Target ID"))
			{
				_targetIDHandle = handle;
				CreateTIdSearchComboBox(inChildBuilder);
			}
			else if (name.Equals("Condition Quest ID"))
			{
				_conditionQuestIDHandle = handle;
				CreateConditionQuestIDSearchComboBox(inChildBuilder);
			}
			else
			{
				inChildBuilder.AddProperty(handle.ToSharedRef());
			}
		}
	}
}

TSharedRef<SWidget> FGsTerritorySpawnLayoutBase::OnGenerateTIdWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGsTerritorySpawnLayoutBase::OnChangedTId(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
{
	if (inSelectInfo != ESelectInfo::Direct)
	{
		FString value = *inNewSelection.Get();
		_selectedTId = value;
		TArray<FString> tIdParses;
		value.ParseIntoArray(tIdParses, TEXT("-"), true);
		if (tIdParses.Num() > 0)
		{			
			int32 id = FCString::Atoi(*tIdParses[0]);
			_targetIDHandle->SetValue(id);
		}		
	}
}

void FGsTerritorySpawnLayoutBase::InitTId()
{	
	if (_tIdList.Num() > 0)
	{
		_tIdText = _tIdList[0];
		_selectedTId = (*_tIdText.Get());
		_targetIDHandle->SetValue(FCString::Atoi(*_selectedTId));
	}
	else
	{
		_tIdText = MakeShareable(new FString(TEXT("No id in table")));
		_targetIDHandle->SetValue(0);
	}
}

void FGsTerritorySpawnLayoutBase::CreateTIdSearchComboBox(IDetailChildrenBuilder& inChildBuilder)
{
	InitTId();

	inChildBuilder.AddProperty(_targetIDHandle.ToSharedRef())
		.CustomWidget()
		.NameContent()
		[
			_targetIDHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			SAssignNew(_tIdSearchableComboBox, SSearchableComboBox)
			.OptionsSource(&_tIdList)
			.OnGenerateWidget(this, &FGsTerritorySpawnLayoutBase::OnGenerateTIdWidget)
			.OnSelectionChanged(this, &FGsTerritorySpawnLayoutBase::OnChangedTId)
			.InitiallySelectedItem(_tIdText)
			.ContentPadding(2)
			.Content()
			[
				SNew(STextBlock)
				.Text(this, &FGsTerritorySpawnLayoutBase::GetSelectedTIdText)
			]
		];
}

FText FGsTerritorySpawnLayoutBase::GetSelectedTIdText() const
{
	return FText::FromString(_selectedTId);
}

int32 FGsTerritorySpawnLayoutBase::GetSelectedTId() const
{
	FString value = _selectedTId;
	TArray<FString> tIdParses;
	value.ParseIntoArray(tIdParses, TEXT("-"), true);
	if (tIdParses.Num() == 0)
	{
		return 0;
	}

	return FCString::Atoi(*tIdParses[0]);
}

TSharedRef<SWidget> FGsTerritorySpawnLayoutBase::OnGenerateConditionQuestIDWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGsTerritorySpawnLayoutBase::OnChangedConditionQuestID(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
{
	if (inSelectInfo != ESelectInfo::Direct)
	{
		FString value = *inNewSelection.Get();
		_selectedConditionQuestId = value;

		TArray<FString> valueParses;
		value.ParseIntoArray(valueParses, TEXT("-"), true);

		if (valueParses.Num() > 0)
		{
			//int32 id = FCString::Atoi(*valueParses[0]);
			_conditionQuestIDHandle->SetValue(valueParses[0]);
		}
	}
}


void FGsTerritorySpawnLayoutBase::InvalidConditionQuestIDInfos()
{
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

	for (const FGsTerritoryKeyDescInfo& data : keyDescList)
	{
		FString temp = FString::FromInt(data.id) + TEXT("-") + data.desc;
		_conditionQuestIdList.Add(MakeShareable(new FString(temp)));
	}
}

void FGsTerritorySpawnLayoutBase::InitConditionQuestID()
{
	if (_conditionQuestIdList.Num() > 0)
	{
		_conditionQuestIdText = _conditionQuestIdList[0];
		_selectedConditionQuestId = (*_conditionQuestIdText.Get());
		_conditionQuestIDHandle->SetValue(_selectedConditionQuestId);
	}
	else
	{
		_conditionQuestIdText = MakeShareable(new FString(TEXT("No id in table")));
		_conditionQuestIDHandle->SetValue(TEXT("0"));
	}
}

void FGsTerritorySpawnLayoutBase::CreateConditionQuestIDSearchComboBox(IDetailChildrenBuilder& inChildBuilder)
{
	InitConditionQuestID();

	inChildBuilder.AddProperty(_conditionQuestIDHandle.ToSharedRef())
		.CustomWidget()
		.NameContent()
		[
			_conditionQuestIDHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			SAssignNew(_conditionQuestIdSearchableComboBox, SSearchableComboBox)
			.OptionsSource(&_conditionQuestIdList)
		.OnGenerateWidget(this, &FGsTerritorySpawnLayoutBase::OnGenerateConditionQuestIDWidget)
		.OnSelectionChanged(this, &FGsTerritorySpawnLayoutBase::OnChangedConditionQuestID)
		.InitiallySelectedItem(_conditionQuestIdText)
		.ContentPadding(2)
		.Content()
		[
			SNew(STextBlock)
			.Text(this, &FGsTerritorySpawnLayoutBase::GetSelectedConditionQuestIDText)
		]
		];
}

FText FGsTerritorySpawnLayoutBase::GetSelectedConditionQuestIDText() const
{
	return FText::FromString(_selectedConditionQuestId);
}

int32 FGsTerritorySpawnLayoutBase::GetSelectedConditionQuestID() const
{
	FString value = _selectedConditionQuestId;
	TArray<FString> valueParses;
	value.ParseIntoArray(valueParses, TEXT("-"), true);
	if (valueParses.Num() == 0)
	{
		return 0;
	}

	return FCString::Atoi(*valueParses[0]);
}

#undef LOCTEXT_NAMESPACE
// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrEdQuestSoundLayout.h"

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

TSharedRef<IPropertyTypeCustomization> FGsTerrEdQuestSoundLayout::MakeInstance()
{
	return MakeShareable(new FGsTerrEdQuestSoundLayout());
}

void FGsTerrEdQuestSoundLayout::CustomizeHeader(TSharedRef<IPropertyHandle> inPropertyHandle
	, FDetailWidgetRow& inHeaderRow
	, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle
	inHeaderRow.NameContent()
		[
			inPropertyHandle->CreatePropertyNameWidget()
		];
}

void FGsTerrEdQuestSoundLayout::CustomizeChildren(TSharedRef<IPropertyHandle> inPropertyHandle, IDetailChildrenBuilder& inChildBuilder, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle			
	InvalidQuestIDInfos();

	uint32 num = 0;
	inPropertyHandle->GetNumChildren(num);
	for (uint32 i = 0; i < num; ++i)
	{
		TSharedPtr<IPropertyHandle> handle = inPropertyHandle->GetChildHandle(i);
		if (handle.IsValid())
		{
			FString name = handle.Get()->GetPropertyDisplayName().ToString();

			if (name.Equals(" Start Quest Id"))
			{				
				//이름만 StringHandle 일뿐 실제 StringHandle이 아님, int64 처리 
				_startQuestComboBox._selectStringHandle = handle;
				CreateQuestIDSearchComboBox(_startQuestComboBox
				, _startQuestId
				, inChildBuilder
				, [&](TSharedPtr<FString> text, ESelectInfo::Type selectInfo)->void{
						ChangedStartQuestID(_startQuestComboBox, _startQuestId, text, selectInfo);
				}
				, [&]()->FText {
					return GetSelectedQuestIDText(_startQuestId);
				});
			}
			else if (name.Equals(" End Quest Id"))
			{
				//이름만 StringHandle 일뿐 실제 StringHandle이 아님, int64 처리 
				_endQuestComboBox._selectStringHandle = handle;
				CreateQuestIDSearchComboBox(_endQuestComboBox
					, _endQuestId
					, inChildBuilder
					, [&](TSharedPtr<FString> text, ESelectInfo::Type selectInfo)->void {
						ChangedStartQuestID(_endQuestComboBox, _endQuestId, text, selectInfo);
					}
					, [&]()->FText {
						return GetSelectedQuestIDText(_endQuestId);
					});				
			}
			else
			{
				inChildBuilder.AddProperty(handle.ToSharedRef());
			}
		}
	}
}
//general ==========================================================================================================================================================
TSharedRef<SWidget> FGsTerrEdQuestSoundLayout::OnGenerateQuestIDWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGsTerrEdQuestSoundLayout::InvalidQuestIDInfos()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdEnvConditionLayout::InvalidConditionQuestIDInfos"));

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
	_questIdList.Empty();

	FString temp = FString::FromInt(0) + TEXT(" - ");
	_questIdList.Add(MakeShareable(new FString(temp)));

	for (const FGsTerritoryKeyDescInfo& data : keyDescList)
	{
		temp = FString::Printf(TEXT("%lld"), data.id) + TEXT("-") + data.desc;
		_questIdList.Add(MakeShareable(new FString(temp)));
	}
}

//start ==========================================================================================================================================================
void FGsTerrEdQuestSoundLayout::ChangedStartQuestID(FGsTerrEdSearchableComboBox& inComboBox, FString& inQuestId, TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
{
	if (inSelectInfo != ESelectInfo::Direct)
	{
		FString value = *inNewSelection.Get();
		inComboBox._selectedString = value;

		TArray<FString> valueParses;
		value.ParseIntoArray(valueParses, TEXT("-"), true);
		if (valueParses.Num() > 0)
		{			
			int64 storyId = FCString::Strtoi64(*valueParses[0], NULL, 10);
			inComboBox._selectStringHandle->SetValue(storyId);
			inQuestId = valueParses[0];
		}
	}
}

void FGsTerrEdQuestSoundLayout::InitQuestID(FGsTerrEdSearchableComboBox& inComboBox, FString& inQuestId)
{
	if (_questIdList.Num() > 0)
	{						
		int64 selectQuestId;
		int64 questId;
		inComboBox._selectStringHandle->GetValue(selectQuestId);

		TArray<FString> parseArray;
		FString parseText;
		for (TSharedPtr<FString> stringPtr : _questIdList)
		{
			parseText = *stringPtr;
			parseText.ParseIntoArray(parseArray, TEXT("_"));
			if (parseArray.Num() > 0)
			{
				questId = FCString::Strtoi64(*parseArray[0], NULL, 10);
				if (questId == selectQuestId)
				{
					inComboBox._selectStringPtr = stringPtr;
					inQuestId = parseArray[0];
					break;
				}
			}		
		}

		inComboBox._selectedString = inQuestId;
	}
	else
	{
		inComboBox._selectStringPtr = MakeShareable(new FString(TEXT("No id in table")));
		inComboBox._selectStringHandle->SetValue(0);
	}
}

void FGsTerrEdQuestSoundLayout::CreateQuestIDSearchComboBox(FGsTerrEdSearchableComboBox& inComboBox
, FString& inQuestId
, IDetailChildrenBuilder& inChildBuilder
, TFunction<void(TSharedPtr<FString>, ESelectInfo::Type)> inChangeEvent
, TFunction<FText(void)> inGetEvent)
{
	InitQuestID(inComboBox, inQuestId);

	TSlateDelegates< TSharedPtr<FString> >::FOnSelectionChanged selectionChanged;
	selectionChanged.BindLambda([&](TSharedPtr<FString> newSelection, ESelectInfo::Type selectInfo) {
		ChangedStartQuestID(inComboBox, inQuestId, newSelection, selectInfo);
		});

	inChildBuilder.AddProperty(inComboBox._selectStringHandle.ToSharedRef())
		.CustomWidget()
		.NameContent()
		[
			inComboBox._selectStringHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			SAssignNew(inComboBox._searchableComboBox, SSearchableComboBox)
			.OptionsSource(&_questIdList)
		.OnGenerateWidget(this, &FGsTerrEdQuestSoundLayout::OnGenerateQuestIDWidget)
		.OnSelectionChanged(selectionChanged)
		.InitiallySelectedItem(inComboBox._selectStringPtr)
		.ContentPadding(2)
		.Content()
		[
			SNew(STextBlock)
			.Text_Lambda([&]()->FText {return FText::FromString(inQuestId); })
		]
		];
}

FText FGsTerrEdQuestSoundLayout::GetSelectedQuestIDText(FString& inText) const
{
	return FText::FromString(inText);
}
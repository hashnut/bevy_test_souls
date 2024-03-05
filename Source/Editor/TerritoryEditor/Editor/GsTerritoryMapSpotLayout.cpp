// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryMapSpotLayout.h"

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
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapSpotInfo.h"

//territory editor
#include "Util/GsTerritoryInputLayoutUtil.h"
#include "UTIL/GsTableUtil.h"
#include "Map/Spot/GsSchemaSpotData.h"
#include "LogMacros.h"

#define LOCTEXT_NAMESPACE "TerritoryWarpInputDetail"

TSharedRef<IPropertyTypeCustomization> FGsTerritoryMapSpotLayout::MakeInstance()
{
	return MakeShareable(new FGsTerritoryMapSpotLayout());
}

void FGsTerritoryMapSpotLayout::CustomizeHeader(TSharedRef<IPropertyHandle> inPropertyHandle, FDetailWidgetRow& inHeaderRow, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle
	inHeaderRow.NameContent()
		[
			inPropertyHandle->CreatePropertyNameWidget()
		];
}

void FGsTerritoryMapSpotLayout::CustomizeChildren(TSharedRef<IPropertyHandle> inPropertyHandle, IDetailChildrenBuilder& inChildBuilder, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle			
	uint32 num = 0;
	inPropertyHandle->GetNumChildren(num);

	for (uint32 i = 0; i < num; ++i)
	{
		TSharedPtr<IPropertyHandle> handle = inPropertyHandle->GetChildHandle(i);

		if (handle.IsValid())
		{
			FString name = handle.Get()->GetPropertyDisplayName().ToString();

			if (name.Equals(" Map Id"))
			{
				_mapIdHandle = handle;
				handle->GetValue(_mapId);

				CreateMapIdSearchableComboBox(inChildBuilder);
			}
			else if (name.Equals(" Spot Id"))
			{
				_spotIdHandle = handle;
				handle->GetValue(_spotId);

				CreateSpotIndexSearchableComboBox(inChildBuilder);
			}
			else
			{
				inChildBuilder.AddProperty(handle.ToSharedRef());
			}
		}
	}
}

void FGsTerritoryMapSpotLayout::InvalidMapIdList()
{	
	const UGsTable* mapTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaMapData::StaticStruct());
	if (nullptr == mapTable)
	{
		return;
	}

	TArray<const FGsSchemaMapData*> dataList;
	mapTable->GetAllRows<FGsSchemaMapData>(dataList);

	TArray<FGsTerritoryTableKeyInfo> keyInfos;

	//add sort id
	for (const FGsSchemaMapData* data : dataList)
	{
		if (data)
		{
			FGsTerritoryTableKeyInfo temp;

			temp.id = FCString::Atoi(*data->RowName.ToString());
			temp.desc = data->nameText;

			keyInfos.Add(temp);
		}	
	}

	keyInfos.Sort([=](const FGsTerritoryTableKeyInfo& source, const FGsTerritoryTableKeyInfo& dest)->bool {
		return source.id < dest.id;
	});

	//add map id 
	_mapIdTextList.Empty();
	_mapIdList.Empty();

	for (const FGsTerritoryTableKeyInfo& keyInfo : keyInfos)
	{
		FString temp = FString::FromInt(keyInfo.id) + TEXT("-") + keyInfo.desc;
		_mapIdTextList.Add(MakeShareable(new FString(temp)));
		_mapIdList.Add(keyInfo.id);
	}
}

void FGsTerritoryMapSpotLayout::InitMapIdSearchableComboBox()
{
	if (_mapIdList.Num() > 0)
	{
		int32 index = _mapIdList.IndexOfByKey(_mapId);

		if (index != INDEX_NONE)
		{			
			_selectedMapIdText = _mapIdTextList[index];
			_initSelectedMapIdText = _selectedMapIdText;		
			
			return;
		}	
	}
	
	_initSelectedMapIdText = MakeShareable(new FString(TEXT("No map id in table")));
	_mapIdHandle->SetValue(0);
}

void FGsTerritoryMapSpotLayout::CreateMapIdSearchableComboBox(IDetailChildrenBuilder& inChildBuilder)
{
	InvalidMapIdList();
	InitMapIdSearchableComboBox();

	inChildBuilder.AddProperty(_mapIdHandle.ToSharedRef())
		.CustomWidget()
		.NameContent()
		[
			_mapIdHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			SAssignNew(_destMapIDSearchableComboBox, SSearchableComboBox)
			.OptionsSource(&_mapIdTextList)
			.OnGenerateWidget(this, &FGsTerritoryMapSpotLayout::OnGenerateMapIdWidget)
			.OnSelectionChanged(this, &FGsTerritoryMapSpotLayout::OnChangeMapId)
			.InitiallySelectedItem(_initSelectedMapIdText)
			.ContentPadding(2)
			.Content()
			[
				SNew(STextBlock)
				.Text(this, &FGsTerritoryMapSpotLayout::GetSelectedMapIdText)
			]
		];
}

TSharedRef<SWidget> FGsTerritoryMapSpotLayout::OnGenerateMapIdWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGsTerritoryMapSpotLayout::OnChangeMapId(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
{
	if (inSelectInfo != ESelectInfo::Direct)
	{
		_selectedMapIdText = inNewSelection;	
		
		int32 index = _mapIdTextList.IndexOfByKey(inNewSelection);

		if (INDEX_NONE != index)
		{
			_mapId = _mapIdList[index];
			_mapIdHandle->SetValue(_mapId);

			int32 test;
			_mapIdHandle->GetValue(test);

			UE_LOG(LogTemp, Log, TEXT("Test %d"), test);
		}
	}

	InvalidSpotIndexList();
	InitSpotIndexSearchableComboBox();

	_spotIdSearchableComboBox->RefreshOptions();
}

FText FGsTerritoryMapSpotLayout::GetSelectedMapIdText() const
{
	if (_selectedMapIdText.IsValid())
	{
		FString value = *_selectedMapIdText.Get();
		return FText::FromString(value);
	}	

	return FText();
}

void FGsTerritoryMapSpotLayout::InvalidSpotIndexList()
{				
	FString levelKeyString = FString::FromInt(_mapId);
	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(levelKeyString);
	const UGsTable* mapTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), tableName);
	if (nullptr == mapTable)
	{
		return;
	}
	
	FString mapID = UGsTerritoryInputLayoutUtil::GetKey(levelKeyString);
	const FGsSchemaSpotData* find;
	if (false == mapTable->FindRow<FGsSchemaSpotData>(*mapID, find))
	{
		return;
	}

	TArray<FGsTerritoryTableKeyInfo> keyInfos;

	//add sort id
	for (const FGsSchemaSpotInfo& data : find->SpotList)
	{
		FGsTerritoryTableKeyInfo temp;

		temp.id = data.id;
		temp.desc = "row name : " + data.RowName.ToString(); 

		keyInfos.Add(temp);
	}

	keyInfos.Sort([=](const FGsTerritoryTableKeyInfo& source, const FGsTerritoryTableKeyInfo& dest)->bool {
		return source.id < dest.id;
	});

	//add map id 
	_spotIdList.Empty();
	_spotIdTextList.Empty();

	for (const FGsTerritoryTableKeyInfo& keyInfo : keyInfos)
	{
		FString temp = FString::FromInt(keyInfo.id) + TEXT("-") + keyInfo.desc;
		_spotIdTextList.Add(MakeShareable(new FString(temp)));
		_spotIdList.Add(keyInfo.id);
	}
}

void FGsTerritoryMapSpotLayout::InitSpotIndexSearchableComboBox()
{
	if (_spotIdList.Num() > 0)
	{
		int32 index = _spotIdList.IndexOfByKey(_spotId);

		if (index != INDEX_NONE)
		{
			_initSelectedSpotIdText = _spotIdTextList[index];
			_selectedSpotIdText = _initSelectedSpotIdText;			

			return;
		}
	}

	_selectedSpotIdText = MakeShareable(new FString(TEXT("No map id in table")));
	_spotIdHandle->SetValue(0);
}

void FGsTerritoryMapSpotLayout::CreateSpotIndexSearchableComboBox(IDetailChildrenBuilder& inChildBuilder)
{
	InvalidSpotIndexList();
	InitSpotIndexSearchableComboBox();

	inChildBuilder.AddProperty(_spotIdHandle.ToSharedRef())
		.CustomWidget()
		.NameContent()
		[
			_spotIdHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			SAssignNew(_spotIdSearchableComboBox, SSearchableComboBox)
			.OptionsSource(&_spotIdTextList)
		.OnGenerateWidget(this, &FGsTerritoryMapSpotLayout::OnGenerateSpotIndexWidget)
		.OnSelectionChanged(this, &FGsTerritoryMapSpotLayout::OnChangeSpotIndex)
		.InitiallySelectedItem(_initSelectedSpotIdText)
		.ContentPadding(2)
		.Content()
		[
			SNew(STextBlock)
			.Text(this, &FGsTerritoryMapSpotLayout::GetSelectedSpotIndexText)
		]
		];
}

TSharedRef<SWidget> FGsTerritoryMapSpotLayout::OnGenerateSpotIndexWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGsTerritoryMapSpotLayout::OnChangeSpotIndex(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
{
	if (inSelectInfo != ESelectInfo::Direct)
	{
		_selectedSpotIdText = inNewSelection;

		int32 index = _spotIdTextList.IndexOfByKey(inNewSelection);

		if (INDEX_NONE != index)
		{
			int32 id = _spotIdList[index];
			_spotIdHandle->SetValue(id);

			return;
		}
	}

	_spotIdHandle->SetValue(0);
}

FText FGsTerritoryMapSpotLayout::GetSelectedSpotIndexText() const
{
	if (_spotIdList.Num() == 0)
	{
		return FText::FromString("There is no spot");
	}

	if (_selectedSpotIdText.IsValid())
	{
		return FText::FromString(*_selectedSpotIdText);
	}	

	return FText();
}

#undef LOCTEXT_NAMESPACE
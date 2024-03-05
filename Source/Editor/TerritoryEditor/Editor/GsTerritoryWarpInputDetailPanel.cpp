// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryWarpInputDetailPanel.h"

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

#define LOCTEXT_NAMESPACE "TerritoryWarpInputDetail"

TSharedRef<IPropertyTypeCustomization> FGsTerritoryWarpInputDetailPanel::MakeInstance()
{
	return MakeShareable(new FGsTerritoryWarpInputDetailPanel());
}

void FGsTerritoryWarpInputDetailPanel::CustomizeHeader(TSharedRef<IPropertyHandle> inPropertyHandle, FDetailWidgetRow& inHeaderRow, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle
	inHeaderRow.NameContent()
		[
			inPropertyHandle->CreatePropertyNameWidget()
		];
}

void FGsTerritoryWarpInputDetailPanel::CustomizeChildren(TSharedRef<IPropertyHandle> inPropertyHandle, IDetailChildrenBuilder& inChildBuilder, IPropertyTypeCustomizationUtils& inCustomizationUtils)
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

			if (name.Equals("Dest Map ID"))
			{
				_destMapIDHandle = handle;

				CreateDestMapIDSearchableComboBox(inChildBuilder);
			}
			else if (name.Equals("Spot Index"))
			{
				_spotIndexHandle = handle;

				CreateSpotIndexSearchableComboBox(inChildBuilder);
			}
			else
			{
				inChildBuilder.AddProperty(handle.ToSharedRef());
			}
		}
	}
}

void FGsTerritoryWarpInputDetailPanel::InvalidDestMapIDList()
{
	UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
	const UGsTable* mapTable = mgr.GetTable(FGsSchemaMapData::StaticStruct());

	TArray<const FGsSchemaMapData*> dataList;
	mapTable->GetAllRows<FGsSchemaMapData>(dataList);

	TArray<FGsTerritoryTableKeyInfo> keyInfos;

	//add sort id
	for (const FGsSchemaMapData* data : dataList)
	{
		FGsTerritoryTableKeyInfo temp;

		temp.id = FCString::Atoi(*data->RowName.ToString());
		temp.desc = data->nameText;

		keyInfos.Add(temp);
	}

	keyInfos.Sort([=](const FGsTerritoryTableKeyInfo& source, const FGsTerritoryTableKeyInfo& dest)->bool {
		return source.id < dest.id;
	});

	//add map id 
	_destMapIDList.Empty();

	for (const FGsTerritoryTableKeyInfo& keyInfo : keyInfos)
	{
		FString temp = FString::FromInt(keyInfo.id) + TEXT("-") + keyInfo.desc;
		_destMapIDList.Add(MakeShareable(new FString(temp)));
	}
}

void FGsTerritoryWarpInputDetailPanel::InitDestMapIDSearchableComboBox()
{
	if (_destMapIDList.Num() > 0)
	{
		_initiallySelectedDestMapID = _destMapIDList[0];
		_selectedDestMapID = (*_initiallySelectedDestMapID.Get());
		_destMapIDHandle->SetValue(FCString::Atoi(*_selectedDestMapID));
	}
	else
	{
		_initiallySelectedDestMapID = MakeShareable(new FString(TEXT("No id in table")));
		_destMapIDHandle->SetValue(0);
	}
}

void FGsTerritoryWarpInputDetailPanel::CreateDestMapIDSearchableComboBox(IDetailChildrenBuilder& inChildBuilder)
{
	InvalidDestMapIDList();
	InitDestMapIDSearchableComboBox();

	inChildBuilder.AddProperty(_destMapIDHandle.ToSharedRef())
		.CustomWidget()
		.NameContent()
		[
			_destMapIDHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			SAssignNew(_destMapIDSearchableComboBox, SSearchableComboBox)
			.OptionsSource(&_destMapIDList)
			.OnGenerateWidget(this, &FGsTerritoryWarpInputDetailPanel::OnGenerateDestMapIDWidget)
			.OnSelectionChanged(this, &FGsTerritoryWarpInputDetailPanel::OnChangeDestMapID)
			.InitiallySelectedItem(_initiallySelectedDestMapID)
			.ContentPadding(2)
			.Content()
			[
				SNew(STextBlock)
				.Text(this, &FGsTerritoryWarpInputDetailPanel::GetSelectedDestMapIDText)
			]
		];
}

TSharedRef<SWidget> FGsTerritoryWarpInputDetailPanel::OnGenerateDestMapIDWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGsTerritoryWarpInputDetailPanel::OnChangeDestMapID(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
{
	if (inSelectInfo != ESelectInfo::Direct)
	{
		FString value = *inNewSelection.Get();
		_selectedDestMapID = value;

		TArray<FString> parses;
		value.ParseIntoArray(parses, TEXT("-"), true);

		if (parses.Num() > 0)
		{
			int32 id = FCString::Atoi(*parses[0]);
			_destMapIDHandle->SetValue(id);
		}
	}

	InvalidSpotIndexList();
	InitSpotIndexSearchableComboBox();

	_spotIndexSearchableComboBox->RefreshOptions();
}

FText FGsTerritoryWarpInputDetailPanel::GetSelectedDestMapIDText() const
{
	return FText::FromString(_selectedDestMapID);
}

void FGsTerritoryWarpInputDetailPanel::InvalidSpotIndexList()
{		
	FString mapName = _selectedDestMapID;
	TArray<FString> mapNameParse;

	mapName.ParseIntoArray(mapNameParse, TEXT("-"));

	if (mapNameParse.Num() != 2)
	{
		return;
	}

	FString levelKeyString = mapNameParse[0];	

	FString tableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableMapTerritoryData>(levelKeyString);
	const UGsTable* mapTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaMapTerritoryData::StaticStruct(), tableName);
	if (nullptr == mapTable)
	{
		return;
	}

	FText text = GetSelectedDestMapIDText();
	FString mapID = UGsTerritoryInputLayoutUtil::GetKey(text.ToString());	
	const FGsSchemaMapTerritoryData* find;

	if (false == mapTable->FindRow<FGsSchemaMapTerritoryData>(*mapID, find))
	{
		return;
	}

	TArray<FGsTerritoryTableKeyInfo> keyInfos;

	//add sort id
	for (const FGsSchemaMapSpotInfo& data : find->spawnList)
	{
		FGsTerritoryTableKeyInfo temp;

		temp.id = data.spotIndex;
		temp.desc = "row name : " + data.RowName.ToString(); 

		keyInfos.Add(temp);
	}

	keyInfos.Sort([=](const FGsTerritoryTableKeyInfo& source, const FGsTerritoryTableKeyInfo& dest)->bool {
		return source.id < dest.id;
	});

	//add map id 
	_spotIndexList.Empty();

	for (const FGsTerritoryTableKeyInfo& keyInfo : keyInfos)
	{
		FString temp = FString::FromInt(keyInfo.id) + TEXT("-") + keyInfo.desc;
		_spotIndexList.Add(MakeShareable(new FString(temp)));
	}
}

void FGsTerritoryWarpInputDetailPanel::InitSpotIndexSearchableComboBox()
{
	if (_spotIndexList.Num() > 0)
	{
		_initiallySelectedSpotIndex = _spotIndexList[0];
		_selectedSpotIndex = (*_initiallySelectedSpotIndex.Get());
		_spotIndexHandle->SetValue(FCString::Atoi(*_selectedSpotIndex));
	}
	else
	{
		_initiallySelectedSpotIndex = MakeShareable(new FString(TEXT("No id in table")));
		_spotIndexHandle->SetValue(0);
	}
}

void FGsTerritoryWarpInputDetailPanel::CreateSpotIndexSearchableComboBox(IDetailChildrenBuilder& inChildBuilder)
{
	InvalidSpotIndexList();
	InitSpotIndexSearchableComboBox();

	inChildBuilder.AddProperty(_spotIndexHandle.ToSharedRef())
		.CustomWidget()
		.NameContent()
		[
			_spotIndexHandle->CreatePropertyNameWidget()
		]
	.ValueContent()
		.MinDesiredWidth(500)
		[
			SAssignNew(_spotIndexSearchableComboBox, SSearchableComboBox)
			.OptionsSource(&_spotIndexList)
		.OnGenerateWidget(this, &FGsTerritoryWarpInputDetailPanel::OnGenerateSpotIndexWidget)
		.OnSelectionChanged(this, &FGsTerritoryWarpInputDetailPanel::OnChangeSpotIndex)
		.InitiallySelectedItem(_initiallySelectedSpotIndex)
		.ContentPadding(2)
		.Content()
		[
			SNew(STextBlock)
			.Text(this, &FGsTerritoryWarpInputDetailPanel::GetSelectedSpotIndexText)
		]
		];
}

TSharedRef<SWidget> FGsTerritoryWarpInputDetailPanel::OnGenerateSpotIndexWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGsTerritoryWarpInputDetailPanel::OnChangeSpotIndex(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
{
	if (inSelectInfo != ESelectInfo::Direct)
	{
		FString value = *inNewSelection.Get();
		_selectedSpotIndex = value;

		TArray<FString> parses;
		value.ParseIntoArray(parses, TEXT("-"), true);

		if (parses.Num() > 0)
		{
			int32 id = FCString::Atoi(*parses[0]);
			_spotIndexHandle->SetValue(id);
		}
	}
}

FText FGsTerritoryWarpInputDetailPanel::GetSelectedSpotIndexText() const
{
	if (_spotIndexList.Num() == 0)
	{
		return FText::FromString("There is no spot");
	}

	return FText::FromString(_selectedSpotIndex);
}

//TSharedRef<SWidget> FGsTerritoryWarpInputDetailPanel::CreateContextMenu()
//{
//	_spotIndexSearchableComboBox->MaxD
//
//	TSharedRef<SWidget> ComboBoxMenuContent =
//		SNew(SBox)
//		.MaxDesiredHeight(InArgs._MaxListHeight)
//
//		[
//			SNew(SVerticalBox)
//
//			+ SVerticalBox::Slot()
//		.AutoHeight()
//		[
//			SAssignNew(this->SearchField, SEditableTextBox)
//			.Font(IDetailLayoutBuilder::GetDetailFont())
//		.HintText(LOCTEXT("Search", "Search"))
//		.OnTextChanged(this, &SSearchableComboBox::OnSearchTextChanged)
//		]
//
//	+ SVerticalBox::Slot()
//		[
//			SAssignNew(this->ComboListView, SComboListType)
//			.ListItemsSource(OptionsSource)
//		.OnGenerateRow(this, &SSearchableComboBox::GenerateMenuItemRow)
//		.OnSelectionChanged(this, &SSearchableComboBox::OnSelectionChanged_Internal)
//		.SelectionMode(ESelectionMode::Single)
//		.ExternalScrollbar(InArgs._CustomScrollbar)
//		]
//		];
//}

#undef LOCTEXT_NAMESPACE
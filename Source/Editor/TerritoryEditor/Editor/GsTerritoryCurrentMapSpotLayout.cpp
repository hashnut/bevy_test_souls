// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryCurrentMapSpotLayout.h"

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
#include "Kismet/GameplayStatics.h"
#include "Actor/Spot/GsTerritorySpot.h"

#define LOCTEXT_NAMESPACE "TerritoryWarpInputDetail"

TSharedRef<IPropertyTypeCustomization> FGsTerritoryCurrentMapSpotLayout::MakeInstance()
{
	return MakeShareable(new FGsTerritoryCurrentMapSpotLayout());
}

void FGsTerritoryCurrentMapSpotLayout::CustomizeHeader(TSharedRef<IPropertyHandle> inPropertyHandle, FDetailWidgetRow& inHeaderRow, IPropertyTypeCustomizationUtils& inCustomizationUtils)
{
	//get property handle
	inHeaderRow.NameContent()
		[
			inPropertyHandle->CreatePropertyNameWidget()
		];
}

void FGsTerritoryCurrentMapSpotLayout::CustomizeChildren(TSharedRef<IPropertyHandle> inPropertyHandle, IDetailChildrenBuilder& inChildBuilder, IPropertyTypeCustomizationUtils& inCustomizationUtils)
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

			if (name.Equals(" Spot Id"))
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

void FGsTerritoryCurrentMapSpotLayout::InvalidSpotIndexList()
{				
	/*int32 levelId = FTerritoryEditor::GetInstance()->GetLevelManager()->GetCurrentLevelId();
	FString levelKeyString = FString::FromInt(levelId);
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

	*/
	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return;
	}

	TArray<AActor*> actorList;
	UGameplayStatics::GetAllActorsOfClass(world, AGsTerritorySpot::StaticClass(), actorList);
	TArray<FGsTerritoryTableKeyInfo> keyInfos;

	//add sort id
	for (const AActor* actor : actorList)
	{
		const AGsTerritorySpot* spot = Cast<AGsTerritorySpot>(actor);
		
		FGsTerritoryTableKeyInfo temp;

		temp.id = spot->_index;
		temp.desc = "desc : " + spot->_name.ToString(); 

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

void FGsTerritoryCurrentMapSpotLayout::InitSpotIndexSearchableComboBox()
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

void FGsTerritoryCurrentMapSpotLayout::CreateSpotIndexSearchableComboBox(IDetailChildrenBuilder& inChildBuilder)
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
		.OnGenerateWidget(this, &FGsTerritoryCurrentMapSpotLayout::OnGenerateSpotIndexWidget)
		.OnSelectionChanged(this, &FGsTerritoryCurrentMapSpotLayout::OnChangeSpotIndex)
		.InitiallySelectedItem(_initSelectedSpotIdText)
		.ContentPadding(2)
		.Content()
		[
			SNew(STextBlock)
			.Text(this, &FGsTerritoryCurrentMapSpotLayout::GetSelectedSpotIndexText)
		]
		];
}

TSharedRef<SWidget> FGsTerritoryCurrentMapSpotLayout::OnGenerateSpotIndexWidget(TSharedPtr<FString> inSelect)
{
	return SNew(STextBlock).Text(FText::FromString(*inSelect)).Font(IDetailLayoutBuilder::GetDetailFont());
}

void FGsTerritoryCurrentMapSpotLayout::OnChangeSpotIndex(TSharedPtr<FString> inNewSelection, ESelectInfo::Type inSelectInfo)
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

FText FGsTerritoryCurrentMapSpotLayout::GetSelectedSpotIndexText() const
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
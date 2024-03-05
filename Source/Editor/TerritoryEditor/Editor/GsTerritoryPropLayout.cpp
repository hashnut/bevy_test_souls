// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryPropLayout.h"

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

#define LOCTEXT_NAMESPACE "TerritoryNpcDataDetail"

TSharedRef<IPropertyTypeCustomization> FGsTerritoryPropLayout::MakeInstance()
{
	return MakeShareable(new FGsTerritoryPropLayout());
}

FReply FGsTerritoryPropLayout::OnClickNpcTable()
{
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();
	UGsTable * table = tableMgr.GetTable(FGsSchemaNpcData::StaticStruct());
	if (nullptr == table)
	{
		return FReply::Handled();
	}

	if (UGsDataTable* dataTable = table->GetCachedDataTable())
	{
		if (UAssetEditorSubsystem* assetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			if (assetEditorSubsystem->OpenEditorForAsset(dataTable))
			{
				FGsTerritoryLevelEvent::_bCreateNpc = false;

				FRSQLEditorDelegates::GetDataTableRowDoubleClicked(dataTable->GetName()).Remove(_getTIdRowNameHandle);
				_getTIdRowNameHandle = FRSQLEditorDelegates::GetDataTableRowDoubleClicked(dataTable->GetName()).AddRaw(this, &FGsTerritoryPropLayout::OnNpcDataTableDoubleClick);
			}
		}	
	}

	return FReply::Handled();
}

void FGsTerritoryPropLayout::OnNpcDataTableDoubleClick(FName inRowName)
{
	UGsTableManagerEditor& tableMgr = UGsTableManagerEditor::GetInstance();
	UGsTable * table = tableMgr.GetTable(FGsSchemaNpcData::StaticStruct());
	if (nullptr == table)
	{
		return;
	}

	if (UGsDataTable* dataTable = table->GetCachedDataTable())
	{
		int32 source = FCString::Atoi(*inRowName.ToString());

		_targetIDHandle->SetValue(source);

		int32 dest;

		_targetIDHandle->GetValue(dest);

		UE_LOG(LogTemp, Log, TEXT("FGsTerritoryChildActorDetailPanel - OnNpcDataTableDoubleClick - targetID : %d"), dest);

		FRSQLEditorDelegates::GetDataTableRowDoubleClicked(dataTable->GetName()).Remove(_getTIdRowNameHandle);

		FGsTerritoryLevelEvent::_bCreateNpc = true;

		if (UAssetEditorSubsystem* assetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
		{
			assetEditorSubsystem->CloseAllEditorsForAsset(dataTable);
		}		

		FPropertyEditorModule& propertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

		TSharedPtr<IDetailsView> detailPanel = propertyModule.FindDetailView(FName(TEXT("LevelEditorSelectionDetails")));

		if (detailPanel.IsValid())
		{
			detailPanel.Get()->ForceRefresh();
		}
	}
}

void FGsTerritoryPropLayout::InvalidTIdInfos()
{
	UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
	const UGsTable* npcTbl = mgr.GetTable(FGsSchemaPropData::StaticStruct());

	TArray<const FGsSchemaPropData*> npcDatas;
	npcTbl->GetAllRows<FGsSchemaPropData>(npcDatas);

	TArray<FGsTerritoryNpcIDInfo> npcIDs;

	//add sort id
	for (const FGsSchemaPropData* data : npcDatas)
	{
		FGsTerritoryNpcIDInfo temp;

		temp.id = data->id;
		temp.desc = data->nameString.ToString();

		npcIDs.Add(temp);		
	}

	npcIDs.Sort([=](const FGsTerritoryNpcIDInfo& source, const FGsTerritoryNpcIDInfo& dest)->bool {
		return source.id < dest.id;
	});

	//add npc id 
	_tIdList.Empty();

	for (const FGsTerritoryNpcIDInfo& data : npcIDs)
	{
		FString temp = FString::FromInt(data.id) + TEXT("-") + data.desc;
		_tIdList.Add(MakeShareable(new FString(temp)));		
	}	
}

#undef LOCTEXT_NAMESPACE
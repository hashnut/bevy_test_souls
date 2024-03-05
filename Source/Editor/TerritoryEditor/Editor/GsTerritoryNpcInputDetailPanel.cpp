// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryNpcInputDetailPanel.h"

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
#include "TerritoryEditor.h"

#define LOCTEXT_NAMESPACE "TerritoryNpcDataDetail"

TSharedRef<IPropertyTypeCustomization> FGsTerritoryNpcInputDetailPanel::MakeInstance()
{
	return MakeShareable(new FGsTerritoryNpcInputDetailPanel());
}

void FGsTerritoryNpcInputDetailPanel::InvalidTIdInfos()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryNpcInputDetailPanel::InvalidTIdInfos"));

	UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
	const UGsTable* npcTbl = mgr.GetTable(FGsSchemaNpcData::StaticStruct());

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
	_tIdList.Empty();

	for (const FGsTerritoryNpcIDInfo& data : npcIDs)
	{
		FString temp = FString::FromInt(data.id) + TEXT("-") + data.desc;
		_tIdList.Add(MakeShareable(new FString(temp)));
	}	
}

#undef LOCTEXT_NAMESPACE
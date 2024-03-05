// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerrEdShapeDataLayout.h"

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
#include "Blutility/Input/GsTerritorySpotInput.h"
#include "TerritoryEditor.h"

#define LOCTEXT_NAMESPACE "TerritoryNpcDataDetail"

TSharedRef<IPropertyTypeCustomization> FGsTerrEdShapeDataLayout::MakeInstance()
{
	return MakeShareable(new FGsTerrEdShapeDataLayout());
}

void FGsTerrEdShapeDataLayout::InvalidTIdInfos()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerrEdShapeDataLayout::InvalidTIdInfos"));

	UGsTableManagerEditor& mgr = UGsTableManagerEditor::GetInstance();
	const UGsTable* shapeTbl = mgr.GetTable(FGsSchemaShapeData::StaticStruct());

	TArray<const FGsSchemaShapeData*> shapeDatas;
	shapeTbl->GetAllRows<FGsSchemaShapeData>(shapeDatas);

	TArray<FGsTerritoryNpcIDInfo> shapeIDs;

	//add sort id
	for (const FGsSchemaShapeData* data : shapeDatas)
	{
		FGsTerritoryNpcIDInfo temp;

		temp.id = data->id;

		FString bleuprintName = data->bPClass.IsValid() ? data->bPClass.GetAssetName() : TEXT("no blueprint");
		FString skeletalName = data->skeletalMeshPath.IsValid() ? data->skeletalMeshPath.GetAssetName() : TEXT("no skeleton");
		FString staticMeshName = data->staticMeshPath.IsValid() ? data->staticMeshPath.GetAssetName() : TEXT("no staticmesh");

		temp.desc = TEXT("blueprint name : ") + bleuprintName + TEXT("\tskeletalName : ") + skeletalName + TEXT("\tstaticMeshName : ") + staticMeshName;

		shapeIDs.Add(temp);
	}

	shapeIDs.Sort([=](const FGsTerritoryNpcIDInfo& source, const FGsTerritoryNpcIDInfo& dest)->bool {
		return source.id < dest.id;
	});

	//add npc id 
	_tIdList.Empty();

	for (const FGsTerritoryNpcIDInfo& data : shapeIDs)
	{
		FString temp = FString::FromInt(data.id) + TEXT("-") + data.desc;
		_tIdList.Add(MakeShareable(new FString(temp)));		
	}	
}

#undef LOCTEXT_NAMESPACE
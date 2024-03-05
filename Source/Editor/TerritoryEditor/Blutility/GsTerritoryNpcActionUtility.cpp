// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryNpcActionUtility.h"

//UE4
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/UnrealEd/Public/UnrealEdGlobals.h"
#include "Editor/UnrealEd/Classes/Editor/EditorEngine.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"
#include "UTIL/GsText.h"
#include "Editor/Blutility/Public/EditorUtilityLibrary.h"

//Territory editor
#include "Actor/Spawn/GsTerritoryActor.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Actor/Spawn/SpawnActor/GsTerritorySpawnActor.h"
#include "Actor/Spawn/SpawnActor/GsTerritoryNpc.h"
#include "TerritoryEditor.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "TerritoryEditor.h"
#include "GsTerritoryUniqueIDManager.h"
#include "Editor/UnrealEd/Public/Toolkits/AssetEditorManager.h"
#include "Editor/UnrealEd/Public/Subsystems/AssetEditorSubsystem.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Private/DataTableEditor/GsDataTableEditor.h"
#include "Editor/EditorWidgets/Public/SAssetSearchBox.h"

const FString NPC_TABLE_PATH = "/Game/DataCenter/DataTable/GameObject/Npc/DTGsNpcData";

void UGsTerritoryNpcActionUtility::OpenNpcTable()
{
	AGsTerritorySpawnActor* actor = GetSelectedTerritoryActor();
	if (nullptr == actor)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *DELETE_FAIL);
		return;
	}
	
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(NPC_TABLE_PATH);
	
	const UGsDataTable* npcTable = FGsSchemaNpcData::GetStaticTable()->GetCachedDataTable();	
	if (nullptr == npcTable)
	{
		return;
	}
	
	TWeakPtr<IToolkit> toolKitPtr = FRSQLEditor::FindEditorForDataTable(npcTable);
	if (false == toolKitPtr.IsValid())
	{
		return;		
	}

	IToolkit* tookKit = toolKitPtr.Pin().Get();
	FGsDataTableEditor* dataTableEditor = static_cast<FGsDataTableEditor*>(tookKit);
	if (nullptr == dataTableEditor)
	{
		return;
	}

	TWeakPtr<SAssetSearchBox> searchBoxPtr = dataTableEditor->GetAssetSearchBox();	
	if (false == searchBoxPtr.IsValid())
	{
		return;
	}

	SAssetSearchBox* searchBox = searchBoxPtr.Pin().Get();
	int32 targetId = actor->_spawnInfo.TargetID;
	FText targetIdText = FText::FromString(FString::FromInt(targetId));
	searchBox->SetText(targetIdText);
	searchBox->Invalidate(EInvalidateWidgetReason::Layout);
}

void UGsTerritoryNpcActionUtility::OverrideAIData(OffensiveType inOffensive, float inSightRadius, bool inIsAggroLink)
{
	AGsTerritorySpawnActor* actor = GetSelectedTerritoryActor();
	if (nullptr == actor)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *DELETE_FAIL);
		return;
	}

	actor->_spawnInfo.offensiveType = inOffensive;
	actor->_spawnInfo.sightRadius = inSightRadius;
	actor->_spawnInfo.isAggroLink = inIsAggroLink;
}

void UGsTerritoryNpcActionUtility::SetOffensive(OffensiveType inOffensive)
{
	AGsTerritorySpawnActor* actor = GetSelectedTerritoryActor();
	if (nullptr == actor)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *DELETE_FAIL);
		return;
	}

	actor->_spawnInfo.offensiveType = inOffensive;
}

void UGsTerritoryNpcActionUtility::UGsTerritoryNpcActionUtility::SetSightRadius(float inSightRadius)
{
	AGsTerritorySpawnActor* actor = GetSelectedTerritoryActor();
	if (nullptr == actor)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *DELETE_FAIL);
		return;
	}
	
	actor->_spawnInfo.sightRadius = inSightRadius;	
}

void UGsTerritoryNpcActionUtility::SetIsAggoLink(bool inIsAggroLink)
{
	AGsTerritorySpawnActor* actor = GetSelectedTerritoryActor();
	if (nullptr == actor)
	{
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, *DELETE_FAIL);
		return;
	}

	actor->_spawnInfo.isAggroLink = inIsAggroLink;
}

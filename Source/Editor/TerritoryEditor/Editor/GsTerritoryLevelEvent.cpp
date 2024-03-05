
// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerritoryLevelEvent.h"

//UE4
#include "Engine/Selection.h"
#include "Misc/MessageDialog.h"
#include "Editor/EditorEngine.h"
#include "LevelEditorViewport.h"

//T1 Project
#include "RSQLEditor/Public/RSQLEditorDelegates.h"
#include "RSQLEditor/Public/GsTableManagerEditor.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"

//Territory editor
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "Level/Territory/GsTerritoryZoneDefine.h"
#include "TerritoryEditor.h"


//NpcTable
int64 FGsTerritoryLevelEvent::_npcID = INDEX_NONE;
bool FGsTerritoryLevelEvent::_bCreateNpc = true;
EGsTerritorySpawnType FGsTerritoryLevelEvent::_spawnType = EGsTerritorySpawnType::Npc;


FGsTerritoryLevelEvent::~FGsTerritoryLevelEvent()
{
	//UnbindNpcTable();
}

void FGsTerritoryLevelEvent::Initialize()
{
	
}

void FGsTerritoryLevelEvent::Clear()
{
	
}

void FGsTerritoryLevelEvent::BindNpcTable()
{		
	UGsTable * table = FGsTerritoryLevelManager::GetCachedNpcData();
	if (nullptr == table)
	{
		UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::BindNpcTable - fail"));
		return;
	}

	if (UGsDataTable* dataTable = table->GetCachedDataTable())
	{
		if (_npcTableDoubleClickHandle.IsValid())
		{
			FRSQLEditorDelegates::GetDataTableRowDoubleClicked(dataTable->GetName()).Remove(_npcTableDoubleClickHandle);	
		}
		_npcTableDoubleClickHandle = FRSQLEditorDelegates::GetDataTableRowDoubleClicked(dataTable->GetName()).AddRaw(this, &FGsTerritoryLevelEvent::OnDoubleClickNpcTable);

		UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::BindNpcTable - success"));
		return;
	}

	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::BindNpcTable - fail"));
}

void FGsTerritoryLevelEvent::UnbindNpcTable()
{	
	if (false == _npcTableDoubleClickHandle.IsValid())
	{	
#if TERRITORY_EDITOR_DEBUG
		UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::UnbindNpcTable - fail"));
#endif
		return;
	}

	UGsTable* table = FGsTerritoryLevelManager::GetCachedNpcData();
	if (nullptr == table)
	{
#if TERRITORY_EDITOR_DEBUG
		UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::UnbindNpcTable - fail"));
#endif
		return;
	}

	if (UGsDataTable* dataTable = table->GetCachedDataTable())
	{				
		FRSQLEditorDelegates::GetDataTableRowDoubleClicked(dataTable->GetName()).Remove(_npcTableDoubleClickHandle);

		UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::UnbindNpcTable - Success"));
		return;
	}

#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::UnbindNpcTable - fail"));
#endif
}

void FGsTerritoryLevelEvent::OnDoubleClickNpcTable(FName inKey)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::OnDoubleClickNpcTable"));

	if (false == _bCreateNpc)
	{
		return;
	}

	USelection* selection = GEditor->GetSelectedActors();
	if (nullptr == selection)
	{
		return;
	}

	TArray<AGsTerritoryZone*> selectObjects;
	int32 selectNum = selection->GetSelectedObjects<AGsTerritoryZone>(selectObjects);

	float zoneRadius;
	FString id;
	float dist;
	float angle;
	FVector relativeLocation;
	FVector location;

	for (AGsTerritoryZone* zone : selectObjects)
	{
		if (nullptr == zone)
		{
			continue;
		}

		zoneRadius = zone->_radius;
		id = inKey.ToString();

		FGsTerritorySpawnInfo spawnData;
		spawnData.Type = EGsTerritorySpawnType::Npc;
		spawnData.TargetID = FCString::Atoi(*id);

		dist = FMath::RandRange(0.0f, zoneRadius);
		angle = FMath::RandRange(0, 360);
		relativeLocation = (FVector::ForwardVector.RotateAngleAxis(angle, FVector::UpVector) * dist);
		location = relativeLocation + zone->GetActorLocation();

		zone->AddTerritorySpawnActor(spawnData, location);

		UE_LOG(LogTerritoryEditor, Log, TEXT("TerratoryEditor - OnDoubleClickNpcTable - zone : %s \tid : %s"), *zone->GetActorLabel(), *id);
	}
}

void FGsTerritoryLevelEvent::BindTerritoryActorTransform()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::BindTerritoryActorTransform"));

	_beignTerritoryActorTransfrom = GEditor->OnBeginObjectMovement().AddRaw(this, &FGsTerritoryLevelEvent::OnBeginTerritoryActorTransform);
	_endTerritoryActorTransfrom = GEditor->OnEndObjectMovement().AddRaw(this, &FGsTerritoryLevelEvent::OnEndTerritoryActorTransfrom);
}

void FGsTerritoryLevelEvent::UnbindTerritoryActorTransform()
{
#if TERRITORY_EDITOR_DEBUG
	UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::UnbindTerritoryActorTransform"));
#endif

	GEditor->OnBeginObjectMovement().Remove(_beignTerritoryActorTransfrom);
	GEditor->OnEndObjectMovement().Remove(_endTerritoryActorTransfrom);
}

void FGsTerritoryLevelEvent::OnBeginTerritoryActorTransform(UObject& inObj)
{			
	if (&inObj)
	{
		if (UObject* temp = &inObj)
		{
			if (AGsTerritorySpawnActor* child = Cast<AGsTerritorySpawnActor>(temp))
			{
				UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::OnBeginTerritoryActorTransform - label : %s"), *child->GetActorLabel());

				child->_selected = true;
				child->_preLocation = child->GetActorLocation();				
			}
		}
	}
}

void FGsTerritoryLevelEvent::OnEndTerritoryActorTransfrom(UObject& inObj)
{
	if (nullptr == &inObj)
	{
		return;
	}
	
	if (UObject* temp = &inObj)
	{
		if (AGsTerritorySpawnActor* child = Cast<AGsTerritorySpawnActor>(temp))
		{
			UE_LOG(LogTerritoryEditor, Log, TEXT("FGsTerritoryLevelEvent::OnEndTerritoryActorTransfrom - label : %s"), *child->GetActorLabel());

			FLevelEditorViewportClient* client = (FLevelEditorViewportClient*)(GEditor->GetActiveViewport()->GetClient());

			//For modify micro gap rotator at last time
			if (client)
			{
				//Check rotate state
				if (FWidget::EWidgetMode::WM_Rotate != client->GetWidgetMode())
				{
					return;
				}
				
				if (ECoordSystem::COORD_Local != client->GetWidgetCoordSystemSpace())
				{
					return;
				}

				//Get last select territory child actor
				if (USelection* selection = GEditor->GetSelectedActors())
				{
					TArray<AGsTerritorySpawnActor*> selectActors;

					selection->GetSelectedObjects<AGsTerritorySpawnActor>(selectActors);

					if (selectActors.Num() > 0)
					{
						AGsTerritorySpawnActor* last = selectActors.Last();

						if (last && child != last)
						{
							//Set rotation from last select territory child actor
							FRotator rot = selectActors.Last()->GetActorRotation();

							child->SetActorRotation(rot);
						}
					}
				}				
			}

			child->_selected = false;
		}
	}
}

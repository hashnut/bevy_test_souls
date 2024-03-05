// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritorySpawnActor.h"

//UE4
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/UnrealEd/Public/UnrealEdGlobals.h"
#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/EditorEngine.h"
#include "LevelEditorViewport.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

//T1 Project
#include "Editor/Widget/GsEditorWidget.h"

//Territory editor
#include "Actor/Spawn/Zone/GsTerritoryZone.h"
#include "ActorComponentEx/WidgetComponentEx.h"
#include "GsTerritoryEditorSetting.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Editor/TerritoryEditor/Component/GsTerrEdWidgetComponent.h"


AGsTerritorySpawnActor::AGsTerritorySpawnActor()
{
	
}

void AGsTerritorySpawnActor::OnConstruction(const FTransform& in_trs)
{
	Super::OnConstruction(in_trs);

	//Check zone for undo
	ReregisterZone();

	if (_zone)
	{
		_spawnInfo.RelativeLocation = in_trs.GetLocation() - _zone->GetActorLocation();
		_spawnInfo.Rotation = in_trs.GetRotation().Euler();
	}

	////New create child actor's shape id not setted yet, only saved actor refresh shape
	//if (HasAnyFlags(RF_WasLoaded))
	//{
	//	InvalidShape();
	//}
	
	RotateAtOwnPivot();
}

void AGsTerritorySpawnActor::DestroyManually()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritorySpawnActor::DestroyManually - label : %s"), *GetActorLabel());

	if (_zone)
	{
		_zone->RemoveTerritoryActor(this);
	}

	Super::DestroyManually();
}

void AGsTerritorySpawnActor::Destroyed()
{
	Super::Destroyed();
}

FVector AGsTerritorySpawnActor::GetRelativeLocation()
{
	return _spawnInfo.RelativeLocation;		
}

void AGsTerritorySpawnActor::ReregisterZone()
{
	if (_zone)
	{
		if (false == _zone->IsTerritoryActorExist(this))
		{
			_zone->AddTerritoryActor(this);
		}
	}
}

bool AGsTerritorySpawnActor::RecoveryTerritoryUniqueID()
{
	if (_uniqueID == INDEX_NONE)
	{
		UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritorySpawnActor::RecoveryTerritoryUniqueID - label : %s"), *GetActorLabel());

		_uniqueID = _spawnInfo.UniqueID;
		Super::RecoveryTerritoryUniqueID();

		return true;
	}

	return false;
}

void AGsTerritorySpawnActor::CreateWidget(const FTransform& inTransform)
{
#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		_widgetComp->SetVisibility(true);
		InvalidWidgetPosition(inTransform.GetLocation());
		InvalidWidgetText();

		if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
		{
			_widgetComp->_viewDistance = settings->_territoryActorViewDistance;
		}
	}
#endif
}

void AGsTerritorySpawnActor::InvalidWidgetPosition(const FVector& in_location)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritorySpawnActor::InvalidWidgetPosition - label : %s"), *GetActorLabel());
#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		const FVector& pos = in_location + FVector(0, 0, _widgetHeight);
		_widgetComp->SetWorldLocation(pos);
	}
#endif
}

FString AGsTerritorySpawnActor::GetSpawnDataText()
{
	int64 zoneUniqueId = 0;

	if (_zone)
	{
		zoneUniqueId = _zone->GetTerritoryUniqueID();
	}

	const FString& uniqueID = FString::Printf(TEXT("%lld"), _uniqueID);

	return FString::Printf(TEXT("Zone unique id : %lld\n"), zoneUniqueId)
		+ FString::Printf(TEXT("Unique id : %s\n"), *uniqueID)
		+ FString::Printf(TEXT("Target id : %d\n"), _spawnInfo.TargetID)
		+ FString::Printf(TEXT("Text Name : %s\n"), *_targetName)
		+ FString::Printf(TEXT("Location : %s\n"), *GetActorLocation().ToString())
		+ FString::Printf(TEXT("Rotation : %s\n"), *GetActorRotation().ToString())
		+ FString::Printf(TEXT("Init Spawn Delay : %f\n"), _spawnInfo.InitSpawnDelay)
		+ FString::Printf(TEXT("Respawn Interval : %f\n"), _spawnInfo.RespawnInterval)
		+ FString::Printf(TEXT("SpawnRate : %d\n"), _spawnInfo.SpawnRate)
		+ FString::Printf(TEXT("Group LImit Range : %f\n"), _spawnInfo.groupLimitRange);
}

FVector AGsTerritorySpawnActor::GetWidgetLocation()
{
	return GetActorLocation() + FVector(0, 0, _widgetHeight);
}

void AGsTerritorySpawnActor::RotateAtOwnPivot()
{	
	if (false == _selected)
	{
		return;
	}

	FLevelEditorViewportClient* client = (FLevelEditorViewportClient*)(GEditor->GetActiveViewport()->GetClient());
	if (nullptr == client)
	{
		return;
	}

	if (FWidget::EWidgetMode::WM_Rotate != client->GetWidgetMode())
	{
		return;
	}

	if (ECoordSystem::COORD_Local != client->GetWidgetCoordSystemSpace())
	{
		return;
	}

	//Check rotate state
	if (USelection* selection = GEditor->GetSelectedActors())
	{
		TArray<AGsTerritorySpawnActor*> selectActors;

		//Get last select territory child actor
		selection->GetSelectedObjects<AGsTerritorySpawnActor>(selectActors);
		if (selectActors.Num() > 0)
		{
			AGsTerritorySpawnActor* last = selectActors.Last();
			if (last)
			{
				const FRotator& rot = selectActors.Last()->GetActorRotation();

				//Set rotation from last select territory child actor
				if (last != this)
				{
					SetActorRotation(rot);
				}

				SetActorLocation(_preLocation);
			}
		}
	}	
}

void AGsTerritorySpawnActor::SelectInternal(UObject* inSelect)
{
	Super::SelectInternal(inSelect);

	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritorySpawnActor::SelectInternal - label : %s"), *GetActorLabel());

	if (inSelect == this)
	{
		if (AGsTerritoryZone* terrainActor = Cast<AGsTerritoryZone>(inSelect))
		{
			GEditor->SelectActor(terrainActor, true, true, true, true);
			GEditor->GetSelectedActors()->EndBatchSelectOperation();
			GEditor->NoteSelectionChange();
		}
		
		if (FGsTerritoryLevelManager::_shapeShowType == EGsTerrEdShowType::ShowOnlySelect)
		{
			FGsTerritoryLevelManager::ActiveTerritoryShape(FGsTerritoryLevelManager::_shapeShowType);
		}
	}
}

void AGsTerritorySpawnActor::ReregisterComponent()
{
	InvalidWidgetPosition(GetActorLocation());
	InvalidWidgetText();
}

void AGsTerritorySpawnActor::SetTerritoryZone(AGsTerritoryZone* in_zone)
{
	_zone = in_zone;
}

void AGsTerritorySpawnActor::SetSpawnInfo(const FGsTerritorySpawnInfo& in_spawn)
{
	_spawnInfo = in_spawn;
}

void AGsTerritorySpawnActor::SetTerritoryRelativeLocation(const FVector& in_location)
{
	_spawnInfo.RelativeLocation = in_location;
}

void AGsTerritorySpawnActor::SetSpawnLevel(int32 inLevel)
{
	_spawnInfo.SpawnLevel = inLevel;

	InvalidWidgetText();
}

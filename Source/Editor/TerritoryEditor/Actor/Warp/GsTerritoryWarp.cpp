// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryWarp.h"

//ue4
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"

//t1 project
#include "UTIL/GsText.h"
#include "ActorcomponentEx/WidgetComponentEx.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"

//territory editor
#include "Util/GsTerritoryZoneShapeUtil.h"
#include "Editor/TerritoryEditor/Util/GsTerritoryLevelUtil.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Actor/Spot/GsTerritorySpot.h"
#include "Actor/Spot/GsTerritoryWarpSpot.h"
#include "Actor/Spawn/GsTerritoryActor.h"
#include "UTIL/GsTableUtil.h"
#include "Map/Spot/GsSchemaSpotData.h"
#include "Editor/TerritoryEditor/Component/GsTerrEdWidgetComponent.h"
#include "GsTerritoryEditorSetting.h"

AGsTerritoryWarp::AGsTerritoryWarp()
{
	_shapeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Area"));
	if (_shapeComponent)
	{
		SetRootComponent(_shapeComponent);			
	}

	SetHidden(true);
	SetCanBeDamaged(false);

	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_warpColor;
	}
}

void AGsTerritoryWarp::OnConstruction(const FTransform& inTransform)
{
	Super::OnConstruction(inTransform);

	InvalidLocation();
}

void AGsTerritoryWarp::DestroyManually()
{
	if (_warpSpot)
	{
		if (_warpSpot->IsValid())
		{
			_warpSpot->UnregisterTerritoryUniqueID(_warpSpot, EGsTerritoryObjType::Spot, _warpSpot->_instanceID);
		}

		_warpSpot->_isCanDelete = true;
		_warpSpot->Destroy();
	}

	Super::DestroyManually();
}

void AGsTerritoryWarp::RegisterWarp()
{
	if (_warpSpot)
	{
		if (_warpSpot->_warp != this)
		{
			_warpSpot->SetWarp(this);
		}
	}
}

void AGsTerritoryWarp::InvalidWidgetText()
{
	if (_isValid)
	{
		_description = FString(TEXT("Warp\n"))
			+ FString(TEXT("source map id : ")) + FString::FromInt(_sourceMapID) + FString(TEXT("\n"))
			+ FString(TEXT("source spot index : ")) + FString::Printf(TEXT("%lld"), _sourceSpotIndex) + FString(TEXT("\n"))
			+ FString(TEXT("dest map id : ")) + FString::FromInt(_destMapID) + FString(TEXT("\n"))
			+ FString(TEXT("dest spot index : ")) + FString::Printf(TEXT("%lld"), _destSpotIndex) + FString(TEXT("\n"))
			+ FString(TEXT("Warp type : ")) + FGsTextUtil::GetStringFromEnum("WarpType", _warpType)
			+ FString(TEXT("Unique id : ")) + FString::Printf(TEXT("%lld"), _uniqueID)
			+ FString(TEXT("Group id : ")) + FString::Printf(TEXT("%d"), _groupId);
	}

	Super::InvalidWidgetText();
}

void AGsTerritoryWarp::InvalidWidgetPosition(const FVector& in_location)
{
	const FVector& location = FVector(in_location.X, in_location.Y, in_location.Z + _widgetHeight);

#if TERR_ED_WIDGET_COMP
	if (_widgetComp)
	{
		_widgetComp->SetWorldLocation(location);
	}
#endif
}

void AGsTerritoryWarp::Export(FGsSchemaMapWarpInfo& outWarp)
{
	const FString& sourceSpotDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(FString::FromInt(_sourceMapID));
	const FString& destSpotDataTableName = UGsTableUtil::GetCustomTerritoryDataTableName<UGsTableSpotData>(FString::FromInt(_destMapID));

	UGsTable* sourceSpotTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), sourceSpotDataTableName);
	UGsTable* destSpotTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaSpotData::StaticStruct(), destSpotDataTableName);
	UGsTable* shapeDataTable = UGsTableManagerEditor::GetInstance().GetTable(FGsSchemaShapeData::StaticStruct());
	if (nullptr == sourceSpotTable 
	|| nullptr == destSpotTable
	|| nullptr == shapeDataTable)
	{
		return;
	}

	FGsSchemaSpotDataRow sourceSpotDataRow;
	FGsSchemaSpotDataRow destSpotDataRow;
	FGsSchemaShapeDataRow shapeDataRow;

	//source spot
	sourceSpotDataRow.DataTable = sourceSpotTable->GetCachedDataTable();
	sourceSpotDataRow.RowName = *FString::FromInt(_sourceMapID);

	//dest spot
	destSpotDataRow.DataTable = destSpotTable->GetCachedDataTable();
	destSpotDataRow.RowName = *FString::FromInt(_destMapID);

	//shape
	UGsTableShapeData* castShapeData = Cast<UGsTableShapeData>(shapeDataTable);
	if (castShapeData)
	{
		const FGsSchemaShapeData* findShapeData;
		if (castShapeData->FindRowById(_shapeID, findShapeData))
		{
			shapeDataRow.DataTable = shapeDataTable->GetCachedDataTable();
			shapeDataRow.RowName = findShapeData->GetRowName();
		}		
	}	

	outWarp.warpId = _uniqueID;

	outWarp.sourceSpot.MapId = sourceSpotDataRow;
	outWarp.sourceSpot.SpotIndex = _sourceSpotIndex;	

	outWarp.destSpot.MapId = destSpotDataRow;
	outWarp.destSpot.SpotIndex = _destSpotIndex;
		
	outWarp.WarpType = _warpType;
	outWarp.shapeId = shapeDataRow;	
	outWarp.groupId = _groupId;

	outWarp.ClientGameId = _uniqueID | CLIENT_PACKET;
}

void AGsTerritoryWarp::SetWarpSpot(AGsTerritoryWarpSpot* inSpot)
{
	if (nullptr == inSpot)
	{
		return;
	}

	ULevel* currentLevel = inSpot->GetLevel();
	if (nullptr == currentLevel)
	{
		return;
	}

	const FString& path = currentLevel->GetPathName();
	if (false == UGsTerritoryLevelUtil::IsServerLevel(path))
	{
		return;
	}

	const FString& levelName = UGsTerritoryLevelUtil::GetLevelName(path);
	FString levelKey;
	FString persistantLevel;
	if (false == UGsTerritoryLevelUtil::IsValidServerLevelName(levelName, persistantLevel, levelKey))
	{
		return;
	}	

	_sourceMapID = FCString::Atoi(*levelKey);
	_sourceSpotIndex = inSpot->_instanceID;
	_warpSpot = inSpot;	

	InvalidLocation();
	InvalidWidgetText();
	InvalidWidgetPosition(GetActorLocation());
}

void AGsTerritoryWarp::InvalidLocation()
{
	if (_warpSpot)
	{
		const FVector& location = _warpSpot->GetActorLocation() + FVector(0, 0, _warpSpot->_warpHeight);
		SetActorLocation(location);
	}
}

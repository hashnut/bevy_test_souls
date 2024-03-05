// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryWarpSpot.h"

//ue4
#include "UObject/ConstructorHelpers.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/Texture2D.h"
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/EditorEngine.h"

//t1 project
#include "UTIL/GsText.h"
#include "UTIL/GsDir.h"

//territory editor
#include "Util/GsTerritoryZoneShapeUtil.h"
#include "Actor/Warp/GsTerritoryWarp.h"
#include "GsTerritoryEditorSetting.h"
#include "Data/GsResourceManager.h"
#include "TerritoryEditor/Util/GsTerritoryLevelUtil.h"

AGsTerritoryWarpSpot::AGsTerritoryWarpSpot()
{
	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_warpSpotColor;
	}

	_shapeChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("WarpChildActor"));
	if (_shapeChildActor)
	{
		_shapeChildActor->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_shapeChildActor->SetRelativeLocation(FVector::ZeroVector);
	}
}

void AGsTerritoryWarpSpot::OnConstruction(const FTransform& inTransform)
{
	Super::OnConstruction(inTransform);

	KeepTerritorWarpLocation(inTransform);
}

void AGsTerritoryWarpSpot::InvalidWidgetText()
{
	if (_isValid)
	{
		_description = FString(TEXT("WarpSpot\n")) + GetSpotDescription();
	}

	SetTerritoryTitleText(TEXT("Warp spot"));

	Super::InvalidWidgetText();
}

FString AGsTerritoryWarpSpot::GetSpotDescription()
{
	return FString(TEXT("Index : ")) + FString::FromInt(_index) + FString(TEXT("\n"))		
		+ FString(TEXT("Random range : ")) + FString::SanitizeFloat(_radius) + FString(TEXT("\n"))
		+ FString(TEXT("Unique id : ")) + FString::Printf(TEXT("%lld"), _uniqueID);		
}

void AGsTerritoryWarpSpot::SetWarp(AGsTerritoryWarp* inWarp)
{
	_warp = inWarp;

	inWarp->SetWarpSpot(this);
	InvalidShape();	
}

void AGsTerritoryWarpSpot::KeepTerritorWarpLocation(const FTransform& inTransform)
{
	if (_warp)
	{
		const FVector& location = inTransform.GetLocation() + FVector(0, 0, _warpHeight);
		_warp->SetActorLocation(location);
		_warp->InvalidWidgetPosition(location);
		_warp->InvalidWidgetText();
	}
}

void AGsTerritoryWarpSpot::DestroyManually()
{
	if (_warp)
	{
		if (_warp->IsValid())
		{
			_warp->UnregisterTerritoryUniqueID(_warp, EGsTerritoryObjType::Warp, _warp->_instanceID);
		}		

		_warp->_isCanDelete = true;
		_warp->Destroy();
	}

	Super::DestroyManually();
}

void AGsTerritoryWarpSpot::Copy()
{
	if (nullptr == _warp)
	{
		return;
	}

	FTransform trs;
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (settings->_warpClass)
	{
		FActorSpawnParameters param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (AActor* temp = GetWorld()->SpawnActor(settings->_warpClass, &trs, param))
		{
			if (AGsTerritoryWarp* warp = Cast<AGsTerritoryWarp>(temp))
			{
				warp->_shapeID = _warp->_shapeID;
				warp->_sourceMapID = _warp->_sourceMapID;
				warp->_sourceSpotIndex = _index;
				warp->_destMapID = _warp->_destMapID;
				warp->_destSpotIndex = _warp->_destSpotIndex;
				warp->_warpType = _warp->_warpType;
				warp->_warpSpot = this;				

				_warp = warp;
				FTerritoryEditor::GetInstance()->GetUniqueIDManager()->SetUniqueID(warp);

				GEditor->SelectActor(temp, false, false);
			}			
		}		
	}
}

bool AGsTerritoryWarpSpot::InvalidSpotIndex()
{
	if (_warp)
	{
		if (_warp->_sourceSpotIndex != _index)
		{
			_warp->SetWarpSpot(this);
			return true;
		}		
	}

	return false;
}

void AGsTerritoryWarpSpot::InvalidShape()
{
	Super::InvalidShape();

	if (nullptr == _shapeChildActor
		|| nullptr == _warp)
	{
		return;
	}

	const FGsSchemaShapeData* shapeData = UGsTerritoryLevelUtil::FindShapeData(_warp->_shapeID);
	if (nullptr == shapeData)
	{
#if WITH_EDITOR		
		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red,
			TEXT("Shape 테이블에 워프 외형 ID 가 존재하지 않습니다."));
#endif
		return;
	}

	UClass* shapeClass = Cast<UClass>(UAssetManager::GetStreamableManager().LoadSynchronous<UClass>(shapeData->bPClass.ToString() + TEXT("_C")));
	if (nullptr == shapeClass)
	{
#if WITH_EDITOR		
		GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Red,
			TEXT("Shape 테이블에 워프 외형 ID 가 존재하지 않습니다."));
#endif
		return;
	}

	_shapeChildActor->SetChildActorClass(shapeClass);
	if (_shapeChildActor->GetChildActor())
	{
		_shapeChildActor->GetChildActor()->SetActorHiddenInGame(true);
	}	
}

void AGsTerritoryWarpSpot::SetWarpIndexForce(int64 inSpotInstanceId, int64 inWarpUniqueId)
{
	SetIndexForce(inSpotInstanceId);

	if (_warp)
	{
		_warp->SetTerritoryUniqueIDForce(inWarpUniqueId);
	}	
}

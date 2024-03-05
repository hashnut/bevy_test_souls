// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZoneShape.h"

//UE4
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "Runtime/UMG/Public/Components/WidgetComponent.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

//T1 Project
#include "T1Project/Editor/Widget/GsEditorWidget.h"

//Territor editor
#include "Component/GsTerritoryZonePolygonComponent.h"
#include "Component/GsTerritoryZoneVerticalStripeComponent.h"
#include "Component/GsTerritoryZonePillarBunchComponent.h"

#include "Editor/TerritoryEditor/TerritoryEditor.h"
#include "Editor/TerritoryEditor/GsTerritoryEditorSetting.h"
#include "Actor/Spawn/Zone/GsTerritoryZone.h"


AGsTerritoryZoneShape::AGsTerritoryZoneShape()
{
	_splineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (_splineComponent)
	{
		_splineComponent->SetRelativeLocation(FVector::ZeroVector);
		_splineComponent->EditorUnselectedSplineSegmentColor = FColor::Blue;
		_splineComponent->EditorSelectedSplineSegmentColor = FColor::Yellow;

		SetRootComponent(_splineComponent);
	}

	_floorComponent = CreateDefaultSubobject<UGsTerritoryZonePolygonComponent>(TEXT("Floor"));
	if (_floorComponent)
	{
		_floorComponent->SetRelativeLocation(FVector::ZeroVector);
		_floorComponent->_shape = this;
		_floorComponent->_isCanCollision = true;
	}

	_fenceComponent = CreateDefaultSubobject<UGsTerritoryZoneVerticalStripeComponent>(TEXT("Fence"));
	if (_fenceComponent)
	{
		_fenceComponent->SetRelativeLocation(FVector::ZeroVector);
		_fenceComponent->_shape = this;
	}

	_pillarBunchComponent = CreateDefaultSubobject<UGsTerritoryZonePillarBunchComponent>(TEXT("PillarBunch"));
	if (_pillarBunchComponent)
	{
		_pillarBunchComponent->SetRelativeLocation(FVector::ZeroVector);
		_pillarBunchComponent->_shape = this;
	}
}

/// <summary>
/// Child Actor의 OnConstruction은 생성시에 호출되고 부모 액터의 이동시, Spline 변경시 호출되지 않는다. 
/// 따라서 OnConstruction에서 Draw를 할 필요가 없다
/// </summary>
/// <param name="in_transform"></param>
void AGsTerritoryZoneShape::OnConstruction(const FTransform& in_transform)
{
	Super::OnConstruction(in_transform);
}

void AGsTerritoryZoneShape::BeginDestroy()
{
	Super::BeginDestroy();
}

void AGsTerritoryZoneShape::DrawZoneShape(const FTransform& in_transform)
{
	DrawPlanes();
	DrawPlillar();
	DrawFloor(in_transform);
}

void AGsTerritoryZoneShape::DrawZoneShape(const FTransform& in_transform, const TArray<FVector>& in_pointArray, float in_radius)
{
	ActiveSplineDebug(false);
	InitPointArray(in_pointArray);
	DrawZoneShape(in_transform);
}

FVector AGsTerritoryZoneShape::GetCenterBetweenPoints(int32 in_start, int32 in_end)
{
	if (_pointArray.IsValidIndex(in_start)
		&& _pointArray.IsValidIndex(in_end))
	{
		return ((_pointArray[in_start] + _pointArray[in_end]) / 2.0f);
	}

	return FVector::ZeroVector;
}

void AGsTerritoryZoneShape::InitPointArray()
{
	_pointArray.Empty();
	int num = _splineComponent->GetNumberOfSplinePoints();
	for (int i = 0; i < num; ++i)
	{
		_pointArray.Add(_splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
	}
}

void AGsTerritoryZoneShape::InitPointArray(const TArray<FVector>& in_pointsArray)
{
	_pointArray.Empty();
	_pointArray = in_pointsArray;
}

void AGsTerritoryZoneShape::DrawPlanes(bool in_close)
{
	if (_fenceComponent)
	{
		_fenceComponent->Draw(_pointArray, true, true, _height, _planeOuterColor, _planeInsideColor);
	}
}

void AGsTerritoryZoneShape::DrawPlillar(bool in_close)
{
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (settings && false == settings->_drawZonePillar)
	{
		return;
	}

	if (_pillarBunchComponent)
	{
		_pillarBunchComponent->Draw(_pointArray, in_close, _height, _pillarColor);
	}
}

void AGsTerritoryZoneShape::DrawFloor(const FTransform& in_transform)
{
	if (_floorComponent)
	{
		FTransform trs = in_transform;
		trs.SetLocation(FVector::ZeroVector);
		_floorComponent->SetRelativeLocation(FVector::ZeroVector);
		_floorComponent->Draw(trs, _floorColor, _pointArray);
	}
}

FVector AGsTerritoryZoneShape::GetRandomPointInTerritoryZone() const
{
	//나중에 다각형 안에서 랜덤한 값을 얻을 수 있도록 변경해줘야함	
	FRandomStream rand;
	rand.GenerateNewSeed();
	int32 num = _pointArray.Num();
	int32 index = rand.RandRange(0, num - 1);

	return _pointArray[index];	
}

void AGsTerritoryZoneShape::SetColors(const FColor& in_pillar, const FColor& in_fenceOuter, const FColor& in_fenceInner, const FColor& in_floor)
{
	_pillarColor = in_pillar;
	_planeOuterColor = in_fenceOuter;
	_planeInsideColor = in_fenceInner;
	_floorColor = in_floor;
}

void AGsTerritoryZoneShape::SetShapeOwner(AGsTerritoryActor* inOwner)
{
	IGsTerritoryShapeOwnerInterface* owner = Cast<IGsTerritoryShapeOwnerInterface>(inOwner);
	if (nullptr == owner)
	{
		return;
	}

	_owner = Cast<AGsTerritoryActor>(inOwner);		
}

void AGsTerritoryZoneShape::ActiveSplineDebug(bool in_active)
{
	if (_splineComponent)
	{
		if (in_active)
		{
			_splineComponent->Deactivate();
			_splineComponent->SetDrawDebug(in_active);
		}
		else
		{
			_splineComponent->Activate();
			_splineComponent->SetDrawDebug(in_active);
		}
	}
}

void AGsTerritoryZoneShape::TransparentFloor()
{
	_floorComponent->TransparentFloor();
}

class UMaterial* AGsTerritoryZoneShape::GetMaterial()
{
	return _material;
}
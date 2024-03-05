// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryShape.h"
#include "Components/SplineComponent.h"
#include "Actor/Spawn/Zone/Shape/Component/GsTerritoryZonePolygonComponent.h"
#include "Actor/Spawn/Zone/Shape/Component/GsTerritoryZoneVerticalStripeComponent.h"
#include "Actor/Spawn/Zone/Shape/Component/GsTerritoryZonePillarBunchComponent.h"
#include "Actor/GsTerritoryShapeOwnerInterface.h"
#include "GsTerritoryEditorSetting.h"
#include "GsTerritoryLevelManager.h"

AGsTerritoryShape::AGsTerritoryShape()
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
void AGsTerritoryShape::OnConstruction(const FTransform& in_transform)
{
	Super::OnConstruction(in_transform);
}

void AGsTerritoryShape::BeginDestroy()
{
	Super::BeginDestroy();
}

void AGsTerritoryShape::DrawShape(const FTransform& in_transform)
{
	DrawPlanes(_splineComponent->IsClosedLoop());
	DrawPlillar();

	if (_isDrawFloor)
	{
		DrawFloor(in_transform);
	}
	else
	{
		ClearFloor();
	}
}

void AGsTerritoryShape::DrawShape(const FTransform& in_transform, const TArray<FVector>& in_pointArray, float in_radius)
{
	ActiveSplineDebug(false);
	InitPointArray(in_pointArray);
	DrawShape(in_transform);
}

FVector AGsTerritoryShape::GetCenterBetweenPoints(int32 in_start, int32 in_end)
{
	if (_pointArray.IsValidIndex(in_start)
		&& _pointArray.IsValidIndex(in_end))
	{
		return ((_pointArray[in_start] + _pointArray[in_end]) / 2.0f);
	}

	return FVector::ZeroVector;
}

void AGsTerritoryShape::InitPointArray()
{
	_pointArray.Empty();
	int num = _splineComponent->GetNumberOfSplinePoints();
	for (int i = 0; i < num; ++i)
	{
		_pointArray.Add(_splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
	}
}

void AGsTerritoryShape::InitPointArray(const TArray<FVector>& in_pointsArray)
{
	_pointArray.Empty();
	_pointArray = in_pointsArray;
}

void AGsTerritoryShape::DrawPlanes(bool in_close)
{
	if (_fenceComponent)
	{
		_fenceComponent->Draw(_pointArray, in_close, true, _height, _planeOuterColor, _planeInsideColor);
	}
}

void AGsTerritoryShape::DrawPlillar(bool in_close)
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

void AGsTerritoryShape::DrawFloor(const FTransform& in_transform)
{
	if (_floorComponent)
	{
		FTransform trs = in_transform;
		trs.SetLocation(FVector::ZeroVector);
		_floorComponent->SetRelativeLocation(FVector::ZeroVector);
		_floorComponent->Draw(trs, _floorColor, _pointArray);
	}
}

void AGsTerritoryShape::ClearFloor()
{
	_floorComponent->Clear();
}

FVector AGsTerritoryShape::GetRandomPointInTerritoryZone() const
{
	//나중에 다각형 안에서 랜덤한 값을 얻을 수 있도록 변경해줘야함	
	FRandomStream rand;
	rand.GenerateNewSeed();
	int32 num = _pointArray.Num();
	int32 index = rand.RandRange(0, num - 1);

	return _pointArray[index];	
}

void AGsTerritoryShape::SetColors(const FColor& in_pillar, const FColor& in_fenceOuter, const FColor& in_fenceInner, const FColor& in_floor)
{
	_pillarColor = in_pillar;
	_planeOuterColor = in_fenceOuter;
	_planeInsideColor = in_fenceInner;
	_floorColor = in_floor;
}

void AGsTerritoryShape::SetShapeOwner(AGsTerritoryActor* inOwner)
{
	IGsTerritoryShapeOwnerInterface* owner = Cast<IGsTerritoryShapeOwnerInterface>(inOwner);
	if (nullptr == owner)
	{
		return;
	}

	_owner = Cast<AGsTerritoryActor>(inOwner);		
}

void AGsTerritoryShape::SetColsedLoop(bool inClose)
{
	_splineComponent->SetClosedLoop(inClose);
}

bool AGsTerritoryShape::IsClosedShape() const
{
	return _splineComponent->IsClosedLoop();
}

void AGsTerritoryShape::ActiveSplineDebug(bool in_active)
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

void AGsTerritoryShape::GetDefaultPoints(EGsTerritoryShapeType inShape, const FVector& inLocation, OUT TArray<FVector>& outPointArray, float inDist, float inRot)
{
	switch (inShape)
	{
	case EGsTerritoryShapeType::Circle:
	{
		outPointArray.Empty();

		int32 num = DEFAULT_TERRITORY_CIRCLE_POINT_NUM;
		float gap = 360.0f / num;
		float degree = 0;
		FVector location;

		for (int32 i = 0; i < num; ++i)
		{
			degree = gap * i;
			location = FVector::ForwardVector.RotateAngleAxis(degree, FVector::UpVector) * inDist;

			outPointArray.Add(inLocation + location);
		}
	}
		break;
	case EGsTerritoryShapeType::Line:
	{
		outPointArray.Empty();

		outPointArray.Add(inLocation);
		
		FVector location = (FVector::ForwardVector.RotateAngleAxis(inRot, FVector::UpVector) * inDist) + inLocation;
		outPointArray.Add(location);
	}
		break;
	case EGsTerritoryShapeType::Polygon:
	{
		outPointArray.Empty();
		FVector location;

		//v1		
		outPointArray.Add(inLocation);

		//v2
		location = FVector(inDist, 0, 0);
		outPointArray.Add(inLocation + location);

		//v3
		location = FVector(0, inDist, 0);
		outPointArray.Add(inLocation + location);
	}
		break;
	case EGsTerritoryShapeType::Sqaure:
	{
		outPointArray.Empty();
		FVector location;
		float degree = 45;

		//v1		
		location = FVector::ForwardVector.RotateAngleAxis(inRot + 45, FVector::UpVector) * inDist;
		outPointArray.Add(inLocation + location);

		//v2 ~ v4
		for (int32 i = 1; i <= 3; ++i)
		{
			degree += 90;
			location = FVector::ForwardVector.RotateAngleAxis(degree, FVector::UpVector) * inDist;
			outPointArray.Add(inLocation + location);
		}
	}
	break;
	}
}

bool AGsTerritoryShape::IsInvalidePointNum(int32 in_num)
{
	switch (_shapeType)
	{
	case EGsTerritoryShapeType::Circle:
		return in_num < DEFAULT_TERRITORY_CIRCLE_POINT_NUM;
		break;	
	case EGsTerritoryShapeType::Polygon:
		return in_num < DEFAULT_TERRITORY_POLYGON_POINT_NUM;
		break;
	case EGsTerritoryShapeType::Sqaure:
		return in_num < DEFAULT_TERRITORY_SQUARE_POINT_NUM;
		break;
	}

	return false;
}

void AGsTerritoryShape::TransparentFloor()
{
	_floorComponent->TransparentFloor();

	/*UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return;
	}

	if (FGsTerritoryLevelManager::_isTransparentFloor)
	{
		_floorComponent->_dynamicMaterial = _floorComponent->CreateDynamicMaterialInstance(0, settings->_transparentMaterial.LoadSynchronous());
		if (_floorComponent && _floorComponent->_dynamicMaterial)
		{
			_floorComponent->_dynamicMaterial->BlendMode = EBlendMode::BLEND_Translucent;
			_floorComponent->_dynamicMaterial->SetScalarParameterValue(TEXT("Transparent"), FGsTerritoryLevelManager::_transparentFloorAlpha);
		}
	}
	else
	{
		_floorComponent->_dynamicMaterial = _floorComponent->CreateDynamicMaterialInstance(0, settings->_opaqueMaterial.LoadSynchronous());
		if (_floorComponent && _floorComponent->_dynamicMaterial)
		{
			_floorComponent->_dynamicMaterial->BlendMode = EBlendMode::BLEND_Translucent;
			_floorComponent->_dynamicMaterial->SetScalarParameterValue(TEXT("Transparent"), 1.0f);
		}
	}

	if (_floorComponent && _floorComponent->_dynamicMaterial)
	{
		_floorComponent->_dynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), _floorColor);
	}*/
}

class UMaterial* AGsTerritoryShape::GetMaterial()
{
	return _material;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryLine.h"
#include "Components/SceneComponent.h"
#include "GsTerritoryEditorSetting.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneCircleShape.h"
#include "Components/SplineComponent.h"
#include "Util/GsTerritoryLevelUtil.h"
#include "Util/GsTerritoryZoneShapeUtil.h"
#include "GameFramework/Actor.h"
#include "SplineComponentVisualizer.h"
#include "Blueprint/UserWidget.h"
#include "Component/GsTerrEdWidgetComponent.h"
#include "Actor/Shape/GsTerritoryShape.h"
#include "NavigationSystem/Public/NavigationData.h"
#include "AIModule/Classes/AITypes.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationSystem/Public/NavigationSystemTypes.h"

AGsTerritoryLine::AGsTerritoryLine()
{
	_splineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	if (_splineComponent)
	{
		_splineComponent->SetRelativeLocation(FVector::ZeroVector);
		_splineComponent->EditorUnselectedSplineSegmentColor = FColor::Blue;
		_splineComponent->EditorSelectedSplineSegmentColor = FColor::Yellow;

		SetRootComponent(_splineComponent);
	}

	_shapeActorChild = CreateDefaultSubobject<UChildActorComponent>(TEXT("ActorChild"));
	if (_shapeActorChild)
	{
		_shapeActorChild->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		_shapeActorChild->SetRelativeLocation(FVector::ZeroVector);
	}

	if (UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>())
	{
		_descWidgetBGColor = settings->_zoneColor;
	}

	bRunConstructionScriptOnDrag = false;
}

void AGsTerritoryLine::OnConstruction(const FTransform& in_transform)
{
	Super::OnConstruction(in_transform);
}

void AGsTerritoryLine::PostEditChangeProperty(FPropertyChangedEvent& inPropertyChangedEvent)
{
	Super::PostEditChangeProperty(inPropertyChangedEvent);

	if (FProperty* property = inPropertyChangedEvent.Property)
	{
		InvalidShape();
	}
}

void AGsTerritoryLine::DestroyManually()
{	
	Super::DestroyManually();
}

void AGsTerritoryLine::InvalidShape()
{
	CreateView(GetActorTransform());
}

void AGsTerritoryLine::CreateView(const FTransform& in_transform)
{
	if (nullptr == _shapeActorChild)
	{
		return;
	}

	_shapeActorChild->SetRelativeLocation(FVector::ZeroVector);
	AGsTerritoryShape* shape = Cast<AGsTerritoryShape>(_shapeActorChild->GetChildActor());
	if (nullptr == shape)
	{
		return;
	}
	
	if (CheckDefaultShape())
	{
		SetDefaultShape(in_transform);		
	}

	InitShape(in_transform);

	FTransform trs;
	trs.SetLocation(in_transform.GetLocation());
	shape->SetShapeOwner(this);
	shape->SetColsedLoop(_splineComponent->IsClosedLoop());
	shape->SetColors(_pillarColor, _fenceOuterColor, _fenceInsideColor);
	shape->SetFenceHeight(_height);
	shape->DrawShape(trs, _points, _radius);

	int32 num = _points.Num();
	if (_textRenderComponentArray.Num() != num)
	{
		UTextRenderComponent* textRender = nullptr;
		int32 gap = num - _textRenderComponentArray.Num();		
		if (gap > 0)
		{
			//Add textRenderCompnent
			for (int32 i = 0; i < gap; ++i)
			{
				textRender = NewObject<UTextRenderComponent>(this);
				textRender->RegisterComponent();
				textRender->AttachToComponent(_splineComponent, FAttachmentTransformRules::KeepWorldTransform);				
				_textRenderComponentArray.Add(textRender);
			}
		}
		else
		{
			//Remove textRenderCompnent
			TArray<UTextRenderComponent*> removeArray;
			gap = FMath::Abs(gap);
			for (int32 i = 0; i < gap; ++i)
			{				
				removeArray.Add(_textRenderComponentArray[i]);
			}

			for (int32 i = 0; i < gap; ++i)
			{
				textRender = removeArray[i];
				textRender->UnregisterComponent();
				_textRenderComponentArray.Remove(textRender);
			}
			removeArray.Empty();
		}	
	}
	
	if (_textRenderComponentArray.Num() == num)
	{
		FVector height = FVector(0, 0, _height + _indexTextHeight);
		for (int32 i = 0; i < num; ++i)
		{
			_textRenderComponentArray[i]->SetText(FText::FromString(FString::FromInt(i)));
			_textRenderComponentArray[i]->SetWorldLocation(_points[i] + height);
			_textRenderComponentArray[i]->SetTextRenderColor(_indexTextColor);
			_textRenderComponentArray[i]->SetXScale(_indexTextSize);
			_textRenderComponentArray[i]->SetYScale(_indexTextSize);
		}
	}
}

void AGsTerritoryLine::SetSplineLoopType()
{
	if (_shapeActorChild)
	{
		if (_splineComponent)
		{
			if (AActor* actor = _shapeActorChild->GetChildActor())
			{
				if (AGsTerritoryZoneShape* shape = Cast<AGsTerritoryZoneShape>(actor))
				{
					_splineComponent->SetClosedLoop(shape->IsClosedShape());
				}
			}
		}
	}
}

bool AGsTerritoryLine::CheckDefaultShape()
{
	AGsTerritoryShape* shape = Cast<AGsTerritoryShape>(_shapeActorChild->GetChildActor());
	if (nullptr == shape)
	{
		return false;
	}

	switch (_lineShape)
	{
	case EGsTerritoryShapeType::Line:
		return _points.Num() < DEFAULT_TERRITORY_LINE_POINT_NUM;
		break;
	case EGsTerritoryShapeType::Circle:
		return _points.Num() < DEFAULT_TERRITORY_CIRCLE_POINT_NUM;
		break;
	case EGsTerritoryShapeType::Polygon:
		return _points.Num() < DEFAULT_TERRITORY_POLYGON_POINT_NUM;
		break;
	default:
		break;
	}

	return false;
}

void AGsTerritoryLine::SetDefaultShape(const FTransform& in_transform)
{
	if (nullptr == _shapeActorChild)
	{
		return;
	}

	AGsTerritoryShape* shape = Cast<AGsTerritoryShape>(_shapeActorChild->GetChildActor());
	if (nullptr == shape)
	{
		return;
	}
				
	TArray<FVector> defaultArray;
	const FVector& origin = in_transform.GetLocation();
	AGsTerritoryShape::GetDefaultPoints(_lineShape, origin, defaultArray);
	_splineComponent->ClearSplinePoints();

	for (const FVector& point : defaultArray)
	{
		_splineComponent->AddSplinePoint(point, ESplineCoordinateSpace::World);
	}
}

void AGsTerritoryLine::InitShape(const FTransform& in_transform)
{
	if (nullptr == _shapeActorChild)
	{
		return;
	}

	if (_lineShape == EGsTerritoryShapeType::Circle)
	{
		InitCircle(in_transform);
	}
	else
	{
		InitPointArray();
	}
}


void AGsTerritoryLine::InitCircle(const FTransform& in_transform)
{
	if (nullptr == _splineComponent)
	{
		return;
	}

	const FVector& origin = in_transform.GetLocation();
	float distance = 0;
	int32 num = _splineComponent->GetNumberOfSplinePoints();
	FVector location;
	int32 out_selectIndex;

	if (UGsTerritoryZoneShapeUtil::TryGetSelectedIndexInSpline(out_selectIndex))
	{
		if (_splineComponent->GetNumberOfSplinePoints() >= out_selectIndex)
		{
			location = _splineComponent->GetWorldLocationAtSplinePoint(out_selectIndex);
			_radius = FVector::Distance(origin, location);
		}
	}

	if (0 == _radius)
	{
		_radius = DEFAULT_TERRITORY_FENCE_DISTANCE;
	}

	num = (num < DEFAULT_TERRITORY_CIRCLE_POINT_NUM ? DEFAULT_TERRITORY_CIRCLE_POINT_NUM : num);

	_splineComponent->ClearSplinePoints();

	if (num > 0)
	{
		float gap = 360.0f / num;
		float degree = 0;
		float startDegree = in_transform.GetRotation().Rotator().Yaw;
		FVector direction;

		for (int i = 0; i < num; ++i)
		{
			degree = (gap * i) + startDegree;
			direction = FVector::ForwardVector.RotateAngleAxis(degree, FVector::UpVector) * _radius;
			_splineComponent->AddSplinePoint(origin + direction, ESplineCoordinateSpace::World);
		}
	}

	InitPointArray();
}

void AGsTerritoryLine::InitPointArray()
{
	_points.Empty();
	if (nullptr == _splineComponent)
	{
		return;
	}
	
	int32 pointNum = _splineComponent->GetNumberOfSplinePoints();
	for (int32 i = 0; i < pointNum; ++i)
	{
		_points.Add(_splineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World));
	}	

	//add distance
	int32 lineNum = pointNum - 1;
	float dist = 0;
	if (lineNum >= 1)
	{		
		for (int32 i = 0; i < lineNum; ++i)
		{
			dist += (_points[i] - _points[i + 1]).Size();
		}
	}	
	_distance = dist;
}

void AGsTerritoryLine::ActiveSelectOutline(bool in_activeOutline)
{

}

void AGsTerritoryLine::Destroyed()
{
	if (_selectHandle.IsValid())
	{
		USelection::SelectObjectEvent.Remove(_selectHandle);
	}

	for (UTextRenderComponent* textRender : _textRenderComponentArray)
	{
		if (textRender)
		{
			textRender->UnregisterComponent();
		}
	}
	_textRenderComponentArray.Empty();

	Super::Destroyed();
}

void AGsTerritoryLine::InvalidWidgetPosition(const FVector& in_location)
{
#if TERR_ED_WIDGET_COMP
	if (nullptr == _widgetComp)
	{
		return;
	}

	FVector pos;
	FVector temp;
	float max = GetActorLocation().Z;
	
	pos = in_location + FVector(0, 0, _height + _widgetHeight);
	_widgetComp->SetWorldLocation(pos);
#endif

}

void AGsTerritoryLine::InvalidDestription()
{
	SetTerritoryTitleText(TEXT("Line"));

	_description = FString("Line spawn data\n")
		+ FString::Printf(TEXT("Line unique id : %lld\n"), _uniqueID)		
		+ FString::Printf(TEXT("Location : %s\n"), *GetActorLocation().ToString());	
}

bool AGsTerritoryLine::RecoveryTerritoryUniqueID()
{
	/*if (_uniqueID == INDEX_NONE)
	{
		_uniqueID = _zone.UniqueID;

		return Super::RecoveryTerritoryUniqueID();

		return true;
	}*/

	return false;
}

void AGsTerritoryLine::Copy()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT(" AGsTerritoryZone::Copy - label : %s"), *GetActorLabel());
}

void AGsTerritoryLine::SelectInternal(UObject* in_select)
{
	Super::SelectInternal(in_select);

	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::SelectInternal - label : %s"), *GetActorLabel());

	//if (this == in_select)
	//{
	//	if (AGsTerritorySpawnActor* terrainActor = Cast<AGsTerritorySpawnActor>(in_select))
	//	{
	//		ActiveSelectOutline(terrainActor->_zone == this);
	//	}
	//}
}

void AGsTerritoryLine::ChangeLineShape(float inRot, float inDist)
{
	_lineShape = EGsTerritoryShapeType::Line;
	if(nullptr == _splineComponent)
	{
		return;
	}	

	_splineComponent->SetClosedLoop(false);

	_splineComponent->ClearSplinePoints();
	TArray<FVector> pointArray;
	AGsTerritoryShape::GetDefaultPoints(EGsTerritoryShapeType::Line, GetActorLocation(), pointArray, inDist, inRot);
	_points = pointArray;
	for (const FVector& point : pointArray)
	{
		_splineComponent->AddSplinePoint(point, ESplineCoordinateSpace::World);
	}	

	InvalidShape();	
}

void AGsTerritoryLine::ChangeCircleShape(float inRadius)
{
	_lineShape = EGsTerritoryShapeType::Circle;
	if (nullptr == _splineComponent)
	{
		return;
	}

	_radius = inRadius;
	_splineComponent->SetClosedLoop(true);

	_splineComponent->ClearSplinePoints();
	TArray<FVector> pointArray;
	AGsTerritoryShape::GetDefaultPoints(EGsTerritoryShapeType::Circle, GetActorLocation(), pointArray, inRadius);
	_points = pointArray;
	for (const FVector& point : pointArray)
	{
		_splineComponent->AddSplinePoint(point, ESplineCoordinateSpace::World);
	}

	InvalidShape();
}

void AGsTerritoryLine::ChangePolygonShape()
{
	_lineShape = EGsTerritoryShapeType::Polygon;
	if (nullptr == _splineComponent)
	{
		return;
	}

	_splineComponent->SetClosedLoop(true);

	_splineComponent->ClearSplinePoints();
	TArray<FVector> pointArray;
	AGsTerritoryShape::GetDefaultPoints(EGsTerritoryShapeType::Polygon, GetActorLocation(), pointArray);
	_points = pointArray;
	for (const FVector& point : pointArray)
	{
		_splineComponent->AddSplinePoint(point, ESplineCoordinateSpace::World);
	}

	InvalidShape();
}

void AGsTerritoryLine::OptimizePath()
{
	UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (nullptr == navSys)
	{
		return;
	}

	TArray<FVector> newPathPointArray;
	int32 num = _points.Num();
	int32 lastIndex = num - 1;
	for (int32 i = 0; i < _points.Num(); ++i)
	{
		if (i == lastIndex)
		{
			break;
		}

		//길찾기 쿼리 셋팅
		FAIMoveRequest moveRequest(_points[i + 1]);
		moveRequest.SetUsePathfinding(true);
		moveRequest.SetAllowPartialPath(true);
		moveRequest.SetNavigationFilter(NULL);
		moveRequest.SetAcceptanceRadius(-1);
		moveRequest.SetReachTestIncludesAgentRadius(true);
		moveRequest.SetCanStrafe(true);

		const ANavigationData* navData = ((navSys == nullptr) ? nullptr : (moveRequest.IsUsingPathfinding() ? navSys->GetNavDataForProps(FNavAgentProperties::DefaultProperties, _points[i]) : navSys->GetAbstractNavData()));
		if (nullptr == navData)
		{
			continue;
		}

		FSharedConstNavQueryFilter navQueryQuery = UNavigationQueryFilter::GetQueryFilter(*navData, this, moveRequest.GetNavigationFilter());
		FPathFindingQuery pathFindQuery = FPathFindingQuery(nullptr, *navData, _points[i], _points[i + 1], navQueryQuery);
		pathFindQuery.SetAllowPartialPaths(moveRequest.IsUsingPartialPaths());
		
		//길찾기
		FPathFindingResult pathResult = navSys->FindPathSync(pathFindQuery);
		if (pathResult.IsSuccessful() && pathResult.Path.IsValid())
		{
			TArray<FNavPathPoint>& pathPointArray = pathResult.Path->GetPathPoints();

			for (FNavPathPoint navPathPoint : pathPointArray)
			{
				if (false == newPathPointArray.Contains(navPathPoint.Location))
				{
					newPathPointArray.Add(navPathPoint.Location);
				}					
			}			
		}
	}

	_points = newPathPointArray;
}

EGsLineShapeType AGsTerritoryLine::GetShapeType() const
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::GetShapeType - label : %s"), *GetActorLabel());

	//if (_shapeActorChild)
	//{
	//	if (AActor* shape = _shapeActorChild->GetChildActor())
	//	{
	//		if (AGsTerritoryZoneShape* zoneShape = Cast<AGsTerritoryZoneShape>(shape))
	//		{
	//			return zoneShape->GetZoneShapeType();
	//		}
	//	}
	//}

	//return EGsTerritoryZoneShapeType::Max;

	return EGsLineShapeType::Max;
}

void AGsTerritoryLine::CopyLine(AGsTerritoryLine* inLine)
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::CopyZoneInfo - label : %s"), *GetActorLabel());

	if (inLine)
	{
		_lineShape = inLine->_lineShape;
		_distance = inLine->_distance;
		_radius = inLine->_radius;
		_points = inLine->_points;

		_splineComponent->ClearSplinePoints();

		if (USplineComponent* sourceSpline = inLine->_splineComponent)
		{
			int32 num = sourceSpline->GetNumberOfSplinePoints();
			FVector location;
			for (int32 i = 0; i < num; ++i)
			{
				location = sourceSpline->GetWorldLocationAtSplinePoint(i);
				_splineComponent->AddSplinePoint(location, ESplineCoordinateSpace::World);
			}
		}

		CreateView(GetTransform());
		CreateWidget(GetTransform());
	}
}

TArray<FVector> AGsTerritoryLine::GetPoints()
{
	UE_LOG(LogTerritoryEditor, Log, TEXT("AGsTerritoryZone::GetPoints - label : %s"), *GetActorLabel());

	TArray<FVector> points;
	FVector location;
	int num = _splineComponent->GetNumberOfSplinePoints();

	for (int i = 0; i < num; ++i)
	{
		location = _splineComponent->GetWorldLocationAtSplinePoint(i);
		points.Add(location);
	}

	return points;
}

void AGsTerritoryLine::Export(FGsSchemaLineElement& outLineElement)
{
	FString levelName;
	FString levelKey;
	if (false == UGsTerritoryLevelUtil::IsValidServerLevel(GetLevel(), levelName, levelKey))
	{
		return;
	}

	FVector location;
	if (UGsTerritoryLevelUtil::IsSpawnActorSpawnOnNavmesh(this, location))
	{
		location += FVector(0, 0, 100);
	}
	else
	{
		location = GetActorLocation();

		UE_LOG(LogTerritoryEditor, Warning, TEXT("Zone is not on navmeh - label : %s\tstance id : %lld"), *GetActorLabel(), _instanceID);
	}

	outLineElement.ShapeType = _lineShape;
	outLineElement.Distance = _distance;
	outLineElement.Radius = _radius;
	outLineElement.VertextList = _points;	
}

void AGsTerritoryLine::Import(const FGsSchemaLineElement* inSpawnData)
{

}
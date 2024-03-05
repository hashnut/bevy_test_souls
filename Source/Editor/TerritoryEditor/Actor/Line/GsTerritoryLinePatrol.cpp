// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryLinePatrol.h"
#include "GsTerritoryEditorSetting.h"
#include "UTIL/GsText.h"
#include "Components/SplineComponent.h"

AGsTerritoryLinePatrol::AGsTerritoryLinePatrol()
{
	bRunConstructionScriptOnDrag = false;
}

void AGsTerritoryLinePatrol::InvalidDestription()
{
	SetTerritoryTitleText(TEXT("Line"));

	_description = FString::Printf(TEXT("Unique id : %lld\n"), _uniqueID);

	FString shapeType = FGsTextUtil::GetStringFromEnum(TEXT("EGsTerritoryShapeType"), _lineShape);

	_description += (TEXT("Shape : ") + shapeType);

	switch (_lineShape)
	{
	case EGsTerritoryShapeType::Circle:
	{		
		_description += FString::Printf(TEXT("Radius : %f\n"), _radius);
	}
		break;
	case EGsTerritoryShapeType::Line:
	case EGsTerritoryShapeType::Polygon:
	{
		_description += FString::Printf(TEXT("Distance : %f\n"), _distance);
	}
		break;
	default:
		break;
	}

	FString returnText = _isReturn ? TEXT("true") : TEXT("false");
	FString repeatText = _isRepeat ? TEXT("true") : TEXT("false");
	FString suicideText = _isSuicide ? TEXT("true") : TEXT("false");
	FString reverseText = _isReverse ? TEXT("true") : TEXT("false");

	_description += FString::Printf(TEXT("IsReturn : %s\n"), *returnText);
	_description += FString::Printf(TEXT("IsRepeat : %s\n"), *repeatText);
	_description += FString::Printf(TEXT("Repeat delay : %f\n"), _repeatDelay);
	_description += FString::Printf(TEXT("IsSuicide : %s\n"), *suicideText);
	_description += FString::Printf(TEXT("Reverse : %s\n"), *reverseText);
	_description += FString::Printf(TEXT("Line unique id : %lld\n"), _uniqueID);	
	_description += FString::Printf(TEXT("Location : %s\n"), *GetActorLocation().ToString());
}

void AGsTerritoryLinePatrol::Export(FGsSchemaLineElement& outLineElement)
{
	Super::Export(outLineElement);

	outLineElement.LineId = _uniqueID;
	outLineElement.isRepeat = _isRepeat;
	outLineElement.repeatDelay = _repeatDelay;
	outLineElement.isReturn = _isReturn;
	outLineElement.isSuicide = _isSuicide;	
	outLineElement.isReverse = _isReverse;
	outLineElement.Center = GetActorLocation();
}

void AGsTerritoryLinePatrol::Import(const FGsSchemaLineElement& inLineElement)
{
	_lineShape = inLineElement.ShapeType;
	_distance = inLineElement.Distance;
	_radius = inLineElement.Radius;
	_isReturn = inLineElement.isReturn;
	_isRepeat = inLineElement.isRepeat;
	_repeatDelay = inLineElement.repeatDelay;
	_isSuicide = inLineElement.isSuicide;
	_isReverse = inLineElement.isReverse;

	switch (_lineShape)
	{
	case EGsTerritoryShapeType::Line:
	{
		_splineComponent->SetClosedLoop(false);
	}
		break;
	case EGsTerritoryShapeType::Circle:
	case EGsTerritoryShapeType::Polygon:
	{
		_splineComponent->SetClosedLoop(true);
	}
		break;
	}
	
	_splineComponent->ClearSplinePoints();
	_points = inLineElement.VertextList;	
	for (const FVector& point : _points)
	{
		_splineComponent->AddSplinePoint(point, ESplineCoordinateSpace::World);
	}

	InvalidShape();
}

void AGsTerritoryLinePatrol::OptimizePath()
{
	switch (_lineShape)
	{
	case EGsTerritoryShapeType::Line:
	{
		_splineComponent->SetClosedLoop(false);
	}
	break;
	case EGsTerritoryShapeType::Circle:
	case EGsTerritoryShapeType::Polygon:
	{
		_splineComponent->SetClosedLoop(true);
	}
	break;
	}

	_splineComponent->ClearSplinePoints();


	Super::OptimizePath();

	for (const FVector& point : _points)
	{
		_splineComponent->AddSplinePoint(point, ESplineCoordinateSpace::World);
	}

	InvalidShape();
}

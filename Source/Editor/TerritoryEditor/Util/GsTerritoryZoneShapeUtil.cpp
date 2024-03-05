// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZoneShapeUtil.h"

//ue4
#include "Editor/UnrealEd/Classes/Editor/UnrealEdEngine.h"
#include "Editor/UnrealEd/Public/UnrealEdGlobals.h"
#include "Editor/ComponentVisualizers/Public/SplineComponentVisualizer.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"
#include "ProceduralMeshComponent.h"

//territory editor
#include "TerritoryEditor.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZonePolygonShape.h"
#include "Editor.h"

bool UGsTerritoryZoneShapeUtil::TryGetSelectedIndexInSpline(int32& outIndex)
{
	outIndex = INVALIDE_TERIITORY_SPLINE_INDEX;

	if (nullptr == GUnrealEd)
	{
		return false;
	}

	TSharedPtr<FSplineComponentVisualizer> splineVisualiser = StaticCastSharedPtr<FSplineComponentVisualizer>(GUnrealEd->FindComponentVisualizer(USplineComponent::StaticClass()));
	if (false == splineVisualiser.IsValid())
	{
		return false;
	}

	const TSet<int32>& indexArray = splineVisualiser.Get()->GetSelectedKeys();

	if (indexArray.Num() > 0)
	{
		outIndex = indexArray.Array()[0];

		return true;
	}

	return false;
}

bool UGsTerritoryZoneShapeUtil::TryGetDistanceFromSplinePoint(float& outDistance, USplineComponent* inSplineComp , const FVector& inLocation)
{
	int32 inIndex;
	
	if (TryGetSelectedIndexInSpline(inIndex))
	{
		if (inSplineComp->GetNumberOfSplinePoints() >= inIndex)
		{
			FVector location = inSplineComp->GetWorldLocationAtSplinePoint(inIndex);
			outDistance = FVector::Distance(inLocation, location);

			return true;
		}
	}

	return false;
}

TArray<FVector> UGsTerritoryZoneShapeUtil::CreateCircleRelativePoints(float inDistance /*= 100.0f*/, int32 inNum /*= 8*/)
{
	TArray<FVector> points;

	if (inNum == 0)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("divide number is zero"));

		return points;
	}

	if (inNum < INVALIDE_TERIITORY_SPLINE_INDEX)
	{
		UE_LOG(LogTerritoryEditor, Error, TEXT("divide number lower than 8"));

		return points;
	}
		
	float gap = 360.0f / inNum;
	float degree = 0;
	FVector location;

	for (int32 i = 0; i < inNum; ++i)
	{
		degree = gap * i;
		location = FVector::ForwardVector.RotateAngleAxis(degree, FVector::UpVector) * inDistance;

		points.Add(location);
	}

	return points;
}

bool UGsTerritoryZoneShapeUtil::IsValidCircleIndex(USplineComponent* inSpline, int32 inIndex)
{	
	if ((inIndex >= 0) && (inIndex < DEFAULT_TERRITORY_CIRCLE_POINT_NUM))
	{
		if (inSpline)
		{
			return (inSpline->GetNumberOfSplinePoints() > inIndex);
		}
	}
	
	return false;
}

bool UGsTerritoryZoneShapeUtil::TryGetRadius(FVector inCenter, USplineComponent* inSpline, int32 inIndex, float& outRadius)
{
	if (nullptr == inSpline)
	{
		return false;
	}

	if (false == IsValidCircleIndex(inSpline, inIndex))
	{
		return false;
	}

	FVector location = inSpline->GetWorldLocationAtSplinePoint(inIndex);
	outRadius = FVector::Distance(inCenter, location);

	return true;
}

void UGsTerritoryZoneShapeUtil::AddTriangle(int32 v1, int32 v2, int32 v3, TArray<int32>& in_triangleArray)
{
	in_triangleArray.Add(v1);
	in_triangleArray.Add(v2);
	in_triangleArray.Add(v3);
}

bool UGsTerritoryZoneShapeUtil::TryGetCircleShapePoints(TArray<FVector>& outPoints, int32 inNum /*= DEFAULT_TERRITORY_CIRCLE_POINT_NUM*/, float inRadius)
{
	TArray<FVector> points;

	inNum = (inNum < DEFAULT_TERRITORY_CIRCLE_POINT_NUM ? DEFAULT_TERRITORY_CIRCLE_POINT_NUM : inNum);
	
	if (inNum > 0)
	{
		float degree = 360.0f / inNum;
		float currentDegree = 0;
		FVector currentPoint;

		for (int i = 0; i < inNum; ++i)
		{
			currentDegree = degree * i;
			currentPoint = FVector::ForwardVector.RotateAngleAxis(currentDegree, FVector::UpVector) * inRadius;

			points.Add(currentPoint);

			return true;
		}
	}

	return false;
}

bool UGsTerritoryZoneShapeUtil::InvalidCircleNum(int32 inNum)
{
	return (inNum < DEFAULT_TERRITORY_CIRCLE_POINT_NUM);
}

FVector UGsTerritoryZoneShapeUtil::FindCentroid(const TArray<FVector>& inPointArray, float& outMaxRadius)
{
	TArray<FVector2D> pointArray;
	GetPoint2DList(inPointArray, pointArray);

	//https://stackoverflow.com/questions/5271583/center-of-gravity-of-a-polygon
	FVector2D off = pointArray[0];
	float twicearea = 0;
	float x = 0;
	float y = 0;	
	FVector2D p1, p2;
	float f;
	for (int i = 0, j = pointArray.Num() - 1; i < pointArray.Num(); j = i++) {
		p1 = pointArray[i];
		p2 = pointArray[j];
		f = (p1.X - off.X) * (p2.Y - off.Y) - (p2.X - off.X) * (p1.Y - off.Y);
		twicearea += f;
		x += (p1.X + p2.X - 2 * off.X) * f;
		y += (p1.Y + p2.Y - 2 * off.Y) * f;
	}

	f = twicearea * 3;
	FVector2D centroid = FVector2D(x / f + off.X, y / f + off.Y);
	float max = 0;
	float length = 0;
	for (FVector2D point : pointArray)
	{		
		length = (point - centroid).Size();
		if (max < length)
		{
			max = length;
		}
	}

	outMaxRadius = max;

	return FVector(x / f + off.X, y / f + off.Y, inPointArray[0].Z);
}

int32 UGsTerritoryZoneShapeUtil::CreatePillarBunch(UProceduralMeshComponent* inMeshComponent, int32 inStartSection, const TArray<FVector>& inPointArray, float inHeight, UMaterial* inMaterial, FColor inColor)
{
	int32 num = inPointArray.Num();

	for (int32 i = 0; i < num; ++i)
	{
		//Draw outer
		CreatePillar(inMeshComponent, inStartSection + i, inPointArray[i], inHeight, inMaterial, inColor);
	}

	//Set Collsion
	SetNoCollision(inMeshComponent);

	return inStartSection + num + 1;
}

void UGsTerritoryZoneShapeUtil::CreatePillar(UProceduralMeshComponent* inMeshComponent, int32 inSection, const FVector& inLocation, float inHeight, UMaterial* inMaterial, FColor inColor)
{
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	FVector v1, v2, v3, v4, v5, v6, v7, v8;
	float factor = 10.0f;

	v1 = (FVector(-1, -1, 0) * factor) + inLocation;
	v2 = (FVector(1, -1, 0) * factor) + inLocation;
	v3 = (FVector(1, 1, 0) * factor) + inLocation;
	v4 = (FVector(-1, 1, 0) * factor) + inLocation;

	v5 = (FVector(-1, -1, 0) * factor + FVector(0, 0, inHeight)) + inLocation;
	v6 = (FVector(1, -1, 0) * factor + FVector(0, 0, inHeight)) + inLocation;
	v7 = (FVector(1, 1, 0) * factor + FVector(0, 0, inHeight)) + inLocation;
	v8 = (FVector(-1, 1, 0) * factor + FVector(0, 0, inHeight)) + inLocation;

	//vertex
	TArray<FVector> vertexs;

	vertexs.Add(v1);
	vertexs.Add(v2);
	vertexs.Add(v3);
	vertexs.Add(v4);

	vertexs.Add(v5);
	vertexs.Add(v6);
	vertexs.Add(v7);
	vertexs.Add(v8);

	//triangle
	TArray<int32> triangles;

	//bottom
	UGsTerritoryZoneShapeUtil::AddTriangle(0, 1, 2, triangles);
	UGsTerritoryZoneShapeUtil::AddTriangle(0, 2, 3, triangles);

	//Up
	UGsTerritoryZoneShapeUtil::AddTriangle(4, 6, 5, triangles);
	UGsTerritoryZoneShapeUtil::AddTriangle(4, 7, 6, triangles);

	//front
	UGsTerritoryZoneShapeUtil::AddTriangle(0, 5, 1, triangles);
	UGsTerritoryZoneShapeUtil::AddTriangle(0, 4, 5, triangles);

	//back
	UGsTerritoryZoneShapeUtil::AddTriangle(3, 2, 6, triangles);
	UGsTerritoryZoneShapeUtil::AddTriangle(3, 6, 7, triangles);

	//Right
	UGsTerritoryZoneShapeUtil::AddTriangle(4, 6, 5, triangles);
	UGsTerritoryZoneShapeUtil::AddTriangle(4, 7, 6, triangles);

	//right
	UGsTerritoryZoneShapeUtil::AddTriangle(1, 6, 2, triangles);
	UGsTerritoryZoneShapeUtil::AddTriangle(1, 5, 6, triangles);

	//Left
	UGsTerritoryZoneShapeUtil::AddTriangle(0, 3, 7, triangles);
	UGsTerritoryZoneShapeUtil::AddTriangle(0, 7, 4, triangles);

	//normal
	TArray<FVector> normals;

	//normals.Add(FVector(0, 0, 1));
	//normals.Add(FVector(0, 0, 1));
	//normals.Add(FVector(0, 0, 1));
	//normals.Add(FVector(0, 0, 1));
	//normals.Add(FVector(0, 0, 1));
	//normals.Add(FVector(0, 0, 1));
	//normals.Add(FVector(0, 0, 1));
	//normals.Add(FVector(0, 0, 1));

	//uv0
	TArray<FVector2D> uv;

	//uv.Add(FVector2D(0, 0));
	//uv.Add(FVector2D(0, 1));
	//uv.Add(FVector2D(1, 1));
	//uv.Add(FVector2D(1, 0));

	inMeshComponent->CreateMeshSection_LinearColor(inSection, vertexs, triangles, normals, uv, vertexColors, tangents, false);

	SetMaterial(inMeshComponent, inSection, inMaterial, inColor);
}

int32 UGsTerritoryZoneShapeUtil::CreateVerticalStripe(UProceduralMeshComponent* inMeshComponent, int32 inStartSection, const TArray<FVector>& inPointArray, bool inLoop, float inHeight, UMaterial* inMaterial, FColor inColor)
{
	//Draw outer
	CreateVerticalStripeSide(inMeshComponent, inStartSection, inPointArray, true, inLoop, inHeight, inMaterial, inColor);

	//Draw inner
	CreateVerticalStripeSide(inMeshComponent, inStartSection + 1, inPointArray, false, inLoop, inHeight, inMaterial, inColor);

	return inStartSection + 2;
}

void UGsTerritoryZoneShapeUtil::CreateVerticalStripeSide(UProceduralMeshComponent* inMeshComponent, int32 inSection, const TArray<FVector>& inPointArray, bool inClockOrient, bool inLoop, float inHeight, UMaterial* inMaterial, FColor inColor)
{
	int32 vertexNum = inPointArray.Num();
	int32 indexMax = vertexNum - 1;

	//vertext buffer
	TArray<FVector> vertexs;
	TArray<FVector> normals;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	//uv
	TArray<FVector2D> uv;
	FVector2D startUV;
	FVector2D curUV;
	float gap = 1.0f / (float)indexMax;
	startUV = FVector2D(1, 0);
	curUV = startUV;

	for (int32 i = 0; i < vertexNum; ++i)
	{
		vertexs.Add(inPointArray[i]);
		normals.Add(FVector(0, 0, 1));

		if (i == indexMax)
		{
			uv.Add(FVector2D(1, 1));
		}
		else
		{
			curUV += FVector2D(0, gap);
			uv.Add(curUV);
		}
	}

	startUV = FVector2D(0, 0);
	curUV = startUV;

	for (int32 i = 0; i < vertexNum; ++i)
	{
		vertexs.Add(inPointArray[i] + FVector(0, 0, inHeight));
		normals.Add(FVector(0, 0, 1));

		if (i == indexMax)
		{
			uv.Add(FVector2D(0, 1));
		}
		else
		{
			curUV += FVector2D(0, gap);
			uv.Add(curUV);
		}
	}

	//Index buffer
	TArray<int32> triangles;

	for (int32 i = 0; i < vertexNum; ++i)
	{
		if (inClockOrient)
		{
			if (i == indexMax)
			{
				if (inLoop)
				{
					triangles.Add(i);
					triangles.Add(i + vertexNum);
					triangles.Add(i + 1);

					triangles.Add(i);
					triangles.Add(i + 1);
					triangles.Add(0);
				}
			}
			else
			{
				triangles.Add(i);
				triangles.Add(i + vertexNum);
				triangles.Add(i + vertexNum + 1);

				triangles.Add(i);
				triangles.Add(i + vertexNum + 1);
				triangles.Add(i + 1);
			}
		}
		else
		{
			if (i == indexMax)
			{
				if (inLoop)
				{
					triangles.Add(i);
					triangles.Add(i + 1);
					triangles.Add(i + vertexNum);

					triangles.Add(i);
					triangles.Add(0);
					triangles.Add(i + 1);
				}
			}
			else
			{
				triangles.Add(i);
				triangles.Add(i + vertexNum + 1);
				triangles.Add(i + vertexNum);

				triangles.Add(i);
				triangles.Add(i + 1);
				triangles.Add(i + 1 + vertexNum);
			}
		}
	}

	inMeshComponent->CreateMeshSection_LinearColor(inSection, vertexs, triangles, normals, uv, vertexColors, tangents, false);
	SetMaterial(inMeshComponent, inSection, inMaterial, inColor);
}

void UGsTerritoryZoneShapeUtil::CreateFloorPolygon(UProceduralMeshComponent* inMeshComponent, int32 inSection, const TArray<FVector>& inPointArray, float inHeight, UMaterial* inMaterial, FColor inColor)
{
	TArray<FPoly> polyArray;

	if (false == TryGetTriangles(inPointArray, polyArray))
	{
		return;
	}

	int32 num = polyArray.Num();
	int32 trianglePointNum = num * 3;

	TArray<FVector> vertexArray;
	TArray<int32> indexArray;

	GetVertexArray(polyArray, vertexArray);
	TryGetIndexArray(vertexArray, polyArray, indexArray);

	TArray<FVector> normals;
	TArray<FVector2D> uv;
	FVector center;
	float maxRadius;
	float uvValue;

	center = UGsTerritoryZoneShapeUtil::FindCentroid(vertexArray, maxRadius);	

	for (FVector& iter : vertexArray)
	{
		normals.Add(FVector(0, 0, 1));
		uvValue = (iter - center).Size() / maxRadius;
		uv.Add(FVector2D(uvValue, uvValue));
	}

	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	for (int32 i = 0; i < trianglePointNum; ++i)
	{
		tangents.Add(FProcMeshTangent(0, 0, 1));
		vertexColors.Add(FLinearColor::Blue);
	}

	inMeshComponent->CreateMeshSection_LinearColor(inSection, vertexArray, indexArray, normals, uv, vertexColors, tangents, true);

	for (int32 i = 0; i < num; ++i)
	{
		SetMaterial(inMeshComponent, inSection + i, inMaterial, inColor);
	}

	SetTerritoryZoneCollision(inMeshComponent);
}

FVector UGsTerritoryZoneShapeUtil::GetCenterLocation(const USplineComponent* inSplineComponent)
{
	if (inSplineComponent)
	{
		int32 num = inSplineComponent->GetNumberOfSplinePoints();
		FVector center;

		if (num > 0)
		{
			for (int32 i = 0; i < num; ++i)
			{
				center += inSplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			}

			return (center / num);
		}		
	}

	return FVector::ZeroVector;
}

bool UGsTerritoryZoneShapeUtil::TryGetTriangles(const TArray<FVector>& inPointArray, TArray<FPoly>& outPolyArray)
{
	outPolyArray.Empty();
	FPoly poly;
	FVector temp;

	for (int i = 0; i < inPointArray.Num(); i++)
	{
		temp = inPointArray[i];
		poly.InsertVertex(i, temp);
	}

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (nullptr == world)
	{
		return false;
	}

	if (poly.Triangulate(world->GetDefaultBrush(), outPolyArray) > 0)
	{
		return true;
	}

	return false;
}

void UGsTerritoryZoneShapeUtil::GetVertexArray(const TArray<FPoly>& inPolyArray, TArray<FVector>& outVertexArray)
{
	int32 num = inPolyArray.Num();
	TSet<FVector> pointSet;

	for (int32 i = 0; i < num; ++i)
	{
		for (const FVector& vertextIter : inPolyArray[i].Vertices)
		{
			if (false == pointSet.Contains(vertextIter))
			{
				pointSet.Add(vertextIter);
			}
		}
	}

	outVertexArray.Empty();
	outVertexArray = pointSet.Array();	
}

bool UGsTerritoryZoneShapeUtil::TryGetIndexArray(const TArray<FVector>& inVertexArray, const TArray<FPoly>& inPolyArray, TArray<int32>& outIndexArray)
{
	//Organize unique point	
	int32 index;
	int32 vertextNum;

	for (const FPoly& iterPloy : inPolyArray)
	{
		vertextNum = iterPloy.Vertices.Num();

		for (int i = vertextNum - 1; i >= 0; --i)
		{
			if (false == inVertexArray.Find(iterPloy.Vertices[i], index))
			{
				return false;
			}

			outIndexArray.Add(index);
		}
	}

	return true;
}

void UGsTerritoryZoneShapeUtil::SetMaterial(UProceduralMeshComponent* inMeshComponent, int32 inSection, UMaterial* inMaterial, FColor inColor)
{	
	if (inMaterial)
	{
		UMaterialInstanceDynamic* instanceMaterial = UMaterialInstanceDynamic::Create(inMaterial, inMeshComponent->GetOwner());
		instanceMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor(inColor));
		inMeshComponent->SetMaterial(inSection, instanceMaterial);
	}
}

void UGsTerritoryZoneShapeUtil::SetNoCollision(UProceduralMeshComponent* inMeshComponent)
{
	if (inMeshComponent)
	{
		inMeshComponent->ContainsPhysicsTriMeshData(false);
		inMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		inMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		inMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel14);
		inMeshComponent->SetNotifyRigidBodyCollision(false);
		inMeshComponent->SetGenerateOverlapEvents(false);
		inMeshComponent->SetCastShadow(true);
	}
}

void UGsTerritoryZoneShapeUtil::SetTerritoryZoneCollision(UProceduralMeshComponent* inMeshComponent)
{
	if (inMeshComponent)
	{
		inMeshComponent->ContainsPhysicsTriMeshData(false);
		inMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		inMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

		inMeshComponent->SetNotifyRigidBodyCollision(false);
		inMeshComponent->SetGenerateOverlapEvents(false);
		inMeshComponent->SetCastShadow(true);

		inMeshComponent->SetCollisionProfileName(TERRITORY_ZONE_BOTTOM_COLLISION_PRESET);
	}
}

void UGsTerritoryZoneShapeUtil::CreatePolygonShape(UProceduralMeshComponent* inMeshComponent, const TArray<FVector>& inPointList, float inHeight, UMaterial* inMaterial, FColor inFloorColor, FColor inVerticalStripe, FColor inPillarColor)
{
	//create floor
	CreateFloorPolygon(inMeshComponent, 0, inPointList, inHeight, inMaterial, inFloorColor);

	//create vertical stripe
	int32 nextSection = CreateVerticalStripe(inMeshComponent, 1, inPointList, true, inHeight, inMaterial, inVerticalStripe);

	//create pillar
	CreatePillarBunch(inMeshComponent, nextSection, inPointList, inHeight, inMaterial, inPillarColor);
}

bool UGsTerritoryZoneShapeUtil::IsValidPolygonNum(int32 inNum)
{
	return (inNum < DEFAULT_TERRITORY_POLYGON_POINT_NUM && inNum > 0);
}

void UGsTerritoryZoneShapeUtil::GetDefaultPolygonShape(TArray<FVector>& outPointArray)
{	
	outPointArray.Empty();
	FVector location;

	//v1
	location = FVector::ZeroVector;
	outPointArray.Add(location);

	//v2
	location = FVector(DEFAULT_TERRITORY_FENCE_DISTANCE, 0, 0);
	outPointArray.Add(location);

	//v3
	location = FVector(0, DEFAULT_TERRITORY_FENCE_DISTANCE, 0);
	outPointArray.Add(location);
}

void UGsTerritoryZoneShapeUtil::InitDefualtPolygonShape(USplineComponent* inSplineComponent)
{
	if (inSplineComponent)
	{
		inSplineComponent->SetClosedLoop(true);

		if (inSplineComponent->GetNumberOfSplinePoints() < DEFAULT_TERRITORY_POLYGON_POINT_NUM)
		{
			TArray<FVector> pointList;

			GetDefaultPolygonShape(pointList);

			inSplineComponent->ClearSplinePoints();

			for (const FVector& point : pointList)
			{
				inSplineComponent->AddSplinePoint(point, ESplineCoordinateSpace::Local);
			}
		}		
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Engine/Polys.h"
#include "Components/SplineComponent.h"
#include "TerritoryEditor.h"
#include "GsTerritoryZoneShapeUtil.generated.h"

/**
 * 
 */
struct FGsProceduralMeshData
{
public:
	TArray<FVector> vertexList;
	TArray<int32> triangleList;

 };


 class UProceduralMeshComponent;
 class USplineComponent;

UCLASS()
class TERRITORYEDITOR_API UGsTerritoryZoneShapeUtil : public UObject
{
	GENERATED_BODY()

public:
	static bool TryGetSelectedIndexInSpline(int32& outIndex);
	static bool TryGetDistanceFromSplinePoint(float& outDistance, USplineComponent* inSplineComp, const FVector& inLocation);
	static TArray<FVector> CreateCircleRelativePoints(float inDistance = 100.0f, int32 inNum = 8);
	static bool IsValidCircleIndex(USplineComponent* inSpline, int32 inIndex);
	static bool TryGetRadius(FVector inCenter, USplineComponent* inSpline, int32 inIndex, float& outRadius);
	static void AddTriangle(int32 v1, int32 v2, int32 v3, TArray<int32>& in_triangleArray);

public:
	static bool TryGetCircleShapePoints(TArray<FVector>& outPoints, int32 inNum = DEFAULT_TERRITORY_CIRCLE_POINT_NUM, float inRadius = DEFAULT_TERRITORY_FENCE_DISTANCE);
	static bool InvalidCircleNum(int32 inNum);

	//draw default component
public:
	//다각형의 무게 중심 값을 얻는 함수 
	static FVector FindCentroid(const TArray<FVector>& inPointArray, float& outMaxRadius);

	template<typename T>
	static void GetPointList(USplineComponent* inSpline, TArray<T>& outPointList, ESplineCoordinateSpace::Type inType = ESplineCoordinateSpace::Type::Local)
	{
		outPointList.Empty();
		int num = inSpline->GetNumberOfSplinePoints();

		for (int i = 0; i < num; ++i)
		{
			outPointList.Add(T(inSpline->GetLocationAtSplinePoint(i, inType)));
		}
	}
	
	static void GetPoint2DList(const TArray<FVector>& inPointList, TArray<FVector2D>& outPointList)
	{
		outPointList.Empty();
		for (int i = 0; i < inPointList.Num(); ++i)
		{
			outPointList.Add((FVector2D)inPointList[i]);
		}
	}

public:
	static int32 CreatePillarBunch(UProceduralMeshComponent* inMeshComponent, int32 inStartSection, const TArray<FVector>& inPointArray, float inHeight, UMaterial* inMaterial, FColor inColor);
	static void CreatePillar(UProceduralMeshComponent* inMeshComponent, int32 inSection, const FVector& inLocation, float inHeight, UMaterial* inMaterial, FColor inColor);

public:
	static int32 CreateVerticalStripe(UProceduralMeshComponent* inMeshComponent, int32 inStartSection, const TArray<FVector>& inPointArray, bool inLoop, float inHeight, UMaterial* inMaterial, FColor inColor);
	static void CreateVerticalStripeSide(UProceduralMeshComponent* inMeshComponent, int32 inStartSection, const TArray<FVector>& inPointArray, bool inClockOrient, bool inLoop, float inHeight, UMaterial* inMaterial, FColor inColor);

public:
	static void CreateFloorPolygon(UProceduralMeshComponent* inMeshComponent, int32 inSection, const TArray<FVector>& inPointArray, float inHeight, UMaterial* inMaterial, FColor inColor);	

	template<typename T>
	static bool TryGetMaxDistance(const TArray<T>& inVertexArray, T& outCenter, float& outDistance)
	{
		int32 num = inVertexArray.Num();
		T accu = T::ZeroVector;

		if (num > 0)
		{
			for (const T& iter : inVertexArray)
			{
				accu += iter;
			}

			outCenter = accu / num;

			float max = LONG_MIN;
			float value;

			for (const T& iter : inVertexArray)
			{
				value = (iter - outCenter).Size();

				if (max < value)
				{
					max = value;
				}
			}

			outDistance = max;

			return true;
		}

		return false;
	}

	static FVector GetCenterLocation(const USplineComponent* inSplineComponent);

private:
	static bool TryGetTriangles(const TArray<FVector>& inPointArray, TArray<FPoly>& outPolyArray);
	static void GetVertexArray(const TArray<FPoly>& inPolyArray, TArray<FVector>& outVertexArray);
	static bool TryGetIndexArray(const TArray<FVector>& inVertexArray, const TArray<FPoly>& inPolyArray, TArray<int32>& outIndexArray);	

public:
	static void SetMaterial(UProceduralMeshComponent* inMeshComponent, int32 inSection, UMaterial* inMaterial, FColor inColor);
	static void SetNoCollision(UProceduralMeshComponent* inMeshComponent);
	static void SetTerritoryZoneCollision(UProceduralMeshComponent* inMeshComponent);

	//Draw shape
public:
	static void CreatePolygonShape(UProceduralMeshComponent* inMeshComponent, const TArray<FVector>& inPointList, float inHeight, UMaterial* inMaterial, FColor inFloorColor, FColor inVerticalStripe, FColor inPillarColor);
	static bool IsValidPolygonNum(int32 inNum);
	static void GetDefaultPolygonShape(TArray<FVector>& outPointArray);
	static void InitDefualtPolygonShape(USplineComponent* inSplineComponent);	
};

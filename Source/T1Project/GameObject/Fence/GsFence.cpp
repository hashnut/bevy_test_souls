// Fill out your copyright notice in the Description page of Project Settings.


#include "GsFence.h"
#include "UTIL/GsText.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "GsFenceHandler.h"
#include "Management/ScopeGame/GsSpaceCrackManager.h"
#include "UTIL/GsDrawDebugHelpers.h"
#include "DrawDebugHelpers.h"

bool FGsFence::IsInPolygon(const TArray<FVector2D>& inPointArray, const FVector2D& inPoint)
{
	//출처 : https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
		//사각형 안에 없으면 패스
	if (false == IsInRectangle(inPointArray, inPoint))
	{
		return false;
	}

	//좌표로 부터 오른쪽에 레이를 쏴서 겹치는 선분이 홀 수 개이면 사각형 안에 있는것이다.
	int32 num = inPointArray.Num();
	bool isInSide = false;
	for (int32 i = 0, j = num - 1; i < num; j = i++)
	{
		if (((inPointArray[i].Y > inPoint.Y) != (inPointArray[j].Y > inPoint.Y)) &&
			(inPoint.X < (inPointArray[j].X - inPointArray[i].X) * (inPoint.Y - inPointArray[i].Y) / (inPointArray[j].Y - inPointArray[i].Y) + inPointArray[i].X))
		{
			isInSide = !isInSide;
		}
	}

	return isInSide;
}

bool FGsFence::ReEnter()
{
	Leave();
	return Enter();
}

bool FGsFence::IsInFence(const FVector& inLocation) const
{
#if TERRITORY_DEBUG
	FVector2D fencePos = FVector2D(_fence->Center);
	FVector2D playerPos = FVector2D(inLocation);
	float length = (fencePos - playerPos).Size();
	if (length <= _fence->Radius)
	{
		TArray<FVector2D> pointArray;
		if (TryGetPointArray(pointArray))
		{
			return FGsFence::IsInPolygon(pointArray, FVector2D(inLocation));
		}
	}
#else
	if ((FVector2D(_fence->Center) - FVector2D(inLocation)).Size() <= _fence->Radius)
	{
		TArray<FVector2D> pointArray;
		if (TryGetPointArray(pointArray))
		{
			return FGsFence::IsInPolygon(pointArray, FVector2D(inLocation));
		}
	}
#endif

	return false;
}

bool FGsFence::IsInRectangle(const TArray<FVector2D>& inPointList, const FVector2D& inPoint)
{
	float minY = TNumericLimits<float>::Max();
	float maxY = TNumericLimits<float>::Lowest();
	float minX = TNumericLimits<float>::Max();
	float maxX = TNumericLimits<float>::Lowest();

	for (const FVector2D& point : inPointList)
	{
		if (point.X > maxX)
		{
			maxX = point.X;
		}

		if (minX > point.X)
		{
			minX = point.X;
		}

		if (point.Y > maxY)
		{
			maxY = point.Y;
		}

		if (minY > point.Y)
		{
			minY = point.Y;
		}
	}

	if (inPoint.X > maxX
		|| inPoint.X < minX
		|| inPoint.Y > maxY
		|| inPoint.Y < minY)
	{
		return false;
	}

	return true;
}

bool FGsFence::TryGetPointArray(OUT TArray<FVector2D>& outPointArray) const
{
	outPointArray.Empty();

	for (int32 i = 0; i < _fence->vertexList.Num(); ++i)
	{
		outPointArray.Add(FVector2D(_fence->vertexList[i]));
	}

	return outPointArray.Num() > 0;
}

void FGsFence::Draw(UWorld* inWorld)
{
	if (const FGsSchemaFenceElement* fenceData = _fence)
	{
		FGsDrawDebugHelpers::DrawDebugFence(inWorld, _fence->vertexList);
		DrawDebugString(inWorld, fenceData->Center, GetFenceDescription(), NULL, FColor::White, 300);

		DrawDebugSphere(inWorld, fenceData->Center, fenceData->Radius / 10.0f, 24, FColor::Red, false, 300);
	}
}
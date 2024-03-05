#pragma once
#include "CoreMinimal.h"

// bak1210 : 언리얼 내부 함수를 3D 좌표계를 2D 계산 가능하도록 컨버팅한 함수

struct FGsMath
{
	// SphereAABBIntersection2D 는 회전처리가 불가능하기때문에 로컬좌표로 변환한후 사용하도록 한다.
	// SphereCenter : 체크할 점(로컬좌표로 변환된 점)
	// RadiusSquared : 체크할 반지름
	// RectMin : 체크할 Rect 최소점
	// RectMin : 체크할 Rect 최대점
	static bool SphereAABBIntersection2D(const FVector2D& SphereCenter, const float RadiusSquared,
		const FVector2D& RectMin, const FVector2D& RectMax)
	{
		// Accumulates the distance as we iterate axis
		float DistSquared = 0.f;
		// Check each axis for min/max and add the distance accordingly
		// NOTE: Loop manually unrolled for > 2x speed up
		if (SphereCenter.X < RectMin.X)
		{
			DistSquared += FMath::Square(SphereCenter.X - RectMin.X);
		}
		else if (SphereCenter.X > RectMax.X)
		{
			DistSquared += FMath::Square(SphereCenter.X - RectMax.X);
		}
		if (SphereCenter.Y < RectMin.Y)
		{
			DistSquared += FMath::Square(SphereCenter.Y - RectMin.Y);
		}
		else if (SphereCenter.Y > RectMax.Y)
		{
			DistSquared += FMath::Square(SphereCenter.Y - RectMax.Y);
		}
		// If the distance is less than or equal to the radius, they intersect
		return DistSquared <= RadiusSquared;
	}

	// 방향을 가진 2D선분에 투영한 점의 길이를 가져온다.
	// Point : 선분에 투영할 점
	// Direction : 2D선분의 방향
	// Origin : 2D선분의 시작점
	static float PointDistToOnLine(const FVector2D &Point, const FVector2D &Direction, const FVector2D &Origin)
	{
		const FVector2D SafeDir = Direction.GetSafeNormal();
		const FVector2D OutClosestPoint = Origin + (SafeDir * ((Point - Origin) | SafeDir));
		return (OutClosestPoint - Point).Size();
	}
	
	// x, y 좌표로 각도 구하기	
	static float CalcDegreesFromCoordinate(float In_x, float In_y)
	{
		float resVal = 0.f;
		float rad = FMath::Atan2(In_x, In_y);
		resVal = FMath::RadiansToDegrees(rad);
		return resVal;
	}

};





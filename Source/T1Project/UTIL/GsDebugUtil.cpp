// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDebugUtil.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

void UGsDebugUtil::DrawDebugSolidTriangle(UWorld* in_world
	, const FVector& in_v1, const FVector& in_v2, const FVector& in_v3
	, const FColor& in_color
	, bool in_persistent, float in_lifeTime
	, uint8 in_depthPriority)
{
	TArray<FVector> vertexArray;

	vertexArray.Add(in_v1);
	vertexArray.Add(in_v2);
	vertexArray.Add(in_v3);

	TArray<int32> indexArray;

	indexArray.Add(0);
	indexArray.Add(1);
	indexArray.Add(2);

	DrawDebugMesh(in_world, vertexArray, indexArray, in_color, in_persistent, in_lifeTime, in_depthPriority);
}

void UGsDebugUtil::DrawDebugSolidCylinder(UWorld* in_world
	, const FVector& in_location
	, float in_radius
	, float in_height
	, int32 in_segment
	, const FColor& in_color
	, bool in_persistent, float in_lifeTime
	, uint8 in_depthPriority)
{
	if (in_segment <= 2)
	{
		return;
	}

	float gap = 360.0f / in_segment;
	float degree = 0;
	FVector location;
	TArray<FVector> vertexArray;
	TArray<FVector> pointArray;

	//bottom point
	for (int32 i = 0; i < in_segment; ++i)
	{
		degree = gap * i;
		location = FVector::ForwardVector.RotateAngleAxis(degree, FVector::UpVector) * in_radius + in_location;
		vertexArray.Add(location);
		pointArray.Add(location);
	}

	//upper point
	for (int32 i = 0; i < in_segment; ++i)
	{
		vertexArray.Add(pointArray[i] + FVector(0, 0, in_height));
	}

	TArray<int32> indexArray;
	int32 indexMax = in_segment - 1;

	for (int32 i = 0; i < in_segment; ++i)
	{
		if (i == indexMax)
		{
			indexArray.Add(i);
			indexArray.Add(i + in_segment);
			indexArray.Add(i + 1);

			indexArray.Add(i);
			indexArray.Add(i + 1);
			indexArray.Add(0);

		}
		else
		{
			indexArray.Add(i);
			indexArray.Add(i + in_segment);
			indexArray.Add(i + in_segment + 1);

			indexArray.Add(i);
			indexArray.Add(i + in_segment + 1);
			indexArray.Add(i + 1);
		}
	}

	DrawDebugMesh(in_world, vertexArray, indexArray, in_color, in_persistent, in_lifeTime, in_depthPriority);
}

void UGsDebugUtil::DrawDebugSolidFence(UWorld* in_world
	, const TArray<FVector>& in_pointArray
	, float in_height
	, bool in_loop
	, const FColor& in_color
	, bool in_persistent, float in_lifeTime
	, uint8 in_depthPriority)
{
	int32 num = in_pointArray.Num();

	TArray<FVector> vertexArray;	

	//bottom point
	for (int32 i = 0; i < num; ++i)
	{		
		vertexArray.Add(in_pointArray[i]);		
	}

	//upper point
	for (int32 i = 0; i < num; ++i)
	{
		vertexArray.Add(in_pointArray[i] + FVector(0, 0, in_height));
	}

	TArray<int32> indexArray;
	int32 indexMax = num - 1;

	for (int32 i = 0; i < num; ++i)
	{
		if (i == indexMax)
		{
			if (in_loop)
			{
				indexArray.Add(i);
				indexArray.Add(i + num);
				indexArray.Add(i + 1);

				indexArray.Add(i);
				indexArray.Add(i + 1);
				indexArray.Add(0);
			}
		}
		else
		{
			indexArray.Add(i);
			indexArray.Add(i + num);
			indexArray.Add(i + num + 1);

			indexArray.Add(i);
			indexArray.Add(i + num + 1);
			indexArray.Add(i + 1);
		}
	}

	DrawDebugMesh(in_world, vertexArray, indexArray, in_color, in_persistent, in_lifeTime, in_depthPriority);
}

void UGsDebugUtil::DrawDebugFan(UWorld* in_world,
	const FVector& in_pos,
	const FVector& in_dir,
	float in_height,
	float in_angle,
	float in_min,
	float in_max,
	const FColor& in_color,
	float in_lifeTime)
{
	if (in_world)
	{
		float halfAngle = in_angle * 0.5f;

		auto hypotenuse1Pos = in_pos;
		auto hypotenuse2Pos = in_pos;
		auto hypotenuse1 = in_pos + in_dir.RotateAngleAxis(halfAngle, FVector::UpVector) * in_max;
		if (in_min > 0.f)
		{
			hypotenuse1Pos = in_pos + in_dir.RotateAngleAxis(halfAngle, FVector::UpVector) * in_min;
		}
		auto hypotenuse2 = in_pos + in_dir.RotateAngleAxis(-halfAngle, FVector::UpVector) * in_max;
		if (in_min > 0.f)
		{
			hypotenuse2Pos = in_pos + in_dir.RotateAngleAxis(-halfAngle, FVector::UpVector) * in_min;
		}

		// 외각 라인 그리기
		DrawDebugLine(in_world, hypotenuse1Pos, hypotenuse1, in_color, false, in_lifeTime);
		DrawDebugLine(in_world, hypotenuse2Pos, hypotenuse2, in_color, false, in_lifeTime);

		int segments = (int)halfAngle * 3;
		float unit = halfAngle / segments;
		for (int i = 0; i < segments; ++i)
		{
			float delta = halfAngle - i * unit;
			auto tempFanl = in_pos + in_dir.RotateAngleAxis(-delta, FVector::UpVector) * in_max;
			auto tempFanr = in_pos + in_dir.RotateAngleAxis(delta, FVector::UpVector) * in_max;

			DrawDebugPoint(in_world, tempFanl, 1.f, FColor::Red, false, in_lifeTime);
			DrawDebugPoint(in_world, tempFanr, 1.f, FColor::Red, false, in_lifeTime);

			if (in_min > 0.f)
			{
				tempFanl = in_pos + in_dir.RotateAngleAxis(-delta, FVector::UpVector) * in_min;
				tempFanr = in_pos + in_dir.RotateAngleAxis(delta, FVector::UpVector) * in_min;

				DrawDebugPoint(in_world, tempFanl, 1.f, FColor::Red, false, in_lifeTime);
				DrawDebugPoint(in_world, tempFanr, 1.f, FColor::Red, false, in_lifeTime);
			}
		}
	}
}
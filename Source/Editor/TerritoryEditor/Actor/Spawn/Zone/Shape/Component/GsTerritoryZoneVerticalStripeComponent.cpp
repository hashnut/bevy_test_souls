// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZoneVerticalStripeComponent.h"

//UE4
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

//Territory editor
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Actor/GsTerritoryShapeInterface.h"


// Sets default values for this component's properties
UGsTerritoryZoneVerticalStripeComponent::UGsTerritoryZoneVerticalStripeComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	CastShadow = false;
	bUseAsyncCooking = true;
}

void UGsTerritoryZoneVerticalStripeComponent::BeginDestroy()
{
	Clear();

	Super::BeginDestroy();
}

void UGsTerritoryZoneVerticalStripeComponent::Draw(
	const TArray<FVector>& in_pointArray, bool in_loop, bool in_twoSide, 
	float in_height, FColor in_outerColor, FColor in_insdieColor
)
{
	Clear();

	//Draw outer
	DrawStripe(in_pointArray, 0, true, in_loop, in_height, in_outerColor);

	if (in_twoSide)
	{
		//Draw inner
		DrawStripe(in_pointArray, 1, false, in_loop, in_height, in_insdieColor);
	}

	//Set material
	//Set Collsion
	SetNoCollision();
}

void UGsTerritoryZoneVerticalStripeComponent::Clear()
{
	int32 num = GetNumSections();
	for (int32 i = 0; i < num; ++i)
	{
		ClearMeshSection(i);
	}

	//ClearAllMeshSections();

	if (_material)
	{
		_material->ConditionalBeginDestroy();
	}
}

void UGsTerritoryZoneVerticalStripeComponent::DrawStripe(
	const TArray<FVector>& in_pointArray, int32 in_section, bool in_clockOrient, 
	bool in_loop, float in_height, FColor in_color
)
{	
	int32 vertexNum = in_pointArray.Num();
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
		vertexs.Add(in_pointArray[i]);
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
		vertexs.Add(in_pointArray[i] + FVector(0, 0, in_height));
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
		if (in_clockOrient)
		{
			if (i == indexMax)
			{
				if (in_loop)
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
				if (in_loop)
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

	CreateMeshSection_LinearColor(in_section, vertexs, triangles, normals, uv, vertexColors, tangents, false);
	SetStripeMaterial(in_color, in_section);
}

void UGsTerritoryZoneVerticalStripeComponent::SetNoCollision()
{
	ContainsPhysicsTriMeshData(false);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel14);
	SetNotifyRigidBodyCollision(false);
	SetGenerateOverlapEvents(false);
	SetCastShadow(true);
}

void UGsTerritoryZoneVerticalStripeComponent::SetStripeMaterial(FColor in_color, int32 in_section)
{
	if (false == _shape.IsValid())
	{
		return;
	}

	IGsTerritoryShapeInterface* shapeInteraface = Cast<IGsTerritoryShapeInterface>(_shape.Get());
	if (nullptr == shapeInteraface)
	{
		return;
	}

	_material = UMaterialInstanceDynamic::Create(shapeInteraface->GetMaterial(), this);
	if (nullptr == _material)
	{
		return;
	}

	_material->SetVectorParameterValue(FName("BaseColor"), FLinearColor(in_color));

	Super::SetMaterial(in_section, _material);
}
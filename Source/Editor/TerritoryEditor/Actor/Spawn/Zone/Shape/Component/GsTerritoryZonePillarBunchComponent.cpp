// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZonePillarBunchComponent.h"

//UE4
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

//T1 Project
#include "T1Project.h"

//Territory editor
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Actor/GsTerritoryShapeInterface.h"
#include "Util/GsTerritoryEditorUtil.h"
#include "Util/GsTerritoryZoneShapeUtil.h"

// Sets default values for this component's properties
UGsTerritoryZonePillarBunchComponent::UGsTerritoryZonePillarBunchComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	CastShadow = false;
	bUseAsyncCooking = true;
}

void UGsTerritoryZonePillarBunchComponent::BeginDestroy()
{
	Clear();

	Super::BeginDestroy();
}

void UGsTerritoryZonePillarBunchComponent::Draw(
	const TArray<FVector>& in_pointArray, bool in_loop, float in_height, FColor in_color
)
{	
	Clear();

	int32 num = in_pointArray.Num();
	for (int32 i = 0; i < num; ++i)
	{
		//Draw outer
		DrawPillar(in_pointArray[i], i, true, in_loop, in_height, in_color);
	}	
	
	//Set Collsion
	SetNoCollision();
}

void UGsTerritoryZonePillarBunchComponent::Clear()
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

void UGsTerritoryZonePillarBunchComponent::DrawPillar(
	const FVector& in_location, int32 in_section, bool in_clockOrient, bool in_loop, float in_height, FColor in_color
)
{
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	FVector v1, v2, v3, v4, v5, v6, v7, v8;
	float factor = 10.0f;

	v1 = (FVector(-1, -1, 0) * factor) + in_location;
	v2 = (FVector(1, -1, 0) * factor) + in_location;
	v3 = (FVector(1, 1, 0) * factor) + in_location;
	v4 = (FVector(-1, 1, 0) * factor) + in_location;

	v5 = (FVector(-1, -1, 0) * factor + FVector(0, 0, in_height)) + in_location;
	v6 = (FVector(1, -1, 0) * factor + FVector(0, 0, in_height)) + in_location;
	v7 = (FVector(1, 1, 0) * factor + FVector(0, 0, in_height)) + in_location;
	v8 = (FVector(-1, 1, 0) * factor + FVector(0, 0, in_height)) + in_location;

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

	CreateMeshSection_LinearColor(in_section, vertexs, triangles, normals, uv, vertexColors, tangents, false);
	SetPillarMaterial(in_color, in_section);
}

void UGsTerritoryZonePillarBunchComponent::SetNoCollision()
{
	ContainsPhysicsTriMeshData(false);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel14);
	SetNotifyRigidBodyCollision(false);
	SetGenerateOverlapEvents(false);
	SetCastShadow(true);
}

void UGsTerritoryZonePillarBunchComponent::SetPillarMaterial(FColor in_color, int32 in_section)
{
	if (false == _shape.IsValid())
	{
		return;
	}

	IGsTerritoryShapeInterface* shapeInteraface = Cast<IGsTerritoryShapeInterface>(_shape);
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
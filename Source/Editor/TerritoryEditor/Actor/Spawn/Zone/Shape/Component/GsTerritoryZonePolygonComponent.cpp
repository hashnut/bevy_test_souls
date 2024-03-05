// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryZonePolygonComponent.h"

//UE4
#include "Runtime/Engine/Classes/Engine/Polys.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"

//Territory editor
#include "Define/GsTerritoryZoneShapeDefine.h"
#include "Actor/Spawn/Zone/Shape/GsTerritoryZoneShape.h"
#include "Actor/GsTerritoryShapeInterface.h"
#include "GsTerritoryLevelManager.h"
#include "GsTerritoryEditorSetting.h"

UGsTerritoryZonePolygonComponent::UGsTerritoryZonePolygonComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	CastShadow = false;
	bUseAsyncCooking = true;
}

void UGsTerritoryZonePolygonComponent::BeginDestroy()
{
	Clear();

	Super::BeginDestroy();
}

void UGsTerritoryZonePolygonComponent::Draw(const FTransform& in_transform
, const FColor& inColor
, const TArray<FVector>& in_pointArray)
{
	_color = inColor;

	Clear();

	//If component have parent, SetWorldLocation will act as relative location	
	SetWorldLocation(in_transform.GetLocation());

	TArray<FPoly> polyArray;
	if (false == TryGetTrianglesFromVertices(in_transform, in_pointArray, polyArray))
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
	float maxRadius = GetMaxRadius(vertexArray, center);
	float uvValue;

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
	 
	CreateMeshSection_LinearColor(0, vertexArray, indexArray, normals, uv, vertexColors, tangents, _isCanCollision);

	if (_isCanCollision)
	{
		SetCollision();
	}
	else
	{
		SetNoCollision();
	}	

	//메테리얼이 없다면 생성
	UMaterialInterface* material = GetMaterial(0);
	UMaterialInstanceDynamic* dyanmicMaterial = nullptr;
	if (nullptr == material)
	{
		UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
		if (settings)
		{
			dyanmicMaterial = CreateDynamicMaterialInstance(0, settings->_transparentMaterial.LoadSynchronous());
			SetMaterial(0, dyanmicMaterial);
		}		
	}

	//그래도 만들어진 메테리얼이 없다면 리턴
	dyanmicMaterial = Cast<UMaterialInstanceDynamic>(GetMaterial(0));
	if (nullptr == dyanmicMaterial)
	{
		return;
	}

	TransparentFloor();			
}

bool UGsTerritoryZonePolygonComponent::TryGetTrianglesFromVertices(const FTransform& in_transform
, const TArray<FVector>& in_pointArray
, TArray<FPoly>& out_polyArray) const
{
	out_polyArray.Empty();
	FPoly poly;
	FVector temp;
	int32 num = in_pointArray.Num();
	for (int i = 0; i < num; i++)
	{
		temp = in_pointArray[i];		
		poly.InsertVertex(i, temp);
	}

	return (poly.Triangulate(GetWorld()->GetDefaultBrush(), out_polyArray) > 0);
}

void UGsTerritoryZonePolygonComponent::TransparentFloor()
{
	UGsTerritoryEditorSetting* settings = GetMutableDefault<UGsTerritoryEditorSetting>();
	if (nullptr == settings)
	{
		return;
	}

	UMaterialInterface* material = GetMaterial(0);
	UMaterialInstanceDynamic* dyanmicMaterial = Cast<UMaterialInstanceDynamic>(material);
	if (nullptr == dyanmicMaterial)
	{
		return;
	}

	FColor floorColor = _color;
	if (0 == floorColor.A)
	{
		floorColor = settings->_zoneFloorColor;		
	}

	if (FGsTerritoryLevelManager::_isTransparentFloor)
	{
		floorColor.A = FGsTerritoryLevelManager::_transparentFloorAlpha;
	}

	dyanmicMaterial->SetVectorParameterValue(TEXT("BaseColor"), floorColor);
}

void UGsTerritoryZonePolygonComponent::SetNoCollision()
{
	ContainsPhysicsTriMeshData(false);
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		
	SetNotifyRigidBodyCollision(false);
	SetGenerateOverlapEvents(false);
	SetCastShadow(true);

	SetCollisionProfileName(TERRITORY_ZONE_BOTTOM_COLLISION_PRESET);
}

void UGsTerritoryZonePolygonComponent::SetCollision()
{
	ContainsPhysicsTriMeshData(false);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		
	SetNotifyRigidBodyCollision(false);
	SetGenerateOverlapEvents(false);
	SetCastShadow(true);

	SetCollisionProfileName(TERRITORY_ZONE_BOTTOM_COLLISION_PRESET);	
}

void UGsTerritoryZonePolygonComponent::Clear()
{
	int32 num = GetNumSections();
	for (int32 i = 0; i < num; ++i)
	{
		ClearMeshSection(i);

	}
	//ClearAllMeshSections();

	CleanUpOverrideMaterials();
	EmptyOverrideMaterials();
}

void UGsTerritoryZonePolygonComponent::GetVertexArray(const TArray<FPoly>& in_polyArray
, TArray<FVector>& out_vertexArray) const
{
	int32 num = in_polyArray.Num();
	TSet<FVector> _PointSet;

	for (int32 i = 0; i < num; ++i)
	{
		for (const FVector& vertextIter : in_polyArray[i].Vertices)
		{
			if (false == _PointSet.Contains(vertextIter))
			{
				_PointSet.Add(vertextIter);
			}
		}
	}

	out_vertexArray.Empty();
	out_vertexArray = _PointSet.Array();	
}

bool UGsTerritoryZonePolygonComponent::TryGetIndexArray(const TArray<FVector>& in_vertexArray
, const TArray<FPoly>& in_polyArray
, TArray<int32>& out_indexArray) const
{
	//Organize unique point	
	int32 index;
	int32 vertextNum;

	for (const FPoly& iterPloy : in_polyArray)
	{		
		vertextNum = iterPloy.Vertices.Num();

		for(int i = vertextNum - 1; i >= 0; --i)
		{
			if (false == in_vertexArray.Find(iterPloy.Vertices[i], index))
			{
				return false;
			}

			out_indexArray.Add(index);
		}
	}
	
	return true;
}

float UGsTerritoryZonePolygonComponent::GetMaxRadius(const TArray<FVector>& in_vertexArray, FVector& out_center) const
{
	int32 num = in_vertexArray.Num();
	FVector accu = FVector::ZeroVector;
	FVector center;

	if (num > 0)
	{
		for (const FVector& iter : in_vertexArray)
		{
			accu += iter;
		}

		center = accu / num;

		float max = LONG_MIN;
		float value;

		for (const FVector& iter : in_vertexArray)
		{
			value = (iter - center).Size();

			if (max < value)
			{
				max = value;
			}
		}

		return max;
	}

	return -1;
}

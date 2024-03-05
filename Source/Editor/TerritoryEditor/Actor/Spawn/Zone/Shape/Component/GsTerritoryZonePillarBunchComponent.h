// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GsTerritoryZonePillarBunchComponent.generated.h"

class AGsTerritoryZoneShape;
class UGsTerritoryPillarComponent;

//----------------------------------------------------------------
// AGsEditorBaseTerritory의 포인트 사이가 연결됐다는 것을 표현하기 위한 평면
//----------------------------------------------------------------
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TERRITORYEDITOR_API UGsTerritoryZonePillarBunchComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<class AActor> _shape;		
	UPROPERTY()
		class UMaterialInstanceDynamic* _material;

public:
	// Sets default values for this component's properties
	UGsTerritoryZonePillarBunchComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginDestroy() override;

public:
	void Draw(const TArray<FVector>& in_pointArray, bool in_loop, float in_height, FColor in_color);
	void Clear();

private:
	void DrawPillar(const FVector& in_location, int32 in_section = 0, bool in_clockOrient = true, bool in_loop = true, float in_height = 100.0f, FColor in_color = FColor::Blue);

private:
	void SetPillarMaterial(FColor in_color, int32 in_section);
	void SetNoCollision();
};

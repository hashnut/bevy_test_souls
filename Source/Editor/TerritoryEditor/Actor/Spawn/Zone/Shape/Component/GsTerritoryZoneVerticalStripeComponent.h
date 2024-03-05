// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GsTerritoryZoneVerticalStripeComponent.generated.h"

class AGsTerritoryZoneShape;
class UMaterialInstanceDynamic;

//----------------------------------------------------------------
// AGsEditorBaseTerritory의 포인트 사이가 연결됐다는 것을 표현하기 위한 평면
//----------------------------------------------------------------
UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TERRITORYEDITOR_API UGsTerritoryZoneVerticalStripeComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	TWeakObjectPtr<class AActor> _shape;
	UPROPERTY()
		UMaterialInstanceDynamic* _material;

public:
	// Sets default values for this component's properties
	UGsTerritoryZoneVerticalStripeComponent(const FObjectInitializer& ObjectInitializer);
	virtual void BeginDestroy() override;

public:
	void Draw(
		const TArray<FVector>& in_pointArray, bool in_loop, bool in_twoSide,
		float in_height, FColor in_outerColor, FColor in_insdieColor
	);
	void Clear();

private:
	void DrawStripe(
		const TArray<FVector>& in_pointArray, int32 in_section = 0, bool in_clockOrient = true,
		bool in_loop = true, float in_height = 100.0f, FColor in_color = FColor::Blue
	);

private:
	void SetStripeMaterial(FColor in_color, int32 in_section);
	void SetNoCollision();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryFenceBase.h"
#include "GsTerritoryFenceSpaceCrack.generated.h"


UCLASS()
class TERRITORYEDITOR_API AGsTerritoryFenceSpaceCrack : public AGsTerritoryFenceBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Territory fence")
		class USplineComponent* _portalSplineComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Territory fence")
		class UChildActorComponent* _portalShapeActorChild;

public:	
	UPROPERTY(EditInstanceOnly)
		EGsEnvEvent _envEvent;
	UPROPERTY(VisibleInstanceOnly)
		float _portalRadius = 500;
	UPROPERTY(VisibleInstanceOnly, Category = "Territory zone")
		TArray<FVector> _portalPoints;

public:
	AGsTerritoryFenceSpaceCrack();

public:
	void InitSpaceCrack(const FText& inName, EGsEnvEvent inEnvEvent);
	virtual TerritoryType GetFenceType() override { return TerritoryType::SPACE_CRACK; }

public:
	virtual void InvalidDestription() override;

public:
	virtual void CreateView(const FTransform& in_transform) override;
	void SetEnvEvent(EGsEnvEvent inEnvEvent);

public:
	void Export(OUT FGsSchemaMapTerritorySpaceCrack& outMapSpaceCrack, OUT FGsSchemaFenceElement& outFenceElement);
	void Import(const FGsSchemaMapTerritorySpaceCrack& inMapSpaceCrack, const FGsSchemaFenceElement& inFenceElement);
		
};

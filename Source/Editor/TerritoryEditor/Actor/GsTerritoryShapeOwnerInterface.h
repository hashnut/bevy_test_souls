// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GsTerritoryShapeOwnerInterface.generated.h"

/**
 * Territory editor에서 child actor 선택을 막기 위한 interface
 */
UINTERFACE(BlueprintType)
class TERRITORYEDITOR_API UGsTerritoryShapeOwnerInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()	

public:
	static void SetSplinePoints(class USplineComponent* inComponent, const TArray<FVector>& inPointArray);
};

class TERRITORYEDITOR_API IGsTerritoryShapeOwnerInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual class UMaterial* GetMaterial(){return nullptr;}
};

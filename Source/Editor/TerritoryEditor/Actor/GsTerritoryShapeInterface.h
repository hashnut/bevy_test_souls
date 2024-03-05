// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GsTerritoryShapeInterface.generated.h"

/**
 * Territory editor에서 child actor 선택을 막기 위한 interface
 */
UINTERFACE(BlueprintType)
class TERRITORYEDITOR_API UGsTerritoryShapeInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()	
};

class TERRITORYEDITOR_API IGsTerritoryShapeInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual class UMaterial* GetMaterial(){return nullptr;}
};

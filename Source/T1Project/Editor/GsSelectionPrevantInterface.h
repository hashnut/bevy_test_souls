// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GsSelectionPrevantInterface.generated.h"

/**
 * Territory editor에서 child actor 선택을 막기 위한 interface
 */
UINTERFACE(BlueprintType)
class T1PROJECT_API UGsSelectionPrevantInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()	
};

class T1PROJECT_API IGsSelectionPrevantInterface
{
	GENERATED_IINTERFACE_BODY()
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/Object.h"
#include "Runtime/Json/Public/Serialization/JsonWriter.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "GsJsonUtil.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsJsonUtil : public UObject
{
	GENERATED_BODY()

public:
	static bool TryGetVectorField(FVector& out_Value, TSharedPtr<FJsonValue> in_jsonValue);
};
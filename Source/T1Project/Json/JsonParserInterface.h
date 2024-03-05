// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/CoreUObject/Public/UObject/Interface.h"
#include "Runtime/CoreUObject/Public/UObject/Object.h"
#include "Runtime/Json/Public/Serialization/JsonWriter.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "./JsonDefine.h"
#include "JsonParserInterface.generated.h"

/**
 * 
 */
class UJsonRoot;

UINTERFACE(BlueprintType)
class T1PROJECT_API UJsonParserInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class T1PROJECT_API IJsonParserInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	virtual TSharedPtr<FJsonRoot> Parse(TSharedPtr<FJsonObject> in_jsonObj) { return nullptr; }
};
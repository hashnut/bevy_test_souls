// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryInputLayoutUtil.generated.h"
/**
 * 
 */
USTRUCT()
struct FGsTerritoryTableKeyInfo
{
	GENERATED_BODY()

public:
	FString desc;
	int32 id;
};

UCLASS()
class TERRITORYEDITOR_API UGsTerritoryInputLayoutUtil : public UObject
{
	GENERATED_BODY()

public:
	static FString GetKey(const FString& inKeyInfo);
};

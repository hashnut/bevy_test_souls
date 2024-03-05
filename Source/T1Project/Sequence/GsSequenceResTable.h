// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GsSequenceResTable.generated.h"

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsSequenceResTable : public UObject
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FGsSequenceResData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
	FString Id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
	FSoftObjectPath Path;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
	TArray<FString> ViewTagList;
};

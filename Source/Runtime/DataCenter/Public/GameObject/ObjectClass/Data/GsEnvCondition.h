// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "GsEnvCondition.generated.h"

USTRUCT(BlueprintType)
struct FGsEnvCondition
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
		int64 _startQuestId;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
		int64 _endQuestId;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
		EGsEnvEvent _envEvent;
};

USTRUCT(BlueprintType)
struct FGsQuestSound
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
		int64 _startQuestId;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
		int64 _endQuestId;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly)
		FGsSchemaSoundResDataRow _sound;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "GsStatDefine.generated.h"

UENUM(BlueprintType)
enum class EPlusStatType : uint8
{
	STR,
	DEX,
	INT,
	CON,
	AGI,
	WIS,
	MAX
};


UCLASS()
class UGsStatUtil : public UObject
{
	GENERATED_BODY()

public:
	static StatType ConvertPlusStatToStat(EPlusStatType inType);
	static EPlusStatType ConvertStatToPlusStat(StatType inType);
};

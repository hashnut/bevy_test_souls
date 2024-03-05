/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpaceCrackConfigDeponMonsterStat.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/SpaceCrack/BSGsSpaceCrackConfigDeponMonsterStat"))
struct DATACENTER_API FGsSchemaSpaceCrackConfigDeponMonsterStat
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="value"))
	int32 value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="typeList"))
	TArray<FGsStatTypeNameSelector> typeList;
public:
	FGsSchemaSpaceCrackConfigDeponMonsterStat();
	
protected:
	bool operator== (const FGsSchemaSpaceCrackConfigDeponMonsterStat& __Other) const
	{
		return FGsSchemaSpaceCrackConfigDeponMonsterStat::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

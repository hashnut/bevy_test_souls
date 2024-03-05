/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/SpaceCrack/GsSchemaSpaceCrackConfigDeponMonster.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/SpaceCrack/GsSchemaSpaceCrackConfigDeponMonsterStat.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpaceCrackConfigDeponMonsterGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/SpaceCrack/BSGsSpaceCrackConfigDeponMonsterGroup"))
struct DATACENTER_API FGsSchemaSpaceCrackConfigDeponMonsterGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityId"))
	FGsSchemaAbnormalitySetRow abnormalityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="stat"))
	FGsSchemaSpaceCrackConfigDeponMonsterStat stat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="deponMonsterList"))
	TArray<FGsSchemaSpaceCrackConfigDeponMonster> deponMonsterList;
public:
	FGsSchemaSpaceCrackConfigDeponMonsterGroup();
	
protected:
	bool operator== (const FGsSchemaSpaceCrackConfigDeponMonsterGroup& __Other) const
	{
		return FGsSchemaSpaceCrackConfigDeponMonsterGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

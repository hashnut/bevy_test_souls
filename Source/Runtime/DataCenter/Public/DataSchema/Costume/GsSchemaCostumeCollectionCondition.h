/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeCollectionCondition.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeCollectionCondition"))
struct DATACENTER_API FGsSchemaCostumeCollectionCondition
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionId", Tooltip))
	int32 conditionId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeId", Tooltip))
	int32 costumeId;
	
public:
	FGsSchemaCostumeCollectionCondition();
	
protected:
	bool operator== (const FGsSchemaCostumeCollectionCondition& __Other) const
	{
		return FGsSchemaCostumeCollectionCondition::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

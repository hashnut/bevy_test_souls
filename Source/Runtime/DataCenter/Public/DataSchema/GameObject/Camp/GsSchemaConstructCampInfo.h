/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaConstructCampInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Camp/BSGsConstructCampInfo"))
struct DATACENTER_API FGsSchemaConstructCampInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="interactionRange"))
	int32 interactionRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureCommonInfoId"))
	FGsSchemaCreatureCommonInfoRow creatureCommonInfoId;
public:
	FGsSchemaConstructCampInfo();
	
protected:
	bool operator== (const FGsSchemaConstructCampInfo& __Other) const
	{
		return FGsSchemaConstructCampInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

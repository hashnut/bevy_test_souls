/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UGsGameObjectMaterialData;
#include "GsSchemaAbnormalityMaterialInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/BSGsAbnormalityMaterialInfo"))
struct DATACENTER_API FGsSchemaAbnormalityMaterialInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="objectData"))
	UGsGameObjectMaterialData* objectData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="priority"))
	int32 priority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
public:
	FGsSchemaAbnormalityMaterialInfo();
	
protected:
	bool operator== (const FGsSchemaAbnormalityMaterialInfo& __Other) const
	{
		return FGsSchemaAbnormalityMaterialInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

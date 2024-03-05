/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EGsPartsType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPartsShapeInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/BSGsPartsShapeInfo"))
struct DATACENTER_API FGsSchemaPartsShapeInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="shapePath"))
	FSoftObjectPath shapePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="subShapePath"))
	FSoftObjectPath subShapePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hiddenParts"))
	TArray<EGsPartsType> hiddenParts;
public:
	FGsSchemaPartsShapeInfo();
	
protected:
	bool operator== (const FGsSchemaPartsShapeInfo& __Other) const
	{
		return FGsSchemaPartsShapeInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

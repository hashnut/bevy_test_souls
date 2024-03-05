/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/GameObject/GsSchemaPartsShapeInfo.h"
#include "GsSchemaCostumePath.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumePath"))
struct DATACENTER_API FGsSchemaCostumePath
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="pathMale"))
	FGsSchemaPartsShapeInfo pathMale;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="pathFemale"))
	FGsSchemaPartsShapeInfo pathFemale;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="customizeColorPalette 사용"))
	int32 colorIndex;
	
public:
	FGsSchemaCostumePath();
	
protected:
	bool operator== (const FGsSchemaCostumePath& __Other) const
	{
		return FGsSchemaCostumePath::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

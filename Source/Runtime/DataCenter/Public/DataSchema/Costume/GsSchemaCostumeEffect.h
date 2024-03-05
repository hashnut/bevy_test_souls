/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeEffect.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeEffect"))
struct DATACENTER_API FGsSchemaCostumeEffect
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="path"))
	FSoftObjectPath path;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="socketName"))
	FName socketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="socketTransform"))
	FTransform socketTransform;
public:
	FGsSchemaCostumeEffect();
	
protected:
	bool operator== (const FGsSchemaCostumeEffect& __Other) const
	{
		return FGsSchemaCostumeEffect::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

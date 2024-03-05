/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBossTimeInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsBossTimeInfo"))
struct DATACENTER_API FGsSchemaBossTimeInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mon"))
	TArray<int32> mon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tue"))
	TArray<int32> tue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wed"))
	TArray<int32> wed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="thu"))
	TArray<int32> thu;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fri"))
	TArray<int32> fri;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sat"))
	TArray<int32> sat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sun"))
	TArray<int32> sun;
public:
	FGsSchemaBossTimeInfo();
	
protected:
	bool operator== (const FGsSchemaBossTimeInfo& __Other) const
	{
		return FGsSchemaBossTimeInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

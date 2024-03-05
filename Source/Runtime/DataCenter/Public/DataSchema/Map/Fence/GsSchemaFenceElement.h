/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFenceElement.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Fence/BSGsFenceElement"))
struct DATACENTER_API FGsSchemaFenceElement
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="FenceId", Tooltip))
	int64 FenceId;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Center;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Radius;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="vertexList", Tooltip))
	TArray<FVector> vertexList;
	
public:
	FGsSchemaFenceElement();
	
protected:
	bool operator== (const FGsSchemaFenceElement& __Other) const
	{
		return FGsSchemaFenceElement::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};
